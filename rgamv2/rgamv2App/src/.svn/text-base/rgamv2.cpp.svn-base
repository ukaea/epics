#include <asynPortDriver.h>
#include "asynOctetSyncIO.h"
#include <epicsExport.h>
#include <drvAsynIPPort.h>
#include <epicsThread.h>
#include <epicsTimer.h>
#include <epicsMath.h>
#include <epicsString.h>
#include <iocsh.h>
#include <string.h>

#include <algorithm>
#include <climits>
#include <string>
#include <vector>
#include <climits>

#define PEAK_JUMP_SUPPORT 0

#define NUM_PARAMS 232
#define NUM_MASSES 200
#define NUM_PJMASSES 12

#define SMALL_BUFFER_SIZE 100
#define LARGE_BUFFER_SIZE 1600


namespace rgamv2
{

std::string escapedFromRaw(const char * buffer, size_t length);
std::string escapedFromRaw(const std::string & str);

template <typename T>
inline T paToMbar(T p)
{
    return 0.01*p;
}

template <typename T>
inline T torrToMbar(T p)
{
    return 1.33322*p;
}


class MKS937
{
public:
    // convert 937 analog output voltage to pressure
    // see TDI-VAC-VEQ-VEM-003 937A operation manual
    // Part #100009272 Rev 4.0 (August 1998)
    // Appendix C pg C.6.
    static double voltageToMbar(double volts)
    {
        // convert analog input to torr
        const double gradient = 15.0/9.0;
        const double intercept = -12.0;
        double torr = pow(10.0, (gradient * volts + intercept));

        // convert to mbar
        return torrToMbar(torr);
    }
};

template <typename T>
class Control
{
public:
    Control(const T & t)
    : control_(t),
      target_(t),
      status_(t),
      force_(false),
      forceT_(false)
    {
    }
    const T & control()    { return control_; }
    const T & target()     { return target_; }
    const T & status()     { return status_; }

    bool hasChanged()      { return (status_ != target_) || forceT_; }
    void changeComplete()
    {
       status_ = target_;
       target_ = control_;
       forceT_ = force_;
       force_  = false;
    }

    void newSetting(const T & t)        { newSetting(t, false); }
    void forceNewSetting(const T & t)   { newSetting(t, true);  }
    void setValue(const T & t)          { status_ = target_ = control_ = t; forceT_ = force_ = false; }

private:
    void newSetting(const T & t, bool force)
    {
        control_ = t;
        force_ = force;
        if (!hasChanged())
        {
            target_ = control_;
            forceT_ = force_;
        }
    }

    T control_;
    T target_;
    T status_;
    bool force_;
    bool forceT_;
};

class MV2 : public asynPortDriver
{
    int P_BARM[NUM_MASSES];
    int P_BAR;
    int P_SUMP;
    int P_TOTP;
    int P_ANA;
    int P_DATA;
    int P_CON;
    int P_HEADSTA;
    int P_SETSCAN;
    int P_SCAN;
    int P_FIL;
    int P_SETFIL;
    int P_FILCON;
    int P_FILSTA;
    int P_MLTCON;
    int P_MLTSTA;
    int P_DET;
    int P_ILK;
    int P_MODE;
    int P_SETMODE;

public:
    MV2(char * name, char * port);
    ~MV2();

    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 * value);
    virtual asynStatus readFloat64(asynUser *pasynUser, epicsFloat64 *value);
    virtual asynStatus readFloat64Array(asynUser *pasynUser, epicsFloat64 *value,
                                        size_t nElements, size_t *nIn);

    void mainRun();
    void handleTimer();

private:
    asynStatus sendCommand(const std::string & data, double timeout);
    void processReceived();


    enum ScanDataType
    {
        NO_DATA,
        BAR50,
        BAR100,
        BAR200,
        ANALOG200,
        PEAKJUMP
    };

    enum HeadState
    {
        STOP,
        LOCAL_CONTROL,
        DEGAS_FILAMENT,
        CAL_FARADAY,
        CAL_MULTIPLIER,
        BARCHART_50,
        BARCHART_100,
        BARCHART_200,
        ANALOG_200,
#if PEAK_JUMP_SUPPORT
        PEAK_JUMP = 10
#endif
    };

    enum ScanMode
    {
        SINGLE,
        CONTINUOUS
    };

    enum FilamentControl
    {
        FC_OFF,
        FC_ON
    };

    enum FilamentState
    {
        FS_OFF,
        FS_ON,
        FS_WARM_UP,
        FS_COOL_DOWN,
        FS_FAILED
    };

    enum TripState
    {
        TS_OK,
        TS_FAIL
    };

    enum SmState
    {
        CONNECTING,
        RELEASE_SENSOR,
        GET_SENSORS,
        SELECT_SENSOR,
        INITIALIZING,
        SELECTED,
        IN_CONTROL,
    };

    enum Task
    {
        NO_TASK,
        INITIAL_RELEASE,
        SENSORS,
        SELECT,
        INIT,
        CONTROL,
        RELEASE,
        CHANGE_STATE,
        RESUME_SCAN,
        FILAMENT_CONTROL,
        SET_FILAMENT,
        GET_INFO
    };

    enum TaskState
    {
        TS_IDLE,
        TS_BUSY,
        TS_COMPLETE
    };

    enum TaskErrorStatus
    {
        TE_OK,
        TE_ASYN_ERROR,
        TE_CMD_TIMEOUT,
        TE_PROTOCOL_ERROR,
    };

    enum CommandState
    {
        CMD_IDLE,
        CMD_BUSY,
    };

    struct SensorInfo
    {
        std::string state;
        std::string serialNumber;
        std::string name;
    };

    void stateMachine();
    void changeState(SmState state);
    asynStatus connect();

    void processTask();
    void startTask(Task task);
    void taskToIdle();

    void commandToIdle();

    void sensors();
    void select();
    void init();
    void controlSensor();
    void release();
    void changeHeadState();
    void resumeScan();
    void setFilamentControl(FilamentControl con);
    void selectFilament(int fil);
    void scanComplete();
    void getInfo();
    void error();

    void handleMassReading(float mass, double reading);

    void processFilamentStatus(const std::string & notification);
    void processFilamentInfo(const std::string & notification);

    double totalPressure();
    std::string getParameterName(int reason);

    std::string makeAddBarChartCommand(const std::string & name);
    std::string makeAddAnalogCommand(const std::string & name);
    std::string makePeakJumpCommand(const std::string & name);
    unsigned lastMass();
    unsigned lastMass(HeadState sta);
    size_t lastIndex(HeadState sta);
    size_t massToIndex(float mass);

    bool isBarchart(HeadState sta);
    bool isScanMode(HeadState sta);

    const static int numFilaments_ = 2;
    const static int numDetectorIndexes_ = 4;
    const static unsigned numAnalogInputs_ = 5;

