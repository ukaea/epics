#define epicsExportSharedSymbols
#include "Exception.h"

#include <sstream>

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

