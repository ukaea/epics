/***************************************************************************
    Copyright 2007 Ulrik Pedersen, Diamond Light Source Ltd.
    
    This program is distributed under the terms of the
    GNU General Public License.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

****************************************************************************/

/*******************/
/* System includes */
/*******************/
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <string>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

/******************/
/* EPICS includes */
/******************/
#include <asynDriver.h>
#include <asynInt32.h>
#include <asynFloat64.h>
#include <asynInt32Array.h>
#include <asynFloat64Array.h>
#include <asynUInt32Digital.h>
#include <asynDrvUser.h>
#include <epicsTypes.h>
#include <epicsString.h>
#include <epicsEvent.h>
#include <epicsMutex.h>
#include <epicsThread.h>
#include <errLog.h>
#include <epicsExit.h>
#include <epicsMessageQueue.h>
#include <epicsTime.h>
#include <iocsh.h>

#include <NIDAQmx.h>

#define epicsExportSharedSymbols
#include <epicsExport.h>

/* various useful enumeration declarations */
#define DEFAULT_SAMPLE_RATE 10000.0
#define DEFAULT_MIN -10.0
#define DEFAULT_MAX 10.0
#define DEFAULT_NSAMPLES 1000
#define DEFAULT_TIMEOUT 5.0
#define DEFAULT_DMA_BUF 128
#define DEFAULT_WAIT_DELAY 0.3
#define MESSAGE_Q_CAPACITY 5
#define EVENT_DATA 1

typedef enum {
	msgNoAction = 0,
	msgConfigure,
	msgStart,
	msgStop
} daqMxMessage;

typedef enum {
    slopeRising = DAQmx_Val_RisingSlope,
    slopeFalling = DAQmx_Val_FallingSlope
} daqMxSlope; 


typedef enum {
	unconfigured = 0,
	reconfigure,
	configure,
	idle,
	start,
	busywait,
	acquireAnlg,
	acquireDig,
	acquireCnt,
	write,
	counterout,
	stop
} daqMxState;

typedef enum {
	continuous = 0,
	oneshot,
	MAX_TRIGGER_MODES,
} daqMxTriggerMode;

#define MAX_ACQ_TYPES 6
#define MIN_ACQ_TYPES 1
typedef enum {
	NotDefined,
	AI,
	AO,
	BI,
	BO,
	CI,
	CO
} daqMxAcqMode;

typedef enum{
    Digital,Analog
	} daqMxTriggerType;

typedef enum {
	typFloat64,
	typFloat64Array,
	typInt32,
	typInt32Array,
	typUInt32Digital,
	MAX_TYPES,
} daqMxDataTypes;

/* interface structures */
typedef struct daqAioPvt {
        void * data;
        epicsInt32 dataSize;
        char * devicename;
	epicsFloat64 avgData;
	epicsFloat64 min;
	epicsFloat64 max;
        epicsInt32  terminal;
} daqAioPvt;

typedef struct daqBioPvt{
    void * data; 
    epicsInt32 dataSize;
    char * devicename;
} daqBioPvt;

typedef struct daqCiPvt{
    void * data;
    epicsInt32 dataSize;
    epicsFloat64 min;
    epicsFloat64 max;
} daqCiPvt;

/* driver structure */
#define MAX_CHANNELS 32
#define ERR_BUF_SIZE 512
typedef struct daqMxPvt {
	ELLNODE node;
	daqMxAcqMode daqMode;
	daqMxTriggerMode trigMode;
        char monstermode;
	TaskHandle taskHandle;
	epicsFloat64 timeout;
	char *portName;
	int nChannels;
	int totalNSamples;
	epicsInt32 nSamples;
	epicsInt32 samplesTransferred;
        char polled; /*  The driver will not read until the record tries to read*/
        epicsEventId pollnow;
        epicsEventId polldone;
	epicsFloat64 sampleRate;
	daqAioPvt *aioPvt[MAX_CHANNELS];
        daqBioPvt *bioPvt[MAX_CHANNELS];
	daqMxState state;
	void *rawData;
        void *prevData;
        epicsInt32 rawDataSize;
	char daqMxErrBuf[ERR_BUF_SIZE];
	epicsMutexId lock;
	epicsMessageQueueId msgQid;
        epicsEventId msgEvent;
	epicsThreadId threadId;
	epicsFloat64 dAcqTime;

    epicsEventId writeEvent;
    char writeNeeded;
    
    char triggerEnabled; 
    daqMxSlope triggerSlope;
    epicsFloat64 triggerLevel;
    epicsInt32 triggerPreSamples;
    char softTriggerMode;
    int softTriggerChannel;
    int softTriggerPos;

    daqMxTriggerType triggerType;
    char * triggerDevice;

    char *clockSource;

    /* Counter stuff */
    daqCiPvt *ciPvt;
    char * counterDevName;
    int counterIdleState;
    epicsFloat64 counterDelay;
    epicsFloat64 counterDutyCycle;
    int counterCI; /* 0=err 1=CIPeriod 2=CICountEdges 3=CIPulseWidth */
    int counterEdge; /*0=DAQmx_Val_Rising 1= DAQmx_Val_Falling*/
    int counterDirection; /*0=DAQmx_Val_CountUp 1=DAQmx_Val_CountDown 2=DAQmx_Val_ExtControlled*/
    int counterRead; /*0=F64 1=ScalarF64 2=ScalerU32 3=U32 */

    int noRestartTask;
	
    

	/* all the asyn crap */
	asynUser *pasynUser;
	asynInterface common;
	asynInterface int32;
	void *int32InterruptPvt;
	asynInterface float64;
	void *float64InterruptPvt;
	asynInterface int32Array;
	void *int32ArrayInterruptPvt;
	asynInterface float64Array;
	void *float64ArrayInterruptPvt;
	asynInterface uint32Digital;
	void *uint32DigitalInterruptPvt;
	asynInterface drvUser;
	epicsEventId intEventId;
} daqMxPvt;

static ELLLIST daqMxDeviceList;

static int globalshutdown;

/* command / reason structures */
typedef enum {
	dataCmd = 0,
	acquireCmd,
	triggerModeCmd,
	sampleRateCmd,
	nSamplesCmd,
	maxNSamplesCmd,
	minVoltCmd,
	maxVoltCmd,
	dTimeCmd,
	stateCmd,
	enableTriggerCmd,
	triggerSlopeCmd,
	triggerLevelCmd,
	triggerPreSamplesCmd,
	terminalCmd,
	
	MAX_COMMANDS,
} daqMxCommand;
/*
 Need to add:
 softTriggerModeCmd
 softTriggerChanCmd
 softTriggerPosCmd (readonly)
 
 counterIdleStateCmd
 counterDelayCmd
 counterDutyCycleCmd

 (I'm sure I'm missing more?!)  

 */

typedef struct {
	daqMxCommand command;
	char *commandString;
} daqMxCommandStuct;

static daqMxCommandStuct daqMxCommands[MAX_COMMANDS] = {
	{dataCmd,			"DATA"},
	{acquireCmd,		"ACQUIRE"},
	{triggerModeCmd,	"TRIGGERMODE"},
	{sampleRateCmd,		"SAMPLERATE"},
	{nSamplesCmd,		"NSAMPLES"},
	{maxNSamplesCmd,    "MAX_NELM"},
	{minVoltCmd,		"MIN_VOLT"},
	{maxVoltCmd,		"MAX_VOLT"},
	{dTimeCmd,			"D_TIME"},
	{stateCmd,          "STATE"},
	{enableTriggerCmd,  "TRIGGERENABLE"},
	{triggerSlopeCmd ,  "TRIGGERSLOPE"},
	{triggerLevelCmd,   "TRIGGERLEVEL"},
	{triggerPreSamplesCmd, "TRIGGERPRESAMPLES"},
	{terminalCmd, "TERMINAL"},
};


/*****************************************************/
/* Function prototypes                              */
/***************************************************/

/* These functions are in public interfaces */
static void dmbReport               (void *drvPvt, FILE *fp, int details);
static asynStatus dmbConnect        (void *drvPvt, asynUser *pasynUser);
static asynStatus dmbDisconnect     (void *drvPvt, asynUser *pasynUser);

static asynStatus int32Write		(void *drvPvt, asynUser *pasynUser, epicsInt32 value);
static asynStatus int32Read			(void *drvPvt, asynUser *pasynUser, epicsInt32 *value);
static asynStatus getBounds			(void *drvPvt, asynUser *pasynUser, epicsInt32 *low, epicsInt32 *high);

static asynStatus float64Write      (void *drvPvt, asynUser *pasynUser,
                                     epicsFloat64 value);
static asynStatus float64Read       (void *drvPvt, asynUser *pasynUser,
                                     epicsFloat64 *value);

static asynStatus int32ArrayRead	(void *drvPvt, asynUser *pasynUser, 
									 epicsInt32 *data, size_t maxChans, 
									 size_t *nactual);
static asynStatus int32ArrayWrite   (void *drvPvt, asynUser *pasynUser,
									 epicsInt32 *data,
									 size_t maxChans);

static asynStatus float64ArrayRead	(void *drvPvt, asynUser *pasynUser, 
									 epicsFloat64 *data, size_t maxElements, 
									 size_t *nactual);
static asynStatus float64ArrayWrite (void *drvPvt, asynUser *pasynUser,
									 epicsFloat64 *data,
									 size_t maxElements);

static asynStatus uint32DigitalWrite(void *drvPvt, asynUser *pasynUser,
									 epicsUInt32 value, epicsUInt32 mask);
static asynStatus uint32DigitalRead	(void *drvPvt, asynUser *pasynUser,
									 epicsUInt32 *value, epicsUInt32 mask);

static asynStatus drvUserCreate     (void *drvPvt, asynUser *pasynUser,
                                     const char *drvInfo,
                                     const char **pptypeName, size_t *psize);
static asynStatus drvUserGetType    (void *drvPvt, asynUser *pasynUser,
                                     const char **pptypeName, size_t *psize);
static asynStatus drvUserDestroy    (void *drvPvt, asynUser *pasynUser);

/* Private driver routines */
void daqThread(void *param);
static asynStatus dmbWrite(	void *drvPvt, asynUser *pasynUser,
							epicsInt32 ivalue, epicsFloat64 dvalue);
static asynStatus dmbRead(	void *drvPvt, asynUser *pasynUser,
							epicsInt32 *pivalue, epicsFloat64 *pfvalue);

static asynStatus allocBuffers(daqMxPvt *pPvt, asynUser *pasynUser);
daqMxPvt *getAsynPort( char *portName );


/* IOC shell command function prototypes */
int DAQmxConfig(char *portName, char * deviceName, int Channelnr, char* sacqType, char* options);

/*****************************************************/
/* Asyn interface Structures                        */
/***************************************************/

/*
 * asynCommon methods
 */
static struct asynCommon dmbCommon = {
	dmbReport,
	dmbConnect,
	dmbDisconnect
};

/* asynInt32 methods */
static asynInt32 dmbInt32 = {
	int32Write,
	int32Read,
	getBounds,
	NULL,
	NULL
};

/* asynFloat64 methods */
static asynFloat64 dmbFloat64 = {
    float64Write,
    float64Read,
    NULL,
    NULL
};

/* asynInt32Array methods */
static asynInt32Array dmbInt32Array = {
	int32ArrayWrite,
	int32ArrayRead,
	NULL,
	NULL
};

/* asynFloat64Array methods */
static asynFloat64Array dmbFloat64Array = {
	float64ArrayWrite,
	float64ArrayRead,
	NULL,
	NULL
};

/* asynUInt32Digital methods */
static asynUInt32Digital dmbUInt32Digital = {
    uint32DigitalWrite,
    uint32DigitalRead,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

/* asynDrvUser methods */
static asynDrvUser dmbDrvUser = {
    drvUserCreate,
    drvUserGetType,
    drvUserDestroy
};

static const char* OnboardClock = "OnboardClock";

/*
  Simply sends a start message
  (only if needed)
 */
int SendStart(daqMxPvt *pPvt)
{
    daqMxMessage msg = msgStart;
    
    if (!pPvt) return -1;
    /* For the following states we do not need to start again: */
    if (pPvt->state == acquireAnlg) return 0;
    if (pPvt->state == acquireDig) return 0;
    if (pPvt->state == acquireCnt) return 0;
    if (pPvt->state == write) return 0;
    if (pPvt->state == busywait) return 0;
    if (pPvt->state == start) return 0;
    if (pPvt->state == counterout) return 0;

    epicsMutexLock(pPvt->lock);
    if( epicsMessageQueueTrySend( pPvt->msgQid, &msg, sizeof(daqMxMessage) ) != 0)
    {
	asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
			"dmbWrite: failed to send message %d\n",msg);
	return -1;
    }	
    epicsEventSignal(pPvt->msgEvent);
    epicsMutexUnlock(pPvt->lock);
    return 0;
}


/**********************************************************************/
/*                                                                   */
/* Function implementations of the ASYN interface                   */
/*                                                                 */
/******************************************************************/


/****            Asyn common routines          *******/
static void dmbReport(void *drvPvt, FILE *fp, int details)
{
    daqMxPvt *pPvt = (daqMxPvt *)drvPvt;
    if (fp == NULL) fp = stdout;
    if (!pPvt){
	fprintf(fp,"Error Reporting - drvPvt == NULL\n");
	return;
    }

    
    fprintf(fp,"DAQport: %s\n",pPvt->portName);
    fprintf(fp,"Nr of Channels: %d\n",pPvt->nChannels);
    fprintf(fp,"Samples per Channel: %d\n",pPvt->nSamples);
    fprintf(fp,"Total N Samples: %d\n",pPvt->totalNSamples);

    switch (pPvt->state){
	case unconfigured:
	    fprintf(fp,"State: unconfigured\n");
	    break;
	case reconfigure:
	    fprintf(fp,"State: reconfigure\n");
	    break;
	case configure:
	    fprintf(fp,"State: configure\n");
	    break;
	case idle:
	    fprintf(fp,"State: idle\n");
	    break;
	case start:
	    fprintf(fp,"State: start\n");
	    break;
	case busywait:
	    fprintf(fp,"State: busywait\n");
	    break;
	case acquireAnlg:
	    fprintf(fp,"State: acquire Analog\n");
	    break;
	case acquireDig:
	    fprintf(fp,"State: acquire Digital\n");
	    break;
	case acquireCnt:
	    fprintf(fp,"State: acquire Counter\n");
	    break;
	case write:
	    fprintf(fp,"State: write\n");
	    break;
	case counterout:
	    fprintf(fp,"State: counter output\n");
	    break;
	case stop:
	    fprintf(fp,"State: stop\n");
	    break;
    }
}

static asynStatus dmbConnect(void *drvPvt, asynUser *pasynUser)
{
	int signal;
	pasynManager->getAddr(pasynUser, &signal);
	pasynManager->exceptionConnect(pasynUser);
	return(asynSuccess);
}

static asynStatus dmbDisconnect(void *drvPvt, asynUser *pasynUser)
{
	pasynManager->exceptionDisconnect(pasynUser);
	return(asynSuccess);
}


/****            Asyn INT32 routines           *******/
static asynStatus int32Write(void *drvPvt, asynUser *pasynUser, epicsInt32 value)
{
	return( dmbWrite( drvPvt, pasynUser, value, 0. ));
}

static asynStatus int32Read(void *drvPvt, asynUser *pasynUser, epicsInt32 *value)
{
	return( dmbRead( drvPvt, pasynUser, value, NULL ));
}

static asynStatus getBounds(void *drvPvt, asynUser *pasynUser, epicsInt32 *low, epicsInt32 *high)
{
    /*  I'm not exactly sure about this? 
     Changed it to return Error as 0,0 is really not correct :)*/
    /*
     *low = 0;
     *high = 0;
     */
	return( asynError );
}

/****          Asyn float64 routines           *******/

static asynStatus float64Write(	void *drvPvt, asynUser *pasynUser,
								epicsFloat64 value)
{
	return(dmbWrite(drvPvt, pasynUser, 0, value));
}

static asynStatus float64Read(	void *drvPvt, asynUser *pasynUser,
								epicsFloat64 *value)
{
	return(dmbRead(drvPvt, pasynUser, NULL, value));
}

/****            Asyn int32 array routines      *******/
static asynStatus int32ArrayRead	(void *drvPvt, asynUser *pasynUser, 
									 epicsInt32 *data, size_t maxNElements, 
									 size_t *nactual)
{
	daqMxPvt *pPvt = (daqMxPvt *)drvPvt;
	int command = pasynUser->reason;
  
	/*asynStatus status = asynSuccess;*/
	int signal;
	size_t elementsToCopy;

	if (command != dataCmd){
	    printf("### ERROR: Int32Array interface can only be used for DATA\n");
	    return (asynError);
	}
	if ((pPvt->daqMode != BI)&&(pPvt->daqMode != BO)){
	    return asynError;
	}

	pasynManager->getAddr(pasynUser, &signal);

/*	asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
				"float64ArrayRead: command = %d (%s), signal = %d\n",
				command, daqMxCommands[command].commandString, signal);*/

	if( signal > pPvt->nChannels || signal < 0)
	{
		asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
					"### ERROR: signal %d has not been configured. Chose signal below %d\n",
					signal, pPvt->nChannels);
		return (asynError);
	}

	if (pPvt->polled){
	    /*
	      Note: There is a copy of this piece of code in dmbRead - alter both
	      I'm not sure if this state check is good enough?
	     */
	    SendStart(pPvt);

	    epicsEventSignal(pPvt->pollnow);
	    if (epicsEventWaitWithTimeout(pPvt->polldone,5.0) != epicsEventWaitOK){
		asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR,
			  "### ERROR: polldone event timeout after 5 secs (Maybe something stalled?)\n");
	    }
	}
	if (maxNElements > pPvt->nSamples)
	    elementsToCopy = pPvt->nSamples;
	else
	    elementsToCopy = maxNElements;
	epicsMutexLock(pPvt->lock);
	/*printf("Reading Float .. signal = %d\n",signal);*/
	memcpy( data, pPvt->bioPvt[signal]->data, elementsToCopy *sizeof(epicsInt32));

	*nactual = elementsToCopy;
	
	epicsMutexUnlock(pPvt->lock);
	return(asynSuccess);
}