    const static unsigned accuracy_ = 5;
    const static unsigned pointsPerPeak_ = 32;
    const static unsigned egain_ = 0;
    const static unsigned source_ = 0;

    std::string port_;
    asynUser* serialPortUser;

    SmState state_;
    bool commsFail_;
    bool scheduleGetInfo_;
    unsigned scheduleGetInfoCounter_;

    std::vector<SensorInfo> sensors_;

    Task task_;
    TaskState taskState_;
    TaskErrorStatus taskErrorStatus_;
    std::vector<std::string> cmds;
    unsigned cmdNumber_;

    std::string cmd_;
    std::string cmdStatus_;
    CommandState cmdState_;
    unsigned busyCounter_;

    ScanMode scanMode_;
    bool scanning_;
#if PEAK_JUMP_SUPPORT
    std::vector<unsigned> peakJumpMasses_;
#endif


    Control<HeadState> headState_;
    Control<unsigned> detector_;
    Control<unsigned> fil_;
    Control<FilamentControl> filCon_;

    class FilamentStatus
    {
    public:
        FilamentStatus()
        : filamentNum_(0), filSta_(FS_OFF), trip_("None")
        {}
        FilamentStatus(unsigned filamentNum, FilamentState filSta, const std::string & trip)
        : filamentNum_(filamentNum), filSta_(filSta), trip_(trip)
        {}
        unsigned        filamentNum_;
        FilamentState   filSta_;
        std::string     trip_; 
    };

    FilamentStatus filamentStatus_;

    std::vector<double> scanData_;

    std::vector<double> barResultsData_;
    std::vector<double> analogResultsData_;
    ScanDataType currentData_;
    unsigned currentDetector_;
    double sumP_;

    class AnalogInput
    {
    public: 
        void init(unsigned numInputs);
        bool hasValue(unsigned input);
        double getValue(unsigned input);
        bool setValue(unsigned input, double value);

    private:
        std::vector<double> values_;
        std::vector<bool> inputsValid_;
    };

    AnalogInput analogInput_;


    class MainThread: public epicsThreadRunable
    {
    private:
        MV2* owner;
        epicsThread thread;

    public:
        MainThread(MV2* owner);
        virtual ~MainThread() {}
        virtual void run();
    };

    MainThread * mainThread_;

    class EventTimer : public epicsTimerNotify
    {
    public:
        EventTimer(MV2 * owner, epicsTimerQueueActive &queue)
        : owner_(owner), timer_(queue.createTimer()), delay_(0.0) {}
        virtual expireStatus expire(const epicsTime & currentTime)
        {
            owner_->handleTimer();
            return expireStatus(restart, delay_);
        }
        void start(double delay)
        {
            delay_ = delay;
            timer_.start(*this, delay);
        }
        ~EventTimer() { timer_.destroy(); }

    private:
        MV2 * owner_;
        epicsTimer &timer_;
        double delay_;
    };

    epicsTimerQueueActive * timerQueue_;
    EventTimer * eventTimer_;
};


MV2::MV2(char * name, char *address)
 :  asynPortDriver(name,
        1,              /* maxAddr */
        NUM_PARAMS,     /* max parameters */
        asynDrvUserMask |
        asynFloat64Mask | 
        asynFloat64ArrayMask | 
        asynInt32Mask,  /* interface mask*/
        0,              /* interrupt mask */
        0,              /* non-blocking, no address */
        1,              /* autoconnect */
        0,              /* default priority */
        0               /* default stack size */
    ),
    port_(address),
    serialPortUser(NULL),
    state_(CONNECTING),
    commsFail_(false),
    scheduleGetInfo_(false),
    scheduleGetInfoCounter_(0),
    task_(NO_TASK),
    taskState_(TS_IDLE),
    taskErrorStatus_(TE_OK),
    cmdNumber_(0),
    cmdState_(CMD_IDLE),
    busyCounter_(0),
    scanMode_(CONTINUOUS),
    scanning_(false),
    headState_(STOP),
    detector_(0),
    fil_(0),
    filCon_(FC_OFF),
    currentData_(NO_DATA),
    currentDetector_(0),
    sumP_(0.0)
{
    // Uncomment this line to enable asyn trace flow and error
    //pasynTrace->setTraceMask(pasynUserSelf, 0xFF);

    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "(%s) Constructor\n", portName);

    for (int i = 0; i < NUM_MASSES; ++i)
    {
        char paramName[SMALL_BUFFER_SIZE];
        snprintf(paramName, SMALL_BUFFER_SIZE, "BARM%d", i+1);
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) Create param %s\n", portName, paramName); 
        createParam(paramName, asynParamFloat64, &P_BARM[i]);
    }

    createParam("SUMP", asynParamFloat64, &P_SUMP);
    createParam("TOTP", asynParamInt32, &P_TOTP);
    createParam("BAR", asynParamFloat64Array, &P_BAR);
    createParam("ANA",  asynParamFloat64Array, &P_ANA);
    createParam("DATA", asynParamInt32, &P_DATA);
    createParam("CON", asynParamInt32, &P_CON);
    createParam("HEADSTA", asynParamInt32, &P_HEADSTA);
    createParam("SETSCAN", asynParamInt32, &P_SETSCAN);
    createParam("SCAN", asynParamInt32, &P_SCAN);
    createParam("FIL", asynParamInt32, &P_FIL);
    createParam("SETFIL", asynParamInt32, &P_SETFIL);
    createParam("FILCON", asynParamInt32, &P_FILCON);
    createParam("FILSTA", asynParamInt32, &P_FILSTA);
    createParam("MLTCON", asynParamInt32, &P_MLTCON);
    createParam("MLTSTA", asynParamInt32, &P_MLTSTA);
    createParam("DET", asynParamInt32, &P_DET);
    createParam("ILK", asynParamInt32, &P_ILK);
    createParam("MODE", asynParamInt32, &P_MODE);
    createParam("SETMODE", asynParamInt32, &P_SETMODE);

    analogInput_.init(numAnalogInputs_);
    scanData_.resize(lastIndex(ANALOG_200)+1);

#if PEAK_JUMP_SUPPORT
    const unsigned initialPeakJumpMasses[] = { 18, 28, 32, 44 }; 
    const size_t IPJ_LENGTH = sizeof(initialPeakJumpMasses)/sizeof(initialPeakJumpMasses[0]);
    for (size_t i = 0; i < IPJ_LENGTH; ++i)
    {
        peakJumpMasses_.push_back(initialPeakJumpMasses[i]);
    }
#endif

    mainThread_ = new MainThread(this);

    timerQueue_ = &epicsTimerQueueActive::allocate(true);
    eventTimer_ = new EventTimer(this, *timerQueue_);
    eventTimer_->start(1.0);
}


MV2::~MV2()
{
    delete eventTimer_;
    delete mainThread_;
}

MV2::MainThread::MainThread(MV2 * owner)
    : owner(owner),
      thread(*this, "stateMachine", epicsThreadGetStackSize(epicsThreadStackMedium))
{
    thread.start();
}

