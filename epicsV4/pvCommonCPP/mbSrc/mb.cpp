// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

#if defined(_WIN32) && !defined(NOMINMAX)
#define NOMINMAX
#endif

#include <time.h>
#include <map>
#include <math.h>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <iostream>
#include <sstream>

#include <iomanip>

#include <osiUnistd.h>
#include <epicsStdio.h>
#include <epicsTypes.h>

#define epicsExportSharedSymbols
#include "mb.h"

#ifdef WITH_MICROBENCH

// TODO clean this up
#if defined(__APPLE__)
#include <mach/mach_time.h>
epicsUInt64 MBTime()
{
    return mach_absolute_time();
}
#elif defined(vxWorks) && (CPU_FAMILY == PPC)
IMPORT void vxTimeBaseGet (UINT32 * pTbu, UINT32 * pTbl);
epicsUInt64 MBTime()
{
    union {
        UINT32 u32[2];
        epicsUInt64 u64;
    } t;
    vxTimeBaseGet(&t.u32[0], &t.u32[1]);
    // FIXME: The PPC timebase doesn't actually increment every 1ns.
    // There is a machine-specific scaling factor to apply...
    return t.u64;
}
#elif defined(_WIN32)
#include <windows.h>
static epicsUInt64 PerformanceFrequency;
epicsUInt64 MBTime()
{
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    epicsUInt64 PerfTicks = static_cast<epicsUInt64>(count.QuadPart) * 1000000000;
    return PerfTicks / PerformanceFrequency;
}
#else
#include <sys/time.h>
epicsUInt64 MBTime()
{
#ifdef CLOCK_REALTIME
     struct timespec ts;
     clock_gettime(CLOCK_REALTIME, &ts);
     return static_cast<epicsUInt64>(ts.tv_sec) * 1000000000 + static_cast<epicsUInt64>(ts.tv_nsec);
#else
     struct timeval tv;
     gettimeofday(&tv, NULL);
     return static_cast<epicsUInt64>(tv.tv_sec) * 1000000000 + static_cast<epicsUInt64>(tv.tv_usec) * 1000;
#endif
}
#endif

#ifdef vxWorks
#include <taskLib.h>
#define GETPID() taskIdSelf()
#elif defined(_WIN32)
#define GETPID() GetCurrentProcessId()
#else
#define GETPID() getpid()
#endif

void MBPointAdd(MBEntity &e, std::ptrdiff_t id, epicsUInt8 stage)
{
    // no copy and no MBPoint init solution
    const std::size_t ix = e.pos.fetch_add(1);
    MBPoint& p = e.points[ix];
    p.id = id; p.stage = stage;
    p.time = MBTime();
}

// NOTE: requires that samples are ordered by iteration (not by stage!) first
void MBCSVExport(MBEntity &e, epicsUInt8 stageOnly, std::size_t skipFirstNSamples, std::ostream &o)
{
    std::size_t iterationCount = 0;
    const std::size_t len = e.pos.load();
    if (stageOnly == 0)
    {
        // export all
        for (std::size_t i = 0; i < len; i++)
        {
            MBPoint& p = e.points[i];
            o << p.id << ',' << static_cast<epicsUInt32>(p.stage) << ',' << static_cast<epicsInt64>(p.time) << std::endl;
        }
    }
    else
    {
        for (std::size_t i = 0; i < len; i++)
        {
            MBPoint& p = e.points[i];

            // new first stage means new iteration
            if (p.stage == 0)
                iterationCount++;

            // select stage
            if (stageOnly != p.stage)
                continue;

            // skip fist N iterations
            // NOTE: we assume stages (iterations) are ordered
            if (iterationCount <= skipFirstNSamples)
                continue;

            o << p.id << ',' << static_cast<epicsUInt32>(p.stage) << ',' << static_cast<epicsInt64>(p.time) << std::endl;
        }
    }
}

// NOTE: this method is not thread-safe
void MBCSVImport(MBEntity &e, std::istream &i)
{
    std::string line;
    char c;    

    e.points.resize(0);
    size_t lc = 0;
    while (getline(i,line))
    {
        lc++;
        std::istringstream is(line);
        MBPoint p;
        is >> p.id >> c;
        epicsUInt32 s; is >> s >> c; p.stage = s;
        is >> p.time;
        
        if (is.good() || is.eof())        
            e.points.push_back(p);
        else
        {
            std::cerr << "failed to parse line " << lc << ": \"" << line << "\"" << std::endl;
        }
    }
    e.pos = e.points.size();
}

void MBNormalize(MBEntity &e)
{
    std::map<std::ptrdiff_t, epicsUInt64> lastTime;
    
    const std::size_t len = e.pos.load(); 
    for (std::size_t i = 0; i < len; i++)
    {
        MBPoint& p = e.points[i];
        if (p.stage == 0)
            lastTime[p.id] = p.time;
            
        std::map<std::ptrdiff_t, epicsUInt64>::iterator last = lastTime.find(p.id);
        if (last == lastTime.end())
        {
            std::cerr << "no 0 stage for " << e.name << ", id = " << p.id << std::endl;
            p.time = -1;    // TODO report error?!
        }
        else
        {
            epicsUInt64 lt = last->second;
            last->second = p.time;
            p.time -= lt;
        }
    }
}

struct MBStatistics
{
    std::size_t count;
    epicsUInt64 min;
    epicsUInt64 max;
    double sum;

    double mean;
    double stdev;
    
    MBStatistics() :
        count(0),
        min(~static_cast<epicsUInt64>(0)),
        max(0),
        sum(0.0),
        mean(0.0),
        stdev(0.0)
    {}

    MBStatistics(epicsUInt64 sample) :
        count(1),
        min(sample),
        max(sample),
        sum(sample),
        mean(0.0),
        stdev(0.0)
    {
    }