static asynStatus int32ArrayWrite   (void *drvPvt, asynUser *pasynUser,
									 epicsInt32 *data,
									 size_t maxNElements)
{

	daqMxPvt *pPvt = (daqMxPvt *)drvPvt;
	int command = pasynUser->reason;
	/*asynStatus status = asynSuccess;*/
	int signal;
	size_t elementsToCopy;


	if (command != dataCmd){
	    printf("### ERROR: Int32Array interface can only be used for DATA\n");
	    return (asynError);
	}
	if (pPvt->daqMode != BO){
	    printf("### ERROR: Int32Array-write interface can only be used for daqMode = BO\n");
	    return (asynError);
	}

	pasynManager->getAddr(pasynUser, &signal);

/*	asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
				"float64ArrayRead: command = %d (%s), signal = %d\n",
				command, daqMxCommands[command].commandString, signal);*/

	if( signal > pPvt->nChannels || signal < 0)
	{
		asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
					"### ERROR: signal %d has not been configured. Chose signal below %d\n",
					signal, pPvt->nChannels);
		return (asynError);
	}


	if (maxNElements > pPvt->nSamples)
	    elementsToCopy = pPvt->nSamples;
	else
	    elementsToCopy = maxNElements;
	epicsMutexLock(pPvt->lock);

	if (!pPvt->bioPvt[signal]){
	    asynPrint(pPvt->pasynUser,ASYN_TRACE_ERROR,"### ERROR Int32ArrayWrite - bioPvt = NULL\n");
	    return asynError;
	}
	if ((!pPvt->bioPvt[signal]->data) ||(pPvt->bioPvt[signal]->dataSize < elementsToCopy)){
	    asynPrint(pPvt->pasynUser,ASYN_TRACE_ERROR,"ERROR Int32ArrayWrite - bioPvt doesn't have data structure (fixing)\n ");
	    allocBuffers(pPvt,pPvt->pasynUser);
	    /*
	    if (pPvt->aioPvt[signal]->data) free(pPvt->aioPvt[signal]->data);
	    pPvt->aioPvt[signal]->data = malloc(pPvt->nSamples*sizeof(epicsFloat64));
	    pPvt->aioPvt[signal]->dataSize = pPvt->nSamples;
	    */
	}
	
	memcpy( pPvt->bioPvt[signal]->data, data, elementsToCopy * sizeof(epicsInt32));
	/* signal to actually write this data: */
	pPvt->writeNeeded = 1;
	epicsEventSignal(pPvt->writeEvent);
	
	epicsMutexUnlock(pPvt->lock);

	return(asynSuccess);
}

/****            Asyn Float64 array routines      *******/
static asynStatus float64ArrayRead	(void *drvPvt, asynUser *pasynUser, 
									 epicsFloat64 *data, size_t maxNElements, 
									 size_t *nactual)
{
	daqMxPvt *pPvt = (daqMxPvt *)drvPvt;
	int command = pasynUser->reason;
  
	/*asynStatus status = asynSuccess;*/
	int signal;
	size_t elementsToCopy;


	if (command != dataCmd){
	    printf("### ERROR: float64Array interface can only be used for DATA\n");
	    return (asynError);
	}
	if ((pPvt->daqMode != AI)&&(pPvt->daqMode != AO)){
	    printf("### ERROR: float64Array interface can only be used for AI or AO\n");
	    return asynError;
	}

	pasynManager->getAddr(pasynUser, &signal);

/*	asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
				"float64ArrayRead: command = %d (%s), signal = %d\n",
				command, daqMxCommands[command].commandString, signal);*/

	if( (signal > pPvt->nChannels) || (signal < 0))
	{
		asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
					"### ERROR: signal %d has not been configured. Chose signal below %d\n",
					signal, pPvt->nChannels);
		return (asynError);
	}

	if (pPvt->rawData == NULL){
	    *nactual = 0;
	    return (asynSuccess);
	}

	if (pPvt->polled){
	    /*
	      Note: There is a copy of this piece of code in dmbRead - alter both
	      I'm not sure if this state check is good enough?
	     */
	    SendStart(pPvt);

	    epicsEventSignal(pPvt->pollnow);
	    if (epicsEventWaitWithTimeout(pPvt->polldone,5.0) != epicsEventWaitOK){
		asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR,
			  "### ERROR: polldone event timeout after 5 secs (Maybe something stalled?)\n");
	    }
	}
	if (maxNElements > pPvt->aioPvt[signal]->dataSize)
	    elementsToCopy = pPvt->aioPvt[signal]->dataSize;
	else
	    elementsToCopy = maxNElements;
	epicsMutexLock(pPvt->lock);

/*	printf("rawData=%p prevData=%p signaldata=%p\n ",pPvt->rawData,pPvt->prevData,pPvt->aioPvt[signal]->data);*/
	if (pPvt->aioPvt[signal]->data)
	    memcpy( data, pPvt->aioPvt[signal]->data, elementsToCopy *sizeof(epicsFloat64));
	else
	    elementsToCopy = 0;

	*nactual = elementsToCopy;
	
	epicsMutexUnlock(pPvt->lock);
	return (asynSuccess);
}

static asynStatus float64ArrayWrite (void *drvPvt, asynUser *pasynUser,
									 epicsFloat64 *data,
									 size_t maxNElements)
{
	daqMxPvt *pPvt = (daqMxPvt *)drvPvt;
	int command = pasynUser->reason;
	/*asynStatus status = asynSuccess;*/
	int signal;
	size_t elementsToCopy;

	if (command != dataCmd){
	    printf("### ERROR: float64Array interface can only be used for DATA\n");
	    return (asynError);
	}
	if (pPvt->daqMode != AO){
	    return asynError;
	}

	pasynManager->getAddr(pasynUser, &signal);

/*	asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
				"float64ArrayRead: command = %d (%s), signal = %d\n",
				command, daqMxCommands[command].commandString, signal);*/



	if( signal > pPvt->nChannels || signal < 0)
	{
		asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
					"### ERROR: signal %d has not been configured. Chose signal below %d\n",
					signal, pPvt->nChannels);
		return (asynError);
	}


	if (maxNElements > pPvt->nSamples)
	    elementsToCopy = pPvt->nSamples;
	else
	    elementsToCopy = maxNElements;
	epicsMutexLock(pPvt->lock);

	if (!pPvt->aioPvt[signal]){
	    asynPrint(pPvt->pasynUser,ASYN_TRACE_ERROR,"### ERROR float64ArrayWrite - aioPvt = NULL\n");
	    return asynError;
	}
	if ((!pPvt->aioPvt[signal]->data) ||(pPvt->aioPvt[signal]->dataSize < elementsToCopy)){
	    asynPrint(pPvt->pasynUser,ASYN_TRACE_ERROR,"ERROR float64ArrayWrite - aioPvt doesn't have data structure (fixing)\n ");
	    allocBuffers(pPvt,pPvt->pasynUser);
	    /*
	    if (pPvt->aioPvt[signal]->data) free(pPvt->aioPvt[signal]->data);
	    pPvt->aioPvt[signal]->data = malloc(pPvt->nSamples*sizeof(epicsFloat64));
	    pPvt->aioPvt[signal]->dataSize = pPvt->nSamples;
	    */
	}
	


	memcpy( pPvt->aioPvt[signal]->data, data, elementsToCopy * sizeof(epicsFloat64));
	/* signal to actually write this data: */
	pPvt->writeNeeded = 1;
	epicsEventSignal(pPvt->writeEvent);
	
	epicsMutexUnlock(pPvt->lock);

    

	return( asynSuccess );
}

/****          Asyn int32 digital routines      *******/
static asynStatus uint32DigitalWrite(void *drvPvt, asynUser *pasynUser,
									 epicsUInt32 value, epicsUInt32 mask)
{
	return( dmbWrite( drvPvt, pasynUser, (epicsInt32)value, 0. ));
}

static asynStatus uint32DigitalRead	(void *drvPvt, asynUser *pasynUser,
									 epicsUInt32 *value, epicsUInt32 mask)
{
	return( dmbRead( drvPvt, pasynUser, (epicsInt32*)value, NULL ));
}


/****            Asyn drvUser routines          *******/
static asynStatus drvUserCreate(void *drvPvt, asynUser *pasynUser,
                                const char *drvInfo,
                                const char **pptypeName, size_t *psize)
{
	int i;
	char *pstring;
	char *token;
	char *dupstring;
	char success;
	char overallsuccess;

	
	asynPrint(	pasynUser, ASYN_TRACE_FLOW,
				"drvDaqMxBase::drvUserCreate, attempting to create cmd: %s\n", drvInfo);

	/* This will handle every "word" in the string seperately */
	overallsuccess = 0;

	dupstring = (char*)malloc(strlen(drvInfo)+2);
	strcpy(dupstring,drvInfo);
	
	token = strtok(dupstring," ");

	while(token){
	    while(dupstring[0] == ' ') dupstring++; /*remove leading spaces*/

	    success = 0;
	    /* First test for standard commands */
	    for (i=0; i < MAX_COMMANDS; i++)
	    {
		pstring = daqMxCommands[i].commandString;
		if (epicsStrCaseCmp( token, pstring) == 0)
		{
		    pasynUser->reason = daqMxCommands[i].command;
		    if (pptypeName)
			*pptypeName = epicsStrDup(pstring);
		    if (psize)
			*psize = sizeof( daqMxCommands[i].command);
		    asynPrint(	pasynUser, ASYN_TRACE_FLOW,
				"drvDaqMxBase::drvUserCreate, command created: %s\n", pstring);
		    success = 1;
		    overallsuccess = 1;
		    break;
		}
	    }
	    
	    /* Now check for some other custom configuration options */
	    if (!success){


	    }
		token = strtok(NULL," ");
	}
	
	free(dupstring);
	if (overallsuccess)
	    return (asynSuccess);
	
			
	asynPrint(	pasynUser, ASYN_TRACE_ERROR,
				"drvDaqMxBase::drvUserCreate, unknown command: %s\n", drvInfo);
	return( asynError );			

}

static asynStatus drvUserGetType(void *drvPvt, asynUser *pasynUser,
                                 const char **pptypeName, size_t *psize)
{
	int command = pasynUser->reason;
	if ( pptypeName )
		*pptypeName = epicsStrDup( daqMxCommands[command].commandString);
	if ( psize )
		*psize = sizeof( command );
	return( asynSuccess );
}

static asynStatus drvUserDestroy    (void *drvPvt, asynUser *pasynUser)
{
	return( asynSuccess );
}
static asynStatus WriteOne(daqMxPvt *pPvt, asynUser *pasynUser, int signal, epicsInt32 ivalue, epicsFloat64 dvalue)
{
    if (pPvt->daqMode == AO){
	if (!pPvt->aioPvt[signal]) {
	    asynPrint(pPvt->pasynUser,ASYN_TRACE_ERROR,"dmbWrite:dataCmd - invalid signal number\n");
	    return asynError;
	}
	
	if (!pPvt->aioPvt[signal]->data)
	    allocBuffers(pPvt,pPvt->pasynUser);
	
	SendStart(pPvt); /* just to make sure */
	
	/* enter 1 value */
	*(epicsFloat64*)pPvt->aioPvt[signal]->data = dvalue;
	/* Tell the system to update it :) */
	pPvt->writeNeeded = 1;
	epicsEventSignal(pPvt->writeEvent);
    }else if (pPvt->daqMode == BO){
	if (!pPvt->bioPvt[signal]) {
	    asynPrint(pPvt->pasynUser,ASYN_TRACE_ERROR,"dmbWrite:dataCmd - invalid signal number\n");
	    return asynError;
	}
	
	if (!pPvt->bioPvt[signal]->data)
	    allocBuffers(pPvt,pPvt->pasynUser);
	
	SendStart(pPvt); /* just to make sure */
	
	/* enter 1 value */
	*(epicsInt32*)pPvt->bioPvt[signal]->data = ivalue;
	/* Tell the system to update it :) */
	pPvt->writeNeeded = 1;
	epicsEventSignal(pPvt->writeEvent);
    }else if (pPvt->daqMode == CO){
	/* This is only a write to CO
	 Data is ignored
	Simply trigger the thing*/
	SendStart(pPvt);
	pPvt->writeNeeded = 1;
	epicsEventSignal(pPvt->writeEvent);
    }else{
	asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR,
		  "### ERROR: DATA-write only supported for AO and BO\n");
	return asynError;
    }


    return asynSuccess;
} 

static asynStatus dmbWrite(	void *drvPvt, asynUser *pasynUser,
							epicsInt32 ivalue, epicsFloat64 dvalue)
{
	daqMxPvt *pPvt = (daqMxPvt *)drvPvt;
	int command = pasynUser->reason;
	asynStatus status = asynSuccess;
	int signal;
	daqMxMessage msg = msgNoAction;
	
	pasynManager->getAddr(pasynUser, &signal);

	asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
				"dmbWrite: command = %d (%s), signal = %d, dval = %f, ival = %i\n",
				command, daqMxCommands[command].commandString, signal, dvalue, ivalue);
	
	epicsMutexLock(pPvt->lock);
	switch(command)
	{
		case dataCmd:
		    status = WriteOne(pPvt,pasynUser,signal,ivalue,dvalue);
		    break;
		case acquireCmd:
			if (ivalue == 1) msg = msgStart;
			else msg = msgStop;
			break;

		case triggerModeCmd:
			if (ivalue == 0){
			    pPvt->trigMode = continuous;
			    pPvt->polled = 0;
			}
			else if (ivalue == 1){
			    pPvt->trigMode = oneshot;
			    pPvt->polled = 1; /* defaults to this */
			}
			else 
			{
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
							"dmbWrite: invalid argument - this command accept only 0 and 1\n");
				status = asynError;
				break;
			}
			msg = msgConfigure;
			break;
		case sampleRateCmd:
		    /*if (ivalue) pPvt->sampleRate = (epicsFloat64)ivalue;*/
			if (dvalue) pPvt->sampleRate = dvalue;
			msg = msgConfigure;
			break;

		case nSamplesCmd:
			if (ivalue > 0)
				pPvt->nSamples = ivalue;
			msg = msgConfigure;
			break;

		case minVoltCmd:
		    pPvt->aioPvt[signal]->min = dvalue;
		    msg = msgConfigure;
		    break;
		case maxVoltCmd:
		    pPvt->aioPvt[signal]->max = dvalue;
		    msg = msgConfigure;
		    break;
	    case enableTriggerCmd:
		if (ivalue){
		    pPvt->triggerEnabled = 1;
		}else{
		    pPvt->triggerEnabled = 0;
		}
		msg = msgConfigure;
		break;
	    case triggerSlopeCmd:
		if (ivalue){
		    if (pPvt->triggerSlope != slopeRising){
			pPvt->triggerSlope = slopeRising;
			msg = msgConfigure;
		    }
		}else{
		    if (pPvt->triggerSlope != slopeFalling){
			pPvt->triggerSlope = slopeFalling;
			msg = msgConfigure;
		    }
		}
		break;
	    case triggerLevelCmd:
		pPvt->triggerLevel = dvalue;
		msg = msgConfigure;
		break;
	    case triggerPreSamplesCmd:
		pPvt->triggerPreSamples = ivalue;
		msg = msgConfigure;
		break;
	    case terminalCmd:
		switch (ivalue){
		    case 0:
			pPvt->aioPvt[signal]->terminal = DAQmx_Val_Cfg_Default;
			break;
		    case 1:
			pPvt->aioPvt[signal]->terminal = DAQmx_Val_RSE;
			break;
		    case 2:
			pPvt->aioPvt[signal]->terminal = DAQmx_Val_NRSE;
			break;
		    case 3:
			pPvt->aioPvt[signal]->terminal = DAQmx_Val_Diff;
			break;
		    default:
			asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR,"dmbWrite terminalCmd only support 0 -> 3 \n");
			break;
		}
		msg = msgConfigure;
		break;
	    default:
		asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
				"dmbWrite: unknonw command: %d\n", command);
		status = asynError;
		break;
			
	}
	if (msg != msgNoAction){
	    if( epicsMessageQueueTrySend( pPvt->msgQid, &msg, sizeof(daqMxMessage) ) != 0)
	    {
		asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
				"dmbWrite: failed to send message %d\n",msg);
		status = asynError;
	    }	
	    epicsEventSignal(pPvt->msgEvent);
	}

	epicsMutexUnlock(pPvt->lock);
	return( status );
}

static asynStatus ReadOne(daqMxPvt *pPvt, asynUser *pasynUser, int signal, epicsInt32 *pivalue, epicsFloat64 *pfvalue)
{
    if (pPvt == NULL)
	return asynError;
    if (pPvt->polled)
    {
	SendStart(pPvt);

	epicsEventSignal(pPvt->pollnow);
	if (epicsEventWaitWithTimeout(pPvt->polldone,5.0) != epicsEventWaitOK){
	    asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR,
		      "### ERROR: polldone event timeout after 5 secs (Maybe something stalled?)\n");
	}
    }



    if ((pPvt->daqMode == AI)||(pPvt->daqMode == AO)){
	if (pPvt->aioPvt[signal] == NULL)
	    return asynError;
	if (pPvt->aioPvt[signal]->data == NULL)
	    return asynError;

	if (pivalue != NULL)
	    *pivalue = (epicsInt32)*(epicsFloat64*)pPvt->aioPvt[signal]->data;
	if (pfvalue != NULL)
	    *pfvalue = *(epicsFloat64*)pPvt->aioPvt[signal]->data;
    }else if ((pPvt->daqMode == BI)||(pPvt->daqMode == BO)){
	if (pPvt->bioPvt[signal] == NULL)
	    return asynError;
	if (pPvt->bioPvt[signal]->data == NULL)
	    return asynError;

	if (pivalue != NULL)
	    *pivalue = *(epicsInt32*)pPvt->bioPvt[signal]->data;
	if (pfvalue != NULL)
	    *pfvalue = (epicsFloat64)*(epicsInt32*)pPvt->bioPvt[signal]->data;
    }else if (pPvt->daqMode == CI){
	if (pPvt->ciPvt == NULL)
	    return asynError;
	if (pPvt->ciPvt->data == NULL) 
	    return asynError;
	if (pivalue != NULL){
	    if (pPvt->counterCI == 2)
		*pivalue = *(epicsInt32*)pPvt->ciPvt->data;
	    else{
		asynPrint(pPvt->pasynUser,ASYN_TRACE_ERROR,
			  "### ERROR: DATA-read int32 COUNTER only if counterCI = CReadScalarU32\n");
		return asynError;
	    }
	}
	if (pfvalue != NULL){
	    if (pPvt->counterCI == 1)
		*pfvalue = *(epicsFloat64*)pPvt->ciPvt->data;
	    else{
		asynPrint(pPvt->pasynUser,ASYN_TRACE_ERROR,
			  "### ERROR: DATA-read int32 COUNTER only if counterCI = CReadScalarF64\n");
		return asynError;
	    }
	}
    }else{
	asynPrint(pPvt->pasynUser,ASYN_TRACE_ERROR,
		   "### ERROR: DATA-read interface only supported for AI and BI\n");
	return asynError;
    }
    return asynSuccess;
}

