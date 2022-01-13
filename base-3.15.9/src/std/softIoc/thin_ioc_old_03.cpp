//
// thin_ioc_old_03.cpp
//

#if 0

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

struct DbDescriptor 
{
  const char * PathToDbFile ;
  const char * Macros ;
} ;

// Cheap and cheerful flags that will be accessed by the C# thread

static volatile int _is_running     = 1 ;

static volatile int _stop_requested = 0 ;

extern "C" __declspec(dllexport) 
int thin_ioc_start_ex_old (
	const DbDescriptor * dbDescriptors,
	int                  nDbDescriptors,
	const char *         pathToCmdFile
	// const char *         pathToDbdFile
) {
	// fprintf(epicsGetStdout(),"THIN_IOC STARTING\n") ;
	// if ( pathToDbdFile == NULL )
	// {
	//   // Hmm, using 'base.dbd' here instead of 'softIoc.dbd'
	// 	// seems to work, but that is probaby not a good idea
	// 	// as we have to rely on calling 'softIoc_registerRecordDeviceDriver'
	// 	// which has been auto-generated from 'softIoc.dbd' ...
	//   // pathToDbdFile = EPICS_BASE "dbd\\base.dbd" ;
	//   pathToDbdFile = EPICS_BASE "dbd\\softIoc.dbd" ;
	// }
	// Note that if we use 'base.dbd' instead of 'softIoc.dbd'
	// it'll be necessary to specify the 'path' argument,
	// otherwise the 'includes' won't be found ...
	const char * path = (
	  NULL
	  // EPICS_BASE "dbd\\" 
  ) ;
	const char * base_dbd = DBD_FILE ;
  if ( dbLoadDatabase(base_dbd,path,NULL) != 0 ) 
	{
    return -1 ;
	}
	// Hmm, this code is generated from 'softIoc'
	// CALLING IT IS NECESSARY, OTHERWISE NOTHING WORKS
  softIoc_registerRecordDeviceDriver(pdbbase) ;
  for ( int iDbDescriptor = 0 ; iDbDescriptor < nDbDescriptors ; iDbDescriptor++ )
	{
		const DbDescriptor & dbDescriptor = dbDescriptors[iDbDescriptor] ;
		const char * dbFilePath = dbDescriptor.PathToDbFile ;
		const char * macros     = dbDescriptor.Macros ;
		if ( dbLoadRecords(dbFilePath,macros) != 0 ) 
		{
			return iDbDescriptor + 1 ;
		}
	}
	if ( iocInit() != 0 )
	{
	  return -2 ;
	}
	// epicsThreadSleep(0.2) ;
	if ( pathToCmdFile != NULL )
	{
	  if ( iocsh(pathToCmdFile) ) 
		{
		  epicsExit(EXIT_FAILURE) ;
			return -3 ;
		}
	}
	// epicsThreadSleep(0.2) ;
	return 0 ;

	// int iSecs = 1 ;
	// do
	// {
	//   epicsThreadSleep(1.0) ; 
	//   fprintf(epicsGetStdout(),"THIN_IOC Running %d\n",iSecs++) ;
	// }
	// while ( 
	//   _stop_requested == 0
  // ) ;
	// fprintf(epicsGetStdout(),"THIN_IOC STOPPING\n") ;
	// epicsExitCallAtExits() ;
	// epicsThreadSleep(0.1) ;
	// // Hmm, this KILLS THE ENTIRE PROCESS !!!
	// // epicsExit(EXIT_SUCCESS) ;
  // _is_running = 0 ;
	// fprintf(epicsGetStdout(),"THIN_IOC RETURNING\n") ;
	// return 0 ;
}

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

//
// Hmm, calling 'iocshLoad' seems to start an infinite loop
// on the current thread, which we don't return from ... ???
// 

// extern "C" __declspec(dllexport)
// int thin_ioc_execute_command_file ( const char * pathToCmdFile, const char* macros )
// {
// 	return iocshLoad(pathToCmdFile,macros) ;
// }
// 
// extern "C" __declspec(dllexport)
// int thin_ioc_execute_command ( const char * command, const char * macros )
// {
// 	return iocshRun(command,macros) ;
// }

