//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		LeyboldBase.h																				//
//																									//
//	Description:																					//
//		Declares the CLeyboldBase class that forms an interm,ediate base class for both				//
//		CLeyboldTurboPortDriver and CLeyboldSimPortDriver.											//
//		Includes parameter management and error handling.											//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 04 Mar 2015.									//
//	Written for CCFE (Culham Centre for Fusion Energy).												//
//																									//
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LEYBOLD_BASE_H
#define LEYBOLD_BASE_H

#include "ParameterDefns.h"

#include <asynPortDriver.h>

#include <stdexcept>
#include <string>
#include <map>

class CLeyboldBase : public asynPortDriver
{
public:
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//																								//
	//	class CLeyboldTurboPortDriver::CException : public std::runtime_error						//
	//	Description:																				//
	//		If an error ocurrs, an object of this type is thrown.									//
	//																								//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	class CException : public std::runtime_error
	{
	public:
		CException(asynUser* AsynUser, const char* functionName, std::string const& what) : std::runtime_error(what) {
			std::string message = "%s:%s ERROR: " + what + "%s\n";
			asynPrint(AsynUser, ASYN_TRACE_ERROR, message.c_str(), __FILE__, functionName, AsynUser->errorMessage);
		}
	};

	static const size_t NoOfPZD6 = 6;
	static const size_t NoOfPZD2 = 2;

	// NB, a string is limited to 40 charachters in EPICS.
	static const size_t MaxEPICSStrLen = 40;

    CLeyboldBase(const char *portName, int maxAddr, int paramTableSize, int NoOfPZD, int interfaceMask) :
		asynPortDriver(portName, maxAddr, paramTableSize, interfaceMask, interfaceMask, ASYN_MULTIDEVICE, 1, 0, 0) {
			m_NoOfPZD = NoOfPZD;
	}
    void createParam(size_t list, size_t ParamIndex) {
		int index;
		const char* ParamName = ParameterDefns[ParamIndex].m_ParamName;
		if (asynPortDriver::createParam(int(list), ParamName, ParameterDefns[ParamIndex].m_ParamType, &index) != asynSuccess)
			throw CException(pasynUserSelf, __FUNCTION__, ParamName);
		m_Parameters[ParamName] = index;
	}
    void setIntegerParam(size_t list, const char* ParamName, int value) {
		if (asynPortDriver::setIntegerParam(int(list), Parameters(ParamName), value) != asynSuccess)
			throw CException(pasynUserSelf, __FUNCTION__, ParamName);
	}
    void setDoubleParam(size_t list, const char* ParamName, double value) {
		if (asynPortDriver::setDoubleParam(int(list), Parameters(ParamName), value) != asynSuccess)
			throw CException(pasynUserSelf, __FUNCTION__, ParamName);
	}
	void setStringParam(size_t list, const char* ParamName, std::string const& value) {
		if (asynPortDriver::setStringParam (int(list), Parameters(ParamName), value.substr(0, MaxEPICSStrLen).c_str()) != asynSuccess)
			throw CException(pasynUserSelf, __FUNCTION__, ParamName);
	}
    int getIntegerParam(int list, const char* ParamName) {
		int value = 0;
		if (asynPortDriver::getIntegerParam(list, Parameters(ParamName), &value) != asynSuccess)
			throw CException(pasynUserSelf, __FUNCTION__, ParamName);
		return value;
	}
    double getDoubleParam(int list, const char* ParamName) {
		double value = 0;
		if (asynPortDriver::getDoubleParam(list, Parameters(ParamName), &value) != asynSuccess)
			throw CException(pasynUserSelf, __FUNCTION__, ParamName);
		return value;
	}
    void getStringParam(int list, const char* ParamName, int maxChars, char *value) {
		if (asynPortDriver::getStringParam(list, Parameters(ParamName), maxChars, value) != asynSuccess)
			throw CException(pasynUserSelf, __FUNCTION__, ParamName);
	}
	int Parameters(std::string const& ParamName) const {
		std::map<std::string, int>::const_iterator Iter = m_Parameters.find(ParamName);
		if (Iter == m_Parameters.end())
			throw CException(pasynUserSelf, __FUNCTION__, ParamName);
		return Iter->second;
	}
private:
	// Each parameter is associated with an int handle.
	// This structure is used in order to address them by name, which is more convenient.
	std::map<std::string, int> m_Parameters;

protected:
	size_t m_NoOfPZD;

};

#endif
