#ifndef LEYBOLD_TURBO_PORT_DRIVER_H
#define LEYBOLD_TURBO_PORT_DRIVER_H

#include <asynPortDriver.h>

#include <map>
#include <string>

class CLeyboldTurboPortDriver : public asynPortDriver {
public:
	class CException;
    CLeyboldTurboPortDriver(const char *AsynPortName, const char* IOPortName);
    ~CLeyboldTurboPortDriver();
    virtual asynStatus readFloat64(asynUser *pasynUser, epicsFloat64 *value);
    virtual asynStatus writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);
                 
    /* These are the methods that we override from asynPortDriver
    virtual asynStatus writeInt8Array(asynUser *pasynUser, epicsInt8 *value,
                                        size_t nElements);
    virtual asynStatus readInt8Array(asynUser *pasynUser, epicsInt8 *value, 
                                        size_t nElements, size_t *nIn);
*/
protected:
    /** Values used for pasynUser->reason, and indexes into the parameter library. */
	std::map<std::string, int> m_Parameters;
	asynUser* m_AsynUser;

private:
    /* Our data */
};

#endif // LEYBOLD_TURBO_PORT_DRIVER_H
