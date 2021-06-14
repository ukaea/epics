/*
 * testAsynPortDriver.cpp
 *
 * Asyn driver that inherits from the asynPortDriver class to demonstrate its use.
 * It simulates a digital scope looking at a 1kHz 1000-point noisy sine wave.  Controls are
 * provided for time/division, volts/division, volt offset, trigger delay, noise amplitude, update time,
 * and run/stop.
 * Readbacks are provides for the waveform data, min, max and mean values.
 *
 * Author: Mark Rivers
 *
 * Created Feb. 5, 2009
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <iocsh.h>

#include "testAsynPortDriver.h"
#include <epicsExport.h>

#define FREQUENCY 10000       /* Frequency in Hz */
#define AMPLITUDE 1.0        /* Plus and minus peaks of sin wave */
#define NUM_DIVISIONS 10     /* Number of scope divisions in X and Y */
#define MIN_UPDATE_TIME 0.02 /* Minimum update time, to prevent CPU saturation */

#define MAX_ENUM_STRING_SIZE 20
static int allVoltsPerDivSelections[NUM_VERT_SELECTIONS]={1,2,5,10};

static const char *driverName="testAsynPortDriver";
void simTask(void *drvPvt);


/** Constructor for the testAsynPortDriver class.
  * Calls constructor for the asynPortDriver base class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxPoints The maximum  number of points in the volt and time arrays */
testAsynPortDriver::testAsynPortDriver(const char *portName, int maxAddr, int maxPoints)
   : asynPortDriver(portName,
                    maxAddr, /* maxAddr */
                    asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynEnumMask | asynDrvUserMask, /* Interface mask */
                    asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynEnumMask,  /* Interrupt mask */
                    0, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0) /* Default stack size*/
{
    asynStatus status;
    int i;
    const char *functionName = "testAsynPortDriver";

    /* Make sure maxPoints is positive */
    if (maxPoints < 1) maxPoints = 100;

	pData_.resize(maxAddr);
	pTimeBase_.resize(maxAddr);

	for (int Addr = 0; Addr < maxAddr; Addr++)
	{
		/* Allocate the waveform array */
		pData_[Addr] = (epicsFloat64 *)calloc(maxPoints, sizeof(epicsFloat64));
		/* Allocate the time base array */
		pTimeBase_[Addr] = (epicsFloat64 *)calloc(maxPoints, sizeof(epicsFloat64));
		/* Set the time base array */
		for (i = 0; i < maxPoints; i++) pTimeBase_[Addr][i] = (double)i / (maxPoints - 1) * NUM_DIVISIONS;
	}

    P_Run.resize(maxAddr);
    P_MaxPoints.resize(maxAddr);;
    P_TimePerDiv.resize(maxAddr);;
    P_TimePerDivSelect.resize(maxAddr);;
    P_VertGain.resize(maxAddr);;
    P_VertGainSelect.resize(maxAddr);;
    P_VoltsPerDiv.resize(maxAddr);;
    P_VoltsPerDivSelect.resize(maxAddr);;
    P_VoltOffset.resize(maxAddr);;
    P_TriggerDelay.resize(maxAddr);;
    P_NoiseAmplitude.resize(maxAddr);;
    P_UpdateTime.resize(maxAddr);;
    P_Waveform.resize(maxAddr);;
    P_TimeBase.resize(maxAddr);;
    P_MinValue.resize(maxAddr);;
    P_MaxValue.resize(maxAddr);;
    P_MeanValue.resize(maxAddr);;
    eventId_ = epicsEventCreate(epicsEventEmpty);
	for (int Addr = 0; Addr < maxAddr; Addr++)
	{
		createParam(Addr, P_RunString, asynParamInt32, &P_Run[Addr]);
        createParam(Addr, P_MaxPointsString, asynParamInt32, &P_MaxPoints[Addr]);
        createParam(Addr, P_TimePerDivString, asynParamFloat64, &P_TimePerDiv[Addr]);
        createParam(Addr, P_TimePerDivSelectString, asynParamInt32, &P_TimePerDivSelect[Addr]);
        createParam(Addr, P_VertGainString, asynParamFloat64, &P_VertGain[Addr]);
        createParam(Addr, P_VertGainSelectString, asynParamInt32, &P_VertGainSelect[Addr]);
        createParam(Addr, P_VoltsPerDivString, asynParamFloat64, &P_VoltsPerDiv[Addr]);
        createParam(Addr, P_VoltsPerDivSelectString, asynParamInt32, &P_VoltsPerDivSelect[Addr]);
        createParam(Addr, P_VoltOffsetString, asynParamFloat64, &P_VoltOffset[Addr]);
        createParam(Addr, P_TriggerDelayString, asynParamFloat64, &P_TriggerDelay[Addr]);
        createParam(Addr, P_NoiseAmplitudeString, asynParamFloat64, &P_NoiseAmplitude[Addr]);
        createParam(Addr, P_UpdateTimeString, asynParamFloat64, &P_UpdateTime[Addr]);
        createParam(Addr, P_WaveformString, asynParamFloat64Array, &P_Waveform[Addr]);
        createParam(Addr, P_TimeBaseString, asynParamFloat64Array, &P_TimeBase[Addr]);
        createParam(Addr, P_MinValueString, asynParamFloat64, &P_MinValue[Addr]);
        createParam(Addr, P_MaxValueString, asynParamFloat64, &P_MaxValue[Addr]);
        createParam(Addr, P_MeanValueString, asynParamFloat64, &P_MeanValue[Addr]);

        for (i = 0; i < NUM_VERT_SELECTIONS; i++) {
            // Compute vertical volts per division in mV
            voltsPerDivValues_[i] = 0;
            voltsPerDivStrings_[i] = (char *)calloc(MAX_ENUM_STRING_SIZE, sizeof(char));
            voltsPerDivSeverities_[i] = 0;
        }

        /* Set the initial values of some parameters */
        setIntegerParam(Addr, P_MaxPoints[Addr], maxPoints);
        setIntegerParam(Addr, P_Run[Addr], 0);
        setIntegerParam(Addr, P_VertGainSelect[Addr], 10);
        setVertGain(Addr);
        setDoubleParam(Addr, P_VoltsPerDiv[Addr], 1.0);
        setDoubleParam(Addr, P_VoltOffset[Addr], 0.0);
        setDoubleParam(Addr, P_TriggerDelay[Addr], 0.0);
        setDoubleParam(Addr, P_TimePerDiv[Addr], 0.001);
        setDoubleParam(Addr, P_UpdateTime[Addr], 0.5);
        setDoubleParam(Addr, P_NoiseAmplitude[Addr], 0.0);
        setDoubleParam(Addr, P_MinValue[Addr], 0.0);
        setDoubleParam(Addr, P_MaxValue[Addr], 0.0);
        setDoubleParam(Addr, P_MeanValue[Addr], 0.0);
    }

    /* Create the thread that computes the waveforms in the background */
    status = (asynStatus)(epicsThreadCreate("testAsynPortDriverTask",
                          epicsThreadPriorityMedium,
                          epicsThreadGetStackSize(epicsThreadStackMedium),
                          (EPICSTHREADFUNC)::simTask,
                          this) == NULL);
    if (status) {
        printf("%s:%s: epicsThreadCreate failure\n", driverName, functionName);
        return;
    }
}

