/* BISDetector.cpp
 *
 * This is a driver for Bruker Instrument Service (BIS) detectors.
 *
 * Original Author: Jeff Gebhardt
 * Current Author:  Mark Rivers
 *         University of Chicago
 *
 * Created:  March 18, 2010
 *
 * Derived from pilatusDetector.cpp
 *
 * Author: Mark Rivers
 *         University of Chicago
 *
 * Created:  June 11, 2008
 */
 
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsEvent.h>
#include <epicsMutex.h>
#include <epicsTimer.h>
#include <epicsString.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <cantProceed.h>
#include <iocsh.h>
#include <epicsExport.h>

#include <asynOctetSyncIO.h>

#include "ADDriver.h"

/** Frame type choices */
typedef enum {
    BISFrameNormal,
    BISFrameDark,
    BISFrameRaw,
    BISFrameDoubleCorrelation
} BISFrameType_t;

/** Messages to/from BIS */
#define MAX_MESSAGE_SIZE 512 
#define MAX_FILENAME_LEN 256
/** Time to poll when reading from BIS */
#define ASYN_POLL_TIME .01 
#define BIS_POLL_DELAY .01 
#define BIS_DEFAULT_TIMEOUT 1.0 
/** Time between checking to see if .SFRM file is complete */
#define FILE_READ_DELAY .01

#define BISSFRMTimeoutString  "SFRM_TIMEOUT"
#define BISNumDarksString     "NUM_DARKS"
#define BISStatusString       "BIS_STATUS"


static const char *driverName = "BISDetector";

/** Driver for Bruker BIS ccd detector using their BIS server over TCP/IP socket */
class BISDetector : public ADDriver {
public:
    BISDetector(const char *portName, const char *BISCommandPort, const char *BISStatusPort,
                    int maxBuffers, size_t maxMemory,
                    int priority, int stackSize);
                 
    /* These are the methods that we override from ADDriver */
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual void setShutter(int open);
    void report(FILE *fp, int details);
    /* These are new methods */
    void BISTask();  /* This should be private but is called from C so must be public */
    void statusTask(); /* This should be private but is called from C so must be public */
    epicsEventId stopEventId;   /**< This should be private but is accessed from C, must be public */
 
 protected:
    int BISSFRMTimeout;
#define FIRST_BIS_PARAM BISSFRMTimeout
    int BISNumDarks;
    int BISStatus;
#define LAST_BIS_PARAM BISStatus

 private:                                       
    /* These are the methods that are new to this class */
    asynStatus readSFRM(const char *fileName, epicsTimeStamp *pStartTime, double timeout, NDArray *pImage);
    asynStatus writeBIS(double timeout);
       
    /* Our data */
    epicsEventId startEventId;
    epicsEventId readoutEventId;
    epicsTimerId timerId;
    char toBIS[MAX_MESSAGE_SIZE];
    char fromBIS[MAX_MESSAGE_SIZE];
    asynUser *pasynUserCommand;
    asynUser *pasynUserStatus;
};

#define NUM_BIS_PARAMS ((int)(&LAST_BIS_PARAM - &FIRST_BIS_PARAM + 1))

/** This function reads .SFRM files.  It is not intended to be general,
 * it is intended to read the .SFRM files that BIS creates.  It does not implement under/overflows.  It checks to make sure
 * that the creation time of the file is after a start time passed to it, to force it to
 * wait for a new file to be created.
 */
 
