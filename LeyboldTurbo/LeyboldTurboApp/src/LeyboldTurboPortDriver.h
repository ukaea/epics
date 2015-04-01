//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		LeyboldTurboPortDriver.h																	//
//																									//
//	Description:																					//
//		Declares the CLeyboldTurboPortDriver class.													//
//		This uses AsynPortDriver and asynOctetSyncIO to connect with either:						//
//			a. One or more serial-connected Leybold turbo pump controllers.							//
//			b. One or more TCP-connected simulatied controllers.									//
//		The class communicates by means of the Universal Serial Interface (USS)						//
//		protocol (http://www.automation.siemens.com/WW/forum/guests/PostShow.aspx?PostID=104133).	//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 05 Jan 2015.									//
//	Written for CCFE (Culham Centre for Fusion Energy).												//
//																									//
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LEYBOLD_TURBO_PORT_DRIVER_H
#define LEYBOLD_TURBO_PORT_DRIVER_H

#include "LeyboldBase.h"
#include "USSPacket.h"

#include <map>
#include <string>
#include <vector>

class CLeyboldTurboPortDriver : public CLeyboldBase {
public:
	// NB, an MBBI string is limited to 40 charachters in EPICS.
	static const size_t MaxEPICSMBBIStrLen = 16;
	static const size_t BitsPerUInt16 = 16;

    CLeyboldTurboPortDriver(const char *AsynPortName, int NumPumps, int NoOfPZD);
    ~CLeyboldTurboPortDriver();
    virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars,
                                        size_t *nActual, int *eomReason);
	void addIOPort(const char* IOPortName);
                 
protected:
	template<size_t NoOfPZD> void writeRead(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD> USSWritePacket, USSPacket<NoOfPZD>& USSReadPacket);
	template<size_t NoOfPZD> void processRead(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD> const& USSReadPacket);
	template<size_t NoOfPZD> void processWrite(int TableIndex, asynUser *pasynUser, epicsInt32 value);
	void process(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD2> const& USSWritePacket, USSPacket<NoOfPZD2>& USSReadPacket);
	void process(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD6> const& USSWritePacket, USSPacket<NoOfPZD6>& USSReadPacket);

private:
	// Each of these is associated with an octet I/O connection (i.e. serial or TCP port).
	std::vector<asynUser*> m_AsynUsers;
};

#endif // LEYBOLD_TURBO_PORT_DRIVER_H
