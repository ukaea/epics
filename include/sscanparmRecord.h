/* sscanparmRecord.h generated from scanparmRecord.dbd */

#ifndef INC_sscanparmRecord_H
#define INC_sscanparmRecord_H

 #include "epicsTypes.h"
 #include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "epicsTime.h"

typedef struct scanparmRecord {
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
    struct rset         *rset;      /* Address of RSET */
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
    epicsFloat32        vers;       /* Code Version */
    epicsFloat64        val;        /* Result */
    epicsInt16          prec;       /* Display Precision */
    epicsInt16          pre;        /* PRE-write command */
    DBLINK              opre;       /* PRE-write OutLink */
    epicsEnum16         sm;         /* StepMode */
    DBLINK              osm;        /* SM OutLink */
    epicsEnum16         ar;         /* absRel */
    DBLINK              oar;        /* AR OutLink */
    epicsEnum16         aft;        /* After */
    DBLINK              oaft;       /* AFT OutLink */
    char                ppv[40];    /* PositionerPVName */
    char                rpv[40];    /* ReadbackPVName */
    char                dpv[40];    /* DetPVName */
    char                tpv[40];    /* TrigPVName */
    DBLINK              oppv;       /* P1PV OutLink */
    DBLINK              orpv;       /* R1PV OutLink */
    DBLINK              odpv;       /* D1PV OutLink */
    DBLINK              otpv;       /* T1PV OutLink */
    epicsFloat64        sp;         /* StartPos */
    DBLINK              osp;        /* SP OutLink */
    epicsFloat64        ep;         /* EndPos */
    DBLINK              oep;        /* EP OutLink */
    epicsInt32          np;         /* nPts */
    DBLINK              onp;        /* NP OutLink */
    epicsInt16          sc;         /* StartCmd */
    DBLINK              osc;        /* SC OutLink */
    epicsFloat64        aqt;        /* Acquire time */
    DBLINK              oaqt;       /* AQT OutLink */
    epicsInt32          mp;         /* MaxPts */
    DBLINK              imp;        /* MP InLink */
    epicsInt16          act;        /* ScanActive */
    DBLINK              iact;       /* InLink */
    epicsInt16          load;       /* Load */
    DBLINK              oload;      /* LOAD OutLink */
    epicsInt16          go;         /* Go */
    DBLINK              ogo;        /* GO OutLink */
    epicsFloat64        step;       /* StepSize */
    epicsFloat64        lstp;       /* Last stepSize */
} scanparmRecord;