void MV2::MainThread::run()
{
    owner->mainRun();
}


void MV2::mainRun()
{
    while (true)
    {
        processReceived();
        processTask();
        stateMachine();
    }
}

void MV2::stateMachine()
{
    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
        "(%s) SM St: %d tsk: %d ts:%d te:%d comms:%s\n",
        portName, state_, task_, taskState_, taskErrorStatus_, !commsFail_ ? "Y" : "N");

    if (taskState_ != TS_BUSY)
    {
        if (taskErrorStatus_ == TE_PROTOCOL_ERROR)
        {
            switch(state_)
            {
            case CONNECTING:
            case RELEASE_SENSOR:
                break;

            default:
                if (task_ == CONTROL)
                {
                    headState_.setValue(LOCAL_CONTROL);
                    changeState(SELECTED);
                }
                else
                {
                    changeState(RELEASE_SENSOR);
                }
                return;
            }
        }

        if (taskErrorStatus_ == TE_CMD_TIMEOUT)
        {
            asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "(%s) TIMEOUT ERROR\n", portName);
            error();
            epicsThreadSleep(1.0);
            return;
        }

        if (taskErrorStatus_ == TE_ASYN_ERROR)
        {
            asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, "(%s) ASYN ERROR\n", portName);
            error();
            epicsThreadSleep(1.0);
            return;
        }

        switch(state_)
        {
        case CONNECTING:
            if (taskState_ == TS_IDLE)
            {
                if (connect() == asynSuccess)
                {
                    changeState(RELEASE_SENSOR);
                }
                else
                {
                    epicsThreadSleep(1.0);
                }
            }
            break;

        case RELEASE_SENSOR:
            if (taskState_ == TS_IDLE)
            {
                startTask(INITIAL_RELEASE);
            }
            else
            {
                changeState(GET_SENSORS);
            }
            break;

        case GET_SENSORS:
            if (taskState_ == TS_IDLE)
            {
                sensors_.clear();
                startTask(SENSORS);
            }
            else
            {
                if (!sensors_.empty())
                {
                    changeState(SELECT_SENSOR);
                    if (commsFail_)
                    {
                        headState_.setValue(STOP);
                        commsFail_ = false;
                    }
                }
                else
                {
                    epicsThreadSleep(1.0);
                }
            }
            break;

        case SELECT_SENSOR:
            if (taskState_ == TS_IDLE)
            {
                startTask(SELECT);
            }
            else
            {
                changeState(INITIALIZING);
            }
            break;

        case INITIALIZING:
            if (taskState_ == TS_IDLE)
            {
                startTask(INIT);
            }
            else
            {
                if (filamentStatus_.filSta_ == FS_ON)
                {
                    filCon_.setValue(FC_ON);
                }
                fil_.setValue(filamentStatus_.filamentNum_);
                detector_.setValue(0);
                changeState(SELECTED);
            }
            break;

        case SELECTED:
            if (taskState_ == TS_IDLE)
            {
                if ((headState_.control() != LOCAL_CONTROL)
                 && (sensors_[0].state == "Ready")) 
                {
                    startTask(CONTROL);
                }
                else
                {
                    headState_.setValue(LOCAL_CONTROL);
                    if (scheduleGetInfo_)
                    {
                        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "(%s) SCHEDULE TASK\n", portName);
                        startTask(GET_INFO);
                    }
                }
            }
            else if (task_ == CONTROL)
            {
                changeState(IN_CONTROL);
            }
            break;

        case IN_CONTROL:
            asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
                "(%s) SM InCrtl con: %d hdsta: %d scan: %s\n",
                portName, headState_.control(), headState_.status(),
                (scanning_ ? "yes" : "no"));

            if (taskState_ == TS_IDLE)
            {
                if (headState_.control() == LOCAL_CONTROL)
                {
                    startTask(RELEASE);
                }
                else if (headState_.hasChanged() || detector_.hasChanged())
                {
                    startTask(CHANGE_STATE);
                }
                else if (!scanning_ &&
                        (scanMode_ == CONTINUOUS) && 
                        (isScanMode(headState_.status()) ))
                {
                    startTask(RESUME_SCAN);
                    scanning_ = true;
                }
                else if (filCon_.hasChanged())
                {
                    startTask(FILAMENT_CONTROL);
                }
                else if (fil_.hasChanged())
                {
                    startTask(SET_FILAMENT);
                }
                else if (scheduleGetInfo_)
                {
                    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "(%s) SCHEDULE TASK\n", portName);
                    startTask(GET_INFO);
                }
            }
            else
            {
                if (task_ == CHANGE_STATE)
                {
                    headState_.changeComplete();
                    detector_.changeComplete();
                }
                else if (task_ == FILAMENT_CONTROL)
                {
                    filCon_.changeComplete();
                }
                else if (task_ == SET_FILAMENT)
                {
                    fil_.changeComplete();
                }
                else if (task_ == RELEASE)
                {
                    headState_.setValue(LOCAL_CONTROL);
                    changeState(SELECTED);
                }
            }
            break;
        }
    }
}

void MV2::changeState(SmState state)
{
    state_ = state;
    commandToIdle();
}

void MV2::commandToIdle()
{
    cmdState_ = CMD_IDLE;
    cmd_="";
    cmdNumber_ = 0;
}

void MV2::taskToIdle()
{
    taskState_ = TS_IDLE;
    taskErrorStatus_ = TE_OK;
    task_ = NO_TASK;
    commandToIdle();
}

void MV2::processTask()
{
    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
        "(%s)  tsk %d ts:%d te:%d no:%d c:%s\n",
         portName, task_, taskState_, taskErrorStatus_, cmdNumber_, cmd_.c_str());

    switch (taskState_)
    {
    case TS_IDLE:
        return;

    case TS_COMPLETE:
        taskToIdle();
        return;

    case TS_BUSY:
        if (cmdState_ != CMD_BUSY)
        {
            if ((cmd_ != "") && (cmdStatus_ != "OK"))
            {
                taskState_ = TS_COMPLETE;
                taskErrorStatus_ = TE_PROTOCOL_ERROR;
                return;
            }

            if (cmdNumber_ < cmds.size())
            {
                asynStatus status = sendCommand(cmds[cmdNumber_], 1.0);

                if (status == asynSuccess)
                {
                    ++cmdNumber_;
                }
                else
                {
                   taskState_ = TS_COMPLETE;
                   taskErrorStatus_ = TE_ASYN_ERROR;
                   return;
                }
            }
            else
            {
                taskState_ = TS_COMPLETE;
            }
        }
        else
        {
            asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "(%s) BUSY!\n", portName);
            const unsigned BUSY_CTR_LIMIT = 10;
            if (busyCounter_ > BUSY_CTR_LIMIT)
            {
                asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                    "(%s) BUSY CTR LIMIT REACHED - ERROR!\n", portName);
                taskState_ = TS_COMPLETE;
                taskErrorStatus_ = TE_CMD_TIMEOUT;
                return;
            }
        }
        break;
    }
}