testAsynPortDriver::~testAsynPortDriver()
{
	for (int Addr = 0; Addr < maxAddr; Addr++)
	{
		/* Allocate the waveform array */
		free(pData_[Addr]);
		/* Allocate the time base array */
		free(pTimeBase_[Addr]);
	}
}

void simTask(void *drvPvt)
{
    testAsynPortDriver *pPvt = (testAsynPortDriver *)drvPvt;

    pPvt->simTask();
}

/** Simulation task that runs as a separate thread.  When the P_Run parameter is set to 1
  * to rub the simulation it computes a 1 kHz sine wave with 1V amplitude and user-controllable
  * noise, and displays it on
  * a simulated scope.  It computes waveforms for the X (time) and Y (volt) axes, and computes
  * statistics about the waveform. */
void testAsynPortDriver::simTask(void)
{
    /* This thread computes the waveform and does callbacks with it */

    double timePerDiv, voltsPerDiv, voltOffset, triggerDelay, noiseAmplitude;
    double updateTime, minValue, maxValue, meanValue;
    double time, timeStep;
    double noise, yScale;
    epicsInt32 run, i, maxPoints;
    double pi=4.0*atan(1.0);

    lock();
    /* Loop forever */
    while (1) {
        for (int Addr = 0; Addr < maxAddr; Addr++)
        {
            getDoubleParam(P_UpdateTime[Addr], &updateTime);
            getIntegerParam(P_Run[Addr], &run);
            // Release the lock while we wait for a command to start or wait for updateTime
            unlock();
            if (run) epicsEventWaitWithTimeout(eventId_, updateTime);
            else     (void)epicsEventWait(eventId_);
            // Take the lock again
            lock();
            /* run could have changed while we were waiting */
            getIntegerParam(P_Run[Addr], &run);
            if (!run) continue;
            getIntegerParam(P_MaxPoints[Addr], &maxPoints);
            getDoubleParam(P_TimePerDiv[Addr], &timePerDiv);
            getDoubleParam(P_VoltsPerDiv[Addr], &voltsPerDiv);
            getDoubleParam(P_VoltOffset[Addr], &voltOffset);
            getDoubleParam(P_TriggerDelay[Addr], &triggerDelay);
            getDoubleParam(P_NoiseAmplitude[Addr], &noiseAmplitude);
            time = triggerDelay;
            timeStep = timePerDiv * (Addr + 1) * NUM_DIVISIONS / maxPoints;
            minValue = 1e6;
            maxValue = -1e6;
            meanValue = 0.;

            yScale = 1.0 / voltsPerDiv;
            for (i=0; i<maxPoints; i++) {
                noise = noiseAmplitude * (rand() / (double)RAND_MAX - 0.5);
                pData_[Addr][i] = AMPLITUDE * (sin(time*FREQUENCY * 2 * pi)) + noise;
                /* Compute statistics before doing the yOffset and yScale */
                if (pData_[Addr][i] < minValue) minValue = pData_[Addr][i];
                if (pData_[Addr][i] > maxValue) maxValue = pData_[Addr][i];
                meanValue += pData_[Addr][i];
                pData_[Addr][i] = NUM_DIVISIONS / 2 + yScale * (voltOffset + pData_[Addr][i]);
                time += timeStep;
            }
            updateTimeStamp();
            meanValue = meanValue / maxPoints;
            setDoubleParam(Addr, P_MinValue[Addr], minValue);
            setDoubleParam(Addr, P_MaxValue[Addr], maxValue);
            setDoubleParam(Addr, P_MeanValue[Addr], meanValue);
            callParamCallbacks(Addr, P_MinValue[Addr]);
            doCallbacksFloat64Array(pData_[Addr], maxPoints, P_Waveform[Addr], Addr);
        }
    }
}

