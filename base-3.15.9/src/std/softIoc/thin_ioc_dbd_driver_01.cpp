//
// thin_ioc_dbd_driver_01.cpp
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

extern "C" int generated_registerRecordDeviceDriver ( struct dbBase * pdbbase ) ;

//
// We can configure our ThinIoc to work with a 'DBD' definition
// that is defined by (A) a .dbd file, and (B) a generated 'C' function
// that is packaged in this DLL :
//
//  int register_record_device_driver (  ) ;
//
// As well as the 'driver' dll, you need the full path
// to the corresponding '.dbd' file.
//

//
// This exported function is invoked by 'thin_ioc'
// once it has performed LoadLibrary on this DLL.
//

extern "C" __declspec(dllexport)
int register_record_device_driver ( )
{
  // No need to provide this ???
  // struct dbBase * pdbbase
  return generated_registerRecordDeviceDriver(pdbbase) ;
}

extern "C" __declspec(dllexport)
const char * query_dbd_name ( )
{
  // No need to provide this ???
  // struct dbBase * pdbbase
  return "softIoc" ;
}

// The driver DLL could contain the text of the '.dbd' definition,
// so everything you need is available in the DLL - no need for
// an external file ...

extern "C" __declspec(dllexport)
const char * query_dbd_file_text ( )
{
  return NULL ;
}

//
// Alternative scheme :
//
//   The 'driver' DLL would package not one but several dbd's,
//   and as well as the generated code for the 'driver' file,
//   it would contain the text of the '.dbd' definition ; so everything
//   you need is available in the DLL.
//
