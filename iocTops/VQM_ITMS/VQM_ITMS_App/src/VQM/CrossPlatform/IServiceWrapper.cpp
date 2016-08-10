#include <asynDriver.h>
#include <asynOctet.h>
#include <asynOctetSyncIO.h>

#include <epicsTypes.h>
#include <epicsGuard.h>

#define epicsExportSharedSymbols
#include "IServiceWrapper.h"
#include "../../Exception.h"

#include "../../VQM/Include/SAnalyzedData.h"
#include "../../VQM/Include/SAverageData.h"

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

	isValidData = true;
	try {
		GaugeDAQ.GrabScanData();
		analyzedData.DenoisedRawData().resize(GaugeDAQ.RawDataSize());

		float AverageNoise = GaugeDAQ.AverageNoise();
		if (GaugeDAQ.AvgMode() != Cumulative_Moving_Avg)
			for(size_t Sample = 0; Sample < analyzedData.DenoisedRawData().size(); Sample++)
				analyzedData.DenoisedRawData()[Sample] = GaugeDAQ.RawData(Sample) - AverageNoise;
		if (GaugeDAQ.AvgMode() == Running_Avg)
		{
			std::deque<std::vector<epicsUInt16> > const& RawData = GaugeDAQ.RawData();
			for (size_t ScanNo = 1; ScanNo < RawData.size(); ScanNo++)
			{
				GaugeDAQ.GrabScanData();
				for(size_t Sample = 0; Sample < analyzedData.DenoisedRawData().size(); Sample++)
					analyzedData.DenoisedRawData()[Sample] += RawData[ScanNo][Sample] - AverageNoise;
			}
			for(size_t Sample = 0; Sample < analyzedData.DenoisedRawData().size(); Sample++)
				analyzedData.DenoisedRawData()[Sample] /= RawData.size();
		}
		else if (GaugeDAQ.AvgMode() == Accumulator)
		{
			for(int ScanNum = 1; ScanNum < GaugeDAQ.numAverages(); ScanNum++)
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
			for(size_t Sample = 0; Sample < analyzedData.DenoisedRawData().size(); Sample++)
				analyzedData.DenoisedRawData()[Sample] = 
				((GaugeDAQ.numAverages() - 1) * analyzedData.DenoisedRawData()[Sample] + GaugeDAQ.RawData(Sample) - AverageNoise) / GaugeDAQ.numAverages();
		}
		lastScanNumber = GaugeDAQ.lastScanNumber();
		controllerState = GaugeDAQ.GaugeState();

		if (controllerState == EnumGaugeState_SCAN)
		{
			size_t ThisSegmentBoundary = 0, NextSegmentBoundary;
			for (size_t Segment = 0; Segment < GaugeDAQ.MaxNrSegments; Segment++)
			{
				// http://homepages.which.net/~paul.hills/Circuits/MercurySwitchFilter/FIR.html
				double DAQFreq = GaugeDAQ.SegmentSizes[Segment] / 0.002;
				static const double Transition = 10E3;
				int Taps = int(0.5 + 3.3 * DAQFreq / Transition);
				int FirstSample = ThisSegmentBoundary - Taps;
				NextSegmentBoundary = ThisSegmentBoundary + GaugeDAQ.SegmentSizes[Segment];
				int LastSample = __min(NextSegmentBoundary, analyzedData.DenoisedRawData().size());
				Filter Filter(LPF, Taps, DAQFreq, Transition);
				for(int Sample = FirstSample; Sample < LastSample; Sample++)
				{
					_ASSERT(Filter.get_error_flag() == 0);
					double DenoisedRawData = (Sample >= 0) ? analyzedData.DenoisedRawData()[Sample] : 0;
					DenoisedRawData = Filter.do_sample( DenoisedRawData );
					if ((Sample >= 0) && (Sample < int(NextSegmentBoundary)))
						analyzedData.DenoisedRawData()[Sample] = DenoisedRawData;
				}
				ThisSegmentBoundary = NextSegmentBoundary;
			}

			analyzedData.PeakArea().assign(size_t(GaugeDAQ.upperRange() - GaugeDAQ.lowerRange()), 0);
			size_t Segment = 0;
			size_t RawPt = 0;
			for(size_t ScaledPt = 0; ScaledPt < analyzedData.PeakArea().size(); ScaledPt++)
			{
				double MassPt = ScaledPt + GaugeDAQ.lowerRange();
				size_t LowRawPt = GaugeDAQ.FindRawPt(RawPt, MassPt-0.5);
				size_t HighRawPt = GaugeDAQ.FindRawPt(RawPt, MassPt+0.5);
				for (RawPt = LowRawPt; RawPt <= HighRawPt; RawPt++)
					analyzedData.PeakArea()[ScaledPt] += analyzedData.DenoisedRawData()[RawPt];
			}
		}
		*headerDataPtr = const_cast<IHeaderData*>(&GaugeDAQ.HeaderData());
		averageData.SetValues(isValidData, GaugeDAQ.numAverages(), 0, 0, EnumPressureSource_None, GaugeDAQ.AvgMode());
	}
	catch(std::range_error&) {
		isValidData = false;
	}

	return SVQM_800_Error();
}