void MV2::startTask(Task task)
{
    task_ = task;
    cmdNumber_ = 0;
    taskState_ = TS_BUSY;
    cmds.clear();
    switch (task)
    {
    case NO_TASK:
        break;

    case INITIAL_RELEASE:
    case RELEASE:
        release(); 
        break;

    case SENSORS:
        sensors();
        break;

    case SELECT:
        select();
        break;

    case INIT:
        init();
        break;

    case CONTROL:
        controlSensor();
        break;

    case GET_INFO:
        getInfo();
        scheduleGetInfo_ = false;
        scheduleGetInfoCounter_ = 0;
        break;

    case RESUME_SCAN:
        resumeScan();
        scanning_ = true;
        break;

    case CHANGE_STATE:
        changeHeadState();
        break;

    case FILAMENT_CONTROL:
        setFilamentControl(filCon_.target());
        break;

    case SET_FILAMENT:
        selectFilament(fil_.target());
        break;
    }
}


asynStatus MV2::connect()
{
    int serialPortAddress = 0;
    asynStatus status = pasynOctetSyncIO->connect(port_.c_str(), serialPortAddress, &serialPortUser, NULL);

    if(status == asynSuccess) 
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) Connected to serial port=%s (%d) %p\n",
                portName, port_.c_str(), status, serialPortUser);
        pasynOctetSyncIO->setInputEos(serialPortUser, "\r\r", 2);
        pasynOctetSyncIO->setOutputEos(serialPortUser, "\r\n", 2);
    }
    else
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "(%s) Failed to connect to serial port=%s error=%d\n", portName, port_.c_str(), status);
    }

    return status;
}

void MV2::error()
{
    asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, "(%s) Error\n", portName);
    scanning_ = false;
    busyCounter_ = 0;
    commsFail_ = true;

    taskToIdle();

    changeState(RELEASE_SENSOR);
}

void MV2::sensors()
{
    cmds.push_back("Sensors");
}

void MV2::select()
{
    std::string cmd = "Select ";
    cmd += sensors_[0].serialNumber;
    cmds.push_back(cmd);
}

void MV2::init()
{
    cmds.push_back("FilamentInfo");
    cmds.push_back("MultiplierInfo");
}

void MV2::controlSensor()
{
    cmds.push_back("Control EPICS R3.14.11");
    cmds.push_back("FilamentInfo");
    //cmds.push_back("EGains");
    cmds.push_back("SourceInfo 0");     // These 2 commands seem to be necessary
    cmds.push_back("DetectorInfo 0");   // to workaround VRGA crashing
    cmds.push_back("MultiplierInfo");
    cmds.push_back("AnalogInputInterval 0 500000");
    cmds.push_back("AnalogInputEnable 0 true");
    cmds.push_back("MeasurementRemoveAll");
}

void MV2::release()
{
    cmds.push_back("Release");
} 

void MV2::getInfo()
{
    cmds.push_back("SensorState");
    cmds.push_back("Info");
    cmds.push_back("FilamentInfo");
    cmds.push_back("MultiplierInfo");
}

void MV2::setFilamentControl(FilamentControl con)
{
    switch (con)
    {
    case FC_OFF:
        cmds.push_back("FilamentControl Off");
        break;
    case FC_ON:
        cmds.push_back("FilamentControl On");
        break;
    }
}

void MV2::selectFilament(int fil)
{
    switch (fil)
    {
    case 0:
        cmds.push_back("FilamentSelect 1");
        break;
    case 1:
        cmds.push_back("FilamentSelect 2");
        break;
    }
}

void MV2::resumeScan()
{
    cmds.push_back("ScanResume 1");
}

std::string MV2::makeAddBarChartCommand(const std::string & name)
{
    char buffer[SMALL_BUFFER_SIZE];
    snprintf(buffer, SMALL_BUFFER_SIZE, "AddBarchart %s 1 %u PeakCenter %u %u %u %d",
        name.c_str(), lastMass(headState_.target()), accuracy_,
        egain_, source_, detector_.target());

    return buffer;
}

std::string MV2::makeAddAnalogCommand(const std::string & name)
{
    char buffer[SMALL_BUFFER_SIZE];
    snprintf(buffer, SMALL_BUFFER_SIZE, "AddAnalog %s 1 %u %u %u %u %u %d",
        name.c_str(), lastMass(headState_.target()), pointsPerPeak_,
        accuracy_, egain_, source_, detector_.target());
    return buffer;
}

std::string MV2::makePeakJumpCommand(const std::string & name)
{
    char buffer[SMALL_BUFFER_SIZE];
    snprintf(buffer, SMALL_BUFFER_SIZE, "AddPeakJump %s PeakCenter %u %u %u %d",
        name.c_str(), accuracy_, egain_, source_, detector_.target());
    return buffer;
}

void MV2::changeHeadState()
{
    cmds.push_back("ScanStop");
    switch (headState_.target())
    {
    case STOP:
    case LOCAL_CONTROL:
        break;

    case BARCHART_50:
    case BARCHART_100:
    case BARCHART_200:
        cmds.push_back("MeasurementRemoveAll"); 
        cmds.push_back(makeAddBarChartCommand("Bar1"));
        cmds.push_back("ScanAdd Bar1");
        cmds.push_back("ScanStart 1");
        break;

    case ANALOG_200:
        cmds.push_back("MeasurementRemoveAll");
        cmds.push_back(makeAddAnalogCommand("Ana1og1"));
        cmds.push_back("ScanAdd Ana1og1");
        cmds.push_back("ScanStart 1");
        break;	

#if PEAK_JUMP_SUPPORT
    case PEAK_JUMP:
        cmds.push_back("MeasurementRemoveAll");
        cmds.push_back(makePeakJumpCommand("PeakJump1"));
        for (std::vector<unsigned>::const_iterator it = peakJumpMasses_.begin();
             it != peakJumpMasses_.end();
             ++it)
        {
            char buffer[SMALL_BUFFER_SIZE];
            sprintf(buffer, "MeasurementAddMass %u", *it);
            cmds.push_back(buffer);
        } 
        cmds.push_back("ScanAdd PeakJump1");
        cmds.push_back("ScanStart 1");
        break;	
#endif

    default:
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "(%s) Unsupported head state - should not get here %u\n", portName, static_cast<unsigned>(headState_.status()));
        break; 
    }
}


asynStatus MV2::sendCommand(const std::string & data, double timeout)
{
    size_t  nWrite = 0;
    cmdState_ = CMD_BUSY;
    cmd_ = data;
    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "(%s) Send cmd: %s\n", portName, cmd_.c_str());

    asynStatus status = pasynOctetSyncIO->write(serialPortUser, cmd_.c_str(), cmd_.length(), timeout, &nWrite);
    return status;
}


