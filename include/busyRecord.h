/* busyRecord.h generated from busyRecord.dbd */

#ifndef INC_busyRecord_H
#define INC_busyRecord_H

#include "epicsTypes.h"
#include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "epicsTime.h"

typedef struct busyRecord {
    char                name[61];   /* Record Name */
    char                desc[41];   /* Descriptor */
    char                asg[29];    /* Access Security Group */
    epicsEnum16         scan;       /* Scan Mechanism */
    epicsEnum16         pini;       /* Process at iocInit */
    epicsInt16          phas;       /* Scan Phase */
    char                evnt[40];   /* Event Name */
    epicsInt16          tse;        /* Time Stamp Event */
    DBLINK              tsel;       /* Time Stamp Link */
    epicsEnum16         dtyp;       /* Device Type */
    epicsInt16          disv;       /* Disable Value */
    epicsInt16          disa;       /* Disable */
    DBLINK              sdis;       /* Scanning Disable */
    epicsMutexId        mlok;       /* Monitor lock */
    ELLLIST             mlis;       /* Monitor List */
    ELLLIST             bklnk;      /* Backwards link tracking */
    epicsUInt8          disp;       /* Disable putField */
    epicsUInt8          proc;       /* Force Processing */
    epicsEnum16         stat;       /* Alarm Status */
    epicsEnum16         sevr;       /* Alarm Severity */
    epicsEnum16         nsta;       /* New Alarm Status */
    epicsEnum16         nsev;       /* New Alarm Severity */
    epicsEnum16         acks;       /* Alarm Ack Severity */
    epicsEnum16         ackt;       /* Alarm Ack Transient */
    epicsEnum16         diss;       /* Disable Alarm Sevrty */
    epicsUInt8          lcnt;       /* Lock Count */
    epicsUInt8          pact;       /* Record active */
    epicsUInt8          putf;       /* dbPutField process */
    epicsUInt8          rpro;       /* Reprocess  */
    struct asgMember    *asp;       /* Access Security Pvt */
    struct processNotify *ppn;      /* pprocessNotify */
    struct processNotifyRecord *ppnr; /* pprocessNotifyRecord */
    struct scan_element *spvt;      /* Scan Private */
    struct typed_rset   *rset;      /* Address of RSET */
    struct dset         *dset;      /* DSET address */
    void                *dpvt;      /* Device Private */
    struct dbRecordType *rdes;      /* Address of dbRecordType */
    struct lockRecord   *lset;      /* Lock Set */
    epicsEnum16         prio;       /* Scheduling Priority */
    epicsUInt8          tpro;       /* Trace Processing */
    char                bkpt;       /* Break Point */
    epicsUInt8          udf;        /* Undefined */
    epicsEnum16         udfs;       /* Undefined Alarm Sevrty */
    epicsTimeStamp      time;       /* Time */
    DBLINK              flnk;       /* Forward Process Link */
    epicsEnum16         val;        /* Current Value */
    epicsEnum16         omsl;       /* Output Mode Select */
    DBLINK              dol;        /* Desired Output Loc */
    DBLINK              out;        /* Output Specification */
    epicsFloat64        high;       /* Seconds to Hold High */
    char                znam[26];   /* Zero Name */
    char                onam[26];   /* One Name */
    epicsUInt32         rval;       /* Raw Value */
    epicsUInt32         oraw;       /* prev Raw Value */
    epicsUInt32         oval;       /* prev Value */
    epicsUInt32         mask;       /* Hardware Mask */
    void *  rpvt;                   /* Record Private */
    void *	wdpt;                    /* Watch Dog Timer ID */
    epicsEnum16         zsv;        /* Zero Error Severity */
    epicsEnum16         osv;        /* One Error Severity */
    epicsEnum16         cosv;       /* Change of State Sevr */
    epicsUInt32         rbv;        /* Readback Value */
    epicsUInt32         orbv;       /* Prev Readback Value */
    epicsUInt16         mlst;       /* Last Value Monitored */
    epicsUInt16         lalm;       /* Last Value Alarmed */
    DBLINK              siol;       /* Sim Output Specifctn */
    DBLINK              siml;       /* Sim Mode Location */
    epicsEnum16         simm;       /* Simulation Mode */
    epicsEnum16         sims;       /* Sim mode Alarm Svrty */
    epicsEnum16         ivoa;       /* INVALID outpt action */
    epicsUInt16         ivov;       /* INVALID output value */
} busyRecord;

