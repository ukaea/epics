/********************************************
 *  p6kAxis.cpp
 * 
 *  P6K Asyn motor based on the 
 *  asynMotorAxis class.
 * 
 *  Matt Pearson
 *  26 March 2014
 * 
 ********************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsExport.h>
#include <epicsExit.h>
#include <epicsString.h>
#include <epicsStdio.h>
#include <iocsh.h>

#include "asynOctetSyncIO.h"
#include "asynInt32SyncIO.h"

#include "parker6kController.h"
#include <iostream>
#include <limits>
using std::cout;
using std::endl;

static const char *driverName = "p6kAxis";


/* TAS Status Bits (position in char array, not TAS bit position) */
const epicsUInt32 p6kAxis::P6K_TAS_MOVING_        = 0;
const epicsUInt32 p6kAxis::P6K_TAS_DIRECTION_     = 1;
const epicsUInt32 p6kAxis::P6K_TAS_ACCELERATING_  = 2;
const epicsUInt32 p6kAxis::P6K_TAS_ATVELOCITY_    = 3;
const epicsUInt32 p6kAxis::P6K_TAS_HOMED_         = 5;
const epicsUInt32 p6kAxis::P6K_TAS_ABSOLUTE_      = 6;
const epicsUInt32 p6kAxis::P6K_TAS_CONTINUOUS_    = 7;
const epicsUInt32 p6kAxis::P6K_TAS_JOG_           = 8;
const epicsUInt32 p6kAxis::P6K_TAS_JOYSTICK_      = 10;
const epicsUInt32 p6kAxis::P6K_TAS_STALL_         = 13;
const epicsUInt32 p6kAxis::P6K_TAS_DRIVE_         = 15;
const epicsUInt32 p6kAxis::P6K_TAS_DRIVEFAULT_    = 16;
const epicsUInt32 p6kAxis::P6K_TAS_POSLIM_        = 17;
const epicsUInt32 p6kAxis::P6K_TAS_NEGLIM_        = 18;
const epicsUInt32 p6kAxis::P6K_TAS_POSLIMSOFT_    = 20;
const epicsUInt32 p6kAxis::P6K_TAS_NEGLIMSOFT_    = 21;
const epicsUInt32 p6kAxis::P6K_TAS_POSERROR_      = 27;
const epicsUInt32 p6kAxis::P6K_TAS_TARGETZONE_    = 28;
const epicsUInt32 p6kAxis::P6K_TAS_TARGETTIMEOUT_ = 30;
const epicsUInt32 p6kAxis::P6K_TAS_GOWHENPEND_    = 31;
const epicsUInt32 p6kAxis::P6K_TAS_MOVEPEND_      = 33; 
const epicsUInt32 p6kAxis::P6K_TAS_PREEMPT_       = 36;

const epicsUInt32 p6kAxis::P6K_STEPPER_     = 0;
const epicsUInt32 p6kAxis::P6K_SERVO_       = 1;

const epicsUInt32 p6kAxis::P6K_LIM_DISABLE_ = 0;
const epicsUInt32 p6kAxis::P6K_LIM_ENABLE_  = 3;

const char * p6kAxis::P6K_DRIVE_SHUTDOWN_STR_ = "DRIVE SHUTDOWN";

/**
 * Asyn shutdown function
 */
static void shutdownCallback(void *pPvt)
{
  p6kController *pC = static_cast<p6kController *>(pPvt);

  pC->lock();
  pC->shuttingDown_ = 1;
  pC->unlock();
}

/**
 * p6kAxis constructor. This initializes all the data members and asyn
 * paramLib parameters. It also triggers a poll.
 * @param pC Pointer to a p6kController object.
 * @param axisNo The axis number for this p6kAxis (1 based).
 */
p6kAxis::p6kAxis(p6kController *pC, int32_t axisNo)
  :   asynMotorAxis(pC, axisNo),
      pC_(pC)
{
  static const char *functionName = "p6kAxis::p6kAxis";

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  if (axisNo > pC_->numAxes_-1) {
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
      "%s ERROR: Axis number out of range. Max: %d\n", 
      functionName, pC_->numAxes_-1);
    return;
  }

  //Initialize non-static data members
  deferredPosition_ = 0.0;
  deferredMove_ = 0;
  deferredRelative_ = 0;
  nowTimeSecs_ = 0.0;
  lastTimeSecs_ = 0.0;
  doneTimeSecs_ = 0.0;
  movingLastPoll_ = false;
  delayDoneMove_ = false;
  printNextError_ = true;
  printErrors_ = true;
  commandError_ = false;
  axisError_ = false;
  driveType_ = P6K_STEPPER_;
  modbusEncPort_ = NULL;

  p6k_cmddir_ = 0;
  p6k_drfen_ = 0;
  p6k_encpol_ = 0;
  p6k_esk_ = 0;
  p6k_estall_ = 0;

  /* Set an EPICS exit handler that will shut down polling before asyn kills the IP sockets */
  epicsAtExit(shutdownCallback, pC_);

  //Initialise some axis specific parameters
  bool paramStatus = true;
  paramStatus = ((setIntegerParam(pC_->P6K_A_DRES_, 0) == asynSuccess) && paramStatus);
  paramStatus = ((setIntegerParam(pC_->P6K_A_ERES_, 0) == asynSuccess) && paramStatus);
  paramStatus = ((setIntegerParam(pC_->P6K_A_MaxDigits_, pC_->P6K_MAX_DIGITS_) == asynSuccess) && paramStatus);
  //NOTE: on 1/16/18 I modified this to always set motorStatusHasEncoder_ = 1. This makes it easier
  //      to switch to using an external PV based encoder value. If we are not using an external encoder, 
  //      and the controller doesn't have an encoder, it's still ok to set this to 1 because the 
  //      the controller puts the motor step count in the encoder register anyway. 
  paramStatus = ((setIntegerParam(pC_->motorStatusHasEncoder_, 1) == asynSuccess) && paramStatus);
  paramStatus = ((setIntegerParam(pC_->motorStatusGainSupport_, 1) == asynSuccess) && paramStatus);
  paramStatus = ((setStringParam(pC_->P6K_A_Command_, " ") == asynSuccess) && paramStatus);
  paramStatus = ((setStringParam(pC_->P6K_A_Response_, " ") == asynSuccess) && paramStatus);
  paramStatus = ((setIntegerParam(pC_->P6K_A_LS_, 0) == asynSuccess) && paramStatus);
  paramStatus = ((setIntegerParam(pC_->P6K_A_LH_, 0) == asynSuccess) && paramStatus);
  paramStatus = ((setStringParam(pC_->P6K_A_Error_, " ") == asynSuccess) && paramStatus);
  paramStatus = ((setStringParam(pC_->P6K_A_MoveError_, " ") == asynSuccess) && paramStatus);
  paramStatus = ((setIntegerParam(pC_->P6K_A_TAS_DriveFault_, 0) == asynSuccess) && paramStatus);
  paramStatus = ((setIntegerParam(pC_->P6K_A_TAS_Timeout_, 0) == asynSuccess) && paramStatus);
  paramStatus = ((setIntegerParam(pC_->P6K_A_TAS_PosErr_, 0) == asynSuccess) && paramStatus);
  paramStatus = ((setIntegerParam(pC_->P6K_A_ModbusEncoder_, 0) == asynSuccess) && paramStatus);
  paramStatus = ((setIntegerParam(pC_->P6K_A_ModbusEncoderAddr_, 0) == asynSuccess) && paramStatus);
  paramStatus = ((setIntegerParam(pC_->P6K_A_ModbusEncoderOffset_, 0) == asynSuccess) && paramStatus);
  // TZ: added TDAC and SOFFS
  paramStatus = ((setDoubleParam(pC_->P6K_A_DriveVoltage_, 0.0) == asynSuccess) && paramStatus);
  paramStatus = ((setDoubleParam(pC_->P6K_A_VoltageOffset_, 0.0) == asynSuccess) && paramStatus);
  

  if (!paramStatus) {
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
      "%s Unable To Set Driver Parameters In Constructor. Axis:%d\n", 
      functionName, axisNo_);
  }
  
  //Do an initial poll to get some values from the P6K
  if (axisNo_ > 0) {
    if (getAxisInitialStatus() != asynSuccess) {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR,
        "%s: getAxisInitialStatus failed to return asynSuccess. Controller: %s, Axis: %d.\n", 
        functionName, pC_->portName, axisNo_);
      return;
    }
  }
 
  callParamCallbacks();

  /* Wake up the poller task which will make it do a poll */   
  pC_->wakeupPoller();
}