// Hmm, this seemed to work but maybe it's not a good idea
// to try and support a 'command file' ... ???

extern "C" __declspec(dllexport)
int thin_ioc_start_ex (
	const DbDescriptor * dbDescriptors,
	int                  nDbDescriptors,
	const char *         pathToCmdFile
	// const char *         pathToDbdFile
) {
	// fprintf(epicsGetStdout(),"THIN_IOC STARTING\n") ;
	// if ( pathToDbdFile == NULL )
	// {
	//   // Hmm, using 'base.dbd' here instead of 'softIoc.dbd'
	// 	 // seems to work, but that is probaby not a good idea
	// 	 // as we have to rely on calling 'softIoc_registerRecordDeviceDriver'
	// 	 // which has been auto-generated from 'softIoc.dbd' ...
	//   // pathToDbdFile = EPICS_BASE "dbd\\base.dbd" ;
	//   pathToDbdFile = EPICS_BASE "dbd\\softIoc.dbd" ;
	// }
	// Note that if we use 'base.dbd' instead of 'softIoc.dbd'
	// it'll be necessary to specify the 'path' argument,
	// otherwise the 'includes' won't be found ...
	const char* path = (
		NULL
		// EPICS_BASE "dbd\\" 
		);
	const char* base_dbd = DBD_FILE;
	if (dbLoadDatabase(base_dbd, path, NULL) != 0)
	{
		return -1;
	}
	// Hmm, this code is generated from 'softIoc'
	// CALLING IT IS NECESSARY, OTHERWISE NOTHING WORKS
	softIoc_registerRecordDeviceDriver(pdbbase);
	for ( int iDbDescriptor = 0; iDbDescriptor < nDbDescriptors; iDbDescriptor++)
	{
		const DbDescriptor& dbDescriptor = dbDescriptors[iDbDescriptor];
		const char* dbFilePath = dbDescriptor.PathToDbFile;
		const char* macros = dbDescriptor.Macros;
		if (dbLoadRecords(dbFilePath, macros) != 0)
		{
			return iDbDescriptor + 1;
		}
	}
	if ( iocInit() != 0 )
	{
		return -2;
	}
	// epicsThreadSleep(0.2) ;
	if ( pathToCmdFile != NULL )
	{
		if ( iocsh(pathToCmdFile) )
		{
			// epicsExit(EXIT_FAILURE) ;
			return -3 ;
		}
	}
	// epicsThreadSleep(0.2) ;
	return 0;

	// int iSecs = 1 ;
	// do
	// {
	//   epicsThreadSleep(1.0) ; 
	//   fprintf(epicsGetStdout(),"THIN_IOC Running %d\n",iSecs++) ;
	// }
	// while ( 
	//   _stop_requested == 0
	// ) ;
	// fprintf(epicsGetStdout(),"THIN_IOC STOPPING\n") ;
	// epicsExitCallAtExits() ;
	// epicsThreadSleep(0.1) ;
	// // Hmm, this KILLS THE ENTIRE PROCESS !!!
	// // epicsExit(EXIT_SUCCESS) ;
	// _is_running = 0 ;
	// fprintf(epicsGetStdout(),"THIN_IOC RETURNING\n") ;
	// return 0 ;
}


// extern "C" __declspec(dllexport)
// int thin_ioc_is_running()
// {
// 	return _is_running;
// }
// 
// extern "C" __declspec(dllexport)
// void thin_ioc_set_is_running ( )
// {
// 	_is_running = 1 ;
// }
// 
// extern "C" __declspec(dllexport) 
// void thin_ioc_request_stop ( )
// {
// 	_stop_requested = 1 ;
// }

extern "C" __declspec(dllexport)
void thin_ioc_call_atExits ( )
{
	epicsExitCallAtExits() ;
	// epicsThreadSleep(0.1) ;
}

#endif