asynStatus BISDetector::readSFRM(const char *fileName, epicsTimeStamp *pStartTime, double timeout, NDArray *pImage)
{
    #define lineLen 80
    #define maxLine 95
    #define blockLen 512
    #define dataOffset 8
    FILE *file=NULL;
    int fileExists=0;
    struct stat statBuff;
    epicsTimeStamp tStart, tCheck;
    time_t acqStartTime;
    double deltaTime;
    int status=-1;
    const char *functionName = "readSFRM";
    int offset, version, format;
    int nPixels;
    size_t nBytes, nRead=0;
    int headerBlocks;
    int numUnderflows, numOverflows1, numOverflows2;
    int nRows, nCols;
    int bytesPerPixel, underflowBytesPerPixel;
    int wordOrder, longOrder;
    int numExposures, bias, baselineOffset, orientation, overscan;
    epicsUInt16 *pOverflows1=NULL;
    epicsUInt32 *pOverflows2=NULL;
    epicsUInt16 *pUnderflows=NULL;
    int i;
    char *buffer=NULL;
    epicsUInt8 *byteBuffer;
    epicsUInt16 *i2Buffer;
    epicsUInt32 *pData = (epicsUInt32 *)pImage->pData;
    int n1=0, n2=0, nu=0;
        
    deltaTime = 0.;
    if (pStartTime) epicsTimeToTime_t(&acqStartTime, pStartTime);
    epicsTimeGetCurrent(&tStart);
    
    while (deltaTime <= timeout) {
        file = fopen(fileName, "rb");
        if (file && (timeout != 0.)) {
            fileExists = 1;
            /* The file exists.  Make sure it is a new file, not an old one.
             * We don't do this check if timeout==0 */
            status = stat(fileName, &statBuff);
            if (status){
                asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                    "%s::%s error calling fstat, errno=%d %s\n",
                    driverName, functionName, errno, fileName);
                fclose(file);
                return(asynError);
            }
            /* We allow up to 10 second clock skew between time on machine running this IOC
             * and the machine with the file system returning modification time */
            if (difftime(statBuff.st_mtime, acqStartTime) > -10) break;
            fclose(file);
            file = NULL;
        }
        /* Sleep, but check for stop event, which can be used to abort a long acquisition */
        unlock();
        status = epicsEventWaitWithTimeout(this->stopEventId, FILE_READ_DELAY);
        lock();
        if (status == epicsEventWaitOK) {
            return(asynError);
        }
        epicsTimeGetCurrent(&tCheck);
        deltaTime = epicsTimeDiffInSeconds(&tCheck, &tStart);
    }
    if (file == NULL) {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "%s::%s timeout waiting for file to be created %s\n",
            driverName, functionName, fileName);
        if (fileExists) {
            asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                "  file exists but is more than 10 seconds old, possible clock synchronization problem\n");
        } 
        return(asynError);
    }
    /* Allocate the buffer */
    buffer = (char *)malloc(lineLen * maxLine);
    /* Read the first 3 lines of the header */
    nBytes = fread(buffer, 1, 3*lineLen, file);
    offset = 0*lineLen + dataOffset;
    sscanf(buffer+offset, "%d", &format);
    offset = 1*lineLen + dataOffset;
    sscanf(buffer+offset, "%d", &version);
    offset = 2*lineLen + dataOffset;
    sscanf(buffer+offset, "%d", &headerBlocks);
    free(buffer);
    if ((format != 100) || (version < 11)) {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, 
            "%s:%s: unsupported format=%d or version=%d\n",
            driverName, functionName, format, version);
        fclose(file);
        return(asynError);
    }
    /* Read the entire header now that we know how big it is */
    buffer = (char *)malloc(headerBlocks*blockLen);
    fseek(file, 0, SEEK_SET);
    nBytes = fread(buffer, 1, headerBlocks*blockLen, file);
    offset = 20*lineLen + dataOffset;
    sscanf(buffer+offset, "%d%d%d", &numUnderflows, &numOverflows1, &numOverflows2);
    offset = 39*lineLen + dataOffset;
    sscanf(buffer+offset, "%d%d", &bytesPerPixel, &underflowBytesPerPixel);
    offset = 40*lineLen + dataOffset;
    sscanf(buffer+offset, "%d", &nRows);
    offset = 41*lineLen + dataOffset;
    sscanf(buffer+offset, "%d", &nCols);
    offset = 42*lineLen + dataOffset;
    sscanf(buffer+offset, "%d", &wordOrder);
    offset = 42*lineLen + dataOffset;
    sscanf(buffer+offset, "%d", &longOrder);
    offset = 79*lineLen + dataOffset;
    sscanf(buffer+offset, "%d%d%d%d%d", &numExposures, &bias, &baselineOffset, &orientation, &overscan);
    free(buffer);
    if ((wordOrder !=0) || (longOrder != 0)) {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, 
            "%s:%s: unsupported wordOrder=%d or longOrder=%d\n",
            driverName, functionName, wordOrder, longOrder);
        fclose(file);
        return(asynError);
    }

    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
        "%s:%s: file=%s, numUnderflows=%d, numOverflows1=%d, numOverflows2=%d, bytesPerPixel=%d, underflowBytesPerPixel=%d, nRows=%d, nCols=%d\n",
        driverName, functionName, fileName, numUnderflows, numOverflows1, numOverflows2, bytesPerPixel, underflowBytesPerPixel, nRows, nCols);
    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
        "%s:%s: numExposures=%d, bias=%d, baselineOffset=%d, orientation=%d, overscan=%d\n",
        driverName, functionName, numExposures, bias, baselineOffset, orientation, overscan);

    /* We have now read enough of the header to read the data and correct it */
    nPixels = nRows*nCols;
    switch (bytesPerPixel) {
        case 1:
            nBytes = nPixels * 1;
            byteBuffer = (epicsUInt8 *)malloc(nBytes);
            nRead = fread(byteBuffer, 1, nBytes, file);
            for (i=0; i<nPixels; i++) pData[i] = byteBuffer[i];
            free(byteBuffer);
            break;
        case 2:
            nBytes = nPixels * 2;
            i2Buffer = (epicsUInt16 *)malloc(nBytes);
            nRead = fread(i2Buffer, 1, nBytes, file);
            for (i=0; i<nPixels; i++) pData[i] = i2Buffer[i];
            free(i2Buffer);
            break;
        case 4:
            nBytes = nPixels * 4;
            nRead = fread(pData, 1, nBytes, file);
            break;
    }
    if (nRead != nBytes) {
        asynPrint(pasynUserSelf, ASYN_TRACE_ERROR,
            "%s:%s: error reading file %s, only read %lu/%lu bytes\n",
            driverName, functionName, fileName, (unsigned long)nRead, (unsigned long)nBytes);
    }
    /* Read the underflow and overflow tables */
    if (numUnderflows > 0) {
        nBytes = ((numUnderflows * underflowBytesPerPixel + 15) / 16) * 16;
        pUnderflows = (epicsUInt16 *)malloc(nBytes);
        fread(pUnderflows, 1, nBytes, file);
    }
    if (numOverflows1 > 0) {
        nBytes = ((numOverflows1 * 2 + 15) / 16) * 16;
        pOverflows1 = (epicsUInt16 *)malloc(nBytes);
        fread(pOverflows1, 1, nBytes, file);
    }    
    if (numOverflows2 > 0) {
        nBytes = ((numOverflows2 * 4 + 15) / 16) * 16;
        pOverflows2 = (epicsUInt32 *)malloc(nBytes);
        fread(pOverflows2, 1, nBytes, file);
    }
    fclose(file);
    if (numUnderflows == -1) baselineOffset = 0;
    for (i=0; i<nPixels; i++) {
        if ((bytesPerPixel == 1) && (pData[i] == 255)) {
            pData[i] = pOverflows1[n1++];
        }
        if ((bytesPerPixel != 4) && (pData[i] == 65535)) {
            pData[i] = pOverflows2[n2++];
        }
        if (pData[i] == 0) {
            if (numUnderflows > 0) {
                pData[i] = pUnderflows[nu++];
            }
            //else printf("0 pixel=%d/%d\n", i, nPixels-1);
        }
        else if (baselineOffset != 0) {
            pData[i] += baselineOffset;
        }
    }
    free(pUnderflows);
    free(pOverflows1);
    free(pOverflows2);
    return(asynSuccess);
}   