/**
 * Connect to the modbus asyn port.
 * This function is used if we are creating Axis objects using 
 * the p6kController::p6kCreateModbusEncAxis function. This will allow
 * us to read the encoder position over Modbus.
 * This uses the asynInt32SyncIO interface, and passes the INT32_BE message to the modbus driver.
 * NOTE: This is assuming we never read more than 4 bytes worth of counts. If we find we do
 *       then we'll have to add the second address (typically the existing
 *       address+2), and use a second asynUser, and then in the driver apply a scaling 
 *       factor so we lose some precision - this would be necessary in order to fit into the motor
 *       record limitation of 4-byte positions.  
 */
asynStatus p6kAxis::modbusPortConnect(const char *modbusPort, int modbusAddr, int modbusOffset)
{
  asynStatus status = asynSuccess;
 
  static const char *functionName = "p6kAxis::modbusPortConnect";

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  status = pasynInt32SyncIO->connect(modbusPort, modbusAddr, &this->modbusEncPort_, "INT32_BE");
  if ((status) || (this->modbusEncPort_ == NULL)) {
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR,
      "p6kAxis::modbusPortConnect: unable to connect to modbus port %s, addr %d\n", 
      modbusPort, modbusAddr);
    return status;
  }

  modbusEncAddr_ = modbusAddr;
  modbusEncOffset_ = modbusOffset;

  setIntegerParam(pC_->P6K_A_ModbusEncoder_, 1);
  setIntegerParam(pC_->P6K_A_ModbusEncoderAddr_, modbusEncAddr_);
  setIntegerParam(pC_->P6K_A_ModbusEncoderOffset_, modbusEncOffset_);
  
  return status;
}

/**
 * Wrapper for common read int param operation at startup.
 * @param cmd The command to send
 * @param param The asyn param to set with the result
 * @param val The result read back
 * @return asynStatus
 */
asynStatus p6kAxis::readIntParam(const char *cmd, epicsUInt32 param, uint32_t *val)
{
  char command[P6K_MAXBUF] = {0};
  char response[P6K_MAXBUF] = {0};
  char scan[P6K_MAXBUF] = {0};
  uint32_t nvals = 0;
  uint32_t axisNum = 0;
  asynStatus status = asynSuccess; 

  static const char *functionName = "p6kAxis::readIntParam";
  
  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  epicsSnprintf(command, P6K_MAXBUF, "%d%s", axisNo_, cmd);
  status = pC_->lowLevelWriteRead(command, response);
  if (status == asynSuccess) {
    epicsSnprintf(scan,  P6K_MAXBUF, "%%d%s%%d", cmd);
    nvals = sscanf(response, scan, &axisNum, val);    
    if (nvals == 2) {
      if (param != 0) {
        setIntegerParam(param, *val);
      }
      status = asynSuccess;
    } else {
      status = asynError;
    }
  }

  if (status != asynSuccess) {
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
	      "%s ERROR: Failed to read %s at startup.\n", functionName, cmd);
  }
  
  return status;
} 

/**
 * Wrapper for common read double param operation at startup.
 * @param cmd The command to send
 * @param param The asyn param to set with the result
 * @param val The result read back
 * @return asynStatus
 */
asynStatus p6kAxis::readDoubleParam(const char *cmd, epicsUInt32 param, double *val)
{
  char command[P6K_MAXBUF] = {0};
  char response[P6K_MAXBUF] = {0};
  char scan[P6K_MAXBUF] = {0};
  uint32_t nvals = 0;
  uint32_t axisNum = 0;
  asynStatus status = asynSuccess; 

  static const char *functionName = "p6kAxis::readDoubleParam";
  
  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  epicsSnprintf(command, P6K_MAXBUF, "%d%s", axisNo_, cmd);
  status = pC_->lowLevelWriteRead(command, response);
  if (status == asynSuccess) {
    epicsSnprintf(scan,  P6K_MAXBUF, "%%d%s%%lf", cmd);
    nvals = sscanf(response, scan, &axisNum, val);
    if (nvals == 2) {
      if (param != 0) {
        setDoubleParam(param, *val);
      }
      status = asynSuccess;
    } else {
      status = asynError;
    }
  }

  if (status != asynSuccess) {
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
      "%s ERROR: Failed to read %s at startup.\n", functionName, cmd);
  }

  return status;
} 


/**
 * Poll for initial axis status (soft limits, PID settings).
 * Set parameters needed for correct motor record behaviour.
 * @return asynStatus
 */
asynStatus p6kAxis::getAxisInitialStatus(void)
{
  uint32_t intVal = 0;
  int32_t integerVal = 0;
  double doubleVal = 0.0;  
  bool stat = true;

  static const char *functionName = "p6kAxis::getAxisInitialStatus";

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  if (axisNo_ != 0) {
    char command[P6K_MAXBUF] = {0};
    char response[P6K_MAXBUF] = {0};
    strncpy(command, P6K_CMD_TREV, P6K_MAXBUF);
    stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
    std::string revisionStr(response);
    if(revisionStr.find(" GEM6K GT6K") != std::string::npos) {
      driveType_ = P6K_STEPPER_;
    } else if(revisionStr.find(" GEM6K GV6K") != std::string::npos) {
      driveType_ = P6K_SERVO_;
    } else if(revisionStr.find(" 6K") != std::string::npos) {
      stat = (readIntParam(P6K_CMD_AXSDEF, pC_->P6K_A_AXSDEF_, &intVal) == asynSuccess) && stat;
      if (stat) {
        driveType_ = intVal;
      }
    } else {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR,
        "%s ERROR: Unsupported controller model.\n", functionName);
    }

    stat = (readIntParam(P6K_CMD_DRES, pC_->P6K_A_DRES_, &intVal) == asynSuccess) && stat;
    stat = (readIntParam(P6K_CMD_ERES, pC_->P6K_A_ERES_, &intVal) == asynSuccess) && stat;
    stat = (readIntParam(P6K_CMD_DRIVE, pC_->motorStatusPowerOn_, &intVal) == asynSuccess) && stat;
    //Don't bother reading ENCCNT and setting motorStatusHasEncoder_ (see note in constructor made on 1/16/18).
    // TZ: We need to read this for steppers. No harm in reading.
    if (driveType_ == P6K_STEPPER_) {
      stat = (readIntParam(P6K_CMD_ENCCNT, pC_->motorStatusHasEncoder_, &intVal) == asynSuccess) && stat;
    } else {
      // TS: For DCs read SFB. ENCCNT will read '-' for DC's most of CLF DC's don't have Encoders attached we need a way to switch off encoders.
      stat = (readIntParam(P6K_CMD_SFB, pC_->motorStatusHasEncoder_, &intVal) == asynSuccess) && stat;
    }

    if (stat) {
      hasEncoder_ = intVal;
    } else {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR,
        "%s ERROR: Cannot verify if axis has an encoder.\n", functionName);
    }

    stat = (readIntParam(P6K_CMD_LH, pC_->P6K_A_LH_, &intVal) == asynSuccess) && stat;
    stat = (readIntParam(P6K_CMD_LS, pC_->P6K_A_LS_, &intVal) == asynSuccess) && stat;
    stat = (readDoubleParam(P6K_CMD_LSPOS, pC_->motorHighLimit_, &doubleVal) == asynSuccess) && stat;
    stat = (readDoubleParam(P6K_CMD_LSNEG, pC_->motorLowLimit_, &doubleVal) == asynSuccess) && stat;

    //Read some params that don't need to go in paramLib.
    stat = (readIntParam(P6K_CMD_CMDDIR, 0, &p6k_cmddir_) == asynSuccess) && stat;
    stat = (readIntParam(P6K_CMD_DRFEN,  0, &p6k_drfen_) == asynSuccess) && stat;
    stat = (readIntParam(P6K_CMD_ENCPOL, 0, &p6k_encpol_) == asynSuccess) && stat;
    
    if(driveType_ != P6K_SERVO_) {    // TS: Encoders should always be fitted with DC servo's only read these for steppers.
      stat = (readIntParam(P6K_CMD_ESK,    0, &p6k_esk_) == asynSuccess) && stat;
      stat = (readIntParam(P6K_CMD_ESTALL, 0, &p6k_estall_) == asynSuccess) && stat;
    }

    // TZ... TDAC and SOFFS
    stat = (readDoubleParam(P6K_CMD_TDAC, pC_->P6K_A_DriveVoltage_, &doubleVal) == asynSuccess) && stat;
    if (stat) {
      driveVoltage_ = doubleVal;
    } else {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR,
        "%s ERROR: Cannot verify drive voltage.\n", functionName);
    }

    stat = (readDoubleParam(P6K_CMD_SOFFS, pC_->P6K_A_VoltageOffset_, &doubleVal) == asynSuccess) && stat;
    if (stat) {
      voltageOffset_ = doubleVal;
    } else {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR,
        "%s ERROR: Cannot verify voltage offset.\n", functionName);
    }

    // TS: Hold voltage for DC's commanded by SOFFS  
    stat = pC_->getDoubleParam(axisNo_,pC_->P6K_A_HoldVoltage_, &doubleVal);    
    if (stat) {
      servoHoldVoltage_ = doubleVal;
      //TS: If SOFFS per axis needs to be configured at reboot the hold value should be amended in the AXIS_CONFIG files that get sent in p6kUpload
    } else {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR,
        "%s ERROR: Cannot verify hold voltage.\n", functionName);
    }

    // TS: Status for commanded TWF or TWR.
    stat = pC_->getIntegerParam(axisNo_,pC_->P6K_A_TargetDirection_, &integerVal);
    if (stat) {
      targetDirection_ = integerVal;
    } else {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR,
        "%s ERROR: Cannot verify target direction.\n", functionName);
    }
  }

  if (!stat) {
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
      "%s ERROR: Could not read all axis parameters at startup.\n", functionName);
    setIntegerParam(pC_->motorStatusCommsError_, 1);
    return asynError;
  } else {
    printAxisParams();
    return asynSuccess;
  }
  
  return asynSuccess;
}