void MV2::handleTimer()
{
    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "(%s) Scheduled get info\n", portName);

    ++scheduleGetInfoCounter_;
    const unsigned GET_INFO_PERIOD = 3;
    if (++scheduleGetInfoCounter_ > GET_INFO_PERIOD)
    {
        scheduleGetInfo_ = true;
        ++scheduleGetInfoCounter_ = 0;
    }
    ++busyCounter_;
}


void MV2::processReceived()
{
    if (serialPortUser != 0)
    {
        char buffer[LARGE_BUFFER_SIZE];
        buffer[sizeof(buffer)-1] = 0;

        int eomReason;
        size_t nRead;
        asynStatus status = pasynOctetSyncIO->read(serialPortUser, buffer, sizeof(buffer)-1, 0.1, &nRead, &eomReason);

        if(status == asynSuccess)
        {
            asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
                "(%s) Read ok, %u bytes\n", portName, nRead);
            buffer[nRead] = 0;
            char headerBuffer[SMALL_BUFFER_SIZE];

            const int HEADER_NUM_SCANNED_EXPECTED = 1;
            if (sscanf(buffer, "%s %*s", headerBuffer) == HEADER_NUM_SCANNED_EXPECTED)
            {
                std::string event(headerBuffer);
                asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
                    "(%s) ASYN_EVENT: %s\n", portName, event.c_str());

                if (event == "MassReading")
                {
                    float mass = 0;
                    double reading = 0;

                    const int NUM_SCANNED_EXPECTED = 2;
                    int numScanned = sscanf(buffer, "MassReading %f %lg", &mass, &reading);

                    if (numScanned == NUM_SCANNED_EXPECTED)
                    {
                        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
                            "(%s) Mass Reading(%f)=%lg\n", portName, mass, reading);

                        handleMassReading(mass, reading);
                    }
                    else
                    {
                        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                            "MassReading invalid\n%s\n", escapedFromRaw(buffer, nRead).c_str());
                    }
                }
                else if (event == cmd_.substr(0, event.length()))
                {
                    char statusBuffer[SMALL_BUFFER_SIZE];
                    const int NUM_SCANNED_EXPECTED = 1;

                    if (sscanf(buffer, "%*s %s\r\n", statusBuffer) == NUM_SCANNED_EXPECTED)
                    {
                        cmdStatus_ = statusBuffer;
                    }
                    else
                    {
                        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                            "(%s) Command status invalid\n%s\n",
                            portName, escapedFromRaw(buffer, nRead).c_str());
                        cmdStatus_ = "Error";
                    }
                    cmdState_ = CMD_IDLE;
                    busyCounter_ = 0;
                    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
                        "CMD STATUS %s %s (%s)\n", event.c_str(), cmd_.c_str(), cmdStatus_.c_str());
                }
                else if (event == "AnalogInput")
                {
                    double ai = 0;
                    unsigned index = 0;
                    const int NUM_SCANNED_EXPECTED = 2;
                    if (sscanf(buffer, "AnalogInput %u %lg", &index, &ai) == NUM_SCANNED_EXPECTED)
                    {
                        if (!analogInput_.setValue(index, ai))
                        {
                            asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                                "(%s) AnalogInput - invalid input number\n%s\n",
                                portName, escapedFromRaw(buffer, nRead).c_str());
                        }
                    }
                    else
                    {
                        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                            "(%s) AnalogInput invalid\n%s\n",
                            portName, escapedFromRaw(buffer, nRead).c_str());
                    }
                }
                else if (event == "FilamentStatus")
                {
                    std::string notification(buffer);
                    processFilamentStatus(notification);
                }
                else if (event == "StartingScan")
                {
                    scanning_ = true;
                }

                if (event == "Sensors")
                {
                    char okBuffer[SMALL_BUFFER_SIZE];
                    if (sscanf(buffer, "Sensors %s\r\n  ", okBuffer))
                    {
                        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "(%s) Status:%s\n", portName, okBuffer);
                        okBuffer[SMALL_BUFFER_SIZE-1] = '\0';
                        if (std::string(okBuffer) == "OK")
                        {
                            sensors_.clear();
                            char state[SMALL_BUFFER_SIZE];
                            char serialNumber[SMALL_BUFFER_SIZE];
                            char name[SMALL_BUFFER_SIZE];
                            int numScanned = sscanf(buffer,
                                "Sensors %*s\r\n  State  SerialNumber   Name\r\n  %s  %s %s",
                                state, serialNumber, name); 

                            const int NUM_SCANNED_EXPECTED = 3;
                            if (numScanned == NUM_SCANNED_EXPECTED)
                            {
                                asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
                                    "(%s) Sensor: State:%s SN:%s Name:%s\n", 
                                    portName, state, serialNumber, name);
                                SensorInfo sensor = { state, serialNumber, name };
                                sensors_.push_back(sensor);
                            }
                            else
                            {
                                asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                                    "(%s) No valid sensor info\n", portName);
                            }
                        }
                    }
                    else
                    {
                        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                            "(%s) Sensors command response not OK\n%s\n",
                            portName, escapedFromRaw(buffer, nRead).c_str());
                    }
                }
                else if (event == "SensorState")
                {
                    char state[SMALL_BUFFER_SIZE];
                    int numScanned = sscanf(buffer,
                        "SensorState OK\r\n"
                        "State %s\r\n"
                        "UserApplication %*s\r\n"
                        "UserVersion %*s\r\n"
                        "UserAddress %*s\r\n",
                         state);

                    const int NUM_SCANNED_EXPECTED = 1;
                    if (numScanned == NUM_SCANNED_EXPECTED)
                    {
                        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
                            "(%s) state %s\n", portName, state);
                        sensors_[0].state = state;
                    }
                    else
                    {
                        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                            "(%s) Sensor State command response not OK\n%s\n",
                            portName, escapedFromRaw(buffer, nRead).c_str());
                    }
                }
                else if (event == "FilamentInfo")
                {
                    std::string notification(buffer);
                    processFilamentInfo(notification);
                }
            }
            else
            {
                asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                    "notification invalid\n%s\n",escapedFromRaw(buffer, nRead).c_str());
            }
        }
    }
}



void MV2::handleMassReading(float mass, double reading)
{
    size_t index = massToIndex(mass);

    if (index < scanData_.size())
    {
        scanData_[index] = reading; 
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) ScanData[%u]:%lg\n", portName, static_cast<unsigned>(index), scanData_[index]);
    }
    else
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "(%s) Err:ScanData[%u]:%lg sd size:%u\n",
            portName, static_cast<unsigned>(index), reading, static_cast<unsigned>(scanData_.size()));
    }
    if (index == lastIndex(headState_.status()))
    {
        scanComplete();
    }
}