asynStatus BISDetector::writeBIS(double timeout)
{
    size_t nwrite, nread;
    int eomReason;
    asynStatus status;
    const char *functionName="writeBIS";

    status = pasynOctetSyncIO->writeRead(this->pasynUserCommand, 
                                         this->toBIS, strlen(this->toBIS), 
                                         this->fromBIS, sizeof(this->fromBIS), 
                                         timeout, &nwrite, &nread, &eomReason);
                                        
    if (status) asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                    "%s:%s, status=%d, sent\n%s\n",
                    driverName, functionName, status, this->toBIS);

    /* Set output string so it can get back to EPICS */
    setStringParam(ADStringToServer, this->toBIS);
    setStringParam(ADStringFromServer, this->fromBIS);
    
    return(status);
}

void BISDetector::setShutter(int open)
{
    ADShutterMode_t shutterMode;
    int itemp;
    double delay;
    double shutterOpenDelay, shutterCloseDelay;
    
    getIntegerParam(ADShutterMode, &itemp); shutterMode = (ADShutterMode_t)itemp;
    getDoubleParam(ADShutterOpenDelay, &shutterOpenDelay);
    getDoubleParam(ADShutterCloseDelay, &shutterCloseDelay);

    switch (shutterMode) {
        case ADShutterModeDetector:
            if (open) {
                /* Open the shutter */
                epicsSnprintf(this->toBIS, sizeof(this->toBIS), 
                    "[Shutter /Status=1]");
                writeBIS(2.0);
                /* This delay is to get the exposure time correct.  
                * It is equal to the opening time of the shutter minus the
                * closing time.  If they are equal then no delay is needed, 
                * except use 1msec so delay is not negative and commands are 
                * not back-to-back */
                delay = shutterOpenDelay - shutterCloseDelay;
                if (delay < .001) delay=.001;
                epicsThreadSleep(delay);
            } else {
                /* Close shutter */
                epicsSnprintf(this->toBIS, sizeof(this->toBIS), 
                    "[Shutter /Status=0]");
                writeBIS(2.0);
                epicsThreadSleep(shutterCloseDelay);
            }
            callParamCallbacks();
            break;
        default:
            ADDriver::setShutter(open);
            break;
    }
}