/**
 * Destructor.
 */
p6kAxis::~p6kAxis() 
{
  //We should never get here.
}

/**
 * Print axis parameters that have been read at startup.
 */
void p6kAxis::printAxisParams(void)
{
  int32_t  intVal = 0;
  int32_t  axisDef = 0;
  double doubleVal = 0.0;
  
  printf("Axis %d\n", axisNo_);
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_AXSDEF_, &axisDef);
  printf("  " P6K_CMD_AXSDEF ": %d\n", axisDef);
  if (static_cast<epicsUInt32>(axisDef) == P6K_STEPPER_) {
    printf("  Stepper Drive\n");
  } else if (static_cast<epicsUInt32>(axisDef) == P6K_SERVO_) {
    printf("  Servo Drive\n");
  } else {
    printf("  Unknown Drive Type\n");
  }
  printf("  " P6K_CMD_CMDDIR ": %d\n", p6k_cmddir_);
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_DRES_, &intVal);  
  printf("  " P6K_CMD_DRES ": %d\n", intVal);
  printf("  " P6K_CMD_DRFEN ": %d\n", p6k_drfen_);
  pC_->getIntegerParam(axisNo_, pC_->motorStatusPowerOn_, &intVal);
  printf("  " P6K_CMD_DRIVE ": %d\n", intVal);
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_ERES_, &intVal);
  printf("  " P6K_CMD_ERES ": %d\n", intVal);
  //Don't bother reading ENCCNT (see note in constructor made on 1/16/18).
  // TZ: We need to know this for open loop control
  pC_->getIntegerParam(axisNo_, pC_->motorStatusHasEncoder_, &intVal);
  printf("  " P6K_CMD_ENCCNT ": %d\n", intVal);
  printf("  " P6K_CMD_ENCPOL ": %d\n", p6k_encpol_);
  // TZ: Only needs to be read for steppers
  if (axisDef == P6K_STEPPER_){
    printf("  " P6K_CMD_ESK ": %d\n", p6k_esk_);
    printf("  " P6K_CMD_ESTALL ": %d\n", p6k_estall_);
  }
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_LS_, &intVal);
  printf("  " P6K_CMD_LS ": %d\n", intVal);
  if (static_cast<epicsUInt32>(intVal) != P6K_LIM_ENABLE_) {
    printf("  WARNING: One or both soft limits are disabled.\n");
  }
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_LH_, &intVal);
  printf("  " P6K_CMD_LH ": %d\n", intVal);
  if (static_cast<epicsUInt32>(intVal) != P6K_LIM_ENABLE_) {
    printf("  WARNING: One or both hard limits are disabled.\n");
  }
  pC_->getDoubleParam(axisNo_, pC_->motorHighLimit_, &doubleVal);
  printf("  " P6K_CMD_LSPOS ": %f\n", doubleVal);
  pC_->getDoubleParam(axisNo_, pC_->motorLowLimit_, &doubleVal);
  printf("  " P6K_CMD_LSNEG ": %f\n", doubleVal);

  // TZ: added TDAC and SOFFS
  pC_->getDoubleParam(axisNo_, pC_->P6K_A_DriveVoltage_, &doubleVal);
  printf("  " P6K_CMD_TDAC ": %f\n", doubleVal);
  pC_->getDoubleParam(axisNo_, pC_->P6K_A_VoltageOffset_, &doubleVal);
  printf("  " P6K_CMD_SOFFS ": %f\n", doubleVal);
}

/**
 * See asynMotorAxis::move
 */
