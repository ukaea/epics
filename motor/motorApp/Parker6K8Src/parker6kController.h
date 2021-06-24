/********************************************
 *  parker6kController.h 
 * 
 *  Parker 6k Asyn motor based on the 
 *  asynMotorController class.
 * 
 *  Matt Pearson
 *  26 March 2014
 * 
 ********************************************/

#ifndef parker6kController_H
#define parker6kController_H

#include "asynMotorController.h"
#include "asynMotorAxis.h"
#include "parker6kAxis.h"

#define P6K_C_FirstParamString "P6K_C_FIRSTPARAM"
#define P6K_C_LastParamString  "P6K_C_LASTPARAM"

#define P6K_C_GlobalStatusString    "P6K_C_GLOBALSTATUS"
#define P6K_C_CommsErrorString      "P6K_C_COMMSERROR"
#define P6K_C_CommandString         "P6K_C_COMMAND"
#define P6K_C_ResponseString        "P6K_C_RESPONSE"
#define P6K_C_ErrorString           "P6K_C_ERROR"
#define P6K_C_ConfigString          "P6K_C_CONFIG"
#define P6K_C_LogString             "P6K_C_LOG"
#define P6K_C_TSS_SystemReadyString "P6K_C_TSS_SYSTEMREADY"
#define P6K_C_TSS_ProgRunningString "P6K_C_TSS_PROGRUNNING"
#define P6K_C_TSS_ImmediateString   "P6K_C_TSS_IMMEDIATE"
#define P6K_C_TSS_CmdErrorString    "P6K_C_TSS_CMDERROR"
#define P6K_C_TSS_MemErrorString    "P6K_C_TSS_MEMERROR"
#define P6K_C_TLIM_EnableString     "P6K_C_TLIM_ENABLE"
#define P6K_C_TLIM_BitsString       "P6K_C_TLIM_BITS"
#define P6K_C_INOUT_EnableString    "P6K_C_INOUT_ENABLE"
#define P6K_C_TOUT_BitsString       "P6K_C_TOUT_BITS"
#define P6K_C_TIN_BitsString        "P6K_C_TIN_BITS"
#define P6K_C_OUT_BitString         "P6K_C_OUT_BIT"
#define P6K_C_OUT_ValString         "P6K_C_OUT_VAL"
#define P6K_C_OUT_AllString         "P6K_C_OUT_ALL"

//Axis specific parameters
#define P6K_A_DRESString       "P6K_A_DRES"
#define P6K_A_ERESString       "P6K_A_ERES"
#define P6K_A_DRIVEString      "P6K_A_DRIVE"
#define P6K_A_AXSDEFString     "P6K_A_AXSDEF"
#define P6K_A_MaxDigitsString  "P6K_A_MAXDIGITS"
#define P6K_A_LimitDriveEnableString  "P6K_A_LIMIT_DRIVE_ENABLE"
#define P6K_A_SendPositionOnlyString  "P6K_A_SEND_POSITION_ONLY"
#define P6K_A_LSEnableString  "P6K_A_LS_ENABLE"
#define P6K_A_CommandString    "P6K_A_COMMAND"
#define P6K_A_ResponseString   "P6K_A_RESPONSE"
#define P6K_A_LSString         "P6K_A_LS"
#define P6K_A_LHString         "P6K_A_LH"
#define P6K_A_ErrorString      "P6K_A_ERROR"
#define P6K_A_MoveErrorString  "P6K_A_MOVEERROR"
#define P6K_A_DelayTimeString  "P6K_A_DELAYTIME"
#define P6K_A_TAS_DriveFaultString  "P6K_A_TAS_DRIVEFAULT"
#define P6K_A_TAS_TimeoutString  "P6K_A_TAS_TIMEOUT"
#define P6K_A_TAS_PosErrString  "P6K_A_TAS_POSERR"
#define P6K_A_AutoDriveEnableString  "P6K_A_AUTO_DRIVE_ENABLE"
#define P6K_A_AutoDriveEnableDelayString  "P6K_A_AUTO_DRIVE_ENABLE_DELAY"
#define P6K_A_DriveRetryString  "P6K_A_DRIVE_RETRY"
#define P6K_A_ExternalEncoderUseString  "P6K_A_EXT_ENC_USE"
#define P6K_A_ExternalEncoderString  "P6K_A_EXT_ENC"
#define P6K_A_ModbusEncoderString  "P6K_A_MODBUS_ENC"
#define P6K_A_ModbusEncoderAddrString  "P6K_A_MODBUS_ENC_ADDR"
#define P6K_A_ModbusEncoderOffsetString  "P6K_A_MODBUS_ENC_OFFSET"
#define P6K_A_ModbusEncoderCheckString  "P6K_A_MODBUS_ENC_CHECK"
#define P6K_A_DriveVoltageString "P6K_A_DRIVE_VOLTS"
#define P6K_A_VoltageOffsetString "P6K_A_VOLT_SOFFS"
#define P6K_A_HoldVoltageString "P6K_A_HOLD_SOFFS"
#define P6K_A_TargetDirectionString "P6K_A_TARGET_DIRECTION"

