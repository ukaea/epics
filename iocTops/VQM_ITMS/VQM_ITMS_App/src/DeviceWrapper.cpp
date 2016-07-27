#include "DeviceWrapper.h"

#include <asynOctetSyncIO.h>
#include <asynOctet.h>

#include <epicsGuard.h>

#include <sstream>

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

DeviceWrapper::DeviceWrapper(asynUser* IOUser)
{
	pasynOctetSyncIO->flush(IOUser);
	pasynOctetSyncIO->setOutputEos(IOUser, "\r", 1);
	pasynOctetSyncIO->setInputEos(IOUser, "\r", 1);
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
	GetLogicalInstrumentMinMaxVoltage(m_Filament.m_Min, m_Filament.m_Max, FILAMENTBIAS, IOUser);
	GetLogicalInstrumentMinMaxVoltage(m_Repeller.m_Min, m_Repeller.m_Max, REPELLERBIAS, IOUser);
	GetLogicalInstrumentMinMaxVoltage(m_EntryPlate.m_Min, m_EntryPlate.m_Max, ENTRYPLATE, IOUser);
	GetLogicalInstrumentMinMaxVoltage(m_PressurePlate.m_Min, m_PressurePlate.m_Max, PRESSUREPLATE, IOUser);
	GetLogicalInstrumentMinMaxVoltage(m_Cups.m_Min, m_Cups.m_Max, CUPS, IOUser);
	GetLogicalInstrumentMinMaxVoltage(m_Transition.m_Min, m_Transition.m_Max, TRANSITION, IOUser);
	GetLogicalInstrumentMinMaxVoltage(m_ExitPlate.m_Min, m_ExitPlate.m_Max, EXITPLATE, IOUser);
	GetLogicalInstrumentMinMaxVoltage(m_EMShield.m_Min, m_EMShield.m_Max, EMSHIELD, IOUser);
	GetLogicalInstrumentMinMaxVoltage(m_EMBias.m_Min, m_EMBias.m_Max, EMBIAS, IOUser);
	GetLogicalInstrumentMinMaxVoltage(m_RFAmp.m_Min, m_RFAmp.m_Max, RFAMP, IOUser);
}

asynStatus DeviceWrapper::DataAnalysisSetAvgMode(EnumAvgMode mode, asynUser* IOUser)
{
	return asynSuccess;
}

asynStatus DeviceWrapper::DataAnalysisSetNumAvgs(int numAverages, asynUser* IOUser)
{
	return asynSuccess;
}

asynStatus DeviceWrapper::GetFilamentEmissionCurrent(double& value, asynUser* IOUser) const
{
	return asynSuccess;
}

asynStatus DeviceWrapper::SetFilamentEmissionCurrentSetpoint(double& value, asynUser* IOUser)
{
	asynStatus status = write(IOUser, "INST FIL");
	if (status == asynSuccess)
		status = write(IOUser, "SOUR CURR " + ToString(value));
	return status;
}


std::string DeviceWrapper::EnumToText(EnumLogicalInstruments logicalInstrumentEnum) const
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

asynStatus DeviceWrapper::GetLogicalInstrumentMinMaxVoltage(double& min, double& max, EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser) const
{
	asynStatus status = write(IOUser, EnumToText(logicalInstrumentEnum));
	std::string MaxResponse, MinResponse;
	if (status == asynSuccess)
		status = writeRead(IOUser, "SOUR:VOLT? MAX", MaxResponse);
	FromString(MaxResponse, max);
	if (status == asynSuccess)
		status = writeRead(IOUser, "SOUR:VOLT? MIN", MinResponse);
	FromString(MinResponse, min);
	return status;
}

asynStatus DeviceWrapper::GetLogicalInstrumentCurrentVoltage(double& value, EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser) const
{
//	asynStatus status = write(IOUser, EnumToText(logicalInstrumentEnum) + ";SOUR:VOLT?");
	return asynSuccess;
}

asynStatus DeviceWrapper::SetLogicalInstrumentVoltageSetpoint(EnumLogicalInstruments logicalInstrumentEnum, double& value, asynUser* IOUser)
{
	asynStatus status = write(IOUser, EnumToText(logicalInstrumentEnum));
	if (status != asynSuccess)
		return status;
	status = write(IOUser, "SOUR:VOLT");
	return status;
}

asynStatus DeviceWrapper::GetMassCalibrationFactor(float& value, asynUser* IOUser) const
{
	return asynSuccess;
}

asynStatus DeviceWrapper::GetElectrometerGain(double& value, asynUser* IOUser) const
{
	return asynSuccess;
}

asynStatus DeviceWrapper::GetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser)
{
	std::string AMUResponse;
	asynStatus status = writeRead(IOUser, "CONF:AMU?", AMUResponse);
	return status;
}

asynStatus DeviceWrapper::SetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser)
{
	return write(IOUser, "CONF:AMU " + ToString(lowerRange) + "," + ToString(upperRange));
}

