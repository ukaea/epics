/* aCalcoutRecord.h generated from aCalcoutRecord.dbd */

#ifndef INC_aCalcoutRecord_H
#define INC_aCalcoutRecord_H

#include "epicsTypes.h"
#include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "epicsTime.h"

#ifndef acalcoutINAP_NUM_CHOICES
typedef enum {
    acalcoutINAP_No                 /* No PROC on Change */,
    acalcoutINAP_Yes                /* PROC on Change */
} acalcoutINAP;
#define acalcoutINAP_NUM_CHOICES 2
#endif

#ifndef acalcoutINAV_NUM_CHOICES
typedef enum {
    acalcoutINAV_EXT_NC             /* Ext PV NC */,
    acalcoutINAV_EXT                /* Ext PV OK */,
    acalcoutINAV_LOC                /* Local PV */,
    acalcoutINAV_CON                /* Constant */
} acalcoutINAV;
#define acalcoutINAV_NUM_CHOICES 4
#endif

#ifndef acalcoutSIZE_NUM_CHOICES
typedef enum {
    acalcoutSIZE_NELM               /* NELM */,
    acalcoutSIZE_NUSE               /* NUSE */
} acalcoutSIZE;
#define acalcoutSIZE_NUM_CHOICES 2
#endif

#ifndef acalcoutDOPT_NUM_CHOICES
typedef enum {
    acalcoutDOPT_Use_VAL            /* Use CALC */,
    acalcoutDOPT_Use_OVAL           /* Use OCAL */
} acalcoutDOPT;
#define acalcoutDOPT_NUM_CHOICES 2
#endif

#ifndef acalcoutWAIT_NUM_CHOICES
typedef enum {
    acalcoutWAIT_NoWait             /* NoWait */,
    acalcoutWAIT_Wait               /* Wait */
} acalcoutWAIT;
#define acalcoutWAIT_NUM_CHOICES 2
#endif

#ifndef acalcoutOOPT_NUM_CHOICES
typedef enum {
    acalcoutOOPT_Every_Time         /* Every Time */,
    acalcoutOOPT_On_Change          /* On Change */,
    acalcoutOOPT_When_Zero          /* When Zero */,
    acalcoutOOPT_When_Non_zero      /* When Non-zero */,
    acalcoutOOPT_Transition_To_Zero /* Transition To Zero */,
    acalcoutOOPT_Transition_To_Non_zero /* Transition To Non-zero */,
    acalcoutOOPT_Never              /* Never */
} acalcoutOOPT;
#define acalcoutOOPT_NUM_CHOICES 7
#endif

typedef struct acalcoutRecord {
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
    epicsFloat64        vers;       /* Code Version */
    void *rpvt;                     /* Record Private */
    epicsFloat64        val;        /* Result */
    double *aval;                   /* Array Value */
    epicsUInt32         nelm;       /* Number of Elements */
    epicsUInt32         nuse;       /* # elem's in use */
    epicsFloat64        pval;       /* Previous Value */
    double *pavl;                   /* Prev. Array Value */
    char                calc[80];   /* Calculation */
    epicsInt32          clcv;       /* CALC Valid */
    DBLINK              inpa;       /* Input A */
    DBLINK              inpb;       /* Input B */
    DBLINK              inpc;       /* Input C */
    DBLINK              inpd;       /* Input D */
    DBLINK              inpe;       /* Input E */
    DBLINK              inpf;       /* Input F */
    DBLINK              inpg;       /* Input G */
    DBLINK              inph;       /* Input H */
    DBLINK              inpi;       /* Input I */
    DBLINK              inpj;       /* Input J */
    DBLINK              inpk;       /* Input K */
    DBLINK              inpl;       /* Input L */
    DBLINK              inaa;       /* Array input AA */
    DBLINK              inbb;       /* Array input BB */
    DBLINK              incc;       /* Array input CC */
    DBLINK              indd;       /* Array input DD */
    DBLINK              inee;       /* Array input EE */
    DBLINK              inff;       /* Array input FF */
    DBLINK              ingg;       /* Array input GG */
    DBLINK              inhh;       /* Array input HH */
    DBLINK              inii;       /* Array input II */
    DBLINK              injj;       /* Array input JJ */
    DBLINK              inkk;       /* Array input KK */
    DBLINK              inll;       /* Array input LL */
    DBLINK              out;        /* Output Link */
    epicsEnum16         inav;       /* INPA PV Status */
    epicsEnum16         inbv;       /* INPB PV Status */
    epicsEnum16         incv;       /* INPC PV Status */
    epicsEnum16         indv;       /* INPD PV Status */
    epicsEnum16         inev;       /* INPE PV Status */
    epicsEnum16         infv;       /* INPF PV Status */
    epicsEnum16         ingv;       /* INPG PV Status */
    epicsEnum16         inhv;       /* INPH PV Status */
    epicsEnum16         iniv;       /* INPI PV Status */
    epicsEnum16         injv;       /* INPJ PV Status */
    epicsEnum16         inkv;       /* INPK PV Status */
    epicsEnum16         inlv;       /* INPL PV Status */
    epicsEnum16         iaav;       /* INAA PV Status */
    epicsEnum16         ibbv;       /* INBB PV Status */
    epicsEnum16         iccv;       /* INCC PV Status */
    epicsEnum16         iddv;       /* INDD PV Status */
    epicsEnum16         ieev;       /* INEE PV Status */
    epicsEnum16         iffv;       /* INFF PV Status */
    epicsEnum16         iggv;       /* INGG PV Status */
    epicsEnum16         ihhv;       /* INHH PV Status */
    epicsEnum16         iiiv;       /* INII PV Status */
    epicsEnum16         ijjv;       /* INJJ PV Status */
    epicsEnum16         ikkv;       /* INKK PV Status */
    epicsEnum16         illv;       /* INLL PV Status */
    epicsEnum16         outv;       /* OUT PV Status */
    epicsEnum16         oopt;       /* Output Execute Opt */
    epicsFloat64        odly;       /* Output Execute Delay */
    epicsEnum16         wait;       /* Wait for completion? */
    epicsUInt16         dlya;       /* Output Delay Active */
    epicsEnum16         dopt;       /* Output Data Opt */
    char                ocal[80];   /* Output Calculation */
    epicsInt32          oclv;       /* OCAL Valid */
    epicsUInt16         oevt;       /* Event To Issue */
    epicsEnum16         ivoa;       /* INVALID output action */
    epicsFloat64        ivov;       /* INVALID output value */
    char                egu[16];    /* Units Name */
    epicsInt16          prec;       /* Display Precision */
    epicsFloat64        hopr;       /* High Operating Rng */
    epicsFloat64        lopr;       /* Low Operating Range */
    epicsFloat64        hihi;       /* Hihi Alarm Limit */
    epicsFloat64        lolo;       /* Lolo Alarm Limit */
    epicsFloat64        high;       /* High Alarm Limit */
    epicsFloat64        low;        /* Low Alarm Limit */
    epicsEnum16         hhsv;       /* Hihi Severity */
    epicsEnum16         llsv;       /* Lolo Severity */
    epicsEnum16         hsv;        /* High Severity */
    epicsEnum16         lsv;        /* Low Severity */
    epicsFloat64        hyst;       /* Alarm Deadband */
    epicsFloat64        adel;       /* Archive Deadband */
    epicsFloat64        mdel;       /* Monitor Deadband */
    epicsFloat64        a;          /* Value of Input A */
    epicsFloat64        b;          /* Value of Input B */
    epicsFloat64        c;          /* Value of Input C */
    epicsFloat64        d;          /* Value of Input D */
    epicsFloat64        e;          /* Value of Input E */
    epicsFloat64        f;          /* Value of Input F */
    epicsFloat64        g;          /* Value of Input G */
    epicsFloat64        h;          /* Value of Input H */
    epicsFloat64        i;          /* Value of Input I */
    epicsFloat64        j;          /* Value of Input J */
    epicsFloat64        k;          /* Value of Input K */
    epicsFloat64        l;          /* Value of Input L */
    double *aa;                     /* Value of array input */
    double *bb;                     /* Value of array input */
    double *cc;                     /* Value of array input */
    double *dd;                     /* Value of array input */
    double *ee;                     /* Value of array input */
    double *ff;                     /* Value of array input */
    double *gg;                     /* Value of array input */
    double *hh;                     /* Value of array input */
    double *ii;                     /* Value of array input */
    double *jj;                     /* Value of array input */
    double *kk;                     /* Value of array input */
    double *ll;                     /* Value of array input */
    double *paa;                    /* Prev Val of array input */
    epicsUInt32         newm;       /* new array value mask */
    epicsFloat64        oval;       /* Output Value */
    double *oav;                    /* Output array value */
    double *poav;                   /* Output array value */
    epicsFloat64        pa;         /* Prev Value of A */
    epicsFloat64        pb;         /* Prev Value of B */
    epicsFloat64        pc;         /* Prev Value of C */
    epicsFloat64        pd;         /* Prev Value of D */
    epicsFloat64        pe;         /* Prev Value of E */
    epicsFloat64        pf;         /* Prev Value of F */
    epicsFloat64        pg;         /* Prev Value of G */
    epicsFloat64        ph;         /* Prev Value of H */
    epicsFloat64        pi;         /* Prev Value of I */
    epicsFloat64        pj;         /* Prev Value of J */
    epicsFloat64        pk;         /* Prev Value of K */
    epicsFloat64        pl;         /* Prev Value of L */
    epicsFloat64        povl;       /* Prev Value of OVAL */
    epicsFloat64        lalm;       /* Last Value Alarmed */
    epicsFloat64        alst;       /* Last Value Archived */
    epicsFloat64        mlst;       /* Last Val Monitored */
    unsigned char rpcl[282];        /* Reverse Polish Calc */
    unsigned char orpc[282];        /* Reverse Polish OCalc */
    epicsUInt8          cact;       /* Calc active */
    epicsInt32          cstat;      /* Calc status */
    epicsUInt32         amask;      /* Array mod */
    epicsEnum16         size;       /* Array size reported to clients */
    epicsInt32          amem;       /* Allocated array memory */
    epicsInt32          pmem;       /* Prev array memory */
} acalcoutRecord;

