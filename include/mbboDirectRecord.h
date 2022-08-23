/* mbboDirectRecord.h generated from mbboDirectRecord.dbd */

#ifndef INC_mbboDirectRecord_H
#define INC_mbboDirectRecord_H

#include "epicsTypes.h"
#include "link.h"
 #include "epicsMutex.h"
#include "ellLib.h"
#include "epicsTime.h"

typedef struct mbboDirectRecord {
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
    char                amsg[40];   /* Alarm Message */
    epicsEnum16         nsta;       /* New Alarm Status */
    epicsEnum16         nsev;       /* New Alarm Severity */
    char                namsg[40];  /* New Alarm Message */
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
    epicsUInt16         val;        /* Word */
    epicsEnum16         omsl;       /* Output Mode Select */
    epicsInt16          nobt;       /* Number of Bits */
    DBLINK              dol;        /* Desired Output Loc */
    DBLINK              out;        /* Output Specification */
    epicsUInt8          b0;         /* Bit 0 */
    epicsUInt8          b1;         /* Bit 1 */
    epicsUInt8          b2;         /* Bit 2 */
    epicsUInt8          b3;         /* Bit 3 */
    epicsUInt8          b4;         /* Bit 4 */
    epicsUInt8          b5;         /* Bit 5 */
    epicsUInt8          b6;         /* Bit 6 */
    epicsUInt8          b7;         /* Bit 7 */
    epicsUInt8          b8;         /* Bit 8 */
    epicsUInt8          b9;         /* Bit 9 */
    epicsUInt8          ba;         /* Bit 10 */
    epicsUInt8          bb;         /* Bit 11 */
    epicsUInt8          bc;         /* Bit 12 */
    epicsUInt8          bd;         /* Bit 13 */
    epicsUInt8          be;         /* Bit 14 */
    epicsUInt8          bf;         /* Bit 15 */
    epicsUInt32         rval;       /* Raw Value */
    epicsUInt32         oraw;       /* Prev Raw Value */
    epicsUInt32         rbv;        /* Readback Value */
    epicsUInt32         orbv;       /* Prev Readback Value */
    epicsUInt32         mask;       /* Hardware Mask */
    epicsUInt32         mlst;       /* Last Value Monitored */
    epicsUInt32         shft;       /* Shift */
    DBLINK              siol;       /* Sim Output Specifctn */
    DBLINK              siml;       /* Sim Mode Location */
    epicsEnum16         simm;       /* Simulation Mode */
    epicsEnum16         sims;       /* Sim mode Alarm Svrty */
    epicsEnum16         ivoa;       /* INVALID outpt action */
    epicsUInt16         ivov;       /* INVALID output value */
} mbboDirectRecord;

