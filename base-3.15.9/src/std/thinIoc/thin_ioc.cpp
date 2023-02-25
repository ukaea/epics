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
#include "epicsString.h"

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
void thin_ioc_call_atExits()
{
  epicsExitCallAtExits();
  // epicsThreadSleep(0.1) ;
}

enum THINIOC_DBL_STATUS {
  THINIOC_DBL_SUCCESS = 0,
  THINIOC_DBL_NO_DB = 1,
  THINIOC_DBL_NO_RECORDS = 2,
  THINIOC_DBL_INSUFFICIENT_SPACE = 3
} ;

extern "C" __declspec(dllexport)
int thin_ioc_dbl ( char * resultBuffer, int nCharsAllocated )
{
  resultBuffer[0] = 0 ;
  if ( pdbbase == 0 )
  {
    return THINIOC_DBL_NO_DB;
  }

  DBENTRY dbentry ;
  DBENTRY* pdbentry = &dbentry ;
  dbInitEntry(pdbbase, pdbentry) ;
  long status = dbFirstRecordType(pdbentry) ;
  if ( status != 0 )
  {
    return THINIOC_DBL_NO_RECORDS ;
  }

  while ( status == 0 )
  {
    status = dbFirstRecord(pdbentry) ;
    while ( status == 0 )
    {
      if ( strlen(resultBuffer) != 0 )
      {
        // This isn't the first item, 
        // so append a ','
        errno_t errorFlag = strncat_s(
          resultBuffer,
          nCharsAllocated,
          ",",
          1
        ) ;
        if ( errorFlag != 0 )
        {
          dbFinishEntry(pdbentry) ;
          return THINIOC_DBL_INSUFFICIENT_SPACE ;
        }
      }
      const char * recordName = dbGetRecordName(pdbentry) ;
      errno_t errorFlag = strncat_s(
        resultBuffer,
        nCharsAllocated,
        recordName,
        strlen(recordName)
      ) ;
      if ( errorFlag != 0 )
      {
        dbFinishEntry(pdbentry);
        return THINIOC_DBL_INSUFFICIENT_SPACE ;
      }
      status = dbNextRecord(pdbentry) ;
    }
    status = dbNextRecordType(pdbentry) ;
  }
  dbFinishEntry(pdbentry) ;
  return THINIOC_DBL_SUCCESS ;
}

long dbl_simplified_01 ( )
{
  if ( !pdbbase )
  {
    printf("No database loaded\n") ;
    return 0 ;
  }

  DBENTRY dbentry ;
  DBENTRY* pdbentry = &dbentry ;
  dbInitEntry(pdbbase,pdbentry) ;
  long status = dbFirstRecordType(pdbentry) ;
  if ( status )
  {
    printf("No record type\n");
  }

  while ( !status )
  {
    status = dbFirstRecord(pdbentry) ;
    while ( !status )
    {
      printf("%s",dbGetRecordName(pdbentry));
      status = dbNextRecord(pdbentry);
    }
    status = dbNextRecordType(pdbentry) ;
  }
  dbFinishEntry(pdbentry) ;
  return 0 ;
}

////////////////////////////////////////////////



//
// Cloned from 'dbTest.c'
//
// Here's how the 'dbl' command is installed in 'dblocRegister.c :
// 
//   static const iocshArg dblArg0 = { "record type",iocshArgString };
//   static const iocshArg dblArg1 = { "fields",iocshArgString };
//   static const iocshArg* const dblArgs[] = { &dblArg0,&dblArg1 };
//   static const iocshFuncDef dblFuncDef = { 
//     "dbl",2,dblArgs,
//     "Database list.\n"
//     "List record/field names.\n"
//     "With no arguments, lists all record names.\n"
//   };
//   static void dblCallFunc(const iocshArgBuf* args)
//   {
//     dbl(args[0].sval, args[1].sval);
//   }
// 

long dbl_original_01 ( const char* precordTypename, const char* fields )
{
  DBENTRY dbentry ;
  DBENTRY* pdbentry = &dbentry ;
  long status ;
  int nfields = 0 ;
  int ifield ;
  char* fieldnames = 0 ;
  char** papfields = 0 ;

  if ( !pdbbase ) 
  {
    printf("No database loaded\n") ;
    return 0 ;
  }

  if (
     precordTypename 
  && (
        (*precordTypename == '\0') 
     || !strcmp(precordTypename,"*")
     )
  ) {
    precordTypename = NULL ;
  }

  if ( 
     fields 
  && (*fields == '\0') 
  ) {
    fields = NULL ;
  }

  if ( fields ) 
  {
    char* pnext;
    fieldnames = epicsStrDup(fields);
    nfields = 1;
    pnext = fieldnames;
    while ( *pnext && (pnext = strchr(pnext, ' ')) ) 
    {
      nfields++;
      while (*pnext == ' ') 
        pnext++;
    }
    papfields = (char**) dbCalloc(nfields, sizeof(char*)); // ???????????????????
    pnext = fieldnames;
    for ( ifield = 0; ifield < nfields; ifield++) 
    {
      papfields[ifield] = pnext;
      if ( ifield < nfields - 1 ) 
      {
        pnext = strchr(pnext, ' ');
        *pnext++ = 0;
        while (*pnext == ' ') pnext++;
      }
    }
  }

  dbInitEntry(pdbbase,pdbentry) ;
  if ( !precordTypename )
    status = dbFirstRecordType(pdbentry);
  else
    status = dbFindRecordType(pdbentry, precordTypename);
  if ( status ) 
  {
    printf("No record type\n");
  }

  while ( !status ) 
  {
    status = dbFirstRecord(pdbentry) ;
    while (!status) 
    {
      printf("%s", dbGetRecordName(pdbentry)) ;
      for ( ifield = 0 ; ifield < nfields ; ifield++ ) 
      {
        char* pvalue ;
        status = dbFindField(pdbentry,papfields[ifield]) ;
        if  (status ) 
        {
          if ( !strcmp(papfields[ifield], "recordType") ) 
          {
            pvalue = dbGetRecordTypeName(pdbentry) ;
          }
          else 
          {
            printf(", ") ;
            continue ;
          }
        }
        else 
        {
          pvalue = dbGetString(pdbentry) ;
        }
        printf(", \"%s\"", pvalue ? pvalue : "") ;
      }
      printf("\n") ;
      status = dbNextRecord(pdbentry) ;
    }
    if ( precordTypename )
      break ;

    status = dbNextRecordType(pdbentry) ;
  }

  if ( nfields > 0 ) 
  {
    free((void*)papfields) ;
    free((void*)fieldnames) ;
  }
  dbFinishEntry(pdbentry) ;
  return 0 ;

}
