#include "Exception.h"

#include <sstream>
#include <stdlib.h>

CException::CException(asynUser* AsynUser, asynStatus Status, const char* File, const char* functionName, std::string const& what) :
	std::runtime_error(what) 
{
	std::string message = "%s:%s ERROR: " + what + "%s %d\n";
	asynPrint(AsynUser, ASYN_TRACE_ERROR, message.c_str(), File, functionName, AsynUser->errorMessage, Status);
	m_Status = Status;
}

std::string Wcstombs(std::wstring const& WideCharString)
{
	char NBuf[MaxEPICSStrLen];
	memset(NBuf, 0, sizeof(NBuf));
	::wcstombs(NBuf, WideCharString.c_str(), sizeof(NBuf)-1);
	return NBuf;
}

std::wstring Mbstowcs(std::string const& MultiByteString)
{
	wchar_t WBuf[MaxEPICSStrLen];
	memset(WBuf, 0, sizeof(WBuf));
	::mbstowcs(WBuf, MultiByteString.c_str(), (sizeof(WBuf) - sizeof(wchar_t)) / sizeof(wchar_t));
	return WBuf;
}

std::string ToString(double value)
{
	std::ostringstream o;
	o << value;
	return o.str ();
}

void FromString(std::string const& String, size_t& value)
{
	value = atoi(String.c_str());
}

void FromHexString(std::string const& String, size_t& value)
{
	char* EndPtr;
	value = strtoul(String.c_str(), &EndPtr, 16);
}

void FromString(std::string const& String, double& value)
{
	value = atof(String.c_str());
}

