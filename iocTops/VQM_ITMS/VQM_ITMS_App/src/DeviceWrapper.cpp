#include "DeviceWrapper.h"

#include <asynOctetSyncIO.h>

#include <sstream>

std::string ToString(double value)
{
	std::ostringstream o;
	o << value;
	return o.str ();
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
	asynStatus status = write(IOUser, "INST FIL;SOUR CURR?");
	return status;
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

asynStatus DeviceWrapper::GetLogicalInstrumentCurrentVoltage(double& value, EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser) const
{
	asynStatus status = write(IOUser, EnumToText(logicalInstrumentEnum) + ";SOUR:VOLT?");
	return status;
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

asynStatus DeviceWrapper::GetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser) const
{
	return write(IOUser, "CONF:AMU?");
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
		status = write (IOUser, "FORM:ALL 0,0");
		if (status == asynSuccess)
			status = write (IOUser, "SOUR:MODE NORMAL");
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
	if (status == asynSuccess)
		status = write (IOUser, "SOUR:MODE?");
	if (status == asynSuccess)
		status = write (IOUser, "FORM:ALL?");
	if (status == asynSuccess)
		status = write (IOUser, "OUTP?");
	return status;
}

asynStatus DeviceWrapper::write(asynUser* IOUser, std::string const& WritePacket) const
{
	size_t nBytesOut;
#ifdef _DEBUG
	// Infinite timeout, convenient for debugging.
	const double TimeOut = -1;
#else
	const double TimeOut = 5;
#endif
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	asynStatus status = pasynOctetSyncIO->write(IOUser,
		WritePacket.c_str(), WritePacket.size(), 
		TimeOut, &nBytesOut);
	return status;
}

asynStatus DeviceWrapper::read(asynUser* IOUser)
{
	size_t nBytesIn = 0;
	int eomReason;
	char Buf[1024];
#ifdef _DEBUG
	// Infinite timeout, convenient for debugging.
	const double TimeOut = -1;
#else
	const double TimeOut = 5;
#endif
	asynStatus status = pasynOctetSyncIO->read(IOUser, Buf, sizeof(Buf), TimeOut, &nBytesIn, &eomReason);
	for (size_t Byte = 0; Byte < nBytesIn; Byte++)
		m_ReadBuffer.push(Buf[Byte]);
	return status;
}
