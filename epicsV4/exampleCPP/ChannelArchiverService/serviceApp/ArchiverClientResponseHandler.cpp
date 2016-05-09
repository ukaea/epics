// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution


#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <alarm.h>
#include <alarmString.h>
#include <ArrayTools.h>

#include <epicsGetopt.h>
#include <epicsStdlib.h>
#include <epicsTime.h>

#include <RawValue.h>

#include <pv/pvData.h>


#include "common.h"
#include "ArchiverClientResponseHandler.h"

namespace epics
{

namespace channelArchiverService
{

/**
 * Converts an epics alarm status and severity to a string, including
 * archiver special severities.
 *
 * @param  status     alarm status
 * @param  severity   alarm severity
 * @return            the result of the string conversion
 */
std::string makeAlarmString(short status, short severity)
{
    std::string result;
    char buf[200];

    severity &= 0xfff;
    switch (severity)
    {
    case NO_ALARM:
        result = "NO ALARM";
        return result;
    //  Archiver specials:
    case ARCH_EST_REPEAT:
        sprintf(buf, "Est_Repeat %d", static_cast<int>(status));
        result = buf;
        return result;
    case ARCH_REPEAT:
        sprintf(buf, "Repeat %d", static_cast<int>(status));
        result = buf;
        return result;
    case ARCH_DISCONNECT:
        result = "Disconnected";
        return result;
    case ARCH_STOPPED:
        result = "Archive_Off";
        return result;
    case ARCH_DISABLED:
        result = "Archive_Disabled";
        return result;
    }

    if (severity < static_cast<short>(SIZEOF_ARRAY(alarmSeverityString))  &&
        status < static_cast<short>(SIZEOF_ARRAY(alarmStatusString)))
    {
        result = alarmSeverityString[severity];
        result += " ";
        result += alarmStatusString[status];
    }
    else
    {
        sprintf(buf, "%d %d", severity, status);
        result = buf;
    }
    return result;
}


/**
 * Converts an epicsTime to a date string
 *
 * @param  t  the time to convert. 
 * @return The date string.
 */
std::string getDate(const epicsTime & t)
{
    char buf[1024];
    buf[0] = '\0';
    buf[sizeof(buf)-1] = '\0';
    t.strftime(buf, sizeof(buf)-1, "%c");
    return buf;
}

/**
 * Converts secs past epoch and nsecs to a date string
 *
 * @param  secsPastEpoch seconds past EPICS epoch. 
 * @param  nsecs         nanoseconds after second.
 * @return The date string.
 */
std::string getDate(int64_t secsPastEpoch, int32_t nsecs)
{
    epicsTimeStamp ts = { static_cast<epicsUInt32>(secsPastEpoch), static_cast<epicsUInt32>(nsecs)};
    epicsTime t(ts);
    return getDate(t);
}

/**
 * Converts data container to strings according to format parameters 
 * and adds to vector of strings 
 *
 * @param  strings    Array of strings to add to. 
 * @param  data       The data container to be added.
 * @param  format     Format used to convert data to string. 
 * @param  precision  Precision used in formatting when converting data to string. 
 */
template <typename A>
void arrayValuesToStrings(std::vector<std::string> & strings, const A & data,
                        const FormatParameters::Format format = FormatParameters::DEFAULT, int precision = 6)
{
    using namespace std;

    strings.reserve(strings.size() + data.size());
    ostringstream oss;

    switch(format)
    {
    case FormatParameters::SCIENTIFIC:
        oss << showpoint << scientific << setprecision(precision);
        break;

    case FormatParameters::FIXED_POINT:
        oss << showpoint << fixed << setprecision(precision);        
        break;

    case FormatParameters::DEFAULT:
        oss << showpoint  << setprecision(precision);
        break;

    case FormatParameters::HEX:
        oss << hex;
        break;
    }
    for (typename A::const_iterator it = data.begin(); it != data.end(); ++it)
    {
        oss << *it;
        strings.push_back(oss.str());
        oss.str("");    
    }
}

template <typename VT, typename CT>
bool isPresent(const VT & value, CT & container)
{
    return (std::find(container.begin(), container.end(), value) != container.end());
}


void RequestResponseHandler::handle(epics::pvData::PVStructurePtr const & response)
{
    makeStrings(response);
    outputResults();
}


void RequestResponseHandler::makeStrings(epics::pvData::PVStructurePtr const & response)
{
    using namespace epics::pvData;
    using namespace std;

    PVStructurePtr responseValues = response->getSubField<PVStructure>("value");
    if (!responseValues)
    {
        cerr << "Data invalid: No value field in table." << endl;
        m_ok = false;  
        return; 
    }

    //  Handle each of the fields in the archiver query response in turn.

    //  Values.
    PVDoubleArrayPtr values = getDoubleArrayField(responseValues, "value");
    if (!values)
    {
        cerr << "Data invalid: No value field in table values." << endl;
        m_ok = false;  
        return;
    }    
    int valuesLength = values->getLength();

    if (isPresent(VALUE, m_parameters.outputtedFields))
    {

        arrayValuesToStrings(outputFieldValues[VALUE], values->view(),
            m_parameters.format, m_parameters.precision);
    }


    //  Seconds.
    PVLongArrayPtr secPastEpochs = getLongArrayField(responseValues, "secondsPastEpoch");
    if (!secPastEpochs)
    {
        cerr << "Data invalid: No secondsPastEpoch field in table values." << endl;
        m_ok = false;  
        return;
    }

    int secPastEpochsLength = secPastEpochs->getLength();
    if (secPastEpochsLength != valuesLength)
    {
        cerr << "Data invalid: secondsPastEpoch and value lengths don't match." << endl;
        m_ok = false;  
        return; 
    }

    PVLongArray::const_svector secPastEpochsData = secPastEpochs->view();

    if (isPresent(SECONDS_PAST_EPOCH, m_parameters.outputtedFields)
     || isPresent(REAL_TIME, m_parameters.outputtedFields))
    {
        arrayValuesToStrings(outputFieldValues[SECONDS_PAST_EPOCH], secPastEpochsData);
    }


    //  Nanoseconds.
    PVIntArrayPtr nsecs = getIntArrayField(responseValues, "nanoseconds");
    if (!nsecs)
    {
        cerr << "Data invalid: No nanoseconds field in table values." << endl;
        m_ok = false;  
        return;
    }

    int nsecsLength =  nsecs->getLength();
    if (nsecsLength != valuesLength)
    {
        cerr << "Data invalid: nanoseconds past epoch and value lengths don't match." << endl;
        m_ok = false;  
        return;  
    }

    PVIntArray::const_svector nsecsData = nsecs->view();

    if (isPresent(NANO_SECONDS, m_parameters.outputtedFields)
     || isPresent(REAL_TIME, m_parameters.outputtedFields))
    {
        arrayValuesToStrings(outputFieldValues[NANO_SECONDS], nsecsData);
    }


    //  Real time in seconds.
    if (isPresent(REAL_TIME, m_parameters.outputtedFields))
    {
        int realTimeLength = min(secPastEpochsLength, nsecsLength);
        vector<string> & realTimeStrings = outputFieldValues[REAL_TIME];
        realTimeStrings.reserve(realTimeLength);

        {
            ostringstream oss;
            for (int i = 0; i < realTimeLength; ++i)
            {
                oss << secPastEpochsData[i]  << ".";
                oss << setfill('0') << setw(9) << nsecsData[i];
                realTimeStrings.push_back(oss.str());
                oss.str("");
           }
        }
    }


    //  Dates.
    if (isPresent(DATE, m_parameters.outputtedFields))
    {
        vector<string> & dateStrings = outputFieldValues[DATE];
        int dateLength = min(secPastEpochsLength, nsecsLength);
        dateStrings.reserve(dateLength);

        for (int i = 0; i < dateLength; ++i)
        {     
            string dateString = getDate(secPastEpochsData[i], nsecsData[i]);
            dateStrings.push_back(dateString);
        }
    }


    //  Alarm status.
    PVIntArrayPtr statuses = getIntArrayField(responseValues, "status");
    if (!statuses)
    {
        cerr << "Data invalid: No status field in table values." << endl;
        m_ok = false;  
        return;
    }

    int statusesLength = statuses->getLength();
    if (statusesLength != valuesLength)
    {
        cerr << "Data invalid: status and value lengths don't match." << endl;
        m_ok = false;  
        return; 
    }
    if (isPresent(STATUS, m_parameters.outputtedFields))
    {
        PVIntArray::const_svector statusData = statuses->view();
        arrayValuesToStrings(outputFieldValues[STATUS], statusData, FormatParameters::HEX);
    }


    //  Alarm severity.
    PVIntArrayPtr severities = getIntArrayField(responseValues, "severity");
    if (!severities)
    {
        cerr << "Data invalid: No severity field in table values." << endl;
        m_ok = false;  
        return;
    }

    int severitiesLength = severities->getLength();
    if (severitiesLength != valuesLength)
    {
        cerr << "Data invalid: severity and value lengths don't match." << endl;
        m_ok = false;  
        return; 
    }
    if (isPresent(SEVERITY, m_parameters.outputtedFields))
    {        
        PVIntArray::const_svector severityData = severities->view();
        arrayValuesToStrings(outputFieldValues[SEVERITY], severityData, FormatParameters::HEX);
    }


    //  Alarm string.
    int alarmStringsLength = min(secPastEpochsLength, nsecsLength);
    
    if (isPresent(ALARM, m_parameters.outputtedFields))
    {
        vector<string> & alarmStrings = outputFieldValues[ALARM];        
        alarmStrings.reserve(alarmStringsLength); 

        PVIntArray::const_svector statusData = statuses->view();
        PVIntArray::const_svector severityData = severities->view();

        for (int i = 0; i < valuesLength; ++i)
        {     
            string alarmString = makeAlarmString(statusData[i], severityData[i]);
            alarmStrings.push_back(alarmString);
        }
    }
}


void RequestResponseHandler::outputResults()
{
    using namespace std;

    if (!m_ok)
    {
        throw logic_error("attempted to output invalid data");
    }

    //  Now output archive data.
    bool outputToFile = m_parameters.filename.compare(string(""));
    ofstream outfile;

    if (outputToFile)
    {
        ios_base::openmode openMode = m_parameters.appendToFile ? (ios_base::out | ios_base::app) : ios_base::out;
        outfile.open(m_parameters.filename.c_str(), openMode);
    }

    ostream & out = outputToFile ? outfile : cout; 

    //  Print title.
    bool printTitle = m_parameters.title.compare(string(""));
    if (printTitle)
    {
        out << m_parameters.prefix << m_parameters.title << endl;
    }

    string columnSpace = "  ";


    //  Output archive data values. 
    typedef ios_base & (*align_t)(ios_base &);
    const align_t alignments[] = {
        right,
        right,
        left,
        left,
        right,
        right, 
        right,
        right
    }; 

    size_t valuesLength = 0;
    if (m_parameters.outputtedFields.size() > 0)
    {
        valuesLength = outputFieldValues[m_parameters.outputtedFields[0]].size();
    }

    vector<string> columnTitles;
    columnTitles.resize(m_parameters.outputtedFields.size());

    if (m_parameters.printColumnTitles)
    {
        for (size_t i = 0; i < m_parameters.outputtedFields.size(); ++i)
        {
            OutputField field = m_parameters.outputtedFields[i];
            string columnTitle = m_parameters.prefix;
            columnTitle += outputFieldNames[field];
            columnTitles[i] = columnTitle;
        }
    }

    if (!m_parameters.transpose)
    {
        size_t maxWidths[NUMBER_OF_FIELDS];
        for (int i = 0; i < NUMBER_OF_FIELDS; ++i)
        {
            maxWidths[i] = maxWidth(outputFieldValues[i]);
        }

        if (m_parameters.outputtedFields.size() > 0)
        {
            if (m_parameters.printColumnTitles)
            {
                for (size_t i = 0; i < m_parameters.outputtedFields.size(); ++i)
                {
                    OutputField field = m_parameters.outputtedFields[i];
                    const string & columnTitle = columnTitles[i];
                    maxWidths[field] = max(maxWidths[field], columnTitle.length());
                    out << setw(maxWidths[field]) << left << columnTitle << columnSpace; 
                }
                out << "\n";
            }
        
            for (size_t j = 0; j < valuesLength; ++j) 
            {
                for (size_t i = 0; i < m_parameters.outputtedFields.size(); ++i)
                {
                    OutputField field = m_parameters.outputtedFields[i];

                    out << setw(maxWidths[field])      << alignments[field]
                        << outputFieldValues[field][j]   << columnSpace;   
                }
                out << "\n";
            }
        }
    }
    else
    {
        size_t titleWidth = 0;
        if (m_parameters.printColumnTitles)
        {  
            titleWidth = maxWidth(columnTitles);
        }

        vector<size_t> maxWidths;
        maxWidths.resize(valuesLength, 0);

        for (size_t i = 0; i < m_parameters.outputtedFields.size(); ++i)
        {
            OutputField field = m_parameters.outputtedFields[i];

            for (size_t j = 0; j < valuesLength; ++j) 
            {
                const string & fieldValue = outputFieldValues[field][j];
                size_t fieldLength = fieldValue.length();
                maxWidths[j] = max(maxWidths[j], fieldLength);   
            }
        }

        for (size_t i = 0; i < m_parameters.outputtedFields.size(); ++i)
        {
            OutputField field = m_parameters.outputtedFields[i];

            if (m_parameters.printColumnTitles)
            {                
                out << setw(titleWidth) << left << columnTitles[i] << columnSpace; 
            }

            for (size_t j = 0; j < valuesLength; ++j) 
            {
                out << setw(maxWidths[j])  << alignments[field]
                    << outputFieldValues[field][j]          << columnSpace;    
            }
            out << "\n";
        }

    }

    out.flush();


    if (outputToFile)
    {
        outfile.close();
    }
    m_ok = true;  
}

}

}
