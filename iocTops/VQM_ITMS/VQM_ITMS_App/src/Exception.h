#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED

#ifdef epicsExportSharedSymbols
#define VQM_ExceptionepicsExportSharedSymbols
#undef epicsExportSharedSymbols
#endif
#include <asynPortDriver.h>
#ifdef VQM_ExceptionepicsExportSharedSymbols
#undef VQM_ExceptionepicsExportSharedSymbols
#define epicsExportSharedSymbols
#include <shareLib.h>
#endif

#include <stdexcept>
#include <string>
#include <stdlib.h>
#include<string.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	class CException::CException : public std::runtime_error									//
//	Description:																				//
//		If an error ocurrs, an object of this type is thrown.									//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
class CException : public std::runtime_error
{
	asynStatus m_Status;
public:
	CException(asynUser* AsynUser, asynStatus Status, const char* File, const char* functionName, std::string const& what);
	asynStatus Status() const {
		return m_Status;
	}
};

// NB, a string is limited to 40 charachters in EPICS.
static const size_t MaxEPICSStrLen = 40;

std::string epicsShareFunc Wcstombs(std::wstring const& WideCharString);

std::wstring epicsShareFunc Mbstowcs(std::string const& MultiByteString);

std::string ToString(double value);
void FromString(std::string const& String, size_t& value);
void FromHexString(std::string const& String, size_t& value);
void FromString(std::string const& String, double& value);

#endif //EXCEPTION_H_INCLUDED