void MV2::processFilamentStatus(const std::string & notification)
{
    // set filsta
    unsigned filNum = 0;

    char filstaBuffer[SMALL_BUFFER_SIZE];
    char tripBuffer[SMALL_BUFFER_SIZE];
    char exTripStateBuffer[SMALL_BUFFER_SIZE];

    int numScanned = sscanf(notification.c_str(),
        "FilamentStatus  %u %100s\r\n"
        "  Trip  %100s\r\n"
        "  Drive %*s\r\n"
        "  EmissionTripState %*s\r\n"
        "  ExternalTripState %100s\r\n"
        "  RVCTripState  %*s\r\n"
        "  GaugeTripState  %*s",
        &filNum, filstaBuffer, tripBuffer, exTripStateBuffer); 

    const int NUM_SCANNED_EXPECTED = 4;
    if (numScanned == NUM_SCANNED_EXPECTED)
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) Filnum: %d State: %s Trip:%s exTripState:%s\n",
             portName, filNum, filstaBuffer, tripBuffer, exTripStateBuffer);

        FilamentState state = FS_FAILED;

        filstaBuffer[SMALL_BUFFER_SIZE-1] = '\0';
        std::string filsta(filstaBuffer);

        if (filsta == "OFF") 
        {
            state = FS_OFF;
        }
        else if (filsta == "ON")
        {
            state = FS_ON;
        }
        else if (filsta == "WARM-UP")
        {
            state = FS_WARM_UP;
        }
        else if (filsta == "COOL-DOWN")
        {
            state = FS_COOL_DOWN;
        }
        else
        {
            asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                "(%s) UNKNOWN FILAMENT STATE\n", portName);
        }

        filamentStatus_ = FilamentStatus(filNum-1, state, tripBuffer);
    }
    else
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "FilamentStatus invalid:\n%s\n", escapedFromRaw(notification).c_str());
    }
}

void MV2::processFilamentInfo(const std::string & notification)
{
    // set filsta
    unsigned filNum = 0;
    char filstaBuffer[SMALL_BUFFER_SIZE];
    char tripBuffer[SMALL_BUFFER_SIZE];
    char exTripStateBuffer[SMALL_BUFFER_SIZE];

    int numScanned = sscanf(notification.c_str(),
        "FilamentInfo  OK\r\n"
        "  SummaryState  %s\r\n"
        "  ActiveFilament  %u\r\n"
        "  ExternalTripEnable %*s\r\n"
        "  ExternalTripMode  %*s\r\n"
        "  EmissionTripEnable %*s\r\n"
        "  MaxOnTime %*s\r\n"
        "  OnTimeRemaining %*s\r\n"
        "  Trip  %s\r\n"
        "  Drive %*s\r\n"
        "  EmissionTripState %*s\r\n"
        "  ExternalTripState %s\r\n"
        "  RVCTripState  %*s\r\n"
        "  GaugeTripState  %*s\r\n",
        filstaBuffer, &filNum, tripBuffer, exTripStateBuffer);

    const int NUM_SCANNED_EXPECTED = 4;
    if (numScanned == NUM_SCANNED_EXPECTED)
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) FilamentInfo: Filnum: %d State: %s Trip:%s exTripState:%s\n",
            portName, filNum, filstaBuffer, tripBuffer, exTripStateBuffer);

        FilamentState state = FS_FAILED;

        filstaBuffer[SMALL_BUFFER_SIZE-1] = '\0';
        std::string filsta(filstaBuffer);

        if (filsta == "OFF")
        {
            state = FS_OFF;
        }
        else if (filsta == "ON")
        {
            state = FS_ON;
        }
        else if (filsta == "WARM-UP")
        {
            state = FS_WARM_UP;
        }
        else if (filsta == "COOL-DOWN")
        {
            state = FS_COOL_DOWN;
        }
        else
        {
            asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                "(%s) UNKNOWN FILAMENT STATE\n", portName);
        }

        filamentStatus_ = FilamentStatus(filNum-1, state, tripBuffer);
    }
    else
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "FilamentInfo invalid:\n%s\n", escapedFromRaw(notification).c_str());
    }
}



void MV2::scanComplete()
{
    switch (headState_.status())
    {
    case BARCHART_50:
        currentData_ = BAR50;
        barResultsData_.resize(50);
        analogResultsData_.resize(0);
        break;

    case BARCHART_100:
        currentData_ = BAR100;
        barResultsData_.resize(100);
        analogResultsData_.resize(0);
        break;

    case BARCHART_200:
        currentData_ = BAR200;
        barResultsData_.resize(200);
        analogResultsData_.resize(0);
        break;

    case ANALOG_200:
        currentData_ = ANALOG200;
        barResultsData_.resize(200);
        analogResultsData_.resize(scanData_.size());
        break;

#if PEAK_JUMP_SUPPORT
    case PEAK_JUMP:
        currentData_ = PEAKJUMP;
        barResultsData_.resize(200);
        analogResultsData_.resize(0);
        break;
#endif

    default:
        break;
    }

    currentDetector_ = detector_.status()+1;

    sumP_ = 0;

    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
        "(%s) Sizes %u %u\n",
        portName, static_cast<unsigned>(barResultsData_.size()), static_cast<unsigned>(scanData_.size()));

    // cut-off partial pressures below threshold as per MV Plus.
    const double MIN_PRESSURE = 1.0e-12;

    switch (headState_.status())
    {
    case BARCHART_50:
    case BARCHART_100:
    case BARCHART_200:
        for (size_t index = 0; index < barResultsData_.size(); ++index)
        {
            barResultsData_[index] = std::max(scanData_[index], MIN_PRESSURE);
            asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
                 "(%s) %u %lg %lg\n",
                 portName, static_cast<unsigned>(index), scanData_[index], barResultsData_[index]);
            sumP_ += barResultsData_[index];
        }
        break;

    case ANALOG_200:
        for (size_t index = 0; index < barResultsData_.size(); ++index)
        {
            double result = 0.0;
            const size_t massOneStart = (lastIndex(ANALOG_200)+1) - (lastMass(ANALOG_200)* pointsPerPeak_);
            for (size_t j = 0; j < pointsPerPeak_; ++j)
            {
                size_t offset = massOneStart + index * pointsPerPeak_ + j;
                if (offset < scanData_.size())
                {
                    result = std::max(result, scanData_[offset]);
                }
                else
                {
                    asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                        "(%s) Bad offset %u %u %u\n",
                        portName, static_cast<unsigned>(index), static_cast<unsigned>(j),
                        static_cast<unsigned>(offset));
                }
            }

            std::copy(scanData_.begin(), scanData_.end(), analogResultsData_.begin());

            barResultsData_[index] = std::max(result, MIN_PRESSURE);
            asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
                 "(%s) %u %g %g\n", portName, static_cast<unsigned>(index), result, barResultsData_[index]);
            sumP_ += barResultsData_[index];
        }
        break;