static asynStatus dmbRead(	void *drvPvt, asynUser *pasynUser,
							epicsInt32 *pivalue, epicsFloat64 *pfvalue)
{
	daqMxPvt *pPvt = (daqMxPvt *)drvPvt;
	int command = pasynUser->reason;
	asynStatus status = asynSuccess;	
	int signal;
	
	pasynManager->getAddr(pasynUser, &signal);

	asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
				"dmbRead: command = %d (%s), signal = %d\n",
				command, daqMxCommands[command].commandString, signal);

	
	if (signal < 0 || signal >= pPvt->nChannels)
	{
		asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
					"dmbRead: invalid signal: %d\n (should be in range {0...%d})\n", signal, pPvt->nChannels - 1);
		return asynError;
	}	
	
	/*epicsMutexLock(pPvt->lock);*/
	switch(command)
	{
  	        case dataCmd:
		    status = ReadOne(pPvt,pasynUser,signal,pivalue,pfvalue);
		    break;
		case acquireCmd:
			if(pivalue != NULL)
			{
				if ((pPvt->state == acquireAnlg)||(pPvt->state == acquireDig)||(pPvt->state == busywait))
				    *pivalue = 1;
				else
				    *pivalue = 0;
			}
			if (pfvalue != NULL) *pfvalue = -1;
			break;
		case triggerModeCmd:
			if(pivalue != NULL) *pivalue = pPvt->trigMode;
			break;
		case sampleRateCmd:
			if (pivalue != NULL) *pivalue = -1;
			if (pfvalue != NULL) *pfvalue = pPvt->sampleRate;
			break;
		case nSamplesCmd:
			if (pfvalue != NULL) *pfvalue = (double)pPvt->nSamples;
			if (pivalue != NULL) *pivalue = pPvt->nSamples;
			break;
		case minVoltCmd:
			if (pivalue != NULL) *pivalue = -1;
			if (pfvalue != NULL) *pfvalue = pPvt->aioPvt[signal]->min;
			break;
		case maxVoltCmd:
			if (pivalue != NULL) *pivalue = -1;
			if (pfvalue != NULL) *pfvalue = pPvt->aioPvt[signal]->max;
			break;
		case dTimeCmd:
			if (pivalue != NULL) *pivalue = (epicsInt32)pPvt->dAcqTime;
			if (pfvalue != NULL) *pfvalue = (epicsFloat64)pPvt->dAcqTime;
			break;
		case stateCmd:
			if (pivalue != NULL) *pivalue = (epicsInt32)pPvt->state;
			break;
	    case enableTriggerCmd:
		if (pivalue != NULL) *pivalue = (epicsInt32)pPvt->triggerEnabled;
		break;
	    case triggerSlopeCmd:
		if (pivalue != NULL){
		    if (pPvt->triggerSlope == slopeRising)
			*pivalue = 1;
		    else
			*pivalue = 0;
		}
		break;
	    case triggerLevelCmd:
		if (pfvalue) *pfvalue = pPvt->triggerLevel;
		break;
	    case triggerPreSamplesCmd:
		if (pivalue) *pivalue = pPvt->triggerPreSamples;
		break;
	    case terminalCmd:
		if (pivalue){
		    switch (pPvt->aioPvt[signal]->terminal){
			case DAQmx_Val_Cfg_Default:
			    *pivalue = 0;
			    break;
			case DAQmx_Val_RSE:
			    *pivalue = 1;
			    break;
			case DAQmx_Val_NRSE:
			    *pivalue = 2;
			    break;
			case DAQmx_Val_Diff:
			    *pivalue = 3;
			    break;
			default:
			    *pivalue = -1;
		    }
		}
		break;

	    default:
		asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
				"dmbRead: unknown command: %d\n", command);
		status = asynError;
		break;
	}
	/*epicsMutexUnlock(pPvt->lock);*/
	return( status );
}

static std::string g_PrevGenPort;
static int g_PrevGenChan;

void DAQmxGenDig(daqMxPvt *pPvt, int Channelnr, char * params)
{
    int dataSize;
    int i;
    epicsInt32 *func;

    if (!pPvt) return;
    if (pPvt->daqMode != BO) return;
    
    if (!pPvt->bioPvt[Channelnr]->data){
	if (allocBuffers(pPvt,pPvt->pasynUser) != asynSuccess){
 	    printf("Buffer create error - Gen not done\n");
	    return;
	}
    }
    dataSize = pPvt->bioPvt[Channelnr]->dataSize;
    func = (epicsInt32*)pPvt->bioPvt[Channelnr]->data;

    printf("Creating digital counter\n");
    for (i = 0; i < dataSize; i++)
    {
	func[i] = i;
    }

    /* Force it to write :) */
    pPvt->writeNeeded = 1;
    epicsEventSignal(pPvt->writeEvent);
    SendStart(pPvt);

}

void DAQmxGen(const char * portName, int Channelnr, char * params)
{
	char * token;
	daqMxPvt * pPvt; 
	int i;
	int dataSize;
	epicsFloat64 * func;
	epicsFloat64 value;
	epicsFloat64 values[26];
	char tmp;
	int function = -1;
	int tmpi2;
	int tmpi3;
	int tmpi4;
	int tmpi5;
	epicsFloat64 tmpf2;
	epicsFloat64 tmpf3;
	epicsFloat64 tmpf4;
	char clear = 0;

	for (i = 0; i < 26; i++)
		values[i] = 0;

	pPvt = (daqMxPvt*) ellFirst(&daqMxDeviceList);
	/* run through the linked list to find a node with the
	* same asyn port name */
	while (pPvt != NULL)
	{
		if( strcmp(portName, pPvt->portName) == 0) break;
		pPvt = (daqMxPvt*) ellNext((ELLNODE*)pPvt);
	}

	if (pPvt == NULL){
		printf("ERROR: Can't find port %s\n",portName);
		return;
	}

	if (Channelnr >= pPvt->nChannels){
		printf("ERROR: Invalid Channelnr (number of channels in port: %d)\n",pPvt->nChannels);
		return;
	}


	g_PrevGenPort = portName;
	g_PrevGenChan = Channelnr;

	if (pPvt->daqMode == BO){
		DAQmxGenDig(pPvt,Channelnr,params);
		return;
	}
	if (pPvt->daqMode != AO){
		printf("ERROR: DAQmxGen does not support this port's DAQ mode - sorry\n");
		return;
	}

	token = strtok(params," ");
	while (token)
	{
		if (epicsStrCaseCmp(token,"clear") == 0){
			clear = 1;
		}else if (epicsStrCaseCmp(token,"block") == 0){
			function = 0;
			values[0] = 1; /* A = samples at top*/
			values[1] = 1; /* B = samples at bottom*/
			values[2] = 1; /* C = top volts level*/
			values[3] = -1; /* D = bottom volts level*/
		}else if (epicsStrCaseCmp(token,"Random") == 0){
			function = 1;
			values[0] = 1; /* A = max value*/
			values[1] = -1; /* B = min value */
			values[2] = 255; /* C = number of levels*/
			values[3] = 0; /* D = seed if non-zero */
		}else if (epicsStrCaseCmp(token,"Triangle") == 0){
			function = 2;
			values[0] = 5; /*A = height of triangle */
			values[1] = 0; /*B = centre of signal */
			/* value of 0 for C or D will go directly to the other side = saw functionality*/
			values[2] = 0.1; /* C = delta Volts per sample upwards edge*/
			values[3] = 0.1; /* D = delta Volts per sample downwards edge*/
			values[4] = 0; /* E = Time stationary at top */
			values[5] = 0; /* F = Time stationary at bottom*/
		}else if (epicsStrCaseCmp(token,"Offset") == 0){
			function = 3; 
			values[0] = 1; /* A = offset to add */
		}else if (epicsStrCaseCmp(token,"Sin") == 0){
			function = 4;
			values[0] = 0.1; /* A =radiants per sample*/
			values[1] = 2; /* B = amplitude */
			values[2] = 0; /* C = offset */
			values[3] = 0; /* D = starting radiants*/
		}else if (epicsStrCaseCmp(token,"RemoveOffset") == 0){
			function = 5;
			/* no params */
		}else if (epicsStrCaseCmp(token,"block2") == 0){
			/* Adds 1 block somewhere in waveform */
			function = 6;
			values[0] = 1; /* A = starting sample number */
			values[1] = 10; /* B = number of samples for block*/
			values[2] = 2.0; /* C = height of block (can be negative)*/
		}else if (epicsStrCaseCmp(token,"Pulse") == 0){
			function = 7;
			/* This is suppose to be a pulse as generated by a signal generator :)*/
			values[0] = 25; /* A = centre of pulse sample number*/
			values[1] = 5; /* B = amplitude of pulse */
			values[2] = 0.7; /* C = gravity factor (actually need an e^ type function) */
		}else if (epicsStrCaseCmp(token,"SmoothRandom") == 0){
			function = 8;
			values[0] = 2.0; /* A = max value*/
			values[1] = -2.0; /* B = min value*/
			values[2] = 0.1; /* C = max delta*/
			values[3] = 0; /* D = seed if non-zero*/
		}else if (epicsStrCaseCmp(token,"StartEndMatch") == 0){
			/* Will alter the beginning and end of waveform as to make them match more smoothly */
			function = 9;
			values[0] = 0.1; /* A = rate of change allowed */
		}else if (epicsStrCaseCmp(token,"smooth") == 0){
			function = 10;
			values[0] = 0.1; /* A = max rate of change allowed*/
		}else if (epicsStrCaseCmp(token,"filter") == 0){
			function = 11;
			values[0] = 4; /* A = filter factor*/

		}else if (epicsStrCaseCmp(token,"buldge") == 0){
			function = 12;
			values[0] = 25; /*A = centre of buldge */
			values[1] = 50; /* B = width of buldge */
			values[2] = 2.0; /* B= amplitude (can be negative)*/
		}else if (epicsStrCaseCmp(token,"clearregion") == 0){
			function = 13;
			values[0] = 0; /* A = starting sample*/
			values[1] = 10; /* B = ending sample number*/
		}else if (epicsStrCaseCmp(token,"Scale") == 0){
			function = 14;
			values[0] = 0.5; /* A = scale factor*/
			values[1] = 0; /* B = starting samples number*/
			values[2] = 0; /* C = ending sample number*/
		}else if (epicsStrCaseCmp(token,"xxx") == 0){

		}else if (strlen(token) > 2){
			/* We have paramters a -> z */
			tmp = *token;
			token++; token++;
			if ((tmp >= 'A')&&(tmp <= 'Z')){
				values[(tmp-'A')] = (epicsFloat64)atof(token);
				printf("%c(%d) = %.2f\n",tmp,(int)(tmp-'A'),values[(tmp-'A')]);
			}
			if ((tmp >= 'a')&&(tmp <= 'z')){
				values[(tmp-'a')] = (epicsFloat64)atof(token);
				printf("%c(%d) = %.2f\n",tmp,(int)(tmp-'a'),values[(tmp-'a')]);
			}
		}
		token = strtok(NULL," ");
	}
	if (!pPvt->aioPvt[Channelnr]->data){
		if (allocBuffers(pPvt,pPvt->pasynUser) != asynSuccess){
			printf("Buffer create error - Gen not done\n");
			return;
		}
	}
	dataSize = pPvt->aioPvt[Channelnr]->dataSize;
	func = (epicsFloat64*)pPvt->aioPvt[Channelnr]->data;


	if (clear){
		printf("Clearing data\n");
		for (i = 0; i < dataSize; i++)
			func[i] = 0;
	}

	/* Function generate 
	See above section for details on how these actually work
	*/
	switch (function){
	case -1:
		printf("No waveform generation function specified\n");
		break;
	case 0: /* block */
		printf("Generating block waveform\n");
		tmpi2 = 0;
		tmpi3 = 0;
		value = values[3];
		for (i = 0 ; i < dataSize ; i++){
			/*printf("Value %d = %.2f\n",i,value);*/
			func[i] = func[i] + value;
			tmpi2++;
			if (tmpi3 == 0){
				/*at bottom*/
				if (tmpi2 >= values[1]){
					value = values[2]; /* top value*/
					tmpi2 = 0;
					tmpi3 = 1;
				}
			}else{
				/*top*/
				if (tmpi2 >= values[0]){
					value = values[3]; /*bottom value*/
					tmpi2 = 0;
					tmpi3 = 0;
				}
			}
		}
		break;
	case 1: 
		printf("Generating random waveform");
		if ((int)values[3] != 0)
			srand((int)values[3]);
		tmpi2 = (int)values[2];
		tmpf2 = values[0] - values[1]; /* calc span*/
		for (i = 0; i < dataSize; i++){
			value = (rand() % tmpi2); /* get a number */
			value = value * tmpf2 / tmpi2; /* scale it down into region we use */
			value = value + values[1]; /*offset added*/
			func[i] = func[i] + value;
		}
		break;
	case 2:
		printf("Generating triangle waveform");
		tmpf2 = values[1] + (values[0]/2); /*top*/
		tmpf3 = values[1] - (values[0]/2); /*bottom*/
		printf("top:%f bottom:%f\n",tmpf2,tmpf3);
		value = values[1]; /* start on centre going up*/
		tmpi4 = 0; /* 0=upwards 1=top 2=downwards 3=bottom*/
		tmpi5 = 0; /* count top/bottom */
		for (i = 0; i < dataSize; i++){
			func[i] = func[i] + value;
			/*printf("%d = %.2f  (tmpi4=%d)\n",i,value,tmpi4);*/
			switch(tmpi4){
			case 0: /*upwards*/
				value += values[2];
				if ((value >= tmpf2)||(values[2] <= 0)){
					value = tmpf2;
					if ((int)values[4] > 0){
						tmpi4 = 1;
						tmpi5 = 0;
					}else{
						tmpi4 = 2;
					}
				}
				break;
			case 1: /*top*/
				tmpi5++;
				if (tmpi5 >= (int)values[4]){
					tmpi4 = 2;
					value -= values[3];
				}
				break;
			case 2:
				value -= values[3];
				if ((value <= tmpf3)||(values[3] <= 0)){
					value = tmpf3;
					if ((int)values[5] > 0){
						tmpi4 = 3;
						tmpi5 = 0;
					}else{
						tmpi4 = 0;
					}
				}
				break;
			case 3: /*bottom*/
				tmpi5++;
				if (tmpi5 >= (int)values[5]){
					tmpi4 = 0;
					value += values[2];
				}
				break;
			}
		}
		break;
	case 3:
		printf("Adding offset");
		for (i = 0; i < dataSize; i++)
			func[i] = func[i] + values[0];
		break;
	case 4:
		printf("Generating Sin waveform\n");
		tmpf2 = values[3];
		for (i = 0; i < dataSize; i++){
			value = (sin(tmpf2)*values[1])+ values[2];
			tmpf2 = tmpf2 + values[0];
			func[i] = func[i] + value;
		}
		break;
	case 5: /* remove the offset*/
		tmpf3 = 0;
		for (i = 0; i < dataSize; i++)
			tmpf3 = tmpf3 + func[i];
		tmpf3 = tmpf3 / dataSize;
		printf("Removing offset (%.3f)\n",tmpf3);
		for (i = 0; i < dataSize; i++)
			func[i] = func[i] - tmpf3;
		break;
	case 6:
		printf("Adding a block into the waveform\n");
		tmpi2 = (int)values[0];
		for (i = 0; i < values[1] ; i++){
			if ((tmpi2 >= 0)&&(tmpi2 < dataSize))
				func[tmpi2] = func[tmpi2] + values[2];
			tmpi2++;
		}
		break;
	case 7:
		printf("Adding pulse");
		tmpi2 = (int)values[0]+1;
		tmpi3 = tmpi2 - 1;
		value = values[1];
		while (fabs(value) > fabs(values[1]/ 25)){
			func[tmpi2] = func[tmpi2] - value;
			func[tmpi3] = func[tmpi3] + value;
			value = value * values[2];
			tmpi2++;
			if (tmpi2 >= dataSize) tmpi2 = 0;
			tmpi3--;
			if (tmpi3 < 0) tmpi3 = dataSize-1;
		}
		break;
	case 8:
		printf("Generating smooth random");
		if ((int)values[3] != 0)
			srand((int)values[3]);
		value = 0;
		for (i = 0; i < dataSize; i++)
		{
			func[i] = func[i] + value;
			tmpf4 = ((epicsFloat64)(rand() % 1000) * values[2]/ 500.0);
			tmpf4 = tmpf4 - values[2];
			value = value + tmpf4;
			if (value >= values[0])
				value = values[0];
			if (value <= values[1])
				value = values[1];
		}
		break;
	case 9: /*start end match*/
		printf("Matching start and end  (%.2f)\n",func[0]-func[dataSize-1]);
		tmpf2 = func[0] - func[dataSize - 1];
		tmpf3 = tmpf2/2;
		tmpi2 = 0;
		while (tmpf3 > 0){
			func[tmpi2] = func[tmpi2] - tmpf3;
			tmpf3 = tmpf3 - values[0];
			tmpi2++;
		}
		tmpf3 = tmpf2/2;
		tmpi2 = dataSize-1;
		while (tmpf3 > 0){
			func[tmpi2] = func[tmpi2] + tmpf3;
			tmpf3 = tmpf3 - values[0];
			tmpi2--;
		}
		break;
	case 10:
		printf("smoothin waveform\n");
		value = func[0];
		for (i = 0; i < dataSize ; i++){
			if (func[i] >= value){
				if (func[i] - value > values[0])
					value = value + values[0];
				else
					value = func[i];
			}else{
				if (value - func[i] >values[0])
					value = value - values[0];
				else
					value = func[i];
			}
			func[i] = value;
		}
		break;
	case 11:
		printf("filtering (method 1)\n");
		value = func[0];
		for (i = 0; i < dataSize ; i++){
			value = (value * values[0]) + func[i];
			value = value / (values[0]+1.0);
			func[i] = value;
		}
		break;
	case 12:
		printf("adding bulge\n");
		tmpi2 = (int)values[0] - ((int)values[1] / 2); /* start value*/
		while (tmpi2 < 0) tmpi2 = tmpi2 + dataSize;
		for (i = 0; i < (int)values[1]; i++){
			tmpf3 = ((M_PI*2) / values[1] * i) - M_PI_2;
			value = sin(tmpf3) + 1.0;
			value = value * values[2] / 2; 
			func[tmpi2] = func[tmpi2] + value;
			tmpi2++;
			if (tmpi2 >= dataSize) tmpi2 = 0;
		}
		break;
	case 13:
		printf("Clearing Region\n");
		tmpi2 = (int)values[0];/* A */
		tmpi3 = (int)values[1];/* B */ 
		tmpi4 = tmpi3 - tmpi2;
		if (tmpi4 < 0) tmpi4 = tmpi4 + dataSize;
		tmpi5 = tmpi2;
		tmpf2 = func[tmpi2];
		tmpf3 = func[tmpi3];
		tmpf4 = tmpf3 - tmpf2;
		for (i = 0; i < tmpi4 ; i++){
			func[tmpi5] = tmpf2 + (tmpf4*((double)i)/((double)tmpi4));
			tmpi5++;
			if (tmpi5 >= dataSize)
				tmpi5 = 0;
		}
		break;
	case 14:
		printf("Scaling\n");
		tmpi2 = (int)values[1];
		tmpi3 = (int)values[2];
		tmpi4 = tmpi2;
		do{
			func[tmpi4] = func[tmpi4] * values[0];
			tmpi4++;
			if (tmpi4 >= dataSize)
				tmpi4 = 0;
		}while(tmpi4 != tmpi3);
		break;
	default:
		printf("ERROR: function not implemented");
		return;
	}
	printf("Start-End jump = %.4f\n",func[dataSize-1]-func[0]);

	/* Force it to write :) */
	pPvt->writeNeeded = 1;
	epicsEventSignal(pPvt->writeEvent);
	SendStart(pPvt);

}

