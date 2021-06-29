/* sseqRecord.h generated from sseqRecord.dbd */

#ifndef INC_sseqRecord_H
#define INC_sseqRecord_H

#include "epicsTypes.h"
#include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "epicsTime.h"

#ifndef sseqSELM_NUM_CHOICES
typedef enum {
    sseqSELM_All                    /* All */,
    sseqSELM_Specified              /* Specified */,
    sseqSELM_Mask                   /* Mask */
} sseqSELM;
#define sseqSELM_NUM_CHOICES 3
#endif

#ifndef sseqWAIT_NUM_CHOICES
typedef enum {
    sseqWAIT_NoWait                 /* NoWait */,
    sseqWAIT_Wait                   /* Wait */,
    sseqWAIT_Wait1                  /* After1 */,
    sseqWAIT_Wait2                  /* After2 */,
    sseqWAIT_Wait3                  /* After3 */,
    sseqWAIT_Wait4                  /* After4 */,
    sseqWAIT_Wait5                  /* After5 */,
    sseqWAIT_Wait6                  /* After6 */,
    sseqWAIT_Wait7                  /* After7 */,
    sseqWAIT_Wait8                  /* After8 */,
    sseqWAIT_Wait9                  /* After9 */,
    sseqWAIT_Wait10                 /* AfterA */
} sseqWAIT;
#define sseqWAIT_NUM_CHOICES 12
#endif

#ifndef sseqLNKV_NUM_CHOICES
typedef enum {
    sseqLNKV_EXT_NC                 /* Ext PV NC */,
    sseqLNKV_EXT                    /* Ext PV OK */,
    sseqLNKV_LOC                    /* Local PV */,
    sseqLNKV_CON                    /* Constant */
} sseqLNKV;
#define sseqLNKV_NUM_CHOICES 4
#endif

typedef struct sseqRecord {
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
    epicsInt32          val;        /* Used to trigger */
    epicsEnum16         selm;       /* Select Mechanism */
    epicsUInt16         seln;       /* Link Selection */
    DBLINK              sell;       /* Link Selection Loc */
    epicsInt16          prec;       /* Display Precision */
    epicsFloat64        dly1;       /* Delay 1 */
    DBLINK              dol1;       /* Input link1 */
    epicsFloat64        do1;        /* Constant input 1 */
    DBLINK              lnk1;       /* Output Link 1 */
    char                str1[40];   /* String value 1 */
    epicsInt16          dt1;        /* DOL link type */
    epicsInt16          lt1;        /* LNK link type */
    epicsEnum16         wait1;      /* Wait for completion? */
    epicsInt16          werr1;      
    epicsInt16          wtg1;       
    epicsInt16          ix1;        
    epicsEnum16         dol1v;      /* DOL LINK Status */
    epicsEnum16         lnk1v;      /* LNK LINK Status */
    epicsFloat64        dly2;       /* Delay 2 */
    DBLINK              dol2;       /* Input link 2 */
    epicsFloat64        do2;        /* Constant input 2 */
    DBLINK              lnk2;       /* Output Link 2 */
    char                str2[40];   /* String value 2 */
    epicsInt16          dt2;        /* DOL link type */
    epicsInt16          lt2;        /* LNK link type */
    epicsEnum16         wait2;      /* Wait for completion? */
    epicsInt16          werr2;      
    epicsInt16          wtg2;       
    epicsInt16          ix2;        
    epicsEnum16         dol2v;      /* DOL LINK Status */
    epicsEnum16         lnk2v;      /* LNK LINK Status */
    epicsFloat64        dly3;       /* Delay 3 */
    DBLINK              dol3;       /* Input link 3 */
    epicsFloat64        do3;        /* Constant input 3 */
    DBLINK              lnk3;       /* Output Link 3 */
    char                str3[40];   /* String value 3 */
    epicsInt16          dt3;        /* DOL link type */
    epicsInt16          lt3;        /* LNK link type */
    epicsEnum16         wait3;      /* Wait for completion? */
    epicsInt16          werr3;      
    epicsInt16          wtg3;       
    epicsInt16          ix3;        
    epicsEnum16         dol3v;      /* DOL LINK Status */
    epicsEnum16         lnk3v;      /* LNK LINK Status */
    epicsFloat64        dly4;       /* Delay 4 */
    DBLINK              dol4;       /* Input link 4 */
    epicsFloat64        do4;        /* Constant input 4 */
    DBLINK              lnk4;       /* Output Link 4 */
    char                str4[40];   /* String value 4 */
    epicsInt16          dt4;        /* DOL link type */
    epicsInt16          lt4;        /* LNK link type */
    epicsEnum16         wait4;      /* Wait for completion? */
    epicsInt16          werr4;      
    epicsInt16          wtg4;       
    epicsInt16          ix4;        
    epicsEnum16         dol4v;      /* DOL LINK Status */
    epicsEnum16         lnk4v;      /* LNK LINK Status */
    epicsFloat64        dly5;       /* Delay 5 */
    DBLINK              dol5;       /* Input link 5 */
    epicsFloat64        do5;        /* Constant input 5 */
    DBLINK              lnk5;       /* Output Link 5 */
    char                str5[40];   /* String value 5 */
    epicsInt16          dt5;        /* DOL link type */
    epicsInt16          lt5;        /* LNK link type */
    epicsEnum16         wait5;      /* Wait for completion? */
    epicsInt16          werr5;      
    epicsInt16          wtg5;       
    epicsInt16          ix5;        
    epicsEnum16         dol5v;      /* DOL LINK Status */
    epicsEnum16         lnk5v;      /* LNK LINK Status */
    epicsFloat64        dly6;       /* Delay 6 */
    DBLINK              dol6;       /* Input link 6 */
    epicsFloat64        do6;        /* Constant input 6 */
    DBLINK              lnk6;       /* Output Link 6 */
    char                str6[40];   /* String value 6 */
    epicsInt16          dt6;        /* DOL link type */
    epicsInt16          lt6;        /* LNK link type */
    epicsEnum16         wait6;      /* Wait for completion? */
    epicsInt16          werr6;      
    epicsInt16          wtg6;       
    epicsInt16          ix6;        
    epicsEnum16         dol6v;      /* DOL LINK Status */
    epicsEnum16         lnk6v;      /* LNK LINK Status */
    epicsFloat64        dly7;       /* Delay 7 */
    DBLINK              dol7;       /* Input link 7 */
    epicsFloat64        do7;        /* Constant input 7 */
    DBLINK              lnk7;       /* Output Link 7 */
    char                str7[40];   /* String value 7 */
    epicsInt16          dt7;        /* DOL link type */
    epicsInt16          lt7;        /* LNK link type */
    epicsEnum16         wait7;      /* Wait for completion? */
    epicsInt16          werr7;      
    epicsInt16          wtg7;       
    epicsInt16          ix7;        
    epicsEnum16         dol7v;      /* DOL LINK Status */
    epicsEnum16         lnk7v;      /* LNK LINK Status */
    epicsFloat64        dly8;       /* Delay 8 */
    DBLINK              dol8;       /* Input link 8 */
    epicsFloat64        do8;        /* Constant input 8 */
    DBLINK              lnk8;       /* Output Link 8 */
    char                str8[40];   /* String value 8 */
    epicsInt16          dt8;        /* DOL link type */
    epicsInt16          lt8;        /* LNK link type */
    epicsEnum16         wait8;      /* Wait for completion? */
    epicsInt16          werr8;      
    epicsInt16          wtg8;       
    epicsInt16          ix8;        
    epicsEnum16         dol8v;      /* DOL LINK Status */
    epicsEnum16         lnk8v;      /* LNK LINK Status */
    epicsFloat64        dly9;       /* Delay 9 */
    DBLINK              dol9;       /* Input link 9 */
    epicsFloat64        do9;        /* Constant input 9 */
    DBLINK              lnk9;       /* Output Link 9 */
    char                str9[40];   /* String value 9 */
    epicsInt16          dt9;        /* DOL link type */
    epicsInt16          lt9;        /* LNK link type */
    epicsEnum16         wait9;      /* Wait for completion? */
    epicsInt16          werr9;      
    epicsInt16          wtg9;       
    epicsInt16          ix9;        
    epicsEnum16         dol9v;      /* DOL LINK Status */
    epicsEnum16         lnk9v;      /* LNK LINK Status */
    epicsFloat64        dlya;       /* Delay 10 */
    DBLINK              dola;       /* Input link 10 */
    epicsFloat64        doa;        /* Constant input 10 */
    DBLINK              lnka;       /* Output Link 10 */
    char                stra[40];   /* String value A */
    epicsInt16          dta;        /* DOL link type */
    epicsInt16          lta;        /* LNK link type */
    epicsEnum16         waita;      /* Wait for completion? */
    epicsInt16          werra;      
    epicsInt16          wtga;       
    epicsInt16          ixa;        
    epicsEnum16         dolav;      /* DOL LINK Status */
    epicsEnum16         lnkav;      /* LNK LINK Status */
    epicsInt16          abort;      /* Abort sequence */
    epicsInt16          aborting;   /* Aborting */
    epicsInt16          busy;       /* Sequence active */
} sseqRecord;

