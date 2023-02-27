//
// thin_ioc.cpp
//

#if 0

#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

//#include "registryFunction.h"
#include "epicsThread.h"
#include "epicsExit.h"
#include "epicsStdio.h"
#include "dbStaticLib.h"
//#include "subRecord.h"
#include "dbAccess.h"
//#include "asDbLib.h"
#include "iocInit.h"
#include "iocsh.h"
#include "epicsInstallDir.h"

extern "C" int softIoc_registerRecordDeviceDriver(struct dbBase *pdbbase) ;

#define DBD_FILE EPICS_BASE "dbd\\softIoc.dbd"

struct DbDescriptor 
{
  char * PathToDbFile ;
  char * Macros ;
} ;

// Cheap and cheerful flags that will be accessed by the C# thread

static volatile int _is_running     = 1 ;

static volatile int _stop_requested = 0 ;

extern "C" __declspec(dllexport) 
int thin_ioc_start(
	DbDescriptor * dbDescriptors,
	int            nDbDescriptors,
	char *         pathToCmdFile,
	char *         pathToDbdFile
) {
	fprintf(epicsGetStdout(),"THIN_IOC STARTING\n") ;
	if ( pathToDbdFile == NULL )
	{
	  // Hmm, using 'base.dbd' instead of 'softIoc.dbd'
		// seems to work, but maybe not a good idea as
		// we have to rely on calling 'softIoc_registerRecordDeviceDriver'
	  // pathToDbdFile = EPICS_BASE "dbd\\base.dbd" ;
	  pathToDbdFile = EPICS_BASE "dbd\\softIoc.dbd" ;
	}
	// Note that if we use 'base.dbd' instead of 'softIoc.dbd'
	// it's necessary to specify the 'path' argument, otherwise
	// the 'includes' are not found ...
	const char * path = EPICS_BASE "dbd\\" ;
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
	epicsThreadSleep(0.2) ;
	if ( pathToCmdFile != NULL )
	{
	  if ( iocsh(pathToCmdFile) ) 
		{
		  epicsExit(EXIT_FAILURE) ;
			return -3 ;
		}
	}
	epicsThreadSleep(0.2) ;

	int iSecs = 1 ;
	do
	{
	  epicsThreadSleep(1.0) ; 
	  fprintf(epicsGetStdout(),"THIN_IOC Running %d\n",iSecs++) ;
	}
	while ( 
	  _stop_requested == 0
  ) ;
	fprintf(epicsGetStdout(),"THIN_IOC STOPPING\n") ;
	epicsExitCallAtExits() ;
	epicsThreadSleep(0.1) ;
	// Hmm, this KILLS THE ENTIRE PROCESS !!!
	// epicsExit(EXIT_SUCCESS) ;
  _is_running = 0 ;
	fprintf(epicsGetStdout(),"THIN_IOC RETURNING\n") ;
	return 0 ;
}

extern "C" __declspec(dllexport)
void thin_ioc_call_atExits ( )
{
	epicsExitCallAtExits() ;
	// epicsThreadSleep(0.1) ;
}

extern "C" __declspec(dllexport)
int thin_ioc_is_running()
{
	return _is_running;
}

extern "C" __declspec(dllexport)
void thin_ioc_set_is_running ( )
{
	_is_running = 1 ;
}

extern "C" __declspec(dllexport) 
void thin_ioc_request_stop ( )
{
	_stop_requested = 1 ;
}

#endif
