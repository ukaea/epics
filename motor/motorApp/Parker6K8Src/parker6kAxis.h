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


#ifndef p6kAxis_H
#define p6kAxis_H

#include "stdint.h"

#include "asynMotorController.h"
#include "asynMotorAxis.h"

class p6kController;

/**
 * p6kAxis derives from the virtual class asynMotorAxis. It re-implements some functions
 * and defines all the axis specific logic, including the polling function that
 * populates the axis parameters.
 */
class p6kAxis : public asynMotorAxis
{
  public:
  //These are the methods we override from the base class
  p6kAxis(p6kController *pController, int32_t axisNo);
  virtual ~p6kAxis();
  asynStatus move(double position, int32_t relative, double min_velocity, double max_velocity, double acceleration);
  asynStatus moveVelocity(double min_velocity, double max_velocity, double acceleration);
  asynStatus home(double min_velocity, double max_velocity, double acceleration, int32_t forwards);
  asynStatus stop(double acceleration);
  asynStatus poll(bool *moving);
  asynStatus setPosition(double position);
  asynStatus setClosedLoop(bool closedLoop);
  asynStatus setEncoderRatio(double ratio);
  asynStatus setHighLimit(double highLimit);
  asynStatus setLowLimit(double lowLimit);
  asynStatus disableSoftwareLimits(bool disable);
  asynStatus modbusPortConnect(const char *modbusPort, int modbusAddr, int modbusOffset);
  
  asynStatus setPGain(double gain);
  asynStatus setIGain(double gain);
  asynStatus setVGain(double gain);

  asynStatus setPID(const double * value, int pidoption);
  asynStatus getPID();
  asynStatus setPIDValue(const double * value, int pidoption);

  private:
  p6kController *pC_;
  

  asynUser* modbusEncPort_;
  epicsInt32 modbusEncAddr_;
  epicsInt32 modbusEncOffset_;

  bool movingLastPoll_;
  bool delayDoneMove_;
  epicsFloat64 doneTimeSecs_;
  

  asynStatus getAxisStatus(bool *moving);
  asynStatus getAxisInitialStatus(void);
  asynStatus readIntParam(const char *cmd, epicsUInt32 param, uint32_t *val);
  asynStatus readDoubleParam(const char *cmd, epicsUInt32 param, double *val);
  void printAxisParams(void);
  asynStatus autoDriveEnable(void);
  int32_t getScaleFactor(void);

  uint32_t deferredPosition_;
  uint32_t deferredMove_;
  uint32_t deferredRelative_;
  epicsTimeStamp nowTime_;
  epicsFloat64 nowTimeSecs_;
  epicsFloat64 lastTimeSecs_; 
  bool printNextError_;
  bool printErrors_;
  uint32_t driveType_;
  bool hasEncoder_;
  bool motorCurDirection_;
  epicsFloat64 driveVoltage_;
  epicsFloat64 voltageOffset_;
  epicsFloat64 servoHoldVoltage_; //TS: stationary +/- voltage for DC servos.
  int32_t targetDirection_; //TS: direction for tweak tracking with SOFFS.

  bool commandError_;
  bool axisError_;

  uint32_t p6k_cmddir_;
  uint32_t p6k_drfen_;
  uint32_t p6k_encpol_;
  uint32_t p6k_esk_;
  uint32_t p6k_estall_;

  static const epicsUInt32 P6K_TAS_MOVING_;
  static const epicsUInt32 P6K_TAS_DIRECTION_;
  static const epicsUInt32 P6K_TAS_ACCELERATING_;
  static const epicsUInt32 P6K_TAS_ATVELOCITY_;
  static const epicsUInt32 P6K_TAS_HOMED_;
  static const epicsUInt32 P6K_TAS_ABSOLUTE_;
  static const epicsUInt32 P6K_TAS_CONTINUOUS_;
  static const epicsUInt32 P6K_TAS_JOG_;
  static const epicsUInt32 P6K_TAS_JOYSTICK_;
  static const epicsUInt32 P6K_TAS_STALL_;
  static const epicsUInt32 P6K_TAS_DRIVE_;
  static const epicsUInt32 P6K_TAS_DRIVEFAULT_;
  static const epicsUInt32 P6K_TAS_POSLIM_;
  static const epicsUInt32 P6K_TAS_NEGLIM_;
  static const epicsUInt32 P6K_TAS_POSLIMSOFT_;
  static const epicsUInt32 P6K_TAS_NEGLIMSOFT_;
  static const epicsUInt32 P6K_TAS_POSERROR_;
  static const epicsUInt32 P6K_TAS_TARGETZONE_;
  static const epicsUInt32 P6K_TAS_TARGETTIMEOUT_;
  static const epicsUInt32 P6K_TAS_GOWHENPEND_;
  static const epicsUInt32 P6K_TAS_MOVEPEND_;
  static const epicsUInt32 P6K_TAS_PREEMPT_;
   
  static const epicsUInt32 P6K_STEPPER_;
  static const epicsUInt32 P6K_SERVO_;

  static const epicsUInt32 P6K_LIM_ENABLE_;
  static const epicsUInt32 P6K_LIM_DISABLE_;

  static const char * P6K_DRIVE_SHUTDOWN_STR_;

  friend class p6kController;
};


#endif /* p6kAxis_H */
