/* motorRecord.h generated from motorRecord.dbd */

#ifndef INC_motorRecord_H
#define INC_motorRecord_H

#include "epicsTypes.h"
#include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "epicsTime.h"

#ifndef menuOmsl_NUM_CHOICES
typedef enum {
    menuOmslsupervisory             /* supervisory */,
    menuOmslclosed_loop             /* closed_loop */
} menuOmsl;
#define menuOmsl_NUM_CHOICES 2
#endif

#ifndef motorDIR_NUM_CHOICES
typedef enum {
    motorDIR_Pos                    /* Pos */,
    motorDIR_Neg                    /* Neg */
} motorDIR;
#define motorDIR_NUM_CHOICES 2
#endif

#ifndef menuYesNo_NUM_CHOICES
typedef enum {
    menuYesNoNO                     /* NO */,
    menuYesNoYES                    /* YES */
} menuYesNo;
#define menuYesNo_NUM_CHOICES 2
#endif

#ifndef motorMODE_NUM_CHOICES
typedef enum {
    motorMODE_Position              /* Position */,
    motorMODE_Velocity              /* Velocity */
} motorMODE;
#define motorMODE_NUM_CHOICES 2
#endif

#ifndef motorFOFF_NUM_CHOICES
typedef enum {
    motorFOFF_Variable              /* Variable */,
    motorFOFF_Frozen                /* Frozen */
} motorFOFF;
#define motorFOFF_NUM_CHOICES 2
#endif

#ifndef motorSET_NUM_CHOICES
typedef enum {
    motorSET_Use                    /* Use */,
    motorSET_Set                    /* Set */
} motorSET;
#define motorSET_NUM_CHOICES 2
#endif

#ifndef motorUEIP_NUM_CHOICES
typedef enum {
    motorUEIP_No                    /* No */,
    motorUEIP_Yes                   /* Yes */
} motorUEIP;
#define motorUEIP_NUM_CHOICES 2
#endif

#ifndef motorSTUP_NUM_CHOICES
typedef enum {
    motorSTUP_OFF                   /* OFF */,
    motorSTUP_ON                    /* ON */,
    motorSTUP_BUSY                  /* BUSY */
} motorSTUP;
#define motorSTUP_NUM_CHOICES 3
#endif

#ifndef motorRMOD_NUM_CHOICES
typedef enum {
    motorRMOD_D                     /* Default */,
    motorRMOD_A                     /* Arithmetic */,
    motorRMOD_G                     /* Geometric */,
    motorRMOD_I                     /* In-Position */
} motorRMOD;
#define motorRMOD_NUM_CHOICES 4
#endif

#ifndef motorTORQ_NUM_CHOICES
typedef enum {
    motorTORQ_Disable               /* Disable */,
    motorTORQ_Enable                /* Enable */
} motorTORQ;
#define motorTORQ_NUM_CHOICES 2
#endif

#ifndef motorSPMG_NUM_CHOICES
typedef enum {
    motorSPMG_Stop                  /* Stop */,
    motorSPMG_Pause                 /* Pause */,
    motorSPMG_Move                  /* Move */,
    motorSPMG_Go                    /* Go */
} motorSPMG;
#define motorSPMG_NUM_CHOICES 4
#endif

