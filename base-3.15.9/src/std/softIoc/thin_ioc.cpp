//
// thin_ioc.cpp
//

#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "registryFunction.h"
#include "epicsThread.h"
#include "epicsExit.h"
#include "epicsStdio.h"
#include "dbStaticLib.h"
#include "subRecord.h"
#include "dbAccess.h"
#include "asDbLib.h"
#include "iocInit.h"
#include "iocsh.h"
#include "epicsInstallDir.h"

extern "C" int softIoc_registerRecordDeviceDriver(struct dbBase *pdbbase) ;

#define DBD_FILE EPICS_BASE  "dbd\\softIoc.dbd"

// #define EXIT_FILE EPICS_BASE "db\\softIocExit.db"

// const char *arg0;
const char *base_dbd = DBD_FILE;

// const char *exit_db = EXIT_FILE;

extern "C" __declspec(dllexport) 
int thin_ioc_start_xx ( short nSecs )
{
  base_dbd = EPICS_BASE "dbd\\base.dbd" ;
	const char * path = (
	  EPICS_BASE "dbd\\"
	  // NULL 
  ) ;
  if ( dbLoadDatabase(base_dbd,path,NULL) ) 
	{
    return -1 ;
	}
  softIoc_registerRecordDeviceDriver(pdbbase);
	char *macros = NULL ;
	if ( dbLoadRecords("C:\\tmp\\xx.db", macros)) 
	{
		return -2 ;
	}
	iocInit() ;
	epicsThreadSleep(0.2) ;
	for ( int i = 0 ; i < nSecs ; i++ )
	{
	  epicsThreadSleep(1.0) ; 
		printf("Sleeping %d\n",i) ;
	}
	epicsExit(EXIT_SUCCESS) ;
  // iocsh(NULL) ;
	return 0 ;
}

extern "C" __declspec(dllexport) 
int thin_ioc_div ( int a, int b )
{
  // return thin_ioc_start_xx() ;
  return a / b ;
}

struct DbDescriptor 
{
  char * PathToDbFile ;
  char * Macros ;
} ;

extern "C" __declspec(dllexport) 
int thin_ioc_start(
	char *         pathToDbdFile,
	DbDescriptor * dbDescriptors,
	int            nDbDescriptors,
	char *         pathToCmdFile
) {
  for ( int iDbDescriptor = 0 ; iDbDescriptor < nDbDescriptors ; iDbDescriptor++ )
	{
	}
	return 123 ;
}



#if 0

registryFunctionAdd("exit", (REGISTRYFUNCTION) exitSubroutine);

static void exitSubroutine(subRecord *precord) {
    epicsExitLater((precord->a == 0.0) ? EXIT_SUCCESS : EXIT_FAILURE);
}

static void usage(int status) {
    printf("Usage: %s [-D softIoc.dbd] [-h] [-S] [-a ascf]\n", arg0);
    puts("\t[-m macro=value,macro2=value2] [-d file.db]");
    puts("\t[-x prefix] [st.cmd]");
    puts("Compiled-in path to softIoc.dbd is:");
    printf("\t%s\n", base_dbd);
    epicsExit(status);
}

int main(int argc, char *argv[])
{
    char *dbd_file = const_cast<char*>(base_dbd);
    char *macros = NULL;
    char xmacro[PVNAME_STRINGSZ + 4];
    int startIocsh = 1;	/* default = start shell */
    int loadedDb = 0;
    
    arg0 = strrchr(*argv, '/');
    if (!arg0) {
	arg0 = *argv;
    } else {
	++arg0;	/* skip the '/' */
    }
    
    --argc, ++argv;
    
    /* Do this here in case the dbd file not available */
    if (argc>0 && **argv=='-' && (*argv)[1]=='h') {
	usage(EXIT_SUCCESS);
    }
    
    if (argc>1 && **argv=='-' && (*argv)[1]=='D') {
	dbd_file = *++argv;
	argc -= 2;
	++argv;
    }
    
    if (dbLoadDatabase(dbd_file, NULL, NULL)) {
	epicsExit(EXIT_FAILURE);
    }
    
    softIoc_registerRecordDeviceDriver(pdbbase);
    registryFunctionAdd("exit", (REGISTRYFUNCTION) exitSubroutine);

    while (argc>1 && **argv == '-') {
	switch ((*argv)[1]) {
	case 'a':
	    if (macros) asSetSubstitutions(macros);
	    asSetFilename(*++argv);
	    --argc;
	    break;
	
	case 'd':
	    if (dbLoadRecords(*++argv, macros)) {
		epicsExit(EXIT_FAILURE);
	    }
	    loadedDb = 1;
	    --argc;
	    break;
	
	case 'h':
	    usage(EXIT_SUCCESS);
	
	case 'm':
	    macros = *++argv;
	    --argc;
	    break;
	
	case 'S':
	    startIocsh = 0;
	    break;
	
	case 's':
	    break;
	
	case 'x':
	    epicsSnprintf(xmacro, sizeof xmacro, "IOC=%s", *++argv);
	    if (dbLoadRecords(exit_db, xmacro)) {
		epicsExit(EXIT_FAILURE);
	    }
	    loadedDb = 1;
	    --argc;
	    break;
	
	default:
	    printf("%s: option '%s' not recognized\n", arg0, *argv);
	    usage(EXIT_FAILURE);
	}
	--argc;
	++argv;
    }
    
    if (argc>0 && **argv=='-') {
	switch((*argv)[1]) {
	case 'a':
	case 'd':
	case 'm':
	case 'x':
	    printf("%s: missing argument to option '%s'\n", arg0, *argv);
	    usage(EXIT_FAILURE);
	
	case 'h':
	    usage(EXIT_SUCCESS);
	
	case 'S':
	    startIocsh = 0;
	    break;
	
	case 's':
	    break;
	
	default:
	    printf("%s: option '%s' not recognized\n", arg0, *argv);
	    usage(EXIT_FAILURE);
	}
	--argc;
	++argv;
    }
    
    if (loadedDb) {
	iocInit();
	epicsThreadSleep(0.2);
    }
    
    /* run user's startup script */
    if (argc>0) {
	if (iocsh(*argv)) epicsExit(EXIT_FAILURE);
	epicsThreadSleep(0.2);
	loadedDb = 1;	/* Give it the benefit of the doubt... */
    }
    
    /* start an interactive shell if it was requested */
    if (startIocsh) {
	iocsh(NULL);
    } else {
	if (loadedDb) {
	    epicsThreadExitMain();
	} else {
	    printf("%s: Nothing to do!\n", arg0);
	    usage(EXIT_FAILURE);
	}
    }
    epicsExit(EXIT_SUCCESS);
    /*Note that the following statement will never be executed*/
    return 0;
}

#endif