#if PEAK_JUMP_SUPPORT
    case PEAK_JUMP:
        for (size_t index = 0; index < barResultsData_.size(); ++index)
        {
            barResultsData_[index] = std::max(scanData_[index], 0.0);
            asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
                 "(%s) %u %u %u\n",
                 portName, index, scanData_[index], barResultsData_[index]);
            sumP_ += barResultsData_[index];
        }
        break;
#endif

    default:
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "(%s) Scan complete in non-scan mode %u\n", portName, static_cast<unsigned>(headState_.status()));
        break;
    }

    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "(%s) SUMP:%lg\n", portName, sumP_);
    scanning_ = false;
    if ((!headState_.hasChanged()) && (scanMode_ == SINGLE))
    {
        headState_.newSetting(STOP);
    }
}




unsigned MV2::lastMass(HeadState sta)
{
    unsigned mass = 0;
    switch (sta)
    {
    case BARCHART_50:
        mass = 50;
        break;

    case BARCHART_100:
        mass = 100;
        break;

    case BARCHART_200:
        mass = 200;
        break;

    case ANALOG_200:
        mass = 200;
        break;

#if PEAK_JUMP_SUPPORT
    case PEAK_JUMP:
        if (!peakJumpMasses_.empty())
        {
            mass = peakJumpMasses_.back();
        }
        break;
#endif

    default:
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "(%s) Last mass called in non-scan mode %u\n", portName, static_cast<unsigned>(sta));
        break;
    }
    return mass;
}

unsigned MV2::lastMass()
{
    return lastMass(headState_.status());
}

size_t MV2::lastIndex(HeadState sta)
{
    unsigned index = 0;
    switch (sta)
    {
    case BARCHART_50:
    case BARCHART_100:
    case BARCHART_200:
        index = lastMass(sta)-1;
        break;

    case ANALOG_200:
        index = lastMass(sta)*pointsPerPeak_+pointsPerPeak_/2-2;
        break;

#if PEAK_JUMP_SUPPORT
    case PEAK_JUMP:
        index = lastMass(sta)-1;
        break;
#endif

    default:
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "(%s) Last index called in non-scan mode %u\n", portName, static_cast<unsigned>(sta));
        break;
    }
    return index;
}

size_t MV2::massToIndex(float mass) 
{
    size_t index = UINT_MAX;

    switch (headState_.status())
    {
    case BARCHART_50:
    case BARCHART_100:
    case BARCHART_200:
#if PEAK_JUMP_SUPPORT
    case PEAK_JUMP:
#endif
        index = static_cast<size_t>(mass-0.5);
        break;

    case ANALOG_200:
        index = static_cast<size_t>(mass*pointsPerPeak_-0.5);
        break;

    default:
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "(%s) Last index called in non-scan mode %u\n", portName, static_cast<unsigned>(headState_.status()));
        break;
    }
    return index;
}

bool MV2::isBarchart(HeadState sta)
{
    bool isBar = false;
    switch (sta)
    {
    case BARCHART_50:
    case BARCHART_100:
    case BARCHART_200:
        isBar = true;
        break;

    default:
        break;
    }
    return isBar;
}

bool MV2::isScanMode(HeadState sta)
{
    bool isScan = false;
    switch (sta)
    {
    case BARCHART_50:
    case BARCHART_100:
    case BARCHART_200:
    case ANALOG_200:
#if PEAK_JUMP_SUPPORT
    case PEAK_JUMP:
#endif
        isScan = true;
        break;

    default:
        break;
    }
    return isScan;
}

double MV2::totalPressure()
{
    const size_t TOTAL_PRESSURE_INPUT = 0;
    double pressure = 0.0;

    if (analogInput_.hasValue(TOTAL_PRESSURE_INPUT))
    {
        double ai = analogInput_.getValue(TOTAL_PRESSURE_INPUT);
        pressure = MKS937::voltageToMbar(ai);
    }
    return pressure;
}

asynStatus MV2::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    int reason = pasynUser->reason;
    asynPrint(pasynUser, ASYN_TRACE_FLOW,
        "(%s) writeInt32 (%s) (%d)\n", portName, getParameterName(reason).c_str(), value);
    asynPortDriver::writeInt32(pasynUser, value);

    if (commsFail_)
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) Wr Int32 %s: DISCONNECTED\n", portName, getParameterName(reason).c_str());
        return asynDisconnected;
    }

    asynStatus status = asynSuccess;

    if (reason == P_CON)
    {
        switch (value)
        {
        case STOP:
        case LOCAL_CONTROL:
        case BARCHART_50:
        case BARCHART_100:
        case BARCHART_200:
        case ANALOG_200:
#if PEAK_JUMP_SUPPORT
        case PEAK_JUMP:
#endif
            headState_.newSetting(static_cast<HeadState>(value));
            break;

        // Not currently implemented. May or may not be implemented in future.
        // (Degas maybe, calibration probably not).
        // Keep these for now for compatibility with current operator panels
        // which may may require certain hard-coded values.
        case DEGAS_FILAMENT:
        case CAL_FARADAY:
        case CAL_MULTIPLIER:
            asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                "(%s) Head state %d not currently supported\n", portName, value);
            status = asynError;
            break;

        default:
            asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                "(%s) Unsupported head state %d\n", portName, value);
            status = asynError;
            break;
        }
    }
    else if (reason == P_SETSCAN)
    {
        switch (value)
        {
        case CONTINUOUS:
            scanMode_ = CONTINUOUS;
            break;

        case SINGLE:
            scanMode_ = SINGLE;
            break;

        default:
            status = asynError;
            break;
        }
    }
    else if (reason == P_FILCON)
    {
        switch (value)
        {
        case FC_ON:
        case FC_OFF:
            {
                FilamentControl fc = static_cast<FilamentControl>(value);
                filCon_.forceNewSetting(fc);
            }
            break;

        default:
            status = asynError;
            break;
        }
    }
    else if (reason == P_MLTCON)
    {
        if ((value >= 0) && (value < numDetectorIndexes_))
        {
            detector_.newSetting(value);
        }
        else
        {
            status = asynError;
        }
    }
    else if (reason == P_SETFIL)
    {
        if ((value >= 0) && (value < numFilaments_))
        {
            fil_.forceNewSetting(value);
        }
        else
        {
            status = asynError;
        }
    }
    else if (reason == P_SETMODE)
    {
        // TODO - Find out what needs to go here
        // as far as can tell this doesn't do anything in MVPlus.
    }
    else
    {
        status = asynError;
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, "(%s) UNKNOWN REASON(%d)\n", portName, reason);
    }

    return status;
}