/** This function is called when the exposure time timer expires */
extern "C" {static void timerCallbackC(void *drvPvt)
{
    BISDetector *pPvt = (BISDetector *)drvPvt;
    
    epicsEventSignal(pPvt->stopEventId);
}}

static void statusTaskC(void *drvPvt)
{
    BISDetector *pPvt = (BISDetector *)drvPvt;
    
    pPvt->statusTask();
}

/** This thread reads status strings from the status socket, makes the string available to EPICS, and
  * sends an eveny when it detects acquisition complete, etc. */
void BISDetector::statusTask()
{
    int status = asynSuccess;
    char response[MAX_MESSAGE_SIZE];
    char *p;
    double timeout=-1.0;
    size_t nread;
    int eomReason;
    int acquire;
    int framesize;
    int open;
    double temperature;

    while(1) {
        status = pasynOctetSyncIO->read(this->pasynUserStatus, response,
                                        sizeof(response), timeout,
                                        &nread, &eomReason);
        if (status == asynSuccess) {
            this->lock();
            setStringParam(BISStatus, response);
            getIntegerParam(ADAcquire, &acquire);
            if (strstr(response, "[INSTRUMENTQUEUE /PROCESSING=0]")) {
                if (acquire) {
                    epicsEventSignal(this->readoutEventId);
                }
            } 
            else if (strstr(response, "[CCDTEMPERATURE")) {
                p = strstr(response, "DEGREESC=");
                sscanf(p, "DEGREESC=%lf", &temperature);
                setDoubleParam(ADTemperature, temperature);
            }
            else if (strstr(response, "[DETECTORSTATUS")) {
                p = strstr(response, "FRAMESIZE=");
                sscanf(p, "FRAMESIZE=%d", &framesize);
                p = strstr(response, "CCDTEMP=");
                sscanf(p, "CCDTEMP=%lf", &temperature);
                setIntegerParam(ADSizeX, framesize);
                setIntegerParam(ADSizeY, framesize);
                setDoubleParam(ADTemperature, temperature);
            }
            else if (strstr(response, "[SHUTTERSTATUS")) {
                p = strstr(response, "STATUS=");
                sscanf(p, "STATUS=%d", &open);
                setIntegerParam(ADShutterStatus, open);
            }
            callParamCallbacks();
            /* Sleep a short while to allow EPICS record to process 
             * before sending the next string */
            epicsThreadSleep(0.01);
            this->unlock();
        }
    }
}

static void BISTaskC(void *drvPvt)
{
    BISDetector *pPvt = (BISDetector *)drvPvt;
    
    pPvt->BISTask();
}

/** This thread controls acquisition, reads SFRM files to get the image data, and
  * does the callbacks to send it to higher layers */