void DAQmxGenP(char * params)
{
	DAQmxGen(g_PrevGenPort.c_str(), g_PrevGenChan,params);
}


void DAQepicsExitFunc(void * param)
{
    /* */
    daqMxMessage msg = msgStop;
    daqMxPvt *pPvt = (daqMxPvt*)param;
    if (!pPvt) return;
    
    globalshutdown = 1;
    
    while (pPvt->state != idle && pPvt->state != unconfigured)
    {
	asynPrint( pPvt->pasynUser, ASYN_TRACE_FLOW, "waiting for NI to Stop (portname=%s)...\n",pPvt->portName);

	epicsMutexLock(pPvt->lock);
	if( epicsMessageQueueTrySend( pPvt->msgQid, &msg, sizeof(daqMxMessage) ) != 0)
	{
		asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
					"dmbWrite: failed to send message %d\n",msg);
	}	
	epicsEventSignal(pPvt->msgEvent);

	epicsMutexUnlock(pPvt->lock);

	epicsThreadSleep(DEFAULT_WAIT_DELAY);
    }

    asynPrint( pPvt->pasynUser, ASYN_TRACE_FLOW, "NI stopped (portname=%s)\n",pPvt->portName);

}
int DAQmxReset(char * devicename)
{
    daqMxPvt *pPvt = NULL;
    daqMxMessage msg = msgConfigure;
    char daqMxErrBuf[256];

    printf("Resetting device %s\n",devicename);
    
    if (DAQmxFailed( DAQmxResetDevice(devicename))){
	DAQmxGetExtendedErrorInfo(daqMxErrBuf, ERR_BUF_SIZE);
	printf("### ERROR (ResetDevice): %s\n",daqMxErrBuf);
	return -1;
    }
    
    ellAdd(&daqMxDeviceList, (ELLNODE*)pPvt);

    pPvt = (daqMxPvt*)ellFirst(&daqMxDeviceList);
    while (pPvt)
    {
	epicsMutexLock(pPvt->lock);
	if (pPvt->state != unconfigured){
	    if( epicsMessageQueueTrySend( pPvt->msgQid, &msg, sizeof(daqMxMessage) ) != 0)
	    {
		asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
				"dmbWrite: failed to send message %d\n",msg);
	    }	
	}
	epicsEventSignal(pPvt->msgEvent);
	epicsMutexUnlock(pPvt->lock);
	pPvt = (daqMxPvt*) ellNext((ELLNODE*)pPvt);
    }
    return 0;
    
}

daqMxPvt *getAsynPort(char *portName )
{

	daqMxPvt *pPvt;
	int status;
	epicsThreadStackSizeClass mediumStack = epicsThreadStackMedium;
	unsigned int threadStackSize = 0;
	char threadName[80];

	pPvt = (daqMxPvt*) ellFirst(&daqMxDeviceList);
	/* run through the linked list to find a node with the
	 * same asyn port name */
	while (pPvt != NULL)
	{
		if( strcmp(portName, pPvt->portName) == 0) break;
		pPvt = (daqMxPvt*) ellNext((ELLNODE*)pPvt);
	}
	/* if we can't find the asyn port in the linked list, it must be 
	 * because we are trying to create a whole new port - perhaps a second
	 * device... In any case: create a new driver structure! 
	 * Be careful with creating multiple driver setups for one single device
	 * since NI HW does not support multiple setups across features. */
	if (pPvt == NULL)
	{
		pPvt = new daqMxPvt();
		if (pPvt == NULL) return NULL;
		pPvt->portName = portName;
		pPvt->nChannels = 0;
		pPvt->nSamples = DEFAULT_NSAMPLES;
		pPvt->state = unconfigured;
		pPvt->daqMode = NotDefined;
		pPvt->rawData = NULL;
		pPvt->prevData = NULL;
		pPvt->rawDataSize = 0;

		pPvt->sampleRate = DEFAULT_SAMPLE_RATE;
		pPvt->timeout = DEFAULT_TIMEOUT;

		/* The default mode is a non-polled continuous non-monster mode
		 We call this Normal mode - driver reads as fast as can but uses finite sampling*/
		pPvt->trigMode = continuous;
		pPvt->monstermode = 0; /* disabled by default */
		pPvt->polled = 0; /* temporarily enabled as default until I can write config options*/
		pPvt->pollnow = epicsEventCreate(epicsEventEmpty);
		pPvt->polldone = epicsEventCreate(epicsEventEmpty);

		pPvt->triggerEnabled = 0;
		pPvt->triggerLevel = 5.0;
		pPvt->triggerSlope = slopeRising;
		pPvt->triggerPreSamples = 0;
		pPvt->triggerDevice = NULL;
		pPvt->triggerType = Digital;
		pPvt->softTriggerMode = 0;
		pPvt->softTriggerChannel = 0;
		pPvt->softTriggerPos = 0;
		pPvt->ciPvt = NULL;

		pPvt->clockSource = (char*)malloc(strlen(OnboardClock)+1);
		strcpy(pPvt->clockSource, OnboardClock);

		pPvt->counterDevName = NULL;
		pPvt->counterIdleState = 0; /* 0=low 1=high*/
		pPvt->counterDelay = 0;
		pPvt->counterDutyCycle = 0.5;
		pPvt->counterCI = 0;
		pPvt->counterEdge = 0;
		pPvt->counterDirection = 0;
		pPvt->counterRead = 0;

		pPvt->noRestartTask = 0;

		pPvt->msgEvent = epicsEventCreate(epicsEventEmpty);

		pPvt->writeEvent = epicsEventCreate(epicsEventEmpty);
		pPvt->writeNeeded = 0;

		/* create task here */
		if (DAQmxFailed( DAQmxCreateTask( pPvt->portName, &pPvt->taskHandle )))
		{
			DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
			printf("### ERROR DAQmx: %s\n", pPvt->daqMxErrBuf);
			delete pPvt;
			return NULL;
		}
		/* create an epics mutex to protect read/writes in the simulation thread */
		pPvt->lock = epicsMutexCreate();
		
		/* Create asyn user */
		pPvt->pasynUser = pasynManager->createAsynUser(0, 0);

		/* Link with higher level routines */
		pPvt->common.interfaceType = asynCommonType;
		pPvt->common.pinterface  = (void *)&dmbCommon;
		pPvt->common.drvPvt = pPvt;
		pPvt->int32.interfaceType = asynInt32Type;
		pPvt->int32.pinterface  = (void *)&dmbInt32;
		pPvt->int32.drvPvt = pPvt;
		pPvt->float64.interfaceType = asynFloat64Type;
		pPvt->float64.pinterface  = (void *)&dmbFloat64;
		pPvt->float64.drvPvt = pPvt;
		pPvt->int32Array.interfaceType = asynInt32ArrayType;
		pPvt->int32Array.pinterface  = (void *)&dmbInt32Array;
		pPvt->int32Array.drvPvt = pPvt;
		pPvt->float64Array.interfaceType = asynFloat64ArrayType;
		pPvt->float64Array.pinterface  = (void *)&dmbFloat64Array;
		pPvt->float64Array.drvPvt = pPvt;
		pPvt->uint32Digital.interfaceType = asynUInt32DigitalType;
		pPvt->uint32Digital.pinterface = (void *)&dmbUInt32Digital;
		pPvt->uint32Digital.drvPvt = pPvt;
		pPvt->drvUser.interfaceType = asynDrvUserType;
		pPvt->drvUser.pinterface = (void *)&dmbDrvUser;
		pPvt->drvUser.drvPvt = pPvt;
		
		/* registerport */
		status = pasynManager->registerPort(pPvt->portName,
											ASYN_MULTIDEVICE,
											1, /* autoconnect */
											0, /* medium priority */
											0); /* default stacksize */
		if (status != asynSuccess)
		{
			errlogPrintf("### ERROR: Can't register myself (port: %s)\n", portName);
			return NULL;
		}

		/* registerInterface */
		status = pasynManager->registerInterface(pPvt->portName, &pPvt->common);
		if (status != asynSuccess) 
		{
			errlogPrintf("### ERROR:Can't register common. (port: %s)\n",pPvt->portName);
			return NULL;
		}

		/* initializes */
		status = pasynInt32Base->initialize(pPvt->portName, &pPvt->int32);
		if (status != asynSuccess) 
		{
			errlogPrintf("### ERROR: Can't register int32. (port: %s)\n",pPvt->portName);
			return NULL;
		}

		status = pasynFloat64Base->initialize(pPvt->portName, &pPvt->float64);
		if (status != asynSuccess) 
		{
			errlogPrintf("### ERROR: Can't register float64.\n");
			return NULL;
		}

		status = pasynInt32ArrayBase->initialize(pPvt->portName, &pPvt->int32Array);
		if (status != asynSuccess) 
		{
			errlogPrintf("### ERROR: Can't register int32Array. (port: %s)\n",pPvt->portName);
			return NULL;
		}

		status = pasynFloat64ArrayBase->initialize(pPvt->portName, &pPvt->float64Array);
		if (status != asynSuccess) 
		{
			errlogPrintf("### ERROR: Can't register float64Array. (port: %s)\n",pPvt->portName);
			return NULL;
		}

		status = pasynUInt32DigitalBase->initialize(pPvt->portName, &pPvt->uint32Digital);
		if (status != asynSuccess) 
		{
			errlogPrintf("mcaSIS3820Config: Can't register uint32Digital.\n");
			return NULL;
		}

		/* register interrupt source here (Not needed in simulation though) */
		pasynManager->registerInterruptSource(	pPvt->portName, &pPvt->int32,
												&pPvt->int32InterruptPvt);

		pasynManager->registerInterruptSource(	pPvt->portName, &pPvt->float64,
												&pPvt->float64InterruptPvt);

		pasynManager->registerInterruptSource(	pPvt->portName, &pPvt->int32Array,
												&pPvt->int32ArrayInterruptPvt);



		pasynManager->registerInterruptSource(	pPvt->portName, &pPvt->float64Array,
												&pPvt->float64ArrayInterruptPvt);


		pasynManager->registerInterruptSource(	portName, &pPvt->uint32Digital,
												&pPvt->uint32DigitalInterruptPvt);

		/* Register pasynManager interface */
		status = pasynManager->registerInterface(pPvt->portName, &pPvt->drvUser);
		if (status != asynSuccess) 
		{
			errlogPrintf("### ERROR: Can't register drvUser\n");
			return NULL;
		}
			
		/* Connect pasynUser to device for debugging */
		status = pasynManager->connectDevice(pPvt->pasynUser, pPvt->portName, 0);
		if (status != asynSuccess) 
		{
			errlogPrintf("### ERROR: connectDevice failed for scaler-sim\n");
			return NULL;
		}
		
		/* create a message queue to send commands to the thread */
		pPvt->msgQid = epicsMessageQueueCreate( MESSAGE_Q_CAPACITY, sizeof(daqMxMessage));
		
		/* create the data acquisition thread here the thread will start in iocinit */
		sprintf(threadName, "DAQmx-%s", pPvt->portName);
		threadStackSize = epicsThreadGetStackSize(mediumStack);
		pPvt->threadId = epicsThreadCreate(	threadName, 
											epicsThreadPriorityLow, 
											threadStackSize, 
											daqThread,
											(void*)pPvt);
		if(pPvt->threadId == NULL)
		{
			errlogPrintf("### ERROR: could not create thread for port: %s\n", pPvt->portName);
			return( NULL );
		}

		ellAdd(&daqMxDeviceList, (ELLNODE*)pPvt);

		/* Maybe we can only do this once and then just scan the DeviceList - would maybe be easier - can stop everything at once then*/
		epicsAtExit(DAQepicsExitFunc,(void*)pPvt);
	}
	return pPvt;
}

static asynStatus setChannelPointers(daqMxPvt *pPvt)
{
    asynStatus result = asynSuccess;
    int i;
    epicsFloat64* d;
    epicsInt32* ptr;
    int offset = 0;


    epicsMutexLock(pPvt->lock);
    if (pPvt->daqMode == AI){
	if (pPvt->softTriggerMode != 0)
	    offset = pPvt->softTriggerPos;
    }

    if ((pPvt->daqMode == AI) || (pPvt->daqMode == AO))
    {
	
	d = (epicsFloat64*)pPvt->prevData;
	for (i = 0; i < pPvt->nChannels; i++)
	{

	    if (pPvt->prevData){
		pPvt->aioPvt[i]->data = (d + (i*pPvt->nSamples) + offset);
 		pPvt->aioPvt[i]->dataSize = pPvt->nSamples - offset;
	    }else{
		pPvt->aioPvt[i]->data = NULL;
		pPvt->aioPvt[i]->dataSize = 0;
		result = asynError;
	    }
	}
    }
    else if ((pPvt->daqMode == BI)||(pPvt->daqMode == BO))
    {
	ptr = (epicsInt32*)pPvt->prevData;
	for (i = 0; i < pPvt->nChannels; i++)
	{
	    if (pPvt->prevData){
		pPvt->bioPvt[i]->data = (ptr + (i*pPvt->nSamples));
		pPvt->bioPvt[i]->dataSize = pPvt->nSamples;
	    }else{
		pPvt->bioPvt[i]->data = NULL;
		pPvt->bioPvt[i]->dataSize = 0;
		result = asynError;
	    }
	}
    }else if (pPvt->daqMode == CI){
	if ((!pPvt->ciPvt) || (!pPvt->prevData)){
	    pPvt->ciPvt->data = NULL;
	    pPvt->ciPvt->dataSize = 0;
	}else{
	    pPvt->ciPvt->data = pPvt->prevData;
	    pPvt->ciPvt->dataSize = pPvt->nSamples;
	}
    }else if (pPvt->daqMode == CO){
	printf("BEEP\n");
    }else{
	
	result = asynError;
    }

    epicsMutexUnlock(pPvt->lock);
    return result;
}

/* Allocates memory for the buffers for each channel + the buffer with the raw data.
 * If memory has already been allocated beforehand, it will be freed before new memory
 * is allocated.
 * Returns asynSuccess or asynError 
 */