typedef enum {
	busyRecordNAME = 0,
	busyRecordDESC = 1,
	busyRecordASG = 2,
	busyRecordSCAN = 3,
	busyRecordPINI = 4,
	busyRecordPHAS = 5,
	busyRecordEVNT = 6,
	busyRecordTSE = 7,
	busyRecordTSEL = 8,
	busyRecordDTYP = 9,
	busyRecordDISV = 10,
	busyRecordDISA = 11,
	busyRecordSDIS = 12,
	busyRecordMLOK = 13,
	busyRecordMLIS = 14,
	busyRecordBKLNK = 15,
	busyRecordDISP = 16,
	busyRecordPROC = 17,
	busyRecordSTAT = 18,
	busyRecordSEVR = 19,
	busyRecordNSTA = 20,
	busyRecordNSEV = 21,
	busyRecordACKS = 22,
	busyRecordACKT = 23,
	busyRecordDISS = 24,
	busyRecordLCNT = 25,
	busyRecordPACT = 26,
	busyRecordPUTF = 27,
	busyRecordRPRO = 28,
	busyRecordASP = 29,
	busyRecordPPN = 30,
	busyRecordPPNR = 31,
	busyRecordSPVT = 32,
	busyRecordRSET = 33,
	busyRecordDSET = 34,
	busyRecordDPVT = 35,
	busyRecordRDES = 36,
	busyRecordLSET = 37,
	busyRecordPRIO = 38,
	busyRecordTPRO = 39,
	busyRecordBKPT = 40,
	busyRecordUDF = 41,
	busyRecordUDFS = 42,
	busyRecordTIME = 43,
	busyRecordFLNK = 44,
	busyRecordVAL = 45,
	busyRecordOMSL = 46,
	busyRecordDOL = 47,
	busyRecordOUT = 48,
	busyRecordHIGH = 49,
	busyRecordZNAM = 50,
	busyRecordONAM = 51,
	busyRecordRVAL = 52,
	busyRecordORAW = 53,
	busyRecordOVAL = 54,
	busyRecordMASK = 55,
	busyRecordRPVT = 56,
	busyRecordWDPT = 57,
	busyRecordZSV = 58,
	busyRecordOSV = 59,
	busyRecordCOSV = 60,
	busyRecordRBV = 61,
	busyRecordORBV = 62,
	busyRecordMLST = 63,
	busyRecordLALM = 64,
	busyRecordSIOL = 65,
	busyRecordSIML = 66,
	busyRecordSIMM = 67,
	busyRecordSIMS = 68,
	busyRecordIVOA = 69,
	busyRecordIVOV = 70
} busyFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsAssert.h>
#include <epicsExport.h>
#ifdef __cplusplus
extern "C" {
#endif
static int busyRecordSizeOffset(dbRecordType *prt)
{
    busyRecord *prec = 0;

    assert(prt->no_fields == 71);
    prt->papFldDes[busyRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[busyRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[busyRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[busyRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[busyRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[busyRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[busyRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[busyRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[busyRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[busyRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[busyRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[busyRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[busyRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[busyRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[busyRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[busyRecordBKLNK]->size = sizeof(prec->bklnk);
    prt->papFldDes[busyRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[busyRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[busyRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[busyRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[busyRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[busyRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[busyRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[busyRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[busyRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[busyRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[busyRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[busyRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[busyRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[busyRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[busyRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[busyRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[busyRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[busyRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[busyRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[busyRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[busyRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[busyRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[busyRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[busyRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[busyRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[busyRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[busyRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[busyRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[busyRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[busyRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[busyRecordOMSL]->size = sizeof(prec->omsl);
    prt->papFldDes[busyRecordDOL]->size = sizeof(prec->dol);
    prt->papFldDes[busyRecordOUT]->size = sizeof(prec->out);
    prt->papFldDes[busyRecordHIGH]->size = sizeof(prec->high);
    prt->papFldDes[busyRecordZNAM]->size = sizeof(prec->znam);
    prt->papFldDes[busyRecordONAM]->size = sizeof(prec->onam);
    prt->papFldDes[busyRecordRVAL]->size = sizeof(prec->rval);
    prt->papFldDes[busyRecordORAW]->size = sizeof(prec->oraw);
    prt->papFldDes[busyRecordOVAL]->size = sizeof(prec->oval);
    prt->papFldDes[busyRecordMASK]->size = sizeof(prec->mask);
    prt->papFldDes[busyRecordRPVT]->size = sizeof(prec->rpvt);
    prt->papFldDes[busyRecordWDPT]->size = sizeof(prec->wdpt);
    prt->papFldDes[busyRecordZSV]->size = sizeof(prec->zsv);
    prt->papFldDes[busyRecordOSV]->size = sizeof(prec->osv);
    prt->papFldDes[busyRecordCOSV]->size = sizeof(prec->cosv);
    prt->papFldDes[busyRecordRBV]->size = sizeof(prec->rbv);
    prt->papFldDes[busyRecordORBV]->size = sizeof(prec->orbv);
    prt->papFldDes[busyRecordMLST]->size = sizeof(prec->mlst);
    prt->papFldDes[busyRecordLALM]->size = sizeof(prec->lalm);
    prt->papFldDes[busyRecordSIOL]->size = sizeof(prec->siol);
    prt->papFldDes[busyRecordSIML]->size = sizeof(prec->siml);
    prt->papFldDes[busyRecordSIMM]->size = sizeof(prec->simm);
    prt->papFldDes[busyRecordSIMS]->size = sizeof(prec->sims);
    prt->papFldDes[busyRecordIVOA]->size = sizeof(prec->ivoa);
    prt->papFldDes[busyRecordIVOV]->size = sizeof(prec->ivov);
    prt->papFldDes[busyRecordNAME]->offset = (unsigned short)((char *)&prec->name - (char *)prec);
    prt->papFldDes[busyRecordDESC]->offset = (unsigned short)((char *)&prec->desc - (char *)prec);
    prt->papFldDes[busyRecordASG]->offset = (unsigned short)((char *)&prec->asg - (char *)prec);
    prt->papFldDes[busyRecordSCAN]->offset = (unsigned short)((char *)&prec->scan - (char *)prec);
    prt->papFldDes[busyRecordPINI]->offset = (unsigned short)((char *)&prec->pini - (char *)prec);
    prt->papFldDes[busyRecordPHAS]->offset = (unsigned short)((char *)&prec->phas - (char *)prec);
    prt->papFldDes[busyRecordEVNT]->offset = (unsigned short)((char *)&prec->evnt - (char *)prec);
    prt->papFldDes[busyRecordTSE]->offset = (unsigned short)((char *)&prec->tse - (char *)prec);
    prt->papFldDes[busyRecordTSEL]->offset = (unsigned short)((char *)&prec->tsel - (char *)prec);
    prt->papFldDes[busyRecordDTYP]->offset = (unsigned short)((char *)&prec->dtyp - (char *)prec);
    prt->papFldDes[busyRecordDISV]->offset = (unsigned short)((char *)&prec->disv - (char *)prec);
    prt->papFldDes[busyRecordDISA]->offset = (unsigned short)((char *)&prec->disa - (char *)prec);
    prt->papFldDes[busyRecordSDIS]->offset = (unsigned short)((char *)&prec->sdis - (char *)prec);
    prt->papFldDes[busyRecordMLOK]->offset = (unsigned short)((char *)&prec->mlok - (char *)prec);
    prt->papFldDes[busyRecordMLIS]->offset = (unsigned short)((char *)&prec->mlis - (char *)prec);
    prt->papFldDes[busyRecordBKLNK]->offset = (unsigned short)((char *)&prec->bklnk - (char *)prec);
    prt->papFldDes[busyRecordDISP]->offset = (unsigned short)((char *)&prec->disp - (char *)prec);
    prt->papFldDes[busyRecordPROC]->offset = (unsigned short)((char *)&prec->proc - (char *)prec);
    prt->papFldDes[busyRecordSTAT]->offset = (unsigned short)((char *)&prec->stat - (char *)prec);
    prt->papFldDes[busyRecordSEVR]->offset = (unsigned short)((char *)&prec->sevr - (char *)prec);
    prt->papFldDes[busyRecordNSTA]->offset = (unsigned short)((char *)&prec->nsta - (char *)prec);
    prt->papFldDes[busyRecordNSEV]->offset = (unsigned short)((char *)&prec->nsev - (char *)prec);
    prt->papFldDes[busyRecordACKS]->offset = (unsigned short)((char *)&prec->acks - (char *)prec);
    prt->papFldDes[busyRecordACKT]->offset = (unsigned short)((char *)&prec->ackt - (char *)prec);
    prt->papFldDes[busyRecordDISS]->offset = (unsigned short)((char *)&prec->diss - (char *)prec);
    prt->papFldDes[busyRecordLCNT]->offset = (unsigned short)((char *)&prec->lcnt - (char *)prec);
    prt->papFldDes[busyRecordPACT]->offset = (unsigned short)((char *)&prec->pact - (char *)prec);
    prt->papFldDes[busyRecordPUTF]->offset = (unsigned short)((char *)&prec->putf - (char *)prec);
    prt->papFldDes[busyRecordRPRO]->offset = (unsigned short)((char *)&prec->rpro - (char *)prec);
    prt->papFldDes[busyRecordASP]->offset = (unsigned short)((char *)&prec->asp - (char *)prec);
    prt->papFldDes[busyRecordPPN]->offset = (unsigned short)((char *)&prec->ppn - (char *)prec);
    prt->papFldDes[busyRecordPPNR]->offset = (unsigned short)((char *)&prec->ppnr - (char *)prec);
    prt->papFldDes[busyRecordSPVT]->offset = (unsigned short)((char *)&prec->spvt - (char *)prec);
    prt->papFldDes[busyRecordRSET]->offset = (unsigned short)((char *)&prec->rset - (char *)prec);
    prt->papFldDes[busyRecordDSET]->offset = (unsigned short)((char *)&prec->dset - (char *)prec);
    prt->papFldDes[busyRecordDPVT]->offset = (unsigned short)((char *)&prec->dpvt - (char *)prec);
    prt->papFldDes[busyRecordRDES]->offset = (unsigned short)((char *)&prec->rdes - (char *)prec);
    prt->papFldDes[busyRecordLSET]->offset = (unsigned short)((char *)&prec->lset - (char *)prec);
    prt->papFldDes[busyRecordPRIO]->offset = (unsigned short)((char *)&prec->prio - (char *)prec);
    prt->papFldDes[busyRecordTPRO]->offset = (unsigned short)((char *)&prec->tpro - (char *)prec);
    prt->papFldDes[busyRecordBKPT]->offset = (unsigned short)((char *)&prec->bkpt - (char *)prec);
    prt->papFldDes[busyRecordUDF]->offset = (unsigned short)((char *)&prec->udf - (char *)prec);
    prt->papFldDes[busyRecordUDFS]->offset = (unsigned short)((char *)&prec->udfs - (char *)prec);
    prt->papFldDes[busyRecordTIME]->offset = (unsigned short)((char *)&prec->time - (char *)prec);
    prt->papFldDes[busyRecordFLNK]->offset = (unsigned short)((char *)&prec->flnk - (char *)prec);
    prt->papFldDes[busyRecordVAL]->offset = (unsigned short)((char *)&prec->val - (char *)prec);
    prt->papFldDes[busyRecordOMSL]->offset = (unsigned short)((char *)&prec->omsl - (char *)prec);
    prt->papFldDes[busyRecordDOL]->offset = (unsigned short)((char *)&prec->dol - (char *)prec);
    prt->papFldDes[busyRecordOUT]->offset = (unsigned short)((char *)&prec->out - (char *)prec);
    prt->papFldDes[busyRecordHIGH]->offset = (unsigned short)((char *)&prec->high - (char *)prec);
    prt->papFldDes[busyRecordZNAM]->offset = (unsigned short)((char *)&prec->znam - (char *)prec);
    prt->papFldDes[busyRecordONAM]->offset = (unsigned short)((char *)&prec->onam - (char *)prec);
    prt->papFldDes[busyRecordRVAL]->offset = (unsigned short)((char *)&prec->rval - (char *)prec);
    prt->papFldDes[busyRecordORAW]->offset = (unsigned short)((char *)&prec->oraw - (char *)prec);
    prt->papFldDes[busyRecordOVAL]->offset = (unsigned short)((char *)&prec->oval - (char *)prec);
    prt->papFldDes[busyRecordMASK]->offset = (unsigned short)((char *)&prec->mask - (char *)prec);
    prt->papFldDes[busyRecordRPVT]->offset = (unsigned short)((char *)&prec->rpvt - (char *)prec);
    prt->papFldDes[busyRecordWDPT]->offset = (unsigned short)((char *)&prec->wdpt - (char *)prec);
    prt->papFldDes[busyRecordZSV]->offset = (unsigned short)((char *)&prec->zsv - (char *)prec);
    prt->papFldDes[busyRecordOSV]->offset = (unsigned short)((char *)&prec->osv - (char *)prec);
    prt->papFldDes[busyRecordCOSV]->offset = (unsigned short)((char *)&prec->cosv - (char *)prec);
    prt->papFldDes[busyRecordRBV]->offset = (unsigned short)((char *)&prec->rbv - (char *)prec);
    prt->papFldDes[busyRecordORBV]->offset = (unsigned short)((char *)&prec->orbv - (char *)prec);
    prt->papFldDes[busyRecordMLST]->offset = (unsigned short)((char *)&prec->mlst - (char *)prec);
    prt->papFldDes[busyRecordLALM]->offset = (unsigned short)((char *)&prec->lalm - (char *)prec);
    prt->papFldDes[busyRecordSIOL]->offset = (unsigned short)((char *)&prec->siol - (char *)prec);
    prt->papFldDes[busyRecordSIML]->offset = (unsigned short)((char *)&prec->siml - (char *)prec);
    prt->papFldDes[busyRecordSIMM]->offset = (unsigned short)((char *)&prec->simm - (char *)prec);
    prt->papFldDes[busyRecordSIMS]->offset = (unsigned short)((char *)&prec->sims - (char *)prec);
    prt->papFldDes[busyRecordIVOA]->offset = (unsigned short)((char *)&prec->ivoa - (char *)prec);
    prt->papFldDes[busyRecordIVOV]->offset = (unsigned short)((char *)&prec->ivov - (char *)prec);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(busyRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_busyRecord_H */