void BISDetector::BISTask()
{
    int status = asynSuccess;
    int imageCounter;
        int numImages, numImagesCounter;
    int imageMode;
    int acquire;
    NDArray *pImage;
    double acquireTime, timeRemaining;
    ADShutterMode_t shutterMode;
    int frameType;
    int numDarks;
    double readSFRMTimeout;
    epicsTimeStamp startTime, currentTime;
    const char *functionName = "BISTask";
    char fullFileName[MAX_FILENAME_LEN];
    char statusMessage[MAX_MESSAGE_SIZE];
    size_t dims[2];
    int itemp;
    int arrayCallbacks;
    
    this->lock();

    /* Loop forever */
    while (1) {
        /* Is acquisition active? */
        getIntegerParam(ADAcquire, &acquire);
        
        /* If we are not acquiring then wait for a semaphore that is given when acquisition is started */
        if (!acquire) {
            setStringParam(ADStatusMessage, "Waiting for acquire command");
            setIntegerParam(ADStatus, ADStatusIdle);
            callParamCallbacks();
            /* Release the lock while we wait for an event that says acquire has started, then lock again */
            this->unlock();
            asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, 
                "%s:%s: waiting for acquire to start\n", driverName, functionName);
            status = epicsEventWait(this->startEventId);
            this->lock();
            setIntegerParam(ADNumImagesCounter, 0);
        }
        
        /* Get current values of some parameters */
        getIntegerParam(ADFrameType, &frameType);
        /* Get the exposure parameters */
        getDoubleParam(ADAcquireTime, &acquireTime);
        getIntegerParam(ADShutterMode, &itemp);  shutterMode = (ADShutterMode_t)itemp;
        getDoubleParam(BISSFRMTimeout, &readSFRMTimeout);
        
        setIntegerParam(ADStatus, ADStatusAcquire);

        /* Create the full filename */
        createFileName(sizeof(fullFileName), fullFileName);
        
        setStringParam(ADStatusMessage, "Starting exposure");
        /* Call the callbacks to update any changes */
        setStringParam(NDFullFileName, fullFileName);
        callParamCallbacks();
        switch (frameType) {
            case BISFrameNormal:
                epicsSnprintf(this->toBIS, sizeof(this->toBIS), 
                    "[Scan /Filename=%s /scantime=%f /Rescan=0]", fullFileName, acquireTime);
                break;
            case BISFrameDark:
                getIntegerParam(BISNumDarks, &numDarks);
                epicsSnprintf(this->toBIS, sizeof(this->toBIS), 
                    "[Dark /AddTime=%f /Repetitions=%d]", acquireTime, numDarks);
                break;
            case BISFrameRaw:
                epicsSnprintf(this->toBIS, sizeof(this->toBIS), 
                    "[Scan /Filename=%s /scantime=%f /Rescan=0 /DarkFlood=0]", fullFileName, acquireTime);
                break;
            case BISFrameDoubleCorrelation:
                epicsSnprintf(this->toBIS, sizeof(this->toBIS), 
                    "[Scan /Filename=%s /scantime=%f /Rescan=1]", fullFileName, acquireTime);
                break;
        }
        /* Send the acquire command to BIS */
        writeBIS(2.0);

        setStringParam(ADStatusMessage, "Waiting for Acquisition");
        callParamCallbacks();
        /* Set the the start time for the TimeRemaining counter */
        epicsTimeGetCurrent(&startTime);
        timeRemaining = acquireTime;

        /* BIS will control the shutter if we are using the hardware shutter signal.
         * If we are using the EPICS shutter then tell it to open */
        if (shutterMode == ADShutterModeEPICS) ADDriver::setShutter(1);

        /* Wait for the exposure time using epicsEventWaitWithTimeout, 
         * so we can abort. */
        epicsTimerStartDelay(this->timerId, acquireTime);
        while(1) {
            this->unlock();
            status = epicsEventWaitWithTimeout(this->stopEventId, BIS_POLL_DELAY);
            this->lock();
            if (status == epicsEventWaitOK) {
                /* The acquisition was stopped before the time was complete */
                epicsTimerCancel(this->timerId);
                break;
            }
            epicsTimeGetCurrent(&currentTime);
            timeRemaining = acquireTime -  epicsTimeDiffInSeconds(&currentTime, &startTime);
            if (timeRemaining < 0.) timeRemaining = 0.;
            setDoubleParam(ADTimeRemaining, timeRemaining);
            callParamCallbacks();
        }
        if (shutterMode == ADShutterModeEPICS) ADDriver::setShutter(0);
        setDoubleParam(ADTimeRemaining, 0.0);
        callParamCallbacks();
        this->unlock();
        status = epicsEventWaitWithTimeout(this->readoutEventId, 5.0);
        this->lock();
        /* If there was an error jump to bottom of loop */
        if (status != epicsEventWaitOK) {
            setIntegerParam(ADAcquire, 0);
            asynPrint(pasynUserSelf, ASYN_TRACE_ERROR,
                "%s:%s: error waiting for readout to complete\n",
                driverName, functionName);
            goto done;
        }
        getIntegerParam(NDArrayCallbacks, &arrayCallbacks);
        getIntegerParam(NDArrayCounter, &imageCounter);
        imageCounter++;
        setIntegerParam(NDArrayCounter, imageCounter);
        getIntegerParam(ADNumImagesCounter, &numImagesCounter);
        numImagesCounter++;
        setIntegerParam(ADNumImagesCounter, numImagesCounter);
        callParamCallbacks();

        if (arrayCallbacks && frameType != BISFrameDark) {
            /* Get an image buffer from the pool */
            getIntegerParam(ADSizeX, &itemp); dims[0] = itemp;
            getIntegerParam(ADSizeY, &itemp); dims[1] = itemp;
            pImage = this->pNDArrayPool->alloc(2, dims, NDInt32, 0, NULL);
            epicsSnprintf(statusMessage, sizeof(statusMessage), "Reading from File %s", fullFileName);
            setStringParam(ADStatusMessage, statusMessage);
            callParamCallbacks();
            status = readSFRM(fullFileName, &startTime, acquireTime + readSFRMTimeout, pImage); 
            /* If there was an error jump to bottom of loop */
            if (status) {
                setIntegerParam(ADAcquire, 0);
                pImage->release();
                goto done;
            } 

            /* Put the frame number and time stamp into the buffer */
            pImage->uniqueId = imageCounter;
            pImage->timeStamp = startTime.secPastEpoch + startTime.nsec / 1.e9;

            /* Get any attributes that have been defined for this driver */        
            this->getAttributes(pImage->pAttributeList);

            /* Call the NDArray callback */
            /* Must release the lock here, or we can get into a deadlock, because we can
             * block on the plugin lock, and the plugin can be calling us */
            this->unlock();
            asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, 
                 "%s:%s: calling NDArray callback\n", driverName, functionName);
            doCallbacksGenericPointer(pImage, NDArrayData, 0);
            this->lock();
            /* Free the image buffer */
            pImage->release();
        }
        getIntegerParam(ADImageMode, &imageMode);
        if (imageMode == ADImageMultiple) {
            getIntegerParam(ADNumImages, &numImages);
            if (numImagesCounter >= numImages) setIntegerParam(ADAcquire, 0);
        }    
        if (imageMode == ADImageSingle) setIntegerParam(ADAcquire, 0);
        done:
        callParamCallbacks();
    }
}