static asynStatus allocBuffers(daqMxPvt *pPvt, asynUser *pasynUser)
{

    size_t sampleWidth;


    asynStatus result = asynSuccess;

    epicsMutexLock(pPvt->lock);
    /* Note that no returns are allowed inside this code
     set result to change exit code*/

    asynPrint(pasynUser, ASYN_TRACE_FLOW,"port %s: Allocating buffers now (nChannels=%d nSamples=%d)\n",pPvt->portName,pPvt->nChannels,pPvt->nSamples);
	
	if (pPvt->daqMode == AI || pPvt->daqMode == AO)
	{
		sampleWidth = sizeof( epicsFloat64 );

		/* I think this is the correct way of calculating this */
		pPvt->totalNSamples = pPvt->nSamples * pPvt->nChannels;

		/* allocate memory for the raw buffer */
		if ((pPvt->rawDataSize < pPvt->totalNSamples) || (pPvt->rawData == NULL)){
		    if (pPvt->rawData != NULL){
			delete pPvt->rawData;
			pPvt->rawData = NULL;
		    }
		    if (pPvt->prevData != NULL){
			free(pPvt->prevData);
			pPvt->prevData = NULL;
		    }
		    pPvt->rawData =  calloc(pPvt->totalNSamples, sampleWidth);
		    /*pPvt->rawDataF = (epicsFloat64*)pPvt->rawData;
		      pPvt->rawDataI = (epicsInt32*)pPvt->rawData;*/
		    if (pPvt->rawData == NULL)
		    {
			asynPrint(	pasynUser, ASYN_TRACE_ERROR, 
					"### ERROR: could not allocate mem for rawData (%d chans)\n", pPvt->nChannels);
			pPvt->nChannels = 0;
			result =  asynError;
		    }
		    if (pPvt->daqMode == AI)
			pPvt->prevData = calloc(pPvt->totalNSamples, sampleWidth);
		    else
			pPvt->prevData = pPvt->rawData;
		    if (pPvt->prevData == NULL)
		    {
			asynPrint(	pasynUser, ASYN_TRACE_ERROR, 
					"### ERROR: could not allocate mem for prevData (%d chans)\n", pPvt->nChannels);
			pPvt->nChannels = 0;
			result =  asynError;
		    }

		    pPvt->rawDataSize = pPvt->totalNSamples;
		}

	}
	else if ((pPvt->daqMode == BI)||(pPvt->daqMode == BO))
	{
	    sampleWidth = sizeof( epicsInt32 );

		/* I think this is the correct way of calculating this */
		pPvt->totalNSamples = pPvt->nSamples * pPvt->nChannels;

		/* allocate memory for the raw buffer */
		if ((pPvt->rawDataSize < pPvt->totalNSamples) || (pPvt->rawData == NULL)){
		    if (pPvt->rawData != NULL){
			free(pPvt->rawData);
			pPvt->rawData = NULL;
		    }
		    if (pPvt->prevData != NULL){
			free(pPvt->prevData);
			pPvt->prevData = NULL;
		    }
		    pPvt->rawData =  calloc(pPvt->totalNSamples, sampleWidth);
		    /*pPvt->rawDataF = (epicsFloat64*)pPvt->rawData;
		      pPvt->rawDataI = (epicsInt32*)pPvt->rawData;*/
		    if (pPvt->rawData == NULL)
		    {
			asynPrint(	pasynUser, ASYN_TRACE_ERROR, 
					"### ERROR: could not allocate mem for rawData (%d chans)\n", pPvt->nChannels);
			pPvt->nChannels = 0;
			result =  asynError;
		    }
		    if (pPvt->daqMode == BI)
			pPvt->prevData =  calloc(pPvt->totalNSamples, sampleWidth);
		    else
			pPvt->prevData = pPvt->rawData;
		    if (pPvt->prevData == NULL)
		    {
			asynPrint(	pasynUser, ASYN_TRACE_ERROR, 
					"### ERROR: could not allocate mem for prevData (%d chans)\n", pPvt->nChannels);
			pPvt->nChannels = 0;
			result =  asynError;
		    }
		    pPvt->rawDataSize = pPvt->totalNSamples;
		}

	}else if (pPvt->daqMode == CI){
	    if ((pPvt->counterRead == 0)||(pPvt->counterRead == 1))
	    { 
		sampleWidth = sizeof(epicsFloat64);
	    }else if ((pPvt->counterRead == 2)||(pPvt->counterRead == 3)){
		sampleWidth = sizeof(epicsFloat64);
	    }else{
		printf("### ERROR allocBuffers - counterRead invalid!\n");
		sampleWidth = 0;
		result = asynError;
	    }
	    if (pPvt->nChannels != 1){
		printf("### ERROR - daqmode = COUNTER but nChannels != 1 - CODE ERROR\n");
		result = asynError;
	    }
	    pPvt->totalNSamples = pPvt->nSamples;
	    if (pPvt->rawData != NULL){
		free(pPvt->rawData);
		pPvt->rawData = NULL;
	    }
	    if (pPvt->prevData != NULL){
		free(pPvt->prevData);
		pPvt->prevData = NULL;
	    }
	    if (sampleWidth){
		pPvt->rawData =  calloc(pPvt->totalNSamples, sampleWidth);
 		if (pPvt->rawData == NULL)
		{
		    asynPrint(	pasynUser, ASYN_TRACE_ERROR, 
				"### ERROR: could not allocate mem for rawData\n");
		    pPvt->nChannels = 0;
		    result =  asynError;
		}
		pPvt->prevData =  calloc(pPvt->totalNSamples, sampleWidth);
 		if (pPvt->prevData == NULL)
		{
		    asynPrint(	pasynUser, ASYN_TRACE_ERROR, 
				"### ERROR: could not allocate mem for prevData\n");
		    pPvt->nChannels = 0;
		    result =  asynError;
		}
	    }
	    
	}else if (pPvt->daqMode == CO){
	    if (pPvt->rawData != NULL){
		free(pPvt->rawData);
		pPvt->rawData = NULL;
	    }
	    if (pPvt->prevData != NULL){
		free(pPvt->prevData);
		pPvt->prevData = NULL;
	    }
	    
	    pPvt->rawData = NULL;
	    pPvt->prevData = NULL;
	}else 
	{
		asynPrint( pasynUser, ASYN_TRACE_ERROR, "### ERROR: unknown daqMode: %d\n", pPvt->daqMode);
		result = asynError;
	}
	epicsMutexUnlock(pPvt->lock);

	if (setChannelPointers(pPvt) != asynSuccess)
	    result = asynError;

	return( result );
}


int PortOptions(daqMxPvt *pPvt, int Channelnr, char * options)
{
	char * token;

	if (pPvt == NULL) return -1;
	if (Channelnr < 0) return -1;
	if (Channelnr >= pPvt->nChannels) return -1;

	token = strtok(options," ");

	while (token)
	{
		printf("Option = %s\n",token);

		if (epicsStrCaseCmp(token,"TerminalDef") == 0)
		{
			if (pPvt->aioPvt[Channelnr]) 
				pPvt->aioPvt[Channelnr]->terminal = DAQmx_Val_Cfg_Default;
		}else if (epicsStrCaseCmp(token,"TerminalRSE") == 0){
			if (pPvt->aioPvt[Channelnr]) 
				pPvt->aioPvt[Channelnr]->terminal = DAQmx_Val_RSE;
		}else if (epicsStrCaseCmp(token,"TerminalNRSE") == 0){
			if (pPvt->aioPvt[Channelnr]) 
				pPvt->aioPvt[Channelnr]->terminal = DAQmx_Val_NRSE;
		}else if (epicsStrCaseCmp(token,"TerminalDiff") == 0){
			if (pPvt->aioPvt[Channelnr]) 
				pPvt->aioPvt[Channelnr]->terminal = DAQmx_Val_Diff;
			/*	    }else if (epicsStrCaseCmp(token,"Polled") == 0){
			pPvt->polled = 1;
			}else if (epicsStrCaseCmp(token,"NotPolled") == 0){
			pPvt->polled = 0;*/
		}else if (epicsStrCaseCmp(token,"MONSTER") == 0){
			pPvt->monstermode = 1;
		}else if (epicsStrCaseCmp(token,"OneShot") == 0){
			pPvt->trigMode = oneshot;
			pPvt->polled = 1; /* default option */
		}else if (epicsStrCaseCmp(token,"Continuous") == 0){
			pPvt->trigMode = continuous;
			pPvt->polled = 0; /* defaults to this */
		}else if (epicsStrCaseCmp(token,"TriggerRising") == 0){
			pPvt->triggerEnabled = 1;
			pPvt->triggerSlope = slopeRising;
			asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,"Port %s enabled trigger  (rising)\n",pPvt->portName);
		}else if (epicsStrCaseCmp(token,"TriggerFalling") == 0){
			pPvt->triggerEnabled = 1;
			pPvt->triggerSlope = slopeFalling;
			asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,"Port %s enabled trigger  (falling)\n",pPvt->portName);
		}else if (epicsStrCaseCmp(token,"TriggerDig") == 0){
			pPvt->triggerType = Digital;
		}else if (epicsStrCaseCmp(token,"TriggerAnlg") == 0){
			pPvt->triggerType = Analog;
		}else if (epicsStrCaseCmp(token,"IdleLow") == 0){
			pPvt->counterIdleState = 0;
		}else if (epicsStrCaseCmp(token,"IdleHigh") == 0){
			pPvt->counterIdleState = 1;
		}else if (epicsStrCaseCmp(token,"CIPeriod") == 0){
			pPvt->counterCI = 1;
			if (pPvt->nSamples == 1)
				pPvt->counterRead = 1;
			else
				pPvt->counterRead = 0;
		}else if (epicsStrCaseCmp(token,"CICountEdges") == 0){
			pPvt->counterCI = 2;
			if (pPvt->nSamples == 1)
				pPvt->counterRead = 2;
			else
				pPvt->counterRead = 3;
			/* For buffered even counting CReadU32 is needed with a clocksource*/
		}else if (epicsStrCaseCmp(token,"CIPulseWidth") == 0){
			pPvt->counterCI = 3;
			if (pPvt->nSamples == 1)
				pPvt->counterRead = 1;
			else
				pPvt->counterRead = 0;
		}else if (epicsStrCaseCmp(token,"CountRising") == 0){
			pPvt->counterEdge = 0;
		}else if (epicsStrCaseCmp(token,"CountFalling") == 0){
			pPvt->counterEdge = 1;
		}else if (epicsStrCaseCmp(token,"CountUp") == 0){
			pPvt->counterDirection = 0;
		}else if (epicsStrCaseCmp(token,"CountDown") == 0){
			pPvt->counterDirection = 1;
		}else if (epicsStrCaseCmp(token,"CountExt") == 0){
			pPvt->counterDirection = 2;
		}else if (epicsStrCaseCmp(token,"CReadF64") == 0){
			pPvt->counterRead = 0;
		}else if (epicsStrCaseCmp(token,"CReadScalarF64") == 0){
			pPvt->counterRead = 1;
		}else if (epicsStrCaseCmp(token,"CReadScalarU32") == 0){
			pPvt->counterRead = 2;
		}else if (epicsStrCaseCmp(token,"CReadU32") == 0){
			pPvt->counterRead = 3;
		}else if (epicsStrCaseCmp(token,"xx") == 0){

		}else{
			switch(*token){
			case 'm': /*min*/
				token++; token++; /* removes the m= */
				if (pPvt->daqMode == CI){
					if (pPvt->ciPvt)
						pPvt->ciPvt->min = (epicsFloat64)atof(token);
				}else{
					if (pPvt->aioPvt[Channelnr]){
						pPvt->aioPvt[Channelnr]->min = (epicsFloat64)atof(token);
						asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
							"Port %s: Channel %d: min value = %.2f\n",pPvt->portName ,Channelnr ,pPvt->aioPvt[Channelnr]->min);
					}
				}
				break;
			case 'M': /*Max*/
				token++; token++; /* removes the M= */
				if (pPvt->daqMode == CI){
					if (pPvt->ciPvt)
						pPvt->ciPvt->max = (epicsFloat64)atof(token);
				}else{
					if (pPvt->aioPvt[Channelnr]){
						pPvt->aioPvt[Channelnr]->max = (epicsFloat64)atof(token);
						asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
							"Port %s: Channel %d: Max value = %.2f\n",pPvt->portName ,Channelnr ,pPvt->aioPvt[Channelnr]->max);
					}
				}
				break;
			case 'F': /* Frequency */
				token++; token++; /* removes the F= */
				pPvt->sampleRate = (epicsFloat64)atof(token);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
					"Port %s: Sample Frequency = %.2f (Hz)\n",pPvt->portName ,pPvt->sampleRate);
				break;
			case 'N': /*N samples*/
				token++; token++;
				pPvt->nSamples = (epicsInt32)atoi(token);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
					"Port %s: Number of Samples = %d (Hz)\n",pPvt->portName ,pPvt->nSamples);
				break;
			case 'T': /*Trigger Level*/
				token++;  token++;
				pPvt->triggerLevel = (epicsFloat64)atof(token);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
					"Port %s: Trigger Level = %.3f (V)\n",pPvt->portName ,pPvt->triggerLevel);
				break;
			case 'p': /* Ref trigger preSamples */
				token++; token++;
				pPvt->triggerPreSamples = (epicsInt32)atoi(token);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
					"Port %s: Ref Trigger preSamples = %d \n",pPvt->portName ,pPvt->triggerPreSamples);
				break;
			case 'S': /* Soft trigger mode*/
				/*  should replace this with 5 different commands for 0-4 values!*/
				token++; token++;
				pPvt->softTriggerMode = (epicsInt32)atoi(token);
				if ((pPvt->softTriggerMode < 0)||(pPvt->softTriggerMode > 4))
					pPvt->softTriggerMode = 0;
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
					"Port %s: Soft Trigger Mode = %d \n",pPvt->portName ,pPvt->softTriggerMode);
				break;
			case 's': /* soft trigger channel number*/
				token++; token++;
				pPvt->softTriggerChannel = (epicsInt32)atoi(token);
				break;
			case 'C': /* Clock Source*/
				token++; token++;
				if (pPvt->clockSource){
					free(pPvt->clockSource);
					pPvt->clockSource = NULL;
				}
				pPvt->clockSource = (char*)malloc(strlen(token)+1);
				strcpy(pPvt->clockSource,token);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
					"Port %s: Clock Source = %s \n",pPvt->portName ,pPvt->clockSource);

				break;
			case 'D': /* Counter Duty Cycle*/
				token++; token++;
				pPvt->counterDutyCycle = (epicsFloat64)atof(token);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
					"Port %s: Counter Duty Cycle = %.3f \n",pPvt->portName ,pPvt->counterDutyCycle);
				break;
			case 'd': /* (Counter) delay */
				token++; token++;
				pPvt->counterDelay = (epicsFloat64)atof(token);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, 
					"Port %s: (Counter) Delay = %.3f \n",pPvt->portName ,pPvt->counterDelay);
				break;
			}
		}
		token = strtok(NULL," ");
	}

	return 0;
}

void DAQmxPortOptions(char * portName, int Channelnr, char * options)
{
    daqMxPvt * pPvt; 
    
    daqMxMessage msg = msgConfigure;
    
    pPvt = (daqMxPvt*) ellFirst(&daqMxDeviceList);
    /* run through the linked list to find a node with the
     * same asyn port name */
    while (pPvt != NULL)
    {
 	if( strcmp(portName, pPvt->portName) == 0) break;
	pPvt = (daqMxPvt*) ellNext((ELLNODE*)pPvt);
    }
	
    if (pPvt == NULL){
	printf("ERROR: Can't find port %s\n",portName);
	return;
    }

    if (Channelnr >= pPvt->nChannels){
	printf("ERROR: Invalid Channelnr (number of channels in port: %d)\n",pPvt->nChannels);
	return;
    }

    if (PortOptions(pPvt,Channelnr, options) != 0){
	printf("ERROR: Options wrong or something\n");
	return;
    }
    /* send reconfigure message  if needed */
    
    if (pPvt->state == unconfigured) return;
   
    epicsMutexLock(pPvt->lock);
    if (pPvt->state != unconfigured){
	if( epicsMessageQueueTrySend( pPvt->msgQid, &msg, sizeof(daqMxMessage) ) != 0)
	{
	    asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
			"dmbWrite: failed to send message %d\n",msg);
	}	
    }
    epicsEventSignal(pPvt->msgEvent);
    epicsMutexUnlock(pPvt->lock);

}
int DAQmxConfig(char *portName, char * deviceName, int Channelnr, char * sacqType, char* options)
{
        int acqType;
	daqMxPvt * pPvt; 
	int stringLength, i;
	char * tmpStrName;


	/* Find acqType */
	if (strcmp(sacqType,"AI") == 0){
	    acqType = AI;
	}else if (strcmp(sacqType,"AO") == 0){
	    acqType = AO;
	}else if (strcmp(sacqType,"BI") == 0){
	    acqType = BI;
	}else if (strcmp(sacqType,"BO") == 0){
	    acqType = BO;
	}else if (strcmp(sacqType,"COUNTER") == 0){
	    acqType = CI;
	}else if (strcmp(sacqType,"CO") == 0){
	    acqType = CO;
	}else{
	    printf("### ERROR: Invalid acquisition mode\n");
	    return -1;
	}
		
	stringLength = strlen(portName);
	tmpStrName = new char(stringLength + 1);
	tmpStrName[stringLength] = 0;
	strcpy(tmpStrName, portName);
 	
	pPvt = getAsynPort( tmpStrName );
	if (pPvt == NULL)
	{
		printf("### ERROR: could not get driver structure for asynport: %s\n", tmpStrName);
		return -1;
	}
	
	if (acqType > MAX_ACQ_TYPES || acqType < MIN_ACQ_TYPES)
	{
		printf("### ERROR: %d is not a valid acquisition type.\n", acqType);
		return -1;
	}
	if (pPvt->daqMode == 0)
	{
		pPvt->daqMode = (daqMxAcqMode)acqType;
		printf("Setting daqMode on %s = %s\n",tmpStrName,sacqType);
	} else if (pPvt->daqMode != (daqMxAcqMode)acqType)
	{
		printf(	"### ERROR: asynport \"%s\" has acqType: %d You can not have multiple daqTypes on one asynport!\n",
				pPvt->portName, pPvt->daqMode);
		return -1;
	}
	if (Channelnr != pPvt->nChannels){
	    printf("### ERROR: Can not set same channelnr twice! and it must be in order from 0 upwards!  (expected:%d got:%d)\n",pPvt->nChannels,Channelnr);
	    return -1;
	}

	if (pPvt->state != unconfigured){
	    printf("### ERROR: Port already used - it is not supported to add Channels at this stage (Please do all DAQmxConfig calls before startioc)\n");
	    printf("Will continue anyways... (good luck :} )\n");
	}

	switch(pPvt->daqMode)
	{
	    case AI:
	    case AO:
		    /* allocate the aioPvt struct*/
		    i = Channelnr;

		    pPvt->aioPvt[i] = new daqAioPvt();
		    if (pPvt->aioPvt[i] == NULL)
		    {
			printf("### ERROR: could not allocate mem for aioPvt\n");
			return -1;
		    }
		    pPvt->aioPvt[i]->avgData = 0;
		    pPvt->aioPvt[i]->devicename = (char *)malloc(strlen(deviceName)+1);
		    strcpy(pPvt->aioPvt[i]->devicename,deviceName);
		    /* pPvt->nSamples = DEFAULT_NSAMPLES;*/
		    pPvt->aioPvt[i]->max = DEFAULT_MAX;
		    pPvt->aioPvt[i]->min = DEFAULT_MIN;
		    pPvt->aioPvt[i]->terminal = DAQmx_Val_Cfg_Default;
		    pPvt->aioPvt[i]->data = NULL;
		    pPvt->aioPvt[i]->dataSize = 0;

		    /*pPvt->aioPvt[i]->data = (epicsFloat64*) calloc( pPvt->nSamples, sizeof(epicsFloat64));*/
		    if (pPvt->nChannels <= Channelnr)
			pPvt->nChannels = Channelnr+1;
		    pPvt->totalNSamples += pPvt->nSamples;
		    break;

		case BI:
		case BO:
		    i = Channelnr;
		    pPvt->bioPvt[i] = (daqBioPvt*)malloc(sizeof(daqBioPvt));
		    if (pPvt->bioPvt[i] == NULL)
		    {
			printf("### ERROR: could not allocate mem for bioPvt\n");
			return -1;
		    }
		    pPvt->bioPvt[i]->devicename = (char*)malloc(strlen(deviceName)+1);
		    strcpy(pPvt->bioPvt[i]->devicename,deviceName);
		    pPvt->bioPvt[i]->data = NULL;
		    pPvt->bioPvt[i]->dataSize = 0;
		    
		    if (pPvt->nChannels <= Channelnr)
			pPvt->nChannels = Channelnr+1;
		    pPvt->totalNSamples += pPvt->nSamples;
		    break;
		case CI:
		    pPvt->nChannels = 1;
		    if (!pPvt->ciPvt){
			pPvt->ciPvt = (daqCiPvt*)malloc(sizeof(daqCiPvt));
			pPvt->ciPvt->data = NULL;
			pPvt->ciPvt->dataSize = 0;
			pPvt->ciPvt->max = DEFAULT_MAX;
			pPvt->ciPvt->min = DEFAULT_MIN;
		    }
		    if (pPvt->counterDevName){
			free(pPvt->counterDevName);
			pPvt->counterDevName = NULL;
		    }
		    pPvt->counterDevName = (char*)malloc(strlen(deviceName)+1);
		    strcpy(pPvt->counterDevName, deviceName);
		    printf("Port %s = Counter (Input) dev = %s (Only 1 counter per port!)\n",pPvt->portName,pPvt->counterDevName);
		    break;
  	        case CO:
		    pPvt->nChannels = 1;
		    if (pPvt->counterDevName){
			free(pPvt->counterDevName);
			pPvt->counterDevName = NULL;
		    }
		    pPvt->counterDevName = (char*)malloc(strlen(deviceName)+1);
		    strcpy(pPvt->counterDevName, deviceName);
		    printf("Port %s = Counter Output dev = %s (Only 1 counter per port!)\n",pPvt->portName,pPvt->counterDevName);
		    break;
		default:
			printf("### ERROR: unknown daqMode: %d\n", pPvt->daqMode);
			return -1;
	}

	/* check config options */
	if (PortOptions(pPvt, Channelnr,options) != 0){
	    printf("### ERROR - Invalid options\n");
	    return -1;
	}
	
	return 0;
}