asynStatus p6kAxis::move(double position, int32_t relative, double min_velocity, double max_velocity, double acceleration)
{
  asynStatus status = asynError;
  char command[P6K_MAXBUF]  = {0};
  char response[P6K_MAXBUF] = {0};
  static const char *functionName = "p6kAxis::move";

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  int32_t maxDigits = 0;
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_MaxDigits_, &maxDigits);

  int32_t scale = getScaleFactor();
  if (scale == 0) {
    return asynError;
  }

  // If the limit drive mode is active we do not send commands to the
  // controller that cannot be carried out because the corresponding limit
  // switch already active. These commands would cause
  // "INVALID CONDITIONS FOR COMMAND-AXIS" Asyn errors as well as STATE MAJOR
  // alarms.
  int32_t limitDriveEnable = 0;
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_LimitDriveEnable_, &limitDriveEnable);

  if (limitDriveEnable) {
    bool moving = true;
    getAxisStatus(&moving);

    int32_t highLimitHit = 0;
    pC_->getIntegerParam(axisNo_, pC_->motorStatusHighLimit_, &highLimitHit);

    int32_t lowLimitHit = 0;
    pC_->getIntegerParam(axisNo_, pC_->motorStatusLowLimit_, &lowLimitHit);

    double motorPosition = 0;
    pC_->getDoubleParam(axisNo_, pC_->motorPosition_, &motorPosition);

    if ((highLimitHit && (position >= pC_->motorPosition_)) ||
        (lowLimitHit && (position <= pC_->motorPosition_))) {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_WARNING,
              "%s: skip move since corresponding limit switch is already in NOK state.\n",
              functionName);
      return asynSuccess;
    }
  }

  //Enable the drive if we are using this drivers parameter to control power.
  //NOTE: this function will fail if the drive is not on.
  if (autoDriveEnable() != asynSuccess) {
    return asynError;
  }

  if (relative > 1) {
    relative = 1;
  }
  epicsSnprintf(command, P6K_MAXBUF, "%d%s%d", axisNo_, P6K_CMD_MA, !relative);
  status = pC_->lowLevelWriteRead(command, response);
  memset(command, 0, sizeof(command));

  //If SendPositionOnly is active, then we don't want to set velocity and accel params
  int32_t sendPositionOnly = 0;
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_SendPositionOnly_, &sendPositionOnly);

  if (sendPositionOnly == 0) {
    if (max_velocity != 0) {
      epicsFloat64 vel = max_velocity / scale;
      epicsSnprintf(command, P6K_MAXBUF, "%d%s%.*f", axisNo_, P6K_CMD_V, maxDigits, vel);
      status = pC_->lowLevelWriteRead(command, response);
      memset(command, 0, sizeof(command));
    }
  }

  epicsFloat64 accel = acceleration / scale;

  // Make sure 1/2 A <= AA <= A as required per command reference.
  // Use int arithmetic to ensure we don't run into rounding issues.
  int iA = rint(pow(10.0, maxDigits) * accel);
  int iAA = (iA % 2) ? iA / 2 + 1 : iA / 2;
  double dA = iA / pow(10.0, maxDigits);
  double dAA = iAA / pow(10.0, maxDigits);

  if (sendPositionOnly == 0) {
    if (iA != 0) {
      if (max_velocity != 0) {
        epicsSnprintf(command, P6K_MAXBUF, "%d%s%.*f", axisNo_, P6K_CMD_A, maxDigits, dA);
        status = pC_->lowLevelWriteRead(command, response);
        memset(command, 0, sizeof(command));
        
        //Set S curve parameters too
        epicsSnprintf(command, P6K_MAXBUF, "%d%s%.*f", axisNo_, P6K_CMD_AA, maxDigits, dAA);
        status = pC_->lowLevelWriteRead(command, response);
        memset(command, 0, sizeof(command));
        
        epicsSnprintf(command, P6K_MAXBUF, "%d%s%.*f", axisNo_, P6K_CMD_AD, maxDigits, dA);
        status = pC_->lowLevelWriteRead(command, response);
        memset(command, 0, sizeof(command));
        
        epicsSnprintf(command, P6K_MAXBUF, "%d%s%.*f", axisNo_, P6K_CMD_ADA, maxDigits, dA);
        status = pC_->lowLevelWriteRead(command, response);
        memset(command, 0, sizeof(command));
            } else {
        asynPrint(pC_->pasynUserSelf, ASYN_TRACE_WARNING,
          "%s: maximum velocity too small (exactly 0 or close to 0). Skip setting S curve parameters.\n",
          functionName);
      }
    } else {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_WARNING,
        "%s: acceleration too small (exactly 0 or close to 0). Skip setting S curve parameters.\n",
        functionName);
    }
  } //end if (sendPositionOnly == 0)

  //Don't set position if we are doing deferred moves.
  //In case we cancel the deferred move.
  epicsUInt32 pos = static_cast<epicsUInt32>(position);
  if (pC_->movesDeferred_ == 0) {
    epicsSnprintf(command, P6K_MAXBUF, "%d%s%d", axisNo_, P6K_CMD_D, pos);
    status = pC_->lowLevelWriteRead(command, response);
    memset(command, 0, sizeof(command));
    epicsSnprintf(command, P6K_MAXBUF, "%d%s", axisNo_, P6K_CMD_GO);
    movingLastPoll_ = true;
  } else { /* deferred moves */
    deferredPosition_ = pos;
    deferredMove_ = 1;
    //deferredRelative_ = relative; //This is already taken care of on the controller by the MA command
  }

  status = pC_->lowLevelWriteRead(command, response);

  //Detect a "DRIVE SHUTDOWN" error. Here we attempt to retry the drive enable.
  if (strstr(response, P6K_DRIVE_SHUTDOWN_STR_) != NULL) {
    int32_t retryDriveEnable = 0;
    pC_->getIntegerParam(axisNo_, pC_->P6K_A_DriveRetry_, &retryDriveEnable);
    if (retryDriveEnable == 1) {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
                "%s We detected a DRIVE SHUTDOWN on axis %d. Waiting 10s...\n", functionName, axisNo_);
      epicsThreadSleep(10);
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
                "%s Sending DRIVE1 again on axis %d...\n", functionName, axisNo_);
      sprintf(command, "%d%s1",  axisNo_, P6K_CMD_DRIVE);
      status = pC_->lowLevelWriteRead(command, response); 
      if (status == asynSuccess) {
        setIntegerParam(pC_->motorStatusPowerOn_, 1);
        asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
                  "%s Successfully sent DRIVE1 on axis %d. Now sending %dGO...\n", functionName, axisNo_, axisNo_);
        memset(command, 0, sizeof(command));
        epicsSnprintf(command, P6K_MAXBUF, "%d%s", axisNo_, P6K_CMD_GO);
        status = pC_->lowLevelWriteRead(command, response);
      }
    }
  }

  //Check the status of the GO command so we are notified of failed moves.
  if (status != asynSuccess) {
    setStringParam(pC_->P6K_A_MoveError_, response);
    commandError_ = true;
  } else {
    setStringParam(pC_->P6K_A_MoveError_, " ");
    commandError_ = false;
  }
  
  return status;
}

/**
 * Determine the scale factor to use for velocity and accel scaling 
 * which is required by the controller.
 */ 
int32_t p6kAxis::getScaleFactor(void)
{
  static const char *functionName = "p6kAxis::getScaleFactor";

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  //Read DRES and ERES for velocity and accel scaling
  int32_t dres = 0;
  int32_t eres = 0;
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_DRES_, &dres);
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_ERES_, &eres);
  int32_t scale = 0;
  if (driveType_ == P6K_SERVO_) {
    scale = eres;
  } else {
    scale = dres;
  }
  
  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s DRES=%d, ERES=%d\n", functionName, dres, eres);
  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s scale=%d\n", functionName, scale);

  return scale;
}


/**
 * Deal with automatic drive enable. If this is enabled then
 * the drive will be powered on. If a P6K_A_AutoDriveEnableDelay_
 * is set then this function will block until a timer expires.
 *
 * We should always call this in the move functions to make sure the
 * drive is powered on before sending a move, even if we are making use
 * use of the asynMotorController auto enable instead. This function 
 * will prevent sending a move command if the drive is not on, which can 
 * cause the next move to fail.
 */
asynStatus p6kAxis::autoDriveEnable(void)
{
  static const char *functionName = "p6kAxis::autoDriveEnable";

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  int32_t auto_drive_enable = 0;
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_AutoDriveEnable_, &auto_drive_enable);
  if (auto_drive_enable == 1) {
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, 
        "%s Auto drive enable\n", functionName);
    if (setClosedLoop(true) != asynSuccess) {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
        "%s ERROR: Failed to enable axis %d\n", functionName, axisNo_);
      setStringParam(pC_->P6K_A_MoveError_, "ERROR: Failed to enable drive");
      return asynError;
    }
  } else {
    //If auto_drive_enable is not on, check motor power is on. Return an error if not.
    //If we send move commands to the GT6K/6K controllers, without the power on
    //then the next move will fail even if we enable the power. So we try to
    //prevent that by this check.
    int32_t power = 0;
    pC_->getIntegerParam(axisNo_, pC_->motorStatusPowerOn_, &power);
    if (power == 0) {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
        "%s ERROR: Not sending move because drive is off. Axis: %d\n", functionName, axisNo_);
      setStringParam(pC_->P6K_A_MoveError_, "ERROR: Drive was off when attempting last move.");
      return asynError;
    }
  }

  int32_t drive_enable_delay = 0;
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_AutoDriveEnableDelay_, &drive_enable_delay);
  if (drive_enable_delay > 0) {
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, 
      "%s Auto drive enable delay: %d\n", functionName, drive_enable_delay);
    epicsThreadSleep(static_cast<double>(drive_enable_delay) / 1000.0);
  }

  return asynSuccess;
}


/**
 * See asynMotorAxis::home
 */ 
