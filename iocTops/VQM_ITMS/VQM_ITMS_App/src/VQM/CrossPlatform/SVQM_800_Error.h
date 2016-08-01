#pragma once

#include <string>
/*
Maps to the VQM_800_ERROR class in the CSServiceWrapper DLL.
*/
//List of error types.
enum EnumErrorType { NO_ERRORS = 0, NET_EXCEPTION, NO_CONTROLLERS_FOUND, COMMUNICATION_EXCEPTION,\
        INSUFFICIENT_PRIVELEGES, NO_HEADER_DATA, NO_CONNECTION_TO_SERVICE, FAILED_TO_CONNECT_TO_CONTROLLER,\
        NO_MORE_AVAILABLE_SERVICES, NO_SERVICES_INSTALLED, SERVICE_FAILED_TO_START, CANNOT_CONNECT_TO_SERVICE, UNKNOWN_SERVICE_CONNECTION_ERROR,\
        INVALID_PATH,\
        CONTROLLER_ERRORS,\
        NO_ANALYZED_DATA,\
        UNKNOWN_ERROR,\
        LABVIEW_EXCEPTION,\
		INVALID_CONNECTION_DEVICE,\
		INVALID_SCAN_COUNT};

struct SVQM_800_Error
{
public:
	EnumErrorType m_ErrorType;
	std::wstring m_ErrorString;
	std::wstring m_ErrorTroubleShooting;

	SVQM_800_Error(EnumErrorType errorType = NO_ERRORS)
	{
		m_ErrorType = errorType;
	}

	SVQM_800_Error(EnumErrorType errorType, const std::wstring& errorString, const std::wstring& errorTroubleShooting)
	{
		m_ErrorType = errorType;
		m_ErrorString = errorString;
		m_ErrorTroubleShooting = errorTroubleShooting;
	}
};