    void addSample(epicsUInt64 sample)
    {
        count++;
        if (sample < min) min = sample;
        if (sample > max) max = sample;
        sum += sample;
    }

    void pass1Epilogue()
    {
        mean = sum/(double)count;
    }

    void addSamplePass2(epicsUInt64 sample)
    {
        double diff = sample - mean;
        stdev += diff*diff;
    }

    void pass2Epilogue()
    {
        stdev = sqrt(stdev/(double)count);
    }

};

typedef std::map<epicsUInt8, MBStatistics> StatsMapPerStage;

// NOTE: requires that samples are ordered by iteration (not by stage!) first, i.e.
// fist all the stages of iteration 0, then iteration 1, etc.
void MBStats(MBEntity &e, epicsUInt8 stageOnly, std::size_t skipFirstNSamples, std::ostream &o)
{
    MBNormalize(e);
    
    MBStatistics overallStats;
    epicsUInt64 iterationSum = 0;

    StatsMapPerStage stats;
    std::size_t iterationCount = 0;

    // pass 1
    const std::size_t len = e.pos.load(); 
    for (std::size_t i = 0; i < len; i++)
    {
        MBPoint& p = e.points[i];
        
        // first stage is start time, skip
        if (p.stage == 0)
        {
            // new first stage means new iteration
            iterationCount++;

            if (iterationCount > 1)
            {
                overallStats.addSample(iterationSum);
                iterationSum = 0;
            }
            continue;
        }
        // process only one stage, if stageOnly is not 0
        else if (stageOnly != 0 && stageOnly != p.stage)
            continue;

        // skip fist N iterations
        // NOTE: we assume stages (iterations) are ordered
        if (iterationCount <= skipFirstNSamples)
            continue;

        iterationSum += p.time;

        StatsMapPerStage::iterator s = stats.find(p.stage);
        if (s == stats.end())
            stats[p.stage] = MBStatistics(p.time);
        else
            s->second.addSample(p.time);
    }
    
    for (StatsMapPerStage::iterator i = stats.begin();
         i != stats.end();
         i++)
        i->second.pass1Epilogue();


    overallStats.addSample(iterationSum);
    overallStats.pass1Epilogue();
    iterationSum = 0;

    bool dumpOverallTimesOnly = (stageOnly > stats.size());
    if (dumpOverallTimesOnly)
        stageOnly = 0;

    iterationCount = 0;

    // pass 2
    for (std::size_t i = 0; i < len; i++)
    {
        MBPoint& p = e.points[i];

        // first stage is start time, skip
        if (p.stage == 0)
        {
            // new first stage means new iteration
            iterationCount++;

            if (iterationCount > 1)
            {
                if (dumpOverallTimesOnly)
                    std::cout << iterationSum << std::endl;

                overallStats.addSamplePass2(iterationSum);
                iterationSum = 0;
            }
            continue;
        }
        // process only one stage, if stageOnly is not 0
        else if (stageOnly != 0 && stageOnly != p.stage)
            continue;

        // skip fist N iterations
        // NOTE: we assume stages (iterations) are ordered
        if (iterationCount <= skipFirstNSamples)
            continue;

        iterationSum += p.time;

        stats[p.stage].addSamplePass2(p.time);
    }

    for (StatsMapPerStage::iterator i = stats.begin();
         i != stats.end();
         i++)
        i->second.pass2Epilogue();

    overallStats.addSamplePass2(iterationSum);
    overallStats.pass2Epilogue();

    if (dumpOverallTimesOnly)
    {
        std::cout << iterationSum << std::endl;
        return;
    }

    o << std::endl;

    for (StatsMapPerStage::iterator i = stats.begin();
         i != stats.end();
         i++)
    {
        o << "stage " << std::setw(3) << static_cast<epicsUInt32>(i->first)
                      << ": min = " << std::setw(9) << i->second.min
                      << ", max = " << std::setw(9) << i->second.max
                      << ", mean = " << std::setw(9) << static_cast<epicsUInt64>(i->second.mean)
                      << ", stdev = " << std::setw(9) << static_cast<epicsUInt64>(i->second.stdev) << std::endl;
    }
    
    o << std::string(80,'-') << std::endl;
    
    o << "iteration"
                  << ": min = " << std::setw(9) << overallStats.min
                  << ", max = " << std::setw(9) << overallStats.max
                  << ", mean = " << std::setw(9) << static_cast<epicsUInt64>(overallStats.mean)
                  << ", stdev = " << std::setw(9) << static_cast<epicsUInt64>(overallStats.stdev) << std::endl;
    double ips = (1000000000L/overallStats.mean);
    double sdips = ips - (1000000000L/(overallStats.mean+overallStats.stdev));
    o << std::endl << std::setprecision(9) << ips << " +/- " << sdips << " iteration(s)/sec" << std::endl << std::endl;
}

MBEntity::~MBEntity()
{
    // skip if empty
    if (this->pos)
    {
        char fileName[1024];
        char* path = getenv("MB_OUTPUT_DIR");
        if (path == 0) path = const_cast<char*>(".");
        epicsSnprintf(fileName, sizeof(fileName), "%s/mb_%s_%d.csv",
            path, this->name, GETPID());
        std::ofstream out(fileName);
        if (out.is_open())
        {
            MBCSVExport(*this, 0, 0, out);
            out.close();
        }
        else
        {
            std::cerr << "Failed to create file " << fileName
                << ", skipping..." << std::endl;
        }
    }
}

#endif
// Need to export something here for Windows DLL builds

void MBInit()
{
#ifdef WITH_MICROBENCH
#ifdef _WIN32
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    PerformanceFrequency = freq.QuadPart;
#endif
#endif
}