typedef struct motorRecord {
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
    epicsFloat32        vers;       /* Code Version */
    epicsFloat64        off;        /* User Offset (EGU) */
    epicsEnum16         foff;       /* Offset-Freeze Switch */
    epicsInt16          fof;        /* Freeze Offset */
    epicsInt16          vof;        /* Variable Offset */
    epicsEnum16         dir;        /* User Direction */
    epicsEnum16         set;        /* Set/Use Switch */
    epicsInt16          sset;       /* Set SET Mode */
    epicsInt16          suse;       /* Set USE Mode */
    epicsFloat64        velo;       /* Velocity (EGU/s) */
    epicsFloat64        vbas;       /* Base Velocity (EGU/s) */
    epicsFloat64        vmax;       /* Max. Velocity (EGU/s) */
    epicsFloat64        s;          /* Speed (revolutions/sec) */
    epicsFloat64        sbas;       /* Base Speed (RPS) */
    epicsFloat64        smax;       /* Max. Speed (RPS) */
    epicsFloat64        accl;       /* Seconds to Velocity */
    epicsFloat64        bdst;       /* BL Distance (EGU) */
    epicsFloat64        bvel;       /* BL Velocity (EGU/s) */
    epicsFloat64        sbak;       /* BL Speed (RPS) */
    epicsFloat64        bacc;       /* BL Seconds to Velocity */
    epicsFloat32        frac;       /* Move Fraction */
    DBLINK              out;        /* Output Specification */
    epicsInt16          card;       /* Card Number */
    DBLINK              rdbl;       /* Readback Location */
    DBLINK              dol;        /* Desired Output Loc */
    epicsEnum16         omsl;       /* Output Mode Select */
    DBLINK              rlnk;       /* Readback OutLink */
    epicsInt32          srev;       /* Steps per Revolution */
    epicsFloat64        urev;       /* EGU's per Revolution */
    epicsFloat64        mres;       /* Motor Step Size (EGU) */
    epicsFloat64        eres;       /* Encoder Step Size (EGU) */
    epicsFloat64        rres;       /* Readback Step Size (EGU */
    epicsEnum16         ueip;       /* Use Encoder If Present */
    epicsEnum16         urip;       /* Use RDBL Link If Presen */
    epicsInt16          prec;       /* Display Precision */
    char                egu[16];    /* Engineering Units */
    epicsFloat64        hlm;        /* User High Limit */
    epicsFloat64        llm;        /* User Low Limit */
    epicsFloat64        dhlm;       /* Dial High Limit */
    epicsFloat64        dllm;       /* Dial Low Limit */
    epicsFloat64        hopr;       /* High Operating Range */
    epicsFloat64        lopr;       /* Low Operating Range */
    epicsInt16          hls;        /* User High Limit Switch */
    epicsInt16          lls;        /* User Low Limit Switch */
    epicsInt16          rhls;       /* Raw High Limit Switch */
    epicsInt16          rlls;       /* Raw Low Limit Switch */
    epicsFloat64        hihi;       /* Hihi Alarm Limit (EGU) */
    epicsFloat64        lolo;       /* Lolo Alarm Limit (EGU) */
    epicsFloat64        high;       /* High Alarm Limit (EGU) */
    epicsFloat64        low;        /* Low Alarm Limit (EGU) */
    epicsEnum16         hhsv;       /* Hihi Severity */
    epicsEnum16         llsv;       /* Lolo Severity */
    epicsEnum16         hsv;        /* High Severity */
    epicsEnum16         lsv;        /* Low Severity */
    epicsEnum16         hlsv;       /* HW Limit Violation Svr */
    epicsFloat64        rdbd;       /* Retry Deadband (EGU) */
    epicsInt16          rcnt;       /* Retry count */
    epicsInt16          rtry;       /* Max retry count */
    epicsInt16          miss;       /* Ran out of retries */
    epicsEnum16         spmg;       /* Stop/Pause/Move/Go */
    epicsEnum16         lspg;       /* Last SPMG */
    epicsInt16          stop;       /* Stop */
    epicsInt16          homf;       /* Home Forward */
    epicsInt16          homr;       /* Home Reverse */
    epicsInt16          jogf;       /* Jog motor Forward */
    epicsInt16          jogr;       /* Jog motor Reverse */
    epicsInt16          twf;        /* Tweak motor Forward */
    epicsInt16          twr;        /* Tweak motor Reverse */
    epicsFloat64        twv;        /* Tweak Step Size (EGU) */
    epicsFloat64        val;        /* User Desired Value (EGU */
    epicsFloat64        lval;       /* Last User Des Val (EGU) */
    epicsFloat64        dval;       /* Dial Desired Value (EGU */
    epicsFloat64        ldvl;       /* Last Dial Des Val (EGU) */
    epicsInt32          rval;       /* Raw Desired Value (step */
    epicsInt32          lrvl;       /* Last Raw Des Val (steps */
    epicsFloat64        rlv;        /* Relative Value (EGU) */
    epicsFloat64        lrlv;       /* Last Rel Value (EGU) */
    epicsFloat64        rbv;        /* User Readback Value */
    epicsFloat64        drbv;       /* Dial Readback Value */
    epicsFloat64        diff;       /* Difference dval-drbv */
    epicsInt32          rdif;       /* Difference rval-rrbv */
    epicsInt16          cdir;       /* Raw cmnd direction */
    epicsInt32          rrbv;       /* Raw Readback Value */
    epicsInt32          rmp;        /* Raw Motor Position */
    epicsInt32          rep;        /* Raw Encoder Position */
    epicsInt32          rvel;       /* Raw Velocity */
    epicsInt16          dmov;       /* Done moving to value */
    epicsInt16          movn;       /* Motor is moving */
    epicsUInt32         msta;       /* Motor Status */
    epicsInt16          lvio;       /* Limit violation */
    epicsInt16          tdir;       /* Direction of Travel */
    epicsInt16          athm;       /* At HOME */
    epicsInt16          pp;         /* Post process command */
    epicsUInt16         mip;        /* Motion In Progress */
    epicsUInt32         mmap;       /* Monitor Mask */
    epicsUInt32         nmap;       /* Monitor Mask (more) */
    epicsFloat64        dly;        /* Readback settle time (s) */
    void             *cbak;         /* Callback structure */
    epicsFloat64        pcof;       /* Proportional Gain */
    epicsFloat64        icof;       /* Integral Gain */
    epicsFloat64        dcof;       /* Derivative Gain */
    epicsEnum16         cnen;       /* Enable control */
    char                init[40];   /* Startup commands */
    char                prem[40];   /* Pre-move commands */
    char                post[40];   /* Post-move commands */
    DBLINK              stoo;       /* STOP OutLink */
    DBLINK              dinp;       /* DMOV Input Link */
    DBLINK              rinp;       /* RMP Input Link */
    epicsFloat64        jvel;       /* Jog Velocity (EGU/s) */
    epicsFloat64        jar;        /* Jog Accel. (EGU/s^2) */
    epicsEnum16         lock;       /* Soft Channel Position Lock */
    epicsEnum16         ntm;        /* New Target Monitor */
    epicsUInt16         ntmf;       /* NTM Deadband Factor */
    epicsFloat64        hvel;       /* Home Velocity (EGU/s) */
    epicsEnum16         stup;       /* Status Update */
    epicsEnum16         rmod;       /* Retry Mode */
    epicsFloat64        adel;       /* Archive Deadband */
    epicsFloat64        mdel;       /* Monitor Deadband */
    epicsFloat64        alst;       /* Last Value Archived */
    epicsFloat64        mlst;       /* Last Val Monitored */
    epicsInt16          sync;       /* Sync position */
    epicsInt16          igset;      /* Ignore SET field */
} motorRecord;