#define P6K_MAXBUF 1024

//Controller commands
#define P6K_CMD_A        "A"
#define P6K_CMD_AA       "AA"
#define P6K_CMD_AD       "AD"
#define P6K_CMD_ADA      "ADA"
#define P6K_CMD_AXSDEF   "AXSDEF"
#define P6K_CMD_CMDDIR   "CMDDIR"
#define P6K_CMD_COMEXC   "COMEXC"
#define P6K_CMD_D        "D"
#define P6K_CMD_DRES     "DRES"
#define P6K_CMD_DRFEN    "DRFEN"
#define P6K_CMD_DRIVE    "DRIVE"
#define P6K_CMD_ECHO     "ECHO"
#define P6K_CMD_ENCCNT   "ENCCNT"
#define P6K_CMD_ENCPOL   "ENCPOL"
#define P6K_CMD_ERES     "ERES"
#define P6K_CMD_ESK      "ESK"
#define P6K_CMD_ESTALL   "ESTALL"
#define P6K_CMD_GO       "GO"
#define P6K_CMD_HOM      "HOM"
#define P6K_CMD_HOMA     "HOMA"
#define P6K_CMD_HOMAA    "HOMAA"
#define P6K_CMD_HOMAD    "HOMAD"
#define P6K_CMD_HOMADA   "HOMADA"
#define P6K_CMD_HOMV     "HOMV"
#define P6K_CMD_KDRIVE   "KDRIVE"
#define P6K_CMD_K        "K"
#define P6K_CMD_LH       "LH"
#define P6K_CMD_LS       "LS"
#define P6K_CMD_LSNEG    "LSNEG"
#define P6K_CMD_LSPOS    "LSPOS"
#define P6K_CMD_MA       "MA"
#define P6K_CMD_OUT      "OUT"
#define P6K_CMD_PESET    "PESET"
#define P6K_CMD_PSET     "PSET"
#define P6K_CMD_S        "S"
#define P6K_CMD_SGP      "SGP"
#define P6K_CMD_SGI      "SGI"
#define P6K_CMD_SGV      "SGV"
#define P6K_CMD_SFB      "SFB"
#define P6K_CMD_SOFFS    "SOFFS"
#define P6K_CMD_TAS      "TAS"
#define P6K_CMD_TDAC     "TDAC"
#define P6K_CMD_TIN      "TIN"
#define P6K_CMD_TLIM     "TLIM"
#define P6K_CMD_TOUT     "TOUT"
#define P6K_CMD_TPC      "TPC"
#define P6K_CMD_TPE      "TPE"
#define P6K_CMD_TREV     "TREV"
#define P6K_CMD_TSS      "TSS"
#define P6K_CMD_V        "V"

/**
 * p6kController derives from the virtual class asynMotorController.
 * 
 */
class p6kController : public asynMotorController {

 public:
  p6kController(const char *portName, const char *lowLevelPortName, int lowLevelPortAddress, int numAxes, double movingPollPeriod, 
		double idlePollPeriod);

  virtual ~p6kController();

  asynStatus printConnectedStatus(void);

  /* These are the methods that we override */
  asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
  asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
  asynStatus setDeferredMoves(bool deferMoves);
  virtual asynStatus writeOctet(asynUser *pasynUser, const char *value, 
                                    size_t nChars, size_t *nActual);
  void report(FILE *fp, int level);
  p6kAxis* getAxis(asynUser *pasynUser);
  p6kAxis* getAxis(int axisNo);
  asynStatus poll();

  asynStatus upload(const char *filename); 

 protected:
  p6kAxis **pAxes_;       /**< Array of pointers to axis objects */

