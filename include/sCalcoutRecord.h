/* sCalcoutRecord.h generated from sCalcoutRecord.dbd */

#ifndef INC_sCalcoutRecord_H
#define INC_sCalcoutRecord_H

#include "epicsTypes.h"
#include "link.h"
 #include "epicsMutex.h"
#include "ellLib.h"
#include "epicsTime.h"

#ifndef scalcoutOOPT_NUM_CHOICES
typedef enum {
    scalcoutOOPT_Every_Time         /* Every Time */,
    scalcoutOOPT_On_Change          /* On Change */,
    scalcoutOOPT_When_Zero          /* When Zero */,
    scalcoutOOPT_When_Non_zero      /* When Non-zero */,
    scalcoutOOPT_Transition_To_Zero /* Transition To Zero */,
    scalcoutOOPT_Transition_To_Non_zero /* Transition To Non-zero */,
    scalcoutOOPT_Never              /* Never */
} scalcoutOOPT;
#define scalcoutOOPT_NUM_CHOICES 7
#endif

#ifndef scalcoutINAV_NUM_CHOICES
typedef enum {
    scalcoutINAV_EXT_NC             /* Ext PV NC */,
    scalcoutINAV_EXT                /* Ext PV OK */,
    scalcoutINAV_LOC                /* Local PV */,
    scalcoutINAV_CON                /* Constant */
} scalcoutINAV;
#define scalcoutINAV_NUM_CHOICES 4
#endif

#ifndef scalcoutWAIT_NUM_CHOICES
typedef enum {
    scalcoutWAIT_NoWait             /* NoWait */,
    scalcoutWAIT_Wait               /* Wait */
} scalcoutWAIT;
#define scalcoutWAIT_NUM_CHOICES 2
#endif

#ifndef scalcoutDOPT_NUM_CHOICES
typedef enum {
    scalcoutDOPT_Use_VAL            /* Use CALC */,
    scalcoutDOPT_Use_OVAL           /* Use OCAL */
} scalcoutDOPT;
#define scalcoutDOPT_NUM_CHOICES 2
#endif

#ifndef scalcoutINAP_NUM_CHOICES
typedef enum {
    scalcoutINAP_No                 /* No PROC on Change */,
    scalcoutINAP_Yes                /* PROC on Change */
} scalcoutINAP;
#define scalcoutINAP_NUM_CHOICES 2
#endif

typedef struct scalcoutRecord {
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
    epicsFloat64        vers;       /* Code Version */
    void *rpvt;                     /* Record Private */
    epicsFloat64        val;        /* Result */
    char                sval[40];   /* String result */
    epicsFloat64        pval;       /* Previous Value */
    char                psvl[40];   /* Previous string result */
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
    DBLINK              inaa;       /* String input AA */
    DBLINK              inbb;       /* String input BB */
    DBLINK              incc;       /* String input CC */
    DBLINK              indd;       /* String input DD */
    DBLINK              inee;       /* String input EE */
    DBLINK              inff;       /* String input FF */
    DBLINK              ingg;       /* String input GG */
    DBLINK              inhh;       /* String input HH */
    DBLINK              inii;       /* String input II */
    DBLINK              injj;       /* String input JJ */
    DBLINK              inkk;       /* String input KK */
    DBLINK              inll;       /* String input LL */
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
    char **strs;                    /* Array of string pointers */
    char                aa[40];     /* Value of string input AA */
    char                bb[40];     /* Value of string input BB */
    char                cc[40];     /* Value of string input CC */
    char                dd[40];     /* Value of string input DD */
    char                ee[40];     /* Value of string input EE */
    char                ff[40];     /* Value of string input FF */
    char                gg[40];     /* Value of string input GG */
    char                hh[40];     /* Value of string input HH */
    char                ii[40];     /* Value of string input II */
    char                jj[40];     /* Value of string input JJ */
    char                kk[40];     /* Value of string input KK */
    char                ll[40];     /* Value of string input LL */
    char *paa;                      /* Prev Value of AA */
    char *pbb;                      /* Prev Value of BB */
    char *pcc;                      /* Prev Value of CC */
    char *pdd;                      /* Prev Value of DD */
    char *pee;                      /* Prev Value of EE */
    char *pff;                      /* Prev Value of FF */
    char *pgg;                      /* Prev Value of GG */
    char *phh;                      /* Prev Value of HH */
    char *pii;                      /* Prev Value of II */
    char *pjj;                      /* Prev Value of JJ */
    char *pkk;                      /* Prev Value of KK */
    char *pll;                      /* Prev Value of LL */
    epicsFloat64        oval;       /* Output Value */
    char                osv[40];    /* Output string value */
    char                posv[40];   /* Previous output string value */
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
    unsigned char rpcl[282];        /* Postfix Calc */
    unsigned char orpc[282];        /* Postfix OCalc */
} scalcoutRecord;