void DAQmxTrigger(char * portName, char *triggersource, char * options)
{
    daqMxPvt * pPvt; 
    
    daqMxMessage msg = msgConfigure;
    
    pPvt = (daqMxPvt*) ellFirst(&daqMxDeviceList);
    /* run through the linked list to find a node with the
     * same asyn port name */
    while (pPvt != NULL)
    {
 	if( strcmp(portName, pPvt->portName) == 0) break;
	pPvt = (daqMxPvt*) ellNext((ELLNODE*)pPvt);
    }
	
    if (pPvt == NULL){
	printf("ERROR: Can't find port %s\n",portName);
	return;
    }
    
    if (pPvt->triggerDevice)
    {
	free(pPvt->triggerDevice);
	pPvt->triggerDevice = NULL;
    }
    pPvt->triggerDevice = (char*)malloc(strlen(triggersource)+1);
    strcpy(pPvt->triggerDevice,triggersource);
    printf("port %s: Trigger source = %s\n",portName,triggersource);

    if (PortOptions(pPvt,0, options) != 0){
	printf("ERROR: Options wrong or something\n");
	return;
    }

    /* send configure message to port*/
    epicsMutexLock(pPvt->lock);
    if (pPvt->state != unconfigured){
	if( epicsMessageQueueTrySend( pPvt->msgQid, &msg, sizeof(daqMxMessage) ) != 0)
	{
	    asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
			"dmbWrite: failed to send message %d\n",msg);
	}	
    }
    epicsEventSignal(pPvt->msgEvent);
    epicsMutexUnlock(pPvt->lock);

}

void DAQmxChangeDeviceName(char * portName, int channelnr, char * newdevicename)
{
    daqMxPvt * pPvt; 
    
    daqMxMessage msg = msgConfigure;
    
    pPvt = (daqMxPvt*) ellFirst(&daqMxDeviceList);
    /* run through the linked list to find a node with the
     * same asyn port name */
    while (pPvt != NULL)
    {
 	if( strcmp(portName, pPvt->portName) == 0) break;
	pPvt = (daqMxPvt*) ellNext((ELLNODE*)pPvt);
    }
	
    if (pPvt == NULL){
	printf("ERROR: Can't find port %s\n",portName);
	return;
    }

    if (channelnr >= pPvt->nChannels){
	printf("ERROR: Invalid Channelnr (number of channels in port: %d)\n",pPvt->nChannels);
	return;
    }
    switch (pPvt->daqMode){
	case AI:
	case AO:
	    if (!pPvt->aioPvt[channelnr]){
		printf("SERIOUS ERROR: pPvt->aioPvt[channelnr] == NULL\n");
		return;
	    }
	    if (pPvt->aioPvt[channelnr]->devicename){
		printf("port %s channel %d: Changing devicename from %s to %s\n",
		       portName,channelnr,pPvt->aioPvt[channelnr]->devicename,newdevicename);
	    }else{
		printf("port %s channel %d: Changing devicename from NULL to %s \n",
		       portName,channelnr,newdevicename);
	    }
	    free(pPvt->aioPvt[channelnr]->devicename);
	    pPvt->aioPvt[channelnr]->devicename = (char*)malloc(strlen(newdevicename)+1);
	    strcpy(pPvt->aioPvt[channelnr]->devicename,newdevicename);
	    break;
	case BI:
	case BO:
	    if (!pPvt->bioPvt[channelnr]){
		printf("SERIOUS ERROR: pPvt->bioPvt[channelnr] == NULL\n");
		return;
	    }
	    if (pPvt->bioPvt[channelnr]->devicename){
		printf("port %s channel %d: Changing devicename from %s to %s\n",
		       portName,channelnr,pPvt->bioPvt[channelnr]->devicename,newdevicename);
	    }else{
		printf("port %s channel %d: Changing devicename from NULL to %s \n",
		       portName,channelnr,newdevicename);
	    }
	    free(pPvt->bioPvt[channelnr]->devicename);
	    pPvt->bioPvt[channelnr]->devicename = (char*)malloc(strlen(newdevicename)+1);
	    strcpy(pPvt->bioPvt[channelnr]->devicename,newdevicename);
	    break;
	case CO:
	case CI:
	    printf("port %s counter dev name changed to %s\n",portName, newdevicename);
	    if (pPvt->counterDevName)
		free(pPvt->counterDevName);
	    pPvt->counterDevName = (char*)malloc(strlen(newdevicename)+1);
	    strcpy(pPvt->counterDevName,newdevicename);
	    break;
	default:
	    printf("ERROR: DAQmode not supported with this command\n");
	    break;
    }
    
    /* send configure message to port*/
    epicsMutexLock(pPvt->lock);
    if (pPvt->state != unconfigured){
	if( epicsMessageQueueTrySend( pPvt->msgQid, &msg, sizeof(daqMxMessage) ) != 0)
	{
	    asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
			"dmbWrite: failed to send message %d\n",msg);
	}	
    }
    epicsEventSignal(pPvt->msgEvent);
    epicsMutexUnlock(pPvt->lock);

}

void DAQmxStart(char * portName)
{
    daqMxPvt * pPvt; 
    
    
    pPvt = (daqMxPvt*) ellFirst(&daqMxDeviceList);
    /* run through the linked list to find a node with the
     * same asyn port name */
    while (pPvt != NULL)
    {
 	if( strcmp(portName, pPvt->portName) == 0) break;
	pPvt = (daqMxPvt*) ellNext((ELLNODE*)pPvt);
    }
	
    if (pPvt == NULL){
	printf("ERROR: Can't find port %s\n",portName);
	return;
    }

    SendStart(pPvt);
    printf("Start Message sent to Port %s\n",portName);
	
}

asynStatus SoftTrigger(daqMxPvt *pPvt)
{
    asynStatus result = asynError;
    int pos = 0;
    int max = pPvt->nSamples-1;
    epicsFloat64* d;
    if (pPvt->triggerPreSamples != 0)
	max = pPvt->triggerPreSamples;


    pPvt->softTriggerPos = 0;

    if (pPvt->daqMode != AI){
	asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
			"softTrigger: Only available for AI\n");
	return asynError;
    }

    if ((pPvt->softTriggerChannel < 0)||(pPvt->softTriggerChannel >= pPvt->nChannels)){
	asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
			"softTrigger: invalid softTriggerChannel!\n");
	return asynError;
    }

    d = (epicsFloat64*)pPvt->prevData;
    /* d = d + (pPvt->softTriggerChannel * pPvt->nSamples);*/


    switch (pPvt->softTriggerMode){
	case 0: /* no soft triggering!*/
	    pPvt->softTriggerPos = 0;
	    return asynError;
	    break;
	case 1: /* first high*/
	    while (pos < max){
		if (d[pos] > pPvt->triggerLevel){
		    pPvt->softTriggerPos = pos;
		    result = asynSuccess;
		    break;
		}
		pos++;
	    }
	    break;
	case 2: /* first low*/
	    while (pos < max){
		if (d[pos] < pPvt->triggerLevel){
		    pPvt->softTriggerPos = pos;
		    result = asynSuccess;
		    break;
		}
		pos++;
	    }
	    break;
	case 3: /* rising edge*/
	    while (pos < max){
		if ((d[pos] <= pPvt->triggerLevel)&&(d[pos+1] > pPvt->triggerLevel)){
		    pPvt->softTriggerPos = pos;
		    result = asynSuccess;
		    break;
		}
		pos++;
	    }
	    break;
	case 4: /* falling edge*/
	    while (pos < max){
		if ((d[pos] >= pPvt->triggerLevel)&&(d[pos+1] < pPvt->triggerLevel)){
		    pPvt->softTriggerPos = pos;
		    result = asynSuccess;
		    break;
		}
		pos++;
	    }
	    break;
	default:
	    asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR, 
			"softTrigger: invalid softTriggerMode : %d!\n",pPvt->softTriggerMode);
	    
	    return asynError;
    }
    return result;
}

void ConfigureCI(daqMxPvt *pPvt)
{
	if (pPvt->counterCI == 1){
		if (DAQmxFailed(DAQmxCreateCIPeriodChan(pPvt->taskHandle,
			pPvt->counterDevName,NULL,
			pPvt->ciPvt->min,
			pPvt->ciPvt->max,
			DAQmx_Val_Seconds,
			((pPvt->counterEdge)?DAQmx_Val_Rising : DAQmx_Val_Falling),
			DAQmx_Val_LowFreq1Ctr,
			0,1,NULL)))
		{
			DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
			asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
				"### DAQmx ERROR (CreateCIPeriod): %s\n", pPvt->daqMxErrBuf);
			pPvt->state = unconfigured;
			return;
		}
	}else if (pPvt->counterCI == 2){
		int32 direction;
		if (pPvt->counterDirection == 1)
			direction = DAQmx_Val_CountDown;
		else if (pPvt->counterDirection == 2)
			direction = DAQmx_Val_ExtControlled;
		else
			direction = DAQmx_Val_CountUp; /* 0 and default */
		if (DAQmxFailed(DAQmxCreateCICountEdgesChan(pPvt->taskHandle,
			pPvt->counterDevName,NULL,
			((pPvt->counterEdge)?DAQmx_Val_Rising:DAQmx_Val_Falling),
			0,
			direction)))
		{
			DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
			asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
				"### DAQmx ERROR (CreateCICountEdge): %s\n", pPvt->daqMxErrBuf);
			pPvt->state = unconfigured;
			return;
		}

	}else if (pPvt->counterCI == 3){
		if (DAQmxFailed(DAQmxCreateCIPulseWidthChan(pPvt->taskHandle,
			pPvt->counterDevName,NULL,
			pPvt->ciPvt->min,
			pPvt->ciPvt->max,
			DAQmx_Val_Seconds,
			((pPvt->counterEdge)?DAQmx_Val_Rising:DAQmx_Val_Falling),
			NULL)))
		{
			DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
			asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
				"### DAQmx ERROR (CreateCIPulseWidth): %s\n", pPvt->daqMxErrBuf);
			pPvt->state = unconfigured;
			return;
		}
	}else{
		asynPrint( pPvt->pasynUser, ASYN_TRACE_ERROR,
			"### DAQmx ERROR (COUNTER): You must specifiy CI option! (%d)\n",pPvt->counterCI);
		pPvt->state = unconfigured;
		return;
	}

	if ((pPvt->counterRead == 1)||(pPvt->counterRead == 2)){
		if ((pPvt->nSamples == 1)||(pPvt->sampleRate == 0)){
			if (pPvt->counterCI == 2){
				pPvt->noRestartTask = 1;
				printf("Disabling restart\n"); /* debug code */
			}

		}
	}
}

void Configure(daqMxPvt *pPvt)
{
	int ch;
	int sampleMode = 0;
	double sampleRate = pPvt->sampleRate;
	int nSamples = pPvt->nSamples;

	if (allocBuffers(pPvt, pPvt->pasynUser) == asynError)
	{
		asynPrint(pPvt->pasynUser,ASYN_TRACE_ERROR,
			"### ERROR:(port:%s) could not allocate mem for channel buffers (%d chans)\n",pPvt->portName,pPvt->nChannels);
		pPvt->nChannels = 0;
		pPvt->state = unconfigured;
		return;
	}
	/* Maybe this configure part should become a seperate function? */

	for (ch= 0 ; ch < pPvt->nChannels; ch++){
		switch(pPvt->daqMode){
		case AI:
			/*printf("SETTING UP %s CHANNEL\n",pPvt->aioPvt[ch]->devicename);*/
			if( DAQmxFailed( DAQmxCreateAIVoltageChan( 	pPvt->taskHandle, 
				pPvt->aioPvt[ch]->devicename, 
				NULL, 
				pPvt->aioPvt[ch]->terminal,
				pPvt->aioPvt[ch]->min, 
				pPvt->aioPvt[ch]->max,
				DAQmx_Val_Volts,
				NULL) ))
			{
				DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
					"### DAQmx ERROR (CreateAI): %s\n", pPvt->daqMxErrBuf);
				pPvt->state = unconfigured;
				break;
			}
			break;
		case AO:
			if (DAQmxFailed( DAQmxCreateAOVoltageChan( pPvt->taskHandle,
				pPvt->aioPvt[ch]->devicename,
				NULL,
				pPvt->aioPvt[ch]->min,
				pPvt->aioPvt[ch]->max,
				DAQmx_Val_Volts,
				NULL )))
			{
				DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
					"### DAQmx ERROR (CreateAO): %s\n", pPvt->daqMxErrBuf);
				pPvt->state = unconfigured;
				break;
			}
			break;
		case BI:
			if (DAQmxFailed ( DAQmxCreateDIChan( pPvt->taskHandle,
				pPvt->bioPvt[ch]->devicename,
				NULL,
				DAQmx_Val_ChanForAllLines)))
			{
				DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
					"### DAQmx ERROR (CreateDI): %s\n", pPvt->daqMxErrBuf);
				pPvt->state = unconfigured;
				break;
			}
			break;
		case BO:
			if (DAQmxFailed ( DAQmxCreateDOChan(pPvt->taskHandle,
				pPvt->bioPvt[ch]->devicename,
				NULL,
				DAQmx_Val_ChanForAllLines)))
			{
				DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
					"### DAQmx ERROR (CreateDO): %s\n", pPvt->daqMxErrBuf);
				pPvt->state = unconfigured;
				break;
			}
			break;
		case CI:
			ConfigureCI(pPvt);
			break;
		case CO:
			if (DAQmxFailed(DAQmxCreateCOPulseChanFreq(pPvt->taskHandle,
				pPvt->counterDevName,
				NULL,
				DAQmx_Val_Hz,
				((pPvt->counterIdleState) ? DAQmx_Val_High:DAQmx_Val_Low),
				pPvt->counterDelay,
				sampleRate,
				pPvt->counterDutyCycle
				)))
			{
				DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
					"### DAQmx ERROR (CreateCO): %s\n", pPvt->daqMxErrBuf);
				pPvt->state = unconfigured;
				break;
			}
			break;
		default:
			pPvt->state = unconfigured;
			asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
				"### DAQmx:configure ERROR : Unknown daqMode \n");
			break;
		}
	}
	if (pPvt->state != configure)
		return; /* something went wrong! */

	sampleMode = DAQmx_Val_FiniteSamps;
	if (pPvt->monstermode)
	{
		sampleMode = DAQmx_Val_ContSamps;
		nSamples = DEFAULT_NSAMPLES;
	}
	if ((!pPvt->clockSource) || (strcmp(pPvt->clockSource, OnboardClock)!=0))
		sampleRate = 1E6; /* maximum expected extternal clock rate */
	if ((sampleRate > 0) && (pPvt->daqMode != BO))
	{
		if (sampleMode == DAQmx_Val_ContSamps){
			asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,"port %s: Configured with continuous sampling\n",pPvt->portName);
		}
		if (!pPvt->clockSource)
		{
			asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR," ### DAQmx ERROR: pPvt->clockSource == NULL!\n");
			return;
		}
		if( DAQmxFailed( DAQmxCfgSampClkTiming( pPvt->taskHandle,
			pPvt->clockSource,
			sampleRate,
			DAQmx_Val_Rising, sampleMode,
			nSamples ) ))
		{
			DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
			asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
				"### DAQmx ERROR (CfgSampClkTiming): %s\n", pPvt->daqMxErrBuf);
			pPvt->state = unconfigured;
			return;
		}
	}else{
		asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,
			"port %s: No SampleClkTiming configuration done (this is not necessarily an error)\n",pPvt->portName);
	}
	/* I'm unsure if this should be in config or start???*/
	/* I think triggering wouldn't really work with continuous sampling
	So I'm not sure if we should lock that somehow?
	*/
	if (pPvt->triggerEnabled){
		if ((pPvt->daqMode != AI)||(pPvt->daqMode != BI)){
			asynPrint(pPvt->pasynUser,ASYN_TRACE_ERROR,
				"Disabled Trigger Automatically - Only for AI and BI modes\n");
			pPvt->triggerEnabled = 0;
		}
	}
	if (pPvt->triggerEnabled && (!pPvt->triggerDevice)){
		asynPrint(pPvt->pasynUser,ASYN_TRACE_ERROR,
			"");
		pPvt->triggerEnabled = 0;
	}
	if (pPvt->triggerEnabled && (pPvt->triggerDevice)){
		/*
		WE can make this alot simpler..
		Since if we have triggerPerSamples = 0 we might as well pass that to RefTrig and get same result as StartTrig :)
		The answer I got from NI wasn't very conclusive if this is a good idea or not:
		"
		Calling EdgeRefTrig with zero pretrigger samples will give you the same results as calling EdgeStartTrig. 
		If you do not want any pretrigger samples rather user the edge start trigger. 
		The EdgeRefTrig function is used if you want information before the trigger is received.
		"
		*/
		if (sampleMode == DAQmx_Val_ContSamps){
			asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR,"port %s: Continuous sampling and Trigger incompatible\n",pPvt->portName);
		}
		asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW, "port %s: Start Trigger\n",pPvt->portName);
		if (pPvt->triggerType == Digital){
			/*
			Note that you can have analog input with digital triggering 
			or possibly the other way around also?
			I'm unsure if you can have multiple tasks that use the same trigger?
			*/
			if (pPvt->triggerPreSamples > 0){
				if (DAQmxFailed(DAQmxCfgAnlgEdgeRefTrig(pPvt->taskHandle,
					pPvt->triggerDevice,
					(int32)pPvt->triggerSlope,
					pPvt->triggerLevel,
					pPvt->triggerPreSamples)))
				{
					DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
						"### DAQmx ERROR (AnlgRefTrig): %s\n", pPvt->daqMxErrBuf);
					pPvt->state = idle;
					return;

				}
			}else{

				if (DAQmxFailed(DAQmxCfgAnlgEdgeStartTrig(pPvt->taskHandle,
					pPvt->triggerDevice,
					(int32)pPvt->triggerSlope,
					pPvt->triggerLevel)))
				{
					DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
						"### DAQmx ERROR (AnlgStartTrig): %s\n", pPvt->daqMxErrBuf);
					pPvt->state = idle;
					return;

				}
			}
		}else{
			/*triggerType = Analog*/
			if (pPvt->triggerPreSamples > 0){
				if (DAQmxFailed(DAQmxCfgDigEdgeRefTrig(pPvt->taskHandle,
					pPvt->triggerDevice,
					(int32)pPvt->triggerSlope,
					pPvt->triggerPreSamples)))
				{
					DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
						"### DAQmx ERROR (DigRefTrig): %s\n", pPvt->daqMxErrBuf);
					pPvt->state = idle;
					return;

				}
			}else{

				if (DAQmxFailed(DAQmxCfgDigEdgeStartTrig(pPvt->taskHandle,
					pPvt->triggerDevice,
					(int32)pPvt->triggerSlope)))
				{
					DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
						"### DAQmx ERROR (DigStartTrig): %s\n", pPvt->daqMxErrBuf);
					pPvt->state = idle;
					return;

				}
			}
		}

	}

	asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, "configuration done: nSamples: %d sampleRate: %fHz\n",
		nSamples, sampleRate);
	pPvt->state = idle;
}