asynStatus p6kAxis::home(double min_velocity, double max_velocity, double acceleration, int32_t forwards)
{
  asynStatus status = asynError;
  char command[P6K_MAXBUF] = {0};
  char response[P6K_MAXBUF] = {0};
  static const char *functionName = "p6kAxis::home";

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  int32_t maxDigits = 0;
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_MaxDigits_, &maxDigits);

  int32_t scale = getScaleFactor();
  if (scale == 0) {
    return asynError;
  }

  //Enable the drive if we are using this drivers parameter to control power.
  //NOTE: this function will fail if the drive is not on.
  if (autoDriveEnable() != asynSuccess) {
    return asynError;
  }

  //If SendPositionOnly is active, then we don't want to set velocity and accel params
  int32_t sendPositionOnly = 0;
  pC_->getIntegerParam(axisNo_, pC_->P6K_A_SendPositionOnly_, &sendPositionOnly);

  if (sendPositionOnly == 0) {
    if (max_velocity != 0) {
      epicsFloat64 vel = max_velocity / scale;
      epicsSnprintf(command, P6K_MAXBUF, "%d%s%.*f", axisNo_, P6K_CMD_HOMV, maxDigits, vel);
      status = pC_->lowLevelWriteRead(command, response);
      memset(command, 0, sizeof(command));
    }
  }

  if (sendPositionOnly == 0) {
    if (acceleration != 0) {
      if (max_velocity != 0) {
        epicsFloat64 accel = acceleration / scale;
        epicsSnprintf(command, P6K_MAXBUF, "%d%s%.*f", axisNo_, P6K_CMD_HOMA, maxDigits, accel);
        status = pC_->lowLevelWriteRead(command, response);
        memset(command, 0, sizeof(command));

        //Set S curve parameters too
        epicsSnprintf(command, P6K_MAXBUF, "%d%s%.*f", axisNo_, P6K_CMD_HOMAA, maxDigits, accel/2);
        status = pC_->lowLevelWriteRead(command, response);
        memset(command, 0, sizeof(command));

        epicsSnprintf(command, P6K_MAXBUF, "%d%s%.*f", axisNo_, P6K_CMD_HOMAD, maxDigits, accel);
        status = pC_->lowLevelWriteRead(command, response);
        memset(command, 0, sizeof(command));

        epicsSnprintf(command, P6K_MAXBUF, "%d%s%.*f", axisNo_, P6K_CMD_HOMADA, maxDigits, accel);
        status = pC_->lowLevelWriteRead(command, response);
        memset(command, 0, sizeof(command));
      }
    }
  } // end if (sendPositionOnly == 0)
  
  epicsSnprintf(command, P6K_MAXBUF, "%d%s%d", axisNo_, P6K_CMD_HOM, (forwards>0?0:1));
  status = pC_->lowLevelWriteRead(command, response);
  memset(command, 0, sizeof(command));

  return status;
}

/**
 * See asynMotorAxis::moveVelocity
 */
asynStatus p6kAxis::moveVelocity(double min_velocity, double max_velocity, double acceleration)
{
  asynStatus status = asynError;
  char command[P6K_MAXBUF]  = {0};
  char response[P6K_MAXBUF] = {0};
  static const char *functionName = "p6kAxis::moveVelocity";

  if (driveType_ == P6K_SERVO_ && !hasEncoder_){
      // TS: set pos/neg for direction with DC openloop control.
      status = pC_->getIntegerParam(axisNo_, pC_->P6K_A_TargetDirection_, &targetDirection_);
      // TZ: write a voltage to SOFFS for openloop control.
      status = pC_->getDoubleParam(axisNo_, pC_->P6K_A_VoltageOffset_, &voltageOffset_);
            
      // TS: direction for tweaks & SOFFS systems
      char negSign = (bool)targetDirection_ ? '+' : '-';
      //printf("***DEBUG*** : directionality %d\n", targetDirection_);
      //printf("***DEBUG*** : OPEN LOOP");   //TS: Debug remove later.
      //printf("***DEBUG*** : 6K8 transmit %d%s%c%lf\n", axisNo_, P6K_CMD_SOFFS, negSign, voltageOffset_);

      epicsSnprintf(command, P6K_MAXBUF, "%d%s%c%lf", axisNo_, P6K_CMD_SOFFS, negSign, voltageOffset_);
      status = pC_->lowLevelWriteRead(command, response);
      memset(command, 0, sizeof(command)); 
  }

  //asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  //asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, "%s moveVelocity not implemented yet.\n", functionName);

  return status;
}

/**
 * See asynMotorAxis::setPosition
 */
asynStatus p6kAxis::setPosition(double position)
{
  asynStatus asynStatus = asynError;
  bool stat = true;
  char command[P6K_MAXBUF]  = {0};
  char response[P6K_MAXBUF] = {0};
  static const char *functionName = "p6kAxis::setPosition";
  
  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  /*Set position on motor axis.*/
  epicsInt32 pos = static_cast<epicsInt32>(floor(position + 0.5));

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, 
	    "%s: Set axis %d on controller %s to position %d\n", 
	    functionName, axisNo_, pC_->portName, pos);

  epicsSnprintf(command, P6K_MAXBUF, "!%d%s", axisNo_, P6K_CMD_S);
  stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
  memset(command, 0, sizeof(command));

  if (stat) {
    epicsSnprintf(command, P6K_MAXBUF, "%d%s%d", axisNo_, P6K_CMD_PSET, pos);
    stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
    memset(command, 0, sizeof(command));
  }

  /*Now set position on encoder axis.*/
  epicsFloat64 encRatio = 0.0;
  pC_->getDoubleParam(axisNo_, pC_->motorEncoderRatio_, &encRatio);
  epicsInt32 encpos = (epicsInt32) floor((position*encRatio) + 0.5);
  if (stat) {             
    if (encRatio != 0) {
      
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, 
        "%s: Set encoder axis %d on controller %s to position %d, encRatio: %f\n", 
        functionName, axisNo_, pC_->portName, pos, encRatio);
      
      epicsSnprintf(command, P6K_MAXBUF, "%d%s%d", axisNo_, P6K_CMD_PESET, encpos);
      stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
      memset(command, 0, sizeof(command));
    } else {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
        "%s: encRation is zero. Not setting encoder position.\n", 
        functionName);
    }
  }

  /*Set the motor and encoder position here, regardless of if any of the 
    above commands worked. This way the motor record has the correct position
    restored if the controller is off. Then autosave will still work when we have UEIP=1.*/
  setDoubleParam(pC_->motorPosition_, pos);
  setDoubleParam(pC_->motorEncoderPosition_, encpos);

  /*Now do a fast update, to get the new position from the controller.*/
  bool moving = true;
  getAxisStatus(&moving);
 
  if (!stat) {
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
      "%s: Failed to set position on axis %d on controller %s.\n", 
      functionName, axisNo_, pC_->portName);
    asynStatus = asynError;
  } else {
    asynStatus = asynSuccess;
  }

  return asynStatus;
}

/**
 * See asynMotorAxis::stop
 */
asynStatus p6kAxis::stop(double acceleration)
{
  static const char *functionName = "p6kAxis::stop";
  asynStatus status = asynError;
  bool stat = true;
  char command[P6K_MAXBUF]  = {0};
  char response[P6K_MAXBUF] = {0};

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  if (driveType_ == P6K_SERVO_ && !hasEncoder_){
    //TS: DC openloop - revert SOFFS to a known value.
    double servoHoldVoltage_;
    status = pC_->getDoubleParam(axisNo_, pC_->P6K_A_HoldVoltage_, &servoHoldVoltage_);
    if (status != 0) {
      servoHoldVoltage_ = 0.0;
    }
    epicsSnprintf(command, P6K_MAXBUF, "%d%s%lf", axisNo_, P6K_CMD_SOFFS, (double)servoHoldVoltage_);
    stat = pC_->lowLevelWriteRead(command, response);
  }

  epicsSnprintf(command, P6K_MAXBUF, "!%d%s", axisNo_, P6K_CMD_S);
  stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;

  deferredMove_ = 0;

  return status;
}

