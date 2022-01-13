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

extern "C" int softIoc_registerRecordDeviceDriver ( struct dbBase * pdbbase ) ;

#define DBD_FILE EPICS_BASE "dbd\\softIoc.dbd"

extern "C" __declspec(dllexport)
int thin_ioc_get_version ( )
{
	return 100 ; 
}

static bool dbdHasBeenLoaded = 0 ;

// Returns 0 if sucessful, otherwise an error code.

const int THIN_IOC_SUCCESS                        = 0 ;
const int THIN_IOC_FAILED_TO_LOAD_DBD_FILE        = 1 ;
const int THIN_IOC_FAILED_TO_REGISTER_DRIVER      = 2 ;
const int THIN_IOC_FAILED_TO_LOAD_DB_FILE         = 3 ;
const int THIN_IOC_DBD_NOT_LOADED                 = 4 ;
const int THIN_IOC_INIT_FAILED                    = 5 ;

extern "C" __declspec(dllexport)
int thin_ioc_load_db_file (
	const char * dbFilePath,
	const char * macros
) {
  // If this is the first time we've been called,
	// it's necessary to load the 'dbd' database that
	// defines all the 'record' types
  if ( ! dbdHasBeenLoaded )
	{
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
	  return THIN_IOC_INIT_FAILED ;
	}
	return THIN_IOC_SUCCESS ;
}

extern "C" __declspec(dllexport)
void thin_ioc_call_atExits ( )
{
	epicsExitCallAtExits() ;
	// epicsThreadSleep(0.1) ;
}