typedef enum {
	scanparmRecordNAME = 0,
	scanparmRecordDESC = 1,
	scanparmRecordASG = 2,
	scanparmRecordSCAN = 3,
	scanparmRecordPINI = 4,
	scanparmRecordPHAS = 5,
	scanparmRecordEVNT = 6,
	scanparmRecordTSE = 7,
	scanparmRecordTSEL = 8,
	scanparmRecordDTYP = 9,
	scanparmRecordDISV = 10,
	scanparmRecordDISA = 11,
	scanparmRecordSDIS = 12,
	scanparmRecordMLOK = 13,
	scanparmRecordMLIS = 14,
	scanparmRecordDISP = 15,
	scanparmRecordPROC = 16,
	scanparmRecordSTAT = 17,
	scanparmRecordSEVR = 18,
	scanparmRecordNSTA = 19,
	scanparmRecordNSEV = 20,
	scanparmRecordACKS = 21,
	scanparmRecordACKT = 22,
	scanparmRecordDISS = 23,
	scanparmRecordLCNT = 24,
	scanparmRecordPACT = 25,
	scanparmRecordPUTF = 26,
	scanparmRecordRPRO = 27,
	scanparmRecordASP = 28,
	scanparmRecordPPN = 29,
	scanparmRecordPPNR = 30,
	scanparmRecordSPVT = 31,
	scanparmRecordRSET = 32,
	scanparmRecordDSET = 33,
	scanparmRecordDPVT = 34,
	scanparmRecordRDES = 35,
	scanparmRecordLSET = 36,
	scanparmRecordPRIO = 37,
	scanparmRecordTPRO = 38,
	scanparmRecordBKPT = 39,
	scanparmRecordUDF = 40,
	scanparmRecordUDFS = 41,
	scanparmRecordTIME = 42,
	scanparmRecordFLNK = 43,
	scanparmRecordVERS = 44,
	scanparmRecordVAL = 45,
	scanparmRecordPREC = 46,
	scanparmRecordPRE = 47,
	scanparmRecordOPRE = 48,
	scanparmRecordSM = 49,
	scanparmRecordOSM = 50,
	scanparmRecordAR = 51,
	scanparmRecordOAR = 52,
	scanparmRecordAFT = 53,
	scanparmRecordOAFT = 54,
	scanparmRecordPPV = 55,
	scanparmRecordRPV = 56,
	scanparmRecordDPV = 57,
	scanparmRecordTPV = 58,
	scanparmRecordOPPV = 59,
	scanparmRecordORPV = 60,
	scanparmRecordODPV = 61,
	scanparmRecordOTPV = 62,
	scanparmRecordSP = 63,
	scanparmRecordOSP = 64,
	scanparmRecordEP = 65,
	scanparmRecordOEP = 66,
	scanparmRecordNP = 67,
	scanparmRecordONP = 68,
	scanparmRecordSC = 69,
	scanparmRecordOSC = 70,
	scanparmRecordAQT = 71,
	scanparmRecordOAQT = 72,
	scanparmRecordMP = 73,
	scanparmRecordIMP = 74,
	scanparmRecordACT = 75,
	scanparmRecordIACT = 76,
	scanparmRecordLOAD = 77,
	scanparmRecordOLOAD = 78,
	scanparmRecordGO = 79,
	scanparmRecordOGO = 80,
	scanparmRecordSTEP = 81,
	scanparmRecordLSTP = 82
} scanparmFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsAssert.h>
#include <epicsExport.h>
#ifdef __cplusplus
extern "C" {
#endif
static int scanparmRecordSizeOffset(dbRecordType *prt)
{
    scanparmRecord *prec = 0;

    assert(prt->no_fields == 83);
    prt->papFldDes[scanparmRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[scanparmRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[scanparmRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[scanparmRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[scanparmRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[scanparmRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[scanparmRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[scanparmRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[scanparmRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[scanparmRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[scanparmRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[scanparmRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[scanparmRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[scanparmRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[scanparmRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[scanparmRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[scanparmRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[scanparmRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[scanparmRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[scanparmRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[scanparmRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[scanparmRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[scanparmRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[scanparmRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[scanparmRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[scanparmRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[scanparmRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[scanparmRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[scanparmRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[scanparmRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[scanparmRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[scanparmRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[scanparmRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[scanparmRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[scanparmRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[scanparmRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[scanparmRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[scanparmRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[scanparmRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[scanparmRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[scanparmRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[scanparmRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[scanparmRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[scanparmRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[scanparmRecordVERS]->size = sizeof(prec->vers);
    prt->papFldDes[scanparmRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[scanparmRecordPREC]->size = sizeof(prec->prec);
    prt->papFldDes[scanparmRecordPRE]->size = sizeof(prec->pre);
    prt->papFldDes[scanparmRecordOPRE]->size = sizeof(prec->opre);
    prt->papFldDes[scanparmRecordSM]->size = sizeof(prec->sm);
    prt->papFldDes[scanparmRecordOSM]->size = sizeof(prec->osm);
    prt->papFldDes[scanparmRecordAR]->size = sizeof(prec->ar);
    prt->papFldDes[scanparmRecordOAR]->size = sizeof(prec->oar);
    prt->papFldDes[scanparmRecordAFT]->size = sizeof(prec->aft);
    prt->papFldDes[scanparmRecordOAFT]->size = sizeof(prec->oaft);
    prt->papFldDes[scanparmRecordPPV]->size = sizeof(prec->ppv);
    prt->papFldDes[scanparmRecordRPV]->size = sizeof(prec->rpv);
    prt->papFldDes[scanparmRecordDPV]->size = sizeof(prec->dpv);
    prt->papFldDes[scanparmRecordTPV]->size = sizeof(prec->tpv);
    prt->papFldDes[scanparmRecordOPPV]->size = sizeof(prec->oppv);
    prt->papFldDes[scanparmRecordORPV]->size = sizeof(prec->orpv);
    prt->papFldDes[scanparmRecordODPV]->size = sizeof(prec->odpv);
    prt->papFldDes[scanparmRecordOTPV]->size = sizeof(prec->otpv);
    prt->papFldDes[scanparmRecordSP]->size = sizeof(prec->sp);
    prt->papFldDes[scanparmRecordOSP]->size = sizeof(prec->osp);
    prt->papFldDes[scanparmRecordEP]->size = sizeof(prec->ep);
    prt->papFldDes[scanparmRecordOEP]->size = sizeof(prec->oep);
    prt->papFldDes[scanparmRecordNP]->size = sizeof(prec->np);
    prt->papFldDes[scanparmRecordONP]->size = sizeof(prec->onp);
    prt->papFldDes[scanparmRecordSC]->size = sizeof(prec->sc);
    prt->papFldDes[scanparmRecordOSC]->size = sizeof(prec->osc);
    prt->papFldDes[scanparmRecordAQT]->size = sizeof(prec->aqt);
    prt->papFldDes[scanparmRecordOAQT]->size = sizeof(prec->oaqt);
    prt->papFldDes[scanparmRecordMP]->size = sizeof(prec->mp);
    prt->papFldDes[scanparmRecordIMP]->size = sizeof(prec->imp);
    prt->papFldDes[scanparmRecordACT]->size = sizeof(prec->act);
    prt->papFldDes[scanparmRecordIACT]->size = sizeof(prec->iact);
    prt->papFldDes[scanparmRecordLOAD]->size = sizeof(prec->load);
    prt->papFldDes[scanparmRecordOLOAD]->size = sizeof(prec->oload);
    prt->papFldDes[scanparmRecordGO]->size = sizeof(prec->go);
    prt->papFldDes[scanparmRecordOGO]->size = sizeof(prec->ogo);
    prt->papFldDes[scanparmRecordSTEP]->size = sizeof(prec->step);
    prt->papFldDes[scanparmRecordLSTP]->size = sizeof(prec->lstp);
    prt->papFldDes[scanparmRecordNAME]->offset = (unsigned short)((char *)&prec->name - (char *)prec);
    prt->papFldDes[scanparmRecordDESC]->offset = (unsigned short)((char *)&prec->desc - (char *)prec);
    prt->papFldDes[scanparmRecordASG]->offset = (unsigned short)((char *)&prec->asg - (char *)prec);
    prt->papFldDes[scanparmRecordSCAN]->offset = (unsigned short)((char *)&prec->scan - (char *)prec);
    prt->papFldDes[scanparmRecordPINI]->offset = (unsigned short)((char *)&prec->pini - (char *)prec);
    prt->papFldDes[scanparmRecordPHAS]->offset = (unsigned short)((char *)&prec->phas - (char *)prec);
    prt->papFldDes[scanparmRecordEVNT]->offset = (unsigned short)((char *)&prec->evnt - (char *)prec);
    prt->papFldDes[scanparmRecordTSE]->offset = (unsigned short)((char *)&prec->tse - (char *)prec);
    prt->papFldDes[scanparmRecordTSEL]->offset = (unsigned short)((char *)&prec->tsel - (char *)prec);
    prt->papFldDes[scanparmRecordDTYP]->offset = (unsigned short)((char *)&prec->dtyp - (char *)prec);
    prt->papFldDes[scanparmRecordDISV]->offset = (unsigned short)((char *)&prec->disv - (char *)prec);
    prt->papFldDes[scanparmRecordDISA]->offset = (unsigned short)((char *)&prec->disa - (char *)prec);
    prt->papFldDes[scanparmRecordSDIS]->offset = (unsigned short)((char *)&prec->sdis - (char *)prec);
    prt->papFldDes[scanparmRecordMLOK]->offset = (unsigned short)((char *)&prec->mlok - (char *)prec);
    prt->papFldDes[scanparmRecordMLIS]->offset = (unsigned short)((char *)&prec->mlis - (char *)prec);
    prt->papFldDes[scanparmRecordDISP]->offset = (unsigned short)((char *)&prec->disp - (char *)prec);
    prt->papFldDes[scanparmRecordPROC]->offset = (unsigned short)((char *)&prec->proc - (char *)prec);
    prt->papFldDes[scanparmRecordSTAT]->offset = (unsigned short)((char *)&prec->stat - (char *)prec);
    prt->papFldDes[scanparmRecordSEVR]->offset = (unsigned short)((char *)&prec->sevr - (char *)prec);
    prt->papFldDes[scanparmRecordNSTA]->offset = (unsigned short)((char *)&prec->nsta - (char *)prec);
    prt->papFldDes[scanparmRecordNSEV]->offset = (unsigned short)((char *)&prec->nsev - (char *)prec);
    prt->papFldDes[scanparmRecordACKS]->offset = (unsigned short)((char *)&prec->acks - (char *)prec);
    prt->papFldDes[scanparmRecordACKT]->offset = (unsigned short)((char *)&prec->ackt - (char *)prec);
    prt->papFldDes[scanparmRecordDISS]->offset = (unsigned short)((char *)&prec->diss - (char *)prec);
    prt->papFldDes[scanparmRecordLCNT]->offset = (unsigned short)((char *)&prec->lcnt - (char *)prec);
    prt->papFldDes[scanparmRecordPACT]->offset = (unsigned short)((char *)&prec->pact - (char *)prec);
    prt->papFldDes[scanparmRecordPUTF]->offset = (unsigned short)((char *)&prec->putf - (char *)prec);
    prt->papFldDes[scanparmRecordRPRO]->offset = (unsigned short)((char *)&prec->rpro - (char *)prec);
    prt->papFldDes[scanparmRecordASP]->offset = (unsigned short)((char *)&prec->asp - (char *)prec);
    prt->papFldDes[scanparmRecordPPN]->offset = (unsigned short)((char *)&prec->ppn - (char *)prec);
    prt->papFldDes[scanparmRecordPPNR]->offset = (unsigned short)((char *)&prec->ppnr - (char *)prec);
    prt->papFldDes[scanparmRecordSPVT]->offset = (unsigned short)((char *)&prec->spvt - (char *)prec);
    prt->papFldDes[scanparmRecordRSET]->offset = (unsigned short)((char *)&prec->rset - (char *)prec);
    prt->papFldDes[scanparmRecordDSET]->offset = (unsigned short)((char *)&prec->dset - (char *)prec);
    prt->papFldDes[scanparmRecordDPVT]->offset = (unsigned short)((char *)&prec->dpvt - (char *)prec);
    prt->papFldDes[scanparmRecordRDES]->offset = (unsigned short)((char *)&prec->rdes - (char *)prec);
    prt->papFldDes[scanparmRecordLSET]->offset = (unsigned short)((char *)&prec->lset - (char *)prec);
    prt->papFldDes[scanparmRecordPRIO]->offset = (unsigned short)((char *)&prec->prio - (char *)prec);
    prt->papFldDes[scanparmRecordTPRO]->offset = (unsigned short)((char *)&prec->tpro - (char *)prec);
    prt->papFldDes[scanparmRecordBKPT]->offset = (unsigned short)((char *)&prec->bkpt - (char *)prec);
    prt->papFldDes[scanparmRecordUDF]->offset = (unsigned short)((char *)&prec->udf - (char *)prec);
    prt->papFldDes[scanparmRecordUDFS]->offset = (unsigned short)((char *)&prec->udfs - (char *)prec);
    prt->papFldDes[scanparmRecordTIME]->offset = (unsigned short)((char *)&prec->time - (char *)prec);
    prt->papFldDes[scanparmRecordFLNK]->offset = (unsigned short)((char *)&prec->flnk - (char *)prec);
    prt->papFldDes[scanparmRecordVERS]->offset = (unsigned short)((char *)&prec->vers - (char *)prec);
    prt->papFldDes[scanparmRecordVAL]->offset = (unsigned short)((char *)&prec->val - (char *)prec);
    prt->papFldDes[scanparmRecordPREC]->offset = (unsigned short)((char *)&prec->prec - (char *)prec);
    prt->papFldDes[scanparmRecordPRE]->offset = (unsigned short)((char *)&prec->pre - (char *)prec);
    prt->papFldDes[scanparmRecordOPRE]->offset = (unsigned short)((char *)&prec->opre - (char *)prec);
    prt->papFldDes[scanparmRecordSM]->offset = (unsigned short)((char *)&prec->sm - (char *)prec);
    prt->papFldDes[scanparmRecordOSM]->offset = (unsigned short)((char *)&prec->osm - (char *)prec);
    prt->papFldDes[scanparmRecordAR]->offset = (unsigned short)((char *)&prec->ar - (char *)prec);
    prt->papFldDes[scanparmRecordOAR]->offset = (unsigned short)((char *)&prec->oar - (char *)prec);
    prt->papFldDes[scanparmRecordAFT]->offset = (unsigned short)((char *)&prec->aft - (char *)prec);
    prt->papFldDes[scanparmRecordOAFT]->offset = (unsigned short)((char *)&prec->oaft - (char *)prec);
    prt->papFldDes[scanparmRecordPPV]->offset = (unsigned short)((char *)&prec->ppv - (char *)prec);
    prt->papFldDes[scanparmRecordRPV]->offset = (unsigned short)((char *)&prec->rpv - (char *)prec);
    prt->papFldDes[scanparmRecordDPV]->offset = (unsigned short)((char *)&prec->dpv - (char *)prec);
    prt->papFldDes[scanparmRecordTPV]->offset = (unsigned short)((char *)&prec->tpv - (char *)prec);
    prt->papFldDes[scanparmRecordOPPV]->offset = (unsigned short)((char *)&prec->oppv - (char *)prec);
    prt->papFldDes[scanparmRecordORPV]->offset = (unsigned short)((char *)&prec->orpv - (char *)prec);
    prt->papFldDes[scanparmRecordODPV]->offset = (unsigned short)((char *)&prec->odpv - (char *)prec);
    prt->papFldDes[scanparmRecordOTPV]->offset = (unsigned short)((char *)&prec->otpv - (char *)prec);
    prt->papFldDes[scanparmRecordSP]->offset = (unsigned short)((char *)&prec->sp - (char *)prec);
    prt->papFldDes[scanparmRecordOSP]->offset = (unsigned short)((char *)&prec->osp - (char *)prec);
    prt->papFldDes[scanparmRecordEP]->offset = (unsigned short)((char *)&prec->ep - (char *)prec);
    prt->papFldDes[scanparmRecordOEP]->offset = (unsigned short)((char *)&prec->oep - (char *)prec);
    prt->papFldDes[scanparmRecordNP]->offset = (unsigned short)((char *)&prec->np - (char *)prec);
    prt->papFldDes[scanparmRecordONP]->offset = (unsigned short)((char *)&prec->onp - (char *)prec);
    prt->papFldDes[scanparmRecordSC]->offset = (unsigned short)((char *)&prec->sc - (char *)prec);
    prt->papFldDes[scanparmRecordOSC]->offset = (unsigned short)((char *)&prec->osc - (char *)prec);
    prt->papFldDes[scanparmRecordAQT]->offset = (unsigned short)((char *)&prec->aqt - (char *)prec);
    prt->papFldDes[scanparmRecordOAQT]->offset = (unsigned short)((char *)&prec->oaqt - (char *)prec);
    prt->papFldDes[scanparmRecordMP]->offset = (unsigned short)((char *)&prec->mp - (char *)prec);
    prt->papFldDes[scanparmRecordIMP]->offset = (unsigned short)((char *)&prec->imp - (char *)prec);
    prt->papFldDes[scanparmRecordACT]->offset = (unsigned short)((char *)&prec->act - (char *)prec);
    prt->papFldDes[scanparmRecordIACT]->offset = (unsigned short)((char *)&prec->iact - (char *)prec);
    prt->papFldDes[scanparmRecordLOAD]->offset = (unsigned short)((char *)&prec->load - (char *)prec);
    prt->papFldDes[scanparmRecordOLOAD]->offset = (unsigned short)((char *)&prec->oload - (char *)prec);
    prt->papFldDes[scanparmRecordGO]->offset = (unsigned short)((char *)&prec->go - (char *)prec);
    prt->papFldDes[scanparmRecordOGO]->offset = (unsigned short)((char *)&prec->ogo - (char *)prec);
    prt->papFldDes[scanparmRecordSTEP]->offset = (unsigned short)((char *)&prec->step - (char *)prec);
    prt->papFldDes[scanparmRecordLSTP]->offset = (unsigned short)((char *)&prec->lstp - (char *)prec);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(scanparmRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_sscanparmRecord_H */