/**
 * See asynMotorAxis::setEncoderRatio
 */
asynStatus p6kAxis::setEncoderRatio(double ratio)
{ 
  asynStatus status = asynSuccess;
  static const char *functionName = "p6kAxis::setEncoderRatio";

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  status = setDoubleParam(pC_->motorEncoderRatio_, ratio);
  return status;
}

/**
 * See asynMotorAxis::setHighLimit
 */
asynStatus p6kAxis::setHighLimit(double highLimit)
{
  asynStatus status = asynSuccess;
  bool stat = true;
  char command[P6K_MAXBUF]  = {0};
  char response[P6K_MAXBUF] = {0};
  static const char *functionName = "p6kAxis::setHighLimit";

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  if(highLimit != std::numeric_limits<double>::infinity()) {
    epicsInt32 limit = static_cast<epicsInt32>(floor(highLimit + 0.5));
    
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW,
              "%s: Setting high limit on controller %s, axis %d to %d\n",
              functionName, pC_->portName, axisNo_, limit);
    
    epicsSnprintf(command, P6K_MAXBUF, "%d%s%d", axisNo_, P6K_CMD_LSPOS, limit);
    stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
    
    if (!stat) {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR,
                "%s: ERROR: Failed to set high limit on controller %s, axis %d\n",
                functionName, pC_->portName, axisNo_);
      status = asynError;
    }
  }
  
  return status;
}

/**
 * See asynMotorAxis::setLowLimit
 */
asynStatus p6kAxis::setLowLimit(double lowLimit)
{
  asynStatus status = asynSuccess;
  bool stat = true;
  char command[P6K_MAXBUF]  = {0};
  char response[P6K_MAXBUF] = {0};
  static const char *functionName = "p6kAxis::setLowLimit";

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);

  if(lowLimit != -std::numeric_limits<double>::infinity()) {
    epicsInt32 limit = static_cast<epicsInt32>(floor(lowLimit + 0.5));
    
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW,
              "%s: Setting high limit on controller %s, axis %d to %d\n",
              functionName, pC_->portName, axisNo_, limit);
    
    epicsSnprintf(command, P6K_MAXBUF, "%d%s%d", axisNo_, P6K_CMD_LSNEG, limit);
    stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
    
    if (!stat) {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR,
                "%s: ERROR: Failed to set low limit on controller %s, axis %d\n",
                functionName, pC_->portName, axisNo_);
      status = asynError;
    }
  }
  
  return status;
}

/**
 * Function to enable or disable software limits on the controller.
 */
asynStatus p6kAxis::disableSoftwareLimits(bool disable)
{
  asynStatus status = asynSuccess;
  bool stat = true;
  char command[P6K_MAXBUF]  = {0};
  char response[P6K_MAXBUF] = {0};
  static const char *functionName = "p6kAxis::disableSoftwareLimits";

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);
  
  if (disable) {
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW,
        "%s: Disabling software limits on controller %s, axis %d.\n",
        functionName, pC_->portName, axisNo_);
    
    epicsSnprintf(command, P6K_MAXBUF, "%d%s0", axisNo_, P6K_CMD_LS);
    stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
  } else {
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW,
        "%s: Enabling software limits on controller %s, axis %d.\n",
        functionName, pC_->portName, axisNo_);
    
    epicsSnprintf(command, P6K_MAXBUF, "%d%s3", axisNo_, P6K_CMD_LS);
    stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
  }

  if (!stat) {
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR,
        "%s: ERROR: Failed to control LS on controller %s, axis %d\n",
        functionName, pC_->portName, axisNo_);
    status = asynError;
  }

  return status;
}

/**
 * See asynMotorAxis::setClosedLoop
 * This function is used to enable and disable the drive before
 * and after a move (if that's enabled). The function checks if we
 * are currently moving, and does nothing if we are.
 */
asynStatus p6kAxis::setClosedLoop(bool closedLoop)
{
  asynStatus status = asynError;
  char command[P6K_MAXBUF]  = {0};
  char response[P6K_MAXBUF] = {0};
  static const char *functionName = "p6kAxis::setClosedLoop";
 
  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s closedLoop: %d\n", functionName, closedLoop);

  int32_t done = 0;
  pC_->getIntegerParam(axisNo_, pC_->motorStatusDone_, &done);

  if (done == 1) {
    //Check drive status, and dont send anything if we dont need to.
    int32_t power = 0;
    pC_->getIntegerParam(axisNo_, pC_->motorStatusPowerOn_, &power);

    if ((power == 1) && (closedLoop)) {
      return asynSuccess;
    }
    if ((power == 0) && (!closedLoop)) {
      return asynSuccess;
    }

    if (closedLoop) {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, 
        "%s Drive enable on axis %d\n", functionName, axisNo_);
      sprintf(command, "%d%s1",  axisNo_, P6K_CMD_DRIVE);
    } else {
      asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, 
        "%s Drive disable on axis %d\n", functionName, axisNo_);
      sprintf(command, "%d%s0",  axisNo_, P6K_CMD_DRIVE);
    }
    status = pC_->lowLevelWriteRead(command, response);
    
    if (status == asynSuccess) {
      setIntegerParam(pC_->motorStatusPowerOn_, static_cast<int>(closedLoop));
    }

  }
  return status;
}

/**
 * See asynMotorAxis::setPGain
 * These 3 functions are used to set PID parameters for closed loop control
 */

asynStatus p6kAxis::setPGain(double value)
{
  asynStatus status = asynSuccess;
  char command[P6K_MAXBUF]  = {0};
  char response[P6K_MAXBUF] = {0};
  static const char *functionName = "p6kAxis::setPGain";

  epicsSnprintf(command, P6K_MAXBUF, "%d%s%lf", axisNo_, P6K_CMD_SGP, value);
  status = pC_->lowLevelWriteRead(command, response);
  memset(command, 0, sizeof(command));
  return status;
  
}


asynStatus p6kAxis::setIGain(double value)
{
  asynStatus status = asynSuccess;
  char command[P6K_MAXBUF]  = {0};
  char response[P6K_MAXBUF] = {0};
  static const char *functionName = "p6kAxis::setIGain";

  epicsSnprintf(command, P6K_MAXBUF, "%d%s%lf", axisNo_, P6K_CMD_SGI, value);
  status = pC_->lowLevelWriteRead(command, response);
  memset(command, 0, sizeof(command));

  return status;
}


asynStatus p6kAxis::setVGain(double value)
{
  asynStatus status = asynSuccess;
  char command[P6K_MAXBUF]  = {0};
  char response[P6K_MAXBUF] = {0};
  static const char *functionName = "p6kAxis::setVGain";

  epicsSnprintf(command, P6K_MAXBUF, "%d%s%lf", axisNo_, P6K_CMD_SGV, value);
  
  status = pC_->lowLevelWriteRead(command, response);
  memset(command, 0, sizeof(command));

  return status;
}



/**
 * See asynMotorAxis::poll
 */
asynStatus p6kAxis::poll(bool *moving)
{
  asynStatus status = asynSuccess;
  static const char *functionName = "p6kAxis::poll";

  asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s Polling axis: %d\n", functionName, this->axisNo_);

  if (axisNo_ != 0) {

    if (!pC_->lowLevelPortUser_) {
      setIntegerParam(pC_->motorStatusCommsError_, 1);
      return asynError;
    }
    
    //Now poll axis status
    if ((status = getAxisStatus(moving)) != asynSuccess) {
      if (printErrors_) {
        asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR,
		      "%s: Controller %s Axis %d. getAxisStatus failed to return asynSuccess.\n", 
		      functionName, pC_->portName, axisNo_);
      }
      setStringParam(pC_->P6K_A_Error_, "Problem reading axis status");
      //setIntegerParam(pC_->motorStatusCommsError_, 1);
    } else {
      if (!axisError_) {
        setStringParam(pC_->P6K_A_Error_, " ");
      }
      setIntegerParam(pC_->motorStatusCommsError_, 0);
    }
  }
  
  callParamCallbacks();
  return status;
}