typedef enum {
	motorRecordNAME = 0,
	motorRecordDESC = 1,
	motorRecordASG = 2,
	motorRecordSCAN = 3,
	motorRecordPINI = 4,
	motorRecordPHAS = 5,
	motorRecordEVNT = 6,
	motorRecordTSE = 7,
	motorRecordTSEL = 8,
	motorRecordDTYP = 9,
	motorRecordDISV = 10,
	motorRecordDISA = 11,
	motorRecordSDIS = 12,
	motorRecordMLOK = 13,
	motorRecordMLIS = 14,
	motorRecordBKLNK = 15,
	motorRecordDISP = 16,
	motorRecordPROC = 17,
	motorRecordSTAT = 18,
	motorRecordSEVR = 19,
	motorRecordNSTA = 20,
	motorRecordNSEV = 21,
	motorRecordACKS = 22,
	motorRecordACKT = 23,
	motorRecordDISS = 24,
	motorRecordLCNT = 25,
	motorRecordPACT = 26,
	motorRecordPUTF = 27,
	motorRecordRPRO = 28,
	motorRecordASP = 29,
	motorRecordPPN = 30,
	motorRecordPPNR = 31,
	motorRecordSPVT = 32,
	motorRecordRSET = 33,
	motorRecordDSET = 34,
	motorRecordDPVT = 35,
	motorRecordRDES = 36,
	motorRecordLSET = 37,
	motorRecordPRIO = 38,
	motorRecordTPRO = 39,
	motorRecordBKPT = 40,
	motorRecordUDF = 41,
	motorRecordUDFS = 42,
	motorRecordTIME = 43,
	motorRecordFLNK = 44,
	motorRecordVERS = 45,
	motorRecordOFF = 46,
	motorRecordFOFF = 47,
	motorRecordFOF = 48,
	motorRecordVOF = 49,
	motorRecordDIR = 50,
	motorRecordSET = 51,
	motorRecordSSET = 52,
	motorRecordSUSE = 53,
	motorRecordVELO = 54,
	motorRecordVBAS = 55,
	motorRecordVMAX = 56,
	motorRecordS = 57,
	motorRecordSBAS = 58,
	motorRecordSMAX = 59,
	motorRecordACCL = 60,
	motorRecordBDST = 61,
	motorRecordBVEL = 62,
	motorRecordSBAK = 63,
	motorRecordBACC = 64,
	motorRecordFRAC = 65,
	motorRecordOUT = 66,
	motorRecordCARD = 67,
	motorRecordRDBL = 68,
	motorRecordDOL = 69,
	motorRecordOMSL = 70,
	motorRecordRLNK = 71,
	motorRecordSREV = 72,
	motorRecordUREV = 73,
	motorRecordMRES = 74,
	motorRecordERES = 75,
	motorRecordRRES = 76,
	motorRecordUEIP = 77,
	motorRecordURIP = 78,
	motorRecordPREC = 79,
	motorRecordEGU = 80,
	motorRecordHLM = 81,
	motorRecordLLM = 82,
	motorRecordDHLM = 83,
	motorRecordDLLM = 84,
	motorRecordHOPR = 85,
	motorRecordLOPR = 86,
	motorRecordHLS = 87,
	motorRecordLLS = 88,
	motorRecordRHLS = 89,
	motorRecordRLLS = 90,
	motorRecordHIHI = 91,
	motorRecordLOLO = 92,
	motorRecordHIGH = 93,
	motorRecordLOW = 94,
	motorRecordHHSV = 95,
	motorRecordLLSV = 96,
	motorRecordHSV = 97,
	motorRecordLSV = 98,
	motorRecordHLSV = 99,
	motorRecordRDBD = 100,
	motorRecordRCNT = 101,
	motorRecordRTRY = 102,
	motorRecordMISS = 103,
	motorRecordSPMG = 104,
	motorRecordLSPG = 105,
	motorRecordSTOP = 106,
	motorRecordHOMF = 107,
	motorRecordHOMR = 108,
	motorRecordJOGF = 109,
	motorRecordJOGR = 110,
	motorRecordTWF = 111,
	motorRecordTWR = 112,
	motorRecordTWV = 113,
	motorRecordVAL = 114,
	motorRecordLVAL = 115,
	motorRecordDVAL = 116,
	motorRecordLDVL = 117,
	motorRecordRVAL = 118,
	motorRecordLRVL = 119,
	motorRecordRLV = 120,
	motorRecordLRLV = 121,
	motorRecordRBV = 122,
	motorRecordDRBV = 123,
	motorRecordDIFF = 124,
	motorRecordRDIF = 125,
	motorRecordCDIR = 126,
	motorRecordRRBV = 127,
	motorRecordRMP = 128,
	motorRecordREP = 129,
	motorRecordRVEL = 130,
	motorRecordDMOV = 131,
	motorRecordMOVN = 132,
	motorRecordMSTA = 133,
	motorRecordLVIO = 134,
	motorRecordTDIR = 135,
	motorRecordATHM = 136,
	motorRecordPP = 137,
	motorRecordMIP = 138,
	motorRecordMMAP = 139,
	motorRecordNMAP = 140,
	motorRecordDLY = 141,
	motorRecordCBAK = 142,
	motorRecordPCOF = 143,
	motorRecordICOF = 144,
	motorRecordDCOF = 145,
	motorRecordCNEN = 146,
	motorRecordINIT = 147,
	motorRecordPREM = 148,
	motorRecordPOST = 149,
	motorRecordSTOO = 150,
	motorRecordDINP = 151,
	motorRecordRINP = 152,
	motorRecordJVEL = 153,
	motorRecordJAR = 154,
	motorRecordLOCK = 155,
	motorRecordNTM = 156,
	motorRecordNTMF = 157,
	motorRecordHVEL = 158,
	motorRecordSTUP = 159,
	motorRecordRMOD = 160,
	motorRecordADEL = 161,
	motorRecordMDEL = 162,
	motorRecordALST = 163,
	motorRecordMLST = 164,
	motorRecordSYNC = 165,
	motorRecordIGSET = 166
} motorFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsAssert.h>
#include <epicsExport.h>
#ifdef __cplusplus
extern "C" {
#endif
static int motorRecordSizeOffset(dbRecordType *prt)
{
    motorRecord *prec = 0;

    assert(prt->no_fields == 167);
    prt->papFldDes[motorRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[motorRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[motorRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[motorRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[motorRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[motorRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[motorRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[motorRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[motorRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[motorRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[motorRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[motorRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[motorRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[motorRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[motorRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[motorRecordBKLNK]->size = sizeof(prec->bklnk);
    prt->papFldDes[motorRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[motorRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[motorRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[motorRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[motorRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[motorRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[motorRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[motorRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[motorRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[motorRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[motorRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[motorRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[motorRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[motorRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[motorRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[motorRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[motorRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[motorRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[motorRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[motorRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[motorRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[motorRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[motorRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[motorRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[motorRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[motorRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[motorRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[motorRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[motorRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[motorRecordVERS]->size = sizeof(prec->vers);
    prt->papFldDes[motorRecordOFF]->size = sizeof(prec->off);
    prt->papFldDes[motorRecordFOFF]->size = sizeof(prec->foff);
    prt->papFldDes[motorRecordFOF]->size = sizeof(prec->fof);
    prt->papFldDes[motorRecordVOF]->size = sizeof(prec->vof);
    prt->papFldDes[motorRecordDIR]->size = sizeof(prec->dir);
    prt->papFldDes[motorRecordSET]->size = sizeof(prec->set);
    prt->papFldDes[motorRecordSSET]->size = sizeof(prec->sset);
    prt->papFldDes[motorRecordSUSE]->size = sizeof(prec->suse);
    prt->papFldDes[motorRecordVELO]->size = sizeof(prec->velo);
    prt->papFldDes[motorRecordVBAS]->size = sizeof(prec->vbas);
    prt->papFldDes[motorRecordVMAX]->size = sizeof(prec->vmax);
    prt->papFldDes[motorRecordS]->size = sizeof(prec->s);
    prt->papFldDes[motorRecordSBAS]->size = sizeof(prec->sbas);
    prt->papFldDes[motorRecordSMAX]->size = sizeof(prec->smax);
    prt->papFldDes[motorRecordACCL]->size = sizeof(prec->accl);
    prt->papFldDes[motorRecordBDST]->size = sizeof(prec->bdst);
    prt->papFldDes[motorRecordBVEL]->size = sizeof(prec->bvel);
    prt->papFldDes[motorRecordSBAK]->size = sizeof(prec->sbak);
    prt->papFldDes[motorRecordBACC]->size = sizeof(prec->bacc);
    prt->papFldDes[motorRecordFRAC]->size = sizeof(prec->frac);
    prt->papFldDes[motorRecordOUT]->size = sizeof(prec->out);
    prt->papFldDes[motorRecordCARD]->size = sizeof(prec->card);
    prt->papFldDes[motorRecordRDBL]->size = sizeof(prec->rdbl);
    prt->papFldDes[motorRecordDOL]->size = sizeof(prec->dol);
    prt->papFldDes[motorRecordOMSL]->size = sizeof(prec->omsl);
    prt->papFldDes[motorRecordRLNK]->size = sizeof(prec->rlnk);
    prt->papFldDes[motorRecordSREV]->size = sizeof(prec->srev);
    prt->papFldDes[motorRecordUREV]->size = sizeof(prec->urev);
    prt->papFldDes[motorRecordMRES]->size = sizeof(prec->mres);
    prt->papFldDes[motorRecordERES]->size = sizeof(prec->eres);
    prt->papFldDes[motorRecordRRES]->size = sizeof(prec->rres);
    prt->papFldDes[motorRecordUEIP]->size = sizeof(prec->ueip);
    prt->papFldDes[motorRecordURIP]->size = sizeof(prec->urip);
    prt->papFldDes[motorRecordPREC]->size = sizeof(prec->prec);
    prt->papFldDes[motorRecordEGU]->size = sizeof(prec->egu);
    prt->papFldDes[motorRecordHLM]->size = sizeof(prec->hlm);
    prt->papFldDes[motorRecordLLM]->size = sizeof(prec->llm);
    prt->papFldDes[motorRecordDHLM]->size = sizeof(prec->dhlm);
    prt->papFldDes[motorRecordDLLM]->size = sizeof(prec->dllm);
    prt->papFldDes[motorRecordHOPR]->size = sizeof(prec->hopr);
    prt->papFldDes[motorRecordLOPR]->size = sizeof(prec->lopr);
    prt->papFldDes[motorRecordHLS]->size = sizeof(prec->hls);
    prt->papFldDes[motorRecordLLS]->size = sizeof(prec->lls);
    prt->papFldDes[motorRecordRHLS]->size = sizeof(prec->rhls);
    prt->papFldDes[motorRecordRLLS]->size = sizeof(prec->rlls);
    prt->papFldDes[motorRecordHIHI]->size = sizeof(prec->hihi);
    prt->papFldDes[motorRecordLOLO]->size = sizeof(prec->lolo);
    prt->papFldDes[motorRecordHIGH]->size = sizeof(prec->high);
    prt->papFldDes[motorRecordLOW]->size = sizeof(prec->low);
    prt->papFldDes[motorRecordHHSV]->size = sizeof(prec->hhsv);
    prt->papFldDes[motorRecordLLSV]->size = sizeof(prec->llsv);
    prt->papFldDes[motorRecordHSV]->size = sizeof(prec->hsv);
    prt->papFldDes[motorRecordLSV]->size = sizeof(prec->lsv);
    prt->papFldDes[motorRecordHLSV]->size = sizeof(prec->hlsv);
    prt->papFldDes[motorRecordRDBD]->size = sizeof(prec->rdbd);
    prt->papFldDes[motorRecordRCNT]->size = sizeof(prec->rcnt);
    prt->papFldDes[motorRecordRTRY]->size = sizeof(prec->rtry);
    prt->papFldDes[motorRecordMISS]->size = sizeof(prec->miss);
    prt->papFldDes[motorRecordSPMG]->size = sizeof(prec->spmg);
    prt->papFldDes[motorRecordLSPG]->size = sizeof(prec->lspg);
    prt->papFldDes[motorRecordSTOP]->size = sizeof(prec->stop);
    prt->papFldDes[motorRecordHOMF]->size = sizeof(prec->homf);
    prt->papFldDes[motorRecordHOMR]->size = sizeof(prec->homr);
    prt->papFldDes[motorRecordJOGF]->size = sizeof(prec->jogf);
    prt->papFldDes[motorRecordJOGR]->size = sizeof(prec->jogr);
    prt->papFldDes[motorRecordTWF]->size = sizeof(prec->twf);
    prt->papFldDes[motorRecordTWR]->size = sizeof(prec->twr);
    prt->papFldDes[motorRecordTWV]->size = sizeof(prec->twv);
    prt->papFldDes[motorRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[motorRecordLVAL]->size = sizeof(prec->lval);
    prt->papFldDes[motorRecordDVAL]->size = sizeof(prec->dval);
    prt->papFldDes[motorRecordLDVL]->size = sizeof(prec->ldvl);
    prt->papFldDes[motorRecordRVAL]->size = sizeof(prec->rval);
    prt->papFldDes[motorRecordLRVL]->size = sizeof(prec->lrvl);
    prt->papFldDes[motorRecordRLV]->size = sizeof(prec->rlv);
    prt->papFldDes[motorRecordLRLV]->size = sizeof(prec->lrlv);
    prt->papFldDes[motorRecordRBV]->size = sizeof(prec->rbv);
    prt->papFldDes[motorRecordDRBV]->size = sizeof(prec->drbv);
    prt->papFldDes[motorRecordDIFF]->size = sizeof(prec->diff);
    prt->papFldDes[motorRecordRDIF]->size = sizeof(prec->rdif);
    prt->papFldDes[motorRecordCDIR]->size = sizeof(prec->cdir);
    prt->papFldDes[motorRecordRRBV]->size = sizeof(prec->rrbv);
    prt->papFldDes[motorRecordRMP]->size = sizeof(prec->rmp);
    prt->papFldDes[motorRecordREP]->size = sizeof(prec->rep);
    prt->papFldDes[motorRecordRVEL]->size = sizeof(prec->rvel);
    prt->papFldDes[motorRecordDMOV]->size = sizeof(prec->dmov);
    prt->papFldDes[motorRecordMOVN]->size = sizeof(prec->movn);
    prt->papFldDes[motorRecordMSTA]->size = sizeof(prec->msta);
    prt->papFldDes[motorRecordLVIO]->size = sizeof(prec->lvio);
    prt->papFldDes[motorRecordTDIR]->size = sizeof(prec->tdir);
    prt->papFldDes[motorRecordATHM]->size = sizeof(prec->athm);
    prt->papFldDes[motorRecordPP]->size = sizeof(prec->pp);
    prt->papFldDes[motorRecordMIP]->size = sizeof(prec->mip);
    prt->papFldDes[motorRecordMMAP]->size = sizeof(prec->mmap);
    prt->papFldDes[motorRecordNMAP]->size = sizeof(prec->nmap);
    prt->papFldDes[motorRecordDLY]->size = sizeof(prec->dly);
    prt->papFldDes[motorRecordCBAK]->size = sizeof(prec->cbak);
    prt->papFldDes[motorRecordPCOF]->size = sizeof(prec->pcof);
    prt->papFldDes[motorRecordICOF]->size = sizeof(prec->icof);
    prt->papFldDes[motorRecordDCOF]->size = sizeof(prec->dcof);
    prt->papFldDes[motorRecordCNEN]->size = sizeof(prec->cnen);
    prt->papFldDes[motorRecordINIT]->size = sizeof(prec->init);
    prt->papFldDes[motorRecordPREM]->size = sizeof(prec->prem);
    prt->papFldDes[motorRecordPOST]->size = sizeof(prec->post);
    prt->papFldDes[motorRecordSTOO]->size = sizeof(prec->stoo);
    prt->papFldDes[motorRecordDINP]->size = sizeof(prec->dinp);
    prt->papFldDes[motorRecordRINP]->size = sizeof(prec->rinp);
    prt->papFldDes[motorRecordJVEL]->size = sizeof(prec->jvel);
    prt->papFldDes[motorRecordJAR]->size = sizeof(prec->jar);
    prt->papFldDes[motorRecordLOCK]->size = sizeof(prec->lock);
    prt->papFldDes[motorRecordNTM]->size = sizeof(prec->ntm);
    prt->papFldDes[motorRecordNTMF]->size = sizeof(prec->ntmf);
    prt->papFldDes[motorRecordHVEL]->size = sizeof(prec->hvel);
    prt->papFldDes[motorRecordSTUP]->size = sizeof(prec->stup);
    prt->papFldDes[motorRecordRMOD]->size = sizeof(prec->rmod);
    prt->papFldDes[motorRecordADEL]->size = sizeof(prec->adel);
    prt->papFldDes[motorRecordMDEL]->size = sizeof(prec->mdel);
    prt->papFldDes[motorRecordALST]->size = sizeof(prec->alst);
    prt->papFldDes[motorRecordMLST]->size = sizeof(prec->mlst);
    prt->papFldDes[motorRecordSYNC]->size = sizeof(prec->sync);
    prt->papFldDes[motorRecordIGSET]->size = sizeof(prec->igset);
    prt->papFldDes[motorRecordNAME]->offset = (unsigned short)((char *)&prec->name - (char *)prec);
    prt->papFldDes[motorRecordDESC]->offset = (unsigned short)((char *)&prec->desc - (char *)prec);
    prt->papFldDes[motorRecordASG]->offset = (unsigned short)((char *)&prec->asg - (char *)prec);
    prt->papFldDes[motorRecordSCAN]->offset = (unsigned short)((char *)&prec->scan - (char *)prec);
    prt->papFldDes[motorRecordPINI]->offset = (unsigned short)((char *)&prec->pini - (char *)prec);
    prt->papFldDes[motorRecordPHAS]->offset = (unsigned short)((char *)&prec->phas - (char *)prec);
    prt->papFldDes[motorRecordEVNT]->offset = (unsigned short)((char *)&prec->evnt - (char *)prec);
    prt->papFldDes[motorRecordTSE]->offset = (unsigned short)((char *)&prec->tse - (char *)prec);
    prt->papFldDes[motorRecordTSEL]->offset = (unsigned short)((char *)&prec->tsel - (char *)prec);
    prt->papFldDes[motorRecordDTYP]->offset = (unsigned short)((char *)&prec->dtyp - (char *)prec);
    prt->papFldDes[motorRecordDISV]->offset = (unsigned short)((char *)&prec->disv - (char *)prec);
    prt->papFldDes[motorRecordDISA]->offset = (unsigned short)((char *)&prec->disa - (char *)prec);
    prt->papFldDes[motorRecordSDIS]->offset = (unsigned short)((char *)&prec->sdis - (char *)prec);
    prt->papFldDes[motorRecordMLOK]->offset = (unsigned short)((char *)&prec->mlok - (char *)prec);
    prt->papFldDes[motorRecordMLIS]->offset = (unsigned short)((char *)&prec->mlis - (char *)prec);
    prt->papFldDes[motorRecordBKLNK]->offset = (unsigned short)((char *)&prec->bklnk - (char *)prec);
    prt->papFldDes[motorRecordDISP]->offset = (unsigned short)((char *)&prec->disp - (char *)prec);
    prt->papFldDes[motorRecordPROC]->offset = (unsigned short)((char *)&prec->proc - (char *)prec);
    prt->papFldDes[motorRecordSTAT]->offset = (unsigned short)((char *)&prec->stat - (char *)prec);
    prt->papFldDes[motorRecordSEVR]->offset = (unsigned short)((char *)&prec->sevr - (char *)prec);
    prt->papFldDes[motorRecordNSTA]->offset = (unsigned short)((char *)&prec->nsta - (char *)prec);
    prt->papFldDes[motorRecordNSEV]->offset = (unsigned short)((char *)&prec->nsev - (char *)prec);
    prt->papFldDes[motorRecordACKS]->offset = (unsigned short)((char *)&prec->acks - (char *)prec);
    prt->papFldDes[motorRecordACKT]->offset = (unsigned short)((char *)&prec->ackt - (char *)prec);
    prt->papFldDes[motorRecordDISS]->offset = (unsigned short)((char *)&prec->diss - (char *)prec);
    prt->papFldDes[motorRecordLCNT]->offset = (unsigned short)((char *)&prec->lcnt - (char *)prec);
    prt->papFldDes[motorRecordPACT]->offset = (unsigned short)((char *)&prec->pact - (char *)prec);
    prt->papFldDes[motorRecordPUTF]->offset = (unsigned short)((char *)&prec->putf - (char *)prec);
    prt->papFldDes[motorRecordRPRO]->offset = (unsigned short)((char *)&prec->rpro - (char *)prec);
    prt->papFldDes[motorRecordASP]->offset = (unsigned short)((char *)&prec->asp - (char *)prec);
    prt->papFldDes[motorRecordPPN]->offset = (unsigned short)((char *)&prec->ppn - (char *)prec);
    prt->papFldDes[motorRecordPPNR]->offset = (unsigned short)((char *)&prec->ppnr - (char *)prec);
    prt->papFldDes[motorRecordSPVT]->offset = (unsigned short)((char *)&prec->spvt - (char *)prec);
    prt->papFldDes[motorRecordRSET]->offset = (unsigned short)((char *)&prec->rset - (char *)prec);
    prt->papFldDes[motorRecordDSET]->offset = (unsigned short)((char *)&prec->dset - (char *)prec);
    prt->papFldDes[motorRecordDPVT]->offset = (unsigned short)((char *)&prec->dpvt - (char *)prec);
    prt->papFldDes[motorRecordRDES]->offset = (unsigned short)((char *)&prec->rdes - (char *)prec);
    prt->papFldDes[motorRecordLSET]->offset = (unsigned short)((char *)&prec->lset - (char *)prec);
    prt->papFldDes[motorRecordPRIO]->offset = (unsigned short)((char *)&prec->prio - (char *)prec);
    prt->papFldDes[motorRecordTPRO]->offset = (unsigned short)((char *)&prec->tpro - (char *)prec);
    prt->papFldDes[motorRecordBKPT]->offset = (unsigned short)((char *)&prec->bkpt - (char *)prec);
    prt->papFldDes[motorRecordUDF]->offset = (unsigned short)((char *)&prec->udf - (char *)prec);
    prt->papFldDes[motorRecordUDFS]->offset = (unsigned short)((char *)&prec->udfs - (char *)prec);
    prt->papFldDes[motorRecordTIME]->offset = (unsigned short)((char *)&prec->time - (char *)prec);
    prt->papFldDes[motorRecordFLNK]->offset = (unsigned short)((char *)&prec->flnk - (char *)prec);
    prt->papFldDes[motorRecordVERS]->offset = (unsigned short)((char *)&prec->vers - (char *)prec);
    prt->papFldDes[motorRecordOFF]->offset = (unsigned short)((char *)&prec->off - (char *)prec);
    prt->papFldDes[motorRecordFOFF]->offset = (unsigned short)((char *)&prec->foff - (char *)prec);
    prt->papFldDes[motorRecordFOF]->offset = (unsigned short)((char *)&prec->fof - (char *)prec);
    prt->papFldDes[motorRecordVOF]->offset = (unsigned short)((char *)&prec->vof - (char *)prec);
    prt->papFldDes[motorRecordDIR]->offset = (unsigned short)((char *)&prec->dir - (char *)prec);
    prt->papFldDes[motorRecordSET]->offset = (unsigned short)((char *)&prec->set - (char *)prec);
    prt->papFldDes[motorRecordSSET]->offset = (unsigned short)((char *)&prec->sset - (char *)prec);
    prt->papFldDes[motorRecordSUSE]->offset = (unsigned short)((char *)&prec->suse - (char *)prec);
    prt->papFldDes[motorRecordVELO]->offset = (unsigned short)((char *)&prec->velo - (char *)prec);
    prt->papFldDes[motorRecordVBAS]->offset = (unsigned short)((char *)&prec->vbas - (char *)prec);
    prt->papFldDes[motorRecordVMAX]->offset = (unsigned short)((char *)&prec->vmax - (char *)prec);
    prt->papFldDes[motorRecordS]->offset = (unsigned short)((char *)&prec->s - (char *)prec);
    prt->papFldDes[motorRecordSBAS]->offset = (unsigned short)((char *)&prec->sbas - (char *)prec);
    prt->papFldDes[motorRecordSMAX]->offset = (unsigned short)((char *)&prec->smax - (char *)prec);
    prt->papFldDes[motorRecordACCL]->offset = (unsigned short)((char *)&prec->accl - (char *)prec);
    prt->papFldDes[motorRecordBDST]->offset = (unsigned short)((char *)&prec->bdst - (char *)prec);
    prt->papFldDes[motorRecordBVEL]->offset = (unsigned short)((char *)&prec->bvel - (char *)prec);
    prt->papFldDes[motorRecordSBAK]->offset = (unsigned short)((char *)&prec->sbak - (char *)prec);
    prt->papFldDes[motorRecordBACC]->offset = (unsigned short)((char *)&prec->bacc - (char *)prec);
    prt->papFldDes[motorRecordFRAC]->offset = (unsigned short)((char *)&prec->frac - (char *)prec);
    prt->papFldDes[motorRecordOUT]->offset = (unsigned short)((char *)&prec->out - (char *)prec);
    prt->papFldDes[motorRecordCARD]->offset = (unsigned short)((char *)&prec->card - (char *)prec);
    prt->papFldDes[motorRecordRDBL]->offset = (unsigned short)((char *)&prec->rdbl - (char *)prec);
    prt->papFldDes[motorRecordDOL]->offset = (unsigned short)((char *)&prec->dol - (char *)prec);
    prt->papFldDes[motorRecordOMSL]->offset = (unsigned short)((char *)&prec->omsl - (char *)prec);
    prt->papFldDes[motorRecordRLNK]->offset = (unsigned short)((char *)&prec->rlnk - (char *)prec);
    prt->papFldDes[motorRecordSREV]->offset = (unsigned short)((char *)&prec->srev - (char *)prec);
    prt->papFldDes[motorRecordUREV]->offset = (unsigned short)((char *)&prec->urev - (char *)prec);
    prt->papFldDes[motorRecordMRES]->offset = (unsigned short)((char *)&prec->mres - (char *)prec);
    prt->papFldDes[motorRecordERES]->offset = (unsigned short)((char *)&prec->eres - (char *)prec);
    prt->papFldDes[motorRecordRRES]->offset = (unsigned short)((char *)&prec->rres - (char *)prec);
    prt->papFldDes[motorRecordUEIP]->offset = (unsigned short)((char *)&prec->ueip - (char *)prec);
    prt->papFldDes[motorRecordURIP]->offset = (unsigned short)((char *)&prec->urip - (char *)prec);
    prt->papFldDes[motorRecordPREC]->offset = (unsigned short)((char *)&prec->prec - (char *)prec);
    prt->papFldDes[motorRecordEGU]->offset = (unsigned short)((char *)&prec->egu - (char *)prec);
    prt->papFldDes[motorRecordHLM]->offset = (unsigned short)((char *)&prec->hlm - (char *)prec);
    prt->papFldDes[motorRecordLLM]->offset = (unsigned short)((char *)&prec->llm - (char *)prec);
    prt->papFldDes[motorRecordDHLM]->offset = (unsigned short)((char *)&prec->dhlm - (char *)prec);
    prt->papFldDes[motorRecordDLLM]->offset = (unsigned short)((char *)&prec->dllm - (char *)prec);
    prt->papFldDes[motorRecordHOPR]->offset = (unsigned short)((char *)&prec->hopr - (char *)prec);
    prt->papFldDes[motorRecordLOPR]->offset = (unsigned short)((char *)&prec->lopr - (char *)prec);
    prt->papFldDes[motorRecordHLS]->offset = (unsigned short)((char *)&prec->hls - (char *)prec);
    prt->papFldDes[motorRecordLLS]->offset = (unsigned short)((char *)&prec->lls - (char *)prec);
    prt->papFldDes[motorRecordRHLS]->offset = (unsigned short)((char *)&prec->rhls - (char *)prec);
    prt->papFldDes[motorRecordRLLS]->offset = (unsigned short)((char *)&prec->rlls - (char *)prec);
    prt->papFldDes[motorRecordHIHI]->offset = (unsigned short)((char *)&prec->hihi - (char *)prec);
    prt->papFldDes[motorRecordLOLO]->offset = (unsigned short)((char *)&prec->lolo - (char *)prec);
    prt->papFldDes[motorRecordHIGH]->offset = (unsigned short)((char *)&prec->high - (char *)prec);
    prt->papFldDes[motorRecordLOW]->offset = (unsigned short)((char *)&prec->low - (char *)prec);
    prt->papFldDes[motorRecordHHSV]->offset = (unsigned short)((char *)&prec->hhsv - (char *)prec);
    prt->papFldDes[motorRecordLLSV]->offset = (unsigned short)((char *)&prec->llsv - (char *)prec);
    prt->papFldDes[motorRecordHSV]->offset = (unsigned short)((char *)&prec->hsv - (char *)prec);
    prt->papFldDes[motorRecordLSV]->offset = (unsigned short)((char *)&prec->lsv - (char *)prec);
    prt->papFldDes[motorRecordHLSV]->offset = (unsigned short)((char *)&prec->hlsv - (char *)prec);
    prt->papFldDes[motorRecordRDBD]->offset = (unsigned short)((char *)&prec->rdbd - (char *)prec);
    prt->papFldDes[motorRecordRCNT]->offset = (unsigned short)((char *)&prec->rcnt - (char *)prec);
    prt->papFldDes[motorRecordRTRY]->offset = (unsigned short)((char *)&prec->rtry - (char *)prec);
    prt->papFldDes[motorRecordMISS]->offset = (unsigned short)((char *)&prec->miss - (char *)prec);
    prt->papFldDes[motorRecordSPMG]->offset = (unsigned short)((char *)&prec->spmg - (char *)prec);
    prt->papFldDes[motorRecordLSPG]->offset = (unsigned short)((char *)&prec->lspg - (char *)prec);
    prt->papFldDes[motorRecordSTOP]->offset = (unsigned short)((char *)&prec->stop - (char *)prec);
    prt->papFldDes[motorRecordHOMF]->offset = (unsigned short)((char *)&prec->homf - (char *)prec);
    prt->papFldDes[motorRecordHOMR]->offset = (unsigned short)((char *)&prec->homr - (char *)prec);
    prt->papFldDes[motorRecordJOGF]->offset = (unsigned short)((char *)&prec->jogf - (char *)prec);
    prt->papFldDes[motorRecordJOGR]->offset = (unsigned short)((char *)&prec->jogr - (char *)prec);
    prt->papFldDes[motorRecordTWF]->offset = (unsigned short)((char *)&prec->twf - (char *)prec);
    prt->papFldDes[motorRecordTWR]->offset = (unsigned short)((char *)&prec->twr - (char *)prec);
    prt->papFldDes[motorRecordTWV]->offset = (unsigned short)((char *)&prec->twv - (char *)prec);
    prt->papFldDes[motorRecordVAL]->offset = (unsigned short)((char *)&prec->val - (char *)prec);
    prt->papFldDes[motorRecordLVAL]->offset = (unsigned short)((char *)&prec->lval - (char *)prec);
    prt->papFldDes[motorRecordDVAL]->offset = (unsigned short)((char *)&prec->dval - (char *)prec);
    prt->papFldDes[motorRecordLDVL]->offset = (unsigned short)((char *)&prec->ldvl - (char *)prec);
    prt->papFldDes[motorRecordRVAL]->offset = (unsigned short)((char *)&prec->rval - (char *)prec);
    prt->papFldDes[motorRecordLRVL]->offset = (unsigned short)((char *)&prec->lrvl - (char *)prec);
    prt->papFldDes[motorRecordRLV]->offset = (unsigned short)((char *)&prec->rlv - (char *)prec);
    prt->papFldDes[motorRecordLRLV]->offset = (unsigned short)((char *)&prec->lrlv - (char *)prec);
    prt->papFldDes[motorRecordRBV]->offset = (unsigned short)((char *)&prec->rbv - (char *)prec);
    prt->papFldDes[motorRecordDRBV]->offset = (unsigned short)((char *)&prec->drbv - (char *)prec);
    prt->papFldDes[motorRecordDIFF]->offset = (unsigned short)((char *)&prec->diff - (char *)prec);
    prt->papFldDes[motorRecordRDIF]->offset = (unsigned short)((char *)&prec->rdif - (char *)prec);
    prt->papFldDes[motorRecordCDIR]->offset = (unsigned short)((char *)&prec->cdir - (char *)prec);
    prt->papFldDes[motorRecordRRBV]->offset = (unsigned short)((char *)&prec->rrbv - (char *)prec);
    prt->papFldDes[motorRecordRMP]->offset = (unsigned short)((char *)&prec->rmp - (char *)prec);
    prt->papFldDes[motorRecordREP]->offset = (unsigned short)((char *)&prec->rep - (char *)prec);
    prt->papFldDes[motorRecordRVEL]->offset = (unsigned short)((char *)&prec->rvel - (char *)prec);
    prt->papFldDes[motorRecordDMOV]->offset = (unsigned short)((char *)&prec->dmov - (char *)prec);
    prt->papFldDes[motorRecordMOVN]->offset = (unsigned short)((char *)&prec->movn - (char *)prec);
    prt->papFldDes[motorRecordMSTA]->offset = (unsigned short)((char *)&prec->msta - (char *)prec);
    prt->papFldDes[motorRecordLVIO]->offset = (unsigned short)((char *)&prec->lvio - (char *)prec);
    prt->papFldDes[motorRecordTDIR]->offset = (unsigned short)((char *)&prec->tdir - (char *)prec);
    prt->papFldDes[motorRecordATHM]->offset = (unsigned short)((char *)&prec->athm - (char *)prec);
    prt->papFldDes[motorRecordPP]->offset = (unsigned short)((char *)&prec->pp - (char *)prec);
    prt->papFldDes[motorRecordMIP]->offset = (unsigned short)((char *)&prec->mip - (char *)prec);
    prt->papFldDes[motorRecordMMAP]->offset = (unsigned short)((char *)&prec->mmap - (char *)prec);
    prt->papFldDes[motorRecordNMAP]->offset = (unsigned short)((char *)&prec->nmap - (char *)prec);
    prt->papFldDes[motorRecordDLY]->offset = (unsigned short)((char *)&prec->dly - (char *)prec);
    prt->papFldDes[motorRecordCBAK]->offset = (unsigned short)((char *)&prec->cbak - (char *)prec);
    prt->papFldDes[motorRecordPCOF]->offset = (unsigned short)((char *)&prec->pcof - (char *)prec);
    prt->papFldDes[motorRecordICOF]->offset = (unsigned short)((char *)&prec->icof - (char *)prec);
    prt->papFldDes[motorRecordDCOF]->offset = (unsigned short)((char *)&prec->dcof - (char *)prec);
    prt->papFldDes[motorRecordCNEN]->offset = (unsigned short)((char *)&prec->cnen - (char *)prec);
    prt->papFldDes[motorRecordINIT]->offset = (unsigned short)((char *)&prec->init - (char *)prec);
    prt->papFldDes[motorRecordPREM]->offset = (unsigned short)((char *)&prec->prem - (char *)prec);
    prt->papFldDes[motorRecordPOST]->offset = (unsigned short)((char *)&prec->post - (char *)prec);
    prt->papFldDes[motorRecordSTOO]->offset = (unsigned short)((char *)&prec->stoo - (char *)prec);
    prt->papFldDes[motorRecordDINP]->offset = (unsigned short)((char *)&prec->dinp - (char *)prec);
    prt->papFldDes[motorRecordRINP]->offset = (unsigned short)((char *)&prec->rinp - (char *)prec);
    prt->papFldDes[motorRecordJVEL]->offset = (unsigned short)((char *)&prec->jvel - (char *)prec);
    prt->papFldDes[motorRecordJAR]->offset = (unsigned short)((char *)&prec->jar - (char *)prec);
    prt->papFldDes[motorRecordLOCK]->offset = (unsigned short)((char *)&prec->lock - (char *)prec);
    prt->papFldDes[motorRecordNTM]->offset = (unsigned short)((char *)&prec->ntm - (char *)prec);
    prt->papFldDes[motorRecordNTMF]->offset = (unsigned short)((char *)&prec->ntmf - (char *)prec);
    prt->papFldDes[motorRecordHVEL]->offset = (unsigned short)((char *)&prec->hvel - (char *)prec);
    prt->papFldDes[motorRecordSTUP]->offset = (unsigned short)((char *)&prec->stup - (char *)prec);
    prt->papFldDes[motorRecordRMOD]->offset = (unsigned short)((char *)&prec->rmod - (char *)prec);
    prt->papFldDes[motorRecordADEL]->offset = (unsigned short)((char *)&prec->adel - (char *)prec);
    prt->papFldDes[motorRecordMDEL]->offset = (unsigned short)((char *)&prec->mdel - (char *)prec);
    prt->papFldDes[motorRecordALST]->offset = (unsigned short)((char *)&prec->alst - (char *)prec);
    prt->papFldDes[motorRecordMLST]->offset = (unsigned short)((char *)&prec->mlst - (char *)prec);
    prt->papFldDes[motorRecordSYNC]->offset = (unsigned short)((char *)&prec->sync - (char *)prec);
    prt->papFldDes[motorRecordIGSET]->offset = (unsigned short)((char *)&prec->igset - (char *)prec);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(motorRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_motorRecord_H */
