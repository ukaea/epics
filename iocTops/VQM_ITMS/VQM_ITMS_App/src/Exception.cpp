#define epicsExportSharedSymbols
#include "Exception.h"

std::string wcstombs(std::wstring const& WideCharString)
{
	char NBuf[MaxEPICSStrLen];
	memset(NBuf, 0, sizeof(NBuf));
	::wcstombs(NBuf, WideCharString.c_str(), sizeof(NBuf)-1);
	return NBuf;
}

std::wstring mbstowcs(std::string const& MultiByteString)
{
	wchar_t WBuf[MaxEPICSStrLen];
	memset(WBuf, 0, sizeof(WBuf));
	::mbstowcs(WBuf, MultiByteString.c_str(), (sizeof(WBuf) - sizeof(wchar_t)) / sizeof(wchar_t));
	return WBuf;
}