/** Called when asyn clients call pasynInt32->write().
  * This function performs actions for some parameters, including ADAcquire, ADTriggerMode, etc.
  * For all parameters it sets the value in the parameter library and calls any registered callbacks..
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Value to write. */
asynStatus BISDetector::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    int function = pasynUser->reason;
    int adstatus;
    int maxSizeX;
    asynStatus status = asynSuccess;
    const char *functionName = "writeInt32";

    status = setIntegerParam(function, value);

    if (function == ADAcquire) {
        getIntegerParam(ADStatus, &adstatus);
        if (value && (adstatus == ADStatusIdle)) {
            /* Send an event to wake up the BIS task.  */
            epicsEventSignal(this->startEventId);
        } 
        if (!value && (adstatus != ADStatusIdle)) {
            /* This was a command to stop acquisition */
            epicsEventSignal(this->stopEventId);
        }
    } 
    else if (function == ADBinX) {
        getIntegerParam(ADMaxSizeX, &maxSizeX);
        if ((value == 1) || (value == 2) || (value == 4) || (value == 8)) {
            /* There is only 1 binning, set X and Y the same */
            setIntegerParam(ADBinY, value);
            epicsSnprintf(this->toBIS, sizeof(this->toBIS), 
                "[ChangeFrameSize /FrameSize=%d]", maxSizeX/value);
            writeBIS(BIS_DEFAULT_TIMEOUT);
        } else {
            asynPrint(pasynUserSelf, ASYN_TRACE_ERROR,
                "%s:%s: invalid binning=%d, must be 1,2,4 or 8\n",
                driverName, functionName, value);
            status = asynError;
        } 
    } else {
        /* If this parameter belongs to a base class call its method */
        if (function < FIRST_BIS_PARAM) status = ADDriver::writeInt32(pasynUser, value);
    }
            
    /* Do callbacks so higher layers see any changes */
    callParamCallbacks();
    
    if (status) 
        asynPrint(pasynUser, ASYN_TRACE_ERROR, 
              "%s:%s: error, status=%d function=%d, value=%d\n", 
              driverName, functionName, status, function, value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, value=%d\n", 
              driverName, functionName, function, value);
    return status;
}


