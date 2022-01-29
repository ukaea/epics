//
// thin_ioc.cpp
//

#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "epicsThread.h"
#include "epicsExit.h"
#include "epicsStdio.h"
#include "dbStaticLib.h"
#include "dbAccess.h"
#include "iocInit.h"
#include "iocsh.h"
#include "epicsInstallDir.h"
#include "envDefs.h"

extern "C" int softIoc_registerRecordDeviceDriver ( struct dbBase * pdbbase ) ;

#define DBD_FILE EPICS_BASE "dbd\\softIoc.dbd"

// This function is useful when we want to verify that the P/Invoke Wrapper
// is expecting the version of the DLL that gets loaded via 'LoadLibrary'.

extern "C" __declspec(dllexport)
int thin_ioc_get_version ( )
{
	return 102 ; 
}

static bool dbdHasBeenLoaded = 0 ;

// Returns 0 if sucessful, otherwise an error code.

const int THIN_IOC_SUCCESS                        = 0 ;
const int THIN_ALREADY_INITIALISED                = 1 ;
const int THIN_IOC_FAILED_TO_LOAD_DBD_FILE        = 2 ;
const int THIN_IOC_FAILED_TO_REGISTER_DRIVER      = 3 ;
const int THIN_IOC_NOT_INITIALISED                = 4 ;
const int THIN_IOC_FAILED_TO_LOAD_DB_FILE         = 5 ;
const int THIN_IOC_DBD_NOT_LOADED                 = 6 ;
const int THIN_IOC_START_FAILED                   = 7 ;

extern "C" __declspec(dllexport)
void thin_ioc_set_env ( 
  const char * name,
	const char * value
) {
  epicsEnvSet(name,value) ;
}

extern "C" __declspec(dllexport)
const char * thin_ioc_get_env ( 
  const char * name
) {
  return getenv(name) ;
}

extern "C" __declspec(dllexport)
int thin_ioc_initialise ( ) 
{
  if ( dbdHasBeenLoaded )
	{
	  return THIN_ALREADY_INITIALISED ;
	}
	// Load the 'dbd' database that defines 
	// the supported 'record' types
	const char * base_dbd = DBD_FILE ;
	if ( dbLoadDatabase(base_dbd,NULL,NULL) != 0 )
	{
		return THIN_IOC_FAILED_TO_LOAD_DBD_FILE ;
	}
	// The code for this function has been generated from 'softIoc'
	// CALLING IT IS NECESSARY, OTHERWISE NOTHING WORKS
	if ( softIoc_registerRecordDeviceDriver(pdbbase) != 0 )
	{
		return THIN_IOC_FAILED_TO_REGISTER_DRIVER ;
	}
  dbdHasBeenLoaded = 1 ;
	return THIN_IOC_SUCCESS ;
}

extern "C" __declspec(dllexport)
int thin_ioc_load_db_file (
	const char * dbFilePath,
	const char * macros
) {
  if ( ! dbdHasBeenLoaded )
	{
	  int init_result = thin_ioc_initialise() ;
		if ( init_result != THIN_IOC_SUCCESS )
		{
		  return init_result ;
		}
	}
  if ( dbLoadRecords(dbFilePath,macros) != 0 )
	{
	  return THIN_IOC_FAILED_TO_LOAD_DB_FILE ;
	}
	return THIN_IOC_SUCCESS ;
}

extern "C" __declspec(dllexport)
int thin_ioc_start ( ) 
{
  if ( ! dbdHasBeenLoaded )
	{
	  return THIN_IOC_DBD_NOT_LOADED ;
	}
	if ( iocInit() != 0 )
	{
	  return THIN_IOC_START_FAILED ;
	}
	return THIN_IOC_SUCCESS ;
}

extern "C" __declspec(dllexport)
void thin_ioc_call_atExits ( )
{
	epicsExitCallAtExits() ;
	// epicsThreadSleep(0.1) ;
}

