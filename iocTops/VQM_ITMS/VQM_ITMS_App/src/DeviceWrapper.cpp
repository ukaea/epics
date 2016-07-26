#include "DeviceWrapper.h"

#include <asynOctetSyncIO.h>

asynStatus DeviceWrapper::DataAnalysisSetAvgMode(EnumAvgMode mode, asynUser* IOUser)
{
}

asynStatus DeviceWrapper::DataAnalysisSetNumAvgs(int numAverages, asynUser* IOUser)
{
}

asynStatus DeviceWrapper::GetFilamentEmissionCurrent(double& value, asynUser* IOUser) const
{
	
}

asynStatus DeviceWrapper::SetFilamentEmissionCurrentSetpoint(double& value, asynUser* IOUser)
{
}


std::string DeviceWrapper::EnumToText(EnumLogicalInstruments logicalInstrumentEnum) const
{
	switch(logicalInstrumentEnum)
	{
		case FILAMENT : return "INST FIL"; break;
		case FILAMENTBIAS : write("INST FIL"); break;
		case REPELLERBIAS : write("INST ENTR"); break;
		case ENTRYPLATE : write("INST FIL"); break;
		case PRESSUREPLATE : return "INST PPL"; break;
		case CUPS : return "INST CUPS"; break;
		case TRANSITION : return "INST TPL"; break;
		case EXITPLATE : write("INST EXIT"); break;
		case EMSHIELD : write("INST EMSH"); break;
		case EMBIAS : write("INST EMUL"); break;
		case RFAMP : write("INST TPL"); break;
		default : _ASSERT(false);
	}
	// DDS, MSP?
}

asynStatus DeviceWrapper::GetLogicalInstrumentCurrentVoltage(double& value, EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser) const
{

	}
	write(
}

asynStatus DeviceWrapper::SetLogicalInstrumentVoltageSetpoint(EnumLogicalInstruments logicalInstrumentEnum, double& value, asynUser* IOUser)
{
}

asynStatus DeviceWrapper::GetMassCalibrationFactor(float& value, asynUser* IOUser) const
{
}

asynStatus DeviceWrapper::GetElectrometerGain(double& value, asynUser* IOUser) const
{
}

asynStatus DeviceWrapper::GetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser) const
{
}

asynStatus DeviceWrapper::SetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser)
{
}

//	asynStatus GetScanData(int& lastScanNumber, SAnalyzedData& analyzedData, IHeaderData** headerDataPtr,
//                                            SAverageData& averageData, const SDeviceConnectionInfo& connectInfo, bool& isValidData, EnumGaugeState& controllerState) = 0;
asynStatus DeviceWrapper::SetGaugeState(EnumGaugeState gaugeState, asynUser* IOUser)
{
}

asynStatus DeviceWrapper::GetGaugeState(EnumGaugeState& gaugeState, asynUser* IOUser)
{
}

asynStatus DeviceWrapper::write(asynUser* IOUser, std::string const& WritePacket)
{
	size_t nBytesOut;
#ifdef _DEBUG
	// Infinite timeout, convenient for debugging.
	const double TimeOut = -1;
#else
	const double TimeOut = 5;
#endif
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	asynStatus Status = pasynOctetSyncIO->write(IOUser,
		WritePacket.c_str(), WritePacket.size(), 
		TimeOut, &nBytesOut);
	return Status;
}

asynStatus DeviceWrapper::writeRead(asynUser* IOUser, std::string const& WritePacket)
{
	int eomReason;
	size_t nBytesOut, nBytesIn;
#ifdef _DEBUG
	// Infinite timeout, convenient for debugging.
	const double TimeOut = -1;
#else
	const double TimeOut = 5;
#endif
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	asynStatus Status = pasynOctetSyncIO->writeRead(IOUser,
		WritePacket.c_str(), WritePacket.size(), 
		&(m_ReadBuffer[0]), m_ReadBuffer.size(),
		TimeOut, &nBytesOut, &nBytesIn, &eomReason);

	return Status;
}
