#pragma once

#include "IServiceWrapper.h"

/*
Interface for ADC Output obtained from the header data.
*/
struct IADCOutput
{
    virtual STRING_TYPE Units() const = 0;
    virtual STRING_TYPE DataTSource() const = 0;
    virtual unsigned int OTCounter() const = 0;
    virtual unsigned int BOCounter() const = 0;
    virtual double BeginAMU() const = 0;
    virtual double EndAMU() const = 0;
    virtual unsigned int BeginSegment() const = 0;
    virtual unsigned int EndSegment() const = 0;
    virtual int NSamples() const = 0;
    virtual double SignalBaselineOffset() const = 0;
    virtual double RMSNoise() const = 0;
    virtual STRING_TYPE CurveDate() const = 0;
    virtual STRING_TYPE CurveTime() const = 0;
	
	/*
	Dispose of this instance when it is no longer needed, to prevent any memory leaks.
	*/
	virtual void Release() = 0;	
};

/*
Interface for Mass Axis obtained from the header data.
*/
struct IMassAxis
{
	virtual STRING_TYPE Units() const = 0;
    virtual double BeginAMU() const = 0;
    virtual double EndAMU() const = 0;
    virtual unsigned int BeginSegment() const = 0;
    virtual unsigned int EndSegment() const = 0;
    virtual unsigned int* Segments(int& size) const = 0;
    virtual int NSamples() const = 0;
    virtual STRING_TYPE CurveDate() const = 0;
    virtual STRING_TYPE CurveTime() const = 0;

	/*
	Dispose of this instance when it is no longer needed, to prevent any memory leaks.
	*/
	virtual void Release() = 0;	
};

/*
Interface for Noise Data obtained from the header data.
*/
struct INoiseData
{
	virtual STRING_TYPE Units() const = 0;
    virtual int NSamples() const = 0;
    virtual double SignalBaselineOffset() const = 0;
    virtual double RMSNoise() const = 0;
    virtual STRING_TYPE CurveDate() const = 0;
    virtual STRING_TYPE CurveTime() const = 0;

	/*
	Dispose of this instance when it is no longer needed, to prevent any memory leaks.
	*/
	virtual void Release() = 0;	
};

/*
Interface for header data.
*/
struct IHeaderData
{
public:	
	virtual STRING_TYPE Version() const = 0;
    virtual STRING_TYPE IdenDate() const = 0;
    virtual STRING_TYPE IdenTime() const = 0;
    virtual STRING_TYPE MID() const = 0;
    virtual STRING_TYPE HardwareRevision() const = 0;
    virtual STRING_TYPE FirmwareRevision() const = 0;
    virtual unsigned int REGSTB() const = 0;
    virtual unsigned int REGSESR() const = 0;
    virtual bool HasErrors() const = 0;
    virtual double MassAxisCalibrationFactor() const = 0;
    virtual double EmissionCurrent() const = 0;
    virtual double FilamentBiasVoltage() const = 0;
    virtual double FilamentPower() const = 0;
    virtual double DDSAmplitude() const = 0;
    virtual double RepellerVoltage() const = 0;
    virtual double EntryPlateVoltage() const = 0;
    virtual double CupsVoltage() const = 0;
    virtual double TransitionVoltage() const = 0;
    virtual double ExitPlateVoltage() const = 0;
    virtual double ElectronMultiplierShieldVoltage() const = 0;
    virtual double ElectronMultiplierVoltage() const = 0;
    virtual double ElectronMultiplierElectrometerVoltage() const = 0;
    virtual double ElectronMultiplierElectrometerGain() const = 0;
    virtual double PressurePlateVoltage() const = 0;
    virtual STRING_TYPE ETPressureDate() const = 0;
    virtual STRING_TYPE ETPressureTime() const = 0;
    virtual STRING_TYPE ETPressureUnits() const = 0;
    virtual double TotalPressure() const = 0;
    virtual STRING_TYPE ESignalDate() const = 0;
    virtual STRING_TYPE ESignalTime() const = 0;
    virtual STRING_TYPE ESignalUnits() const = 0;
    virtual double ExternalSignalValue() const = 0;

	virtual unsigned int QuestionableContStat() const = 0;
    virtual unsigned int OperationalContStat() const = 0;        
    virtual unsigned int QuestionableIonizerStat() const = 0;        
    virtual unsigned int OperationalIonizerStat() const = 0;        
    virtual unsigned int QuestionableMseparatorStat() const = 0;        
    virtual unsigned int OperationalMseparatorStat() const = 0;        
    virtual unsigned int QuestionableDetectorStat() const = 0;        
    virtual unsigned int OperationalDetectorStat() const = 0;        
    virtual unsigned int QuestionableESignalStat() const = 0;        
    virtual unsigned int OperationalESignalStat() const = 0;        
    virtual unsigned int QuestionableETPressure1Stat() const = 0;        
    virtual unsigned int QuestionableETPressure2Stat() const = 0;        
    virtual unsigned int QuestionableETPressure3Stat() const = 0;        
    virtual unsigned int QuestionableETPressureStat() const = 0;
    virtual double ElectronMultiplierElectrometerOffset() const = 0;

	virtual const IADCOutput* ADCOutput() const = 0;
    virtual const IMassAxis* MassAxis() const = 0;
    virtual const INoiseData* NoiseData() const = 0;

	/*
	Dispose of this instance when it is no longer needed, to prevent any memory leaks.
	*/
	virtual void Release() = 0;	
};