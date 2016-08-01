#include <asynOctetSyncIO.h>
#include <asynOctet.h>

#include <epicsGuard.h>

#include <sstream>

#define epicsExportSharedSymbols
#include "IServiceWrapper.h"
#include "../../Exception.h"

#ifdef _DEBUG
// Infinite timeout, convenient for debugging.
const double TimeOut = -1;
#else
const double TimeOut = 5;
#endif

std::string ToString(double value)
{
	std::ostringstream o;
	o << value;
	return o.str ();
}

void FromString(std::string const& String, int& value)
{
	value = atoi(String.c_str());
}

void FromString(std::string const& String, double& value)
{
	value = atof(String.c_str());
}

SVQM_800_Error IServiceWrapper::ConnectToDevice(asynUser* IOUser, bool isMaster)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	ThrowException(IOUser, pasynOctetSyncIO->flush(IOUser), __FUNCTION__, "flush");
	ThrowException(IOUser, pasynOctetSyncIO->setOutputEos(IOUser, "\r", 1), __FUNCTION__, "setOutputEos");
	ThrowException(IOUser, pasynOctetSyncIO->setInputEos(IOUser, "\r", 1), __FUNCTION__, "setInputEos");
	write(IOUser, "INST MSP");
	std::string ErrResponse;
	writeRead(IOUser, "SYST:ERR:ALL?", ErrResponse);
	std::string ReportResponse;
	writeRead(IOUser, "TEST:REPORT?", ReportResponse);
	epicsTime Time = epicsTime::getCurrent();
	char Date[11], TimeOfDay[9]; 
    Time.strftime ( Date, sizeof(Date), "%Y,%m,%d");
    Time.strftime ( TimeOfDay, sizeof(TimeOfDay), "%H,%M,%S");

	write(IOUser, "SYST:DATE " + std::string(Date));
	write(IOUser, "SYST:TIME " + std::string(TimeOfDay));
	write(IOUser, "STAT:OPER:CONT:NTR 32767");
	GetGaugeState(m_GaugeState, IOUser);
	SetGaugeState(EnumGaugeState_OFF, IOUser);
	write(IOUser, "INST FIL");
	write(IOUser, "SOUR:MODE ADJ");
	writeRead(IOUser, "SYST:ERR:ALL?", ErrResponse);
	GetLogicalInstrumentMinMaxVoltage(FILAMENTBIAS, IOUser);
	GetLogicalInstrumentMinMaxVoltage(REPELLERBIAS, IOUser);
	GetLogicalInstrumentMinMaxVoltage(ENTRYPLATE, IOUser);
	GetLogicalInstrumentMinMaxVoltage(PRESSUREPLATE, IOUser);
	GetLogicalInstrumentMinMaxVoltage(CUPS, IOUser);
	GetLogicalInstrumentMinMaxVoltage(TRANSITION, IOUser);
	GetLogicalInstrumentMinMaxVoltage(EXITPLATE, IOUser);
	GetLogicalInstrumentMinMaxVoltage(EMSHIELD, IOUser);
	GetLogicalInstrumentMinMaxVoltage(EMBIAS, IOUser);
	GetLogicalInstrumentMinMaxVoltage(RFAMP, IOUser);
	std::string AMUResponse;
	writeRead(IOUser, "CONF:AMU?", AMUResponse);
	// +9.28392E-01,+1.47596E+02
	size_t CommaPos = FindMarkerPos(AMUResponse, 0, ",");
	std::string lowerRange = AMUResponse.substr(0, CommaPos-1);
	FromString(lowerRange, m_lowerRange);
	std::string upperRange = AMUResponse.substr(CommaPos+1);
	FromString(upperRange, m_upperRange);
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::DataAnalysisSetAvgMode(EnumAvgMode mode, asynUser* IOUser)
{
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::DataAnalysisSetNumAvgs(int numAverages, asynUser* IOUser)
{
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetFilamentEmissionCurrent(double& value, asynUser* IOUser) const
{
	value = m_EmissionCurrent;
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::SetFilamentEmissionCurrentSetpoint(double& value, asynUser* IOUser)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	write(IOUser, "INST FIL");
	write(IOUser, "SOUR CURR " + ToString(value));
	return SVQM_800_Error();
}


std::string IServiceWrapper::EnumToText(EnumLogicalInstruments logicalInstrumentEnum) const
{
	switch(logicalInstrumentEnum)
	{
		case FILAMENT : // duplicate?
		case FILAMENTBIAS : return "INST FIL"; break;
		case REPELLERBIAS : return "INST REP"; break;
		case ENTRYPLATE : return "INST ENTR"; break;
		case PRESSUREPLATE : return "INST PPL"; break;
		case CUPS : return "INST CUPS"; break;
		case TRANSITION : return "INST TPL"; break;
		case EXITPLATE : return "INST EXIT"; break;
		case EMSHIELD : return "INST EMSH"; break;
		case EMBIAS : return "INST EMUL"; break;
		case RFAMP : return "INST DDS"; break;
		default : _ASSERT(false);
	}
	return "";
}

SVQM_800_Error IServiceWrapper::GetLogicalInstrumentMinMaxVoltage(EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	double Min, Max;
	write(IOUser, EnumToText(logicalInstrumentEnum));
	std::string MaxResponse, MinResponse;
	writeRead(IOUser, "SOUR:VOLT? MAX", MaxResponse);
	FromString(MaxResponse, Max);
	writeRead(IOUser, "SOUR:VOLT? MIN", MinResponse);
	FromString(MinResponse, Min);
	m_InstrumentVoltages[logicalInstrumentEnum].m_Min = Min;
	m_InstrumentVoltages[logicalInstrumentEnum].m_Max = Max;
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetLogicalInstrumentCurrentVoltage(double& value, EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser) const
{
	std::map<EnumLogicalInstruments, InstrumentVoltage>::const_iterator Iter = m_InstrumentVoltages.find(logicalInstrumentEnum);
	if (Iter == m_InstrumentVoltages.end())
		return SVQM_800_Error(NO_ANALYZED_DATA, L"Instrument not found", L"GetLogicalInstrumentCurrentVoltage");
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::SetLogicalInstrumentVoltageSetpoint(EnumLogicalInstruments logicalInstrumentEnum, double& value, asynUser* IOUser)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	write(IOUser, EnumToText(logicalInstrumentEnum));
	write(IOUser, "SOUR:VOLT");
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetMassCalibrationFactor(float& value, asynUser* IOUser) const
{
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetElectrometerGain(double& value, asynUser* IOUser) const
{
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser)
{
	lowerRange = m_lowerRange;
	upperRange = m_upperRange;
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::SetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	write(IOUser, "CONF:AMU " + ToString(lowerRange) + "," + ToString(upperRange));
	return SVQM_800_Error();
}

//	asynStatus GetScanData(int& lastScanNumber, SAnalyzedData& analyzedData, IHeaderData** headerDataPtr,
//                                            SAverageData& averageData, const SDeviceConnectionInfo& connectInfo, bool& isValidData, EnumGaugeState& controllerState) = 0;
SVQM_800_Error IServiceWrapper::SetGaugeState(EnumGaugeState gaugeState, asynUser* IOUser)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	write(IOUser, "INST MSP");
	switch (gaugeState)
	{
	case EnumGaugeState_OFF :
		write (IOUser, "OUTP OFF");
		break;
	case EnumGaugeState_STANDBY :
		write (IOUser, "SOUR:MODE STANDBY");
		write (IOUser, "OUTP ON");
		break;
	case EnumGaugeState_ON :
		write (IOUser, "OUTP ON");
		break;
	case EnumGaugeState_SCAN :
		write (IOUser, "OUTP ON");
		write (IOUser, "FORM:ALL 1,0");
		write (IOUser, "INIT:CONT ON");
		break;
	default: _ASSERT(false);
	}
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetGaugeState(EnumGaugeState& gaugeState, asynUser* IOUser)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	write(IOUser, "INST MSP");
	std::string OUTPResponse, INITResponse;
	int OUTPValue = 0, INITValue;
	writeRead(IOUser, "OUTP?", OUTPResponse);
	FromString(OUTPResponse, OUTPValue);
	writeRead(IOUser, "INIT:CONT?", INITResponse);
	FromString(INITResponse, INITValue);
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetTSETingsValues(std::string const&  TSETingsValues)
{
	// enum EnumLogicalInstruments { FILAMENT = 0, FILAMENTBIAS, REPELLERBIAS, ENTRYPLATE, PRESSUREPLATE, CUPS, TRANSITION, EXITPLATE, EMSHIELD, EMBIAS, RFAMP };

	//								  RepellerBias  Emission    FilamentBias   No idea     EntryPlate   PressurePlate Cups        Transition   ExitPlate     RFAmp       EMShield     EMBias      no idea      no idea
	// (MEASurement=TSETtings (VALues -5.29913E+01,+7.50000E-05,+3.09385E+01,+2.83090E+00,+1.30018E+02,+7.50945E+01,+2.69785E+01,-6.85262E+02,+1.21151E+02,+4.50000E-01,+1.23000E+02,-8.58475E+02,+1.90069E-08,+9.99999E-10))
	size_t FirstCommaPos = 0, SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos, ",");
	std::string RepellerBias = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(RepellerBias, m_InstrumentVoltages[REPELLERBIAS].m_Current);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string EmissionCurrent = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(EmissionCurrent, m_EmissionCurrent);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string FilamentBias = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(FilamentBias, m_InstrumentVoltages[FILAMENTBIAS].m_Current);

	// No idea (+2.83090E+00)
	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string EntryPlate = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(EntryPlate, m_InstrumentVoltages[ENTRYPLATE].m_Current);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string PressurePlate = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(PressurePlate, m_InstrumentVoltages[PRESSUREPLATE].m_Current);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string Cups = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(Cups, m_InstrumentVoltages[CUPS].m_Current);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string Transition = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(Transition, m_InstrumentVoltages[TRANSITION].m_Current);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string ExitPlate = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(Transition, m_InstrumentVoltages[EXITPLATE].m_Current);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string RFAmp = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(RFAmp, m_InstrumentVoltages[RFAMP].m_Current);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string EMShield = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(EMShield, m_InstrumentVoltages[EMSHIELD].m_Current);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string EMBias = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(EMBias, m_InstrumentVoltages[EMBIAS].m_Current);
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetScanData(int& lastScanNumber, SAnalyzedData& analyzedData, IHeaderData** headerDataPtr,
                               SAverageData& averageData, asynUser* IOUser, bool& isValidData, EnumGaugeState& controllerState)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	std::vector<char> ReadPacket;
	std::string HeaderData;
	write(IOUser, "fetc?");
	ThrowException(IOUser, pasynOctetSyncIO->setInputEos(IOUser, "", 0), __FUNCTION__, "setInputEos");
	readTill(IOUser, HeaderData, "   ");

	size_t IDPos = FindMarkerPos(HeaderData, 0, "(DIF (VERSion 1999.0) IDENtify", "(ID \"");
	size_t QuotePos = FindMarkerPos(HeaderData, IDPos, "\"");
	m_HeaderData.m_MID = mbstowcs(HeaderData.substr(IDPos, QuotePos-IDPos));

	size_t DESignPos = FindMarkerPos(HeaderData, QuotePos, "DESign \"");

	// (DIF (VERSion 1999.0) IDENtify (DATE 2016,07,25 TIME 13,55,09.737 UUT (ID "835A0405"  DESign "0A,03.001.01301"))
	size_t CommaPos = FindMarkerPos(HeaderData, DESignPos, ",");
	QuotePos = FindMarkerPos(HeaderData, CommaPos, "\"");
	m_HeaderData.m_HardwareRevision = mbstowcs(HeaderData.substr(DESignPos, CommaPos-DESignPos-1));
	m_HeaderData.m_FirmwareRevision = mbstowcs(HeaderData.substr(CommaPos, QuotePos-CommaPos-1));

	// enum EnumLogicalInstruments { FILAMENT = 0, FILAMENTBIAS, REPELLERBIAS, ENTRYPLATE, PRESSUREPLATE, CUPS, TRANSITION, EXITPLATE, EMSHIELD, EMBIAS, RFAMP };
	//								  RepellerBias  Emission    FilamentBias   No idea     EntryPlate   PressurePlate Cups        Transition   ExitPlate     RFAmp       EMShield     no idea     no idea      no idea
	// (MEASurement=TSETtings (VALues -5.29913E+01,+7.50000E-05,+3.09385E+01,+2.83090E+00,+1.30018E+02,+7.50945E+01,+2.69785E+01,-6.85262E+02,+1.21151E+02,+4.50000E-01,+1.23000E+02,-8.58475E+02,+1.90069E-08,+9.99999E-10))
	size_t VALuesPos = FindMarkerPos(HeaderData, QuotePos, "DATA (MEASurement=TSETtings (VALues ");
	size_t CloseBracketPos = FindMarkerPos(HeaderData, VALuesPos, "))");
	std::string TSETingsValues = HeaderData.substr(VALuesPos, CloseBracketPos-VALuesPos-2);
	SVQM_800_Error Error = GetTSETingsValues(TSETingsValues);

	VALuesPos = FindMarkerPos(HeaderData, CloseBracketPos, "CURVe (", "VALues #");
	int VALues = 0;
	FromString(HeaderData.substr(VALuesPos), VALues);

	ReadPacket.resize(VALues*4);
	read(IOUser, ReadPacket);
	pasynOctetSyncIO->setInputEos(IOUser, "\r", 1);
	*headerDataPtr = &m_HeaderData;
	return Error;
}

size_t IServiceWrapper::FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* FirstMarker)
{
	size_t FirstMarkerPos = HeaderData.find(FirstMarker, Offset);
	if (FirstMarkerPos != std::string::npos)
		FirstMarkerPos += strlen(FirstMarker);
	return FirstMarkerPos;
}

size_t IServiceWrapper::FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* FirstMarker, const char* SecondMarker)
{
	size_t FirstMarkerPos = FindMarkerPos(HeaderData, Offset, FirstMarker);
	size_t SecondMarkerPos = std::string::npos;
	if (FirstMarkerPos != std::string::npos)
	{
		FirstMarkerPos += strlen(FirstMarker);
		SecondMarkerPos = HeaderData.find(SecondMarker, FirstMarkerPos);
		if (SecondMarkerPos != std::string::npos)
			SecondMarkerPos += strlen(SecondMarker);
	}
	return SecondMarkerPos;
}

void IServiceWrapper::write(asynUser* IOUser, std::string const& WritePacket) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	size_t nBytesOut;
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	ThrowException(IOUser, pasynOctetSyncIO->write(IOUser, WritePacket.c_str(), WritePacket.size(), TimeOut, &nBytesOut), __FUNCTION__, "write");
}

void IServiceWrapper::writeRead(asynUser* IOUser, std::string const& WritePacket, std::string& ReadPacket) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	size_t nBytesIn;
	int eomReason = ASYN_EOM_CNT;
	char Buf[1024];
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	write(IOUser, WritePacket);
	ReadPacket.clear();
	while (eomReason == ASYN_EOM_CNT)
	{
		ThrowException(IOUser, pasynOctetSyncIO->read(IOUser, Buf, sizeof(Buf), TimeOut, &nBytesIn, &eomReason), __FUNCTION__, "read");
		for(size_t Byte = 0; Byte < nBytesIn; Byte++)
			ReadPacket.push_back(Buf[Byte]);
	}
}