/** Called when asyn clients call pasynInt32->write().
  * This function sends a signal to the simTask thread if the value of P_Run has changed.
  * For all parameters it sets the value in the parameter library and calls any registered callbacks..
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Value to write. */
asynStatus testAsynPortDriver::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    int function = pasynUser->reason;
    int Addr = 0;
    asynStatus status = asynSuccess;
    const char *paramName;
    const char* functionName = "writeInt32";
    getAddress(pasynUser, &Addr);

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setIntegerParam(function, value);

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);

    if (function == P_Run[Addr]) {
        /* If run was set then wake up the simulation task */
        if (value) epicsEventSignal(eventId_);
    }
    else if (function == P_VertGainSelect[Addr]) {
        setVertGain(Addr);
    }
    else if (function == P_VoltsPerDivSelect[Addr]) {
        setVoltsPerDiv(Addr);
    }
    else if (function == P_TimePerDivSelect[Addr]) {
        setTimePerDiv(Addr);
    }
    else {
        /* All other parameters just get set in parameter list, no need to
         * act on them here */
    }

    /* Do callbacks so higher layers see any changes */
    status = (asynStatus) callParamCallbacks();

    if (status)
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                  "%s:%s: status=%d, function=%d, name=%s, value=%d",
                  driverName, functionName, status, function, paramName, value);
    else
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
              "%s:%s: function=%d, name=%s, value=%d\n",
              driverName, functionName, function, paramName, value);
    return status;
}

/** Called when asyn clients call pasynFloat64->write().
  * This function sends a signal to the simTask thread if the value of P_UpdateTime has changed.
  * For all  parameters it  sets the value in the parameter library and calls any registered callbacks.
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Value to write. */
asynStatus testAsynPortDriver::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    int function = pasynUser->reason;
    int Addr = 0;
    asynStatus status = asynSuccess;
    epicsInt32 run;
    const char *paramName;
    const char* functionName = "writeFloat64";
    getAddress(pasynUser, &Addr);

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setDoubleParam(function, value);

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);

    if (function == P_UpdateTime[Addr]) {
        /* Make sure the update time is valid. If not change it and put back in parameter library */
        if (value < MIN_UPDATE_TIME) {
            asynPrint(pasynUser, ASYN_TRACE_WARNING,
                "%s:%s: warning, update time too small, changed from %f to %f\n",
                driverName, functionName, value, MIN_UPDATE_TIME);
            value = MIN_UPDATE_TIME;
            setDoubleParam(P_UpdateTime[Addr], value);
        }
        /* If the update time has changed and we are running then wake up the simulation task */
        getIntegerParam(P_Run[Addr], &run);
        if (run) epicsEventSignal(eventId_);
    } else {
        /* All other parameters just get set in parameter list, no need to
         * act on them here */
    }

    /* Do callbacks so higher layers see any changes */
    status = (asynStatus) callParamCallbacks();

    if (status)
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                  "%s:%s: status=%d, function=%d, name=%s, value=%f",
                  driverName, functionName, status, function, paramName, value);
    else
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
              "%s:%s: function=%d, name=%s, value=%f\n",
              driverName, functionName, function, paramName, value);
    return status;
}