  #define FIRST_P6K_PARAM P6K_C_FirstParam_
  int P6K_C_FirstParam_;
  int P6K_C_GlobalStatus_;
  int P6K_C_CommsError_;
  int P6K_A_DRES_;
  int P6K_A_ERES_;
  int P6K_A_DRIVE_;
  int P6K_A_AXSDEF_;
  int P6K_A_MaxDigits_;
  int P6K_A_LimitDriveEnable_;
  int P6K_A_SendPositionOnly_;
  int P6K_A_LS_Enable_;
  int P6K_A_LS_;
  int P6K_A_LH_;
  int P6K_C_Command_;
  int P6K_A_Command_;
  int P6K_C_Response_;
  int P6K_A_Response_;
  int P6K_A_Error_;
  int P6K_A_MoveError_;
  int P6K_C_Error_;
  int P6K_C_Config_;
  int P6K_C_Log_;
  int P6K_A_DelayTime_;
  int P6K_A_TAS_DriveFault_;
  int P6K_A_TAS_Timeout_;
  int P6K_A_TAS_PosErr_;
  int P6K_A_AutoDriveEnable_;
  int P6K_A_AutoDriveEnableDelay_;
  int P6K_A_DriveRetry_;
  int P6K_A_ExternalEncoderUse_;
  int P6K_A_ExternalEncoder_;
  int P6K_A_ModbusEncoder_;
  int P6K_A_ModbusEncoderAddr_;
  int P6K_A_ModbusEncoderOffset_;
  int P6K_A_ModbusEncoderCheck_;
  int P6K_A_DriveVoltage_;
  int P6K_A_VoltageOffset_;
  int P6K_A_HoldVoltage_;
  int P6K_A_TargetDirection_;
  int P6K_C_TSS_SystemReady_;
  int P6K_C_TSS_ProgRunning_;
  int P6K_C_TSS_Immediate_;
  int P6K_C_TSS_CmdError_;
  int P6K_C_TSS_MemError_;
  int P6K_C_TLIM_Enable_;
  int P6K_C_TLIM_Bits_;
  int P6K_C_INOUT_Enable_;
  int P6K_C_TOUT_Bits_;
  int P6K_C_TIN_Bits_;
  int P6K_C_OUT_Bit_;
  int P6K_C_OUT_Val_;
  int P6K_C_OUT_All_;
  int P6K_C_LastParam_;
  #define LAST_P6K_PARAM P6K_C_LastParam_

 private:
  p6kAxis *pAxisZero;
  asynUser* lowLevelPortUser_;
  epicsUInt32 movesDeferred_;
  epicsTimeStamp nowTime_;
  epicsFloat64 nowTimeSecs_;
  epicsFloat64 lastTimeSecs_;
  bool printNextError_;
  bool printErrors_;
  double movingPollPeriod_;
  double idlePollPeriod_;
  asynStatus lowLevelWriteRead(const char *command, char *response);
  asynStatus trimResponse(char *input, char *output);
  asynStatus errorResponse(char *input, char *output);
  asynStatus lowLevelPortConnect(const char *port, int addr, asynUser **ppasynUser, const char *inputEos, const char *outputEos);
  asynStatus startPoller(void);
  asynStatus setDigitalOutput(epicsInt32 bit, epicsInt32 enable);
  asynStatus setDigitalOutputs(epicsInt32 enable);
  asynStatus getDigital(const char *command, size_t size, uint32_t *bits);

  //static class data members

  static const epicsUInt32 P6K_MAXBUF_;
  static const epicsUInt32 P6K_MAXAXES_;
  static const epicsFloat64 P6K_TIMEOUT_;
  static const epicsUInt32 P6K_FORCED_FAST_POLLS_;
  static const epicsUInt32 P6K_OK_;
  static const epicsUInt32 P6K_ERROR_;
  static const epicsUInt32 P6K_ERROR_PRINT_TIME_;
  static const epicsUInt32 P6K_MAX_DIGITS_;

  static const char * P6K_ASYN_IEOS_;
  static const char * P6K_ASYN_IEOS_PROG_;
  static const char * P6K_ASYN_OEOS_;

  static const char P6K_ON_;
  static const char P6K_OFF_;
  static const char P6K_NOCHANGE_;
  static const char P6K_UNDERSCORE_;

  static const epicsUInt32 P6K_TSS_SYSTEMREADY_;
  static const epicsUInt32 P6K_TSS_PROGRUNNING_;
  static const epicsUInt32 P6K_TSS_IMMEDIATE_;
  static const epicsUInt32 P6K_TSS_CMDERROR_;
  static const epicsUInt32 P6K_TSS_MEMERROR_;

  static const epicsUInt32 P6K_TLIM_BIT1_;
  static const epicsUInt32 P6K_TLIM_BIT2_;
  static const epicsUInt32 P6K_TLIM_BIT3_;
  static const epicsUInt32 P6K_TLIM_SIZE_;

  static const epicsUInt32 P6K_UINT32_SIZE_;

  friend class p6kAxis;

};

#define NUM_P6K_PARAMS (&LAST_P6K_PARAM - &FIRST_P6K_PARAM + 1)

#endif /* parker6kController_H */