/**
 * Read the axis status and set axis related parameters.
 * @param moving Boolean flag to indicate if the axis is moving. This is set by this function
 * to indcate to the polling thread how quickly to poll for status.
 * @return asynStatus
 */
asynStatus p6kAxis::getAxisStatus(bool *moving)
{
    char command[P6K_MAXBUF] = {0};
    char response[P6K_MAXBUF] = {0};
    bool stat = true;
    int32_t nvals = 0;
    int32_t axisNum = 0;
    int32_t intVal = 0;
    epicsFloat64 doubleVal = 0.0;
    int32_t externalEncoderUse = 0;
    int32_t externalEncoder = 0;
    epicsInt32 modbusEncoder = 0;
    char stringVal[P6K_MAXBUF] = {0};
    bool doneMoving = false;
    bool controllerDoneMoving = false;
    uint32_t problem = 0;
    
    static const char *functionName = "p6kAxis::getAxisStatus";
    
    asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, "%s\n", functionName);
    
    axisError_ = false;

    //Get the time and decide if we want to print errors.
    //Crude error message throttling.
    epicsTimeGetCurrent(&nowTime_);
    nowTimeSecs_ = nowTime_.secPastEpoch;
    if ((nowTimeSecs_ - lastTimeSecs_) < pC_->P6K_ERROR_PRINT_TIME_) {
      printErrors_ = false;
    } else {
      printErrors_ = true;
      lastTimeSecs_ = nowTimeSecs_;
    }
    
    if (printNextError_) {
      printErrors_ = true;
    }

    /* Transfer axis status */
    epicsSnprintf(command, P6K_MAXBUF, "%d%s", axisNo_, P6K_CMD_TAS);
    stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
    if (stat) {
      nvals = sscanf(response, "%d" P6K_CMD_TAS "%s", &axisNum, stringVal);
      if (nvals != 2) {
        stat = false;
      } 
    }
    memset(command, 0, sizeof(command));

    #if 0
    //First check if we read the encoder position from a parameter.
    //Then check if we are reading the encoder via modbus.
    //Otherwise read from controller.
    pC_->getIntegerParam(axisNo_, pC_->P6K_A_ExternalEncoderUse_, &externalEncoderUse);
    if (externalEncoderUse == 1) {
      //Allow time for encoder position to be written from data via writeFloat64
      //Otherwise the positions are stale because we are blocked by the poller lock taken
      //in asynMotorController::asynMotorPoller.
      pC_->unlock();
      epicsThreadSleep(0.01);
      pC_->lock();
      if (pC_->getIntegerParam(axisNo_, pC_->P6K_A_ExternalEncoder_, &externalEncoder) == asynSuccess) {
        setDoubleParam(pC_->motorEncoderPosition_, externalEncoder);
        asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, 
                    "%s: External encoder position on controller %s axis %d is %d\n", 
                    functionName, pC_->portName, axisNo_, externalEncoder);
      }
    } else if (modbusEncPort_ != NULL) {
      //We are reading the encoder position over modbus
      //Apply a small delay to ensure we have an up to date value
      epicsThreadSleep(0.1);
      //Check if we care about bad readings
      epicsInt32 modbusEncCheck = 0;
      pC_->getIntegerParam(axisNo_, pC_->P6K_A_ModbusEncoderCheck_, &modbusEncCheck);
      if (pasynInt32SyncIO->read(this->modbusEncPort_, &modbusEncoder, 1.0) != asynSuccess) {
        if (modbusEncCheck != 0) {
          asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
                    "%s: ERROR: Problem reading modbus encoder position axis %d\n", 
                    functionName, axisNo_);
          problem = 1;
        }
      } else {
        if (modbusEncoder == 0) { //If modbus encoder is zero, consider this an error
          if (modbusEncCheck != 0) {
            if (printErrors_) {
              asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
                        "%s: Modbus encoder position is zero on controller %s axis %d.\n", 
                        functionName, pC_->portName, axisNo_);
              printNextError_ = false;
            }
            problem = 1;
          }
          setDoubleParam(pC_->motorEncoderPosition_, 0.0);
        } else {
          asynPrint(pC_->pasynUserSelf, ASYN_TRACE_FLOW, 
                    "%s: Modbus encoder position on controller %s axis %d is %d\n", 
                    functionName, pC_->portName, axisNo_, modbusEncoder);
          //Apply the count offset that we specified in the IOC startup script
          modbusEncoder = modbusEncoder + modbusEncOffset_;
          setDoubleParam(pC_->motorEncoderPosition_, modbusEncoder);
        }
      }
    } else {
      //Else we are just reading the encoder from the controller as normal
      epicsSnprintf(command, P6K_MAXBUF, "%d%s", axisNo_, P6K_CMD_TPE);
      stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
      if (stat) {
        nvals = sscanf(response, "%d"P6K_CMD_TPE"%d", &axisNum, &intVal);
        if (nvals == 2) {
          setDoubleParam(pC_->motorEncoderPosition_, intVal);
        }
      }
    }
#else
    // TZ: Just read encoders regardless the situation
    epicsSnprintf(command, P6K_MAXBUF, "%d%s", axisNo_, P6K_CMD_TPE);
    stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
    if (stat) {
      nvals = sscanf(response, "%dP6K_CMD_TPE%d", &axisNum, &intVal);
      if (nvals == 2) {
        setDoubleParam(pC_->motorEncoderPosition_, intVal);
      }
    }