//	asynStatus GetScanData(int& lastScanNumber, SAnalyzedData& analyzedData, IHeaderData** headerDataPtr,
//                                            SAverageData& averageData, const SDeviceConnectionInfo& connectInfo, bool& isValidData, EnumGaugeState& controllerState) = 0;
asynStatus DeviceWrapper::SetGaugeState(EnumGaugeState gaugeState, asynUser* IOUser)
{
	asynStatus status = write(IOUser, "INST MSP");
	switch (gaugeState)
	{
	case EnumGaugeState_OFF :
		if (status == asynSuccess)
			status = write (IOUser, "OUTP OFF");
		break;
	case EnumGaugeState_STANDBY :
		status = write (IOUser, "SOUR:MODE STANDBY");
		if (status == asynSuccess)
			status = write (IOUser, "OUTP ON");
		break;
	case EnumGaugeState_ON :
		status = write (IOUser, "OUTP ON");
		break;
	case EnumGaugeState_SCAN :
		status = write (IOUser, "OUTP ON");
		if (status == asynSuccess)
			status = write (IOUser, "FORM:ALL 1,0");
		if (status == asynSuccess)
			status = write (IOUser, "INIT:CONT ON");
		break;
	default: _ASSERT(false);
	}
	return status;
}

asynStatus DeviceWrapper::GetGaugeState(EnumGaugeState& gaugeState, asynUser* IOUser)
{
	asynStatus status = write(IOUser, "INST MSP");
	std::string OUTPResponse, INITResponse;
	int OUTPValue = 0, INITValue;
	if (status == asynSuccess)
		status = writeRead(IOUser, "OUTP?", OUTPResponse);
	FromString(OUTPResponse, OUTPValue);
	if (status == asynSuccess)
		status = writeRead(IOUser, "INIT:CONT?", INITResponse);
	FromString(INITResponse, INITValue);
	return status;
}

asynStatus DeviceWrapper::GetScanData(int& lastScanNumber, IHeaderData** headerDataPtr, asynUser* IOUser, bool& isValidData, EnumGaugeState& controllerState)
{
	std::vector<char> ReadPacket;
	std::string HeaderData;
	asynStatus status = write(IOUser, "fetc?");
	if (status == asynSuccess)
		status = pasynOctetSyncIO->setInputEos(IOUser, "", 0);
	if (status == asynSuccess)
		status = readTill(IOUser, HeaderData, "   ");

	size_t VALuesPos = FindMarkerPos(HeaderData, 0, "CURVe (", "VALues #");
	int VALues = 0;
	if (VALuesPos  == std::string::npos)
	{
		if (status == asynSuccess)
			status = asynError;
	}
	else
		FromString(HeaderData.substr(VALuesPos), VALues);

	ReadPacket.resize(VALues);
	if (status == asynSuccess)
		status = read(IOUser, ReadPacket);
	pasynOctetSyncIO->setInputEos(IOUser, "\r", 1);
	_ASSERT(status == asynSuccess);
	return status;
}

size_t DeviceWrapper::FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* FirstMarker, const char* SecondMarker)
{
	size_t FirstMarkerPos = HeaderData.find(FirstMarker, Offset);
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

asynStatus DeviceWrapper::write(asynUser* IOUser, std::string const& WritePacket) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	size_t nBytesOut;
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	asynStatus status = pasynOctetSyncIO->write(IOUser,
		WritePacket.c_str(), WritePacket.size(), 
		TimeOut, &nBytesOut);
	return status;
}

asynStatus DeviceWrapper::writeRead(asynUser* IOUser, std::string const& WritePacket, std::string& ReadPacket) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	size_t nBytesIn;
	int eomReason = ASYN_EOM_CNT;
	char Buf[1024];
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	asynStatus Status = write(IOUser, WritePacket);
	ReadPacket.clear();
	while ((Status == asynSuccess) && (eomReason == ASYN_EOM_CNT))
	{
		Status = pasynOctetSyncIO->read(IOUser, Buf, sizeof(Buf), TimeOut, &nBytesIn, &eomReason);
		for(size_t Byte = 0; Byte < nBytesIn; Byte++)
			ReadPacket.push_back(Buf[Byte]);
	}
	return Status;
}

asynStatus DeviceWrapper::readTill(asynUser* IOUser, std::string& ReadPacket, std::string const& Termination) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	size_t nBytesIn;
	int eomReason = ASYN_EOM_CNT;
	char Buf;
	ReadPacket.clear();
	asynStatus Status = asynSuccess;
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	while ((Status == asynSuccess) && 
		   ((ReadPacket.size() < Termination.size()) ||
		    (ReadPacket.substr(ReadPacket.size()-Termination.size(), Termination.size()) != Termination)))
	{
		Status = pasynOctetSyncIO->read(IOUser, &Buf, sizeof(Buf), TimeOut, &nBytesIn, &eomReason);
		_ASSERT(Status == asynSuccess);
		ReadPacket += Buf;
	}
	return Status;
}

asynStatus DeviceWrapper::read(asynUser* IOUser, std::vector<char>& ReadPacket) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	size_t nBytesIn;
	int eomReason = ASYN_EOM_CNT;
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	asynStatus Status = pasynOctetSyncIO->read(IOUser, &(ReadPacket[0]), ReadPacket.size(), TimeOut, &nBytesIn, &eomReason);
#ifdef _DEBUG
	size_t ExtraData = 0;
	char Buf;
	while ((Status == asynSuccess) && (eomReason == ASYN_EOM_CNT))
	{
		asynStatus Status = pasynOctetSyncIO->read(IOUser, &Buf, sizeof(Buf), 0.01, &nBytesIn, &eomReason);
		ExtraData += nBytesIn;
	}
	_ASSERT(nBytesIn == 0);
	_ASSERT(Status == asynSuccess);
#endif
	return Status;
}
