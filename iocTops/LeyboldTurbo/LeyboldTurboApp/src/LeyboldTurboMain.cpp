//////////////////////////////////////////////////////////////////////////////////
//																				//
//	Module: LeyboldTurboMain.cpp												//
//																				//
//	Description: IOC Application startup.										//
//	Author:  Peter Heesterman (Tessella plc). Date: 05 Jan 2015.				//
//	Written for CCFE (Culham Centre for Fusion Energy).							//
//																				//
//	LeyboldTurbo is distributed subject to a Software License Agreement			//
//	found in file LICENSE that is included with this distribution.				//
//																				//
//////////////////////////////////////////////////////////////////////////////////

#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "epicsExit.h"
#include "epicsThread.h"
#include "iocsh.h"

int main(int argc,char *argv[])
{
    if(argc>=2) {    
        iocsh(argv[1]);
        epicsThreadSleep(.2);
    }
    iocsh(NULL);
    epicsExit(0);
    return(0);
}