#endif

    memset(command, 0, sizeof(command));

    /* Transfer current position and encoder position.*/
    epicsSnprintf(command, P6K_MAXBUF, "%d%s", axisNo_, P6K_CMD_TPC);
    stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
    if (stat) {
      nvals = sscanf(response, "%d" P6K_CMD_TPC "%d", &axisNum, &intVal);
      if (nvals == 2) {
        setDoubleParam(pC_->motorPosition_, intVal);
      }
    }
    memset(command, 0, sizeof(command));

    // TZ: Transfer Digital-to-Analog Converter (DAC) Voltage
    epicsSnprintf(command, P6K_MAXBUF, "%d%s", axisNo_, P6K_CMD_TDAC);
    stat = (pC_->lowLevelWriteRead(command, response) == asynSuccess) && stat;
      if (stat) {
        nvals = sscanf(response, "%d" P6K_CMD_TDAC "%lf", &axisNum, &doubleVal);
        if (nvals == 2) {
          setDoubleParam(pC_->P6K_A_DriveVoltage_, doubleVal);
        }
      }
    memset(command, 0, sizeof(command));

    if (!stat) {
      if (printErrors_) {
        asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
            "%s: ERROR: Problem reading position and status on controller %s, axis %d\n", 
            functionName, pC_->portName, axisNo_);
        printNextError_ = false;
      }
    } else {
      if (deferredMove_) {
        doneMoving = false; 
      } else {
        doneMoving = !(stringVal[P6K_TAS_MOVING_] == pC_->P6K_ON_);
      }
      
      if (doneMoving) {
        if (driveType_ == P6K_SERVO_) {
          bool targetZone = (stringVal[P6K_TAS_TARGETZONE_] == pC_->P6K_ON_);
          doneMoving = targetZone && !(stringVal[P6K_TAS_TARGETTIMEOUT_] == pC_->P6K_ON_);
        }
      }

      controllerDoneMoving = doneMoving;

      //Optionally delay the done moving callback at the end of a move
      double delayTime = 0.0;
      pC_->getDoubleParam(axisNo_, pC_->P6K_A_DelayTime_, &delayTime);
      if (delayTime > 0) {
        if (doneMoving) {
          if (movingLastPoll_) {
            delayDoneMove_ = true;
            doneTimeSecs_ = nowTimeSecs_;
          }
          if (delayDoneMove_) {
            if ((nowTimeSecs_ - doneTimeSecs_) > delayTime) {
              delayDoneMove_ = false;
            }
          }
        }
      }
      if (delayDoneMove_) {
        doneMoving = false;
      }
      movingLastPoll_ = !controllerDoneMoving;
      
      if (!doneMoving) {
        *moving = true;
      } else {
	      *moving = false;
      }
      
      //Set MSTA bits
      stat = (setIntegerParam(pC_->motorStatusDone_,
	      doneMoving) == asynSuccess) && stat;
      stat = (setIntegerParam(pC_->motorStatusMoving_, 
	     (stringVal[P6K_TAS_MOVING_] == pC_->P6K_ON_)) == asynSuccess) && stat;

      // TZ: Get direction from TAS
      motorCurDirection_ = (stringVal[P6K_TAS_DIRECTION_] == pC_->P6K_OFF_);
      stat = (setIntegerParam(pC_->motorStatusDirection_, motorCurDirection_)== asynSuccess) && stat;
      stat = (setIntegerParam(pC_->motorStatusHighLimit_, 
	    ((stringVal[P6K_TAS_POSLIM_] == pC_->P6K_ON_) || 
	     (stringVal[P6K_TAS_POSLIMSOFT_] == pC_->P6K_ON_))) == asynSuccess) && stat;
      stat = (setIntegerParam(pC_->motorStatusLowLimit_, 
	    ((stringVal[P6K_TAS_NEGLIM_] == pC_->P6K_ON_) || 
        (stringVal[P6K_TAS_NEGLIMSOFT_] == pC_->P6K_ON_))) == asynSuccess) && stat;
      stat = (setIntegerParam(pC_->motorStatusHomed_, 
	     (stringVal[P6K_TAS_HOMED_] == pC_->P6K_ON_)) == asynSuccess) && stat;
      stat = (setIntegerParam(pC_->motorStatusPowerOn_, 
	     (stringVal[P6K_TAS_DRIVE_] == pC_->P6K_OFF_)) == asynSuccess) && stat;

      //Check TLIM uint32_t from controller object for limit switch status
      //We do this so that the axis object can reflect the limit
      //switch status even if TAS doesn't reflect limit switch status. 
      //To disable this, disable polling TLIM in the controller object.
      //NOTE: I don't check LIMLVL. I assume we are using fail safe inputs
      //so that 1=not activated, and 0=activated.
      int32_t tlim_bits = 0;
      int32_t tlim_size = 0;
      pC_->getIntegerParam(pC_->P6K_C_TLIM_Bits_, &tlim_bits);
      stat = (setIntegerParam(pC_->motorStatusAtHome_, 0) == asynSuccess) && stat;
      stat = (setIntegerParam(pC_->motorStatusHome_, 0) == asynSuccess) && stat;
      if (tlim_bits > 0) {
        tlim_size = (axisNo_ - 1)*pC_->P6K_TLIM_SIZE_;
        if ((tlim_bits & (0x1 << (tlim_size + pC_->P6K_TLIM_BIT1_))) == 0) {
          stat = (setIntegerParam(pC_->motorStatusHighLimit_, 1) == asynSuccess) && stat;
        }
        if ((tlim_bits & (0x1 << (tlim_size + pC_->P6K_TLIM_BIT2_))) == 0) {
          stat = (setIntegerParam(pC_->motorStatusLowLimit_, 1) == asynSuccess) && stat;
        }
        if (tlim_bits & (0x1 << (tlim_size + pC_->P6K_TLIM_BIT3_))) {
          stat = (setIntegerParam(pC_->motorStatusAtHome_, 1) == asynSuccess) && stat;
          stat = (setIntegerParam(pC_->motorStatusHome_, 1) == asynSuccess) && stat;
        }
      }
      
      //Set limit error message for users
      if (stringVal[P6K_TAS_POSLIM_] == pC_->P6K_ON_) {
        axisError_ = true;
        setStringParam(pC_->P6K_A_Error_, "ERROR: Hardware High Limit");
      }
      if (stringVal[P6K_TAS_NEGLIM_] == pC_->P6K_ON_) {
        axisError_ = true;
        setStringParam(pC_->P6K_A_Error_, "ERROR: Hardware Low Limit");
      }
      if (stringVal[P6K_TAS_POSLIMSOFT_] == pC_->P6K_ON_) {
        axisError_ = true;
        setStringParam(pC_->P6K_A_Error_, "ERROR: Software High Limit");
      }
      if (stringVal[P6K_TAS_NEGLIMSOFT_] == pC_->P6K_ON_) {
        axisError_ = true;
        setStringParam(pC_->P6K_A_Error_, "ERROR: Software Low Limit");
      }

      if (driveType_ == P6K_SERVO_) {
        stat = (setIntegerParam(pC_->motorStatusFollowingError_, 
          (stringVal[P6K_TAS_POSERROR_] == pC_->P6K_ON_)) == asynSuccess) && stat;
      } else {
        stat = (setIntegerParam(pC_->motorStatusFollowingError_, 
          (stringVal[P6K_TAS_STALL_] == pC_->P6K_ON_)) == asynSuccess) && stat;
      }
      
      if (stringVal[P6K_TAS_STALL_] == pC_->P6K_ON_) {
        axisError_ = true;
        setStringParam(pC_->P6K_A_Error_, "ERROR: Stall Detected");
      }
      
      if (commandError_) {
        problem = 1;
      }

      //We only detect drive fault input when a move is attempted.
      //Unless we also poll extended axis status (TASX), which always 
      //reports drive fault status.
      if (stringVal[P6K_TAS_DRIVEFAULT_] == pC_->P6K_ON_) {
        stat = (setIntegerParam(pC_->P6K_A_TAS_DriveFault_, 1) == asynSuccess) && stat;
        problem = 1;
        if (printErrors_) {
          asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
              "%s: ERROR: Drive fault on controller %s, axis %d\n", 
              functionName, pC_->portName, axisNo_);
          printNextError_ = false;
        }
      } else {
        stat = (setIntegerParam(pC_->P6K_A_TAS_DriveFault_, 0) == asynSuccess) && stat;
      }

      if (stringVal[P6K_TAS_TARGETTIMEOUT_] == pC_->P6K_ON_) {
        stat = (setIntegerParam(pC_->P6K_A_TAS_Timeout_, 1) == asynSuccess) && stat;
        problem = 1;
        if (printErrors_) {
          asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
                "%s: ERROR: Target timeout on controller %s, axis %d\n", 
                functionName, pC_->portName, axisNo_);
          printNextError_ = false;
        }
      } else {
        stat = (setIntegerParam(pC_->P6K_A_TAS_Timeout_, 0) == asynSuccess) && stat;
      }

      if (stringVal[P6K_TAS_POSERROR_] == pC_->P6K_ON_) {
        stat = (setIntegerParam(pC_->P6K_A_TAS_PosErr_, 1) == asynSuccess) && stat;
        problem = 1;
        if (printErrors_) {
          asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
            "%s: ERROR: Position error on controller %s, axis %d\n", 
            functionName, pC_->portName, axisNo_);
          printNextError_ = false;
        }
      } else {
        stat = (setIntegerParam(pC_->P6K_A_TAS_PosErr_, 0) == asynSuccess) && stat;
      }

      stat = (setIntegerParam(pC_->motorStatusProblem_, (problem!=0)) == asynSuccess) && stat;
      if (problem == 1) {
        axisError_ = true;
        setStringParam(pC_->P6K_A_Error_, "ERROR: Problem detected or move failed.");
      }

      if (!stat) {
        if (printErrors_) {
          asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
            "%s: ERROR: Problem setting params on controller %s, axis %d\n", 
            functionName, pC_->portName, axisNo_);
          printNextError_ = false;
        }
      }
    }
    
    //Clear error print flag for this axis if problem has been removed.
    if (stat) {
      if (!problem && !printErrors_) {
        asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, 
            "%s Problem cleared on controller %s, axis %d\n", 
            functionName, pC_->portName, axisNo_);
        printNextError_ = true;
      }
      //Always return asynSuccess unless we failed to read from the controller
      //or failed to read or write driver params.
      return asynSuccess;
    }
    
    return asynError;
}