asynStatus MV2::readInt32(asynUser *pasynUser, epicsInt32 * value)
{
    int reason = pasynUser->reason;

    if (commsFail_)
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) Rd Int32 %s: DISCONNECTED\n", portName, getParameterName(reason).c_str());
        return asynDisconnected;
    }

    epicsInt32 newValue = *value;

    if (reason == P_DATA)
    {
        newValue = currentData_;
    }
    else if (reason == P_HEADSTA)
    {
        newValue = headState_.status();
    }
    else if (reason == P_SCAN)
    {
        newValue = scanMode_;
    }
    else if (reason == P_FILSTA)
    {
        newValue = filamentStatus_.filSta_;
    }
    else if (reason == P_MLTSTA)
    {
        newValue = detector_.status();
    }
    else if (reason == P_FIL)
    {
        newValue = filamentStatus_.filamentNum_;
    }
    else if (reason == P_DET)
    {
        newValue = currentDetector_;
    }
    else if (reason == P_ILK)
    {
        newValue = (filamentStatus_.trip_ == "None") ? TS_OK : TS_FAIL;
    }
    else if (reason == P_MODE)
    {
        // as far as can tell this doesn't do anything in MVPlus except return 0 so do that for now.
        // leave for now for compatibility with MVPlus panels
        newValue = 0;
    }
    else if (reason == P_CON)
    {
        newValue = headState_.control();
    }
    else if (reason == P_FILCON)
    {
        newValue = filCon_.control();
    }
    else if (reason == P_SETFIL)
    {
        newValue = fil_.control();
    }
    else if (reason == P_SETSCAN)
    {
        newValue = scanMode_;
    }
    else if (reason == P_MLTCON)
    {
        newValue = detector_.status();
    }
    else if (reason == P_SETMODE)
    {
        // as per MODE
        newValue = 0;
    }
    else
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "(%s) Rd32 Error %s\n", portName, getParameterName(reason).c_str());
        return asynError;
    }

    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
        "(%s) Rd32 %s: %d->%d\n",
        portName, getParameterName(reason).c_str(), *value, newValue);
    *value = newValue;

    return asynSuccess;
}

asynStatus MV2::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
{
    int reason = pasynUser->reason;

    if (commsFail_)
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) Rd Flt64 %s: DISCONNECTED\n", portName, getParameterName(reason).c_str());
        return asynDisconnected;
    }

    epicsFloat64 newValue = 0.0;

    // try and match to a BAR mass reading first. Assume BARM parameters contiguous.
    int barIndex = (reason - P_BARM[0]);
    bool isBar = ((barIndex >= 0 && barIndex < NUM_MASSES) && (reason == P_BARM[barIndex]));

    if (isBar)
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) P_BARM[%d]=%d\n", portName, barIndex, P_BARM[barIndex]);

        if (static_cast<size_t>(barIndex) < barResultsData_.size())
        {
            newValue = paToMbar(barResultsData_[barIndex]);
            asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "(%s) readFloat64 %s %lg->%lg\n",
            portName, getParameterName(reason).c_str(), *value, newValue);
        }
    }
    else if (reason == P_SUMP)
    {
        newValue = paToMbar(sumP_);
    }
    else if (reason == P_TOTP)
    {
        newValue = totalPressure();
    }
    else
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) Unexpected parameter (%s)\n", portName, getParameterName(reason).c_str());
        return asynError;
    }

    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "(%s) readFloat64 %s %lg->%lg\n",
        portName, getParameterName(reason).c_str(), *value, newValue);

    *value = newValue;
    return asynSuccess;
}

asynStatus MV2::readFloat64Array(asynUser *pasynUser, epicsFloat64 *value,
                            size_t nElements, size_t *nIn)
{
    int reason = pasynUser->reason;

    if (commsFail_)
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) Rd Flt64Array %s: DISCONNECTED\n", portName, getParameterName(reason).c_str());
        return asynDisconnected;
    }

    if (reason == P_ANA)
    {
        *nIn = std::min(analogResultsData_.size(), nElements);
        //std::copy(analogResultsData_.begin(), analogResultsData_.begin() + *nIn, value);
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) Rd Flt64Array ANA:%u elements\n", portName, static_cast<unsigned>(*nIn));

        for (std::vector<double>::const_iterator it = analogResultsData_.begin();
             it != analogResultsData_.begin() + *nIn; ++it)
        {
            *value++ = paToMbar(*it);
        }
        return asynSuccess;
    }
    else if (reason == P_BAR)
    {
        *nIn = std::min(barResultsData_.size(), nElements);
        //std::copy(barResultsData_.begin(), barResultsData_.begin() + *nIn, value);
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "(%s) Rd Flt64Array BAR:%u elements\n", portName, static_cast<unsigned>(*nIn));

        for (std::vector<double>::const_iterator it = barResultsData_.begin();
             it != barResultsData_.begin() + *nIn; ++it)
        {
            *value++ =  paToMbar(*it);
        }
        return asynSuccess;
    }
    else
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "(%s) Rd Flt64Array (Unkown): Error\n", portName);
        return asynError;
    }
}

std::string MV2::getParameterName(int reason)
{
    const char *paramName;
    if (getParamName(reason, &paramName) == asynSuccess)
    {
        return paramName;
    }
    return "Unkown";
}


void MV2::AnalogInput::init(unsigned numInputs)
{
    values_.resize(numInputs);
    inputsValid_.resize(numInputs);
}

bool MV2::AnalogInput::hasValue(unsigned input)
{
    return (input < inputsValid_.size()) && inputsValid_[input];
}

double MV2::AnalogInput::getValue(unsigned input)
{
    double value = 0.0;
    if (input < values_.size())
    {
        value = values_[input];
    }
    return value;
}

bool MV2::AnalogInput::setValue(unsigned input, double value)
{
    bool ok = (input < values_.size()) && (input < inputsValid_.size());
    if (ok)
    {
        values_[input] = value;
        inputsValid_[input] = true;
    }
    return ok;
}

std::string escapedFromRaw(const char * buffer, size_t length)
{
    const size_t OUT_LENGTH = 2*length+1;
    char outBuffer[OUT_LENGTH];
    outBuffer[OUT_LENGTH-1] = '\0';
    epicsStrnEscapedFromRaw(outBuffer, OUT_LENGTH-1, buffer, length);
    return outBuffer;
}

std::string escapedFromRaw(const std::string & str)
{
    return escapedFromRaw(str.c_str(), str.length()+1);
}


} // namespace rgamv2

extern "C"
{
    static const iocshArg InitArg[] = { { "name", iocshArgString } , { "port", iocshArgString }};
    static const iocshArg * const InitArgs[] = { &InitArg[0], &InitArg[1] };
    static const iocshFuncDef InitFuncDef = {"mv2init", 2, InitArgs};

    static void InitCallFunc(const iocshArgBuf * args)
    {
        printf("InitCallFunc %s %s\n", args[0].sval, args[1].sval);
        new rgamv2::MV2(args[0].sval, args[1].sval);
    }

    void mv2AsynRegistrar(void)
    {
        iocshRegister(&InitFuncDef,InitCallFunc);
    }
    epicsExportRegistrar(mv2AsynRegistrar);
}