void daqThread(void *param)
{
	daqMxPvt * pPvt = (daqMxPvt *)param;
	/*epicsFloat64* d;*/
	void * tmpp;
	int32 samplesTransferred = 0;
	/*bool32 boolresult;*/
	int  reason, signal;
	daqMxMessage msg;
	int ErrorCode;
	daqMxState laststate = pPvt->state;

	ELLLIST *pclientList;
	interruptNode *pNode;
	asynFloat64Interrupt *pFloat64Interrupt;
	asynFloat64ArrayInterrupt *pFloat64ArrayInterrupt;
	asynInt32Interrupt *pInt32Interrupt;
	asynInt32ArrayInterrupt *pInt32ArrayInterrupt;

	asynUInt32DigitalInterrupt *pUInt32DigitalInterrupt;

	int ignoreMsg = 0;
	struct epicsTimeStamp tp;
	struct epicsTimeStamp oldtp;

	memset(&tp,0,sizeof(tp));
	memset(&tp,0,sizeof(oldtp));
	/*clock_getcpuclockid( 0, &clockId);   -  Rather just use: CLOCK_REALTIME*/
	epicsTimeGetCurrent(&oldtp);
	//clock_gettime( CLOCK_REALTIME, &oldtp);

	asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, "Started thread for port: \"%s\"\n", pPvt->portName); 

	while(1)
	{
	    if (globalshutdown)
		if ((pPvt->state == idle)||(pPvt->state == unconfigured)){
		    printf("DAQmx: Thread exiting\n");
		    return;
		}

		if (ignoreMsg != 1)
		{
			if (epicsMessageQueueTryReceive( pPvt->msgQid, &msg, sizeof(daqMxMessage) ) != -1)
			{
			    switch(msg){
				case msgNoAction:
				    break;
				case msgConfigure:
				    asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, "Thread: got message: Configure\n");
				    break;
				case msgStart:
				    asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, "Thread: got message: Start\n");
				    break;
				case msgStop:
				    asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, "Thread: got message: Stop\n");
				    break;
			    }
			} else msg = msgNoAction;
		}
		switch(msg)
		{
			case msgNoAction:
				break;
			case msgConfigure:
				if (pPvt->state == unconfigured )
				{
					pPvt->state = configure;
					ignoreMsg = 0;
					msg = msgNoAction;
					break;
				}
				if( pPvt->state == idle )
				{
					pPvt->state = reconfigure;
					ignoreMsg = 1;
					break;
				}
				if ((pPvt->state != unconfigured)&&(pPvt->state != configure))
				{
					pPvt->state = stop; /* go to stop which will then transfer to idle */
					ignoreMsg = 1;
					break;
				}
				ignoreMsg = 0;
				msg = msgNoAction;
				break;
				
			case msgStart:
				if (pPvt->state == idle || pPvt->state == start)
				{
					pPvt->state = start;
					ignoreMsg = 0;
					msg = msgNoAction;
					break;
				}
				if (pPvt->state < idle) /* this includes unconfigured*/
				{
				    /* should be reconfigure rather?? */
					pPvt->state = configure;
					ignoreMsg = 1;
					break;
				}
				if (pPvt->state > idle)
				{
					pPvt->state = stop;
					ignoreMsg = 1;
				}
				break;
				
			case msgStop:
				if ((pPvt->state != unconfigured)&&(pPvt->state != configure)&&(pPvt->state != stop)&&(pPvt->state != idle))
				{
					pPvt->state = stop;
				}
				ignoreMsg = 0;
				msg = msgNoAction;
				break;
			default:
				asynPrint( pPvt->pasynUser, ASYN_TRACE_ERROR, "Thread: unknown message: %d!\n", msg);
				break;
		} 
		
		/* If we change state, send an interrupt to the STATE command
		 * to allow the user to keep track of which state the driver is in. */
		if (laststate != pPvt->state)
		{
			/*asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, "Thread: statechange to %d!\n",pPvt->state);*/
			pasynManager->interruptStart(pPvt->uint32DigitalInterruptPvt, &pclientList);
			pNode = (interruptNode *)ellFirst(pclientList);
			while (pNode != NULL)
			{
				pUInt32DigitalInterrupt = static_cast<asynUInt32DigitalInterrupt*>(pNode->drvPvt);
				reason = pUInt32DigitalInterrupt->pasynUser->reason;
				if (reason == stateCmd)
				{
/*					pasynManager->getAddr(pPvt->pasynUser, &signal);*/
					pUInt32DigitalInterrupt->callback(	pUInt32DigitalInterrupt->userPvt,
														pUInt32DigitalInterrupt->pasynUser,
														pPvt->state);
				}
				pNode = (interruptNode *)ellNext(&pNode->node);
			}
			pasynManager->interruptEnd(pPvt->uint32DigitalInterruptPvt);
	
		}
		laststate = pPvt->state;
		
		switch(pPvt->state)
		{
			case unconfigured:
			    /*epicsThreadSleep(DEFAULT_WAIT_DELAY);*/
				epicsEventWaitWithTimeout(pPvt->msgEvent,DEFAULT_WAIT_DELAY);

				break;
			case reconfigure:
  			    DAQmxStopTask(pPvt->taskHandle); /* just for safety*/
				if( DAQmxFailed (DAQmxClearTask( pPvt->taskHandle ) ))
				{
					DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
								"### DAQmx ERROR (ClearTask): %s\n", pPvt->daqMxErrBuf);
					pPvt->state = unconfigured;
				}
				if (DAQmxFailed( DAQmxCreateTask( pPvt->portName, &pPvt->taskHandle )))
				{
					DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
								"### DAQmx ERROR (CreateTask): %s\n", pPvt->daqMxErrBuf);
					pPvt->state = unconfigured;
				}
				pPvt->state = configure;
				break;
			
			case configure:
				Configure(pPvt);
				break;
			    
			case idle:
			    /* Rather should wait for some event here - this is not optimal */
			    /*epicsThreadSleep(DEFAULT_WAIT_DELAY);*/
				epicsEventWaitWithTimeout(pPvt->msgEvent,DEFAULT_WAIT_DELAY);
				break;
				
			case start:
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_FLOW, "thread: starting task!\n");

				if( DAQmxFailed( DAQmxStartTask( pPvt->taskHandle )))
				{
					DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
							"### DAQmx ERROR (StartTask): %s\n", pPvt->daqMxErrBuf);
					pPvt->state = idle;
					break;
				}
				if (pPvt->daqMode == AI){
				    pPvt->state = acquireAnlg;
				}else if (pPvt->daqMode == BI){
				    pPvt->state = acquireDig;
				}else if (pPvt->daqMode == CI){
				    pPvt->state = acquireCnt;
				}else if (pPvt->daqMode == CO){
				    pPvt->state = counterout;
				}else{
				    pPvt->state = write;
				}

				pasynManager->interruptStart(pPvt->uint32DigitalInterruptPvt, &pclientList);
				pNode = (interruptNode *)ellFirst(pclientList);
				while (pNode != NULL)
				{
					pUInt32DigitalInterrupt = static_cast<asynUInt32DigitalInterrupt*>(pNode->drvPvt);
					reason = pUInt32DigitalInterrupt->pasynUser->reason;
					if (reason == acquireCmd)
					{
						pasynManager->getAddr(pUInt32DigitalInterrupt->pasynUser, &signal);
						pUInt32DigitalInterrupt->callback(	pUInt32DigitalInterrupt->userPvt,
															pUInt32DigitalInterrupt->pasynUser,
															1);
					}
					pNode = (interruptNode *)ellNext(&pNode->node);
				}
				pasynManager->interruptEnd(pPvt->uint32DigitalInterruptPvt);
				
				break;

		        case busywait:
 			     /* This is where the driver waits until a signal is sent to actually acquire data */
			    
			    /*  Need timeout to not starve msg queue*/
			    if (epicsEventWaitWithTimeout(pPvt->pollnow,DEFAULT_WAIT_DELAY) == epicsEventWaitOK){
				/* Poll now rcvd */
			    }else{
				/* Not triggered yet! */
				break;
			    }
			    if (pPvt->daqMode == AI)
				pPvt->state = acquireAnlg;
			    else if (pPvt->daqMode == BI)
				pPvt->state = acquireDig;
			    else if (pPvt->daqMode == CI)
				pPvt->state = acquireCnt;
			    else{
				asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR
				    ,"### ERROR: In state busywait but doesn't have AI or BI daqMode\n");
				pPvt->state = stop;
			    }
			    
			    /*  Start/stop aint needed so I removed this (will delete later)

			    Seems like it is needed afterall!!*/
			    if (!pPvt->noRestartTask)
				if( DAQmxFailed( DAQmxStartTask( pPvt->taskHandle )))
				{
				    DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
				    asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
						"### DAQmx ERROR (StartTask): %s\n", pPvt->daqMxErrBuf);
				    pPvt->state = idle;
				    break;
				}
			    
			    break;
				
			case acquireAnlg:
			    
			    if ((pPvt->daqMode == AO) || (pPvt->daqMode == BO)){
				asynPrint( pPvt->pasynUser, ASYN_TRACE_ERROR,
					   "### ERROR - got into acquire mode when not allowed (going to write mode)\n");
				pPvt->state = write;
				break;
			    }
			    if (pPvt->daqMode != AI)
			    {
				asynPrint( pPvt->pasynUser, ASYN_TRACE_ERROR,
					   "### ERROR - got into acquireAnlg mode but daqMode != AI\n");
				pPvt->state = stop;
				break;
			    }

				epicsTimeGetCurrent(&tp);
 				pPvt->dAcqTime = (tp.secPastEpoch + (1e-9 * tp.nsec)) - (oldtp.secPastEpoch + (1e-9 * oldtp.nsec));

				if( DAQmxFailed( DAQmxReadAnalogF64(	pPvt->taskHandle, 
															pPvt->nSamples,
															pPvt->timeout,
															DAQmx_Val_GroupByChannel,
															static_cast<double*>(pPvt->rawData),
															pPvt->totalNSamples,
 										                    &samplesTransferred,
															NULL) ))
				{
					DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
								"### DAQmx ERROR (ReadAnalogF64): %s\n", pPvt->daqMxErrBuf);
					pPvt->state = stop;
					break;

				}

				/* swap buffers and then update channel ptrs*/
				epicsMutexLock(pPvt->lock);

				pPvt->samplesTransferred = samplesTransferred;
				if (pPvt->samplesTransferred != pPvt->nSamples){
				    asynPrint( pPvt->pasynUser , ASYN_TRACE_ERROR, "### DAQmx ERROR : did not read requisted amount of samples\n");
				}
				tmpp = pPvt->rawData;
				pPvt->rawData = pPvt->prevData;
				pPvt->prevData = tmpp;
				/*  trigger */
				if (pPvt->softTriggerMode != 0){
				    if (SoftTrigger(pPvt) != asynSuccess)
					asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW," softTrigger: Did not find trigger \n");
				    /* should actually have some variable to show this? */
				}
				epicsMutexUnlock(pPvt->lock);


				if (setChannelPointers(pPvt) != asynSuccess){
				    asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, "### DAQmx ERROR: setChannelPointers failed! (acquireAnlg)\n");
				}
	
				/* Now put the read data in the correct places */
				
				/*d = (epicsFloat64*)pPvt->rawData;*/
				epicsMutexLock(pPvt->lock);
				
				/* Interrupt for float64Array :) */
				pasynManager->interruptStart(pPvt->float64ArrayInterruptPvt, &pclientList);
				pNode = (interruptNode *)ellFirst(pclientList);
				while (pNode != NULL)
				{
				    /*asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,
				      "Finding interrupt node\n");*/
					pFloat64ArrayInterrupt = static_cast<asynFloat64ArrayInterrupt*>(pNode->drvPvt);
					reason = pFloat64ArrayInterrupt->pasynUser->reason;

					if (reason == dataCmd)
					{
						pasynManager->getAddr(pFloat64ArrayInterrupt->pasynUser, &signal);
						/*asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,
						  "sending IO/intr (signal=%d).\n",signal);*/
						if (signal >= 0 && signal < pPvt->nChannels){
						    pFloat64ArrayInterrupt->callback(	pFloat64ArrayInterrupt->userPvt,
											pFloat64ArrayInterrupt->pasynUser,
											(epicsFloat64*)pPvt->aioPvt[signal]->data,
											(size_t)pPvt->aioPvt[signal]->dataSize);
						}
					}
					pNode = (interruptNode *)ellNext(&pNode->node);
				}
				pasynManager->interruptEnd(pPvt->float64ArrayInterruptPvt);
				/* Interrupt for float64 */
				pasynManager->interruptStart(pPvt->float64InterruptPvt, &pclientList);
				pNode = (interruptNode *)ellFirst(pclientList);
				while (pNode != NULL)
				{
				    /*asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,
				      "Finding interrupt node\n");*/
					pFloat64Interrupt = static_cast<asynFloat64Interrupt*>(pNode->drvPvt);
					reason = pFloat64Interrupt->pasynUser->reason;

					if ((reason == dataCmd)||(reason == dTimeCmd))
					{
						pasynManager->getAddr(pFloat64Interrupt->pasynUser, &signal);
						/*asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,
						  "sending IO/intr (signal=%d).\n",signal);*/
						if (signal >= 0 && signal < pPvt->nChannels){
						    pFloat64Interrupt->callback(	pFloat64Interrupt->userPvt,
											pFloat64Interrupt->pasynUser,
											*(epicsFloat64*)pPvt->aioPvt[signal]->data);
						}
					}
					pNode = (interruptNode *)ellNext(&pNode->node);
				}
				pasynManager->interruptEnd(pPvt->float64ArrayInterruptPvt);

				epicsMutexUnlock(pPvt->lock);
				oldtp = tp;

				/* SHOULDNT WE REMOVE THIS?*/
				/*post_event( EVENT_DATA ); - ok removed!*/


				/* This is now removed - no longer functional as it overlaps alot with 
				 Polled mode */
				/*if (pPvt->trigMode == oneshot) pPvt->state = stop;*/
				
				/* Normally if pPvt->trigMode = oneshot then polled is also true */
				if (pPvt->polled){ 
				    
				    epicsEventSignal(pPvt->polldone);
				    pPvt->state = busywait;
				    /* Not needed in polled/oneshot mode
				    if( DAQmxFailed( DAQmxStopTask( pPvt->taskHandle ) ))
				    {
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
							"### DAQmx ERROR (StopTask): %s\n", pPvt->daqMxErrBuf);
				    }
				    */
				}
				break;
  		        case acquireDig:
			    /*
			      acquireAnlg and acquireDig is fairly similiar
			      Offcourse the use different DAQmxRead functions.
			      The major difference is in interrupt calls.
			      acquireAnlg calls float64 and float64array interrupts
			      acquireDig calls int32 and int32array interrupts
			      
			      Most things are first tested and implemented in acquireAnlg and then simply copied here

			    */
			    if ((pPvt->daqMode == AO) || (pPvt->daqMode == BO)){
				asynPrint( pPvt->pasynUser, ASYN_TRACE_ERROR,
					   "### ERROR - got into acquire mode when not allowed (going to write mode)\n");
				pPvt->state = write;
				break;
			    }
			    if (pPvt->daqMode != BI)
			    {
				asynPrint( pPvt->pasynUser, ASYN_TRACE_ERROR,
					   "### ERROR - got into acquireDig mode but daqMode != BI\n");
				pPvt->state = stop;
				break;
			    }
			   
				epicsTimeGetCurrent(&tp);
