#include <asynDriver.h>
#include <asynOctet.h>
#include <asynOctetSyncIO.h>

#include <epicsTypes.h>
#include <epicsGuard.h>

#define epicsExportSharedSymbols
#include "IServiceWrapper.h"
#include "../../Exception.h"

#include "../../VQM/Include/SAnalyzedData.h"

#include "FIR-filter-class/filt.h"

GaugeDAQ& IServiceWrapper::getGaugeDAQ(asynUser* IOUser)
{
	std::map<asynUser*, GaugeDAQ*>::iterator Iter = m_GaugeDAQs.find(IOUser);
	return *(Iter->second);
}

GaugeDAQ const& IServiceWrapper::getGaugeDAQ(asynUser* IOUser) const
{
	std::map<asynUser*, GaugeDAQ*>::const_iterator Iter = m_GaugeDAQs.find(IOUser);
	return *(Iter->second);
}

IServiceWrapper::IServiceWrapper()
{
}

IServiceWrapper::~IServiceWrapper()
{
	for(std::map<asynUser*, GaugeDAQ*>::iterator Iter = m_GaugeDAQs.begin(); Iter != m_GaugeDAQs.end(); Iter++)
		delete Iter->second;
}

SVQM_800_Error IServiceWrapper::ConnectToDevice(asynUser* IOUser, bool isMaster)
{
	if (m_GaugeDAQs[IOUser] == NULL)
		m_GaugeDAQs[IOUser] = new GaugeDAQ(IOUser);

	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetExtendedRangeCapabilities(bool& isExtendedRange, asynUser* IOUser) const
{
	isExtendedRange = getGaugeDAQ(IOUser).GetExtendedRangeCapabilities();
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::DataAnalysisSetAvgMode(EnumAvgMode mode, asynUser* IOUser)
{
	getGaugeDAQ(IOUser).DataAnalysisSetAvgMode(mode);
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::DataAnalysisSetNumAvgs(int numAverages, asynUser* IOUser)
{
	getGaugeDAQ(IOUser).DataAnalysisSetNumAvgs(numAverages); 
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::SetFilamentEmissionCurrentSetpoint(double value, asynUser* IOUser)
{
	getGaugeDAQ(IOUser).SetFilamentEmissionCurrentSetpoint(value);
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::SetElectrometerGainSetpoint(double value, asynUser* IOUser)
{
	getGaugeDAQ(IOUser).SetElectrometerGainSetpoint(value);
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetLogicalInstrumentMinMaxVoltage(EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser)
{
	GaugeDAQ& GaugeDAQ = getGaugeDAQ(IOUser);
	GaugeDAQ.GetLogicalInstrumentMinMaxVoltage(logicalInstrumentEnum);
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::SetLogicalInstrumentVoltageSetpoint(EnumLogicalInstruments logicalInstrumentEnum, double& value, asynUser* IOUser)
{
	getGaugeDAQ(IOUser).SetLogicalInstrumentVoltageSetpoint(logicalInstrumentEnum, value);
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser) const
{
	getGaugeDAQ(IOUser).GetScanRange(lowerRange, upperRange);
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::SetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser)
{
	getGaugeDAQ(IOUser).SetScanRange(lowerRange, upperRange);
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::SetGaugeState(EnumGaugeState gaugeState, asynUser* IOUser)
{
	getGaugeDAQ(IOUser).SetGaugeState(gaugeState);
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetGaugeState(EnumGaugeState& gaugeState, asynUser* IOUser) const
{
	gaugeState = getGaugeDAQ(IOUser).GetGaugeState();
	return SVQM_800_Error();
}

SVQM_800_Error IServiceWrapper::GetScanData(int& lastScanNumber, SAnalyzedData& analyzedData, IHeaderData** headerDataPtr,
                               SAverageData& averageData, asynUser* IOUser, bool& isValidData, EnumGaugeState& controllerState)
{
	GaugeDAQ& GaugeDAQ = getGaugeDAQ(IOUser);

	analyzedData.DenoisedRawData().assign(GaugeDAQ.RawDataSize(), 0);

	float AverageNoise = GaugeDAQ.AverageNoise();
	if (GaugeDAQ.AvgMode() == Off)
	{
		GaugeDAQ.GrabScanData();
		for(size_t Sample = 0; Sample < analyzedData.DenoisedRawData().size(); Sample++)
			analyzedData.DenoisedRawData()[Sample] = GaugeDAQ.RawData(Sample) - AverageNoise;
	}
	else if (GaugeDAQ.AvgMode() == Running_Avg)
	{
		for(size_t Sample = 0; Sample < analyzedData.DenoisedRawData().size(); Sample++)
			analyzedData.DenoisedRawData()[Sample] = 0;
		std::deque<std::vector<epicsUInt16> > const& RawData = GaugeDAQ.RawData();

		for (size_t ScanNo = 0; ScanNo < RawData.size(); ScanNo++)
		{
			for(size_t Sample = 0; Sample < analyzedData.DenoisedRawData().size(); Sample++)
				analyzedData.DenoisedRawData()[Sample] += RawData[ScanNo][Sample] - AverageNoise;
		}
	}
	else if (GaugeDAQ.AvgMode() == Accumulator)
	{
		for(int ScanNum = 0; ScanNum < GaugeDAQ.numAverages(); ScanNum++)
		{
			GaugeDAQ.GrabScanData();
			for(size_t Sample = 0; Sample < analyzedData.DenoisedRawData().size(); Sample++)
				analyzedData.DenoisedRawData()[Sample] += GaugeDAQ.RawData(Sample) - AverageNoise;
		}
		for(size_t Sample = 0; Sample < analyzedData.DenoisedRawData().size(); Sample++)
			analyzedData.DenoisedRawData()[Sample] /= GaugeDAQ.numAverages();
	}
	else if (GaugeDAQ.AvgMode() == Cumulative_Moving_Avg)
	{
		GaugeDAQ.GrabScanData();
		for(size_t Sample = 0; Sample < analyzedData.DenoisedRawData().size(); Sample++)
			analyzedData.DenoisedRawData()[Sample] = 
				((GaugeDAQ.numAverages() - 1) * analyzedData.DenoisedRawData()[Sample] + GaugeDAQ.RawData(Sample) - AverageNoise) / GaugeDAQ.numAverages();
	}
	lastScanNumber = GaugeDAQ.lastScanNumber();
	controllerState = EnumGaugeState_SCAN;

	for (size_t Segment = 0; Segment < GaugeDAQ.SegmentBoundaries().size(); Segment++)
	{
		// http://homepages.which.net/~paul.hills/Circuits/MercurySwitchFilter/FIR.html
		double DAQFreq = GaugeDAQ.SegmentSizes[Segment] / 0.004;
		static const double Transition = 10E3;
		int Taps = int(0.5 + 3.3 * DAQFreq / Transition);
		int FirstSample = GaugeDAQ.SegmentBoundaries()[Segment].m_RawPoint - Taps;
		if (FirstSample < 0)
			FirstSample = 0;
		int LastSample = analyzedData.DenoisedRawData().size();
		if (Segment+1 < GaugeDAQ.SegmentBoundaries().size())
			LastSample =  GaugeDAQ.SegmentBoundaries()[Segment+1].m_RawPoint;
		Filter Filter(LPF, Taps, DAQFreq, Transition);
		for(int Sample = FirstSample; Sample < LastSample; Sample++)
		{
			int Error_Flag = Filter.get_error_flag();
			_ASSERT(Error_Flag == 0);
	 		analyzedData.DenoisedRawData()[Sample] = Filter.do_sample( analyzedData.DenoisedRawData()[Sample] );
		}
	}

	analyzedData.PeakArea().assign(size_t(GaugeDAQ.upperRange() - GaugeDAQ.lowerRange()), 0);
	pasynOctetSyncIO->setInputEos(IOUser, "\r", 1);
	*headerDataPtr = const_cast<IHeaderData*>(&GaugeDAQ.HeaderData());
	size_t Segment = 0;
	size_t RawPt = 0;
	for(size_t MassPt = 0; MassPt < analyzedData.PeakArea().size(); MassPt++)
	{
		size_t LowRawPt = GaugeDAQ.FindRawPt(Segment, MassPt-0.5);
		size_t HighRawPt = GaugeDAQ.FindRawPt(Segment, MassPt+0.5);
		for (RawPt = LowRawPt; RawPt < HighRawPt; RawPt++)
			analyzedData.PeakArea()[MassPt] += analyzedData.DenoisedRawData()[RawPt];
	}
	isValidData = true;
	return SVQM_800_Error();
}

