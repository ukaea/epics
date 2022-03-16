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

#include <windows.h> // Required for 'LoadLibrary'

extern "C" int thinIoc_registerRecordDeviceDriver ( struct dbBase * pdbbase ) ;

#define DBD_FILE EPICS_BASE "dbd\\thinBaseIOC.dbd"

// This function is useful when we want to verify that the P/Invoke Wrapper
// is expecting the version of the DLL that gets loaded via 'LoadLibrary'.

extern "C" __declspec(dllexport)
int thin_ioc_get_version ( )
{
	return 103 ; 
}

static bool dbdHasBeenLoaded = 0 ;

// Returns 0 if sucessful, otherwise an error code.

// const int THIN_IOC_SUCCESS                        = 0 ;
// const int THIN_IOC_ALREADY_INITIALISED            = 1 ;
// const int THIN_IOC_FAILED_TO_LOAD_DBD_FILE        = 2 ;
// const int THIN_IOC_FAILED_TO_REGISTER_DRIVER      = 3 ;
// const int THIN_IOC_NOT_INITIALISED                = 4 ;
// const int THIN_IOC_FAILED_TO_LOAD_DB_FILE         = 5 ;
// const int THIN_IOC_DBD_NOT_LOADED                 = 6 ;
// const int THIN_IOC_START_FAILED                   = 7 ;
// const int THIN_IOC_DBD_OPTION_NOT_SUPPORTED       = 8 ;

// Better to declare these as enums, so that
// the values can just be copied-and-pasted 
// into the equivalent C# definition

enum THIN_IOC_STATUS : int {
	THIN_IOC_SUCCESS                        = 0,
	THIN_IOC_ALREADY_INITIALISED            = 1,
	THIN_IOC_FAILED_TO_LOAD_DBD_FILE        = 2,
	THIN_IOC_FAILED_TO_REGISTER_DRIVER      = 3,
	THIN_IOC_NOT_INITIALISED                = 4,
	THIN_IOC_FAILED_TO_LOAD_DB_FILE         = 5,
	THIN_IOC_DBD_NOT_LOADED                 = 6,
	THIN_IOC_START_FAILED                   = 7,
	THIN_IOC_DBD_OPTION_NOT_SUPPORTED       = 8
} ;

// DBD file options

const int THIN_IOC_DBD_OPTION_SoftIoc = 0 ;

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
int thin_ioc_how_many_dbd_options ( ) 
{
  return 1 ;
}

extern "C" __declspec(dllexport)
const char * thin_ioc_get_dbd_option_name ( int dbdOption ) 
{
  switch ( dbdOption )
	{
	case THIN_IOC_DBD_OPTION_SoftIoc:
	  return "SoftIoc" ;
	default:
	  return NULL ;
	}
}

//
// We can configure our ThinIoc to work with a 'DBD' definition
// that is defined by (A) a .dbd file, and (B) a generated 'C' function
// that is packaged as a DLL, with an entry point defined as
//
//  int register_record_device_driver ( struct dbBase * pdbbase ) ;
//
// As well as the 'driver' dll, you need the full path
// to the corresponding '.dbd' file.
//

extern "C" __declspec(dllexport)
int thin_ioc_initialise_with_custom_dbd ( 
  const char * dbdFile,
	const char * driverRegistrationDll
) {
  if ( dbdHasBeenLoaded )
	{
	  return THIN_IOC_ALREADY_INITIALISED ;
	}
	// Load the 'dbd' database that defines 
	// the supported 'record' types
	if ( dbLoadDatabase(dbdFile,NULL,NULL) != 0 )
	{
		return THIN_IOC_FAILED_TO_LOAD_DBD_FILE ;
	}
  // Load the DLL that hosts our custom registraion function
  HMODULE hDLL = LoadLibrary(driverRegistrationDll) ;
	if ( hDLL == NULL )
	{
	  return -1 ;
	}
	// Look up the 'register_record_device_driver' in the DLL
	// https://docs.microsoft.com/en-gb/windows/win32/dlls/using-run-time-dynamic-linking?redirectedfrom=MSDN
	typedef int ( __cdecl * DriverRegistrationFunc) ( struct dbBase * pdbbase ) ;
  DriverRegistrationFunc driverRegistrationFunc = (DriverRegistrationFunc) (
	  GetProcAddress(
		  hDLL,
			"register_record_device_driver"
		)
	) ;
	if ( driverRegistrationFunc == NULL )
	{
	  return -1 ;
	}
	// Invoke the registration function
	int driverRegistrationResult = driverRegistrationFunc(pdbbase) ;
	FreeLibrary(hDLL) ;
	if ( driverRegistrationResult != 0 )
	{
		return THIN_IOC_FAILED_TO_REGISTER_DRIVER ;
	}
	else
	{
		dbdHasBeenLoaded = 1 ;
		return THIN_IOC_SUCCESS ;
	}
}

//
// Alternative scheme :
//
//   The 'driver' DLL would package not one but several dbd's,
//   and as well as the generated code for the 'driver' file,
//   it would contain the text of the '.dbd' definition ; so everything
//   you need is available in the DLL.
//

extern "C" __declspec(dllexport)
int thin_ioc_initialise ( int dbdOption ) 
{
  // At present we only support a single 'dbd' option ('softIoc')
	// but the 'dbdOption' argument gives us the possibility of
	// loading different definitions eg accommodating motor records
	// or whatever.
	if ( dbdOption != THIN_IOC_DBD_OPTION_SoftIoc )
	{
	  return THIN_IOC_DBD_OPTION_NOT_SUPPORTED ;
	}
  if ( dbdHasBeenLoaded )
	{
	  return THIN_IOC_ALREADY_INITIALISED ;
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
	if ( thinIoc_registerRecordDeviceDriver(pdbbase) != 0 )
	{
		return THIN_IOC_FAILED_TO_REGISTER_DRIVER ;
	}
  dbdHasBeenLoaded = 1 ;
	return THIN_IOC_SUCCESS ;
}

extern "C" __declspec(dllexport)
int thin_ioc_initialise_with_dbd_path ( const char * dbdFile ) 
{
  // At present we only support a single 'dbd' option ('softIoc')
	// but the 'dbdOption' argument gives us the possibility of
	// loading different definitions eg accommodating motor records
	// or whatever.
  if ( dbdHasBeenLoaded )
	{
	  return THIN_IOC_ALREADY_INITIALISED ;
	}
	// Load the 'dbd' database that defines 
	// the supported 'record' types
	const char * base_dbd = (
	  dbdFile == NULL
		? DBD_FILE 
		: dbdFile
	) ;
	if ( dbLoadDatabase(base_dbd,NULL,NULL) != 0 )
	{
		return THIN_IOC_FAILED_TO_LOAD_DBD_FILE ;
	}
	// The code for this function has been generated from 'thinIoc'
	// CALLING IT IS NECESSARY, OTHERWISE NOTHING WORKS
	if ( thinIoc_registerRecordDeviceDriver(pdbbase) != 0 )
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
    return THIN_IOC_NOT_INITIALISED ;
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
    return THIN_IOC_NOT_INITIALISED ;
	}
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

