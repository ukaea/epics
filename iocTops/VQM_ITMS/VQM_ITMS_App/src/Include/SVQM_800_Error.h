#pragma once

#include "IServiceWrapper.h"

/*
Maps to the VQM_800_ERROR class in the CSServiceWrapper DLL.
*/
struct SVQM_800_Error
{
public:
	EnumErrorType m_ErrorType;
	STRING_TYPE m_ErrorString;
	STRING_TYPE m_ErrorTroubleShooting;

	SVQM_800_Error()
	{
		m_ErrorType = UNKNOWN_ERROR;
	}

	SVQM_800_Error(EnumErrorType errorType)
	{
		m_ErrorType = errorType;
	}

	SVQM_800_Error(EnumErrorType errorType, const STRING_TYPE& errorString, const STRING_TYPE& errorTroubleShooting)
	{
		m_ErrorType = errorType;
		m_ErrorString = errorString;
		m_ErrorTroubleShooting = errorTroubleShooting;
	}
};