typedef enum {
	sseqRecordNAME = 0,
	sseqRecordDESC = 1,
	sseqRecordASG = 2,
	sseqRecordSCAN = 3,
	sseqRecordPINI = 4,
	sseqRecordPHAS = 5,
	sseqRecordEVNT = 6,
	sseqRecordTSE = 7,
	sseqRecordTSEL = 8,
	sseqRecordDTYP = 9,
	sseqRecordDISV = 10,
	sseqRecordDISA = 11,
	sseqRecordSDIS = 12,
	sseqRecordMLOK = 13,
	sseqRecordMLIS = 14,
	sseqRecordBKLNK = 15,
	sseqRecordDISP = 16,
	sseqRecordPROC = 17,
	sseqRecordSTAT = 18,
	sseqRecordSEVR = 19,
	sseqRecordNSTA = 20,
	sseqRecordNSEV = 21,
	sseqRecordACKS = 22,
	sseqRecordACKT = 23,
	sseqRecordDISS = 24,
	sseqRecordLCNT = 25,
	sseqRecordPACT = 26,
	sseqRecordPUTF = 27,
	sseqRecordRPRO = 28,
	sseqRecordASP = 29,
	sseqRecordPPN = 30,
	sseqRecordPPNR = 31,
	sseqRecordSPVT = 32,
	sseqRecordRSET = 33,
	sseqRecordDSET = 34,
	sseqRecordDPVT = 35,
	sseqRecordRDES = 36,
	sseqRecordLSET = 37,
	sseqRecordPRIO = 38,
	sseqRecordTPRO = 39,
	sseqRecordBKPT = 40,
	sseqRecordUDF = 41,
	sseqRecordUDFS = 42,
	sseqRecordTIME = 43,
	sseqRecordFLNK = 44,
	sseqRecordVAL = 45,
	sseqRecordSELM = 46,
	sseqRecordSELN = 47,
	sseqRecordSELL = 48,
	sseqRecordPREC = 49,
	sseqRecordDLY1 = 50,
	sseqRecordDOL1 = 51,
	sseqRecordDO1 = 52,
	sseqRecordLNK1 = 53,
	sseqRecordSTR1 = 54,
	sseqRecordDT1 = 55,
	sseqRecordLT1 = 56,
	sseqRecordWAIT1 = 57,
	sseqRecordWERR1 = 58,
	sseqRecordWTG1 = 59,
	sseqRecordIX1 = 60,
	sseqRecordDOL1V = 61,
	sseqRecordLNK1V = 62,
	sseqRecordDLY2 = 63,
	sseqRecordDOL2 = 64,
	sseqRecordDO2 = 65,
	sseqRecordLNK2 = 66,
	sseqRecordSTR2 = 67,
	sseqRecordDT2 = 68,
	sseqRecordLT2 = 69,
	sseqRecordWAIT2 = 70,
	sseqRecordWERR2 = 71,
	sseqRecordWTG2 = 72,
	sseqRecordIX2 = 73,
	sseqRecordDOL2V = 74,
	sseqRecordLNK2V = 75,
	sseqRecordDLY3 = 76,
	sseqRecordDOL3 = 77,
	sseqRecordDO3 = 78,
	sseqRecordLNK3 = 79,
	sseqRecordSTR3 = 80,
	sseqRecordDT3 = 81,
	sseqRecordLT3 = 82,
	sseqRecordWAIT3 = 83,
	sseqRecordWERR3 = 84,
	sseqRecordWTG3 = 85,
	sseqRecordIX3 = 86,
	sseqRecordDOL3V = 87,
	sseqRecordLNK3V = 88,
	sseqRecordDLY4 = 89,
	sseqRecordDOL4 = 90,
	sseqRecordDO4 = 91,
	sseqRecordLNK4 = 92,
	sseqRecordSTR4 = 93,
	sseqRecordDT4 = 94,
	sseqRecordLT4 = 95,
	sseqRecordWAIT4 = 96,
	sseqRecordWERR4 = 97,
	sseqRecordWTG4 = 98,
	sseqRecordIX4 = 99,
	sseqRecordDOL4V = 100,
	sseqRecordLNK4V = 101,
	sseqRecordDLY5 = 102,
	sseqRecordDOL5 = 103,
	sseqRecordDO5 = 104,
	sseqRecordLNK5 = 105,
	sseqRecordSTR5 = 106,
	sseqRecordDT5 = 107,
	sseqRecordLT5 = 108,
	sseqRecordWAIT5 = 109,
	sseqRecordWERR5 = 110,
	sseqRecordWTG5 = 111,
	sseqRecordIX5 = 112,
	sseqRecordDOL5V = 113,
	sseqRecordLNK5V = 114,
	sseqRecordDLY6 = 115,
	sseqRecordDOL6 = 116,
	sseqRecordDO6 = 117,
	sseqRecordLNK6 = 118,
	sseqRecordSTR6 = 119,
	sseqRecordDT6 = 120,
	sseqRecordLT6 = 121,
	sseqRecordWAIT6 = 122,
	sseqRecordWERR6 = 123,
	sseqRecordWTG6 = 124,
	sseqRecordIX6 = 125,
	sseqRecordDOL6V = 126,
	sseqRecordLNK6V = 127,
	sseqRecordDLY7 = 128,
	sseqRecordDOL7 = 129,
	sseqRecordDO7 = 130,
	sseqRecordLNK7 = 131,
	sseqRecordSTR7 = 132,
	sseqRecordDT7 = 133,
	sseqRecordLT7 = 134,
	sseqRecordWAIT7 = 135,
	sseqRecordWERR7 = 136,
	sseqRecordWTG7 = 137,
	sseqRecordIX7 = 138,
	sseqRecordDOL7V = 139,
	sseqRecordLNK7V = 140,
	sseqRecordDLY8 = 141,
	sseqRecordDOL8 = 142,
	sseqRecordDO8 = 143,
	sseqRecordLNK8 = 144,
	sseqRecordSTR8 = 145,
	sseqRecordDT8 = 146,
	sseqRecordLT8 = 147,
	sseqRecordWAIT8 = 148,
	sseqRecordWERR8 = 149,
	sseqRecordWTG8 = 150,
	sseqRecordIX8 = 151,
	sseqRecordDOL8V = 152,
	sseqRecordLNK8V = 153,
	sseqRecordDLY9 = 154,
	sseqRecordDOL9 = 155,
	sseqRecordDO9 = 156,
	sseqRecordLNK9 = 157,
	sseqRecordSTR9 = 158,
	sseqRecordDT9 = 159,
	sseqRecordLT9 = 160,
	sseqRecordWAIT9 = 161,
	sseqRecordWERR9 = 162,
	sseqRecordWTG9 = 163,
	sseqRecordIX9 = 164,
	sseqRecordDOL9V = 165,
	sseqRecordLNK9V = 166,
	sseqRecordDLYA = 167,
	sseqRecordDOLA = 168,
	sseqRecordDOA = 169,
	sseqRecordLNKA = 170,
	sseqRecordSTRA = 171,
	sseqRecordDTA = 172,
	sseqRecordLTA = 173,
	sseqRecordWAITA = 174,
	sseqRecordWERRA = 175,
	sseqRecordWTGA = 176,
	sseqRecordIXA = 177,
	sseqRecordDOLAV = 178,
	sseqRecordLNKAV = 179,
	sseqRecordABORT = 180,
	sseqRecordABORTING = 181,
	sseqRecordBUSY = 182
} sseqFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsAssert.h>
#include <epicsExport.h>
#ifdef __cplusplus
extern "C" {
#endif
static int sseqRecordSizeOffset(dbRecordType *prt)
{
    sseqRecord *prec = 0;

    assert(prt->no_fields == 183);
    prt->papFldDes[sseqRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[sseqRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[sseqRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[sseqRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[sseqRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[sseqRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[sseqRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[sseqRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[sseqRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[sseqRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[sseqRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[sseqRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[sseqRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[sseqRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[sseqRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[sseqRecordBKLNK]->size = sizeof(prec->bklnk);
    prt->papFldDes[sseqRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[sseqRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[sseqRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[sseqRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[sseqRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[sseqRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[sseqRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[sseqRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[sseqRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[sseqRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[sseqRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[sseqRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[sseqRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[sseqRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[sseqRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[sseqRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[sseqRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[sseqRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[sseqRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[sseqRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[sseqRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[sseqRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[sseqRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[sseqRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[sseqRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[sseqRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[sseqRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[sseqRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[sseqRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[sseqRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[sseqRecordSELM]->size = sizeof(prec->selm);
    prt->papFldDes[sseqRecordSELN]->size = sizeof(prec->seln);
    prt->papFldDes[sseqRecordSELL]->size = sizeof(prec->sell);
    prt->papFldDes[sseqRecordPREC]->size = sizeof(prec->prec);
    prt->papFldDes[sseqRecordDLY1]->size = sizeof(prec->dly1);
    prt->papFldDes[sseqRecordDOL1]->size = sizeof(prec->dol1);
    prt->papFldDes[sseqRecordDO1]->size = sizeof(prec->do1);
    prt->papFldDes[sseqRecordLNK1]->size = sizeof(prec->lnk1);
    prt->papFldDes[sseqRecordSTR1]->size = sizeof(prec->str1);
    prt->papFldDes[sseqRecordDT1]->size = sizeof(prec->dt1);
    prt->papFldDes[sseqRecordLT1]->size = sizeof(prec->lt1);
    prt->papFldDes[sseqRecordWAIT1]->size = sizeof(prec->wait1);
    prt->papFldDes[sseqRecordWERR1]->size = sizeof(prec->werr1);
    prt->papFldDes[sseqRecordWTG1]->size = sizeof(prec->wtg1);
    prt->papFldDes[sseqRecordIX1]->size = sizeof(prec->ix1);
    prt->papFldDes[sseqRecordDOL1V]->size = sizeof(prec->dol1v);
    prt->papFldDes[sseqRecordLNK1V]->size = sizeof(prec->lnk1v);
    prt->papFldDes[sseqRecordDLY2]->size = sizeof(prec->dly2);
    prt->papFldDes[sseqRecordDOL2]->size = sizeof(prec->dol2);
    prt->papFldDes[sseqRecordDO2]->size = sizeof(prec->do2);
    prt->papFldDes[sseqRecordLNK2]->size = sizeof(prec->lnk2);
    prt->papFldDes[sseqRecordSTR2]->size = sizeof(prec->str2);
    prt->papFldDes[sseqRecordDT2]->size = sizeof(prec->dt2);
    prt->papFldDes[sseqRecordLT2]->size = sizeof(prec->lt2);
    prt->papFldDes[sseqRecordWAIT2]->size = sizeof(prec->wait2);
    prt->papFldDes[sseqRecordWERR2]->size = sizeof(prec->werr2);
    prt->papFldDes[sseqRecordWTG2]->size = sizeof(prec->wtg2);
    prt->papFldDes[sseqRecordIX2]->size = sizeof(prec->ix2);
    prt->papFldDes[sseqRecordDOL2V]->size = sizeof(prec->dol2v);
    prt->papFldDes[sseqRecordLNK2V]->size = sizeof(prec->lnk2v);
    prt->papFldDes[sseqRecordDLY3]->size = sizeof(prec->dly3);
    prt->papFldDes[sseqRecordDOL3]->size = sizeof(prec->dol3);
    prt->papFldDes[sseqRecordDO3]->size = sizeof(prec->do3);
    prt->papFldDes[sseqRecordLNK3]->size = sizeof(prec->lnk3);
    prt->papFldDes[sseqRecordSTR3]->size = sizeof(prec->str3);
    prt->papFldDes[sseqRecordDT3]->size = sizeof(prec->dt3);
    prt->papFldDes[sseqRecordLT3]->size = sizeof(prec->lt3);
    prt->papFldDes[sseqRecordWAIT3]->size = sizeof(prec->wait3);
    prt->papFldDes[sseqRecordWERR3]->size = sizeof(prec->werr3);
    prt->papFldDes[sseqRecordWTG3]->size = sizeof(prec->wtg3);
    prt->papFldDes[sseqRecordIX3]->size = sizeof(prec->ix3);
    prt->papFldDes[sseqRecordDOL3V]->size = sizeof(prec->dol3v);
    prt->papFldDes[sseqRecordLNK3V]->size = sizeof(prec->lnk3v);
    prt->papFldDes[sseqRecordDLY4]->size = sizeof(prec->dly4);
    prt->papFldDes[sseqRecordDOL4]->size = sizeof(prec->dol4);
    prt->papFldDes[sseqRecordDO4]->size = sizeof(prec->do4);
    prt->papFldDes[sseqRecordLNK4]->size = sizeof(prec->lnk4);
    prt->papFldDes[sseqRecordSTR4]->size = sizeof(prec->str4);
    prt->papFldDes[sseqRecordDT4]->size = sizeof(prec->dt4);
    prt->papFldDes[sseqRecordLT4]->size = sizeof(prec->lt4);
    prt->papFldDes[sseqRecordWAIT4]->size = sizeof(prec->wait4);
    prt->papFldDes[sseqRecordWERR4]->size = sizeof(prec->werr4);
    prt->papFldDes[sseqRecordWTG4]->size = sizeof(prec->wtg4);
    prt->papFldDes[sseqRecordIX4]->size = sizeof(prec->ix4);
    prt->papFldDes[sseqRecordDOL4V]->size = sizeof(prec->dol4v);
    prt->papFldDes[sseqRecordLNK4V]->size = sizeof(prec->lnk4v);
    prt->papFldDes[sseqRecordDLY5]->size = sizeof(prec->dly5);
    prt->papFldDes[sseqRecordDOL5]->size = sizeof(prec->dol5);
    prt->papFldDes[sseqRecordDO5]->size = sizeof(prec->do5);
    prt->papFldDes[sseqRecordLNK5]->size = sizeof(prec->lnk5);
    prt->papFldDes[sseqRecordSTR5]->size = sizeof(prec->str5);
    prt->papFldDes[sseqRecordDT5]->size = sizeof(prec->dt5);
    prt->papFldDes[sseqRecordLT5]->size = sizeof(prec->lt5);
    prt->papFldDes[sseqRecordWAIT5]->size = sizeof(prec->wait5);
    prt->papFldDes[sseqRecordWERR5]->size = sizeof(prec->werr5);
    prt->papFldDes[sseqRecordWTG5]->size = sizeof(prec->wtg5);
    prt->papFldDes[sseqRecordIX5]->size = sizeof(prec->ix5);
    prt->papFldDes[sseqRecordDOL5V]->size = sizeof(prec->dol5v);
    prt->papFldDes[sseqRecordLNK5V]->size = sizeof(prec->lnk5v);
    prt->papFldDes[sseqRecordDLY6]->size = sizeof(prec->dly6);
    prt->papFldDes[sseqRecordDOL6]->size = sizeof(prec->dol6);
    prt->papFldDes[sseqRecordDO6]->size = sizeof(prec->do6);
    prt->papFldDes[sseqRecordLNK6]->size = sizeof(prec->lnk6);
    prt->papFldDes[sseqRecordSTR6]->size = sizeof(prec->str6);
    prt->papFldDes[sseqRecordDT6]->size = sizeof(prec->dt6);
    prt->papFldDes[sseqRecordLT6]->size = sizeof(prec->lt6);
    prt->papFldDes[sseqRecordWAIT6]->size = sizeof(prec->wait6);
    prt->papFldDes[sseqRecordWERR6]->size = sizeof(prec->werr6);
    prt->papFldDes[sseqRecordWTG6]->size = sizeof(prec->wtg6);
    prt->papFldDes[sseqRecordIX6]->size = sizeof(prec->ix6);
    prt->papFldDes[sseqRecordDOL6V]->size = sizeof(prec->dol6v);
    prt->papFldDes[sseqRecordLNK6V]->size = sizeof(prec->lnk6v);
    prt->papFldDes[sseqRecordDLY7]->size = sizeof(prec->dly7);
    prt->papFldDes[sseqRecordDOL7]->size = sizeof(prec->dol7);
    prt->papFldDes[sseqRecordDO7]->size = sizeof(prec->do7);
    prt->papFldDes[sseqRecordLNK7]->size = sizeof(prec->lnk7);
    prt->papFldDes[sseqRecordSTR7]->size = sizeof(prec->str7);
    prt->papFldDes[sseqRecordDT7]->size = sizeof(prec->dt7);
    prt->papFldDes[sseqRecordLT7]->size = sizeof(prec->lt7);
    prt->papFldDes[sseqRecordWAIT7]->size = sizeof(prec->wait7);
    prt->papFldDes[sseqRecordWERR7]->size = sizeof(prec->werr7);
    prt->papFldDes[sseqRecordWTG7]->size = sizeof(prec->wtg7);
    prt->papFldDes[sseqRecordIX7]->size = sizeof(prec->ix7);
    prt->papFldDes[sseqRecordDOL7V]->size = sizeof(prec->dol7v);
    prt->papFldDes[sseqRecordLNK7V]->size = sizeof(prec->lnk7v);
    prt->papFldDes[sseqRecordDLY8]->size = sizeof(prec->dly8);
    prt->papFldDes[sseqRecordDOL8]->size = sizeof(prec->dol8);
    prt->papFldDes[sseqRecordDO8]->size = sizeof(prec->do8);
    prt->papFldDes[sseqRecordLNK8]->size = sizeof(prec->lnk8);
    prt->papFldDes[sseqRecordSTR8]->size = sizeof(prec->str8);
    prt->papFldDes[sseqRecordDT8]->size = sizeof(prec->dt8);
    prt->papFldDes[sseqRecordLT8]->size = sizeof(prec->lt8);
    prt->papFldDes[sseqRecordWAIT8]->size = sizeof(prec->wait8);
    prt->papFldDes[sseqRecordWERR8]->size = sizeof(prec->werr8);
    prt->papFldDes[sseqRecordWTG8]->size = sizeof(prec->wtg8);
    prt->papFldDes[sseqRecordIX8]->size = sizeof(prec->ix8);
    prt->papFldDes[sseqRecordDOL8V]->size = sizeof(prec->dol8v);
    prt->papFldDes[sseqRecordLNK8V]->size = sizeof(prec->lnk8v);
    prt->papFldDes[sseqRecordDLY9]->size = sizeof(prec->dly9);
    prt->papFldDes[sseqRecordDOL9]->size = sizeof(prec->dol9);
    prt->papFldDes[sseqRecordDO9]->size = sizeof(prec->do9);
    prt->papFldDes[sseqRecordLNK9]->size = sizeof(prec->lnk9);
    prt->papFldDes[sseqRecordSTR9]->size = sizeof(prec->str9);
    prt->papFldDes[sseqRecordDT9]->size = sizeof(prec->dt9);
    prt->papFldDes[sseqRecordLT9]->size = sizeof(prec->lt9);
    prt->papFldDes[sseqRecordWAIT9]->size = sizeof(prec->wait9);
    prt->papFldDes[sseqRecordWERR9]->size = sizeof(prec->werr9);
    prt->papFldDes[sseqRecordWTG9]->size = sizeof(prec->wtg9);
    prt->papFldDes[sseqRecordIX9]->size = sizeof(prec->ix9);
    prt->papFldDes[sseqRecordDOL9V]->size = sizeof(prec->dol9v);
    prt->papFldDes[sseqRecordLNK9V]->size = sizeof(prec->lnk9v);
    prt->papFldDes[sseqRecordDLYA]->size = sizeof(prec->dlya);
    prt->papFldDes[sseqRecordDOLA]->size = sizeof(prec->dola);
    prt->papFldDes[sseqRecordDOA]->size = sizeof(prec->doa);
    prt->papFldDes[sseqRecordLNKA]->size = sizeof(prec->lnka);
    prt->papFldDes[sseqRecordSTRA]->size = sizeof(prec->stra);
    prt->papFldDes[sseqRecordDTA]->size = sizeof(prec->dta);
    prt->papFldDes[sseqRecordLTA]->size = sizeof(prec->lta);
    prt->papFldDes[sseqRecordWAITA]->size = sizeof(prec->waita);
    prt->papFldDes[sseqRecordWERRA]->size = sizeof(prec->werra);
    prt->papFldDes[sseqRecordWTGA]->size = sizeof(prec->wtga);
    prt->papFldDes[sseqRecordIXA]->size = sizeof(prec->ixa);
    prt->papFldDes[sseqRecordDOLAV]->size = sizeof(prec->dolav);
    prt->papFldDes[sseqRecordLNKAV]->size = sizeof(prec->lnkav);
    prt->papFldDes[sseqRecordABORT]->size = sizeof(prec->abort);
    prt->papFldDes[sseqRecordABORTING]->size = sizeof(prec->aborting);
    prt->papFldDes[sseqRecordBUSY]->size = sizeof(prec->busy);
    prt->papFldDes[sseqRecordNAME]->offset = (unsigned short)((char *)&prec->name - (char *)prec);
    prt->papFldDes[sseqRecordDESC]->offset = (unsigned short)((char *)&prec->desc - (char *)prec);
    prt->papFldDes[sseqRecordASG]->offset = (unsigned short)((char *)&prec->asg - (char *)prec);
    prt->papFldDes[sseqRecordSCAN]->offset = (unsigned short)((char *)&prec->scan - (char *)prec);
    prt->papFldDes[sseqRecordPINI]->offset = (unsigned short)((char *)&prec->pini - (char *)prec);
    prt->papFldDes[sseqRecordPHAS]->offset = (unsigned short)((char *)&prec->phas - (char *)prec);
    prt->papFldDes[sseqRecordEVNT]->offset = (unsigned short)((char *)&prec->evnt - (char *)prec);
    prt->papFldDes[sseqRecordTSE]->offset = (unsigned short)((char *)&prec->tse - (char *)prec);
    prt->papFldDes[sseqRecordTSEL]->offset = (unsigned short)((char *)&prec->tsel - (char *)prec);
    prt->papFldDes[sseqRecordDTYP]->offset = (unsigned short)((char *)&prec->dtyp - (char *)prec);
    prt->papFldDes[sseqRecordDISV]->offset = (unsigned short)((char *)&prec->disv - (char *)prec);
    prt->papFldDes[sseqRecordDISA]->offset = (unsigned short)((char *)&prec->disa - (char *)prec);
    prt->papFldDes[sseqRecordSDIS]->offset = (unsigned short)((char *)&prec->sdis - (char *)prec);
    prt->papFldDes[sseqRecordMLOK]->offset = (unsigned short)((char *)&prec->mlok - (char *)prec);
    prt->papFldDes[sseqRecordMLIS]->offset = (unsigned short)((char *)&prec->mlis - (char *)prec);
    prt->papFldDes[sseqRecordBKLNK]->offset = (unsigned short)((char *)&prec->bklnk - (char *)prec);
    prt->papFldDes[sseqRecordDISP]->offset = (unsigned short)((char *)&prec->disp - (char *)prec);
    prt->papFldDes[sseqRecordPROC]->offset = (unsigned short)((char *)&prec->proc - (char *)prec);
    prt->papFldDes[sseqRecordSTAT]->offset = (unsigned short)((char *)&prec->stat - (char *)prec);
    prt->papFldDes[sseqRecordSEVR]->offset = (unsigned short)((char *)&prec->sevr - (char *)prec);
    prt->papFldDes[sseqRecordNSTA]->offset = (unsigned short)((char *)&prec->nsta - (char *)prec);
    prt->papFldDes[sseqRecordNSEV]->offset = (unsigned short)((char *)&prec->nsev - (char *)prec);
    prt->papFldDes[sseqRecordACKS]->offset = (unsigned short)((char *)&prec->acks - (char *)prec);
    prt->papFldDes[sseqRecordACKT]->offset = (unsigned short)((char *)&prec->ackt - (char *)prec);
    prt->papFldDes[sseqRecordDISS]->offset = (unsigned short)((char *)&prec->diss - (char *)prec);
    prt->papFldDes[sseqRecordLCNT]->offset = (unsigned short)((char *)&prec->lcnt - (char *)prec);
    prt->papFldDes[sseqRecordPACT]->offset = (unsigned short)((char *)&prec->pact - (char *)prec);
    prt->papFldDes[sseqRecordPUTF]->offset = (unsigned short)((char *)&prec->putf - (char *)prec);
    prt->papFldDes[sseqRecordRPRO]->offset = (unsigned short)((char *)&prec->rpro - (char *)prec);
    prt->papFldDes[sseqRecordASP]->offset = (unsigned short)((char *)&prec->asp - (char *)prec);
    prt->papFldDes[sseqRecordPPN]->offset = (unsigned short)((char *)&prec->ppn - (char *)prec);
    prt->papFldDes[sseqRecordPPNR]->offset = (unsigned short)((char *)&prec->ppnr - (char *)prec);
    prt->papFldDes[sseqRecordSPVT]->offset = (unsigned short)((char *)&prec->spvt - (char *)prec);
    prt->papFldDes[sseqRecordRSET]->offset = (unsigned short)((char *)&prec->rset - (char *)prec);
    prt->papFldDes[sseqRecordDSET]->offset = (unsigned short)((char *)&prec->dset - (char *)prec);
    prt->papFldDes[sseqRecordDPVT]->offset = (unsigned short)((char *)&prec->dpvt - (char *)prec);
    prt->papFldDes[sseqRecordRDES]->offset = (unsigned short)((char *)&prec->rdes - (char *)prec);
    prt->papFldDes[sseqRecordLSET]->offset = (unsigned short)((char *)&prec->lset - (char *)prec);
    prt->papFldDes[sseqRecordPRIO]->offset = (unsigned short)((char *)&prec->prio - (char *)prec);
    prt->papFldDes[sseqRecordTPRO]->offset = (unsigned short)((char *)&prec->tpro - (char *)prec);
    prt->papFldDes[sseqRecordBKPT]->offset = (unsigned short)((char *)&prec->bkpt - (char *)prec);
    prt->papFldDes[sseqRecordUDF]->offset = (unsigned short)((char *)&prec->udf - (char *)prec);
    prt->papFldDes[sseqRecordUDFS]->offset = (unsigned short)((char *)&prec->udfs - (char *)prec);
    prt->papFldDes[sseqRecordTIME]->offset = (unsigned short)((char *)&prec->time - (char *)prec);
    prt->papFldDes[sseqRecordFLNK]->offset = (unsigned short)((char *)&prec->flnk - (char *)prec);
    prt->papFldDes[sseqRecordVAL]->offset = (unsigned short)((char *)&prec->val - (char *)prec);
    prt->papFldDes[sseqRecordSELM]->offset = (unsigned short)((char *)&prec->selm - (char *)prec);
    prt->papFldDes[sseqRecordSELN]->offset = (unsigned short)((char *)&prec->seln - (char *)prec);
    prt->papFldDes[sseqRecordSELL]->offset = (unsigned short)((char *)&prec->sell - (char *)prec);
    prt->papFldDes[sseqRecordPREC]->offset = (unsigned short)((char *)&prec->prec - (char *)prec);
    prt->papFldDes[sseqRecordDLY1]->offset = (unsigned short)((char *)&prec->dly1 - (char *)prec);
    prt->papFldDes[sseqRecordDOL1]->offset = (unsigned short)((char *)&prec->dol1 - (char *)prec);
    prt->papFldDes[sseqRecordDO1]->offset = (unsigned short)((char *)&prec->do1 - (char *)prec);
    prt->papFldDes[sseqRecordLNK1]->offset = (unsigned short)((char *)&prec->lnk1 - (char *)prec);
    prt->papFldDes[sseqRecordSTR1]->offset = (unsigned short)((char *)&prec->str1 - (char *)prec);
    prt->papFldDes[sseqRecordDT1]->offset = (unsigned short)((char *)&prec->dt1 - (char *)prec);
    prt->papFldDes[sseqRecordLT1]->offset = (unsigned short)((char *)&prec->lt1 - (char *)prec);
    prt->papFldDes[sseqRecordWAIT1]->offset = (unsigned short)((char *)&prec->wait1 - (char *)prec);
    prt->papFldDes[sseqRecordWERR1]->offset = (unsigned short)((char *)&prec->werr1 - (char *)prec);
    prt->papFldDes[sseqRecordWTG1]->offset = (unsigned short)((char *)&prec->wtg1 - (char *)prec);
    prt->papFldDes[sseqRecordIX1]->offset = (unsigned short)((char *)&prec->ix1 - (char *)prec);
    prt->papFldDes[sseqRecordDOL1V]->offset = (unsigned short)((char *)&prec->dol1v - (char *)prec);
    prt->papFldDes[sseqRecordLNK1V]->offset = (unsigned short)((char *)&prec->lnk1v - (char *)prec);
    prt->papFldDes[sseqRecordDLY2]->offset = (unsigned short)((char *)&prec->dly2 - (char *)prec);
    prt->papFldDes[sseqRecordDOL2]->offset = (unsigned short)((char *)&prec->dol2 - (char *)prec);
    prt->papFldDes[sseqRecordDO2]->offset = (unsigned short)((char *)&prec->do2 - (char *)prec);
    prt->papFldDes[sseqRecordLNK2]->offset = (unsigned short)((char *)&prec->lnk2 - (char *)prec);
    prt->papFldDes[sseqRecordSTR2]->offset = (unsigned short)((char *)&prec->str2 - (char *)prec);
    prt->papFldDes[sseqRecordDT2]->offset = (unsigned short)((char *)&prec->dt2 - (char *)prec);
    prt->papFldDes[sseqRecordLT2]->offset = (unsigned short)((char *)&prec->lt2 - (char *)prec);
    prt->papFldDes[sseqRecordWAIT2]->offset = (unsigned short)((char *)&prec->wait2 - (char *)prec);
    prt->papFldDes[sseqRecordWERR2]->offset = (unsigned short)((char *)&prec->werr2 - (char *)prec);
    prt->papFldDes[sseqRecordWTG2]->offset = (unsigned short)((char *)&prec->wtg2 - (char *)prec);
    prt->papFldDes[sseqRecordIX2]->offset = (unsigned short)((char *)&prec->ix2 - (char *)prec);
    prt->papFldDes[sseqRecordDOL2V]->offset = (unsigned short)((char *)&prec->dol2v - (char *)prec);
    prt->papFldDes[sseqRecordLNK2V]->offset = (unsigned short)((char *)&prec->lnk2v - (char *)prec);
    prt->papFldDes[sseqRecordDLY3]->offset = (unsigned short)((char *)&prec->dly3 - (char *)prec);
    prt->papFldDes[sseqRecordDOL3]->offset = (unsigned short)((char *)&prec->dol3 - (char *)prec);
    prt->papFldDes[sseqRecordDO3]->offset = (unsigned short)((char *)&prec->do3 - (char *)prec);
    prt->papFldDes[sseqRecordLNK3]->offset = (unsigned short)((char *)&prec->lnk3 - (char *)prec);
    prt->papFldDes[sseqRecordSTR3]->offset = (unsigned short)((char *)&prec->str3 - (char *)prec);
    prt->papFldDes[sseqRecordDT3]->offset = (unsigned short)((char *)&prec->dt3 - (char *)prec);
    prt->papFldDes[sseqRecordLT3]->offset = (unsigned short)((char *)&prec->lt3 - (char *)prec);
    prt->papFldDes[sseqRecordWAIT3]->offset = (unsigned short)((char *)&prec->wait3 - (char *)prec);
    prt->papFldDes[sseqRecordWERR3]->offset = (unsigned short)((char *)&prec->werr3 - (char *)prec);
    prt->papFldDes[sseqRecordWTG3]->offset = (unsigned short)((char *)&prec->wtg3 - (char *)prec);
    prt->papFldDes[sseqRecordIX3]->offset = (unsigned short)((char *)&prec->ix3 - (char *)prec);
    prt->papFldDes[sseqRecordDOL3V]->offset = (unsigned short)((char *)&prec->dol3v - (char *)prec);
    prt->papFldDes[sseqRecordLNK3V]->offset = (unsigned short)((char *)&prec->lnk3v - (char *)prec);
    prt->papFldDes[sseqRecordDLY4]->offset = (unsigned short)((char *)&prec->dly4 - (char *)prec);
    prt->papFldDes[sseqRecordDOL4]->offset = (unsigned short)((char *)&prec->dol4 - (char *)prec);
    prt->papFldDes[sseqRecordDO4]->offset = (unsigned short)((char *)&prec->do4 - (char *)prec);
    prt->papFldDes[sseqRecordLNK4]->offset = (unsigned short)((char *)&prec->lnk4 - (char *)prec);
    prt->papFldDes[sseqRecordSTR4]->offset = (unsigned short)((char *)&prec->str4 - (char *)prec);
    prt->papFldDes[sseqRecordDT4]->offset = (unsigned short)((char *)&prec->dt4 - (char *)prec);
    prt->papFldDes[sseqRecordLT4]->offset = (unsigned short)((char *)&prec->lt4 - (char *)prec);
    prt->papFldDes[sseqRecordWAIT4]->offset = (unsigned short)((char *)&prec->wait4 - (char *)prec);
    prt->papFldDes[sseqRecordWERR4]->offset = (unsigned short)((char *)&prec->werr4 - (char *)prec);
    prt->papFldDes[sseqRecordWTG4]->offset = (unsigned short)((char *)&prec->wtg4 - (char *)prec);
    prt->papFldDes[sseqRecordIX4]->offset = (unsigned short)((char *)&prec->ix4 - (char *)prec);
    prt->papFldDes[sseqRecordDOL4V]->offset = (unsigned short)((char *)&prec->dol4v - (char *)prec);
    prt->papFldDes[sseqRecordLNK4V]->offset = (unsigned short)((char *)&prec->lnk4v - (char *)prec);
    prt->papFldDes[sseqRecordDLY5]->offset = (unsigned short)((char *)&prec->dly5 - (char *)prec);
    prt->papFldDes[sseqRecordDOL5]->offset = (unsigned short)((char *)&prec->dol5 - (char *)prec);
    prt->papFldDes[sseqRecordDO5]->offset = (unsigned short)((char *)&prec->do5 - (char *)prec);
    prt->papFldDes[sseqRecordLNK5]->offset = (unsigned short)((char *)&prec->lnk5 - (char *)prec);
    prt->papFldDes[sseqRecordSTR5]->offset = (unsigned short)((char *)&prec->str5 - (char *)prec);
    prt->papFldDes[sseqRecordDT5]->offset = (unsigned short)((char *)&prec->dt5 - (char *)prec);
    prt->papFldDes[sseqRecordLT5]->offset = (unsigned short)((char *)&prec->lt5 - (char *)prec);
    prt->papFldDes[sseqRecordWAIT5]->offset = (unsigned short)((char *)&prec->wait5 - (char *)prec);
    prt->papFldDes[sseqRecordWERR5]->offset = (unsigned short)((char *)&prec->werr5 - (char *)prec);
    prt->papFldDes[sseqRecordWTG5]->offset = (unsigned short)((char *)&prec->wtg5 - (char *)prec);
    prt->papFldDes[sseqRecordIX5]->offset = (unsigned short)((char *)&prec->ix5 - (char *)prec);
    prt->papFldDes[sseqRecordDOL5V]->offset = (unsigned short)((char *)&prec->dol5v - (char *)prec);
    prt->papFldDes[sseqRecordLNK5V]->offset = (unsigned short)((char *)&prec->lnk5v - (char *)prec);
    prt->papFldDes[sseqRecordDLY6]->offset = (unsigned short)((char *)&prec->dly6 - (char *)prec);
    prt->papFldDes[sseqRecordDOL6]->offset = (unsigned short)((char *)&prec->dol6 - (char *)prec);
    prt->papFldDes[sseqRecordDO6]->offset = (unsigned short)((char *)&prec->do6 - (char *)prec);
    prt->papFldDes[sseqRecordLNK6]->offset = (unsigned short)((char *)&prec->lnk6 - (char *)prec);
    prt->papFldDes[sseqRecordSTR6]->offset = (unsigned short)((char *)&prec->str6 - (char *)prec);
    prt->papFldDes[sseqRecordDT6]->offset = (unsigned short)((char *)&prec->dt6 - (char *)prec);
    prt->papFldDes[sseqRecordLT6]->offset = (unsigned short)((char *)&prec->lt6 - (char *)prec);
    prt->papFldDes[sseqRecordWAIT6]->offset = (unsigned short)((char *)&prec->wait6 - (char *)prec);
    prt->papFldDes[sseqRecordWERR6]->offset = (unsigned short)((char *)&prec->werr6 - (char *)prec);
    prt->papFldDes[sseqRecordWTG6]->offset = (unsigned short)((char *)&prec->wtg6 - (char *)prec);
    prt->papFldDes[sseqRecordIX6]->offset = (unsigned short)((char *)&prec->ix6 - (char *)prec);
    prt->papFldDes[sseqRecordDOL6V]->offset = (unsigned short)((char *)&prec->dol6v - (char *)prec);
    prt->papFldDes[sseqRecordLNK6V]->offset = (unsigned short)((char *)&prec->lnk6v - (char *)prec);
    prt->papFldDes[sseqRecordDLY7]->offset = (unsigned short)((char *)&prec->dly7 - (char *)prec);
    prt->papFldDes[sseqRecordDOL7]->offset = (unsigned short)((char *)&prec->dol7 - (char *)prec);
    prt->papFldDes[sseqRecordDO7]->offset = (unsigned short)((char *)&prec->do7 - (char *)prec);
    prt->papFldDes[sseqRecordLNK7]->offset = (unsigned short)((char *)&prec->lnk7 - (char *)prec);
    prt->papFldDes[sseqRecordSTR7]->offset = (unsigned short)((char *)&prec->str7 - (char *)prec);
    prt->papFldDes[sseqRecordDT7]->offset = (unsigned short)((char *)&prec->dt7 - (char *)prec);
    prt->papFldDes[sseqRecordLT7]->offset = (unsigned short)((char *)&prec->lt7 - (char *)prec);
    prt->papFldDes[sseqRecordWAIT7]->offset = (unsigned short)((char *)&prec->wait7 - (char *)prec);
    prt->papFldDes[sseqRecordWERR7]->offset = (unsigned short)((char *)&prec->werr7 - (char *)prec);
    prt->papFldDes[sseqRecordWTG7]->offset = (unsigned short)((char *)&prec->wtg7 - (char *)prec);
    prt->papFldDes[sseqRecordIX7]->offset = (unsigned short)((char *)&prec->ix7 - (char *)prec);
    prt->papFldDes[sseqRecordDOL7V]->offset = (unsigned short)((char *)&prec->dol7v - (char *)prec);
    prt->papFldDes[sseqRecordLNK7V]->offset = (unsigned short)((char *)&prec->lnk7v - (char *)prec);
    prt->papFldDes[sseqRecordDLY8]->offset = (unsigned short)((char *)&prec->dly8 - (char *)prec);
    prt->papFldDes[sseqRecordDOL8]->offset = (unsigned short)((char *)&prec->dol8 - (char *)prec);
    prt->papFldDes[sseqRecordDO8]->offset = (unsigned short)((char *)&prec->do8 - (char *)prec);
    prt->papFldDes[sseqRecordLNK8]->offset = (unsigned short)((char *)&prec->lnk8 - (char *)prec);
    prt->papFldDes[sseqRecordSTR8]->offset = (unsigned short)((char *)&prec->str8 - (char *)prec);
    prt->papFldDes[sseqRecordDT8]->offset = (unsigned short)((char *)&prec->dt8 - (char *)prec);
    prt->papFldDes[sseqRecordLT8]->offset = (unsigned short)((char *)&prec->lt8 - (char *)prec);
    prt->papFldDes[sseqRecordWAIT8]->offset = (unsigned short)((char *)&prec->wait8 - (char *)prec);
    prt->papFldDes[sseqRecordWERR8]->offset = (unsigned short)((char *)&prec->werr8 - (char *)prec);
    prt->papFldDes[sseqRecordWTG8]->offset = (unsigned short)((char *)&prec->wtg8 - (char *)prec);
    prt->papFldDes[sseqRecordIX8]->offset = (unsigned short)((char *)&prec->ix8 - (char *)prec);
    prt->papFldDes[sseqRecordDOL8V]->offset = (unsigned short)((char *)&prec->dol8v - (char *)prec);
    prt->papFldDes[sseqRecordLNK8V]->offset = (unsigned short)((char *)&prec->lnk8v - (char *)prec);
    prt->papFldDes[sseqRecordDLY9]->offset = (unsigned short)((char *)&prec->dly9 - (char *)prec);
    prt->papFldDes[sseqRecordDOL9]->offset = (unsigned short)((char *)&prec->dol9 - (char *)prec);
    prt->papFldDes[sseqRecordDO9]->offset = (unsigned short)((char *)&prec->do9 - (char *)prec);
    prt->papFldDes[sseqRecordLNK9]->offset = (unsigned short)((char *)&prec->lnk9 - (char *)prec);
    prt->papFldDes[sseqRecordSTR9]->offset = (unsigned short)((char *)&prec->str9 - (char *)prec);
    prt->papFldDes[sseqRecordDT9]->offset = (unsigned short)((char *)&prec->dt9 - (char *)prec);
    prt->papFldDes[sseqRecordLT9]->offset = (unsigned short)((char *)&prec->lt9 - (char *)prec);
    prt->papFldDes[sseqRecordWAIT9]->offset = (unsigned short)((char *)&prec->wait9 - (char *)prec);
    prt->papFldDes[sseqRecordWERR9]->offset = (unsigned short)((char *)&prec->werr9 - (char *)prec);
    prt->papFldDes[sseqRecordWTG9]->offset = (unsigned short)((char *)&prec->wtg9 - (char *)prec);
    prt->papFldDes[sseqRecordIX9]->offset = (unsigned short)((char *)&prec->ix9 - (char *)prec);
    prt->papFldDes[sseqRecordDOL9V]->offset = (unsigned short)((char *)&prec->dol9v - (char *)prec);
    prt->papFldDes[sseqRecordLNK9V]->offset = (unsigned short)((char *)&prec->lnk9v - (char *)prec);
    prt->papFldDes[sseqRecordDLYA]->offset = (unsigned short)((char *)&prec->dlya - (char *)prec);
    prt->papFldDes[sseqRecordDOLA]->offset = (unsigned short)((char *)&prec->dola - (char *)prec);
    prt->papFldDes[sseqRecordDOA]->offset = (unsigned short)((char *)&prec->doa - (char *)prec);
    prt->papFldDes[sseqRecordLNKA]->offset = (unsigned short)((char *)&prec->lnka - (char *)prec);
    prt->papFldDes[sseqRecordSTRA]->offset = (unsigned short)((char *)&prec->stra - (char *)prec);
    prt->papFldDes[sseqRecordDTA]->offset = (unsigned short)((char *)&prec->dta - (char *)prec);
    prt->papFldDes[sseqRecordLTA]->offset = (unsigned short)((char *)&prec->lta - (char *)prec);
    prt->papFldDes[sseqRecordWAITA]->offset = (unsigned short)((char *)&prec->waita - (char *)prec);
    prt->papFldDes[sseqRecordWERRA]->offset = (unsigned short)((char *)&prec->werra - (char *)prec);
    prt->papFldDes[sseqRecordWTGA]->offset = (unsigned short)((char *)&prec->wtga - (char *)prec);
    prt->papFldDes[sseqRecordIXA]->offset = (unsigned short)((char *)&prec->ixa - (char *)prec);
    prt->papFldDes[sseqRecordDOLAV]->offset = (unsigned short)((char *)&prec->dolav - (char *)prec);
    prt->papFldDes[sseqRecordLNKAV]->offset = (unsigned short)((char *)&prec->lnkav - (char *)prec);
    prt->papFldDes[sseqRecordABORT]->offset = (unsigned short)((char *)&prec->abort - (char *)prec);
    prt->papFldDes[sseqRecordABORTING]->offset = (unsigned short)((char *)&prec->aborting - (char *)prec);
    prt->papFldDes[sseqRecordBUSY]->offset = (unsigned short)((char *)&prec->busy - (char *)prec);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(sseqRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_sseqRecord_H */