typedef enum {
	acalcoutRecordNAME = 0,
	acalcoutRecordDESC = 1,
	acalcoutRecordASG = 2,
	acalcoutRecordSCAN = 3,
	acalcoutRecordPINI = 4,
	acalcoutRecordPHAS = 5,
	acalcoutRecordEVNT = 6,
	acalcoutRecordTSE = 7,
	acalcoutRecordTSEL = 8,
	acalcoutRecordDTYP = 9,
	acalcoutRecordDISV = 10,
	acalcoutRecordDISA = 11,
	acalcoutRecordSDIS = 12,
	acalcoutRecordMLOK = 13,
	acalcoutRecordMLIS = 14,
	acalcoutRecordBKLNK = 15,
	acalcoutRecordDISP = 16,
	acalcoutRecordPROC = 17,
	acalcoutRecordSTAT = 18,
	acalcoutRecordSEVR = 19,
	acalcoutRecordNSTA = 20,
	acalcoutRecordNSEV = 21,
	acalcoutRecordACKS = 22,
	acalcoutRecordACKT = 23,
	acalcoutRecordDISS = 24,
	acalcoutRecordLCNT = 25,
	acalcoutRecordPACT = 26,
	acalcoutRecordPUTF = 27,
	acalcoutRecordRPRO = 28,
	acalcoutRecordASP = 29,
	acalcoutRecordPPN = 30,
	acalcoutRecordPPNR = 31,
	acalcoutRecordSPVT = 32,
	acalcoutRecordRSET = 33,
	acalcoutRecordDSET = 34,
	acalcoutRecordDPVT = 35,
	acalcoutRecordRDES = 36,
	acalcoutRecordLSET = 37,
	acalcoutRecordPRIO = 38,
	acalcoutRecordTPRO = 39,
	acalcoutRecordBKPT = 40,
	acalcoutRecordUDF = 41,
	acalcoutRecordUDFS = 42,
	acalcoutRecordTIME = 43,
	acalcoutRecordFLNK = 44,
	acalcoutRecordVERS = 45,
	acalcoutRecordRPVT = 46,
	acalcoutRecordVAL = 47,
	acalcoutRecordAVAL = 48,
	acalcoutRecordNELM = 49,
	acalcoutRecordNUSE = 50,
	acalcoutRecordPVAL = 51,
	acalcoutRecordPAVL = 52,
	acalcoutRecordCALC = 53,
	acalcoutRecordCLCV = 54,
	acalcoutRecordINPA = 55,
	acalcoutRecordINPB = 56,
	acalcoutRecordINPC = 57,
	acalcoutRecordINPD = 58,
	acalcoutRecordINPE = 59,
	acalcoutRecordINPF = 60,
	acalcoutRecordINPG = 61,
	acalcoutRecordINPH = 62,
	acalcoutRecordINPI = 63,
	acalcoutRecordINPJ = 64,
	acalcoutRecordINPK = 65,
	acalcoutRecordINPL = 66,
	acalcoutRecordINAA = 67,
	acalcoutRecordINBB = 68,
	acalcoutRecordINCC = 69,
	acalcoutRecordINDD = 70,
	acalcoutRecordINEE = 71,
	acalcoutRecordINFF = 72,
	acalcoutRecordINGG = 73,
	acalcoutRecordINHH = 74,
	acalcoutRecordINII = 75,
	acalcoutRecordINJJ = 76,
	acalcoutRecordINKK = 77,
	acalcoutRecordINLL = 78,
	acalcoutRecordOUT = 79,
	acalcoutRecordINAV = 80,
	acalcoutRecordINBV = 81,
	acalcoutRecordINCV = 82,
	acalcoutRecordINDV = 83,
	acalcoutRecordINEV = 84,
	acalcoutRecordINFV = 85,
	acalcoutRecordINGV = 86,
	acalcoutRecordINHV = 87,
	acalcoutRecordINIV = 88,
	acalcoutRecordINJV = 89,
	acalcoutRecordINKV = 90,
	acalcoutRecordINLV = 91,
	acalcoutRecordIAAV = 92,
	acalcoutRecordIBBV = 93,
	acalcoutRecordICCV = 94,
	acalcoutRecordIDDV = 95,
	acalcoutRecordIEEV = 96,
	acalcoutRecordIFFV = 97,
	acalcoutRecordIGGV = 98,
	acalcoutRecordIHHV = 99,
	acalcoutRecordIIIV = 100,
	acalcoutRecordIJJV = 101,
	acalcoutRecordIKKV = 102,
	acalcoutRecordILLV = 103,
	acalcoutRecordOUTV = 104,
	acalcoutRecordOOPT = 105,
	acalcoutRecordODLY = 106,
	acalcoutRecordWAIT = 107,
	acalcoutRecordDLYA = 108,
	acalcoutRecordDOPT = 109,
	acalcoutRecordOCAL = 110,
	acalcoutRecordOCLV = 111,
	acalcoutRecordOEVT = 112,
	acalcoutRecordIVOA = 113,
	acalcoutRecordIVOV = 114,
	acalcoutRecordEGU = 115,
	acalcoutRecordPREC = 116,
	acalcoutRecordHOPR = 117,
	acalcoutRecordLOPR = 118,
	acalcoutRecordHIHI = 119,
	acalcoutRecordLOLO = 120,
	acalcoutRecordHIGH = 121,
	acalcoutRecordLOW = 122,
	acalcoutRecordHHSV = 123,
	acalcoutRecordLLSV = 124,
	acalcoutRecordHSV = 125,
	acalcoutRecordLSV = 126,
	acalcoutRecordHYST = 127,
	acalcoutRecordADEL = 128,
	acalcoutRecordMDEL = 129,
	acalcoutRecordA = 130,
	acalcoutRecordB = 131,
	acalcoutRecordC = 132,
	acalcoutRecordD = 133,
	acalcoutRecordE = 134,
	acalcoutRecordF = 135,
	acalcoutRecordG = 136,
	acalcoutRecordH = 137,
	acalcoutRecordI = 138,
	acalcoutRecordJ = 139,
	acalcoutRecordK = 140,
	acalcoutRecordL = 141,
	acalcoutRecordAA = 142,
	acalcoutRecordBB = 143,
	acalcoutRecordCC = 144,
	acalcoutRecordDD = 145,
	acalcoutRecordEE = 146,
	acalcoutRecordFF = 147,
	acalcoutRecordGG = 148,
	acalcoutRecordHH = 149,
	acalcoutRecordII = 150,
	acalcoutRecordJJ = 151,
	acalcoutRecordKK = 152,
	acalcoutRecordLL = 153,
	acalcoutRecordPAA = 154,
	acalcoutRecordNEWM = 155,
	acalcoutRecordOVAL = 156,
	acalcoutRecordOAV = 157,
	acalcoutRecordPOAV = 158,
	acalcoutRecordPA = 159,
	acalcoutRecordPB = 160,
	acalcoutRecordPC = 161,
	acalcoutRecordPD = 162,
	acalcoutRecordPE = 163,
	acalcoutRecordPF = 164,
	acalcoutRecordPG = 165,
	acalcoutRecordPH = 166,
	acalcoutRecordPI = 167,
	acalcoutRecordPJ = 168,
	acalcoutRecordPK = 169,
	acalcoutRecordPL = 170,
	acalcoutRecordPOVL = 171,
	acalcoutRecordLALM = 172,
	acalcoutRecordALST = 173,
	acalcoutRecordMLST = 174,
	acalcoutRecordRPCL = 175,
	acalcoutRecordORPC = 176,
	acalcoutRecordCACT = 177,
	acalcoutRecordCSTAT = 178,
	acalcoutRecordAMASK = 179,
	acalcoutRecordSIZE = 180,
	acalcoutRecordAMEM = 181,
	acalcoutRecordPMEM = 182
} acalcoutFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsAssert.h>
#include <epicsExport.h>
#ifdef __cplusplus
extern "C" {
#endif
static int acalcoutRecordSizeOffset(dbRecordType *prt)
{
    acalcoutRecord *prec = 0;

    assert(prt->no_fields == 183);
    prt->papFldDes[acalcoutRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[acalcoutRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[acalcoutRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[acalcoutRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[acalcoutRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[acalcoutRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[acalcoutRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[acalcoutRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[acalcoutRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[acalcoutRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[acalcoutRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[acalcoutRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[acalcoutRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[acalcoutRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[acalcoutRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[acalcoutRecordBKLNK]->size = sizeof(prec->bklnk);
    prt->papFldDes[acalcoutRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[acalcoutRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[acalcoutRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[acalcoutRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[acalcoutRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[acalcoutRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[acalcoutRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[acalcoutRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[acalcoutRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[acalcoutRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[acalcoutRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[acalcoutRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[acalcoutRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[acalcoutRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[acalcoutRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[acalcoutRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[acalcoutRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[acalcoutRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[acalcoutRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[acalcoutRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[acalcoutRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[acalcoutRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[acalcoutRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[acalcoutRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[acalcoutRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[acalcoutRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[acalcoutRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[acalcoutRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[acalcoutRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[acalcoutRecordVERS]->size = sizeof(prec->vers);
    prt->papFldDes[acalcoutRecordRPVT]->size = sizeof(prec->rpvt);
    prt->papFldDes[acalcoutRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[acalcoutRecordAVAL]->size = sizeof(prec->aval);
    prt->papFldDes[acalcoutRecordNELM]->size = sizeof(prec->nelm);
    prt->papFldDes[acalcoutRecordNUSE]->size = sizeof(prec->nuse);
    prt->papFldDes[acalcoutRecordPVAL]->size = sizeof(prec->pval);
    prt->papFldDes[acalcoutRecordPAVL]->size = sizeof(prec->pavl);
    prt->papFldDes[acalcoutRecordCALC]->size = sizeof(prec->calc);
    prt->papFldDes[acalcoutRecordCLCV]->size = sizeof(prec->clcv);
    prt->papFldDes[acalcoutRecordINPA]->size = sizeof(prec->inpa);
    prt->papFldDes[acalcoutRecordINPB]->size = sizeof(prec->inpb);
    prt->papFldDes[acalcoutRecordINPC]->size = sizeof(prec->inpc);
    prt->papFldDes[acalcoutRecordINPD]->size = sizeof(prec->inpd);
    prt->papFldDes[acalcoutRecordINPE]->size = sizeof(prec->inpe);
    prt->papFldDes[acalcoutRecordINPF]->size = sizeof(prec->inpf);
    prt->papFldDes[acalcoutRecordINPG]->size = sizeof(prec->inpg);
    prt->papFldDes[acalcoutRecordINPH]->size = sizeof(prec->inph);
    prt->papFldDes[acalcoutRecordINPI]->size = sizeof(prec->inpi);
    prt->papFldDes[acalcoutRecordINPJ]->size = sizeof(prec->inpj);
    prt->papFldDes[acalcoutRecordINPK]->size = sizeof(prec->inpk);
    prt->papFldDes[acalcoutRecordINPL]->size = sizeof(prec->inpl);
    prt->papFldDes[acalcoutRecordINAA]->size = sizeof(prec->inaa);
    prt->papFldDes[acalcoutRecordINBB]->size = sizeof(prec->inbb);
    prt->papFldDes[acalcoutRecordINCC]->size = sizeof(prec->incc);
    prt->papFldDes[acalcoutRecordINDD]->size = sizeof(prec->indd);
    prt->papFldDes[acalcoutRecordINEE]->size = sizeof(prec->inee);
    prt->papFldDes[acalcoutRecordINFF]->size = sizeof(prec->inff);
    prt->papFldDes[acalcoutRecordINGG]->size = sizeof(prec->ingg);
    prt->papFldDes[acalcoutRecordINHH]->size = sizeof(prec->inhh);
    prt->papFldDes[acalcoutRecordINII]->size = sizeof(prec->inii);
    prt->papFldDes[acalcoutRecordINJJ]->size = sizeof(prec->injj);
    prt->papFldDes[acalcoutRecordINKK]->size = sizeof(prec->inkk);
    prt->papFldDes[acalcoutRecordINLL]->size = sizeof(prec->inll);
    prt->papFldDes[acalcoutRecordOUT]->size = sizeof(prec->out);
    prt->papFldDes[acalcoutRecordINAV]->size = sizeof(prec->inav);
    prt->papFldDes[acalcoutRecordINBV]->size = sizeof(prec->inbv);
    prt->papFldDes[acalcoutRecordINCV]->size = sizeof(prec->incv);
    prt->papFldDes[acalcoutRecordINDV]->size = sizeof(prec->indv);
    prt->papFldDes[acalcoutRecordINEV]->size = sizeof(prec->inev);
    prt->papFldDes[acalcoutRecordINFV]->size = sizeof(prec->infv);
    prt->papFldDes[acalcoutRecordINGV]->size = sizeof(prec->ingv);
    prt->papFldDes[acalcoutRecordINHV]->size = sizeof(prec->inhv);
    prt->papFldDes[acalcoutRecordINIV]->size = sizeof(prec->iniv);
    prt->papFldDes[acalcoutRecordINJV]->size = sizeof(prec->injv);
    prt->papFldDes[acalcoutRecordINKV]->size = sizeof(prec->inkv);
    prt->papFldDes[acalcoutRecordINLV]->size = sizeof(prec->inlv);
    prt->papFldDes[acalcoutRecordIAAV]->size = sizeof(prec->iaav);
    prt->papFldDes[acalcoutRecordIBBV]->size = sizeof(prec->ibbv);
    prt->papFldDes[acalcoutRecordICCV]->size = sizeof(prec->iccv);
    prt->papFldDes[acalcoutRecordIDDV]->size = sizeof(prec->iddv);
    prt->papFldDes[acalcoutRecordIEEV]->size = sizeof(prec->ieev);
    prt->papFldDes[acalcoutRecordIFFV]->size = sizeof(prec->iffv);
    prt->papFldDes[acalcoutRecordIGGV]->size = sizeof(prec->iggv);
    prt->papFldDes[acalcoutRecordIHHV]->size = sizeof(prec->ihhv);
    prt->papFldDes[acalcoutRecordIIIV]->size = sizeof(prec->iiiv);
    prt->papFldDes[acalcoutRecordIJJV]->size = sizeof(prec->ijjv);
    prt->papFldDes[acalcoutRecordIKKV]->size = sizeof(prec->ikkv);
    prt->papFldDes[acalcoutRecordILLV]->size = sizeof(prec->illv);
    prt->papFldDes[acalcoutRecordOUTV]->size = sizeof(prec->outv);
    prt->papFldDes[acalcoutRecordOOPT]->size = sizeof(prec->oopt);
    prt->papFldDes[acalcoutRecordODLY]->size = sizeof(prec->odly);
    prt->papFldDes[acalcoutRecordWAIT]->size = sizeof(prec->wait);
    prt->papFldDes[acalcoutRecordDLYA]->size = sizeof(prec->dlya);
    prt->papFldDes[acalcoutRecordDOPT]->size = sizeof(prec->dopt);
    prt->papFldDes[acalcoutRecordOCAL]->size = sizeof(prec->ocal);
    prt->papFldDes[acalcoutRecordOCLV]->size = sizeof(prec->oclv);
    prt->papFldDes[acalcoutRecordOEVT]->size = sizeof(prec->oevt);
    prt->papFldDes[acalcoutRecordIVOA]->size = sizeof(prec->ivoa);
    prt->papFldDes[acalcoutRecordIVOV]->size = sizeof(prec->ivov);
    prt->papFldDes[acalcoutRecordEGU]->size = sizeof(prec->egu);
    prt->papFldDes[acalcoutRecordPREC]->size = sizeof(prec->prec);
    prt->papFldDes[acalcoutRecordHOPR]->size = sizeof(prec->hopr);
    prt->papFldDes[acalcoutRecordLOPR]->size = sizeof(prec->lopr);
    prt->papFldDes[acalcoutRecordHIHI]->size = sizeof(prec->hihi);
    prt->papFldDes[acalcoutRecordLOLO]->size = sizeof(prec->lolo);
    prt->papFldDes[acalcoutRecordHIGH]->size = sizeof(prec->high);
    prt->papFldDes[acalcoutRecordLOW]->size = sizeof(prec->low);
    prt->papFldDes[acalcoutRecordHHSV]->size = sizeof(prec->hhsv);
    prt->papFldDes[acalcoutRecordLLSV]->size = sizeof(prec->llsv);
    prt->papFldDes[acalcoutRecordHSV]->size = sizeof(prec->hsv);
    prt->papFldDes[acalcoutRecordLSV]->size = sizeof(prec->lsv);
    prt->papFldDes[acalcoutRecordHYST]->size = sizeof(prec->hyst);
    prt->papFldDes[acalcoutRecordADEL]->size = sizeof(prec->adel);
    prt->papFldDes[acalcoutRecordMDEL]->size = sizeof(prec->mdel);
    prt->papFldDes[acalcoutRecordA]->size = sizeof(prec->a);
    prt->papFldDes[acalcoutRecordB]->size = sizeof(prec->b);
    prt->papFldDes[acalcoutRecordC]->size = sizeof(prec->c);
    prt->papFldDes[acalcoutRecordD]->size = sizeof(prec->d);
    prt->papFldDes[acalcoutRecordE]->size = sizeof(prec->e);
    prt->papFldDes[acalcoutRecordF]->size = sizeof(prec->f);
    prt->papFldDes[acalcoutRecordG]->size = sizeof(prec->g);
    prt->papFldDes[acalcoutRecordH]->size = sizeof(prec->h);
    prt->papFldDes[acalcoutRecordI]->size = sizeof(prec->i);
    prt->papFldDes[acalcoutRecordJ]->size = sizeof(prec->j);
    prt->papFldDes[acalcoutRecordK]->size = sizeof(prec->k);
    prt->papFldDes[acalcoutRecordL]->size = sizeof(prec->l);
    prt->papFldDes[acalcoutRecordAA]->size = sizeof(prec->aa);
    prt->papFldDes[acalcoutRecordBB]->size = sizeof(prec->bb);
    prt->papFldDes[acalcoutRecordCC]->size = sizeof(prec->cc);
    prt->papFldDes[acalcoutRecordDD]->size = sizeof(prec->dd);
    prt->papFldDes[acalcoutRecordEE]->size = sizeof(prec->ee);
    prt->papFldDes[acalcoutRecordFF]->size = sizeof(prec->ff);
    prt->papFldDes[acalcoutRecordGG]->size = sizeof(prec->gg);
    prt->papFldDes[acalcoutRecordHH]->size = sizeof(prec->hh);
    prt->papFldDes[acalcoutRecordII]->size = sizeof(prec->ii);
    prt->papFldDes[acalcoutRecordJJ]->size = sizeof(prec->jj);
    prt->papFldDes[acalcoutRecordKK]->size = sizeof(prec->kk);
    prt->papFldDes[acalcoutRecordLL]->size = sizeof(prec->ll);
    prt->papFldDes[acalcoutRecordPAA]->size = sizeof(prec->paa);
    prt->papFldDes[acalcoutRecordNEWM]->size = sizeof(prec->newm);
    prt->papFldDes[acalcoutRecordOVAL]->size = sizeof(prec->oval);
    prt->papFldDes[acalcoutRecordOAV]->size = sizeof(prec->oav);
    prt->papFldDes[acalcoutRecordPOAV]->size = sizeof(prec->poav);
    prt->papFldDes[acalcoutRecordPA]->size = sizeof(prec->pa);
    prt->papFldDes[acalcoutRecordPB]->size = sizeof(prec->pb);
    prt->papFldDes[acalcoutRecordPC]->size = sizeof(prec->pc);
    prt->papFldDes[acalcoutRecordPD]->size = sizeof(prec->pd);
    prt->papFldDes[acalcoutRecordPE]->size = sizeof(prec->pe);
    prt->papFldDes[acalcoutRecordPF]->size = sizeof(prec->pf);
    prt->papFldDes[acalcoutRecordPG]->size = sizeof(prec->pg);
    prt->papFldDes[acalcoutRecordPH]->size = sizeof(prec->ph);
    prt->papFldDes[acalcoutRecordPI]->size = sizeof(prec->pi);
    prt->papFldDes[acalcoutRecordPJ]->size = sizeof(prec->pj);
    prt->papFldDes[acalcoutRecordPK]->size = sizeof(prec->pk);
    prt->papFldDes[acalcoutRecordPL]->size = sizeof(prec->pl);
    prt->papFldDes[acalcoutRecordPOVL]->size = sizeof(prec->povl);
    prt->papFldDes[acalcoutRecordLALM]->size = sizeof(prec->lalm);
    prt->papFldDes[acalcoutRecordALST]->size = sizeof(prec->alst);
    prt->papFldDes[acalcoutRecordMLST]->size = sizeof(prec->mlst);
    prt->papFldDes[acalcoutRecordRPCL]->size = sizeof(prec->rpcl);
    prt->papFldDes[acalcoutRecordORPC]->size = sizeof(prec->orpc);
    prt->papFldDes[acalcoutRecordCACT]->size = sizeof(prec->cact);
    prt->papFldDes[acalcoutRecordCSTAT]->size = sizeof(prec->cstat);
    prt->papFldDes[acalcoutRecordAMASK]->size = sizeof(prec->amask);
    prt->papFldDes[acalcoutRecordSIZE]->size = sizeof(prec->size);
    prt->papFldDes[acalcoutRecordAMEM]->size = sizeof(prec->amem);
    prt->papFldDes[acalcoutRecordPMEM]->size = sizeof(prec->pmem);
    prt->papFldDes[acalcoutRecordNAME]->offset = (unsigned short)((char *)&prec->name - (char *)prec);
    prt->papFldDes[acalcoutRecordDESC]->offset = (unsigned short)((char *)&prec->desc - (char *)prec);
    prt->papFldDes[acalcoutRecordASG]->offset = (unsigned short)((char *)&prec->asg - (char *)prec);
    prt->papFldDes[acalcoutRecordSCAN]->offset = (unsigned short)((char *)&prec->scan - (char *)prec);
    prt->papFldDes[acalcoutRecordPINI]->offset = (unsigned short)((char *)&prec->pini - (char *)prec);
    prt->papFldDes[acalcoutRecordPHAS]->offset = (unsigned short)((char *)&prec->phas - (char *)prec);
    prt->papFldDes[acalcoutRecordEVNT]->offset = (unsigned short)((char *)&prec->evnt - (char *)prec);
    prt->papFldDes[acalcoutRecordTSE]->offset = (unsigned short)((char *)&prec->tse - (char *)prec);
    prt->papFldDes[acalcoutRecordTSEL]->offset = (unsigned short)((char *)&prec->tsel - (char *)prec);
    prt->papFldDes[acalcoutRecordDTYP]->offset = (unsigned short)((char *)&prec->dtyp - (char *)prec);
    prt->papFldDes[acalcoutRecordDISV]->offset = (unsigned short)((char *)&prec->disv - (char *)prec);
    prt->papFldDes[acalcoutRecordDISA]->offset = (unsigned short)((char *)&prec->disa - (char *)prec);
    prt->papFldDes[acalcoutRecordSDIS]->offset = (unsigned short)((char *)&prec->sdis - (char *)prec);
    prt->papFldDes[acalcoutRecordMLOK]->offset = (unsigned short)((char *)&prec->mlok - (char *)prec);
    prt->papFldDes[acalcoutRecordMLIS]->offset = (unsigned short)((char *)&prec->mlis - (char *)prec);
    prt->papFldDes[acalcoutRecordBKLNK]->offset = (unsigned short)((char *)&prec->bklnk - (char *)prec);
    prt->papFldDes[acalcoutRecordDISP]->offset = (unsigned short)((char *)&prec->disp - (char *)prec);
    prt->papFldDes[acalcoutRecordPROC]->offset = (unsigned short)((char *)&prec->proc - (char *)prec);
    prt->papFldDes[acalcoutRecordSTAT]->offset = (unsigned short)((char *)&prec->stat - (char *)prec);
    prt->papFldDes[acalcoutRecordSEVR]->offset = (unsigned short)((char *)&prec->sevr - (char *)prec);
    prt->papFldDes[acalcoutRecordNSTA]->offset = (unsigned short)((char *)&prec->nsta - (char *)prec);
    prt->papFldDes[acalcoutRecordNSEV]->offset = (unsigned short)((char *)&prec->nsev - (char *)prec);
    prt->papFldDes[acalcoutRecordACKS]->offset = (unsigned short)((char *)&prec->acks - (char *)prec);
    prt->papFldDes[acalcoutRecordACKT]->offset = (unsigned short)((char *)&prec->ackt - (char *)prec);
    prt->papFldDes[acalcoutRecordDISS]->offset = (unsigned short)((char *)&prec->diss - (char *)prec);
    prt->papFldDes[acalcoutRecordLCNT]->offset = (unsigned short)((char *)&prec->lcnt - (char *)prec);
    prt->papFldDes[acalcoutRecordPACT]->offset = (unsigned short)((char *)&prec->pact - (char *)prec);
    prt->papFldDes[acalcoutRecordPUTF]->offset = (unsigned short)((char *)&prec->putf - (char *)prec);
    prt->papFldDes[acalcoutRecordRPRO]->offset = (unsigned short)((char *)&prec->rpro - (char *)prec);
    prt->papFldDes[acalcoutRecordASP]->offset = (unsigned short)((char *)&prec->asp - (char *)prec);
    prt->papFldDes[acalcoutRecordPPN]->offset = (unsigned short)((char *)&prec->ppn - (char *)prec);
    prt->papFldDes[acalcoutRecordPPNR]->offset = (unsigned short)((char *)&prec->ppnr - (char *)prec);
    prt->papFldDes[acalcoutRecordSPVT]->offset = (unsigned short)((char *)&prec->spvt - (char *)prec);
    prt->papFldDes[acalcoutRecordRSET]->offset = (unsigned short)((char *)&prec->rset - (char *)prec);
    prt->papFldDes[acalcoutRecordDSET]->offset = (unsigned short)((char *)&prec->dset - (char *)prec);
    prt->papFldDes[acalcoutRecordDPVT]->offset = (unsigned short)((char *)&prec->dpvt - (char *)prec);
    prt->papFldDes[acalcoutRecordRDES]->offset = (unsigned short)((char *)&prec->rdes - (char *)prec);
    prt->papFldDes[acalcoutRecordLSET]->offset = (unsigned short)((char *)&prec->lset - (char *)prec);
    prt->papFldDes[acalcoutRecordPRIO]->offset = (unsigned short)((char *)&prec->prio - (char *)prec);
    prt->papFldDes[acalcoutRecordTPRO]->offset = (unsigned short)((char *)&prec->tpro - (char *)prec);
    prt->papFldDes[acalcoutRecordBKPT]->offset = (unsigned short)((char *)&prec->bkpt - (char *)prec);
    prt->papFldDes[acalcoutRecordUDF]->offset = (unsigned short)((char *)&prec->udf - (char *)prec);
    prt->papFldDes[acalcoutRecordUDFS]->offset = (unsigned short)((char *)&prec->udfs - (char *)prec);
    prt->papFldDes[acalcoutRecordTIME]->offset = (unsigned short)((char *)&prec->time - (char *)prec);
    prt->papFldDes[acalcoutRecordFLNK]->offset = (unsigned short)((char *)&prec->flnk - (char *)prec);
    prt->papFldDes[acalcoutRecordVERS]->offset = (unsigned short)((char *)&prec->vers - (char *)prec);
    prt->papFldDes[acalcoutRecordRPVT]->offset = (unsigned short)((char *)&prec->rpvt - (char *)prec);
    prt->papFldDes[acalcoutRecordVAL]->offset = (unsigned short)((char *)&prec->val - (char *)prec);
    prt->papFldDes[acalcoutRecordAVAL]->offset = (unsigned short)((char *)&prec->aval - (char *)prec);
    prt->papFldDes[acalcoutRecordNELM]->offset = (unsigned short)((char *)&prec->nelm - (char *)prec);
    prt->papFldDes[acalcoutRecordNUSE]->offset = (unsigned short)((char *)&prec->nuse - (char *)prec);
    prt->papFldDes[acalcoutRecordPVAL]->offset = (unsigned short)((char *)&prec->pval - (char *)prec);
    prt->papFldDes[acalcoutRecordPAVL]->offset = (unsigned short)((char *)&prec->pavl - (char *)prec);
    prt->papFldDes[acalcoutRecordCALC]->offset = (unsigned short)((char *)&prec->calc - (char *)prec);
    prt->papFldDes[acalcoutRecordCLCV]->offset = (unsigned short)((char *)&prec->clcv - (char *)prec);
    prt->papFldDes[acalcoutRecordINPA]->offset = (unsigned short)((char *)&prec->inpa - (char *)prec);
    prt->papFldDes[acalcoutRecordINPB]->offset = (unsigned short)((char *)&prec->inpb - (char *)prec);
    prt->papFldDes[acalcoutRecordINPC]->offset = (unsigned short)((char *)&prec->inpc - (char *)prec);
    prt->papFldDes[acalcoutRecordINPD]->offset = (unsigned short)((char *)&prec->inpd - (char *)prec);
    prt->papFldDes[acalcoutRecordINPE]->offset = (unsigned short)((char *)&prec->inpe - (char *)prec);
    prt->papFldDes[acalcoutRecordINPF]->offset = (unsigned short)((char *)&prec->inpf - (char *)prec);
    prt->papFldDes[acalcoutRecordINPG]->offset = (unsigned short)((char *)&prec->inpg - (char *)prec);
    prt->papFldDes[acalcoutRecordINPH]->offset = (unsigned short)((char *)&prec->inph - (char *)prec);
    prt->papFldDes[acalcoutRecordINPI]->offset = (unsigned short)((char *)&prec->inpi - (char *)prec);
    prt->papFldDes[acalcoutRecordINPJ]->offset = (unsigned short)((char *)&prec->inpj - (char *)prec);
    prt->papFldDes[acalcoutRecordINPK]->offset = (unsigned short)((char *)&prec->inpk - (char *)prec);
    prt->papFldDes[acalcoutRecordINPL]->offset = (unsigned short)((char *)&prec->inpl - (char *)prec);
    prt->papFldDes[acalcoutRecordINAA]->offset = (unsigned short)((char *)&prec->inaa - (char *)prec);
    prt->papFldDes[acalcoutRecordINBB]->offset = (unsigned short)((char *)&prec->inbb - (char *)prec);
    prt->papFldDes[acalcoutRecordINCC]->offset = (unsigned short)((char *)&prec->incc - (char *)prec);
    prt->papFldDes[acalcoutRecordINDD]->offset = (unsigned short)((char *)&prec->indd - (char *)prec);
    prt->papFldDes[acalcoutRecordINEE]->offset = (unsigned short)((char *)&prec->inee - (char *)prec);
    prt->papFldDes[acalcoutRecordINFF]->offset = (unsigned short)((char *)&prec->inff - (char *)prec);
    prt->papFldDes[acalcoutRecordINGG]->offset = (unsigned short)((char *)&prec->ingg - (char *)prec);
    prt->papFldDes[acalcoutRecordINHH]->offset = (unsigned short)((char *)&prec->inhh - (char *)prec);
    prt->papFldDes[acalcoutRecordINII]->offset = (unsigned short)((char *)&prec->inii - (char *)prec);
    prt->papFldDes[acalcoutRecordINJJ]->offset = (unsigned short)((char *)&prec->injj - (char *)prec);
    prt->papFldDes[acalcoutRecordINKK]->offset = (unsigned short)((char *)&prec->inkk - (char *)prec);
    prt->papFldDes[acalcoutRecordINLL]->offset = (unsigned short)((char *)&prec->inll - (char *)prec);
    prt->papFldDes[acalcoutRecordOUT]->offset = (unsigned short)((char *)&prec->out - (char *)prec);
    prt->papFldDes[acalcoutRecordINAV]->offset = (unsigned short)((char *)&prec->inav - (char *)prec);
    prt->papFldDes[acalcoutRecordINBV]->offset = (unsigned short)((char *)&prec->inbv - (char *)prec);
    prt->papFldDes[acalcoutRecordINCV]->offset = (unsigned short)((char *)&prec->incv - (char *)prec);
    prt->papFldDes[acalcoutRecordINDV]->offset = (unsigned short)((char *)&prec->indv - (char *)prec);
    prt->papFldDes[acalcoutRecordINEV]->offset = (unsigned short)((char *)&prec->inev - (char *)prec);
    prt->papFldDes[acalcoutRecordINFV]->offset = (unsigned short)((char *)&prec->infv - (char *)prec);
    prt->papFldDes[acalcoutRecordINGV]->offset = (unsigned short)((char *)&prec->ingv - (char *)prec);
    prt->papFldDes[acalcoutRecordINHV]->offset = (unsigned short)((char *)&prec->inhv - (char *)prec);
    prt->papFldDes[acalcoutRecordINIV]->offset = (unsigned short)((char *)&prec->iniv - (char *)prec);
    prt->papFldDes[acalcoutRecordINJV]->offset = (unsigned short)((char *)&prec->injv - (char *)prec);
    prt->papFldDes[acalcoutRecordINKV]->offset = (unsigned short)((char *)&prec->inkv - (char *)prec);
    prt->papFldDes[acalcoutRecordINLV]->offset = (unsigned short)((char *)&prec->inlv - (char *)prec);
    prt->papFldDes[acalcoutRecordIAAV]->offset = (unsigned short)((char *)&prec->iaav - (char *)prec);
    prt->papFldDes[acalcoutRecordIBBV]->offset = (unsigned short)((char *)&prec->ibbv - (char *)prec);
    prt->papFldDes[acalcoutRecordICCV]->offset = (unsigned short)((char *)&prec->iccv - (char *)prec);
    prt->papFldDes[acalcoutRecordIDDV]->offset = (unsigned short)((char *)&prec->iddv - (char *)prec);
    prt->papFldDes[acalcoutRecordIEEV]->offset = (unsigned short)((char *)&prec->ieev - (char *)prec);
    prt->papFldDes[acalcoutRecordIFFV]->offset = (unsigned short)((char *)&prec->iffv - (char *)prec);
    prt->papFldDes[acalcoutRecordIGGV]->offset = (unsigned short)((char *)&prec->iggv - (char *)prec);
    prt->papFldDes[acalcoutRecordIHHV]->offset = (unsigned short)((char *)&prec->ihhv - (char *)prec);
    prt->papFldDes[acalcoutRecordIIIV]->offset = (unsigned short)((char *)&prec->iiiv - (char *)prec);
    prt->papFldDes[acalcoutRecordIJJV]->offset = (unsigned short)((char *)&prec->ijjv - (char *)prec);
    prt->papFldDes[acalcoutRecordIKKV]->offset = (unsigned short)((char *)&prec->ikkv - (char *)prec);
    prt->papFldDes[acalcoutRecordILLV]->offset = (unsigned short)((char *)&prec->illv - (char *)prec);
    prt->papFldDes[acalcoutRecordOUTV]->offset = (unsigned short)((char *)&prec->outv - (char *)prec);
    prt->papFldDes[acalcoutRecordOOPT]->offset = (unsigned short)((char *)&prec->oopt - (char *)prec);
    prt->papFldDes[acalcoutRecordODLY]->offset = (unsigned short)((char *)&prec->odly - (char *)prec);
    prt->papFldDes[acalcoutRecordWAIT]->offset = (unsigned short)((char *)&prec->wait - (char *)prec);
    prt->papFldDes[acalcoutRecordDLYA]->offset = (unsigned short)((char *)&prec->dlya - (char *)prec);
    prt->papFldDes[acalcoutRecordDOPT]->offset = (unsigned short)((char *)&prec->dopt - (char *)prec);
    prt->papFldDes[acalcoutRecordOCAL]->offset = (unsigned short)((char *)&prec->ocal - (char *)prec);
    prt->papFldDes[acalcoutRecordOCLV]->offset = (unsigned short)((char *)&prec->oclv - (char *)prec);
    prt->papFldDes[acalcoutRecordOEVT]->offset = (unsigned short)((char *)&prec->oevt - (char *)prec);
    prt->papFldDes[acalcoutRecordIVOA]->offset = (unsigned short)((char *)&prec->ivoa - (char *)prec);
    prt->papFldDes[acalcoutRecordIVOV]->offset = (unsigned short)((char *)&prec->ivov - (char *)prec);
    prt->papFldDes[acalcoutRecordEGU]->offset = (unsigned short)((char *)&prec->egu - (char *)prec);
    prt->papFldDes[acalcoutRecordPREC]->offset = (unsigned short)((char *)&prec->prec - (char *)prec);
    prt->papFldDes[acalcoutRecordHOPR]->offset = (unsigned short)((char *)&prec->hopr - (char *)prec);
    prt->papFldDes[acalcoutRecordLOPR]->offset = (unsigned short)((char *)&prec->lopr - (char *)prec);
    prt->papFldDes[acalcoutRecordHIHI]->offset = (unsigned short)((char *)&prec->hihi - (char *)prec);
    prt->papFldDes[acalcoutRecordLOLO]->offset = (unsigned short)((char *)&prec->lolo - (char *)prec);
    prt->papFldDes[acalcoutRecordHIGH]->offset = (unsigned short)((char *)&prec->high - (char *)prec);
    prt->papFldDes[acalcoutRecordLOW]->offset = (unsigned short)((char *)&prec->low - (char *)prec);
    prt->papFldDes[acalcoutRecordHHSV]->offset = (unsigned short)((char *)&prec->hhsv - (char *)prec);
    prt->papFldDes[acalcoutRecordLLSV]->offset = (unsigned short)((char *)&prec->llsv - (char *)prec);
    prt->papFldDes[acalcoutRecordHSV]->offset = (unsigned short)((char *)&prec->hsv - (char *)prec);
    prt->papFldDes[acalcoutRecordLSV]->offset = (unsigned short)((char *)&prec->lsv - (char *)prec);
    prt->papFldDes[acalcoutRecordHYST]->offset = (unsigned short)((char *)&prec->hyst - (char *)prec);
    prt->papFldDes[acalcoutRecordADEL]->offset = (unsigned short)((char *)&prec->adel - (char *)prec);
    prt->papFldDes[acalcoutRecordMDEL]->offset = (unsigned short)((char *)&prec->mdel - (char *)prec);
    prt->papFldDes[acalcoutRecordA]->offset = (unsigned short)((char *)&prec->a - (char *)prec);
    prt->papFldDes[acalcoutRecordB]->offset = (unsigned short)((char *)&prec->b - (char *)prec);
    prt->papFldDes[acalcoutRecordC]->offset = (unsigned short)((char *)&prec->c - (char *)prec);
    prt->papFldDes[acalcoutRecordD]->offset = (unsigned short)((char *)&prec->d - (char *)prec);
    prt->papFldDes[acalcoutRecordE]->offset = (unsigned short)((char *)&prec->e - (char *)prec);
    prt->papFldDes[acalcoutRecordF]->offset = (unsigned short)((char *)&prec->f - (char *)prec);
    prt->papFldDes[acalcoutRecordG]->offset = (unsigned short)((char *)&prec->g - (char *)prec);
    prt->papFldDes[acalcoutRecordH]->offset = (unsigned short)((char *)&prec->h - (char *)prec);
    prt->papFldDes[acalcoutRecordI]->offset = (unsigned short)((char *)&prec->i - (char *)prec);
    prt->papFldDes[acalcoutRecordJ]->offset = (unsigned short)((char *)&prec->j - (char *)prec);
    prt->papFldDes[acalcoutRecordK]->offset = (unsigned short)((char *)&prec->k - (char *)prec);
    prt->papFldDes[acalcoutRecordL]->offset = (unsigned short)((char *)&prec->l - (char *)prec);
    prt->papFldDes[acalcoutRecordAA]->offset = (unsigned short)((char *)&prec->aa - (char *)prec);
    prt->papFldDes[acalcoutRecordBB]->offset = (unsigned short)((char *)&prec->bb - (char *)prec);
    prt->papFldDes[acalcoutRecordCC]->offset = (unsigned short)((char *)&prec->cc - (char *)prec);
    prt->papFldDes[acalcoutRecordDD]->offset = (unsigned short)((char *)&prec->dd - (char *)prec);
    prt->papFldDes[acalcoutRecordEE]->offset = (unsigned short)((char *)&prec->ee - (char *)prec);
    prt->papFldDes[acalcoutRecordFF]->offset = (unsigned short)((char *)&prec->ff - (char *)prec);
    prt->papFldDes[acalcoutRecordGG]->offset = (unsigned short)((char *)&prec->gg - (char *)prec);
    prt->papFldDes[acalcoutRecordHH]->offset = (unsigned short)((char *)&prec->hh - (char *)prec);
    prt->papFldDes[acalcoutRecordII]->offset = (unsigned short)((char *)&prec->ii - (char *)prec);
    prt->papFldDes[acalcoutRecordJJ]->offset = (unsigned short)((char *)&prec->jj - (char *)prec);
    prt->papFldDes[acalcoutRecordKK]->offset = (unsigned short)((char *)&prec->kk - (char *)prec);
    prt->papFldDes[acalcoutRecordLL]->offset = (unsigned short)((char *)&prec->ll - (char *)prec);
    prt->papFldDes[acalcoutRecordPAA]->offset = (unsigned short)((char *)&prec->paa - (char *)prec);
    prt->papFldDes[acalcoutRecordNEWM]->offset = (unsigned short)((char *)&prec->newm - (char *)prec);
    prt->papFldDes[acalcoutRecordOVAL]->offset = (unsigned short)((char *)&prec->oval - (char *)prec);
    prt->papFldDes[acalcoutRecordOAV]->offset = (unsigned short)((char *)&prec->oav - (char *)prec);
    prt->papFldDes[acalcoutRecordPOAV]->offset = (unsigned short)((char *)&prec->poav - (char *)prec);
    prt->papFldDes[acalcoutRecordPA]->offset = (unsigned short)((char *)&prec->pa - (char *)prec);
    prt->papFldDes[acalcoutRecordPB]->offset = (unsigned short)((char *)&prec->pb - (char *)prec);
    prt->papFldDes[acalcoutRecordPC]->offset = (unsigned short)((char *)&prec->pc - (char *)prec);
    prt->papFldDes[acalcoutRecordPD]->offset = (unsigned short)((char *)&prec->pd - (char *)prec);
    prt->papFldDes[acalcoutRecordPE]->offset = (unsigned short)((char *)&prec->pe - (char *)prec);
    prt->papFldDes[acalcoutRecordPF]->offset = (unsigned short)((char *)&prec->pf - (char *)prec);
    prt->papFldDes[acalcoutRecordPG]->offset = (unsigned short)((char *)&prec->pg - (char *)prec);
    prt->papFldDes[acalcoutRecordPH]->offset = (unsigned short)((char *)&prec->ph - (char *)prec);
    prt->papFldDes[acalcoutRecordPI]->offset = (unsigned short)((char *)&prec->pi - (char *)prec);
    prt->papFldDes[acalcoutRecordPJ]->offset = (unsigned short)((char *)&prec->pj - (char *)prec);
    prt->papFldDes[acalcoutRecordPK]->offset = (unsigned short)((char *)&prec->pk - (char *)prec);
    prt->papFldDes[acalcoutRecordPL]->offset = (unsigned short)((char *)&prec->pl - (char *)prec);
    prt->papFldDes[acalcoutRecordPOVL]->offset = (unsigned short)((char *)&prec->povl - (char *)prec);
    prt->papFldDes[acalcoutRecordLALM]->offset = (unsigned short)((char *)&prec->lalm - (char *)prec);
    prt->papFldDes[acalcoutRecordALST]->offset = (unsigned short)((char *)&prec->alst - (char *)prec);
    prt->papFldDes[acalcoutRecordMLST]->offset = (unsigned short)((char *)&prec->mlst - (char *)prec);
    prt->papFldDes[acalcoutRecordRPCL]->offset = (unsigned short)((char *)&prec->rpcl - (char *)prec);
    prt->papFldDes[acalcoutRecordORPC]->offset = (unsigned short)((char *)&prec->orpc - (char *)prec);
    prt->papFldDes[acalcoutRecordCACT]->offset = (unsigned short)((char *)&prec->cact - (char *)prec);
    prt->papFldDes[acalcoutRecordCSTAT]->offset = (unsigned short)((char *)&prec->cstat - (char *)prec);
    prt->papFldDes[acalcoutRecordAMASK]->offset = (unsigned short)((char *)&prec->amask - (char *)prec);
    prt->papFldDes[acalcoutRecordSIZE]->offset = (unsigned short)((char *)&prec->size - (char *)prec);
    prt->papFldDes[acalcoutRecordAMEM]->offset = (unsigned short)((char *)&prec->amem - (char *)prec);
    prt->papFldDes[acalcoutRecordPMEM]->offset = (unsigned short)((char *)&prec->pmem - (char *)prec);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(acalcoutRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_aCalcoutRecord_H */
