/* transformRecord.h generated from transformRecord.dbd */

#ifndef INC_transformRecord_H
#define INC_transformRecord_H

 #include "epicsTypes.h"
 #include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "epicsTime.h"

#ifndef transformIVLA_NUM_CHOICES
typedef enum {
    transformIVLA_IGNORE            /* Ignore error */,
    transformIVLA_DO_NOTHING        /* Do Nothing */
} transformIVLA;
#define transformIVLA_NUM_CHOICES 2
#endif

#ifndef transformCOPT_NUM_CHOICES
typedef enum {
    transformCOPT_CONDITIONAL       /* Conditional */,
    transformCOPT_ALWAYS            /* Always */
} transformCOPT;
#define transformCOPT_NUM_CHOICES 2
#endif

#ifndef transformIAV_NUM_CHOICES
typedef enum {
    transformIAV_EXT_NC             /* Ext PV NC */,
    transformIAV_EXT                /* Ext PV OK */,
    transformIAV_LOC                /* Local PV */,
    transformIAV_CON                /* Constant */
} transformIAV;
#define transformIAV_NUM_CHOICES 4
#endif

typedef struct transformRecord {
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
    void *         rpvt;            /* Record Private */
    epicsEnum16         ivla;       /* Invalid link action */
    epicsEnum16         copt;       /* Calc option */
    epicsFloat64        val;        /* Result */
    char                clca[120];  /* Calculation A */
    char                clcb[120];  /* Calculation B */
    char                clcc[120];  /* Calculation C */
    char                clcd[120];  /* Calculation D */
    char                clce[120];  /* Calculation E */
    char                clcf[120];  /* Calculation F */
    char                clcg[120];  /* Calculation G */
    char                clch[120];  /* Calculation H */
    char                clci[120];  /* Calculation I */
    char                clcj[120];  /* Calculation J */
    char                clck[120];  /* Calculation K */
    char                clcl[120];  /* Calculation L */
    char                clcm[120];  /* Calculation M */
    char                clcn[120];  /* Calculation N */
    char                clco[120];  /* Calculation O */
    char                clcp[120];  /* Calculation P */
    epicsInt32          cav;        /* CALC Invalid */
    epicsInt32          cbv;        /* CALC Invalid */
    epicsInt32          ccv;        /* CALC Invalid */
    epicsInt32          cdv;        /* CALC Invalid */
    epicsInt32          cev;        /* CALC Invalid */
    epicsInt32          cfv;        /* CALC Invalid */
    epicsInt32          cgv;        /* CALC Invalid */
    epicsInt32          chv;        /* CALC Invalid */
    epicsInt32          civ;        /* CALC Invalid */
    epicsInt32          cjv;        /* CALC Invalid */
    epicsInt32          ckv;        /* CALC Invalid */
    epicsInt32          clv;        /* CALC Invalid */
    epicsInt32          cmv;        /* CALC Invalid */
    epicsInt32          cnv;        /* CALC Invalid */
    epicsInt32          cov;        /* CALC Invalid */
    epicsInt32          cpv;        /* CALC Invalid */
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
    DBLINK              inpm;       /* Input M */
    DBLINK              inpn;       /* Input N */
    DBLINK              inpo;       /* Input O */
    DBLINK              inpp;       /* Input P */
    DBLINK              outa;       /* Output A */
    DBLINK              outb;       /* Output B */
    DBLINK              outc;       /* Output C */
    DBLINK              outd;       /* Output D */
    DBLINK              oute;       /* Output E */
    DBLINK              outf;       /* Output F */
    DBLINK              outg;       /* Output G */
    DBLINK              outh;       /* Output H */
    DBLINK              outi;       /* Output I */
    DBLINK              outj;       /* Output J */
    DBLINK              outk;       /* Output K */
    DBLINK              outl;       /* Output L */
    DBLINK              outm;       /* Output M */
    DBLINK              outn;       /* Output N */
    DBLINK              outo;       /* Output O */
    DBLINK              outp;       /* Output P */
    char                egu[16];    /* Units Name */
    epicsInt16          prec;       /* Display Precision */
    epicsFloat64        a;          /* Value of A */
    epicsFloat64        b;          /* Value of B */
    epicsFloat64        c;          /* Value of C */
    epicsFloat64        d;          /* Value of D */
    epicsFloat64        e;          /* Value of E */
    epicsFloat64        f;          /* Value of F */
    epicsFloat64        g;          /* Value of G */
    epicsFloat64        h;          /* Value of H */
    epicsFloat64        i;          /* Value of I */
    epicsFloat64        j;          /* Value of J */
    epicsFloat64        k;          /* Value of K */
    epicsFloat64        l;          /* Value of L */
    epicsFloat64        m;          /* Value of M */
    epicsFloat64        n;          /* Value of N */
    epicsFloat64        o;          /* Value of O */
    epicsFloat64        p;          /* Value of P */
    epicsFloat64        la;         /* Prev Value of A */
    epicsFloat64        lb;         /* Prev Value of B */
    epicsFloat64        lc;         /* Prev Value of C */
    epicsFloat64        ld;         /* Prev Value of D */
    epicsFloat64        le;         /* Prev Value of E */
    epicsFloat64        lf;         /* Prev Value of F */
    epicsFloat64        lg;         /* Prev Value of G */
    epicsFloat64        lh;         /* Prev Value of H */
    epicsFloat64        li;         /* Prev Value of I */
    epicsFloat64        lj;         /* Prev Value of J */
    epicsFloat64        lk;         /* Prev Value of K */
    epicsFloat64        ll;         /* Prev Value of L */
    epicsFloat64        lm;         /* Prev Value of M */
    epicsFloat64        ln;         /* Prev Value of N */
    epicsFloat64        lo;         /* Prev Value of O */
    epicsFloat64        lp;         /* Prev Value of P */
    unsigned char rpca[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc A */
    unsigned char rpcb[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc B */
    unsigned char rpcc[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc C */
    unsigned char rpcd[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc D */
    unsigned char rpce[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc E */
    unsigned char rpcf[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc F */
    unsigned char rpcg[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc G */
    unsigned char rpch[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc H */
    unsigned char rpci[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc I */
    unsigned char rpcj[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc J */
    unsigned char rpck[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc K */
    unsigned char rpcl[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc L */
    unsigned char rpcm[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc M */
    unsigned char rpcn[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc N */
    unsigned char rpco[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc O */
    unsigned char rpcp[SCALC_INFIX_TO_POSTFIX_SIZE(120)]; /* Postfix Calc P */
    char                cmta[39];   /* Comment A */
    char                cmtb[39];   /* Comment B */
    char                cmtc[39];   /* Comment C */
    char                cmtd[39];   /* Comment D */
    char                cmte[39];   /* Comment E */
    char                cmtf[39];   /* Comment F */
    char                cmtg[39];   /* Comment G */
    char                cmth[39];   /* Comment H */
    char                cmti[39];   /* Comment I */
    char                cmtj[39];   /* Comment J */
    char                cmtk[39];   /* Comment K */
    char                cmtl[39];   /* Comment L */
    char                cmtm[39];   /* Comment M */
    char                cmtn[39];   /* Comment N */
    char                cmto[39];   /* Comment O */
    char                cmtp[39];   /* Comment P */
    epicsInt16          map;        /* Input bitmap */
    epicsEnum16         iav;        /* Input link valid */
    epicsEnum16         ibv;        /* Input link valid */
    epicsEnum16         icv;        /* Input link valid */
    epicsEnum16         idv;        /* Input link valid */
    epicsEnum16         iev;        /* Input link valid */
    epicsEnum16         ifv;        /* Input link valid */
    epicsEnum16         igv;        /* Input link valid */
    epicsEnum16         ihv;        /* Input link valid */
    epicsEnum16         iiv;        /* Input link valid */
    epicsEnum16         ijv;        /* Input link valid */
    epicsEnum16         ikv;        /* Input link valid */
    epicsEnum16         ilv;        /* Input link valid */
    epicsEnum16         imv;        /* Input link valid */
    epicsEnum16         inv;        /* Input link valid */
    epicsEnum16         iov;        /* Input link valid */
    epicsEnum16         ipv;        /* Input link valid */
    epicsEnum16         oav;        /* Output link valid */
    epicsEnum16         obv;        /* Output link valid */
    epicsEnum16         ocv;        /* Output link valid */
    epicsEnum16         odv;        /* Output link valid */
    epicsEnum16         oev;        /* Output link valid */
    epicsEnum16         ofv;        /* Output link valid */
    epicsEnum16         ogv;        /* Output link valid */
    epicsEnum16         ohv;        /* Output link valid */
    epicsEnum16         oiv;        /* Output link valid */
    epicsEnum16         ojv;        /* Output link valid */
    epicsEnum16         okv;        /* Output link valid */
    epicsEnum16         olv;        /* Output link valid */
    epicsEnum16         omv;        /* Output link valid */
    epicsEnum16         onv;        /* Output link valid */
    epicsEnum16         oov;        /* Output link valid */
    epicsEnum16         opv;        /* Output link valid */
} transformRecord;

typedef enum {
	transformRecordNAME = 0,
	transformRecordDESC = 1,
	transformRecordASG = 2,
	transformRecordSCAN = 3,
	transformRecordPINI = 4,
	transformRecordPHAS = 5,
	transformRecordEVNT = 6,
	transformRecordTSE = 7,
	transformRecordTSEL = 8,
	transformRecordDTYP = 9,
	transformRecordDISV = 10,
	transformRecordDISA = 11,
	transformRecordSDIS = 12,
	transformRecordMLOK = 13,
	transformRecordMLIS = 14,
	transformRecordDISP = 15,
	transformRecordPROC = 16,
	transformRecordSTAT = 17,
	transformRecordSEVR = 18,
	transformRecordNSTA = 19,
	transformRecordNSEV = 20,
	transformRecordACKS = 21,
	transformRecordACKT = 22,
	transformRecordDISS = 23,
	transformRecordLCNT = 24,
	transformRecordPACT = 25,
	transformRecordPUTF = 26,
	transformRecordRPRO = 27,
	transformRecordASP = 28,
	transformRecordPPN = 29,
	transformRecordPPNR = 30,
	transformRecordSPVT = 31,
	transformRecordRSET = 32,
	transformRecordDSET = 33,
	transformRecordDPVT = 34,
	transformRecordRDES = 35,
	transformRecordLSET = 36,
	transformRecordPRIO = 37,
	transformRecordTPRO = 38,
	transformRecordBKPT = 39,
	transformRecordUDF = 40,
	transformRecordUDFS = 41,
	transformRecordTIME = 42,
	transformRecordFLNK = 43,
	transformRecordVERS = 44,
	transformRecordRPVT = 45,
	transformRecordIVLA = 46,
	transformRecordCOPT = 47,
	transformRecordVAL = 48,
	transformRecordCLCA = 49,
	transformRecordCLCB = 50,
	transformRecordCLCC = 51,
	transformRecordCLCD = 52,
	transformRecordCLCE = 53,
	transformRecordCLCF = 54,
	transformRecordCLCG = 55,
	transformRecordCLCH = 56,
	transformRecordCLCI = 57,
	transformRecordCLCJ = 58,
	transformRecordCLCK = 59,
	transformRecordCLCL = 60,
	transformRecordCLCM = 61,
	transformRecordCLCN = 62,
	transformRecordCLCO = 63,
	transformRecordCLCP = 64,
	transformRecordCAV = 65,
	transformRecordCBV = 66,
	transformRecordCCV = 67,
	transformRecordCDV = 68,
	transformRecordCEV = 69,
	transformRecordCFV = 70,
	transformRecordCGV = 71,
	transformRecordCHV = 72,
	transformRecordCIV = 73,
	transformRecordCJV = 74,
	transformRecordCKV = 75,
	transformRecordCLV = 76,
	transformRecordCMV = 77,
	transformRecordCNV = 78,
	transformRecordCOV = 79,
	transformRecordCPV = 80,
	transformRecordINPA = 81,
	transformRecordINPB = 82,
	transformRecordINPC = 83,
	transformRecordINPD = 84,
	transformRecordINPE = 85,
	transformRecordINPF = 86,
	transformRecordINPG = 87,
	transformRecordINPH = 88,
	transformRecordINPI = 89,
	transformRecordINPJ = 90,
	transformRecordINPK = 91,
	transformRecordINPL = 92,
	transformRecordINPM = 93,
	transformRecordINPN = 94,
	transformRecordINPO = 95,
	transformRecordINPP = 96,
	transformRecordOUTA = 97,
	transformRecordOUTB = 98,
	transformRecordOUTC = 99,
	transformRecordOUTD = 100,
	transformRecordOUTE = 101,
	transformRecordOUTF = 102,
	transformRecordOUTG = 103,
	transformRecordOUTH = 104,
	transformRecordOUTI = 105,
	transformRecordOUTJ = 106,
	transformRecordOUTK = 107,
	transformRecordOUTL = 108,
	transformRecordOUTM = 109,
	transformRecordOUTN = 110,
	transformRecordOUTO = 111,
	transformRecordOUTP = 112,
	transformRecordEGU = 113,
	transformRecordPREC = 114,
	transformRecordA = 115,
	transformRecordB = 116,
	transformRecordC = 117,
	transformRecordD = 118,
	transformRecordE = 119,
	transformRecordF = 120,
	transformRecordG = 121,
	transformRecordH = 122,
	transformRecordI = 123,
	transformRecordJ = 124,
	transformRecordK = 125,
	transformRecordL = 126,
	transformRecordM = 127,
	transformRecordN = 128,
	transformRecordO = 129,
	transformRecordP = 130,
	transformRecordLA = 131,
	transformRecordLB = 132,
	transformRecordLC = 133,
	transformRecordLD = 134,
	transformRecordLE = 135,
	transformRecordLF = 136,
	transformRecordLG = 137,
	transformRecordLH = 138,
	transformRecordLI = 139,
	transformRecordLJ = 140,
	transformRecordLK = 141,
	transformRecordLL = 142,
	transformRecordLM = 143,
	transformRecordLN = 144,
	transformRecordLO = 145,
	transformRecordLP = 146,
	transformRecordRPCA = 147,
	transformRecordRPCB = 148,
	transformRecordRPCC = 149,
	transformRecordRPCD = 150,
	transformRecordRPCE = 151,
	transformRecordRPCF = 152,
	transformRecordRPCG = 153,
	transformRecordRPCH = 154,
	transformRecordRPCI = 155,
	transformRecordRPCJ = 156,
	transformRecordRPCK = 157,
	transformRecordRPCL = 158,
	transformRecordRPCM = 159,
	transformRecordRPCN = 160,
	transformRecordRPCO = 161,
	transformRecordRPCP = 162,
	transformRecordCMTA = 163,
	transformRecordCMTB = 164,
	transformRecordCMTC = 165,
	transformRecordCMTD = 166,
	transformRecordCMTE = 167,
	transformRecordCMTF = 168,
	transformRecordCMTG = 169,
	transformRecordCMTH = 170,
	transformRecordCMTI = 171,
	transformRecordCMTJ = 172,
	transformRecordCMTK = 173,
	transformRecordCMTL = 174,
	transformRecordCMTM = 175,
	transformRecordCMTN = 176,
	transformRecordCMTO = 177,
	transformRecordCMTP = 178,
	transformRecordMAP = 179,
	transformRecordIAV = 180,
	transformRecordIBV = 181,
	transformRecordICV = 182,
	transformRecordIDV = 183,
	transformRecordIEV = 184,
	transformRecordIFV = 185,
	transformRecordIGV = 186,
	transformRecordIHV = 187,
	transformRecordIIV = 188,
	transformRecordIJV = 189,
	transformRecordIKV = 190,
	transformRecordILV = 191,
	transformRecordIMV = 192,
	transformRecordINV = 193,
	transformRecordIOV = 194,
	transformRecordIPV = 195,
	transformRecordOAV = 196,
	transformRecordOBV = 197,
	transformRecordOCV = 198,
	transformRecordODV = 199,
	transformRecordOEV = 200,
	transformRecordOFV = 201,
	transformRecordOGV = 202,
	transformRecordOHV = 203,
	transformRecordOIV = 204,
	transformRecordOJV = 205,
	transformRecordOKV = 206,
	transformRecordOLV = 207,
	transformRecordOMV = 208,
	transformRecordONV = 209,
	transformRecordOOV = 210,
	transformRecordOPV = 211
} transformFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsAssert.h>
#include <epicsExport.h>
#ifdef __cplusplus
extern "C" {
#endif
static int transformRecordSizeOffset(dbRecordType *prt)
{
    transformRecord *prec = 0;

    assert(prt->no_fields == 212);
    prt->papFldDes[transformRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[transformRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[transformRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[transformRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[transformRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[transformRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[transformRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[transformRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[transformRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[transformRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[transformRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[transformRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[transformRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[transformRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[transformRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[transformRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[transformRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[transformRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[transformRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[transformRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[transformRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[transformRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[transformRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[transformRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[transformRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[transformRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[transformRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[transformRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[transformRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[transformRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[transformRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[transformRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[transformRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[transformRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[transformRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[transformRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[transformRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[transformRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[transformRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[transformRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[transformRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[transformRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[transformRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[transformRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[transformRecordVERS]->size = sizeof(prec->vers);
    prt->papFldDes[transformRecordRPVT]->size = sizeof(prec->rpvt);
    prt->papFldDes[transformRecordIVLA]->size = sizeof(prec->ivla);
    prt->papFldDes[transformRecordCOPT]->size = sizeof(prec->copt);
    prt->papFldDes[transformRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[transformRecordCLCA]->size = sizeof(prec->clca);
    prt->papFldDes[transformRecordCLCB]->size = sizeof(prec->clcb);
    prt->papFldDes[transformRecordCLCC]->size = sizeof(prec->clcc);
    prt->papFldDes[transformRecordCLCD]->size = sizeof(prec->clcd);
    prt->papFldDes[transformRecordCLCE]->size = sizeof(prec->clce);
    prt->papFldDes[transformRecordCLCF]->size = sizeof(prec->clcf);
    prt->papFldDes[transformRecordCLCG]->size = sizeof(prec->clcg);
    prt->papFldDes[transformRecordCLCH]->size = sizeof(prec->clch);
    prt->papFldDes[transformRecordCLCI]->size = sizeof(prec->clci);
    prt->papFldDes[transformRecordCLCJ]->size = sizeof(prec->clcj);
    prt->papFldDes[transformRecordCLCK]->size = sizeof(prec->clck);
    prt->papFldDes[transformRecordCLCL]->size = sizeof(prec->clcl);
    prt->papFldDes[transformRecordCLCM]->size = sizeof(prec->clcm);
    prt->papFldDes[transformRecordCLCN]->size = sizeof(prec->clcn);
    prt->papFldDes[transformRecordCLCO]->size = sizeof(prec->clco);
    prt->papFldDes[transformRecordCLCP]->size = sizeof(prec->clcp);
    prt->papFldDes[transformRecordCAV]->size = sizeof(prec->cav);
    prt->papFldDes[transformRecordCBV]->size = sizeof(prec->cbv);
    prt->papFldDes[transformRecordCCV]->size = sizeof(prec->ccv);
    prt->papFldDes[transformRecordCDV]->size = sizeof(prec->cdv);
    prt->papFldDes[transformRecordCEV]->size = sizeof(prec->cev);
    prt->papFldDes[transformRecordCFV]->size = sizeof(prec->cfv);
    prt->papFldDes[transformRecordCGV]->size = sizeof(prec->cgv);
    prt->papFldDes[transformRecordCHV]->size = sizeof(prec->chv);
    prt->papFldDes[transformRecordCIV]->size = sizeof(prec->civ);
    prt->papFldDes[transformRecordCJV]->size = sizeof(prec->cjv);
    prt->papFldDes[transformRecordCKV]->size = sizeof(prec->ckv);
    prt->papFldDes[transformRecordCLV]->size = sizeof(prec->clv);
    prt->papFldDes[transformRecordCMV]->size = sizeof(prec->cmv);
    prt->papFldDes[transformRecordCNV]->size = sizeof(prec->cnv);
    prt->papFldDes[transformRecordCOV]->size = sizeof(prec->cov);
    prt->papFldDes[transformRecordCPV]->size = sizeof(prec->cpv);
    prt->papFldDes[transformRecordINPA]->size = sizeof(prec->inpa);
    prt->papFldDes[transformRecordINPB]->size = sizeof(prec->inpb);
    prt->papFldDes[transformRecordINPC]->size = sizeof(prec->inpc);
    prt->papFldDes[transformRecordINPD]->size = sizeof(prec->inpd);
    prt->papFldDes[transformRecordINPE]->size = sizeof(prec->inpe);
    prt->papFldDes[transformRecordINPF]->size = sizeof(prec->inpf);
    prt->papFldDes[transformRecordINPG]->size = sizeof(prec->inpg);
    prt->papFldDes[transformRecordINPH]->size = sizeof(prec->inph);
    prt->papFldDes[transformRecordINPI]->size = sizeof(prec->inpi);
    prt->papFldDes[transformRecordINPJ]->size = sizeof(prec->inpj);
    prt->papFldDes[transformRecordINPK]->size = sizeof(prec->inpk);
    prt->papFldDes[transformRecordINPL]->size = sizeof(prec->inpl);
    prt->papFldDes[transformRecordINPM]->size = sizeof(prec->inpm);
    prt->papFldDes[transformRecordINPN]->size = sizeof(prec->inpn);
    prt->papFldDes[transformRecordINPO]->size = sizeof(prec->inpo);
    prt->papFldDes[transformRecordINPP]->size = sizeof(prec->inpp);
    prt->papFldDes[transformRecordOUTA]->size = sizeof(prec->outa);
    prt->papFldDes[transformRecordOUTB]->size = sizeof(prec->outb);
    prt->papFldDes[transformRecordOUTC]->size = sizeof(prec->outc);
    prt->papFldDes[transformRecordOUTD]->size = sizeof(prec->outd);
    prt->papFldDes[transformRecordOUTE]->size = sizeof(prec->oute);
    prt->papFldDes[transformRecordOUTF]->size = sizeof(prec->outf);
    prt->papFldDes[transformRecordOUTG]->size = sizeof(prec->outg);
    prt->papFldDes[transformRecordOUTH]->size = sizeof(prec->outh);
    prt->papFldDes[transformRecordOUTI]->size = sizeof(prec->outi);
    prt->papFldDes[transformRecordOUTJ]->size = sizeof(prec->outj);
    prt->papFldDes[transformRecordOUTK]->size = sizeof(prec->outk);
    prt->papFldDes[transformRecordOUTL]->size = sizeof(prec->outl);
    prt->papFldDes[transformRecordOUTM]->size = sizeof(prec->outm);
    prt->papFldDes[transformRecordOUTN]->size = sizeof(prec->outn);
    prt->papFldDes[transformRecordOUTO]->size = sizeof(prec->outo);
    prt->papFldDes[transformRecordOUTP]->size = sizeof(prec->outp);
    prt->papFldDes[transformRecordEGU]->size = sizeof(prec->egu);
    prt->papFldDes[transformRecordPREC]->size = sizeof(prec->prec);
    prt->papFldDes[transformRecordA]->size = sizeof(prec->a);
    prt->papFldDes[transformRecordB]->size = sizeof(prec->b);
    prt->papFldDes[transformRecordC]->size = sizeof(prec->c);
    prt->papFldDes[transformRecordD]->size = sizeof(prec->d);
    prt->papFldDes[transformRecordE]->size = sizeof(prec->e);
    prt->papFldDes[transformRecordF]->size = sizeof(prec->f);
    prt->papFldDes[transformRecordG]->size = sizeof(prec->g);
    prt->papFldDes[transformRecordH]->size = sizeof(prec->h);
    prt->papFldDes[transformRecordI]->size = sizeof(prec->i);
    prt->papFldDes[transformRecordJ]->size = sizeof(prec->j);
    prt->papFldDes[transformRecordK]->size = sizeof(prec->k);
    prt->papFldDes[transformRecordL]->size = sizeof(prec->l);
    prt->papFldDes[transformRecordM]->size = sizeof(prec->m);
    prt->papFldDes[transformRecordN]->size = sizeof(prec->n);
    prt->papFldDes[transformRecordO]->size = sizeof(prec->o);
    prt->papFldDes[transformRecordP]->size = sizeof(prec->p);
    prt->papFldDes[transformRecordLA]->size = sizeof(prec->la);
    prt->papFldDes[transformRecordLB]->size = sizeof(prec->lb);
    prt->papFldDes[transformRecordLC]->size = sizeof(prec->lc);
    prt->papFldDes[transformRecordLD]->size = sizeof(prec->ld);
    prt->papFldDes[transformRecordLE]->size = sizeof(prec->le);
    prt->papFldDes[transformRecordLF]->size = sizeof(prec->lf);
    prt->papFldDes[transformRecordLG]->size = sizeof(prec->lg);
    prt->papFldDes[transformRecordLH]->size = sizeof(prec->lh);
    prt->papFldDes[transformRecordLI]->size = sizeof(prec->li);
    prt->papFldDes[transformRecordLJ]->size = sizeof(prec->lj);
    prt->papFldDes[transformRecordLK]->size = sizeof(prec->lk);
    prt->papFldDes[transformRecordLL]->size = sizeof(prec->ll);
    prt->papFldDes[transformRecordLM]->size = sizeof(prec->lm);
    prt->papFldDes[transformRecordLN]->size = sizeof(prec->ln);
    prt->papFldDes[transformRecordLO]->size = sizeof(prec->lo);
    prt->papFldDes[transformRecordLP]->size = sizeof(prec->lp);
    prt->papFldDes[transformRecordRPCA]->size = sizeof(prec->rpca);
    prt->papFldDes[transformRecordRPCB]->size = sizeof(prec->rpcb);
    prt->papFldDes[transformRecordRPCC]->size = sizeof(prec->rpcc);
    prt->papFldDes[transformRecordRPCD]->size = sizeof(prec->rpcd);
    prt->papFldDes[transformRecordRPCE]->size = sizeof(prec->rpce);
    prt->papFldDes[transformRecordRPCF]->size = sizeof(prec->rpcf);
    prt->papFldDes[transformRecordRPCG]->size = sizeof(prec->rpcg);
    prt->papFldDes[transformRecordRPCH]->size = sizeof(prec->rpch);
    prt->papFldDes[transformRecordRPCI]->size = sizeof(prec->rpci);
    prt->papFldDes[transformRecordRPCJ]->size = sizeof(prec->rpcj);
    prt->papFldDes[transformRecordRPCK]->size = sizeof(prec->rpck);
    prt->papFldDes[transformRecordRPCL]->size = sizeof(prec->rpcl);
    prt->papFldDes[transformRecordRPCM]->size = sizeof(prec->rpcm);
    prt->papFldDes[transformRecordRPCN]->size = sizeof(prec->rpcn);
    prt->papFldDes[transformRecordRPCO]->size = sizeof(prec->rpco);
    prt->papFldDes[transformRecordRPCP]->size = sizeof(prec->rpcp);
    prt->papFldDes[transformRecordCMTA]->size = sizeof(prec->cmta);
    prt->papFldDes[transformRecordCMTB]->size = sizeof(prec->cmtb);
    prt->papFldDes[transformRecordCMTC]->size = sizeof(prec->cmtc);
    prt->papFldDes[transformRecordCMTD]->size = sizeof(prec->cmtd);
    prt->papFldDes[transformRecordCMTE]->size = sizeof(prec->cmte);
    prt->papFldDes[transformRecordCMTF]->size = sizeof(prec->cmtf);
    prt->papFldDes[transformRecordCMTG]->size = sizeof(prec->cmtg);
    prt->papFldDes[transformRecordCMTH]->size = sizeof(prec->cmth);
    prt->papFldDes[transformRecordCMTI]->size = sizeof(prec->cmti);
    prt->papFldDes[transformRecordCMTJ]->size = sizeof(prec->cmtj);
    prt->papFldDes[transformRecordCMTK]->size = sizeof(prec->cmtk);
    prt->papFldDes[transformRecordCMTL]->size = sizeof(prec->cmtl);
    prt->papFldDes[transformRecordCMTM]->size = sizeof(prec->cmtm);
    prt->papFldDes[transformRecordCMTN]->size = sizeof(prec->cmtn);
    prt->papFldDes[transformRecordCMTO]->size = sizeof(prec->cmto);
    prt->papFldDes[transformRecordCMTP]->size = sizeof(prec->cmtp);
    prt->papFldDes[transformRecordMAP]->size = sizeof(prec->map);
    prt->papFldDes[transformRecordIAV]->size = sizeof(prec->iav);
    prt->papFldDes[transformRecordIBV]->size = sizeof(prec->ibv);
    prt->papFldDes[transformRecordICV]->size = sizeof(prec->icv);
    prt->papFldDes[transformRecordIDV]->size = sizeof(prec->idv);
    prt->papFldDes[transformRecordIEV]->size = sizeof(prec->iev);
    prt->papFldDes[transformRecordIFV]->size = sizeof(prec->ifv);
    prt->papFldDes[transformRecordIGV]->size = sizeof(prec->igv);
    prt->papFldDes[transformRecordIHV]->size = sizeof(prec->ihv);
    prt->papFldDes[transformRecordIIV]->size = sizeof(prec->iiv);
    prt->papFldDes[transformRecordIJV]->size = sizeof(prec->ijv);
    prt->papFldDes[transformRecordIKV]->size = sizeof(prec->ikv);
    prt->papFldDes[transformRecordILV]->size = sizeof(prec->ilv);
    prt->papFldDes[transformRecordIMV]->size = sizeof(prec->imv);
    prt->papFldDes[transformRecordINV]->size = sizeof(prec->inv);
    prt->papFldDes[transformRecordIOV]->size = sizeof(prec->iov);
    prt->papFldDes[transformRecordIPV]->size = sizeof(prec->ipv);
    prt->papFldDes[transformRecordOAV]->size = sizeof(prec->oav);
    prt->papFldDes[transformRecordOBV]->size = sizeof(prec->obv);
    prt->papFldDes[transformRecordOCV]->size = sizeof(prec->ocv);
    prt->papFldDes[transformRecordODV]->size = sizeof(prec->odv);
    prt->papFldDes[transformRecordOEV]->size = sizeof(prec->oev);
    prt->papFldDes[transformRecordOFV]->size = sizeof(prec->ofv);
    prt->papFldDes[transformRecordOGV]->size = sizeof(prec->ogv);
    prt->papFldDes[transformRecordOHV]->size = sizeof(prec->ohv);
    prt->papFldDes[transformRecordOIV]->size = sizeof(prec->oiv);
    prt->papFldDes[transformRecordOJV]->size = sizeof(prec->ojv);
    prt->papFldDes[transformRecordOKV]->size = sizeof(prec->okv);
    prt->papFldDes[transformRecordOLV]->size = sizeof(prec->olv);
    prt->papFldDes[transformRecordOMV]->size = sizeof(prec->omv);
    prt->papFldDes[transformRecordONV]->size = sizeof(prec->onv);
    prt->papFldDes[transformRecordOOV]->size = sizeof(prec->oov);
    prt->papFldDes[transformRecordOPV]->size = sizeof(prec->opv);
    prt->papFldDes[transformRecordNAME]->offset = (unsigned short)((char *)&prec->name - (char *)prec);
    prt->papFldDes[transformRecordDESC]->offset = (unsigned short)((char *)&prec->desc - (char *)prec);
    prt->papFldDes[transformRecordASG]->offset = (unsigned short)((char *)&prec->asg - (char *)prec);
    prt->papFldDes[transformRecordSCAN]->offset = (unsigned short)((char *)&prec->scan - (char *)prec);
    prt->papFldDes[transformRecordPINI]->offset = (unsigned short)((char *)&prec->pini - (char *)prec);
    prt->papFldDes[transformRecordPHAS]->offset = (unsigned short)((char *)&prec->phas - (char *)prec);
    prt->papFldDes[transformRecordEVNT]->offset = (unsigned short)((char *)&prec->evnt - (char *)prec);
    prt->papFldDes[transformRecordTSE]->offset = (unsigned short)((char *)&prec->tse - (char *)prec);
    prt->papFldDes[transformRecordTSEL]->offset = (unsigned short)((char *)&prec->tsel - (char *)prec);
    prt->papFldDes[transformRecordDTYP]->offset = (unsigned short)((char *)&prec->dtyp - (char *)prec);
    prt->papFldDes[transformRecordDISV]->offset = (unsigned short)((char *)&prec->disv - (char *)prec);
    prt->papFldDes[transformRecordDISA]->offset = (unsigned short)((char *)&prec->disa - (char *)prec);
    prt->papFldDes[transformRecordSDIS]->offset = (unsigned short)((char *)&prec->sdis - (char *)prec);
    prt->papFldDes[transformRecordMLOK]->offset = (unsigned short)((char *)&prec->mlok - (char *)prec);
    prt->papFldDes[transformRecordMLIS]->offset = (unsigned short)((char *)&prec->mlis - (char *)prec);
    prt->papFldDes[transformRecordDISP]->offset = (unsigned short)((char *)&prec->disp - (char *)prec);
    prt->papFldDes[transformRecordPROC]->offset = (unsigned short)((char *)&prec->proc - (char *)prec);
    prt->papFldDes[transformRecordSTAT]->offset = (unsigned short)((char *)&prec->stat - (char *)prec);
    prt->papFldDes[transformRecordSEVR]->offset = (unsigned short)((char *)&prec->sevr - (char *)prec);
    prt->papFldDes[transformRecordNSTA]->offset = (unsigned short)((char *)&prec->nsta - (char *)prec);
    prt->papFldDes[transformRecordNSEV]->offset = (unsigned short)((char *)&prec->nsev - (char *)prec);
    prt->papFldDes[transformRecordACKS]->offset = (unsigned short)((char *)&prec->acks - (char *)prec);
    prt->papFldDes[transformRecordACKT]->offset = (unsigned short)((char *)&prec->ackt - (char *)prec);
    prt->papFldDes[transformRecordDISS]->offset = (unsigned short)((char *)&prec->diss - (char *)prec);
    prt->papFldDes[transformRecordLCNT]->offset = (unsigned short)((char *)&prec->lcnt - (char *)prec);
    prt->papFldDes[transformRecordPACT]->offset = (unsigned short)((char *)&prec->pact - (char *)prec);
    prt->papFldDes[transformRecordPUTF]->offset = (unsigned short)((char *)&prec->putf - (char *)prec);
    prt->papFldDes[transformRecordRPRO]->offset = (unsigned short)((char *)&prec->rpro - (char *)prec);
    prt->papFldDes[transformRecordASP]->offset = (unsigned short)((char *)&prec->asp - (char *)prec);
    prt->papFldDes[transformRecordPPN]->offset = (unsigned short)((char *)&prec->ppn - (char *)prec);
    prt->papFldDes[transformRecordPPNR]->offset = (unsigned short)((char *)&prec->ppnr - (char *)prec);
    prt->papFldDes[transformRecordSPVT]->offset = (unsigned short)((char *)&prec->spvt - (char *)prec);
    prt->papFldDes[transformRecordRSET]->offset = (unsigned short)((char *)&prec->rset - (char *)prec);
    prt->papFldDes[transformRecordDSET]->offset = (unsigned short)((char *)&prec->dset - (char *)prec);
    prt->papFldDes[transformRecordDPVT]->offset = (unsigned short)((char *)&prec->dpvt - (char *)prec);
    prt->papFldDes[transformRecordRDES]->offset = (unsigned short)((char *)&prec->rdes - (char *)prec);
    prt->papFldDes[transformRecordLSET]->offset = (unsigned short)((char *)&prec->lset - (char *)prec);
    prt->papFldDes[transformRecordPRIO]->offset = (unsigned short)((char *)&prec->prio - (char *)prec);
    prt->papFldDes[transformRecordTPRO]->offset = (unsigned short)((char *)&prec->tpro - (char *)prec);
    prt->papFldDes[transformRecordBKPT]->offset = (unsigned short)((char *)&prec->bkpt - (char *)prec);
    prt->papFldDes[transformRecordUDF]->offset = (unsigned short)((char *)&prec->udf - (char *)prec);
    prt->papFldDes[transformRecordUDFS]->offset = (unsigned short)((char *)&prec->udfs - (char *)prec);
    prt->papFldDes[transformRecordTIME]->offset = (unsigned short)((char *)&prec->time - (char *)prec);
    prt->papFldDes[transformRecordFLNK]->offset = (unsigned short)((char *)&prec->flnk - (char *)prec);
    prt->papFldDes[transformRecordVERS]->offset = (unsigned short)((char *)&prec->vers - (char *)prec);
    prt->papFldDes[transformRecordRPVT]->offset = (unsigned short)((char *)&prec->rpvt - (char *)prec);
    prt->papFldDes[transformRecordIVLA]->offset = (unsigned short)((char *)&prec->ivla - (char *)prec);
    prt->papFldDes[transformRecordCOPT]->offset = (unsigned short)((char *)&prec->copt - (char *)prec);
    prt->papFldDes[transformRecordVAL]->offset = (unsigned short)((char *)&prec->val - (char *)prec);
    prt->papFldDes[transformRecordCLCA]->offset = (unsigned short)((char *)&prec->clca - (char *)prec);
    prt->papFldDes[transformRecordCLCB]->offset = (unsigned short)((char *)&prec->clcb - (char *)prec);
    prt->papFldDes[transformRecordCLCC]->offset = (unsigned short)((char *)&prec->clcc - (char *)prec);
    prt->papFldDes[transformRecordCLCD]->offset = (unsigned short)((char *)&prec->clcd - (char *)prec);
    prt->papFldDes[transformRecordCLCE]->offset = (unsigned short)((char *)&prec->clce - (char *)prec);
    prt->papFldDes[transformRecordCLCF]->offset = (unsigned short)((char *)&prec->clcf - (char *)prec);
    prt->papFldDes[transformRecordCLCG]->offset = (unsigned short)((char *)&prec->clcg - (char *)prec);
    prt->papFldDes[transformRecordCLCH]->offset = (unsigned short)((char *)&prec->clch - (char *)prec);
    prt->papFldDes[transformRecordCLCI]->offset = (unsigned short)((char *)&prec->clci - (char *)prec);
    prt->papFldDes[transformRecordCLCJ]->offset = (unsigned short)((char *)&prec->clcj - (char *)prec);
    prt->papFldDes[transformRecordCLCK]->offset = (unsigned short)((char *)&prec->clck - (char *)prec);
    prt->papFldDes[transformRecordCLCL]->offset = (unsigned short)((char *)&prec->clcl - (char *)prec);
    prt->papFldDes[transformRecordCLCM]->offset = (unsigned short)((char *)&prec->clcm - (char *)prec);
    prt->papFldDes[transformRecordCLCN]->offset = (unsigned short)((char *)&prec->clcn - (char *)prec);
    prt->papFldDes[transformRecordCLCO]->offset = (unsigned short)((char *)&prec->clco - (char *)prec);
    prt->papFldDes[transformRecordCLCP]->offset = (unsigned short)((char *)&prec->clcp - (char *)prec);
    prt->papFldDes[transformRecordCAV]->offset = (unsigned short)((char *)&prec->cav - (char *)prec);
    prt->papFldDes[transformRecordCBV]->offset = (unsigned short)((char *)&prec->cbv - (char *)prec);
    prt->papFldDes[transformRecordCCV]->offset = (unsigned short)((char *)&prec->ccv - (char *)prec);
    prt->papFldDes[transformRecordCDV]->offset = (unsigned short)((char *)&prec->cdv - (char *)prec);
    prt->papFldDes[transformRecordCEV]->offset = (unsigned short)((char *)&prec->cev - (char *)prec);
    prt->papFldDes[transformRecordCFV]->offset = (unsigned short)((char *)&prec->cfv - (char *)prec);
    prt->papFldDes[transformRecordCGV]->offset = (unsigned short)((char *)&prec->cgv - (char *)prec);
    prt->papFldDes[transformRecordCHV]->offset = (unsigned short)((char *)&prec->chv - (char *)prec);
    prt->papFldDes[transformRecordCIV]->offset = (unsigned short)((char *)&prec->civ - (char *)prec);
    prt->papFldDes[transformRecordCJV]->offset = (unsigned short)((char *)&prec->cjv - (char *)prec);
    prt->papFldDes[transformRecordCKV]->offset = (unsigned short)((char *)&prec->ckv - (char *)prec);
    prt->papFldDes[transformRecordCLV]->offset = (unsigned short)((char *)&prec->clv - (char *)prec);
    prt->papFldDes[transformRecordCMV]->offset = (unsigned short)((char *)&prec->cmv - (char *)prec);
    prt->papFldDes[transformRecordCNV]->offset = (unsigned short)((char *)&prec->cnv - (char *)prec);
    prt->papFldDes[transformRecordCOV]->offset = (unsigned short)((char *)&prec->cov - (char *)prec);
    prt->papFldDes[transformRecordCPV]->offset = (unsigned short)((char *)&prec->cpv - (char *)prec);
    prt->papFldDes[transformRecordINPA]->offset = (unsigned short)((char *)&prec->inpa - (char *)prec);
    prt->papFldDes[transformRecordINPB]->offset = (unsigned short)((char *)&prec->inpb - (char *)prec);
    prt->papFldDes[transformRecordINPC]->offset = (unsigned short)((char *)&prec->inpc - (char *)prec);
    prt->papFldDes[transformRecordINPD]->offset = (unsigned short)((char *)&prec->inpd - (char *)prec);
    prt->papFldDes[transformRecordINPE]->offset = (unsigned short)((char *)&prec->inpe - (char *)prec);
    prt->papFldDes[transformRecordINPF]->offset = (unsigned short)((char *)&prec->inpf - (char *)prec);
    prt->papFldDes[transformRecordINPG]->offset = (unsigned short)((char *)&prec->inpg - (char *)prec);
    prt->papFldDes[transformRecordINPH]->offset = (unsigned short)((char *)&prec->inph - (char *)prec);
    prt->papFldDes[transformRecordINPI]->offset = (unsigned short)((char *)&prec->inpi - (char *)prec);
    prt->papFldDes[transformRecordINPJ]->offset = (unsigned short)((char *)&prec->inpj - (char *)prec);
    prt->papFldDes[transformRecordINPK]->offset = (unsigned short)((char *)&prec->inpk - (char *)prec);
    prt->papFldDes[transformRecordINPL]->offset = (unsigned short)((char *)&prec->inpl - (char *)prec);
    prt->papFldDes[transformRecordINPM]->offset = (unsigned short)((char *)&prec->inpm - (char *)prec);
    prt->papFldDes[transformRecordINPN]->offset = (unsigned short)((char *)&prec->inpn - (char *)prec);
    prt->papFldDes[transformRecordINPO]->offset = (unsigned short)((char *)&prec->inpo - (char *)prec);
    prt->papFldDes[transformRecordINPP]->offset = (unsigned short)((char *)&prec->inpp - (char *)prec);
    prt->papFldDes[transformRecordOUTA]->offset = (unsigned short)((char *)&prec->outa - (char *)prec);
    prt->papFldDes[transformRecordOUTB]->offset = (unsigned short)((char *)&prec->outb - (char *)prec);
    prt->papFldDes[transformRecordOUTC]->offset = (unsigned short)((char *)&prec->outc - (char *)prec);
    prt->papFldDes[transformRecordOUTD]->offset = (unsigned short)((char *)&prec->outd - (char *)prec);
    prt->papFldDes[transformRecordOUTE]->offset = (unsigned short)((char *)&prec->oute - (char *)prec);
    prt->papFldDes[transformRecordOUTF]->offset = (unsigned short)((char *)&prec->outf - (char *)prec);
    prt->papFldDes[transformRecordOUTG]->offset = (unsigned short)((char *)&prec->outg - (char *)prec);
    prt->papFldDes[transformRecordOUTH]->offset = (unsigned short)((char *)&prec->outh - (char *)prec);
    prt->papFldDes[transformRecordOUTI]->offset = (unsigned short)((char *)&prec->outi - (char *)prec);
    prt->papFldDes[transformRecordOUTJ]->offset = (unsigned short)((char *)&prec->outj - (char *)prec);
    prt->papFldDes[transformRecordOUTK]->offset = (unsigned short)((char *)&prec->outk - (char *)prec);
    prt->papFldDes[transformRecordOUTL]->offset = (unsigned short)((char *)&prec->outl - (char *)prec);
    prt->papFldDes[transformRecordOUTM]->offset = (unsigned short)((char *)&prec->outm - (char *)prec);
    prt->papFldDes[transformRecordOUTN]->offset = (unsigned short)((char *)&prec->outn - (char *)prec);
    prt->papFldDes[transformRecordOUTO]->offset = (unsigned short)((char *)&prec->outo - (char *)prec);
    prt->papFldDes[transformRecordOUTP]->offset = (unsigned short)((char *)&prec->outp - (char *)prec);
    prt->papFldDes[transformRecordEGU]->offset = (unsigned short)((char *)&prec->egu - (char *)prec);
    prt->papFldDes[transformRecordPREC]->offset = (unsigned short)((char *)&prec->prec - (char *)prec);
    prt->papFldDes[transformRecordA]->offset = (unsigned short)((char *)&prec->a - (char *)prec);
    prt->papFldDes[transformRecordB]->offset = (unsigned short)((char *)&prec->b - (char *)prec);
    prt->papFldDes[transformRecordC]->offset = (unsigned short)((char *)&prec->c - (char *)prec);
    prt->papFldDes[transformRecordD]->offset = (unsigned short)((char *)&prec->d - (char *)prec);
    prt->papFldDes[transformRecordE]->offset = (unsigned short)((char *)&prec->e - (char *)prec);
    prt->papFldDes[transformRecordF]->offset = (unsigned short)((char *)&prec->f - (char *)prec);
    prt->papFldDes[transformRecordG]->offset = (unsigned short)((char *)&prec->g - (char *)prec);
    prt->papFldDes[transformRecordH]->offset = (unsigned short)((char *)&prec->h - (char *)prec);
    prt->papFldDes[transformRecordI]->offset = (unsigned short)((char *)&prec->i - (char *)prec);
    prt->papFldDes[transformRecordJ]->offset = (unsigned short)((char *)&prec->j - (char *)prec);
    prt->papFldDes[transformRecordK]->offset = (unsigned short)((char *)&prec->k - (char *)prec);
    prt->papFldDes[transformRecordL]->offset = (unsigned short)((char *)&prec->l - (char *)prec);
    prt->papFldDes[transformRecordM]->offset = (unsigned short)((char *)&prec->m - (char *)prec);
    prt->papFldDes[transformRecordN]->offset = (unsigned short)((char *)&prec->n - (char *)prec);
    prt->papFldDes[transformRecordO]->offset = (unsigned short)((char *)&prec->o - (char *)prec);
    prt->papFldDes[transformRecordP]->offset = (unsigned short)((char *)&prec->p - (char *)prec);
    prt->papFldDes[transformRecordLA]->offset = (unsigned short)((char *)&prec->la - (char *)prec);
    prt->papFldDes[transformRecordLB]->offset = (unsigned short)((char *)&prec->lb - (char *)prec);
    prt->papFldDes[transformRecordLC]->offset = (unsigned short)((char *)&prec->lc - (char *)prec);
    prt->papFldDes[transformRecordLD]->offset = (unsigned short)((char *)&prec->ld - (char *)prec);
    prt->papFldDes[transformRecordLE]->offset = (unsigned short)((char *)&prec->le - (char *)prec);
    prt->papFldDes[transformRecordLF]->offset = (unsigned short)((char *)&prec->lf - (char *)prec);
    prt->papFldDes[transformRecordLG]->offset = (unsigned short)((char *)&prec->lg - (char *)prec);
    prt->papFldDes[transformRecordLH]->offset = (unsigned short)((char *)&prec->lh - (char *)prec);
    prt->papFldDes[transformRecordLI]->offset = (unsigned short)((char *)&prec->li - (char *)prec);
    prt->papFldDes[transformRecordLJ]->offset = (unsigned short)((char *)&prec->lj - (char *)prec);
    prt->papFldDes[transformRecordLK]->offset = (unsigned short)((char *)&prec->lk - (char *)prec);
    prt->papFldDes[transformRecordLL]->offset = (unsigned short)((char *)&prec->ll - (char *)prec);
    prt->papFldDes[transformRecordLM]->offset = (unsigned short)((char *)&prec->lm - (char *)prec);
    prt->papFldDes[transformRecordLN]->offset = (unsigned short)((char *)&prec->ln - (char *)prec);
    prt->papFldDes[transformRecordLO]->offset = (unsigned short)((char *)&prec->lo - (char *)prec);
    prt->papFldDes[transformRecordLP]->offset = (unsigned short)((char *)&prec->lp - (char *)prec);
    prt->papFldDes[transformRecordRPCA]->offset = (unsigned short)((char *)&prec->rpca - (char *)prec);
    prt->papFldDes[transformRecordRPCB]->offset = (unsigned short)((char *)&prec->rpcb - (char *)prec);
    prt->papFldDes[transformRecordRPCC]->offset = (unsigned short)((char *)&prec->rpcc - (char *)prec);
    prt->papFldDes[transformRecordRPCD]->offset = (unsigned short)((char *)&prec->rpcd - (char *)prec);
    prt->papFldDes[transformRecordRPCE]->offset = (unsigned short)((char *)&prec->rpce - (char *)prec);
    prt->papFldDes[transformRecordRPCF]->offset = (unsigned short)((char *)&prec->rpcf - (char *)prec);
    prt->papFldDes[transformRecordRPCG]->offset = (unsigned short)((char *)&prec->rpcg - (char *)prec);
    prt->papFldDes[transformRecordRPCH]->offset = (unsigned short)((char *)&prec->rpch - (char *)prec);
    prt->papFldDes[transformRecordRPCI]->offset = (unsigned short)((char *)&prec->rpci - (char *)prec);
    prt->papFldDes[transformRecordRPCJ]->offset = (unsigned short)((char *)&prec->rpcj - (char *)prec);
    prt->papFldDes[transformRecordRPCK]->offset = (unsigned short)((char *)&prec->rpck - (char *)prec);
    prt->papFldDes[transformRecordRPCL]->offset = (unsigned short)((char *)&prec->rpcl - (char *)prec);
    prt->papFldDes[transformRecordRPCM]->offset = (unsigned short)((char *)&prec->rpcm - (char *)prec);
    prt->papFldDes[transformRecordRPCN]->offset = (unsigned short)((char *)&prec->rpcn - (char *)prec);
    prt->papFldDes[transformRecordRPCO]->offset = (unsigned short)((char *)&prec->rpco - (char *)prec);
    prt->papFldDes[transformRecordRPCP]->offset = (unsigned short)((char *)&prec->rpcp - (char *)prec);
    prt->papFldDes[transformRecordCMTA]->offset = (unsigned short)((char *)&prec->cmta - (char *)prec);
    prt->papFldDes[transformRecordCMTB]->offset = (unsigned short)((char *)&prec->cmtb - (char *)prec);
    prt->papFldDes[transformRecordCMTC]->offset = (unsigned short)((char *)&prec->cmtc - (char *)prec);
    prt->papFldDes[transformRecordCMTD]->offset = (unsigned short)((char *)&prec->cmtd - (char *)prec);
    prt->papFldDes[transformRecordCMTE]->offset = (unsigned short)((char *)&prec->cmte - (char *)prec);
    prt->papFldDes[transformRecordCMTF]->offset = (unsigned short)((char *)&prec->cmtf - (char *)prec);
    prt->papFldDes[transformRecordCMTG]->offset = (unsigned short)((char *)&prec->cmtg - (char *)prec);
    prt->papFldDes[transformRecordCMTH]->offset = (unsigned short)((char *)&prec->cmth - (char *)prec);
    prt->papFldDes[transformRecordCMTI]->offset = (unsigned short)((char *)&prec->cmti - (char *)prec);
    prt->papFldDes[transformRecordCMTJ]->offset = (unsigned short)((char *)&prec->cmtj - (char *)prec);
    prt->papFldDes[transformRecordCMTK]->offset = (unsigned short)((char *)&prec->cmtk - (char *)prec);
    prt->papFldDes[transformRecordCMTL]->offset = (unsigned short)((char *)&prec->cmtl - (char *)prec);
    prt->papFldDes[transformRecordCMTM]->offset = (unsigned short)((char *)&prec->cmtm - (char *)prec);
    prt->papFldDes[transformRecordCMTN]->offset = (unsigned short)((char *)&prec->cmtn - (char *)prec);
    prt->papFldDes[transformRecordCMTO]->offset = (unsigned short)((char *)&prec->cmto - (char *)prec);
    prt->papFldDes[transformRecordCMTP]->offset = (unsigned short)((char *)&prec->cmtp - (char *)prec);
    prt->papFldDes[transformRecordMAP]->offset = (unsigned short)((char *)&prec->map - (char *)prec);
    prt->papFldDes[transformRecordIAV]->offset = (unsigned short)((char *)&prec->iav - (char *)prec);
    prt->papFldDes[transformRecordIBV]->offset = (unsigned short)((char *)&prec->ibv - (char *)prec);
    prt->papFldDes[transformRecordICV]->offset = (unsigned short)((char *)&prec->icv - (char *)prec);
    prt->papFldDes[transformRecordIDV]->offset = (unsigned short)((char *)&prec->idv - (char *)prec);
    prt->papFldDes[transformRecordIEV]->offset = (unsigned short)((char *)&prec->iev - (char *)prec);
    prt->papFldDes[transformRecordIFV]->offset = (unsigned short)((char *)&prec->ifv - (char *)prec);
    prt->papFldDes[transformRecordIGV]->offset = (unsigned short)((char *)&prec->igv - (char *)prec);
    prt->papFldDes[transformRecordIHV]->offset = (unsigned short)((char *)&prec->ihv - (char *)prec);
    prt->papFldDes[transformRecordIIV]->offset = (unsigned short)((char *)&prec->iiv - (char *)prec);
    prt->papFldDes[transformRecordIJV]->offset = (unsigned short)((char *)&prec->ijv - (char *)prec);
    prt->papFldDes[transformRecordIKV]->offset = (unsigned short)((char *)&prec->ikv - (char *)prec);
    prt->papFldDes[transformRecordILV]->offset = (unsigned short)((char *)&prec->ilv - (char *)prec);
    prt->papFldDes[transformRecordIMV]->offset = (unsigned short)((char *)&prec->imv - (char *)prec);
    prt->papFldDes[transformRecordINV]->offset = (unsigned short)((char *)&prec->inv - (char *)prec);
    prt->papFldDes[transformRecordIOV]->offset = (unsigned short)((char *)&prec->iov - (char *)prec);
    prt->papFldDes[transformRecordIPV]->offset = (unsigned short)((char *)&prec->ipv - (char *)prec);
    prt->papFldDes[transformRecordOAV]->offset = (unsigned short)((char *)&prec->oav - (char *)prec);
    prt->papFldDes[transformRecordOBV]->offset = (unsigned short)((char *)&prec->obv - (char *)prec);
    prt->papFldDes[transformRecordOCV]->offset = (unsigned short)((char *)&prec->ocv - (char *)prec);
    prt->papFldDes[transformRecordODV]->offset = (unsigned short)((char *)&prec->odv - (char *)prec);
    prt->papFldDes[transformRecordOEV]->offset = (unsigned short)((char *)&prec->oev - (char *)prec);
    prt->papFldDes[transformRecordOFV]->offset = (unsigned short)((char *)&prec->ofv - (char *)prec);
    prt->papFldDes[transformRecordOGV]->offset = (unsigned short)((char *)&prec->ogv - (char *)prec);
    prt->papFldDes[transformRecordOHV]->offset = (unsigned short)((char *)&prec->ohv - (char *)prec);
    prt->papFldDes[transformRecordOIV]->offset = (unsigned short)((char *)&prec->oiv - (char *)prec);
    prt->papFldDes[transformRecordOJV]->offset = (unsigned short)((char *)&prec->ojv - (char *)prec);
    prt->papFldDes[transformRecordOKV]->offset = (unsigned short)((char *)&prec->okv - (char *)prec);
    prt->papFldDes[transformRecordOLV]->offset = (unsigned short)((char *)&prec->olv - (char *)prec);
    prt->papFldDes[transformRecordOMV]->offset = (unsigned short)((char *)&prec->omv - (char *)prec);
    prt->papFldDes[transformRecordONV]->offset = (unsigned short)((char *)&prec->onv - (char *)prec);
    prt->papFldDes[transformRecordOOV]->offset = (unsigned short)((char *)&prec->oov - (char *)prec);
    prt->papFldDes[transformRecordOPV]->offset = (unsigned short)((char *)&prec->opv - (char *)prec);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(transformRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_transformRecord_H */