/** Report status of the driver.
  * Prints details about the driver if details>0.
  * It then calls the ADDriver::report() method.
  * \param[in] fp File pointed passed by caller where the output is written to.
  * \param[in] details If >0 then driver details are printed.
  */
void BISDetector::report(FILE *fp, int details)
{

    fprintf(fp, "BIS detector %s\n", this->portName);
    if (details > 0) {
        int nx, ny, dataType;
        getIntegerParam(ADSizeX, &nx);
        getIntegerParam(ADSizeY, &ny);
        getIntegerParam(NDDataType, &dataType);
        fprintf(fp, "  NX, NY:            %d  %d\n", nx, ny);
        fprintf(fp, "  Data type:         %d\n", dataType);
    }
    /* Invoke the base class method */
    ADDriver::report(fp, details);
}

extern "C" int BISDetectorConfig(const char *portName, const char *commandPort, const char *statusPort,
                                   int maxBuffers, size_t maxMemory,
                                   int priority, int stackSize)
{
    new BISDetector(portName, commandPort, statusPort, maxBuffers, maxMemory,
                        priority, stackSize);
    return(asynSuccess);
}

/** Constructor for BIS driver; most parameters are simply passed to ADDriver::ADDriver.
  * After calling the base class constructor this method creates a thread to collect the detector data, 
  * and sets reasonable default values for the parameters defined in this class, asynNDArrayDriver, and ADDriver.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] commandPort The name of the asyn port previously created with drvAsynIPPortConfigure to
  *            send commands to BIS.
  * \param[in] statusPort The name of the asyn port previously created with drvAsynIPPortConfigure to
  *            receive status information from BIS.
  * \param[in] maxBuffers The maximum number of NDArray buffers that the NDArrayPool for this driver is 
  *            allowed to allocate. Set this to -1 to allow an unlimited number of buffers.
  * \param[in] maxMemory The maximum amount of memory that the NDArrayPool for this driver is 
  *            allowed to allocate. Set this to -1 to allow an unlimited amount of memory.
  * \param[in] priority The thread priority for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags.
  * \param[in] stackSize The stack size for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags.
  */