//			    clock_gettime( CLOCK_REALTIME, &tp);
			    pPvt->dAcqTime = (tp.secPastEpoch + (1e-9 * tp.nsec)) - (oldtp.secPastEpoch + (1e-9 * oldtp.nsec));
			    if( DAQmxFailed( DAQmxReadDigitalU32(	pPvt->taskHandle, 
									pPvt->nSamples,
									pPvt->timeout,
									DAQmx_Val_GroupByChannel,
									static_cast<uInt32*>(pPvt->rawData),
									pPvt->totalNSamples,
									&samplesTransferred,
									NULL) ))
			    {
				DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
						"### DAQmx ERROR (ReadDigitalU32): %s\n", pPvt->daqMxErrBuf);
				pPvt->state = stop;
				break;
			    }
			    pPvt->samplesTransferred = samplesTransferred;

			    /* swap buffers and then update channel ptrs*/
			    epicsMutexLock(pPvt->lock);
			    tmpp = pPvt->rawData;
			    pPvt->rawData = pPvt->prevData;
			    pPvt->prevData = tmpp;
			    epicsMutexUnlock(pPvt->lock);
			    setChannelPointers(pPvt);
			    		    

			    epicsMutexLock(pPvt->lock);
			    /* Call interrupts - do not exit between lock and unlock!! */
			    
			    pasynManager->interruptStart(pPvt->int32ArrayInterruptPvt, &pclientList);
			    pNode = (interruptNode *)ellFirst(pclientList);
			    while (pNode != NULL)
			    {
				/*asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,
				  "Finding interrupt node\n");*/
				pInt32ArrayInterrupt = static_cast<asynInt32ArrayInterrupt*>(pNode->drvPvt);
				reason = pInt32ArrayInterrupt->pasynUser->reason;
				
				if (reason == dataCmd)
				{
				    pasynManager->getAddr(pInt32ArrayInterrupt->pasynUser, &signal);
				    asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,
					      "sending IO/intr (signal=%d).\n",signal);
				    if (signal >= 0 && signal < pPvt->nChannels){
					pInt32ArrayInterrupt->callback(	pInt32ArrayInterrupt->userPvt,
									pInt32ArrayInterrupt->pasynUser,
									(epicsInt32*)pPvt->bioPvt[signal]->data,
									(size_t)pPvt->samplesTransferred);
				    }
				}
				pNode = (interruptNode *)ellNext(&pNode->node);
			    }
			    pasynManager->interruptEnd(pPvt->int32ArrayInterruptPvt);
			    
			    pasynManager->interruptStart(pPvt->int32InterruptPvt, &pclientList);
			    pNode = (interruptNode *)ellFirst(pclientList);
			    while (pNode != NULL)
			    {
				/*asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,
				  "Finding interrupt node\n");*/
				pInt32Interrupt = static_cast<asynInt32Interrupt*>(pNode->drvPvt);
				reason = pInt32Interrupt->pasynUser->reason;
				
				if ((reason == dataCmd)||(reason == dTimeCmd))
				{
				    pasynManager->getAddr(pInt32Interrupt->pasynUser, &signal);
				    asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,
					      "sending IO/intr (signal=%d).\n",signal);
				    if (signal >= 0 && signal < pPvt->nChannels){
					pInt32Interrupt->callback(	pInt32Interrupt->userPvt,
									pInt32Interrupt->pasynUser,
									*(epicsInt32*)pPvt->bioPvt[signal]->data);
				    }
				}
				pNode = (interruptNode *)ellNext(&pNode->node);
			    }
			    pasynManager->interruptEnd(pPvt->int32InterruptPvt);


			    epicsMutexUnlock(pPvt->lock);
			    oldtp = tp;

			    /* SHOULDNT WE REMOVE THIS?*/
			    /*post_event( EVENT_DATA ); - ok removed */
			    
			    if (pPvt->polled){ 
				epicsEventSignal(pPvt->polldone);
				pPvt->state = busywait;
			    }
			    break;
  		        case acquireCnt:
			    /* Acquire Counter */
			    
			    if (pPvt->daqMode != CI)
			    {
				asynPrint( pPvt->pasynUser, ASYN_TRACE_ERROR,
					   "### ERROR - got into acquireCnt mode but daqMode != COUNTER\n");
				pPvt->state = stop;
				break;
			    }
			    
			    
				epicsTimeGetCurrent(&tp);
//			    clock_gettime( CLOCK_REALTIME, &tp);
			    pPvt->dAcqTime = (tp.secPastEpoch + (1e-9 * tp.nsec)) - (oldtp.secPastEpoch + (1e-9 * oldtp.nsec));
			    
			    switch (pPvt->counterRead){
				case 0: /*F64*/
				    if( DAQmxFailed( DAQmxReadCounterF64(	pPvt->taskHandle, 
										pPvt->nSamples,
										pPvt->timeout,
										static_cast<double*>(pPvt->rawData),
										pPvt->totalNSamples,
										&samplesTransferred,
										NULL) ))
				    {
					DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
							"### DAQmx ERROR (ReadCounterF64): %s\n", pPvt->daqMxErrBuf);
					pPvt->state = stop;
					break;
				    }
				    break;
				case 1: /*Scalar F64*/
				    samplesTransferred = 1;
				    if( DAQmxFailed( DAQmxReadCounterScalarF64(	pPvt->taskHandle, 
											pPvt->timeout,
											static_cast<double*>(pPvt->rawData),
											NULL) ))
				    {
					DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
							"### DAQmx ERROR (ReadCounterScalarF64): %s\n", pPvt->daqMxErrBuf);
					pPvt->state = stop;
					break;
				    }
				    break;
				case 2: /*Scalar U32*/
				    samplesTransferred = 1;
					ErrorCode = DAQmxReadCounterScalarU32(pPvt->taskHandle, pPvt->timeout, static_cast<uInt32*>(pPvt->rawData), NULL);
				    if (( DAQmxFailed(ErrorCode) ) && (ErrorCode != DAQmxErrorSamplesNotYetAvailable))
				    {
					DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
							"### DAQmx ERROR (ReadCounterScalarU32): %s\n", pPvt->daqMxErrBuf);
					pPvt->state = stop;
					break;
				    }

				    break;
				case 3: /*U32*/
				    if( DAQmxFailed( DAQmxReadCounterU32(	pPvt->taskHandle, 
										pPvt->nSamples,
										pPvt->timeout,
										static_cast<uInt32*>(pPvt->rawData),
										pPvt->totalNSamples,
										&samplesTransferred,
										NULL) ))
				    {
					DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
							"### DAQmx ERROR (ReadCounterU32): %s\n", pPvt->daqMxErrBuf);
					pPvt->state = stop;
					break;
				    }
				    break;
				default:
				    asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
						"### DAQmx ERROR: Invalid value for pPvt->counterRead\n");
				    pPvt->state = idle;
				    break;
			    }
			    if (pPvt->state != acquireCnt) break;
			   
			    pPvt->samplesTransferred = samplesTransferred;

			    
			    /* swap buffers and then update channel ptrs*/
			    epicsMutexLock(pPvt->lock);
			    tmpp = pPvt->rawData;
			    pPvt->rawData = pPvt->prevData;
			    pPvt->prevData = tmpp;
			    epicsMutexUnlock(pPvt->lock);
			    setChannelPointers(pPvt);

			    /* should maybe stop task before starting again as in Anlg? */

			    
			    /* TODO - complete interrupts here */
			    

			    epicsMutexLock(pPvt->lock);
			    /* problem here is that dTime interrupt would not always be called :( */

			    switch (pPvt->counterRead){
				case 0: /* F64 */
				    
				    break;
				case 1: /* Scalar F64*/
				    break;
				case 2: /* Scalar U32*/
				    
				    /* Calling int32 interrupts only */
				    pasynManager->interruptStart(pPvt->int32InterruptPvt, &pclientList);
				    pNode = (interruptNode *)ellFirst(pclientList);
				    while (pNode != NULL)
				    {
					/*asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,
					  "Finding interrupt node\n");*/
					pInt32Interrupt = static_cast<asynInt32Interrupt*>(pNode->drvPvt);
					reason = pInt32Interrupt->pasynUser->reason;
					
					if ((reason == dataCmd)||(reason == dTimeCmd))
					{
					      pasynManager->getAddr(pInt32Interrupt->pasynUser, &signal);
					      
					      if (signal == 0){
						  /*asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,
						    "sending IO/intr (signal=%d).\n",signal);  - floods the whole thing!!*/
						  pInt32Interrupt->callback(	pInt32Interrupt->userPvt,
										pInt32Interrupt->pasynUser,
										*(epicsInt32*)pPvt->ciPvt->data);
					      }
					}
					pNode = (interruptNode *)ellNext(&pNode->node);
				    }
				    pasynManager->interruptEnd(pPvt->int32InterruptPvt);
				    break;
				case 3: /* U32*/
				    break;
				default:
				    printf("### program ERROR #123 - FIX CODE!\n");
				    break;
			    }
			    epicsMutexUnlock(pPvt->lock);
			    

			    

			    oldtp = tp;

			    if (pPvt->polled){ 
				epicsEventSignal(pPvt->polldone);
				pPvt->state = busywait;
			    }


			    break;
 		        case write:
			    /* Write mode */
			    
			    /* Step 1 - wait :)  (see if we need todo something) */	
			    epicsEventWaitWithTimeout(pPvt->writeEvent,DEFAULT_WAIT_DELAY);
		    
			    if (!pPvt->writeNeeded)
				break;
			    /* Ok write some data...*/
			    /*  I'm not sure if these mutexes are correct here? */
			    epicsMutexLock(pPvt->lock);
			    
			    pPvt->writeNeeded = 0; /* clear the write*/
			    
			    epicsMutexUnlock(pPvt->lock);
			    
			    if (pPvt->daqMode == AO){
				if (DAQmxFailed( DAQmxWriteAnalogF64(pPvt->taskHandle,
									 pPvt->nSamples,
									 FALSE,
									 pPvt->timeout,
									 DAQmx_Val_GroupByChannel,
									 static_cast<double*>(pPvt->rawData),
									 &samplesTransferred, /* using same variable as reading - should be ok? */
									 NULL)))
				{
				    DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
				    asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
						"### DAQmx ERROR (WriteAnalogF64): %s\n", pPvt->daqMxErrBuf);
				    pPvt->state = stop;
				}
			    }else if (pPvt->daqMode == BO){
				if (DAQmxFailed( DAQmxWriteDigitalU32(pPvt->taskHandle,
									 pPvt->nSamples,
									 FALSE,
									 pPvt->timeout,
									 DAQmx_Val_GroupByChannel,
									 static_cast<uInt32*>(pPvt->rawData),
									 &samplesTransferred, /* using same variable as reading - should be ok? */
									 NULL)))
				{
				    DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
				    asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
						"### DAQmx ERROR (WriteDigitalU32): %s\n", pPvt->daqMxErrBuf);
				    pPvt->state = stop;
				}
				
			    }else{
				asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR,
					  "### ERROR: Got into write state with unsupported daqMode\n");
				pPvt->state = stop;
			    }
			    asynPrint(pPvt->pasynUser, ASYN_TRACE_FLOW,
				      "port %s: Write - channels:%d nSamples:%d written:%d\n",
				      pPvt->portName,pPvt->nChannels,pPvt->nSamples,samplesTransferred);

			    pPvt->samplesTransferred = samplesTransferred;
			    if (pPvt->samplesTransferred != pPvt->nSamples)
			    {
				asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR,
					   "port %s: AO Write - didn't write all requisted samples\n",pPvt->portName);
			    }

			    break;

		        case counterout:
			    
			    if (!pPvt->writeNeeded) /* maybe this is a bad idea - should rather always signal the event?*/
				epicsEventWaitWithTimeout(pPvt->writeEvent,DEFAULT_WAIT_DELAY);
		    
			    if (!pPvt->writeNeeded)
				break;
			    /* Ok write some data...*/
			    /*  I'm not sure if these mutexes are correct here? */
			    epicsMutexLock(pPvt->lock);
			    
			    pPvt->writeNeeded = 0; /* clear the write*/
			    
			    epicsMutexUnlock(pPvt->lock);
			    
			    /* (should first make sure it's stopped correctly???)
			      Should start/restart the thing
			     So can in this way be used to generate pulses or series of pulses*/
			    if( DAQmxFailed( DAQmxStartTask( pPvt->taskHandle )))
			    {
				DAQmxGetExtendedErrorInfo(pPvt->daqMxErrBuf, ERR_BUF_SIZE);
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
						"### DAQmx ERROR (StartTask): %s\n", pPvt->daqMxErrBuf);
				pPvt->state = idle;
				break;
			    }
			    
			    break;
			case stop:
				if( DAQmxFailed( DAQmxStopTask( pPvt->taskHandle ) ))
				{
					asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
								"### DAQmx ERROR (StopTask): %s\n", pPvt->daqMxErrBuf);
				}
				pasynManager->interruptStart(pPvt->uint32DigitalInterruptPvt, &pclientList);
				pNode = (interruptNode *)ellFirst(pclientList);
				pPvt->state = idle;
				while (pNode != NULL)
				{
					pUInt32DigitalInterrupt = static_cast<asynUInt32DigitalInterrupt*>(pNode->drvPvt);
					reason = pUInt32DigitalInterrupt->pasynUser->reason;
					if (reason == acquireCmd)
					{
						pasynManager->getAddr(pUInt32DigitalInterrupt->pasynUser, &signal);
						pUInt32DigitalInterrupt->callback(	pUInt32DigitalInterrupt->userPvt,
															pUInt32DigitalInterrupt->pasynUser,
															0);
					}
					pNode = (interruptNode *)ellNext(&pNode->node);
				}
				pasynManager->interruptEnd(pPvt->uint32DigitalInterruptPvt);
				break;
				

			default:
				asynPrint(	pPvt->pasynUser, ASYN_TRACE_ERROR,
							"### Thread: unknown state: %d\n", (int)pPvt->state);
				epicsThreadSleep(DEFAULT_WAIT_DELAY);
				break;
		}
	}
}


	

static const iocshArg DAQmxConfigArg0 = {"portName", iocshArgString};
static const iocshArg DAQmxConfigArg1 = {"deviceName", iocshArgString};
static const iocshArg DAQmxConfigArg2 = {"Channelnr", iocshArgInt};
static const iocshArg DAQmxConfigArg3 = {"acqType", iocshArgString};
static const iocshArg DAQmxConfigArg4 = {"options", iocshArgString};
static const iocshArg * const DAQmxConfigArgs[] = {	&DAQmxConfigArg0,
								&DAQmxConfigArg1,
								&DAQmxConfigArg2,
								&DAQmxConfigArg3,
								&DAQmxConfigArg4};
static const iocshFuncDef DAQmxConfigFuncDef = {"DAQmxConfig", 5, DAQmxConfigArgs};


static const iocshArg DAQmxResetArg0 = {"deviceName",iocshArgString};
static const iocshArg * const DAQmxResetArgs[] = {&DAQmxResetArg0};
static const iocshFuncDef DAQmxResetFuncDef = {"DAQmxReset",1,DAQmxResetArgs};

static const iocshArg DAQmxChangeDeviceNameArg0 = {"portName",iocshArgString};
static const iocshArg DAQmxChangeDeviceNameArg1 = {"Channelnr",iocshArgInt};
static const iocshArg DAQmxChangeDeviceNameArg2 = {"newdeviceName",iocshArgString};
static const iocshArg * const DAQmxChangeDeviceNameArgs[] = {&DAQmxChangeDeviceNameArg0,&DAQmxChangeDeviceNameArg1,&DAQmxChangeDeviceNameArg2};
static const iocshFuncDef DAQmxChangeDeviceNameFuncDef = {"DAQmxChangeDeviceName",3,DAQmxChangeDeviceNameArgs};

static const iocshArg DAQmxPortOptionsArg0 = {"portName",iocshArgString};
static const iocshArg DAQmxPortOptionsArg1 = {"Channelnr",iocshArgInt};
static const iocshArg DAQmxPortOptionsArg2 = {"options",iocshArgString};
static const iocshArg * const DAQmxPortOptionsArgs[] = { &DAQmxPortOptionsArg0, &DAQmxPortOptionsArg1, &DAQmxPortOptionsArg2};
static const iocshFuncDef DAQmxPortOptionsFuncDef = {"DAQmxPortOptions",3,DAQmxPortOptionsArgs};


static const iocshArg DAQmxGenArg0 = {"portName",iocshArgString};
static const iocshArg DAQmxGenArg1 = {"Channelnr",iocshArgInt};
static const iocshArg DAQmxGenArg2 = {"params",iocshArgString};
static const iocshArg * const DAQmxGenArgs[] = {&DAQmxGenArg0, &DAQmxGenArg1, &DAQmxGenArg2};
static const iocshFuncDef DAQmxGenFuncDef = {"DAQmxGen",3,DAQmxGenArgs};

static const iocshArg DAQmxGenPArg0 = {"params",iocshArgString};
static const iocshArg * const DAQmxGenPArgs[] = {&DAQmxGenArg0};
static const iocshFuncDef DAQmxGenPFuncDef = {"DAQmxGenP",1,DAQmxGenPArgs};

static const iocshArg DAQmxTriggerArg0 = {"portName",iocshArgString};
static const iocshArg DAQmxTriggerArg1 = {"triggerSource",iocshArgString};
static const iocshArg DAQmxTriggerArg2 = {"triggerOptions",iocshArgString};
static const iocshArg * const DAQmxTriggerArgs[] = {&DAQmxTriggerArg0,&DAQmxTriggerArg1,&DAQmxTriggerArg2};
static const iocshFuncDef DAQmxTriggerFuncDef = {"DAQmxTrigger",3,DAQmxTriggerArgs};

static const iocshArg DAQmxStartArg0 = {"portName" , iocshArgString};
static const iocshArg * const DAQmxStartArgs[] = {&DAQmxStartArg0};
static const iocshFuncDef DAQmxStartFuncDef = {"DAQmxStart",1,DAQmxStartArgs};

static void DAQmxConfigCallFunc(const iocshArgBuf *args)
{
    if (DAQmxConfig( args[0].sval, args[1].sval, args[2].ival, args[3].sval,args[4].sval) != 0)
    {
	printf("### ERROR - DAQmxConfig Failed!!\n");
    }
}
static void DAQmxResetCallFunc(const iocshArgBuf *args)
{
    DAQmxReset(args[0].sval);
}
static void DAQmxChangeDeviceNameFunc(const iocshArgBuf *args)
{
    DAQmxChangeDeviceName(args[0].sval,args[1].ival,args[2].sval);
}
static void DAQmxPortOptionsFunc(const iocshArgBuf * args)
{
    DAQmxPortOptions(args[0].sval,args[1].ival,args[2].sval);
}
static void DAQmxGenFunc(const iocshArgBuf *args)
{
    DAQmxGen(args[0].sval,args[1].ival,args[2].sval);
}
static void DAQmxGenPFunc(const iocshArgBuf *args)
{
    DAQmxGenP(args[0].sval);
}
static void DAQmxTriggerFunc(const iocshArgBuf *args)
{
    DAQmxTrigger(args[0].sval,args[1].sval,args[2].sval);
}
static void DAQmxStartFunc(const iocshArgBuf *args)
{
	if (!args[0].sval)
		fprintf(stderr, "Task not specified\n");
	else
		DAQmxStart(args[0].sval);
}

static void DAQmxBaseRegistrar(void)
{
    globalshutdown = 0;
    g_PrevGenPort = "";
    g_PrevGenChan = 0;

    iocshRegister(&DAQmxConfigFuncDef, DAQmxConfigCallFunc);
	iocshRegister(&DAQmxResetFuncDef, DAQmxResetCallFunc);
	iocshRegister(&DAQmxChangeDeviceNameFuncDef, DAQmxChangeDeviceNameFunc);
	iocshRegister(&DAQmxPortOptionsFuncDef, DAQmxPortOptionsFunc);
	iocshRegister(&DAQmxGenFuncDef, DAQmxGenFunc);
	iocshRegister(&DAQmxGenPFuncDef, DAQmxGenPFunc);
	iocshRegister(&DAQmxTriggerFuncDef, DAQmxTriggerFunc);
	iocshRegister(&DAQmxStartFuncDef, DAQmxStartFunc);
}

extern "C" {

epicsExportRegistrar(DAQmxBaseRegistrar);

}