/* DAQmxBaseMain.cpp */
/* Author:  Marty Kraimer Date:    17MAR2000 */

#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <direct.h>

#include "epicsExit.h"
#include "epicsThread.h"
#include "iocsh.h"

#include <dbAccessDefs.h>

extern "C" int DAQmxBase_registerRecordDeviceDriver(struct dbBase *pdbbase);

int main(int argc,char *argv[])
{
    if (dbLoadDatabase("..\\..\\dbd\\DAQmxBase.dbd", NULL, NULL)) {
		epicsExit(EXIT_FAILURE);
    }

	DAQmxBase_registerRecordDeviceDriver(pdbbase);
    if(argc>=2) {    
        iocsh(argv[1]);
        epicsThreadSleep(.2);
    }
    iocsh(NULL);
    epicsExit(0);
    return(0);
}