typedef enum {
	scalcoutRecordNAME = 0,
	scalcoutRecordDESC = 1,
	scalcoutRecordASG = 2,
	scalcoutRecordSCAN = 3,
	scalcoutRecordPINI = 4,
	scalcoutRecordPHAS = 5,
	scalcoutRecordEVNT = 6,
	scalcoutRecordTSE = 7,
	scalcoutRecordTSEL = 8,
	scalcoutRecordDTYP = 9,
	scalcoutRecordDISV = 10,
	scalcoutRecordDISA = 11,
	scalcoutRecordSDIS = 12,
	scalcoutRecordMLOK = 13,
	scalcoutRecordMLIS = 14,
	scalcoutRecordBKLNK = 15,
	scalcoutRecordDISP = 16,
	scalcoutRecordPROC = 17,
	scalcoutRecordSTAT = 18,
	scalcoutRecordSEVR = 19,
	scalcoutRecordAMSG = 20,
	scalcoutRecordNSTA = 21,
	scalcoutRecordNSEV = 22,
	scalcoutRecordNAMSG = 23,
	scalcoutRecordACKS = 24,
	scalcoutRecordACKT = 25,
	scalcoutRecordDISS = 26,
	scalcoutRecordLCNT = 27,
	scalcoutRecordPACT = 28,
	scalcoutRecordPUTF = 29,
	scalcoutRecordRPRO = 30,
	scalcoutRecordASP = 31,
	scalcoutRecordPPN = 32,
	scalcoutRecordPPNR = 33,
	scalcoutRecordSPVT = 34,
	scalcoutRecordRSET = 35,
	scalcoutRecordDSET = 36,
	scalcoutRecordDPVT = 37,
	scalcoutRecordRDES = 38,
	scalcoutRecordLSET = 39,
	scalcoutRecordPRIO = 40,
	scalcoutRecordTPRO = 41,
	scalcoutRecordBKPT = 42,
	scalcoutRecordUDF = 43,
	scalcoutRecordUDFS = 44,
	scalcoutRecordTIME = 45,
	scalcoutRecordFLNK = 46,
	scalcoutRecordVERS = 47,
	scalcoutRecordRPVT = 48,
	scalcoutRecordVAL = 49,
	scalcoutRecordSVAL = 50,
	scalcoutRecordPVAL = 51,
	scalcoutRecordPSVL = 52,
	scalcoutRecordCALC = 53,
	scalcoutRecordCLCV = 54,
	scalcoutRecordINPA = 55,
	scalcoutRecordINPB = 56,
	scalcoutRecordINPC = 57,
	scalcoutRecordINPD = 58,
	scalcoutRecordINPE = 59,
	scalcoutRecordINPF = 60,
	scalcoutRecordINPG = 61,
	scalcoutRecordINPH = 62,
	scalcoutRecordINPI = 63,
	scalcoutRecordINPJ = 64,
	scalcoutRecordINPK = 65,
	scalcoutRecordINPL = 66,
	scalcoutRecordINAA = 67,
	scalcoutRecordINBB = 68,
	scalcoutRecordINCC = 69,
	scalcoutRecordINDD = 70,
	scalcoutRecordINEE = 71,
	scalcoutRecordINFF = 72,
	scalcoutRecordINGG = 73,
	scalcoutRecordINHH = 74,
	scalcoutRecordINII = 75,
	scalcoutRecordINJJ = 76,
	scalcoutRecordINKK = 77,
	scalcoutRecordINLL = 78,
	scalcoutRecordOUT = 79,
	scalcoutRecordINAV = 80,
	scalcoutRecordINBV = 81,
	scalcoutRecordINCV = 82,
	scalcoutRecordINDV = 83,
	scalcoutRecordINEV = 84,
	scalcoutRecordINFV = 85,
	scalcoutRecordINGV = 86,
	scalcoutRecordINHV = 87,
	scalcoutRecordINIV = 88,
	scalcoutRecordINJV = 89,
	scalcoutRecordINKV = 90,
	scalcoutRecordINLV = 91,
	scalcoutRecordIAAV = 92,
	scalcoutRecordIBBV = 93,
	scalcoutRecordICCV = 94,
	scalcoutRecordIDDV = 95,
	scalcoutRecordIEEV = 96,
	scalcoutRecordIFFV = 97,
	scalcoutRecordIGGV = 98,
	scalcoutRecordIHHV = 99,
	scalcoutRecordIIIV = 100,
	scalcoutRecordIJJV = 101,
	scalcoutRecordIKKV = 102,
	scalcoutRecordILLV = 103,
	scalcoutRecordOUTV = 104,
	scalcoutRecordOOPT = 105,
	scalcoutRecordODLY = 106,
	scalcoutRecordWAIT = 107,
	scalcoutRecordDLYA = 108,
	scalcoutRecordDOPT = 109,
	scalcoutRecordOCAL = 110,
	scalcoutRecordOCLV = 111,
	scalcoutRecordOEVT = 112,
	scalcoutRecordIVOA = 113,
	scalcoutRecordIVOV = 114,
	scalcoutRecordEGU = 115,
	scalcoutRecordPREC = 116,
	scalcoutRecordHOPR = 117,
	scalcoutRecordLOPR = 118,
	scalcoutRecordHIHI = 119,
	scalcoutRecordLOLO = 120,
	scalcoutRecordHIGH = 121,
	scalcoutRecordLOW = 122,
	scalcoutRecordHHSV = 123,
	scalcoutRecordLLSV = 124,
	scalcoutRecordHSV = 125,
	scalcoutRecordLSV = 126,
	scalcoutRecordHYST = 127,
	scalcoutRecordADEL = 128,
	scalcoutRecordMDEL = 129,
	scalcoutRecordA = 130,
	scalcoutRecordB = 131,
	scalcoutRecordC = 132,
	scalcoutRecordD = 133,
	scalcoutRecordE = 134,
	scalcoutRecordF = 135,
	scalcoutRecordG = 136,
	scalcoutRecordH = 137,
	scalcoutRecordI = 138,
	scalcoutRecordJ = 139,
	scalcoutRecordK = 140,
	scalcoutRecordL = 141,
	scalcoutRecordSTRS = 142,
	scalcoutRecordAA = 143,
	scalcoutRecordBB = 144,
	scalcoutRecordCC = 145,
	scalcoutRecordDD = 146,
	scalcoutRecordEE = 147,
	scalcoutRecordFF = 148,
	scalcoutRecordGG = 149,
	scalcoutRecordHH = 150,
	scalcoutRecordII = 151,
	scalcoutRecordJJ = 152,
	scalcoutRecordKK = 153,
	scalcoutRecordLL = 154,
	scalcoutRecordPAA = 155,
	scalcoutRecordPBB = 156,
	scalcoutRecordPCC = 157,
	scalcoutRecordPDD = 158,
	scalcoutRecordPEE = 159,
	scalcoutRecordPFF = 160,
	scalcoutRecordPGG = 161,
	scalcoutRecordPHH = 162,
	scalcoutRecordPII = 163,
	scalcoutRecordPJJ = 164,
	scalcoutRecordPKK = 165,
	scalcoutRecordPLL = 166,
	scalcoutRecordOVAL = 167,
	scalcoutRecordOSV = 168,
	scalcoutRecordPOSV = 169,
	scalcoutRecordPA = 170,
	scalcoutRecordPB = 171,
	scalcoutRecordPC = 172,
	scalcoutRecordPD = 173,
	scalcoutRecordPE = 174,
	scalcoutRecordPF = 175,
	scalcoutRecordPG = 176,
	scalcoutRecordPH = 177,
	scalcoutRecordPI = 178,
	scalcoutRecordPJ = 179,
	scalcoutRecordPK = 180,
	scalcoutRecordPL = 181,
	scalcoutRecordPOVL = 182,
	scalcoutRecordLALM = 183,
	scalcoutRecordALST = 184,
	scalcoutRecordMLST = 185,
	scalcoutRecordRPCL = 186,
	scalcoutRecordORPC = 187
} scalcoutFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsAssert.h>
#include <epicsExport.h>
#ifdef __cplusplus
extern "C" {
#endif
static int scalcoutRecordSizeOffset(dbRecordType *prt)
{
    scalcoutRecord *prec = 0;

    assert(prt->no_fields == 188);
    prt->papFldDes[scalcoutRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[scalcoutRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[scalcoutRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[scalcoutRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[scalcoutRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[scalcoutRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[scalcoutRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[scalcoutRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[scalcoutRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[scalcoutRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[scalcoutRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[scalcoutRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[scalcoutRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[scalcoutRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[scalcoutRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[scalcoutRecordBKLNK]->size = sizeof(prec->bklnk);
    prt->papFldDes[scalcoutRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[scalcoutRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[scalcoutRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[scalcoutRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[scalcoutRecordAMSG]->size = sizeof(prec->amsg);
    prt->papFldDes[scalcoutRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[scalcoutRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[scalcoutRecordNAMSG]->size = sizeof(prec->namsg);
    prt->papFldDes[scalcoutRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[scalcoutRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[scalcoutRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[scalcoutRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[scalcoutRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[scalcoutRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[scalcoutRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[scalcoutRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[scalcoutRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[scalcoutRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[scalcoutRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[scalcoutRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[scalcoutRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[scalcoutRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[scalcoutRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[scalcoutRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[scalcoutRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[scalcoutRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[scalcoutRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[scalcoutRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[scalcoutRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[scalcoutRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[scalcoutRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[scalcoutRecordVERS]->size = sizeof(prec->vers);
    prt->papFldDes[scalcoutRecordRPVT]->size = sizeof(prec->rpvt);
    prt->papFldDes[scalcoutRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[scalcoutRecordSVAL]->size = sizeof(prec->sval);
    prt->papFldDes[scalcoutRecordPVAL]->size = sizeof(prec->pval);
    prt->papFldDes[scalcoutRecordPSVL]->size = sizeof(prec->psvl);
    prt->papFldDes[scalcoutRecordCALC]->size = sizeof(prec->calc);
    prt->papFldDes[scalcoutRecordCLCV]->size = sizeof(prec->clcv);
    prt->papFldDes[scalcoutRecordINPA]->size = sizeof(prec->inpa);
    prt->papFldDes[scalcoutRecordINPB]->size = sizeof(prec->inpb);
    prt->papFldDes[scalcoutRecordINPC]->size = sizeof(prec->inpc);
    prt->papFldDes[scalcoutRecordINPD]->size = sizeof(prec->inpd);
    prt->papFldDes[scalcoutRecordINPE]->size = sizeof(prec->inpe);
    prt->papFldDes[scalcoutRecordINPF]->size = sizeof(prec->inpf);
    prt->papFldDes[scalcoutRecordINPG]->size = sizeof(prec->inpg);
    prt->papFldDes[scalcoutRecordINPH]->size = sizeof(prec->inph);
    prt->papFldDes[scalcoutRecordINPI]->size = sizeof(prec->inpi);
    prt->papFldDes[scalcoutRecordINPJ]->size = sizeof(prec->inpj);
    prt->papFldDes[scalcoutRecordINPK]->size = sizeof(prec->inpk);
    prt->papFldDes[scalcoutRecordINPL]->size = sizeof(prec->inpl);
    prt->papFldDes[scalcoutRecordINAA]->size = sizeof(prec->inaa);
    prt->papFldDes[scalcoutRecordINBB]->size = sizeof(prec->inbb);
    prt->papFldDes[scalcoutRecordINCC]->size = sizeof(prec->incc);
    prt->papFldDes[scalcoutRecordINDD]->size = sizeof(prec->indd);
    prt->papFldDes[scalcoutRecordINEE]->size = sizeof(prec->inee);
    prt->papFldDes[scalcoutRecordINFF]->size = sizeof(prec->inff);
    prt->papFldDes[scalcoutRecordINGG]->size = sizeof(prec->ingg);
    prt->papFldDes[scalcoutRecordINHH]->size = sizeof(prec->inhh);
    prt->papFldDes[scalcoutRecordINII]->size = sizeof(prec->inii);
    prt->papFldDes[scalcoutRecordINJJ]->size = sizeof(prec->injj);
    prt->papFldDes[scalcoutRecordINKK]->size = sizeof(prec->inkk);
    prt->papFldDes[scalcoutRecordINLL]->size = sizeof(prec->inll);
    prt->papFldDes[scalcoutRecordOUT]->size = sizeof(prec->out);
    prt->papFldDes[scalcoutRecordINAV]->size = sizeof(prec->inav);
    prt->papFldDes[scalcoutRecordINBV]->size = sizeof(prec->inbv);
    prt->papFldDes[scalcoutRecordINCV]->size = sizeof(prec->incv);
    prt->papFldDes[scalcoutRecordINDV]->size = sizeof(prec->indv);
    prt->papFldDes[scalcoutRecordINEV]->size = sizeof(prec->inev);
    prt->papFldDes[scalcoutRecordINFV]->size = sizeof(prec->infv);
    prt->papFldDes[scalcoutRecordINGV]->size = sizeof(prec->ingv);
    prt->papFldDes[scalcoutRecordINHV]->size = sizeof(prec->inhv);
    prt->papFldDes[scalcoutRecordINIV]->size = sizeof(prec->iniv);
    prt->papFldDes[scalcoutRecordINJV]->size = sizeof(prec->injv);
    prt->papFldDes[scalcoutRecordINKV]->size = sizeof(prec->inkv);
    prt->papFldDes[scalcoutRecordINLV]->size = sizeof(prec->inlv);
    prt->papFldDes[scalcoutRecordIAAV]->size = sizeof(prec->iaav);
    prt->papFldDes[scalcoutRecordIBBV]->size = sizeof(prec->ibbv);
    prt->papFldDes[scalcoutRecordICCV]->size = sizeof(prec->iccv);
    prt->papFldDes[scalcoutRecordIDDV]->size = sizeof(prec->iddv);
    prt->papFldDes[scalcoutRecordIEEV]->size = sizeof(prec->ieev);
    prt->papFldDes[scalcoutRecordIFFV]->size = sizeof(prec->iffv);
    prt->papFldDes[scalcoutRecordIGGV]->size = sizeof(prec->iggv);
    prt->papFldDes[scalcoutRecordIHHV]->size = sizeof(prec->ihhv);
    prt->papFldDes[scalcoutRecordIIIV]->size = sizeof(prec->iiiv);
    prt->papFldDes[scalcoutRecordIJJV]->size = sizeof(prec->ijjv);
    prt->papFldDes[scalcoutRecordIKKV]->size = sizeof(prec->ikkv);
    prt->papFldDes[scalcoutRecordILLV]->size = sizeof(prec->illv);
    prt->papFldDes[scalcoutRecordOUTV]->size = sizeof(prec->outv);
    prt->papFldDes[scalcoutRecordOOPT]->size = sizeof(prec->oopt);
    prt->papFldDes[scalcoutRecordODLY]->size = sizeof(prec->odly);
    prt->papFldDes[scalcoutRecordWAIT]->size = sizeof(prec->wait);
    prt->papFldDes[scalcoutRecordDLYA]->size = sizeof(prec->dlya);
    prt->papFldDes[scalcoutRecordDOPT]->size = sizeof(prec->dopt);
    prt->papFldDes[scalcoutRecordOCAL]->size = sizeof(prec->ocal);
    prt->papFldDes[scalcoutRecordOCLV]->size = sizeof(prec->oclv);
    prt->papFldDes[scalcoutRecordOEVT]->size = sizeof(prec->oevt);
    prt->papFldDes[scalcoutRecordIVOA]->size = sizeof(prec->ivoa);
    prt->papFldDes[scalcoutRecordIVOV]->size = sizeof(prec->ivov);
    prt->papFldDes[scalcoutRecordEGU]->size = sizeof(prec->egu);
    prt->papFldDes[scalcoutRecordPREC]->size = sizeof(prec->prec);
    prt->papFldDes[scalcoutRecordHOPR]->size = sizeof(prec->hopr);
    prt->papFldDes[scalcoutRecordLOPR]->size = sizeof(prec->lopr);
    prt->papFldDes[scalcoutRecordHIHI]->size = sizeof(prec->hihi);
    prt->papFldDes[scalcoutRecordLOLO]->size = sizeof(prec->lolo);
    prt->papFldDes[scalcoutRecordHIGH]->size = sizeof(prec->high);
    prt->papFldDes[scalcoutRecordLOW]->size = sizeof(prec->low);
    prt->papFldDes[scalcoutRecordHHSV]->size = sizeof(prec->hhsv);
    prt->papFldDes[scalcoutRecordLLSV]->size = sizeof(prec->llsv);
    prt->papFldDes[scalcoutRecordHSV]->size = sizeof(prec->hsv);
    prt->papFldDes[scalcoutRecordLSV]->size = sizeof(prec->lsv);
    prt->papFldDes[scalcoutRecordHYST]->size = sizeof(prec->hyst);
    prt->papFldDes[scalcoutRecordADEL]->size = sizeof(prec->adel);
    prt->papFldDes[scalcoutRecordMDEL]->size = sizeof(prec->mdel);
    prt->papFldDes[scalcoutRecordA]->size = sizeof(prec->a);
    prt->papFldDes[scalcoutRecordB]->size = sizeof(prec->b);
    prt->papFldDes[scalcoutRecordC]->size = sizeof(prec->c);
    prt->papFldDes[scalcoutRecordD]->size = sizeof(prec->d);
    prt->papFldDes[scalcoutRecordE]->size = sizeof(prec->e);
    prt->papFldDes[scalcoutRecordF]->size = sizeof(prec->f);
    prt->papFldDes[scalcoutRecordG]->size = sizeof(prec->g);
    prt->papFldDes[scalcoutRecordH]->size = sizeof(prec->h);
    prt->papFldDes[scalcoutRecordI]->size = sizeof(prec->i);
    prt->papFldDes[scalcoutRecordJ]->size = sizeof(prec->j);
    prt->papFldDes[scalcoutRecordK]->size = sizeof(prec->k);
    prt->papFldDes[scalcoutRecordL]->size = sizeof(prec->l);
    prt->papFldDes[scalcoutRecordSTRS]->size = sizeof(prec->strs);
    prt->papFldDes[scalcoutRecordAA]->size = sizeof(prec->aa);
    prt->papFldDes[scalcoutRecordBB]->size = sizeof(prec->bb);
    prt->papFldDes[scalcoutRecordCC]->size = sizeof(prec->cc);
    prt->papFldDes[scalcoutRecordDD]->size = sizeof(prec->dd);
    prt->papFldDes[scalcoutRecordEE]->size = sizeof(prec->ee);
    prt->papFldDes[scalcoutRecordFF]->size = sizeof(prec->ff);
    prt->papFldDes[scalcoutRecordGG]->size = sizeof(prec->gg);
    prt->papFldDes[scalcoutRecordHH]->size = sizeof(prec->hh);
    prt->papFldDes[scalcoutRecordII]->size = sizeof(prec->ii);
    prt->papFldDes[scalcoutRecordJJ]->size = sizeof(prec->jj);
    prt->papFldDes[scalcoutRecordKK]->size = sizeof(prec->kk);
    prt->papFldDes[scalcoutRecordLL]->size = sizeof(prec->ll);
    prt->papFldDes[scalcoutRecordPAA]->size = sizeof(prec->paa);
    prt->papFldDes[scalcoutRecordPBB]->size = sizeof(prec->pbb);
    prt->papFldDes[scalcoutRecordPCC]->size = sizeof(prec->pcc);
    prt->papFldDes[scalcoutRecordPDD]->size = sizeof(prec->pdd);
    prt->papFldDes[scalcoutRecordPEE]->size = sizeof(prec->pee);
    prt->papFldDes[scalcoutRecordPFF]->size = sizeof(prec->pff);
    prt->papFldDes[scalcoutRecordPGG]->size = sizeof(prec->pgg);
    prt->papFldDes[scalcoutRecordPHH]->size = sizeof(prec->phh);
    prt->papFldDes[scalcoutRecordPII]->size = sizeof(prec->pii);
    prt->papFldDes[scalcoutRecordPJJ]->size = sizeof(prec->pjj);
    prt->papFldDes[scalcoutRecordPKK]->size = sizeof(prec->pkk);
    prt->papFldDes[scalcoutRecordPLL]->size = sizeof(prec->pll);
    prt->papFldDes[scalcoutRecordOVAL]->size = sizeof(prec->oval);
    prt->papFldDes[scalcoutRecordOSV]->size = sizeof(prec->osv);
    prt->papFldDes[scalcoutRecordPOSV]->size = sizeof(prec->posv);
    prt->papFldDes[scalcoutRecordPA]->size = sizeof(prec->pa);
    prt->papFldDes[scalcoutRecordPB]->size = sizeof(prec->pb);
    prt->papFldDes[scalcoutRecordPC]->size = sizeof(prec->pc);
    prt->papFldDes[scalcoutRecordPD]->size = sizeof(prec->pd);
    prt->papFldDes[scalcoutRecordPE]->size = sizeof(prec->pe);
    prt->papFldDes[scalcoutRecordPF]->size = sizeof(prec->pf);
    prt->papFldDes[scalcoutRecordPG]->size = sizeof(prec->pg);
    prt->papFldDes[scalcoutRecordPH]->size = sizeof(prec->ph);
    prt->papFldDes[scalcoutRecordPI]->size = sizeof(prec->pi);
    prt->papFldDes[scalcoutRecordPJ]->size = sizeof(prec->pj);
    prt->papFldDes[scalcoutRecordPK]->size = sizeof(prec->pk);
    prt->papFldDes[scalcoutRecordPL]->size = sizeof(prec->pl);
    prt->papFldDes[scalcoutRecordPOVL]->size = sizeof(prec->povl);
    prt->papFldDes[scalcoutRecordLALM]->size = sizeof(prec->lalm);
    prt->papFldDes[scalcoutRecordALST]->size = sizeof(prec->alst);
    prt->papFldDes[scalcoutRecordMLST]->size = sizeof(prec->mlst);
    prt->papFldDes[scalcoutRecordRPCL]->size = sizeof(prec->rpcl);
    prt->papFldDes[scalcoutRecordORPC]->size = sizeof(prec->orpc);
    prt->papFldDes[scalcoutRecordNAME]->offset = (unsigned short)((char *)&prec->name - (char *)prec);
    prt->papFldDes[scalcoutRecordDESC]->offset = (unsigned short)((char *)&prec->desc - (char *)prec);
    prt->papFldDes[scalcoutRecordASG]->offset = (unsigned short)((char *)&prec->asg - (char *)prec);
    prt->papFldDes[scalcoutRecordSCAN]->offset = (unsigned short)((char *)&prec->scan - (char *)prec);
    prt->papFldDes[scalcoutRecordPINI]->offset = (unsigned short)((char *)&prec->pini - (char *)prec);
    prt->papFldDes[scalcoutRecordPHAS]->offset = (unsigned short)((char *)&prec->phas - (char *)prec);
    prt->papFldDes[scalcoutRecordEVNT]->offset = (unsigned short)((char *)&prec->evnt - (char *)prec);
    prt->papFldDes[scalcoutRecordTSE]->offset = (unsigned short)((char *)&prec->tse - (char *)prec);
    prt->papFldDes[scalcoutRecordTSEL]->offset = (unsigned short)((char *)&prec->tsel - (char *)prec);
    prt->papFldDes[scalcoutRecordDTYP]->offset = (unsigned short)((char *)&prec->dtyp - (char *)prec);
    prt->papFldDes[scalcoutRecordDISV]->offset = (unsigned short)((char *)&prec->disv - (char *)prec);
    prt->papFldDes[scalcoutRecordDISA]->offset = (unsigned short)((char *)&prec->disa - (char *)prec);
    prt->papFldDes[scalcoutRecordSDIS]->offset = (unsigned short)((char *)&prec->sdis - (char *)prec);
    prt->papFldDes[scalcoutRecordMLOK]->offset = (unsigned short)((char *)&prec->mlok - (char *)prec);
    prt->papFldDes[scalcoutRecordMLIS]->offset = (unsigned short)((char *)&prec->mlis - (char *)prec);
    prt->papFldDes[scalcoutRecordBKLNK]->offset = (unsigned short)((char *)&prec->bklnk - (char *)prec);
    prt->papFldDes[scalcoutRecordDISP]->offset = (unsigned short)((char *)&prec->disp - (char *)prec);
    prt->papFldDes[scalcoutRecordPROC]->offset = (unsigned short)((char *)&prec->proc - (char *)prec);
    prt->papFldDes[scalcoutRecordSTAT]->offset = (unsigned short)((char *)&prec->stat - (char *)prec);
    prt->papFldDes[scalcoutRecordSEVR]->offset = (unsigned short)((char *)&prec->sevr - (char *)prec);
    prt->papFldDes[scalcoutRecordAMSG]->offset = (unsigned short)((char *)&prec->amsg - (char *)prec);
    prt->papFldDes[scalcoutRecordNSTA]->offset = (unsigned short)((char *)&prec->nsta - (char *)prec);
    prt->papFldDes[scalcoutRecordNSEV]->offset = (unsigned short)((char *)&prec->nsev - (char *)prec);
    prt->papFldDes[scalcoutRecordNAMSG]->offset = (unsigned short)((char *)&prec->namsg - (char *)prec);
    prt->papFldDes[scalcoutRecordACKS]->offset = (unsigned short)((char *)&prec->acks - (char *)prec);
    prt->papFldDes[scalcoutRecordACKT]->offset = (unsigned short)((char *)&prec->ackt - (char *)prec);
    prt->papFldDes[scalcoutRecordDISS]->offset = (unsigned short)((char *)&prec->diss - (char *)prec);
    prt->papFldDes[scalcoutRecordLCNT]->offset = (unsigned short)((char *)&prec->lcnt - (char *)prec);
    prt->papFldDes[scalcoutRecordPACT]->offset = (unsigned short)((char *)&prec->pact - (char *)prec);
    prt->papFldDes[scalcoutRecordPUTF]->offset = (unsigned short)((char *)&prec->putf - (char *)prec);
    prt->papFldDes[scalcoutRecordRPRO]->offset = (unsigned short)((char *)&prec->rpro - (char *)prec);
    prt->papFldDes[scalcoutRecordASP]->offset = (unsigned short)((char *)&prec->asp - (char *)prec);
    prt->papFldDes[scalcoutRecordPPN]->offset = (unsigned short)((char *)&prec->ppn - (char *)prec);
    prt->papFldDes[scalcoutRecordPPNR]->offset = (unsigned short)((char *)&prec->ppnr - (char *)prec);
    prt->papFldDes[scalcoutRecordSPVT]->offset = (unsigned short)((char *)&prec->spvt - (char *)prec);
    prt->papFldDes[scalcoutRecordRSET]->offset = (unsigned short)((char *)&prec->rset - (char *)prec);
    prt->papFldDes[scalcoutRecordDSET]->offset = (unsigned short)((char *)&prec->dset - (char *)prec);
    prt->papFldDes[scalcoutRecordDPVT]->offset = (unsigned short)((char *)&prec->dpvt - (char *)prec);
    prt->papFldDes[scalcoutRecordRDES]->offset = (unsigned short)((char *)&prec->rdes - (char *)prec);
    prt->papFldDes[scalcoutRecordLSET]->offset = (unsigned short)((char *)&prec->lset - (char *)prec);
    prt->papFldDes[scalcoutRecordPRIO]->offset = (unsigned short)((char *)&prec->prio - (char *)prec);
    prt->papFldDes[scalcoutRecordTPRO]->offset = (unsigned short)((char *)&prec->tpro - (char *)prec);
    prt->papFldDes[scalcoutRecordBKPT]->offset = (unsigned short)((char *)&prec->bkpt - (char *)prec);
    prt->papFldDes[scalcoutRecordUDF]->offset = (unsigned short)((char *)&prec->udf - (char *)prec);
    prt->papFldDes[scalcoutRecordUDFS]->offset = (unsigned short)((char *)&prec->udfs - (char *)prec);
    prt->papFldDes[scalcoutRecordTIME]->offset = (unsigned short)((char *)&prec->time - (char *)prec);
    prt->papFldDes[scalcoutRecordFLNK]->offset = (unsigned short)((char *)&prec->flnk - (char *)prec);
    prt->papFldDes[scalcoutRecordVERS]->offset = (unsigned short)((char *)&prec->vers - (char *)prec);
    prt->papFldDes[scalcoutRecordRPVT]->offset = (unsigned short)((char *)&prec->rpvt - (char *)prec);
    prt->papFldDes[scalcoutRecordVAL]->offset = (unsigned short)((char *)&prec->val - (char *)prec);
    prt->papFldDes[scalcoutRecordSVAL]->offset = (unsigned short)((char *)&prec->sval - (char *)prec);
    prt->papFldDes[scalcoutRecordPVAL]->offset = (unsigned short)((char *)&prec->pval - (char *)prec);
    prt->papFldDes[scalcoutRecordPSVL]->offset = (unsigned short)((char *)&prec->psvl - (char *)prec);
    prt->papFldDes[scalcoutRecordCALC]->offset = (unsigned short)((char *)&prec->calc - (char *)prec);
    prt->papFldDes[scalcoutRecordCLCV]->offset = (unsigned short)((char *)&prec->clcv - (char *)prec);
    prt->papFldDes[scalcoutRecordINPA]->offset = (unsigned short)((char *)&prec->inpa - (char *)prec);
    prt->papFldDes[scalcoutRecordINPB]->offset = (unsigned short)((char *)&prec->inpb - (char *)prec);
    prt->papFldDes[scalcoutRecordINPC]->offset = (unsigned short)((char *)&prec->inpc - (char *)prec);
    prt->papFldDes[scalcoutRecordINPD]->offset = (unsigned short)((char *)&prec->inpd - (char *)prec);
    prt->papFldDes[scalcoutRecordINPE]->offset = (unsigned short)((char *)&prec->inpe - (char *)prec);
    prt->papFldDes[scalcoutRecordINPF]->offset = (unsigned short)((char *)&prec->inpf - (char *)prec);
    prt->papFldDes[scalcoutRecordINPG]->offset = (unsigned short)((char *)&prec->inpg - (char *)prec);
    prt->papFldDes[scalcoutRecordINPH]->offset = (unsigned short)((char *)&prec->inph - (char *)prec);
    prt->papFldDes[scalcoutRecordINPI]->offset = (unsigned short)((char *)&prec->inpi - (char *)prec);
    prt->papFldDes[scalcoutRecordINPJ]->offset = (unsigned short)((char *)&prec->inpj - (char *)prec);
    prt->papFldDes[scalcoutRecordINPK]->offset = (unsigned short)((char *)&prec->inpk - (char *)prec);
    prt->papFldDes[scalcoutRecordINPL]->offset = (unsigned short)((char *)&prec->inpl - (char *)prec);
    prt->papFldDes[scalcoutRecordINAA]->offset = (unsigned short)((char *)&prec->inaa - (char *)prec);
    prt->papFldDes[scalcoutRecordINBB]->offset = (unsigned short)((char *)&prec->inbb - (char *)prec);
    prt->papFldDes[scalcoutRecordINCC]->offset = (unsigned short)((char *)&prec->incc - (char *)prec);
    prt->papFldDes[scalcoutRecordINDD]->offset = (unsigned short)((char *)&prec->indd - (char *)prec);
    prt->papFldDes[scalcoutRecordINEE]->offset = (unsigned short)((char *)&prec->inee - (char *)prec);
    prt->papFldDes[scalcoutRecordINFF]->offset = (unsigned short)((char *)&prec->inff - (char *)prec);
    prt->papFldDes[scalcoutRecordINGG]->offset = (unsigned short)((char *)&prec->ingg - (char *)prec);
    prt->papFldDes[scalcoutRecordINHH]->offset = (unsigned short)((char *)&prec->inhh - (char *)prec);
    prt->papFldDes[scalcoutRecordINII]->offset = (unsigned short)((char *)&prec->inii - (char *)prec);
    prt->papFldDes[scalcoutRecordINJJ]->offset = (unsigned short)((char *)&prec->injj - (char *)prec);
    prt->papFldDes[scalcoutRecordINKK]->offset = (unsigned short)((char *)&prec->inkk - (char *)prec);
    prt->papFldDes[scalcoutRecordINLL]->offset = (unsigned short)((char *)&prec->inll - (char *)prec);
    prt->papFldDes[scalcoutRecordOUT]->offset = (unsigned short)((char *)&prec->out - (char *)prec);
    prt->papFldDes[scalcoutRecordINAV]->offset = (unsigned short)((char *)&prec->inav - (char *)prec);
    prt->papFldDes[scalcoutRecordINBV]->offset = (unsigned short)((char *)&prec->inbv - (char *)prec);
    prt->papFldDes[scalcoutRecordINCV]->offset = (unsigned short)((char *)&prec->incv - (char *)prec);
    prt->papFldDes[scalcoutRecordINDV]->offset = (unsigned short)((char *)&prec->indv - (char *)prec);
    prt->papFldDes[scalcoutRecordINEV]->offset = (unsigned short)((char *)&prec->inev - (char *)prec);
    prt->papFldDes[scalcoutRecordINFV]->offset = (unsigned short)((char *)&prec->infv - (char *)prec);
    prt->papFldDes[scalcoutRecordINGV]->offset = (unsigned short)((char *)&prec->ingv - (char *)prec);
    prt->papFldDes[scalcoutRecordINHV]->offset = (unsigned short)((char *)&prec->inhv - (char *)prec);
    prt->papFldDes[scalcoutRecordINIV]->offset = (unsigned short)((char *)&prec->iniv - (char *)prec);
    prt->papFldDes[scalcoutRecordINJV]->offset = (unsigned short)((char *)&prec->injv - (char *)prec);
    prt->papFldDes[scalcoutRecordINKV]->offset = (unsigned short)((char *)&prec->inkv - (char *)prec);
    prt->papFldDes[scalcoutRecordINLV]->offset = (unsigned short)((char *)&prec->inlv - (char *)prec);
    prt->papFldDes[scalcoutRecordIAAV]->offset = (unsigned short)((char *)&prec->iaav - (char *)prec);
    prt->papFldDes[scalcoutRecordIBBV]->offset = (unsigned short)((char *)&prec->ibbv - (char *)prec);
    prt->papFldDes[scalcoutRecordICCV]->offset = (unsigned short)((char *)&prec->iccv - (char *)prec);
    prt->papFldDes[scalcoutRecordIDDV]->offset = (unsigned short)((char *)&prec->iddv - (char *)prec);
    prt->papFldDes[scalcoutRecordIEEV]->offset = (unsigned short)((char *)&prec->ieev - (char *)prec);
    prt->papFldDes[scalcoutRecordIFFV]->offset = (unsigned short)((char *)&prec->iffv - (char *)prec);
    prt->papFldDes[scalcoutRecordIGGV]->offset = (unsigned short)((char *)&prec->iggv - (char *)prec);
    prt->papFldDes[scalcoutRecordIHHV]->offset = (unsigned short)((char *)&prec->ihhv - (char *)prec);
    prt->papFldDes[scalcoutRecordIIIV]->offset = (unsigned short)((char *)&prec->iiiv - (char *)prec);
    prt->papFldDes[scalcoutRecordIJJV]->offset = (unsigned short)((char *)&prec->ijjv - (char *)prec);
    prt->papFldDes[scalcoutRecordIKKV]->offset = (unsigned short)((char *)&prec->ikkv - (char *)prec);
    prt->papFldDes[scalcoutRecordILLV]->offset = (unsigned short)((char *)&prec->illv - (char *)prec);
    prt->papFldDes[scalcoutRecordOUTV]->offset = (unsigned short)((char *)&prec->outv - (char *)prec);
    prt->papFldDes[scalcoutRecordOOPT]->offset = (unsigned short)((char *)&prec->oopt - (char *)prec);
    prt->papFldDes[scalcoutRecordODLY]->offset = (unsigned short)((char *)&prec->odly - (char *)prec);
    prt->papFldDes[scalcoutRecordWAIT]->offset = (unsigned short)((char *)&prec->wait - (char *)prec);
    prt->papFldDes[scalcoutRecordDLYA]->offset = (unsigned short)((char *)&prec->dlya - (char *)prec);
    prt->papFldDes[scalcoutRecordDOPT]->offset = (unsigned short)((char *)&prec->dopt - (char *)prec);
    prt->papFldDes[scalcoutRecordOCAL]->offset = (unsigned short)((char *)&prec->ocal - (char *)prec);
    prt->papFldDes[scalcoutRecordOCLV]->offset = (unsigned short)((char *)&prec->oclv - (char *)prec);
    prt->papFldDes[scalcoutRecordOEVT]->offset = (unsigned short)((char *)&prec->oevt - (char *)prec);
    prt->papFldDes[scalcoutRecordIVOA]->offset = (unsigned short)((char *)&prec->ivoa - (char *)prec);
    prt->papFldDes[scalcoutRecordIVOV]->offset = (unsigned short)((char *)&prec->ivov - (char *)prec);
    prt->papFldDes[scalcoutRecordEGU]->offset = (unsigned short)((char *)&prec->egu - (char *)prec);
    prt->papFldDes[scalcoutRecordPREC]->offset = (unsigned short)((char *)&prec->prec - (char *)prec);
    prt->papFldDes[scalcoutRecordHOPR]->offset = (unsigned short)((char *)&prec->hopr - (char *)prec);
    prt->papFldDes[scalcoutRecordLOPR]->offset = (unsigned short)((char *)&prec->lopr - (char *)prec);
    prt->papFldDes[scalcoutRecordHIHI]->offset = (unsigned short)((char *)&prec->hihi - (char *)prec);
    prt->papFldDes[scalcoutRecordLOLO]->offset = (unsigned short)((char *)&prec->lolo - (char *)prec);
    prt->papFldDes[scalcoutRecordHIGH]->offset = (unsigned short)((char *)&prec->high - (char *)prec);
    prt->papFldDes[scalcoutRecordLOW]->offset = (unsigned short)((char *)&prec->low - (char *)prec);
    prt->papFldDes[scalcoutRecordHHSV]->offset = (unsigned short)((char *)&prec->hhsv - (char *)prec);
    prt->papFldDes[scalcoutRecordLLSV]->offset = (unsigned short)((char *)&prec->llsv - (char *)prec);
    prt->papFldDes[scalcoutRecordHSV]->offset = (unsigned short)((char *)&prec->hsv - (char *)prec);
    prt->papFldDes[scalcoutRecordLSV]->offset = (unsigned short)((char *)&prec->lsv - (char *)prec);
    prt->papFldDes[scalcoutRecordHYST]->offset = (unsigned short)((char *)&prec->hyst - (char *)prec);
    prt->papFldDes[scalcoutRecordADEL]->offset = (unsigned short)((char *)&prec->adel - (char *)prec);
    prt->papFldDes[scalcoutRecordMDEL]->offset = (unsigned short)((char *)&prec->mdel - (char *)prec);
    prt->papFldDes[scalcoutRecordA]->offset = (unsigned short)((char *)&prec->a - (char *)prec);
    prt->papFldDes[scalcoutRecordB]->offset = (unsigned short)((char *)&prec->b - (char *)prec);
    prt->papFldDes[scalcoutRecordC]->offset = (unsigned short)((char *)&prec->c - (char *)prec);
    prt->papFldDes[scalcoutRecordD]->offset = (unsigned short)((char *)&prec->d - (char *)prec);
    prt->papFldDes[scalcoutRecordE]->offset = (unsigned short)((char *)&prec->e - (char *)prec);
    prt->papFldDes[scalcoutRecordF]->offset = (unsigned short)((char *)&prec->f - (char *)prec);
    prt->papFldDes[scalcoutRecordG]->offset = (unsigned short)((char *)&prec->g - (char *)prec);
    prt->papFldDes[scalcoutRecordH]->offset = (unsigned short)((char *)&prec->h - (char *)prec);
    prt->papFldDes[scalcoutRecordI]->offset = (unsigned short)((char *)&prec->i - (char *)prec);
    prt->papFldDes[scalcoutRecordJ]->offset = (unsigned short)((char *)&prec->j - (char *)prec);
    prt->papFldDes[scalcoutRecordK]->offset = (unsigned short)((char *)&prec->k - (char *)prec);
    prt->papFldDes[scalcoutRecordL]->offset = (unsigned short)((char *)&prec->l - (char *)prec);
    prt->papFldDes[scalcoutRecordSTRS]->offset = (unsigned short)((char *)&prec->strs - (char *)prec);
    prt->papFldDes[scalcoutRecordAA]->offset = (unsigned short)((char *)&prec->aa - (char *)prec);
    prt->papFldDes[scalcoutRecordBB]->offset = (unsigned short)((char *)&prec->bb - (char *)prec);
    prt->papFldDes[scalcoutRecordCC]->offset = (unsigned short)((char *)&prec->cc - (char *)prec);
    prt->papFldDes[scalcoutRecordDD]->offset = (unsigned short)((char *)&prec->dd - (char *)prec);
    prt->papFldDes[scalcoutRecordEE]->offset = (unsigned short)((char *)&prec->ee - (char *)prec);
    prt->papFldDes[scalcoutRecordFF]->offset = (unsigned short)((char *)&prec->ff - (char *)prec);
    prt->papFldDes[scalcoutRecordGG]->offset = (unsigned short)((char *)&prec->gg - (char *)prec);
    prt->papFldDes[scalcoutRecordHH]->offset = (unsigned short)((char *)&prec->hh - (char *)prec);
    prt->papFldDes[scalcoutRecordII]->offset = (unsigned short)((char *)&prec->ii - (char *)prec);
    prt->papFldDes[scalcoutRecordJJ]->offset = (unsigned short)((char *)&prec->jj - (char *)prec);
    prt->papFldDes[scalcoutRecordKK]->offset = (unsigned short)((char *)&prec->kk - (char *)prec);
    prt->papFldDes[scalcoutRecordLL]->offset = (unsigned short)((char *)&prec->ll - (char *)prec);
    prt->papFldDes[scalcoutRecordPAA]->offset = (unsigned short)((char *)&prec->paa - (char *)prec);
    prt->papFldDes[scalcoutRecordPBB]->offset = (unsigned short)((char *)&prec->pbb - (char *)prec);
    prt->papFldDes[scalcoutRecordPCC]->offset = (unsigned short)((char *)&prec->pcc - (char *)prec);
    prt->papFldDes[scalcoutRecordPDD]->offset = (unsigned short)((char *)&prec->pdd - (char *)prec);
    prt->papFldDes[scalcoutRecordPEE]->offset = (unsigned short)((char *)&prec->pee - (char *)prec);
    prt->papFldDes[scalcoutRecordPFF]->offset = (unsigned short)((char *)&prec->pff - (char *)prec);
    prt->papFldDes[scalcoutRecordPGG]->offset = (unsigned short)((char *)&prec->pgg - (char *)prec);
    prt->papFldDes[scalcoutRecordPHH]->offset = (unsigned short)((char *)&prec->phh - (char *)prec);
    prt->papFldDes[scalcoutRecordPII]->offset = (unsigned short)((char *)&prec->pii - (char *)prec);
    prt->papFldDes[scalcoutRecordPJJ]->offset = (unsigned short)((char *)&prec->pjj - (char *)prec);
    prt->papFldDes[scalcoutRecordPKK]->offset = (unsigned short)((char *)&prec->pkk - (char *)prec);
    prt->papFldDes[scalcoutRecordPLL]->offset = (unsigned short)((char *)&prec->pll - (char *)prec);
    prt->papFldDes[scalcoutRecordOVAL]->offset = (unsigned short)((char *)&prec->oval - (char *)prec);
    prt->papFldDes[scalcoutRecordOSV]->offset = (unsigned short)((char *)&prec->osv - (char *)prec);
    prt->papFldDes[scalcoutRecordPOSV]->offset = (unsigned short)((char *)&prec->posv - (char *)prec);
    prt->papFldDes[scalcoutRecordPA]->offset = (unsigned short)((char *)&prec->pa - (char *)prec);
    prt->papFldDes[scalcoutRecordPB]->offset = (unsigned short)((char *)&prec->pb - (char *)prec);
    prt->papFldDes[scalcoutRecordPC]->offset = (unsigned short)((char *)&prec->pc - (char *)prec);
    prt->papFldDes[scalcoutRecordPD]->offset = (unsigned short)((char *)&prec->pd - (char *)prec);
    prt->papFldDes[scalcoutRecordPE]->offset = (unsigned short)((char *)&prec->pe - (char *)prec);
    prt->papFldDes[scalcoutRecordPF]->offset = (unsigned short)((char *)&prec->pf - (char *)prec);
    prt->papFldDes[scalcoutRecordPG]->offset = (unsigned short)((char *)&prec->pg - (char *)prec);
    prt->papFldDes[scalcoutRecordPH]->offset = (unsigned short)((char *)&prec->ph - (char *)prec);
    prt->papFldDes[scalcoutRecordPI]->offset = (unsigned short)((char *)&prec->pi - (char *)prec);
    prt->papFldDes[scalcoutRecordPJ]->offset = (unsigned short)((char *)&prec->pj - (char *)prec);
    prt->papFldDes[scalcoutRecordPK]->offset = (unsigned short)((char *)&prec->pk - (char *)prec);
    prt->papFldDes[scalcoutRecordPL]->offset = (unsigned short)((char *)&prec->pl - (char *)prec);
    prt->papFldDes[scalcoutRecordPOVL]->offset = (unsigned short)((char *)&prec->povl - (char *)prec);
    prt->papFldDes[scalcoutRecordLALM]->offset = (unsigned short)((char *)&prec->lalm - (char *)prec);
    prt->papFldDes[scalcoutRecordALST]->offset = (unsigned short)((char *)&prec->alst - (char *)prec);
    prt->papFldDes[scalcoutRecordMLST]->offset = (unsigned short)((char *)&prec->mlst - (char *)prec);
    prt->papFldDes[scalcoutRecordRPCL]->offset = (unsigned short)((char *)&prec->rpcl - (char *)prec);
    prt->papFldDes[scalcoutRecordORPC]->offset = (unsigned short)((char *)&prec->orpc - (char *)prec);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(scalcoutRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_sCalcoutRecord_H */