void IServiceWrapper::readTill(asynUser* IOUser, std::string& ReadPacket, std::string const& Termination) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	size_t nBytesIn;
	int eomReason = ASYN_EOM_CNT;
	char Buf;
	ReadPacket.clear();
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	while ((ReadPacket.size() < Termination.size()) ||
		   (ReadPacket.substr(ReadPacket.size()-Termination.size(), Termination.size()) != Termination))
	{
		ThrowException(IOUser, pasynOctetSyncIO->read(IOUser, &Buf, sizeof(Buf), TimeOut, &nBytesIn, &eomReason), __FUNCTION__, "read");
		ReadPacket += Buf;
	}
}

void IServiceWrapper::read(asynUser* IOUser, std::vector<char>& ReadPacket) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	size_t nBytesIn;
	int eomReason = ASYN_EOM_CNT;
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	ThrowException(IOUser, pasynOctetSyncIO->read(IOUser, &(ReadPacket[0]), ReadPacket.size(), TimeOut, &nBytesIn, &eomReason), __FUNCTION__, "read");
	_ASSERT(eomReason == ASYN_EOM_CNT);
#ifdef _DEBUG
	size_t ExtraData = 0;
	char Buf;
	do
	{
		asynStatus Status = pasynOctetSyncIO->read(IOUser, &Buf, sizeof(Buf), 0.01, &nBytesIn, &eomReason);
		ExtraData += nBytesIn;
	}
	while (eomReason == ASYN_EOM_CNT);
	_ASSERT(ExtraData == 0);
#endif*/
}

IServiceWrapper::CException::CException(asynUser* AsynUser, asynStatus Status, const char* functionName, std::string const& what) :
	std::runtime_error(what) 
{
	std::string message = "%s:%s ERROR: " + what + "%s\n";
	asynPrint(AsynUser, ASYN_TRACE_ERROR, message.c_str(), __FILE__, functionName, AsynUser->errorMessage);
	m_Status = Status;
}

void IServiceWrapper::ThrowException(asynUser* pasynUser, asynStatus Status, const char* Function, std::string const& what) const
{
	if (Status == asynSuccess)
		return;
	throw CException(pasynUser, Status, Function, what);
}