/** Called when asyn clients call pasynFloat64Array->read().
  * Returns the value of the P_Waveform or P_TimeBase arrays.
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Pointer to the array to read.
  * \param[in] nElements Number of elements to read.
  * \param[out] nIn Number of elements actually read. */
asynStatus testAsynPortDriver::readFloat64Array(asynUser *pasynUser, epicsFloat64 *value,
                                         size_t nElements, size_t *nIn)
{
    int function = pasynUser->reason;
	int Addr = 0;
	size_t ncopy;
    epicsInt32 itemp;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp;
    const char *functionName = "readFloat64Array";

	getAddress(pasynUser, &Addr);
	getTimeStamp(&timeStamp);
    pasynUser->timestamp = timeStamp;
    getIntegerParam(Addr, P_MaxPoints[Addr], &itemp); ncopy = itemp;
    if (nElements < ncopy) ncopy = nElements;
    if (function == P_Waveform[Addr]) {
        memcpy(value, pData_[Addr], ncopy*sizeof(epicsFloat64));
        *nIn = ncopy;
    }
    else if (function == P_TimeBase[Addr]) {
        memcpy(value, pTimeBase_[Addr], ncopy*sizeof(epicsFloat64));
        *nIn = ncopy;
    }
    if (status)
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                  "%s:%s: status=%d, function=%d",
                  driverName, functionName, status, function);
    else
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
              "%s:%s: function=%d\n",
              driverName, functionName, function);
    return status;
}

asynStatus testAsynPortDriver::readEnum(asynUser *pasynUser, char *strings[], int values[], int severities[], size_t nElements, size_t *nIn)
{
    int function = pasynUser->reason;
	int Addr = 0;
	size_t i;
	getAddress(pasynUser, &Addr);

    if (function == P_VoltsPerDivSelect[Addr]) {
        for (i=0; ((i<NUM_VERT_SELECTIONS) && (i<nElements)); i++) {
            if (strings[i]) free(strings[i]);
            strings[i] = epicsStrDup(voltsPerDivStrings_[i]);
            values[i] = voltsPerDivValues_[i];
            severities[i] = 0;
        }
    }
    else {
        *nIn = 0;
        return asynError;
    }
    *nIn = i;
    return asynSuccess;
}

void testAsynPortDriver::setVertGain(int Addr)
{
    epicsInt32 igain, i;
    double gain;

    getIntegerParam(Addr, P_VertGainSelect[Addr], &igain);
    gain = igain;
    setDoubleParam(Addr, P_VertGain[Addr], gain);
    for (i = 0; i < NUM_VERT_SELECTIONS; i++) {
        epicsSnprintf(voltsPerDivStrings_[i], MAX_ENUM_STRING_SIZE, "%.2f", allVoltsPerDivSelections[i] / gain);
        // The values are in mV
        voltsPerDivValues_[i] = (int)(allVoltsPerDivSelections[i] / gain * 1000. + 0.5);
    }
    doCallbacksEnum(voltsPerDivStrings_, voltsPerDivValues_, voltsPerDivSeverities_, NUM_VERT_SELECTIONS, P_VoltsPerDivSelect[Addr], 0);
}

void testAsynPortDriver::setVoltsPerDiv(int Addr)
{
    epicsInt32 mVPerDiv;

    // Integer volts are in mV
    getIntegerParam(Addr, P_VoltsPerDivSelect[Addr], &mVPerDiv);
    setDoubleParam(Addr, P_VoltsPerDiv[Addr], mVPerDiv / 1000.);
}

void testAsynPortDriver::setTimePerDiv(int Addr)
{
    epicsInt32 microSecPerDiv;

    // Integer times are in microseconds
    getIntegerParam(Addr, P_TimePerDivSelect[Addr], &microSecPerDiv);
    setDoubleParam(Addr, P_TimePerDiv[Addr], microSecPerDiv / 1000000.);
}


/* Configuration routine.  Called directly, or from the iocsh function below */

extern "C" {

/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxPoints The maximum  number of points in the volt and time arrays */
int testAsynPortDriverConfigure(const char *portName, int maxAddr, int maxPoints)
{
    new testAsynPortDriver(portName, maxAddr, maxPoints);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */

static const iocshArg initArg0 = { "portName",iocshArgString};
static const iocshArg initArg1 = { "max addr",iocshArgInt };
static const iocshArg initArg2 = { "max points",iocshArgInt };
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1,
                                            &initArg2 };
static const iocshFuncDef initFuncDef = {"testAsynPortDriverConfigure",3,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    testAsynPortDriverConfigure(args[0].sval, args[1].ival, args[2].ival);
}

void testAsynPortDriverRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(testAsynPortDriverRegister);

}