typedef enum {
	mbboDirectRecordNAME = 0,
	mbboDirectRecordDESC = 1,
	mbboDirectRecordASG = 2,
	mbboDirectRecordSCAN = 3,
	mbboDirectRecordPINI = 4,
	mbboDirectRecordPHAS = 5,
	mbboDirectRecordEVNT = 6,
	mbboDirectRecordTSE = 7,
	mbboDirectRecordTSEL = 8,
	mbboDirectRecordDTYP = 9,
	mbboDirectRecordDISV = 10,
	mbboDirectRecordDISA = 11,
	mbboDirectRecordSDIS = 12,
	mbboDirectRecordMLOK = 13,
	mbboDirectRecordMLIS = 14,
	mbboDirectRecordBKLNK = 15,
	mbboDirectRecordDISP = 16,
	mbboDirectRecordPROC = 17,
	mbboDirectRecordSTAT = 18,
	mbboDirectRecordSEVR = 19,
	mbboDirectRecordAMSG = 20,
	mbboDirectRecordNSTA = 21,
	mbboDirectRecordNSEV = 22,
	mbboDirectRecordNAMSG = 23,
	mbboDirectRecordACKS = 24,
	mbboDirectRecordACKT = 25,
	mbboDirectRecordDISS = 26,
	mbboDirectRecordLCNT = 27,
	mbboDirectRecordPACT = 28,
	mbboDirectRecordPUTF = 29,
	mbboDirectRecordRPRO = 30,
	mbboDirectRecordASP = 31,
	mbboDirectRecordPPN = 32,
	mbboDirectRecordPPNR = 33,
	mbboDirectRecordSPVT = 34,
	mbboDirectRecordRSET = 35,
	mbboDirectRecordDSET = 36,
	mbboDirectRecordDPVT = 37,
	mbboDirectRecordRDES = 38,
	mbboDirectRecordLSET = 39,
	mbboDirectRecordPRIO = 40,
	mbboDirectRecordTPRO = 41,
	mbboDirectRecordBKPT = 42,
	mbboDirectRecordUDF = 43,
	mbboDirectRecordUDFS = 44,
	mbboDirectRecordTIME = 45,
	mbboDirectRecordFLNK = 46,
	mbboDirectRecordVAL = 47,
	mbboDirectRecordOMSL = 48,
	mbboDirectRecordNOBT = 49,
	mbboDirectRecordDOL = 50,
	mbboDirectRecordOUT = 51,
	mbboDirectRecordB0 = 52,
	mbboDirectRecordB1 = 53,
	mbboDirectRecordB2 = 54,
	mbboDirectRecordB3 = 55,
	mbboDirectRecordB4 = 56,
	mbboDirectRecordB5 = 57,
	mbboDirectRecordB6 = 58,
	mbboDirectRecordB7 = 59,
	mbboDirectRecordB8 = 60,
	mbboDirectRecordB9 = 61,
	mbboDirectRecordBA = 62,
	mbboDirectRecordBB = 63,
	mbboDirectRecordBC = 64,
	mbboDirectRecordBD = 65,
	mbboDirectRecordBE = 66,
	mbboDirectRecordBF = 67,
	mbboDirectRecordRVAL = 68,
	mbboDirectRecordORAW = 69,
	mbboDirectRecordRBV = 70,
	mbboDirectRecordORBV = 71,
	mbboDirectRecordMASK = 72,
	mbboDirectRecordMLST = 73,
	mbboDirectRecordSHFT = 74,
	mbboDirectRecordSIOL = 75,
	mbboDirectRecordSIML = 76,
	mbboDirectRecordSIMM = 77,
	mbboDirectRecordSIMS = 78,
	mbboDirectRecordIVOA = 79,
	mbboDirectRecordIVOV = 80
} mbboDirectFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsAssert.h>
#include <epicsExport.h>
#ifdef __cplusplus
extern "C" {
#endif
static int mbboDirectRecordSizeOffset(dbRecordType *prt)
{
    mbboDirectRecord *prec = 0;

    assert(prt->no_fields == 81);
    prt->papFldDes[mbboDirectRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[mbboDirectRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[mbboDirectRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[mbboDirectRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[mbboDirectRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[mbboDirectRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[mbboDirectRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[mbboDirectRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[mbboDirectRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[mbboDirectRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[mbboDirectRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[mbboDirectRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[mbboDirectRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[mbboDirectRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[mbboDirectRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[mbboDirectRecordBKLNK]->size = sizeof(prec->bklnk);
    prt->papFldDes[mbboDirectRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[mbboDirectRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[mbboDirectRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[mbboDirectRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[mbboDirectRecordAMSG]->size = sizeof(prec->amsg);
    prt->papFldDes[mbboDirectRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[mbboDirectRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[mbboDirectRecordNAMSG]->size = sizeof(prec->namsg);
    prt->papFldDes[mbboDirectRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[mbboDirectRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[mbboDirectRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[mbboDirectRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[mbboDirectRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[mbboDirectRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[mbboDirectRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[mbboDirectRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[mbboDirectRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[mbboDirectRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[mbboDirectRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[mbboDirectRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[mbboDirectRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[mbboDirectRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[mbboDirectRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[mbboDirectRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[mbboDirectRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[mbboDirectRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[mbboDirectRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[mbboDirectRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[mbboDirectRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[mbboDirectRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[mbboDirectRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[mbboDirectRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[mbboDirectRecordOMSL]->size = sizeof(prec->omsl);
    prt->papFldDes[mbboDirectRecordNOBT]->size = sizeof(prec->nobt);
    prt->papFldDes[mbboDirectRecordDOL]->size = sizeof(prec->dol);
    prt->papFldDes[mbboDirectRecordOUT]->size = sizeof(prec->out);
    prt->papFldDes[mbboDirectRecordB0]->size = sizeof(prec->b0);
    prt->papFldDes[mbboDirectRecordB1]->size = sizeof(prec->b1);
    prt->papFldDes[mbboDirectRecordB2]->size = sizeof(prec->b2);
    prt->papFldDes[mbboDirectRecordB3]->size = sizeof(prec->b3);
    prt->papFldDes[mbboDirectRecordB4]->size = sizeof(prec->b4);
    prt->papFldDes[mbboDirectRecordB5]->size = sizeof(prec->b5);
    prt->papFldDes[mbboDirectRecordB6]->size = sizeof(prec->b6);
    prt->papFldDes[mbboDirectRecordB7]->size = sizeof(prec->b7);
    prt->papFldDes[mbboDirectRecordB8]->size = sizeof(prec->b8);
    prt->papFldDes[mbboDirectRecordB9]->size = sizeof(prec->b9);
    prt->papFldDes[mbboDirectRecordBA]->size = sizeof(prec->ba);
    prt->papFldDes[mbboDirectRecordBB]->size = sizeof(prec->bb);
    prt->papFldDes[mbboDirectRecordBC]->size = sizeof(prec->bc);
    prt->papFldDes[mbboDirectRecordBD]->size = sizeof(prec->bd);
    prt->papFldDes[mbboDirectRecordBE]->size = sizeof(prec->be);
    prt->papFldDes[mbboDirectRecordBF]->size = sizeof(prec->bf);
    prt->papFldDes[mbboDirectRecordRVAL]->size = sizeof(prec->rval);
    prt->papFldDes[mbboDirectRecordORAW]->size = sizeof(prec->oraw);
    prt->papFldDes[mbboDirectRecordRBV]->size = sizeof(prec->rbv);
    prt->papFldDes[mbboDirectRecordORBV]->size = sizeof(prec->orbv);
    prt->papFldDes[mbboDirectRecordMASK]->size = sizeof(prec->mask);
    prt->papFldDes[mbboDirectRecordMLST]->size = sizeof(prec->mlst);
    prt->papFldDes[mbboDirectRecordSHFT]->size = sizeof(prec->shft);
    prt->papFldDes[mbboDirectRecordSIOL]->size = sizeof(prec->siol);
    prt->papFldDes[mbboDirectRecordSIML]->size = sizeof(prec->siml);
    prt->papFldDes[mbboDirectRecordSIMM]->size = sizeof(prec->simm);
    prt->papFldDes[mbboDirectRecordSIMS]->size = sizeof(prec->sims);
    prt->papFldDes[mbboDirectRecordIVOA]->size = sizeof(prec->ivoa);
    prt->papFldDes[mbboDirectRecordIVOV]->size = sizeof(prec->ivov);
    prt->papFldDes[mbboDirectRecordNAME]->offset = (unsigned short)((char *)&prec->name - (char *)prec);
    prt->papFldDes[mbboDirectRecordDESC]->offset = (unsigned short)((char *)&prec->desc - (char *)prec);
    prt->papFldDes[mbboDirectRecordASG]->offset = (unsigned short)((char *)&prec->asg - (char *)prec);
    prt->papFldDes[mbboDirectRecordSCAN]->offset = (unsigned short)((char *)&prec->scan - (char *)prec);
    prt->papFldDes[mbboDirectRecordPINI]->offset = (unsigned short)((char *)&prec->pini - (char *)prec);
    prt->papFldDes[mbboDirectRecordPHAS]->offset = (unsigned short)((char *)&prec->phas - (char *)prec);
    prt->papFldDes[mbboDirectRecordEVNT]->offset = (unsigned short)((char *)&prec->evnt - (char *)prec);
    prt->papFldDes[mbboDirectRecordTSE]->offset = (unsigned short)((char *)&prec->tse - (char *)prec);
    prt->papFldDes[mbboDirectRecordTSEL]->offset = (unsigned short)((char *)&prec->tsel - (char *)prec);
    prt->papFldDes[mbboDirectRecordDTYP]->offset = (unsigned short)((char *)&prec->dtyp - (char *)prec);
    prt->papFldDes[mbboDirectRecordDISV]->offset = (unsigned short)((char *)&prec->disv - (char *)prec);
    prt->papFldDes[mbboDirectRecordDISA]->offset = (unsigned short)((char *)&prec->disa - (char *)prec);
    prt->papFldDes[mbboDirectRecordSDIS]->offset = (unsigned short)((char *)&prec->sdis - (char *)prec);
    prt->papFldDes[mbboDirectRecordMLOK]->offset = (unsigned short)((char *)&prec->mlok - (char *)prec);
    prt->papFldDes[mbboDirectRecordMLIS]->offset = (unsigned short)((char *)&prec->mlis - (char *)prec);
    prt->papFldDes[mbboDirectRecordBKLNK]->offset = (unsigned short)((char *)&prec->bklnk - (char *)prec);
    prt->papFldDes[mbboDirectRecordDISP]->offset = (unsigned short)((char *)&prec->disp - (char *)prec);
    prt->papFldDes[mbboDirectRecordPROC]->offset = (unsigned short)((char *)&prec->proc - (char *)prec);
    prt->papFldDes[mbboDirectRecordSTAT]->offset = (unsigned short)((char *)&prec->stat - (char *)prec);
    prt->papFldDes[mbboDirectRecordSEVR]->offset = (unsigned short)((char *)&prec->sevr - (char *)prec);
    prt->papFldDes[mbboDirectRecordAMSG]->offset = (unsigned short)((char *)&prec->amsg - (char *)prec);
    prt->papFldDes[mbboDirectRecordNSTA]->offset = (unsigned short)((char *)&prec->nsta - (char *)prec);
    prt->papFldDes[mbboDirectRecordNSEV]->offset = (unsigned short)((char *)&prec->nsev - (char *)prec);
    prt->papFldDes[mbboDirectRecordNAMSG]->offset = (unsigned short)((char *)&prec->namsg - (char *)prec);
    prt->papFldDes[mbboDirectRecordACKS]->offset = (unsigned short)((char *)&prec->acks - (char *)prec);
    prt->papFldDes[mbboDirectRecordACKT]->offset = (unsigned short)((char *)&prec->ackt - (char *)prec);
    prt->papFldDes[mbboDirectRecordDISS]->offset = (unsigned short)((char *)&prec->diss - (char *)prec);
    prt->papFldDes[mbboDirectRecordLCNT]->offset = (unsigned short)((char *)&prec->lcnt - (char *)prec);
    prt->papFldDes[mbboDirectRecordPACT]->offset = (unsigned short)((char *)&prec->pact - (char *)prec);
    prt->papFldDes[mbboDirectRecordPUTF]->offset = (unsigned short)((char *)&prec->putf - (char *)prec);
    prt->papFldDes[mbboDirectRecordRPRO]->offset = (unsigned short)((char *)&prec->rpro - (char *)prec);
    prt->papFldDes[mbboDirectRecordASP]->offset = (unsigned short)((char *)&prec->asp - (char *)prec);
    prt->papFldDes[mbboDirectRecordPPN]->offset = (unsigned short)((char *)&prec->ppn - (char *)prec);
    prt->papFldDes[mbboDirectRecordPPNR]->offset = (unsigned short)((char *)&prec->ppnr - (char *)prec);
    prt->papFldDes[mbboDirectRecordSPVT]->offset = (unsigned short)((char *)&prec->spvt - (char *)prec);
    prt->papFldDes[mbboDirectRecordRSET]->offset = (unsigned short)((char *)&prec->rset - (char *)prec);
    prt->papFldDes[mbboDirectRecordDSET]->offset = (unsigned short)((char *)&prec->dset - (char *)prec);
    prt->papFldDes[mbboDirectRecordDPVT]->offset = (unsigned short)((char *)&prec->dpvt - (char *)prec);
    prt->papFldDes[mbboDirectRecordRDES]->offset = (unsigned short)((char *)&prec->rdes - (char *)prec);
    prt->papFldDes[mbboDirectRecordLSET]->offset = (unsigned short)((char *)&prec->lset - (char *)prec);
    prt->papFldDes[mbboDirectRecordPRIO]->offset = (unsigned short)((char *)&prec->prio - (char *)prec);
    prt->papFldDes[mbboDirectRecordTPRO]->offset = (unsigned short)((char *)&prec->tpro - (char *)prec);
    prt->papFldDes[mbboDirectRecordBKPT]->offset = (unsigned short)((char *)&prec->bkpt - (char *)prec);
    prt->papFldDes[mbboDirectRecordUDF]->offset = (unsigned short)((char *)&prec->udf - (char *)prec);
    prt->papFldDes[mbboDirectRecordUDFS]->offset = (unsigned short)((char *)&prec->udfs - (char *)prec);
    prt->papFldDes[mbboDirectRecordTIME]->offset = (unsigned short)((char *)&prec->time - (char *)prec);
    prt->papFldDes[mbboDirectRecordFLNK]->offset = (unsigned short)((char *)&prec->flnk - (char *)prec);
    prt->papFldDes[mbboDirectRecordVAL]->offset = (unsigned short)((char *)&prec->val - (char *)prec);
    prt->papFldDes[mbboDirectRecordOMSL]->offset = (unsigned short)((char *)&prec->omsl - (char *)prec);
    prt->papFldDes[mbboDirectRecordNOBT]->offset = (unsigned short)((char *)&prec->nobt - (char *)prec);
    prt->papFldDes[mbboDirectRecordDOL]->offset = (unsigned short)((char *)&prec->dol - (char *)prec);
    prt->papFldDes[mbboDirectRecordOUT]->offset = (unsigned short)((char *)&prec->out - (char *)prec);
    prt->papFldDes[mbboDirectRecordB0]->offset = (unsigned short)((char *)&prec->b0 - (char *)prec);
    prt->papFldDes[mbboDirectRecordB1]->offset = (unsigned short)((char *)&prec->b1 - (char *)prec);
    prt->papFldDes[mbboDirectRecordB2]->offset = (unsigned short)((char *)&prec->b2 - (char *)prec);
    prt->papFldDes[mbboDirectRecordB3]->offset = (unsigned short)((char *)&prec->b3 - (char *)prec);
    prt->papFldDes[mbboDirectRecordB4]->offset = (unsigned short)((char *)&prec->b4 - (char *)prec);
    prt->papFldDes[mbboDirectRecordB5]->offset = (unsigned short)((char *)&prec->b5 - (char *)prec);
    prt->papFldDes[mbboDirectRecordB6]->offset = (unsigned short)((char *)&prec->b6 - (char *)prec);
    prt->papFldDes[mbboDirectRecordB7]->offset = (unsigned short)((char *)&prec->b7 - (char *)prec);
    prt->papFldDes[mbboDirectRecordB8]->offset = (unsigned short)((char *)&prec->b8 - (char *)prec);
    prt->papFldDes[mbboDirectRecordB9]->offset = (unsigned short)((char *)&prec->b9 - (char *)prec);
    prt->papFldDes[mbboDirectRecordBA]->offset = (unsigned short)((char *)&prec->ba - (char *)prec);
    prt->papFldDes[mbboDirectRecordBB]->offset = (unsigned short)((char *)&prec->bb - (char *)prec);
    prt->papFldDes[mbboDirectRecordBC]->offset = (unsigned short)((char *)&prec->bc - (char *)prec);
    prt->papFldDes[mbboDirectRecordBD]->offset = (unsigned short)((char *)&prec->bd - (char *)prec);
    prt->papFldDes[mbboDirectRecordBE]->offset = (unsigned short)((char *)&prec->be - (char *)prec);
    prt->papFldDes[mbboDirectRecordBF]->offset = (unsigned short)((char *)&prec->bf - (char *)prec);
    prt->papFldDes[mbboDirectRecordRVAL]->offset = (unsigned short)((char *)&prec->rval - (char *)prec);
    prt->papFldDes[mbboDirectRecordORAW]->offset = (unsigned short)((char *)&prec->oraw - (char *)prec);
    prt->papFldDes[mbboDirectRecordRBV]->offset = (unsigned short)((char *)&prec->rbv - (char *)prec);
    prt->papFldDes[mbboDirectRecordORBV]->offset = (unsigned short)((char *)&prec->orbv - (char *)prec);
    prt->papFldDes[mbboDirectRecordMASK]->offset = (unsigned short)((char *)&prec->mask - (char *)prec);
    prt->papFldDes[mbboDirectRecordMLST]->offset = (unsigned short)((char *)&prec->mlst - (char *)prec);
    prt->papFldDes[mbboDirectRecordSHFT]->offset = (unsigned short)((char *)&prec->shft - (char *)prec);
    prt->papFldDes[mbboDirectRecordSIOL]->offset = (unsigned short)((char *)&prec->siol - (char *)prec);
    prt->papFldDes[mbboDirectRecordSIML]->offset = (unsigned short)((char *)&prec->siml - (char *)prec);
    prt->papFldDes[mbboDirectRecordSIMM]->offset = (unsigned short)((char *)&prec->simm - (char *)prec);
    prt->papFldDes[mbboDirectRecordSIMS]->offset = (unsigned short)((char *)&prec->sims - (char *)prec);
    prt->papFldDes[mbboDirectRecordIVOA]->offset = (unsigned short)((char *)&prec->ivoa - (char *)prec);
    prt->papFldDes[mbboDirectRecordIVOV]->offset = (unsigned short)((char *)&prec->ivov - (char *)prec);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(mbboDirectRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_mbboDirectRecord_H */