BISDetector::BISDetector(const char *portName, const char *commandPort, const char *statusPort,
                             int maxBuffers, size_t maxMemory,
                             int priority, int stackSize)

    : ADDriver(portName, 1, NUM_BIS_PARAMS, maxBuffers, maxMemory,
               0, 0,             /* No interfaces beyond those set in ADDriver.cpp */
               ASYN_CANBLOCK, 1, /* ASYN_CANBLOCK=1, ASYN_MULTIDEVICE=0, autoConnect=1 */
               priority, stackSize)
{
    int status = asynSuccess;
    epicsTimerQueueId timerQ;
    const char *functionName = "BISDetector";

    createParam(BISSFRMTimeoutString,  asynParamFloat64, &BISSFRMTimeout);
    createParam(BISNumDarksString,     asynParamInt32,   &BISNumDarks);
    createParam(BISStatusString,       asynParamOctet,   &BISStatus);

    /* Create the epicsEvents for signaling to the BIS task when acquisition starts and stops */
    this->startEventId = epicsEventMustCreate(epicsEventEmpty);
    this->stopEventId = epicsEventMustCreate(epicsEventEmpty);
    this->readoutEventId = epicsEventMustCreate(epicsEventEmpty);
    
    /* Create the epicsTimerQueue for exposure time handling */
    timerQ = epicsTimerQueueAllocate(1, epicsThreadPriorityScanHigh);
    this->timerId = epicsTimerQueueCreateTimer(timerQ, timerCallbackC, this);

    /* Connect to BIS */
    status = pasynOctetSyncIO->connect(commandPort, 0, &this->pasynUserCommand, NULL);
    status = pasynOctetSyncIO->connect(statusPort, 0,  &this->pasynUserStatus, NULL);

    /* Set some default values for parameters */
    status =  setStringParam (ADManufacturer, "Bruker");
    status |= setStringParam (ADModel, "BIS");
    status |= setIntegerParam(ADMaxSizeX, 4096);
    status |= setIntegerParam(ADMaxSizeY, 4096);
    status |= setIntegerParam(ADSizeX, 0);
    status |= setIntegerParam(ADSizeY, 0);
    status |= setIntegerParam(NDArraySizeX, 0);
    status |= setIntegerParam(NDArraySizeY, 0);
    status |= setIntegerParam(NDArraySize, 0);
    status |= setIntegerParam(NDDataType,  NDUInt32);
    status |= setIntegerParam(ADImageMode, ADImageContinuous);
       
    if (status) {
        printf("%s: unable to set camera parameters\n", functionName);
        return;
    }
    
    /* Create the thread that updates the images */
    status = (epicsThreadCreate("BISDetTask",
                                epicsThreadPriorityMedium,
                                epicsThreadGetStackSize(epicsThreadStackMedium),
                                (EPICSTHREADFUNC)BISTaskC,
                                this) == NULL);
    if (status) {
        printf("%s:%s epicsThreadCreate failure for image task\n", 
            driverName, functionName);
        return;
    }

    /* Create the thread that reads the status socket */
    status = (epicsThreadCreate("BISStatusTask",
                                epicsThreadPriorityMedium,
                                epicsThreadGetStackSize(epicsThreadStackMedium),
                                (EPICSTHREADFUNC)statusTaskC,
                                this) == NULL);
    if (status) {
        printf("%s:%s epicsThreadCreate failure for status task\n", 
            driverName, functionName);
        return;
    }
}

/* Code for iocsh registration */
static const iocshArg BISDetectorConfigArg0 = {"Port name", iocshArgString};
static const iocshArg BISDetectorConfigArg1 = {"BIS port name", iocshArgString};
static const iocshArg BISDetectorConfigArg2 = {"Status port name", iocshArgString};
static const iocshArg BISDetectorConfigArg3 = {"maxBuffers", iocshArgInt};
static const iocshArg BISDetectorConfigArg4 = {"maxMemory", iocshArgInt};
static const iocshArg BISDetectorConfigArg5 = {"priority", iocshArgInt};
static const iocshArg BISDetectorConfigArg6 = {"stackSize", iocshArgInt};
static const iocshArg * const BISDetectorConfigArgs[] =    {&BISDetectorConfigArg0,
                                                            &BISDetectorConfigArg1,
                                                            &BISDetectorConfigArg2,
                                                            &BISDetectorConfigArg3,
                                                            &BISDetectorConfigArg4,
                                                            &BISDetectorConfigArg5,
                                                            &BISDetectorConfigArg6};
static const iocshFuncDef configBISDetector = {"BISDetectorConfig", 7, BISDetectorConfigArgs};
static void configBISDetectorCallFunc(const iocshArgBuf *args)
{
    BISDetectorConfig(args[0].sval, args[1].sval, args[2].sval, args[3].ival,  args[4].ival,  
                      args[5].ival, args[6].ival);
}

static void BISDetectorRegister(void)
{

    iocshRegister(&configBISDetector, configBISDetectorCallFunc);
}

extern "C" {
epicsExportRegistrar(BISDetectorRegister);
}

