/* sscanRecord.h generated from sscanRecord.dbd */

#ifndef INC_sscanRecord_H
#define INC_sscanRecord_H

 #include "epicsTypes.h"
 #include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "epicsTime.h"

typedef struct sscanRecord {
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
    epicsFloat64        vers;       /* Code Version */
    epicsFloat64        val;        /* Value Field */
    char                smsg[40];   /* Record State Msg */
    epicsEnum16         cmnd;       /* Command Field */
    epicsUInt8          alrt;       /* Operator Alert */
    void *           rpvt;          /* Ptr to Pvt Struct */
    epicsInt32          mpts;       /* Max # of Points */
    epicsInt16          exsc;       /* Execute Scan */
    epicsInt16          xsc;        /* Internal execScan */
    epicsUInt8          pxsc;       /* Previous XScan */
    epicsUInt8          busy;       /* Scan in progress */
    epicsUInt8          kill;       /* Abort right now */
    epicsInt16          wait;       /* Wait for client(s) */
    epicsInt16          wcnt;       /* Wait count) */
    epicsInt16          awct;       /* Auto WCNT */
    epicsInt16          wtng;       /* Waiting for client(s) */
    epicsInt16          await;      /* Waiting for data-storage client */
    epicsEnum16         aawait;     /* AutoWait for data-storage client */
    epicsInt16          data;       /* Scan data ready */
    epicsInt16          refd;       /* Reference detector */
    epicsInt32          npts;       /* Number of Points */
    epicsEnum16         fpts;       /* Freeze Num of Points */
    epicsEnum16         ffo;        /* Freeze Flag Override */
    epicsInt32          cpt;        /* Current Point */
    epicsInt32          bcpt;       /* Bufferred Current Point */
    epicsInt32          dpt;        /* Desired Point */
    epicsInt32          pcpt;       /* Point ofLast Posting */
    epicsEnum16         pasm;       /* After Scan Mode */
    epicsTimeStamp	tolp;            /* Time of Last Posting */
    epicsTimeStamp	tlap;            /* Time of Last Array Posting */
    epicsFloat32        atime;      /* Array post time period */
    char                p1pv[40];   /* Positioner 1 PV Name */
    char                p2pv[40];   /* Positioner 2 PV Name */
    char                p3pv[40];   /* Positioner 3 PV Name */
    char                p4pv[40];   /* Positioner 4 PV Name */
    char                r1pv[40];   /* P1 Readback  PV Name */
    char                r2pv[40];   /* P2 Readback  PV Name */
    char                r3pv[40];   /* P3 Readback  PV Name */
    char                r4pv[40];   /* P4 Readback  PV Name */
    char                d01pv[40];  /* Detector PV Name */
    char                d02pv[40];  /* Detector PV Name */
    char                d03pv[40];  /* Detector PV Name */
    char                d04pv[40];  /* Detector PV Name */
    char                d05pv[40];  /* Detector PV Name */
    char                d06pv[40];  /* Detector PV Name */
    char                d07pv[40];  /* Detector PV Name */
    char                d08pv[40];  /* Detector PV Name */
    char                d09pv[40];  /* Detector PV Name */
    char                d10pv[40];  /* Detector PV Name */
    char                d11pv[40];  /* Detector PV Name */
    char                d12pv[40];  /* Detector PV Name */
    char                d13pv[40];  /* Detector PV Name */
    char                d14pv[40];  /* Detector PV Name */
    char                d15pv[40];  /* Detector PV Name */
    char                d16pv[40];  /* Detector PV Name */
    char                d17pv[40];  /* Detector PV Name */
    char                d18pv[40];  /* Detector PV Name */
    char                d19pv[40];  /* Detector PV Name */
    char                d20pv[40];  /* Detector PV Name */
    char                d21pv[40];  /* Detector PV Name */
    char                d22pv[40];  /* Detector PV Name */
    char                d23pv[40];  /* Detector PV Name */
    char                d24pv[40];  /* Detector PV Name */
    char                d25pv[40];  /* Detector PV Name */
    char                d26pv[40];  /* Detector PV Name */
    char                d27pv[40];  /* Detector PV Name */
    char                d28pv[40];  /* Detector PV Name */
    char                d29pv[40];  /* Detector PV Name */
    char                d30pv[40];  /* Detector PV Name */
    char                d31pv[40];  /* Detector PV Name */
    char                d32pv[40];  /* Detector PV Name */
    char                d33pv[40];  /* Detector PV Name */
    char                d34pv[40];  /* Detector PV Name */
    char                d35pv[40];  /* Detector PV Name */
    char                d36pv[40];  /* Detector PV Name */
    char                d37pv[40];  /* Detector PV Name */
    char                d38pv[40];  /* Detector PV Name */
    char                d39pv[40];  /* Detector PV Name */
    char                d40pv[40];  /* Detector PV Name */
    char                d41pv[40];  /* Detector PV Name */
    char                d42pv[40];  /* Detector PV Name */
    char                d43pv[40];  /* Detector PV Name */
    char                d44pv[40];  /* Detector PV Name */
    char                d45pv[40];  /* Detector PV Name */
    char                d46pv[40];  /* Detector PV Name */
    char                d47pv[40];  /* Detector PV Name */
    char                d48pv[40];  /* Detector PV Name */
    char                d49pv[40];  /* Detector PV Name */
    char                d50pv[40];  /* Detector PV Name */
    char                d51pv[40];  /* Detector PV Name */
    char                d52pv[40];  /* Detector PV Name */
    char                d53pv[40];  /* Detector PV Name */
    char                d54pv[40];  /* Detector PV Name */
    char                d55pv[40];  /* Detector PV Name */
    char                d56pv[40];  /* Detector PV Name */
    char                d57pv[40];  /* Detector PV Name */
    char                d58pv[40];  /* Detector PV Name */
    char                d59pv[40];  /* Detector PV Name */
    char                d60pv[40];  /* Detector PV Name */
    char                d61pv[40];  /* Detector PV Name */
    char                d62pv[40];  /* Detector PV Name */
    char                d63pv[40];  /* Detector PV Name */
    char                d64pv[40];  /* Detector PV Name */
    char                d65pv[40];  /* Detector PV Name */
    char                d66pv[40];  /* Detector PV Name */
    char                d67pv[40];  /* Detector PV Name */
    char                d68pv[40];  /* Detector PV Name */
    char                d69pv[40];  /* Detector PV Name */
    char                d70pv[40];  /* Detector PV Name */
    char                t1pv[40];   /* Trigger 1    PV Name */
    char                t2pv[40];   /* Trigger 2    PV Name */
    char                t3pv[40];   /* Trigger 3    PV Name */
    char                t4pv[40];   /* Trigger 4    PV Name */
    char                a1pv[40];   /* Array-read trigger 1 PV Name */
    char                bspv[40];   /* Before Scan  PV Name */
    char                aspv[40];   /* After Scan   PV Name */
    epicsEnum16         bswait;     /* Wait for completion? */
    epicsEnum16         aswait;     /* Wait for completion? */
    epicsEnum16         p1nv;       /* P1  PV Status */
    epicsEnum16         p2nv;       /* P2  PV Status */
    epicsEnum16         p3nv;       /* P3  PV Status */
    epicsEnum16         p4nv;       /* P4  PV Status */
    epicsEnum16         r1nv;       /* R1  PV Status */
    epicsEnum16         r2nv;       /* R2  PV Status */
    epicsEnum16         r3nv;       /* R3  PV Status */
    epicsEnum16         r4nv;       /* R4  PV Status */
    epicsEnum16         d01nv;      /* PV Status */
    epicsEnum16         d02nv;      /* PV Status */
    epicsEnum16         d03nv;      /* PV Status */
    epicsEnum16         d04nv;      /* PV Status */
    epicsEnum16         d05nv;      /* PV Status */
    epicsEnum16         d06nv;      /* PV Status */
    epicsEnum16         d07nv;      /* PV Status */
    epicsEnum16         d08nv;      /* PV Status */
    epicsEnum16         d09nv;      /* PV Status */
    epicsEnum16         d10nv;      /* PV Status */
    epicsEnum16         d11nv;      /* PV Status */
    epicsEnum16         d12nv;      /* PV Status */
    epicsEnum16         d13nv;      /* PV Status */
    epicsEnum16         d14nv;      /* PV Status */
    epicsEnum16         d15nv;      /* PV Status */
    epicsEnum16         d16nv;      /* PV Status */
    epicsEnum16         d17nv;      /* PV Status */
    epicsEnum16         d18nv;      /* PV Status */
    epicsEnum16         d19nv;      /* PV Status */
    epicsEnum16         d20nv;      /* PV Status */
    epicsEnum16         d21nv;      /* PV Status */
    epicsEnum16         d22nv;      /* PV Status */
    epicsEnum16         d23nv;      /* PV Status */
    epicsEnum16         d24nv;      /* PV Status */
    epicsEnum16         d25nv;      /* PV Status */
    epicsEnum16         d26nv;      /* PV Status */
    epicsEnum16         d27nv;      /* PV Status */
    epicsEnum16         d28nv;      /* PV Status */
    epicsEnum16         d29nv;      /* PV Status */
    epicsEnum16         d30nv;      /* PV Status */
    epicsEnum16         d31nv;      /* PV Status */
    epicsEnum16         d32nv;      /* PV Status */
    epicsEnum16         d33nv;      /* PV Status */
    epicsEnum16         d34nv;      /* PV Status */
    epicsEnum16         d35nv;      /* PV Status */
    epicsEnum16         d36nv;      /* PV Status */
    epicsEnum16         d37nv;      /* PV Status */
    epicsEnum16         d38nv;      /* PV Status */
    epicsEnum16         d39nv;      /* PV Status */
    epicsEnum16         d40nv;      /* PV Status */
    epicsEnum16         d41nv;      /* PV Status */
    epicsEnum16         d42nv;      /* PV Status */
    epicsEnum16         d43nv;      /* PV Status */
    epicsEnum16         d44nv;      /* PV Status */
    epicsEnum16         d45nv;      /* PV Status */
    epicsEnum16         d46nv;      /* PV Status */
    epicsEnum16         d47nv;      /* PV Status */
    epicsEnum16         d48nv;      /* PV Status */
    epicsEnum16         d49nv;      /* PV Status */
    epicsEnum16         d50nv;      /* PV Status */
    epicsEnum16         d51nv;      /* PV Status */
    epicsEnum16         d52nv;      /* PV Status */
    epicsEnum16         d53nv;      /* PV Status */
    epicsEnum16         d54nv;      /* PV Status */
    epicsEnum16         d55nv;      /* PV Status */
    epicsEnum16         d56nv;      /* PV Status */
    epicsEnum16         d57nv;      /* PV Status */
    epicsEnum16         d58nv;      /* PV Status */
    epicsEnum16         d59nv;      /* PV Status */
    epicsEnum16         d60nv;      /* PV Status */
    epicsEnum16         d61nv;      /* PV Status */
    epicsEnum16         d62nv;      /* PV Status */
    epicsEnum16         d63nv;      /* PV Status */
    epicsEnum16         d64nv;      /* PV Status */
    epicsEnum16         d65nv;      /* PV Status */
    epicsEnum16         d66nv;      /* PV Status */
    epicsEnum16         d67nv;      /* PV Status */
    epicsEnum16         d68nv;      /* PV Status */
    epicsEnum16         d69nv;      /* PV Status */
    epicsEnum16         d70nv;      /* PV Status */
    epicsEnum16         t1nv;       /* T1  PV Status */
    epicsEnum16         t2nv;       /* T2  PV Status */
    epicsEnum16         t3nv;       /* T3  PV Status */
    epicsEnum16         t4nv;       /* T4  PV Status */
    epicsEnum16         a1nv;       /* A1  PV Status */
    epicsEnum16         bsnv;       /* BeforeScan PV Status */
    epicsEnum16         asnv;       /* After Scan PV Status */
    epicsFloat64        p1pp;       /* P1 Previous Position */
    epicsFloat64        p1cv;       /* P1 Current Value */
    epicsFloat64        p1dv;       /* P1 Desired Value */
    epicsFloat64        p1lv;       /* P1 Last Value Posted */
    epicsFloat64        p1sp;       /* P1 Start Position */
    epicsFloat64        p1si;       /* P1 Step Increment */
    epicsFloat64        p1ep;       /* P1 End Position */
    epicsFloat64        p1cp;       /* P1 Center Position */
    epicsFloat64        p1wd;       /* P1 Scan Width */
    epicsFloat64        r1cv;       /* P1 Readback Value */
    epicsFloat64        r1lv;       /* P1 Rdbk Last Val Pst */
    epicsFloat64        r1dl;       /* P1 Readback Delta */
    epicsFloat64        p1hr;       /* P1 High Oper Range */
    epicsFloat64        p1lr;       /* P1 Low  Oper Range */
    double *         p1pa;          /* P1 Step Array */
    double *         p1ra;          /* P1 Readback Array */
    double *         p1ca;          /* P1 Current Readback Array */
    epicsEnum16         p1fs;       /* P1 Freeze Start Pos */
    epicsEnum16         p1fi;       /* P1 Freeze Step Inc */
    epicsEnum16         p1fe;       /* P1 Freeze End Pos */
    epicsEnum16         p1fc;       /* P1 Freeze Center Pos */
    epicsEnum16         p1fw;       /* P1 Freeze Width */
    epicsEnum16         p1sm;       /* P1 Step Mode */
    epicsEnum16         p1ar;       /* P1 Absolute/Relative */
    char                p1eu[16];   /* P1 Engineering Units */
    epicsInt16          p1pr;       /* P1 Display Precision */
    epicsFloat64        p2pp;       /* P2 Previous Position */
    epicsFloat64        p2cv;       /* P2 Current Value */
    epicsFloat64        p2dv;       /* P2 Desired Value */
    epicsFloat64        p2lv;       /* P2 Last Value Posted */
    epicsFloat64        p2sp;       /* P2 Start Position */
    epicsFloat64        p2si;       /* P2 Step Increment */
    epicsFloat64        p2ep;       /* P2 End Position */
    epicsFloat64        p2cp;       /* P2 Center Position */
    epicsFloat64        p2wd;       /* P2 Scan Width */
    epicsFloat64        r2cv;       /* P2 Readback Value */
    epicsFloat64        r2lv;       /* P2 Rdbk Last Val Pst */
    epicsFloat64        r2dl;       /* P2 Readback Delta */
    epicsFloat64        p2hr;       /* P2 High Oper Range */
    epicsFloat64        p2lr;       /* P2 Low  Oper Range */
    double *         p2pa;          /* P2 Step Array */
    double *         p2ra;          /* P2 Readback Array */
    double *         p2ca;          /* P2 Current Readback Array */
    epicsEnum16         p2fs;       /* P2 Freeze Start Pos */
    epicsEnum16         p2fi;       /* P2 Freeze Step Inc */
    epicsEnum16         p2fe;       /* P2 Freeze End Pos */
    epicsEnum16         p2fc;       /* P2 Freeze Center Pos */
    epicsEnum16         p2fw;       /* P2 Freeze Width */
    epicsEnum16         p2sm;       /* P2 Step Mode */
    epicsEnum16         p2ar;       /* P2 Absolute/Relative */
    char                p2eu[16];   /* P2 Engineering Units */
    epicsInt16          p2pr;       /* P2 Display Precision */
    epicsFloat64        p3pp;       /* P3 Previous Position */
    epicsFloat64        p3cv;       /* P3 Current Value */
    epicsFloat64        p3dv;       /* P3 Desired Value */
    epicsFloat64        p3lv;       /* P3 Last Value Posted */
    epicsFloat64        p3sp;       /* P3 Start Position */
    epicsFloat64        p3si;       /* P3 Step Increment */
    epicsFloat64        p3ep;       /* P3 End Position */
    epicsFloat64        p3cp;       /* P3 Center Position */
    epicsFloat64        p3wd;       /* P3 Scan Width */
    epicsFloat64        r3cv;       /* P3 Readback Value */
    epicsFloat64        r3lv;       /* P3 Rdbk Last Val Pst */
    epicsFloat64        r3dl;       /* P3 Readback Delta */
    epicsFloat64        p3hr;       /* P3 High Oper Range */
    epicsFloat64        p3lr;       /* P3 Low  Oper Range */
    double *         p3pa;          /* P3 Step Array */
    double *         p3ra;          /* P3 Readback Array */
    double *         p3ca;          /* P3 Current Readback Array */
    epicsEnum16         p3fs;       /* P3 Freeze Start Pos */
    epicsEnum16         p3fi;       /* P3 Freeze Step Inc */
    epicsEnum16         p3fe;       /* P3 Freeze End Pos */
    epicsEnum16         p3fc;       /* P3 Freeze Center Pos */
    epicsEnum16         p3fw;       /* P3 Freeze Width */
    epicsEnum16         p3sm;       /* P3 Step Mode */
    epicsEnum16         p3ar;       /* P3 Absolute/Relative */
    char                p3eu[16];   /* P3 Engineering Units */
    epicsInt16          p3pr;       /* P3 Display Precision */
    epicsFloat64        p4pp;       /* P4 Previous Position */
    epicsFloat64        p4cv;       /* P4 Current Value */
    epicsFloat64        p4dv;       /* P4 Desired Value */
    epicsFloat64        p4lv;       /* P4 Last Value Posted */
    epicsFloat64        p4sp;       /* P4 Start Position */
    epicsFloat64        p4si;       /* P4 Step Increment */
    epicsFloat64        p4ep;       /* P4 End Position */
    epicsFloat64        p4cp;       /* P4 Center Position */
    epicsFloat64        p4wd;       /* P4 Scan Width */
    epicsFloat64        r4cv;       /* P4 Readback Value */
    epicsFloat64        r4lv;       /* P4 Rdbk Last Val Pst */
    epicsFloat64        r4dl;       /* P4 Readback Delta */
    epicsFloat64        p4hr;       /* P4 High Oper Range */
    epicsFloat64        p4lr;       /* P4 Low  Oper Range */
    double *         p4pa;          /* P4 Step Array */
    double *         p4ra;          /* P4 Readback Array */
    double *         p4ca;          /* P4 Current Readback Array */
    epicsEnum16         p4fs;       /* P4 Freeze Start Pos */
    epicsEnum16         p4fi;       /* P4 Freeze Step Inc */
    epicsEnum16         p4fe;       /* P4 Freeze End Pos */
    epicsEnum16         p4fc;       /* P4 Freeze Center Pos */
    epicsEnum16         p4fw;       /* P4 Freeze Width */
    epicsEnum16         p4sm;       /* P4 Step Mode */
    epicsEnum16         p4ar;       /* P4 Absolute/Relative */
    char                p4eu[16];   /* P4 Engineering Units */
    epicsInt16          p4pr;       /* P4 Display Precision */
    epicsFloat64        d01hr;      /* High Oper Range */
    epicsFloat64        d01lr;      /* Low  Oper Range */
    float *          d01da;         /* Data Array */
    float *          d01ca;         /* Current Data Array */
    epicsFloat32        d01cv;      /* Current Value */
    epicsFloat32        d01lv;      /* Last Value Posted */
    epicsUInt32         d01ne;      /* # of Elements/Pt */
    char                d01eu[16];  /* Engineering Units */
    epicsInt16          d01pr;      /* Display Precision */
    epicsFloat64        d02hr;      /* High Oper Range */
    epicsFloat64        d02lr;      /* Low  Oper Range */
    float *          d02da;         /* Data Array */
    float *          d02ca;         /* Current Data Array */
    epicsFloat32        d02cv;      /* Current Value */
    epicsFloat32        d02lv;      /* Last Value Posted */
    epicsUInt32         d02ne;      /* # of Elements/Pt */
    char                d02eu[16];  /* Engineering Units */
    epicsInt16          d02pr;      /* Display Precision */
    epicsFloat64        d03hr;      /* High Oper Range */
    epicsFloat64        d03lr;      /* Low  Oper Range */
    float *          d03da;         /* Data Array */
    float *          d03ca;         /* Current Data Array */
    epicsFloat32        d03cv;      /* Current Value */
    epicsFloat32        d03lv;      /* Last Value Posted */
    epicsUInt32         d03ne;      /* # of Elements/Pt */
    char                d03eu[16];  /* Engineering Units */
    epicsInt16          d03pr;      /* Display Precision */
    epicsFloat64        d04hr;      /* High Oper Range */
    epicsFloat64        d04lr;      /* Low  Oper Range */
    float *          d04da;         /* Data Array */
    float *          d04ca;         /* Current Data Array */
    epicsFloat32        d04cv;      /* Current Value */
    epicsFloat32        d04lv;      /* Last Value Posted */
    epicsUInt32         d04ne;      /* # of Elements/Pt */
    char                d04eu[16];  /* Engineering Units */
    epicsInt16          d04pr;      /* Display Precision */
    epicsFloat64        d05hr;      /* High Oper Range */
    epicsFloat64        d05lr;      /* Low  Oper Range */
    float *          d05da;         /* Data Array */
    float *          d05ca;         /* Current Data Array */
    epicsFloat32        d05cv;      /* Current Value */
    epicsFloat32        d05lv;      /* Last Value Posted */
    epicsUInt32         d05ne;      /* # of Elements/Pt */
    char                d05eu[16];  /* Engineering Units */
    epicsInt16          d05pr;      /* Display Precision */
    epicsFloat64        d06hr;      /* High Oper Range */
    epicsFloat64        d06lr;      /* Low  Oper Range */
    float *          d06da;         /* Data Array */
    float *          d06ca;         /* Current Data Array */
    epicsFloat32        d06cv;      /* Current Value */
    epicsFloat32        d06lv;      /* Last Value Posted */
    epicsUInt32         d06ne;      /* # of Elements/Pt */
    char                d06eu[16];  /* Engineering Units */
    epicsInt16          d06pr;      /* Display Precision */
    epicsFloat64        d07hr;      /* High Oper Range */
    epicsFloat64        d07lr;      /* Low  Oper Range */
    float *          d07da;         /* Data Array */
    float *          d07ca;         /* Current Data Array */
    epicsFloat32        d07cv;      /* Current Value */
    epicsFloat32        d07lv;      /* Last Value Posted */
    epicsUInt32         d07ne;      /* # of Elements/Pt */
    char                d07eu[16];  /* Engineering Units */
    epicsInt16          d07pr;      /* Display Precision */
    epicsFloat64        d08hr;      /* High Oper Range */
    epicsFloat64        d08lr;      /* Low  Oper Range */
    float *          d08da;         /* Data Array */
    float *          d08ca;         /* Current Data Array */
    epicsFloat32        d08cv;      /* Current Value */
    epicsFloat32        d08lv;      /* Last Value Posted */
    epicsUInt32         d08ne;      /* # of Elements/Pt */
    char                d08eu[16];  /* Engineering Units */
    epicsInt16          d08pr;      /* Display Precision */
    epicsFloat64        d09hr;      /* High Oper Range */
    epicsFloat64        d09lr;      /* Low  Oper Range */
    float *          d09da;         /* Data Array */
    float *          d09ca;         /* Current Data Array */
    epicsFloat32        d09cv;      /* Current Value */
    epicsFloat32        d09lv;      /* Last Value Posted */
    epicsUInt32         d09ne;      /* # of Elements/Pt */
    char                d09eu[16];  /* Engineering Units */
    epicsInt16          d09pr;      /* Display Precision */
    epicsFloat64        d10hr;      /* High Oper Range */
    epicsFloat64        d10lr;      /* Low  Oper Range */
    float *          d10da;         /* Data Array */
    float *          d10ca;         /* Current Data Array */
    epicsFloat32        d10cv;      /* Current Value */
    epicsFloat32        d10lv;      /* Last Value Posted */
    epicsUInt32         d10ne;      /* # of Elements/Pt */
    char                d10eu[16];  /* Engineering Units */
    epicsInt16          d10pr;      /* Display Precision */
    epicsFloat64        d11hr;      /* High Oper Range */
    epicsFloat64        d11lr;      /* Low  Oper Range */
    float *          d11da;         /* Data Array */
    float *          d11ca;         /* Current Data Array */
    epicsFloat32        d11cv;      /* Current Value */
    epicsFloat32        d11lv;      /* Last Value Posted */
    epicsUInt32         d11ne;      /* # of Elements/Pt */
    char                d11eu[16];  /* Engineering Units */
    epicsInt16          d11pr;      /* Display Precision */
    epicsFloat64        d12hr;      /* High Oper Range */
    epicsFloat64        d12lr;      /* Low  Oper Range */
    float *          d12da;         /* Data Array */
    float *          d12ca;         /* Current Data Array */
    epicsFloat32        d12cv;      /* Current Value */
    epicsFloat32        d12lv;      /* Last Value Posted */
    epicsUInt32         d12ne;      /* # of Elements/Pt */
    char                d12eu[16];  /* Engineering Units */
    epicsInt16          d12pr;      /* Display Precision */
    epicsFloat64        d13hr;      /* High Oper Range */
    epicsFloat64        d13lr;      /* Low  Oper Range */
    float *          d13da;         /* Data Array */
    float *          d13ca;         /* Current Data Array */
    epicsFloat32        d13cv;      /* Current Value */
    epicsFloat32        d13lv;      /* Last Value Posted */
    epicsUInt32         d13ne;      /* # of Elements/Pt */
    char                d13eu[16];  /* Engineering Units */
    epicsInt16          d13pr;      /* Display Precision */
    epicsFloat64        d14hr;      /* High Oper Range */
    epicsFloat64        d14lr;      /* Low  Oper Range */
    float *          d14da;         /* Data Array */
    float *          d14ca;         /* Current Data Array */
    epicsFloat32        d14cv;      /* Current Value */
    epicsFloat32        d14lv;      /* Last Value Posted */
    epicsUInt32         d14ne;      /* # of Elements/Pt */
    char                d14eu[16];  /* Engineering Units */
    epicsInt16          d14pr;      /* Display Precision */
    epicsFloat64        d15hr;      /* High Oper Range */
    epicsFloat64        d15lr;      /* Low  Oper Range */
    float *          d15da;         /* Data Array */
    float *          d15ca;         /* Current Data Array */
    epicsFloat32        d15cv;      /* Current Value */
    epicsFloat32        d15lv;      /* Last Value Posted */
    epicsUInt32         d15ne;      /* # of Elements/Pt */
    char                d15eu[16];  /* Engineering Units */
    epicsInt16          d15pr;      /* Display Precision */
    epicsFloat64        d16hr;      /* High Oper Range */
    epicsFloat64        d16lr;      /* Low  Oper Range */
    float *          d16da;         /* Data Array */
    float *          d16ca;         /* Current Data Array */
    epicsFloat32        d16cv;      /* Current Value */
    epicsFloat32        d16lv;      /* Last Value Posted */
    epicsUInt32         d16ne;      /* # of Elements/Pt */
    char                d16eu[16];  /* Engineering Units */
    epicsInt16          d16pr;      /* Display Precision */
    epicsFloat64        d17hr;      /* High Oper Range */
    epicsFloat64        d17lr;      /* Low  Oper Range */
    float *          d17da;         /* Data Array */
    float *          d17ca;         /* Current Data Array */
    epicsFloat32        d17cv;      /* Current Value */
    epicsFloat32        d17lv;      /* Last Value Posted */
    epicsUInt32         d17ne;      /* # of Elements/Pt */
    char                d17eu[16];  /* Engineering Units */
    epicsInt16          d17pr;      /* Display Precision */
    epicsFloat64        d18hr;      /* High Oper Range */
    epicsFloat64        d18lr;      /* Low  Oper Range */
    float *          d18da;         /* Data Array */
    float *          d18ca;         /* Current Data Array */
    epicsFloat32        d18cv;      /* Current Value */
    epicsFloat32        d18lv;      /* Last Value Posted */
    epicsUInt32         d18ne;      /* # of Elements/Pt */
    char                d18eu[16];  /* Engineering Units */
    epicsInt16          d18pr;      /* Display Precision */
    epicsFloat64        d19hr;      /* High Oper Range */
    epicsFloat64        d19lr;      /* Low  Oper Range */
    float *          d19da;         /* Data Array */
    float *          d19ca;         /* Current Data Array */
    epicsFloat32        d19cv;      /* Current Value */
    epicsFloat32        d19lv;      /* Last Value Posted */
    epicsUInt32         d19ne;      /* # of Elements/Pt */
    char                d19eu[16];  /* Engineering Units */
    epicsInt16          d19pr;      /* Display Precision */
    epicsFloat64        d20hr;      /* High Oper Range */
    epicsFloat64        d20lr;      /* Low  Oper Range */
    float *          d20da;         /* Data Array */
    float *          d20ca;         /* Current Data Array */
    epicsFloat32        d20cv;      /* Current Value */
    epicsFloat32        d20lv;      /* Last Value Posted */
    epicsUInt32         d20ne;      /* # of Elements/Pt */
    char                d20eu[16];  /* Engineering Units */
    epicsInt16          d20pr;      /* Display Precision */
    epicsFloat64        d21hr;      /* High Oper Range */
    epicsFloat64        d21lr;      /* Low  Oper Range */
    float *          d21da;         /* Data Array */
    float *          d21ca;         /* Current Data Array */
    epicsFloat32        d21cv;      /* Current Value */
    epicsFloat32        d21lv;      /* Last Value Posted */
    epicsUInt32         d21ne;      /* # of Elements/Pt */
    char                d21eu[16];  /* Engineering Units */
    epicsInt16          d21pr;      /* Display Precision */
    epicsFloat64        d22hr;      /* High Oper Range */
    epicsFloat64        d22lr;      /* Low  Oper Range */
    float *          d22da;         /* Data Array */
    float *          d22ca;         /* Current Data Array */
    epicsFloat32        d22cv;      /* Current Value */
    epicsFloat32        d22lv;      /* Last Value Posted */
    epicsUInt32         d22ne;      /* # of Elements/Pt */
    char                d22eu[16];  /* Engineering Units */
    epicsInt16          d22pr;      /* Display Precision */
    epicsFloat64        d23hr;      /* High Oper Range */
    epicsFloat64        d23lr;      /* Low  Oper Range */
    float *          d23da;         /* Data Array */
    float *          d23ca;         /* Current Data Array */
    epicsFloat32        d23cv;      /* Current Value */
    epicsFloat32        d23lv;      /* Last Value Posted */
    epicsUInt32         d23ne;      /* # of Elements/Pt */
    char                d23eu[16];  /* Engineering Units */
    epicsInt16          d23pr;      /* Display Precision */
    epicsFloat64        d24hr;      /* High Oper Range */
    epicsFloat64        d24lr;      /* Low  Oper Range */
    float *          d24da;         /* Data Array */
    float *          d24ca;         /* Current Data Array */
    epicsFloat32        d24cv;      /* Current Value */
    epicsFloat32        d24lv;      /* Last Value Posted */
    epicsUInt32         d24ne;      /* # of Elements/Pt */
    char                d24eu[16];  /* Engineering Units */
    epicsInt16          d24pr;      /* Display Precision */
    epicsFloat64        d25hr;      /* High Oper Range */
    epicsFloat64        d25lr;      /* Low  Oper Range */
    float *          d25da;         /* Data Array */
    float *          d25ca;         /* Current Data Array */
    epicsFloat32        d25cv;      /* Current Value */
    epicsFloat32        d25lv;      /* Last Value Posted */
    epicsUInt32         d25ne;      /* # of Elements/Pt */
    char                d25eu[16];  /* Engineering Units */
    epicsInt16          d25pr;      /* Display Precision */
    epicsFloat64        d26hr;      /* High Oper Range */
    epicsFloat64        d26lr;      /* Low  Oper Range */
    float *          d26da;         /* Data Array */
    float *          d26ca;         /* Current Data Array */
    epicsFloat32        d26cv;      /* Current Value */
    epicsFloat32        d26lv;      /* Last Value Posted */
    epicsUInt32         d26ne;      /* # of Elements/Pt */
    char                d26eu[16];  /* Engineering Units */
    epicsInt16          d26pr;      /* Display Precision */
    epicsFloat64        d27hr;      /* High Oper Range */
    epicsFloat64        d27lr;      /* Low  Oper Range */
    float *          d27da;         /* Data Array */
    float *          d27ca;         /* Current Data Array */
    epicsFloat32        d27cv;      /* Current Value */
    epicsFloat32        d27lv;      /* Last Value Posted */
    epicsUInt32         d27ne;      /* # of Elements/Pt */
    char                d27eu[16];  /* Engineering Units */
    epicsInt16          d27pr;      /* Display Precision */
    epicsFloat64        d28hr;      /* High Oper Range */
    epicsFloat64        d28lr;      /* Low  Oper Range */
    float *          d28da;         /* Data Array */
    float *          d28ca;         /* Current Data Array */
    epicsFloat32        d28cv;      /* Current Value */
    epicsFloat32        d28lv;      /* Last Value Posted */
    epicsUInt32         d28ne;      /* # of Elements/Pt */
    char                d28eu[16];  /* Engineering Units */
    epicsInt16          d28pr;      /* Display Precision */
    epicsFloat64        d29hr;      /* High Oper Range */
    epicsFloat64        d29lr;      /* Low  Oper Range */
    float *          d29da;         /* Data Array */
    float *          d29ca;         /* Current Data Array */
    epicsFloat32        d29cv;      /* Current Value */
    epicsFloat32        d29lv;      /* Last Value Posted */
    epicsUInt32         d29ne;      /* # of Elements/Pt */
    char                d29eu[16];  /* Engineering Units */
    epicsInt16          d29pr;      /* Display Precision */
    epicsFloat64        d30hr;      /* High Oper Range */
    epicsFloat64        d30lr;      /* Low  Oper Range */
    float *          d30da;         /* Data Array */
    float *          d30ca;         /* Current Data Array */
    epicsFloat32        d30cv;      /* Current Value */
    epicsFloat32        d30lv;      /* Last Value Posted */
    epicsUInt32         d30ne;      /* # of Elements/Pt */
    char                d30eu[16];  /* Engineering Units */
    epicsInt16          d30pr;      /* Display Precision */
    epicsFloat64        d31hr;      /* High Oper Range */
    epicsFloat64        d31lr;      /* Low  Oper Range */
    float *          d31da;         /* Data Array */
    float *          d31ca;         /* Current Data Array */
    epicsFloat32        d31cv;      /* Current Value */
    epicsFloat32        d31lv;      /* Last Value Posted */
    epicsUInt32         d31ne;      /* # of Elements/Pt */
    char                d31eu[16];  /* Engineering Units */
    epicsInt16          d31pr;      /* Display Precision */
    epicsFloat64        d32hr;      /* High Oper Range */
    epicsFloat64        d32lr;      /* Low  Oper Range */
    float *          d32da;         /* Data Array */
    float *          d32ca;         /* Current Data Array */
    epicsFloat32        d32cv;      /* Current Value */
    epicsFloat32        d32lv;      /* Last Value Posted */
    epicsUInt32         d32ne;      /* # of Elements/Pt */
    char                d32eu[16];  /* Engineering Units */
    epicsInt16          d32pr;      /* Display Precision */
    epicsFloat64        d33hr;      /* High Oper Range */
    epicsFloat64        d33lr;      /* Low  Oper Range */
    float *          d33da;         /* Data Array */
    float *          d33ca;         /* Current Data Array */
    epicsFloat32        d33cv;      /* Current Value */
    epicsFloat32        d33lv;      /* Last Value Posted */
    epicsUInt32         d33ne;      /* # of Elements/Pt */
    char                d33eu[16];  /* Engineering Units */
    epicsInt16          d33pr;      /* Display Precision */
    epicsFloat64        d34hr;      /* High Oper Range */
    epicsFloat64        d34lr;      /* Low  Oper Range */
    float *          d34da;         /* Data Array */
    float *          d34ca;         /* Current Data Array */
    epicsFloat32        d34cv;      /* Current Value */
    epicsFloat32        d34lv;      /* Last Value Posted */
    epicsUInt32         d34ne;      /* # of Elements/Pt */
    char                d34eu[16];  /* Engineering Units */
    epicsInt16          d34pr;      /* Display Precision */
    epicsFloat64        d35hr;      /* High Oper Range */
    epicsFloat64        d35lr;      /* Low  Oper Range */
    float *          d35da;         /* Data Array */
    float *          d35ca;         /* Current Data Array */
    epicsFloat32        d35cv;      /* Current Value */
    epicsFloat32        d35lv;      /* Last Value Posted */
    epicsUInt32         d35ne;      /* # of Elements/Pt */
    char                d35eu[16];  /* Engineering Units */
    epicsInt16          d35pr;      /* Display Precision */
    epicsFloat64        d36hr;      /* High Oper Range */
    epicsFloat64        d36lr;      /* Low  Oper Range */
    float *          d36da;         /* Data Array */
    float *          d36ca;         /* Current Data Array */
    epicsFloat32        d36cv;      /* Current Value */
    epicsFloat32        d36lv;      /* Last Value Posted */
    epicsUInt32         d36ne;      /* # of Elements/Pt */
    char                d36eu[16];  /* Engineering Units */
    epicsInt16          d36pr;      /* Display Precision */
    epicsFloat64        d37hr;      /* High Oper Range */
    epicsFloat64        d37lr;      /* Low  Oper Range */
    float *          d37da;         /* Data Array */
    float *          d37ca;         /* Current Data Array */
    epicsFloat32        d37cv;      /* Current Value */
    epicsFloat32        d37lv;      /* Last Value Posted */
    epicsUInt32         d37ne;      /* # of Elements/Pt */
    char                d37eu[16];  /* Engineering Units */
    epicsInt16          d37pr;      /* Display Precision */
    epicsFloat64        d38hr;      /* High Oper Range */
    epicsFloat64        d38lr;      /* Low  Oper Range */
    float *          d38da;         /* Data Array */
    float *          d38ca;         /* Current Data Array */
    epicsFloat32        d38cv;      /* Current Value */
    epicsFloat32        d38lv;      /* Last Value Posted */
    epicsUInt32         d38ne;      /* # of Elements/Pt */
    char                d38eu[16];  /* Engineering Units */
    epicsInt16          d38pr;      /* Display Precision */
    epicsFloat64        d39hr;      /* High Oper Range */
    epicsFloat64        d39lr;      /* Low  Oper Range */
    float *          d39da;         /* Data Array */
    float *          d39ca;         /* Current Data Array */
    epicsFloat32        d39cv;      /* Current Value */
    epicsFloat32        d39lv;      /* Last Value Posted */
    epicsUInt32         d39ne;      /* # of Elements/Pt */
    char                d39eu[16];  /* Engineering Units */
    epicsInt16          d39pr;      /* Display Precision */
    epicsFloat64        d40hr;      /* High Oper Range */
    epicsFloat64        d40lr;      /* Low  Oper Range */
    float *          d40da;         /* Data Array */
    float *          d40ca;         /* Current Data Array */
    epicsFloat32        d40cv;      /* Current Value */
    epicsFloat32        d40lv;      /* Last Value Posted */
    epicsUInt32         d40ne;      /* # of Elements/Pt */
    char                d40eu[16];  /* Engineering Units */
    epicsInt16          d40pr;      /* Display Precision */
    epicsFloat64        d41hr;      /* High Oper Range */
    epicsFloat64        d41lr;      /* Low  Oper Range */
    float *          d41da;         /* Data Array */
    float *          d41ca;         /* Current Data Array */
    epicsFloat32        d41cv;      /* Current Value */
    epicsFloat32        d41lv;      /* Last Value Posted */
    epicsUInt32         d41ne;      /* # of Elements/Pt */
    char                d41eu[16];  /* Engineering Units */
    epicsInt16          d41pr;      /* Display Precision */
    epicsFloat64        d42hr;      /* High Oper Range */
    epicsFloat64        d42lr;      /* Low  Oper Range */
    float *          d42da;         /* Data Array */
    float *          d42ca;         /* Current Data Array */
    epicsFloat32        d42cv;      /* Current Value */
    epicsFloat32        d42lv;      /* Last Value Posted */
    epicsUInt32         d42ne;      /* # of Elements/Pt */
    char                d42eu[16];  /* Engineering Units */
    epicsInt16          d42pr;      /* Display Precision */
    epicsFloat64        d43hr;      /* High Oper Range */
    epicsFloat64        d43lr;      /* Low  Oper Range */
    float *          d43da;         /* Data Array */
    float *          d43ca;         /* Current Data Array */
    epicsFloat32        d43cv;      /* Current Value */
    epicsFloat32        d43lv;      /* Last Value Posted */
    epicsUInt32         d43ne;      /* # of Elements/Pt */
    char                d43eu[16];  /* Engineering Units */
    epicsInt16          d43pr;      /* Display Precision */
    epicsFloat64        d44hr;      /* High Oper Range */
    epicsFloat64        d44lr;      /* Low  Oper Range */
    float *          d44da;         /* Data Array */
    float *          d44ca;         /* Current Data Array */
    epicsFloat32        d44cv;      /* Current Value */
    epicsFloat32        d44lv;      /* Last Value Posted */
    epicsUInt32         d44ne;      /* # of Elements/Pt */
    char                d44eu[16];  /* Engineering Units */
    epicsInt16          d44pr;      /* Display Precision */
    epicsFloat64        d45hr;      /* High Oper Range */
    epicsFloat64        d45lr;      /* Low  Oper Range */
    float *          d45da;         /* Data Array */
    float *          d45ca;         /* Current Data Array */
    epicsFloat32        d45cv;      /* Current Value */
    epicsFloat32        d45lv;      /* Last Value Posted */
    epicsUInt32         d45ne;      /* # of Elements/Pt */
    char                d45eu[16];  /* Engineering Units */
    epicsInt16          d45pr;      /* Display Precision */
    epicsFloat64        d46hr;      /* High Oper Range */
    epicsFloat64        d46lr;      /* Low  Oper Range */
    float *          d46da;         /* Data Array */
    float *          d46ca;         /* Current Data Array */
    epicsFloat32        d46cv;      /* Current Value */
    epicsFloat32        d46lv;      /* Last Value Posted */
    epicsUInt32         d46ne;      /* # of Elements/Pt */
    char                d46eu[16];  /* Engineering Units */
    epicsInt16          d46pr;      /* Display Precision */
    epicsFloat64        d47hr;      /* High Oper Range */
    epicsFloat64        d47lr;      /* Low  Oper Range */
    float *          d47da;         /* Data Array */
    float *          d47ca;         /* Current Data Array */
    epicsFloat32        d47cv;      /* Current Value */
    epicsFloat32        d47lv;      /* Last Value Posted */
    epicsUInt32         d47ne;      /* # of Elements/Pt */
    char                d47eu[16];  /* Engineering Units */
    epicsInt16          d47pr;      /* Display Precision */
    epicsFloat64        d48hr;      /* High Oper Range */
    epicsFloat64        d48lr;      /* Low  Oper Range */
    float *          d48da;         /* Data Array */
    float *          d48ca;         /* Current Data Array */
    epicsFloat32        d48cv;      /* Current Value */
    epicsFloat32        d48lv;      /* Last Value Posted */
    epicsUInt32         d48ne;      /* # of Elements/Pt */
    char                d48eu[16];  /* Engineering Units */
    epicsInt16          d48pr;      /* Display Precision */
    epicsFloat64        d49hr;      /* High Oper Range */
    epicsFloat64        d49lr;      /* Low  Oper Range */
    float *          d49da;         /* Data Array */
    float *          d49ca;         /* Current Data Array */
    epicsFloat32        d49cv;      /* Current Value */
    epicsFloat32        d49lv;      /* Last Value Posted */
    epicsUInt32         d49ne;      /* # of Elements/Pt */
    char                d49eu[16];  /* Engineering Units */
    epicsInt16          d49pr;      /* Display Precision */
    epicsFloat64        d50hr;      /* High Oper Range */
    epicsFloat64        d50lr;      /* Low  Oper Range */
    float *          d50da;         /* Data Array */
    float *          d50ca;         /* Current Data Array */
    epicsFloat32        d50cv;      /* Current Value */
    epicsFloat32        d50lv;      /* Last Value Posted */
    epicsUInt32         d50ne;      /* # of Elements/Pt */
    char                d50eu[16];  /* Engineering Units */
    epicsInt16          d50pr;      /* Display Precision */
    epicsFloat64        d51hr;      /* High Oper Range */
    epicsFloat64        d51lr;      /* Low  Oper Range */
    float *          d51da;         /* Data Array */
    float *          d51ca;         /* Current Data Array */
    epicsFloat32        d51cv;      /* Current Value */
    epicsFloat32        d51lv;      /* Last Value Posted */
    epicsUInt32         d51ne;      /* # of Elements/Pt */
    char                d51eu[16];  /* Engineering Units */
    epicsInt16          d51pr;      /* Display Precision */
    epicsFloat64        d52hr;      /* High Oper Range */
    epicsFloat64        d52lr;      /* Low  Oper Range */
    float *          d52da;         /* Data Array */
    float *          d52ca;         /* Current Data Array */
    epicsFloat32        d52cv;      /* Current Value */
    epicsFloat32        d52lv;      /* Last Value Posted */
    epicsUInt32         d52ne;      /* # of Elements/Pt */
    char                d52eu[16];  /* Engineering Units */
    epicsInt16          d52pr;      /* Display Precision */
    epicsFloat64        d53hr;      /* High Oper Range */
    epicsFloat64        d53lr;      /* Low  Oper Range */
    float *          d53da;         /* Data Array */
    float *          d53ca;         /* Current Data Array */
    epicsFloat32        d53cv;      /* Current Value */
    epicsFloat32        d53lv;      /* Last Value Posted */
    epicsUInt32         d53ne;      /* # of Elements/Pt */
    char                d53eu[16];  /* Engineering Units */
    epicsInt16          d53pr;      /* Display Precision */
    epicsFloat64        d54hr;      /* High Oper Range */
    epicsFloat64        d54lr;      /* Low  Oper Range */
    float *          d54da;         /* Data Array */
    float *          d54ca;         /* Current Data Array */
    epicsFloat32        d54cv;      /* Current Value */
    epicsFloat32        d54lv;      /* Last Value Posted */
    epicsUInt32         d54ne;      /* # of Elements/Pt */
    char                d54eu[16];  /* Engineering Units */
    epicsInt16          d54pr;      /* Display Precision */
    epicsFloat64        d55hr;      /* High Oper Range */
    epicsFloat64        d55lr;      /* Low  Oper Range */
    float *          d55da;         /* Data Array */
    float *          d55ca;         /* Current Data Array */
    epicsFloat32        d55cv;      /* Current Value */
    epicsFloat32        d55lv;      /* Last Value Posted */
    epicsUInt32         d55ne;      /* # of Elements/Pt */
    char                d55eu[16];  /* Engineering Units */
    epicsInt16          d55pr;      /* Display Precision */
    epicsFloat64        d56hr;      /* High Oper Range */
    epicsFloat64        d56lr;      /* Low  Oper Range */
    float *          d56da;         /* Data Array */
    float *          d56ca;         /* Current Data Array */
    epicsFloat32        d56cv;      /* Current Value */
    epicsFloat32        d56lv;      /* Last Value Posted */
    epicsUInt32         d56ne;      /* # of Elements/Pt */
    char                d56eu[16];  /* Engineering Units */
    epicsInt16          d56pr;      /* Display Precision */
    epicsFloat64        d57hr;      /* High Oper Range */
    epicsFloat64        d57lr;      /* Low  Oper Range */
    float *          d57da;         /* Data Array */
    float *          d57ca;         /* Current Data Array */
    epicsFloat32        d57cv;      /* Current Value */
    epicsFloat32        d57lv;      /* Last Value Posted */
    epicsUInt32         d57ne;      /* # of Elements/Pt */
    char                d57eu[16];  /* Engineering Units */
    epicsInt16          d57pr;      /* Display Precision */
    epicsFloat64        d58hr;      /* High Oper Range */
    epicsFloat64        d58lr;      /* Low  Oper Range */
    float *          d58da;         /* Data Array */
    float *          d58ca;         /* Current Data Array */
    epicsFloat32        d58cv;      /* Current Value */
    epicsFloat32        d58lv;      /* Last Value Posted */
    epicsUInt32         d58ne;      /* # of Elements/Pt */
    char                d58eu[16];  /* Engineering Units */
    epicsInt16          d58pr;      /* Display Precision */
    epicsFloat64        d59hr;      /* High Oper Range */
    epicsFloat64        d59lr;      /* Low  Oper Range */
    float *          d59da;         /* Data Array */
    float *          d59ca;         /* Current Data Array */
    epicsFloat32        d59cv;      /* Current Value */
    epicsFloat32        d59lv;      /* Last Value Posted */
    epicsUInt32         d59ne;      /* # of Elements/Pt */
    char                d59eu[16];  /* Engineering Units */
    epicsInt16          d59pr;      /* Display Precision */
    epicsFloat64        d60hr;      /* High Oper Range */
    epicsFloat64        d60lr;      /* Low  Oper Range */
    float *          d60da;         /* Data Array */
    float *          d60ca;         /* Current Data Array */
    epicsFloat32        d60cv;      /* Current Value */
    epicsFloat32        d60lv;      /* Last Value Posted */
    epicsUInt32         d60ne;      /* # of Elements/Pt */
    char                d60eu[16];  /* Engineering Units */
    epicsInt16          d60pr;      /* Display Precision */
    epicsFloat64        d61hr;      /* High Oper Range */
    epicsFloat64        d61lr;      /* Low  Oper Range */
    float *          d61da;         /* Data Array */
    float *          d61ca;         /* Current Data Array */
    epicsFloat32        d61cv;      /* Current Value */
    epicsFloat32        d61lv;      /* Last Value Posted */
    epicsUInt32         d61ne;      /* # of Elements/Pt */
    char                d61eu[16];  /* Engineering Units */
    epicsInt16          d61pr;      /* Display Precision */
    epicsFloat64        d62hr;      /* High Oper Range */
    epicsFloat64        d62lr;      /* Low  Oper Range */
    float *          d62da;         /* Data Array */
    float *          d62ca;         /* Current Data Array */
    epicsFloat32        d62cv;      /* Current Value */
    epicsFloat32        d62lv;      /* Last Value Posted */
    epicsUInt32         d62ne;      /* # of Elements/Pt */
    char                d62eu[16];  /* Engineering Units */
    epicsInt16          d62pr;      /* Display Precision */
    epicsFloat64        d63hr;      /* High Oper Range */
    epicsFloat64        d63lr;      /* Low  Oper Range */
    float *          d63da;         /* Data Array */
    float *          d63ca;         /* Current Data Array */
    epicsFloat32        d63cv;      /* Current Value */
    epicsFloat32        d63lv;      /* Last Value Posted */
    epicsUInt32         d63ne;      /* # of Elements/Pt */
    char                d63eu[16];  /* Engineering Units */
    epicsInt16          d63pr;      /* Display Precision */
    epicsFloat64        d64hr;      /* High Oper Range */
    epicsFloat64        d64lr;      /* Low  Oper Range */
    float *          d64da;         /* Data Array */
    float *          d64ca;         /* Current Data Array */
    epicsFloat32        d64cv;      /* Current Value */
    epicsFloat32        d64lv;      /* Last Value Posted */
    epicsUInt32         d64ne;      /* # of Elements/Pt */
    char                d64eu[16];  /* Engineering Units */
    epicsInt16          d64pr;      /* Display Precision */
    epicsFloat64        d65hr;      /* High Oper Range */
    epicsFloat64        d65lr;      /* Low  Oper Range */
    float *          d65da;         /* Data Array */
    float *          d65ca;         /* Current Data Array */
    epicsFloat32        d65cv;      /* Current Value */
    epicsFloat32        d65lv;      /* Last Value Posted */
    epicsUInt32         d65ne;      /* # of Elements/Pt */
    char                d65eu[16];  /* Engineering Units */
    epicsInt16          d65pr;      /* Display Precision */
    epicsFloat64        d66hr;      /* High Oper Range */
    epicsFloat64        d66lr;      /* Low  Oper Range */
    float *          d66da;         /* Data Array */
    float *          d66ca;         /* Current Data Array */
    epicsFloat32        d66cv;      /* Current Value */
    epicsFloat32        d66lv;      /* Last Value Posted */
    epicsUInt32         d66ne;      /* # of Elements/Pt */
    char                d66eu[16];  /* Engineering Units */
    epicsInt16          d66pr;      /* Display Precision */
    epicsFloat64        d67hr;      /* High Oper Range */
    epicsFloat64        d67lr;      /* Low  Oper Range */
    float *          d67da;         /* Data Array */
    float *          d67ca;         /* Current Data Array */
    epicsFloat32        d67cv;      /* Current Value */
    epicsFloat32        d67lv;      /* Last Value Posted */
    epicsUInt32         d67ne;      /* # of Elements/Pt */
    char                d67eu[16];  /* Engineering Units */
    epicsInt16          d67pr;      /* Display Precision */
    epicsFloat64        d68hr;      /* High Oper Range */
    epicsFloat64        d68lr;      /* Low  Oper Range */
    float *          d68da;         /* Data Array */
    float *          d68ca;         /* Current Data Array */
    epicsFloat32        d68cv;      /* Current Value */
    epicsFloat32        d68lv;      /* Last Value Posted */
    epicsUInt32         d68ne;      /* # of Elements/Pt */
    char                d68eu[16];  /* Engineering Units */
    epicsInt16          d68pr;      /* Display Precision */
    epicsFloat64        d69hr;      /* High Oper Range */
    epicsFloat64        d69lr;      /* Low  Oper Range */
    float *          d69da;         /* Data Array */
    float *          d69ca;         /* Current Data Array */
    epicsFloat32        d69cv;      /* Current Value */
    epicsFloat32        d69lv;      /* Last Value Posted */
    epicsUInt32         d69ne;      /* # of Elements/Pt */
    char                d69eu[16];  /* Engineering Units */
    epicsInt16          d69pr;      /* Display Precision */
    epicsFloat64        d70hr;      /* High Oper Range */
    epicsFloat64        d70lr;      /* Low  Oper Range */
    float *          d70da;         /* Data Array */
    float *          d70ca;         /* Current Data Array */
    epicsFloat32        d70cv;      /* Current Value */
    epicsFloat32        d70lv;      /* Last Value Posted */
    epicsUInt32         d70ne;      /* # of Elements/Pt */
    char                d70eu[16];  /* Engineering Units */
    epicsInt16          d70pr;      /* Display Precision */
    epicsFloat32        t1cd;       /* T1 Cmnd */
    epicsFloat32        t2cd;       /* T2 Cmnd */
    epicsFloat32        t3cd;       /* T3 Cmnd */
    epicsFloat32        t4cd;       /* T4 Cmnd */
    epicsFloat32        a1cd;       /* A1 Cmnd */
    epicsFloat32        bscd;       /* Before Scan Cmnd */
    epicsFloat32        ascd;       /* After Scan Cmnd */
    epicsEnum16         paus;       /* Go/Pause control */
    epicsEnum16         lpau;       /* Last value of Go/Pause */
    epicsFloat32        pdly;       /* Positioner-settling delay */
    epicsFloat32        ddly;       /* Detector-settling delay */
    epicsFloat32        rdly;       /* Pause resume delay */
    epicsEnum16         faze;       /* Scan phase */
    epicsEnum16         acqm;       /* Acquisition mode */
    epicsEnum16         acqt;       /* Acquisition type */
    epicsEnum16         dstate;     /* Data state */
    epicsInt32          copyto;     /* Copy Last Pt Thru */
} sscanRecord;

typedef enum {
	sscanRecordNAME = 0,
	sscanRecordDESC = 1,
	sscanRecordASG = 2,
	sscanRecordSCAN = 3,
	sscanRecordPINI = 4,
	sscanRecordPHAS = 5,
	sscanRecordEVNT = 6,
	sscanRecordTSE = 7,
	sscanRecordTSEL = 8,
	sscanRecordDTYP = 9,
	sscanRecordDISV = 10,
	sscanRecordDISA = 11,
	sscanRecordSDIS = 12,
	sscanRecordMLOK = 13,
	sscanRecordMLIS = 14,
	sscanRecordDISP = 15,
	sscanRecordPROC = 16,
	sscanRecordSTAT = 17,
	sscanRecordSEVR = 18,
	sscanRecordNSTA = 19,
	sscanRecordNSEV = 20,
	sscanRecordACKS = 21,
	sscanRecordACKT = 22,
	sscanRecordDISS = 23,
	sscanRecordLCNT = 24,
	sscanRecordPACT = 25,
	sscanRecordPUTF = 26,
	sscanRecordRPRO = 27,
	sscanRecordASP = 28,
	sscanRecordPPN = 29,
	sscanRecordPPNR = 30,
	sscanRecordSPVT = 31,
	sscanRecordRSET = 32,
	sscanRecordDSET = 33,
	sscanRecordDPVT = 34,
	sscanRecordRDES = 35,
	sscanRecordLSET = 36,
	sscanRecordPRIO = 37,
	sscanRecordTPRO = 38,
	sscanRecordBKPT = 39,
	sscanRecordUDF = 40,
	sscanRecordUDFS = 41,
	sscanRecordTIME = 42,
	sscanRecordFLNK = 43,
	sscanRecordVERS = 44,
	sscanRecordVAL = 45,
	sscanRecordSMSG = 46,
	sscanRecordCMND = 47,
	sscanRecordALRT = 48,
	sscanRecordRPVT = 49,
	sscanRecordMPTS = 50,
	sscanRecordEXSC = 51,
	sscanRecordXSC = 52,
	sscanRecordPXSC = 53,
	sscanRecordBUSY = 54,
	sscanRecordKILL = 55,
	sscanRecordWAIT = 56,
	sscanRecordWCNT = 57,
	sscanRecordAWCT = 58,
	sscanRecordWTNG = 59,
	sscanRecordAWAIT = 60,
	sscanRecordAAWAIT = 61,
	sscanRecordDATA = 62,
	sscanRecordREFD = 63,
	sscanRecordNPTS = 64,
	sscanRecordFPTS = 65,
	sscanRecordFFO = 66,
	sscanRecordCPT = 67,
	sscanRecordBCPT = 68,
	sscanRecordDPT = 69,
	sscanRecordPCPT = 70,
	sscanRecordPASM = 71,
	sscanRecordTOLP = 72,
	sscanRecordTLAP = 73,
	sscanRecordATIME = 74,
	sscanRecordP1PV = 75,
	sscanRecordP2PV = 76,
	sscanRecordP3PV = 77,
	sscanRecordP4PV = 78,
	sscanRecordR1PV = 79,
	sscanRecordR2PV = 80,
	sscanRecordR3PV = 81,
	sscanRecordR4PV = 82,
	sscanRecordD01PV = 83,
	sscanRecordD02PV = 84,
	sscanRecordD03PV = 85,
	sscanRecordD04PV = 86,
	sscanRecordD05PV = 87,
	sscanRecordD06PV = 88,
	sscanRecordD07PV = 89,
	sscanRecordD08PV = 90,
	sscanRecordD09PV = 91,
	sscanRecordD10PV = 92,
	sscanRecordD11PV = 93,
	sscanRecordD12PV = 94,
	sscanRecordD13PV = 95,
	sscanRecordD14PV = 96,
	sscanRecordD15PV = 97,
	sscanRecordD16PV = 98,
	sscanRecordD17PV = 99,
	sscanRecordD18PV = 100,
	sscanRecordD19PV = 101,
	sscanRecordD20PV = 102,
	sscanRecordD21PV = 103,
	sscanRecordD22PV = 104,
	sscanRecordD23PV = 105,
	sscanRecordD24PV = 106,
	sscanRecordD25PV = 107,
	sscanRecordD26PV = 108,
	sscanRecordD27PV = 109,
	sscanRecordD28PV = 110,
	sscanRecordD29PV = 111,
	sscanRecordD30PV = 112,
	sscanRecordD31PV = 113,
	sscanRecordD32PV = 114,
	sscanRecordD33PV = 115,
	sscanRecordD34PV = 116,
	sscanRecordD35PV = 117,
	sscanRecordD36PV = 118,
	sscanRecordD37PV = 119,
	sscanRecordD38PV = 120,
	sscanRecordD39PV = 121,
	sscanRecordD40PV = 122,
	sscanRecordD41PV = 123,
	sscanRecordD42PV = 124,
	sscanRecordD43PV = 125,
	sscanRecordD44PV = 126,
	sscanRecordD45PV = 127,
	sscanRecordD46PV = 128,
	sscanRecordD47PV = 129,
	sscanRecordD48PV = 130,
	sscanRecordD49PV = 131,
	sscanRecordD50PV = 132,
	sscanRecordD51PV = 133,
	sscanRecordD52PV = 134,
	sscanRecordD53PV = 135,
	sscanRecordD54PV = 136,
	sscanRecordD55PV = 137,
	sscanRecordD56PV = 138,
	sscanRecordD57PV = 139,
	sscanRecordD58PV = 140,
	sscanRecordD59PV = 141,
	sscanRecordD60PV = 142,
	sscanRecordD61PV = 143,
	sscanRecordD62PV = 144,
	sscanRecordD63PV = 145,
	sscanRecordD64PV = 146,
	sscanRecordD65PV = 147,
	sscanRecordD66PV = 148,
	sscanRecordD67PV = 149,
	sscanRecordD68PV = 150,
	sscanRecordD69PV = 151,
	sscanRecordD70PV = 152,
	sscanRecordT1PV = 153,
	sscanRecordT2PV = 154,
	sscanRecordT3PV = 155,
	sscanRecordT4PV = 156,
	sscanRecordA1PV = 157,
	sscanRecordBSPV = 158,
	sscanRecordASPV = 159,
	sscanRecordBSWAIT = 160,
	sscanRecordASWAIT = 161,
	sscanRecordP1NV = 162,
	sscanRecordP2NV = 163,
	sscanRecordP3NV = 164,
	sscanRecordP4NV = 165,
	sscanRecordR1NV = 166,
	sscanRecordR2NV = 167,
	sscanRecordR3NV = 168,
	sscanRecordR4NV = 169,
	sscanRecordD01NV = 170,
	sscanRecordD02NV = 171,
	sscanRecordD03NV = 172,
	sscanRecordD04NV = 173,
	sscanRecordD05NV = 174,
	sscanRecordD06NV = 175,
	sscanRecordD07NV = 176,
	sscanRecordD08NV = 177,
	sscanRecordD09NV = 178,
	sscanRecordD10NV = 179,
	sscanRecordD11NV = 180,
	sscanRecordD12NV = 181,
	sscanRecordD13NV = 182,
	sscanRecordD14NV = 183,
	sscanRecordD15NV = 184,
	sscanRecordD16NV = 185,
	sscanRecordD17NV = 186,
	sscanRecordD18NV = 187,
	sscanRecordD19NV = 188,
	sscanRecordD20NV = 189,
	sscanRecordD21NV = 190,
	sscanRecordD22NV = 191,
	sscanRecordD23NV = 192,
	sscanRecordD24NV = 193,
	sscanRecordD25NV = 194,
	sscanRecordD26NV = 195,
	sscanRecordD27NV = 196,
	sscanRecordD28NV = 197,
	sscanRecordD29NV = 198,
	sscanRecordD30NV = 199,
	sscanRecordD31NV = 200,
	sscanRecordD32NV = 201,
	sscanRecordD33NV = 202,
	sscanRecordD34NV = 203,
	sscanRecordD35NV = 204,
	sscanRecordD36NV = 205,
	sscanRecordD37NV = 206,
	sscanRecordD38NV = 207,
	sscanRecordD39NV = 208,
	sscanRecordD40NV = 209,
	sscanRecordD41NV = 210,
	sscanRecordD42NV = 211,
	sscanRecordD43NV = 212,
	sscanRecordD44NV = 213,
	sscanRecordD45NV = 214,
	sscanRecordD46NV = 215,
	sscanRecordD47NV = 216,
	sscanRecordD48NV = 217,
	sscanRecordD49NV = 218,
	sscanRecordD50NV = 219,
	sscanRecordD51NV = 220,
	sscanRecordD52NV = 221,
	sscanRecordD53NV = 222,
	sscanRecordD54NV = 223,
	sscanRecordD55NV = 224,
	sscanRecordD56NV = 225,
	sscanRecordD57NV = 226,
	sscanRecordD58NV = 227,
	sscanRecordD59NV = 228,
	sscanRecordD60NV = 229,
	sscanRecordD61NV = 230,
	sscanRecordD62NV = 231,
	sscanRecordD63NV = 232,
	sscanRecordD64NV = 233,
	sscanRecordD65NV = 234,
	sscanRecordD66NV = 235,
	sscanRecordD67NV = 236,
	sscanRecordD68NV = 237,
	sscanRecordD69NV = 238,
	sscanRecordD70NV = 239,
	sscanRecordT1NV = 240,
	sscanRecordT2NV = 241,
	sscanRecordT3NV = 242,
	sscanRecordT4NV = 243,
	sscanRecordA1NV = 244,
	sscanRecordBSNV = 245,
	sscanRecordASNV = 246,
	sscanRecordP1PP = 247,
	sscanRecordP1CV = 248,
	sscanRecordP1DV = 249,
	sscanRecordP1LV = 250,
	sscanRecordP1SP = 251,
	sscanRecordP1SI = 252,
	sscanRecordP1EP = 253,
	sscanRecordP1CP = 254,
	sscanRecordP1WD = 255,
	sscanRecordR1CV = 256,
	sscanRecordR1LV = 257,
	sscanRecordR1DL = 258,
	sscanRecordP1HR = 259,
	sscanRecordP1LR = 260,
	sscanRecordP1PA = 261,
	sscanRecordP1RA = 262,
	sscanRecordP1CA = 263,
	sscanRecordP1FS = 264,
	sscanRecordP1FI = 265,
	sscanRecordP1FE = 266,
	sscanRecordP1FC = 267,
	sscanRecordP1FW = 268,
	sscanRecordP1SM = 269,
	sscanRecordP1AR = 270,
	sscanRecordP1EU = 271,
	sscanRecordP1PR = 272,
	sscanRecordP2PP = 273,
	sscanRecordP2CV = 274,
	sscanRecordP2DV = 275,
	sscanRecordP2LV = 276,
	sscanRecordP2SP = 277,
	sscanRecordP2SI = 278,
	sscanRecordP2EP = 279,
	sscanRecordP2CP = 280,
	sscanRecordP2WD = 281,
	sscanRecordR2CV = 282,
	sscanRecordR2LV = 283,
	sscanRecordR2DL = 284,
	sscanRecordP2HR = 285,
	sscanRecordP2LR = 286,
	sscanRecordP2PA = 287,
	sscanRecordP2RA = 288,
	sscanRecordP2CA = 289,
	sscanRecordP2FS = 290,
	sscanRecordP2FI = 291,
	sscanRecordP2FE = 292,
	sscanRecordP2FC = 293,
	sscanRecordP2FW = 294,
	sscanRecordP2SM = 295,
	sscanRecordP2AR = 296,
	sscanRecordP2EU = 297,
	sscanRecordP2PR = 298,
	sscanRecordP3PP = 299,
	sscanRecordP3CV = 300,
	sscanRecordP3DV = 301,
	sscanRecordP3LV = 302,
	sscanRecordP3SP = 303,
	sscanRecordP3SI = 304,
	sscanRecordP3EP = 305,
	sscanRecordP3CP = 306,
	sscanRecordP3WD = 307,
	sscanRecordR3CV = 308,
	sscanRecordR3LV = 309,
	sscanRecordR3DL = 310,
	sscanRecordP3HR = 311,
	sscanRecordP3LR = 312,
	sscanRecordP3PA = 313,
	sscanRecordP3RA = 314,
	sscanRecordP3CA = 315,
	sscanRecordP3FS = 316,
	sscanRecordP3FI = 317,
	sscanRecordP3FE = 318,
	sscanRecordP3FC = 319,
	sscanRecordP3FW = 320,
	sscanRecordP3SM = 321,
	sscanRecordP3AR = 322,
	sscanRecordP3EU = 323,
	sscanRecordP3PR = 324,
	sscanRecordP4PP = 325,
	sscanRecordP4CV = 326,
	sscanRecordP4DV = 327,
	sscanRecordP4LV = 328,
	sscanRecordP4SP = 329,
	sscanRecordP4SI = 330,
	sscanRecordP4EP = 331,
	sscanRecordP4CP = 332,
	sscanRecordP4WD = 333,
	sscanRecordR4CV = 334,
	sscanRecordR4LV = 335,
	sscanRecordR4DL = 336,
	sscanRecordP4HR = 337,
	sscanRecordP4LR = 338,
	sscanRecordP4PA = 339,
	sscanRecordP4RA = 340,
	sscanRecordP4CA = 341,
	sscanRecordP4FS = 342,
	sscanRecordP4FI = 343,
	sscanRecordP4FE = 344,
	sscanRecordP4FC = 345,
	sscanRecordP4FW = 346,
	sscanRecordP4SM = 347,
	sscanRecordP4AR = 348,
	sscanRecordP4EU = 349,
	sscanRecordP4PR = 350,
	sscanRecordD01HR = 351,
	sscanRecordD01LR = 352,
	sscanRecordD01DA = 353,
	sscanRecordD01CA = 354,
	sscanRecordD01CV = 355,
	sscanRecordD01LV = 356,
	sscanRecordD01NE = 357,
	sscanRecordD01EU = 358,
	sscanRecordD01PR = 359,
	sscanRecordD02HR = 360,
	sscanRecordD02LR = 361,
	sscanRecordD02DA = 362,
	sscanRecordD02CA = 363,
	sscanRecordD02CV = 364,
	sscanRecordD02LV = 365,
	sscanRecordD02NE = 366,
	sscanRecordD02EU = 367,
	sscanRecordD02PR = 368,
	sscanRecordD03HR = 369,
	sscanRecordD03LR = 370,
	sscanRecordD03DA = 371,
	sscanRecordD03CA = 372,
	sscanRecordD03CV = 373,
	sscanRecordD03LV = 374,
	sscanRecordD03NE = 375,
	sscanRecordD03EU = 376,
	sscanRecordD03PR = 377,
	sscanRecordD04HR = 378,
	sscanRecordD04LR = 379,
	sscanRecordD04DA = 380,
	sscanRecordD04CA = 381,
	sscanRecordD04CV = 382,
	sscanRecordD04LV = 383,
	sscanRecordD04NE = 384,
	sscanRecordD04EU = 385,
	sscanRecordD04PR = 386,
	sscanRecordD05HR = 387,
	sscanRecordD05LR = 388,
	sscanRecordD05DA = 389,
	sscanRecordD05CA = 390,
	sscanRecordD05CV = 391,
	sscanRecordD05LV = 392,
	sscanRecordD05NE = 393,
	sscanRecordD05EU = 394,
	sscanRecordD05PR = 395,
	sscanRecordD06HR = 396,
	sscanRecordD06LR = 397,
	sscanRecordD06DA = 398,
	sscanRecordD06CA = 399,
	sscanRecordD06CV = 400,
	sscanRecordD06LV = 401,
	sscanRecordD06NE = 402,
	sscanRecordD06EU = 403,
	sscanRecordD06PR = 404,
	sscanRecordD07HR = 405,
	sscanRecordD07LR = 406,
	sscanRecordD07DA = 407,
	sscanRecordD07CA = 408,
	sscanRecordD07CV = 409,
	sscanRecordD07LV = 410,
	sscanRecordD07NE = 411,
	sscanRecordD07EU = 412,
	sscanRecordD07PR = 413,
	sscanRecordD08HR = 414,
	sscanRecordD08LR = 415,
	sscanRecordD08DA = 416,
	sscanRecordD08CA = 417,
	sscanRecordD08CV = 418,
	sscanRecordD08LV = 419,
	sscanRecordD08NE = 420,
	sscanRecordD08EU = 421,
	sscanRecordD08PR = 422,
	sscanRecordD09HR = 423,
	sscanRecordD09LR = 424,
	sscanRecordD09DA = 425,
	sscanRecordD09CA = 426,
	sscanRecordD09CV = 427,
	sscanRecordD09LV = 428,
	sscanRecordD09NE = 429,
	sscanRecordD09EU = 430,
	sscanRecordD09PR = 431,
	sscanRecordD10HR = 432,
	sscanRecordD10LR = 433,
	sscanRecordD10DA = 434,
	sscanRecordD10CA = 435,
	sscanRecordD10CV = 436,
	sscanRecordD10LV = 437,
	sscanRecordD10NE = 438,
	sscanRecordD10EU = 439,
	sscanRecordD10PR = 440,
	sscanRecordD11HR = 441,
	sscanRecordD11LR = 442,
	sscanRecordD11DA = 443,
	sscanRecordD11CA = 444,
	sscanRecordD11CV = 445,
	sscanRecordD11LV = 446,
	sscanRecordD11NE = 447,
	sscanRecordD11EU = 448,
	sscanRecordD11PR = 449,
	sscanRecordD12HR = 450,
	sscanRecordD12LR = 451,
	sscanRecordD12DA = 452,
	sscanRecordD12CA = 453,
	sscanRecordD12CV = 454,
	sscanRecordD12LV = 455,
	sscanRecordD12NE = 456,
	sscanRecordD12EU = 457,
	sscanRecordD12PR = 458,
	sscanRecordD13HR = 459,
	sscanRecordD13LR = 460,
	sscanRecordD13DA = 461,
	sscanRecordD13CA = 462,
	sscanRecordD13CV = 463,
	sscanRecordD13LV = 464,
	sscanRecordD13NE = 465,
	sscanRecordD13EU = 466,
	sscanRecordD13PR = 467,
	sscanRecordD14HR = 468,
	sscanRecordD14LR = 469,
	sscanRecordD14DA = 470,
	sscanRecordD14CA = 471,
	sscanRecordD14CV = 472,
	sscanRecordD14LV = 473,
	sscanRecordD14NE = 474,
	sscanRecordD14EU = 475,
	sscanRecordD14PR = 476,
	sscanRecordD15HR = 477,
	sscanRecordD15LR = 478,
	sscanRecordD15DA = 479,
	sscanRecordD15CA = 480,
	sscanRecordD15CV = 481,
	sscanRecordD15LV = 482,
	sscanRecordD15NE = 483,
	sscanRecordD15EU = 484,
	sscanRecordD15PR = 485,
	sscanRecordD16HR = 486,
	sscanRecordD16LR = 487,
	sscanRecordD16DA = 488,
	sscanRecordD16CA = 489,
	sscanRecordD16CV = 490,
	sscanRecordD16LV = 491,
	sscanRecordD16NE = 492,
	sscanRecordD16EU = 493,
	sscanRecordD16PR = 494,
	sscanRecordD17HR = 495,
	sscanRecordD17LR = 496,
	sscanRecordD17DA = 497,
	sscanRecordD17CA = 498,
	sscanRecordD17CV = 499,
	sscanRecordD17LV = 500,
	sscanRecordD17NE = 501,
	sscanRecordD17EU = 502,
	sscanRecordD17PR = 503,
	sscanRecordD18HR = 504,
	sscanRecordD18LR = 505,
	sscanRecordD18DA = 506,
	sscanRecordD18CA = 507,
	sscanRecordD18CV = 508,
	sscanRecordD18LV = 509,
	sscanRecordD18NE = 510,
	sscanRecordD18EU = 511,
	sscanRecordD18PR = 512,
	sscanRecordD19HR = 513,
	sscanRecordD19LR = 514,
	sscanRecordD19DA = 515,
	sscanRecordD19CA = 516,
	sscanRecordD19CV = 517,
	sscanRecordD19LV = 518,
	sscanRecordD19NE = 519,
	sscanRecordD19EU = 520,
	sscanRecordD19PR = 521,
	sscanRecordD20HR = 522,
	sscanRecordD20LR = 523,
	sscanRecordD20DA = 524,
	sscanRecordD20CA = 525,
	sscanRecordD20CV = 526,
	sscanRecordD20LV = 527,
	sscanRecordD20NE = 528,
	sscanRecordD20EU = 529,
	sscanRecordD20PR = 530,
	sscanRecordD21HR = 531,
	sscanRecordD21LR = 532,
	sscanRecordD21DA = 533,
	sscanRecordD21CA = 534,
	sscanRecordD21CV = 535,
	sscanRecordD21LV = 536,
	sscanRecordD21NE = 537,
	sscanRecordD21EU = 538,
	sscanRecordD21PR = 539,
	sscanRecordD22HR = 540,
	sscanRecordD22LR = 541,
	sscanRecordD22DA = 542,
	sscanRecordD22CA = 543,
	sscanRecordD22CV = 544,
	sscanRecordD22LV = 545,
	sscanRecordD22NE = 546,
	sscanRecordD22EU = 547,
	sscanRecordD22PR = 548,
	sscanRecordD23HR = 549,
	sscanRecordD23LR = 550,
	sscanRecordD23DA = 551,
	sscanRecordD23CA = 552,
	sscanRecordD23CV = 553,
	sscanRecordD23LV = 554,
	sscanRecordD23NE = 555,
	sscanRecordD23EU = 556,
	sscanRecordD23PR = 557,
	sscanRecordD24HR = 558,
	sscanRecordD24LR = 559,
	sscanRecordD24DA = 560,
	sscanRecordD24CA = 561,
	sscanRecordD24CV = 562,
	sscanRecordD24LV = 563,
	sscanRecordD24NE = 564,
	sscanRecordD24EU = 565,
	sscanRecordD24PR = 566,
	sscanRecordD25HR = 567,
	sscanRecordD25LR = 568,
	sscanRecordD25DA = 569,
	sscanRecordD25CA = 570,
	sscanRecordD25CV = 571,
	sscanRecordD25LV = 572,
	sscanRecordD25NE = 573,
	sscanRecordD25EU = 574,
	sscanRecordD25PR = 575,
	sscanRecordD26HR = 576,
	sscanRecordD26LR = 577,
	sscanRecordD26DA = 578,
	sscanRecordD26CA = 579,
	sscanRecordD26CV = 580,
	sscanRecordD26LV = 581,
	sscanRecordD26NE = 582,
	sscanRecordD26EU = 583,
	sscanRecordD26PR = 584,
	sscanRecordD27HR = 585,
	sscanRecordD27LR = 586,
	sscanRecordD27DA = 587,
	sscanRecordD27CA = 588,
	sscanRecordD27CV = 589,
	sscanRecordD27LV = 590,
	sscanRecordD27NE = 591,
	sscanRecordD27EU = 592,
	sscanRecordD27PR = 593,
	sscanRecordD28HR = 594,
	sscanRecordD28LR = 595,
	sscanRecordD28DA = 596,
	sscanRecordD28CA = 597,
	sscanRecordD28CV = 598,
	sscanRecordD28LV = 599,
	sscanRecordD28NE = 600,
	sscanRecordD28EU = 601,
	sscanRecordD28PR = 602,
	sscanRecordD29HR = 603,
	sscanRecordD29LR = 604,
	sscanRecordD29DA = 605,
	sscanRecordD29CA = 606,
	sscanRecordD29CV = 607,
	sscanRecordD29LV = 608,
	sscanRecordD29NE = 609,
	sscanRecordD29EU = 610,
	sscanRecordD29PR = 611,
	sscanRecordD30HR = 612,
	sscanRecordD30LR = 613,
	sscanRecordD30DA = 614,
	sscanRecordD30CA = 615,
	sscanRecordD30CV = 616,
	sscanRecordD30LV = 617,
	sscanRecordD30NE = 618,
	sscanRecordD30EU = 619,
	sscanRecordD30PR = 620,
	sscanRecordD31HR = 621,
	sscanRecordD31LR = 622,
	sscanRecordD31DA = 623,
	sscanRecordD31CA = 624,
	sscanRecordD31CV = 625,
	sscanRecordD31LV = 626,
	sscanRecordD31NE = 627,
	sscanRecordD31EU = 628,
	sscanRecordD31PR = 629,
	sscanRecordD32HR = 630,
	sscanRecordD32LR = 631,
	sscanRecordD32DA = 632,
	sscanRecordD32CA = 633,
	sscanRecordD32CV = 634,
	sscanRecordD32LV = 635,
	sscanRecordD32NE = 636,
	sscanRecordD32EU = 637,
	sscanRecordD32PR = 638,
	sscanRecordD33HR = 639,
	sscanRecordD33LR = 640,
	sscanRecordD33DA = 641,
	sscanRecordD33CA = 642,
	sscanRecordD33CV = 643,
	sscanRecordD33LV = 644,
	sscanRecordD33NE = 645,
	sscanRecordD33EU = 646,
	sscanRecordD33PR = 647,
	sscanRecordD34HR = 648,
	sscanRecordD34LR = 649,
	sscanRecordD34DA = 650,
	sscanRecordD34CA = 651,
	sscanRecordD34CV = 652,
	sscanRecordD34LV = 653,
	sscanRecordD34NE = 654,
	sscanRecordD34EU = 655,
	sscanRecordD34PR = 656,
	sscanRecordD35HR = 657,
	sscanRecordD35LR = 658,
	sscanRecordD35DA = 659,
	sscanRecordD35CA = 660,
	sscanRecordD35CV = 661,
	sscanRecordD35LV = 662,
	sscanRecordD35NE = 663,
	sscanRecordD35EU = 664,
	sscanRecordD35PR = 665,
	sscanRecordD36HR = 666,
	sscanRecordD36LR = 667,
	sscanRecordD36DA = 668,
	sscanRecordD36CA = 669,
	sscanRecordD36CV = 670,
	sscanRecordD36LV = 671,
	sscanRecordD36NE = 672,
	sscanRecordD36EU = 673,
	sscanRecordD36PR = 674,
	sscanRecordD37HR = 675,
	sscanRecordD37LR = 676,
	sscanRecordD37DA = 677,
	sscanRecordD37CA = 678,
	sscanRecordD37CV = 679,
	sscanRecordD37LV = 680,
	sscanRecordD37NE = 681,
	sscanRecordD37EU = 682,
	sscanRecordD37PR = 683,
	sscanRecordD38HR = 684,
	sscanRecordD38LR = 685,
	sscanRecordD38DA = 686,
	sscanRecordD38CA = 687,
	sscanRecordD38CV = 688,
	sscanRecordD38LV = 689,
	sscanRecordD38NE = 690,
	sscanRecordD38EU = 691,
	sscanRecordD38PR = 692,
	sscanRecordD39HR = 693,
	sscanRecordD39LR = 694,
	sscanRecordD39DA = 695,
	sscanRecordD39CA = 696,
	sscanRecordD39CV = 697,
	sscanRecordD39LV = 698,
	sscanRecordD39NE = 699,
	sscanRecordD39EU = 700,
	sscanRecordD39PR = 701,
	sscanRecordD40HR = 702,
	sscanRecordD40LR = 703,
	sscanRecordD40DA = 704,
	sscanRecordD40CA = 705,
	sscanRecordD40CV = 706,
	sscanRecordD40LV = 707,
	sscanRecordD40NE = 708,
	sscanRecordD40EU = 709,
	sscanRecordD40PR = 710,
	sscanRecordD41HR = 711,
	sscanRecordD41LR = 712,
	sscanRecordD41DA = 713,
	sscanRecordD41CA = 714,
	sscanRecordD41CV = 715,
	sscanRecordD41LV = 716,
	sscanRecordD41NE = 717,
	sscanRecordD41EU = 718,
	sscanRecordD41PR = 719,
	sscanRecordD42HR = 720,
	sscanRecordD42LR = 721,
	sscanRecordD42DA = 722,
	sscanRecordD42CA = 723,
	sscanRecordD42CV = 724,
	sscanRecordD42LV = 725,
	sscanRecordD42NE = 726,
	sscanRecordD42EU = 727,
	sscanRecordD42PR = 728,
	sscanRecordD43HR = 729,
	sscanRecordD43LR = 730,
	sscanRecordD43DA = 731,
	sscanRecordD43CA = 732,
	sscanRecordD43CV = 733,
	sscanRecordD43LV = 734,
	sscanRecordD43NE = 735,
	sscanRecordD43EU = 736,
	sscanRecordD43PR = 737,
	sscanRecordD44HR = 738,
	sscanRecordD44LR = 739,
	sscanRecordD44DA = 740,
	sscanRecordD44CA = 741,
	sscanRecordD44CV = 742,
	sscanRecordD44LV = 743,
	sscanRecordD44NE = 744,
	sscanRecordD44EU = 745,
	sscanRecordD44PR = 746,
	sscanRecordD45HR = 747,
	sscanRecordD45LR = 748,
	sscanRecordD45DA = 749,
	sscanRecordD45CA = 750,
	sscanRecordD45CV = 751,
	sscanRecordD45LV = 752,
	sscanRecordD45NE = 753,
	sscanRecordD45EU = 754,
	sscanRecordD45PR = 755,
	sscanRecordD46HR = 756,
	sscanRecordD46LR = 757,
	sscanRecordD46DA = 758,
	sscanRecordD46CA = 759,
	sscanRecordD46CV = 760,
	sscanRecordD46LV = 761,
	sscanRecordD46NE = 762,
	sscanRecordD46EU = 763,
	sscanRecordD46PR = 764,
	sscanRecordD47HR = 765,
	sscanRecordD47LR = 766,
	sscanRecordD47DA = 767,
	sscanRecordD47CA = 768,
	sscanRecordD47CV = 769,
	sscanRecordD47LV = 770,
	sscanRecordD47NE = 771,
	sscanRecordD47EU = 772,
	sscanRecordD47PR = 773,
	sscanRecordD48HR = 774,
	sscanRecordD48LR = 775,
	sscanRecordD48DA = 776,
	sscanRecordD48CA = 777,
	sscanRecordD48CV = 778,
	sscanRecordD48LV = 779,
	sscanRecordD48NE = 780,
	sscanRecordD48EU = 781,
	sscanRecordD48PR = 782,
	sscanRecordD49HR = 783,
	sscanRecordD49LR = 784,
	sscanRecordD49DA = 785,
	sscanRecordD49CA = 786,
	sscanRecordD49CV = 787,
	sscanRecordD49LV = 788,
	sscanRecordD49NE = 789,
	sscanRecordD49EU = 790,
	sscanRecordD49PR = 791,
	sscanRecordD50HR = 792,
	sscanRecordD50LR = 793,
	sscanRecordD50DA = 794,
	sscanRecordD50CA = 795,
	sscanRecordD50CV = 796,
	sscanRecordD50LV = 797,
	sscanRecordD50NE = 798,
	sscanRecordD50EU = 799,
	sscanRecordD50PR = 800,
	sscanRecordD51HR = 801,
	sscanRecordD51LR = 802,
	sscanRecordD51DA = 803,
	sscanRecordD51CA = 804,
	sscanRecordD51CV = 805,
	sscanRecordD51LV = 806,
	sscanRecordD51NE = 807,
	sscanRecordD51EU = 808,
	sscanRecordD51PR = 809,
	sscanRecordD52HR = 810,
	sscanRecordD52LR = 811,
	sscanRecordD52DA = 812,
	sscanRecordD52CA = 813,
	sscanRecordD52CV = 814,
	sscanRecordD52LV = 815,
	sscanRecordD52NE = 816,
	sscanRecordD52EU = 817,
	sscanRecordD52PR = 818,
	sscanRecordD53HR = 819,
	sscanRecordD53LR = 820,
	sscanRecordD53DA = 821,
	sscanRecordD53CA = 822,
	sscanRecordD53CV = 823,
	sscanRecordD53LV = 824,
	sscanRecordD53NE = 825,
	sscanRecordD53EU = 826,
	sscanRecordD53PR = 827,
	sscanRecordD54HR = 828,
	sscanRecordD54LR = 829,
	sscanRecordD54DA = 830,
	sscanRecordD54CA = 831,
	sscanRecordD54CV = 832,
	sscanRecordD54LV = 833,
	sscanRecordD54NE = 834,
	sscanRecordD54EU = 835,
	sscanRecordD54PR = 836,
	sscanRecordD55HR = 837,
	sscanRecordD55LR = 838,
	sscanRecordD55DA = 839,
	sscanRecordD55CA = 840,
	sscanRecordD55CV = 841,
	sscanRecordD55LV = 842,
	sscanRecordD55NE = 843,
	sscanRecordD55EU = 844,
	sscanRecordD55PR = 845,
	sscanRecordD56HR = 846,
	sscanRecordD56LR = 847,
	sscanRecordD56DA = 848,
	sscanRecordD56CA = 849,
	sscanRecordD56CV = 850,
	sscanRecordD56LV = 851,
	sscanRecordD56NE = 852,
	sscanRecordD56EU = 853,
	sscanRecordD56PR = 854,
	sscanRecordD57HR = 855,
	sscanRecordD57LR = 856,
	sscanRecordD57DA = 857,
	sscanRecordD57CA = 858,
	sscanRecordD57CV = 859,
	sscanRecordD57LV = 860,
	sscanRecordD57NE = 861,
	sscanRecordD57EU = 862,
	sscanRecordD57PR = 863,
	sscanRecordD58HR = 864,
	sscanRecordD58LR = 865,
	sscanRecordD58DA = 866,
	sscanRecordD58CA = 867,
	sscanRecordD58CV = 868,
	sscanRecordD58LV = 869,
	sscanRecordD58NE = 870,
	sscanRecordD58EU = 871,
	sscanRecordD58PR = 872,
	sscanRecordD59HR = 873,
	sscanRecordD59LR = 874,
	sscanRecordD59DA = 875,
	sscanRecordD59CA = 876,
	sscanRecordD59CV = 877,
	sscanRecordD59LV = 878,
	sscanRecordD59NE = 879,
	sscanRecordD59EU = 880,
	sscanRecordD59PR = 881,
	sscanRecordD60HR = 882,
	sscanRecordD60LR = 883,
	sscanRecordD60DA = 884,
	sscanRecordD60CA = 885,
	sscanRecordD60CV = 886,
	sscanRecordD60LV = 887,
	sscanRecordD60NE = 888,
	sscanRecordD60EU = 889,
	sscanRecordD60PR = 890,
	sscanRecordD61HR = 891,
	sscanRecordD61LR = 892,
	sscanRecordD61DA = 893,
	sscanRecordD61CA = 894,
	sscanRecordD61CV = 895,
	sscanRecordD61LV = 896,
	sscanRecordD61NE = 897,
	sscanRecordD61EU = 898,
	sscanRecordD61PR = 899,
	sscanRecordD62HR = 900,
	sscanRecordD62LR = 901,
	sscanRecordD62DA = 902,
	sscanRecordD62CA = 903,
	sscanRecordD62CV = 904,
	sscanRecordD62LV = 905,
	sscanRecordD62NE = 906,
	sscanRecordD62EU = 907,
	sscanRecordD62PR = 908,
	sscanRecordD63HR = 909,
	sscanRecordD63LR = 910,
	sscanRecordD63DA = 911,
	sscanRecordD63CA = 912,
	sscanRecordD63CV = 913,
	sscanRecordD63LV = 914,
	sscanRecordD63NE = 915,
	sscanRecordD63EU = 916,
	sscanRecordD63PR = 917,
	sscanRecordD64HR = 918,
	sscanRecordD64LR = 919,
	sscanRecordD64DA = 920,
	sscanRecordD64CA = 921,
	sscanRecordD64CV = 922,
	sscanRecordD64LV = 923,
	sscanRecordD64NE = 924,
	sscanRecordD64EU = 925,
	sscanRecordD64PR = 926,
	sscanRecordD65HR = 927,
	sscanRecordD65LR = 928,
	sscanRecordD65DA = 929,
	sscanRecordD65CA = 930,
	sscanRecordD65CV = 931,
	sscanRecordD65LV = 932,
	sscanRecordD65NE = 933,
	sscanRecordD65EU = 934,
	sscanRecordD65PR = 935,
	sscanRecordD66HR = 936,
	sscanRecordD66LR = 937,
	sscanRecordD66DA = 938,
	sscanRecordD66CA = 939,
	sscanRecordD66CV = 940,
	sscanRecordD66LV = 941,
	sscanRecordD66NE = 942,
	sscanRecordD66EU = 943,
	sscanRecordD66PR = 944,
	sscanRecordD67HR = 945,
	sscanRecordD67LR = 946,
	sscanRecordD67DA = 947,
	sscanRecordD67CA = 948,
	sscanRecordD67CV = 949,
	sscanRecordD67LV = 950,
	sscanRecordD67NE = 951,
	sscanRecordD67EU = 952,
	sscanRecordD67PR = 953,
	sscanRecordD68HR = 954,
	sscanRecordD68LR = 955,
	sscanRecordD68DA = 956,
	sscanRecordD68CA = 957,
	sscanRecordD68CV = 958,
	sscanRecordD68LV = 959,
	sscanRecordD68NE = 960,
	sscanRecordD68EU = 961,
	sscanRecordD68PR = 962,
	sscanRecordD69HR = 963,
	sscanRecordD69LR = 964,
	sscanRecordD69DA = 965,
	sscanRecordD69CA = 966,
	sscanRecordD69CV = 967,
	sscanRecordD69LV = 968,
	sscanRecordD69NE = 969,
	sscanRecordD69EU = 970,
	sscanRecordD69PR = 971,
	sscanRecordD70HR = 972,
	sscanRecordD70LR = 973,
	sscanRecordD70DA = 974,
	sscanRecordD70CA = 975,
	sscanRecordD70CV = 976,
	sscanRecordD70LV = 977,
	sscanRecordD70NE = 978,
	sscanRecordD70EU = 979,
	sscanRecordD70PR = 980,
	sscanRecordT1CD = 981,
	sscanRecordT2CD = 982,
	sscanRecordT3CD = 983,
	sscanRecordT4CD = 984,
	sscanRecordA1CD = 985,
	sscanRecordBSCD = 986,
	sscanRecordASCD = 987,
	sscanRecordPAUS = 988,
	sscanRecordLPAU = 989,
	sscanRecordPDLY = 990,
	sscanRecordDDLY = 991,
	sscanRecordRDLY = 992,
	sscanRecordFAZE = 993,
	sscanRecordACQM = 994,
	sscanRecordACQT = 995,
	sscanRecordDSTATE = 996,
	sscanRecordCOPYTO = 997
} sscanFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsAssert.h>
#include <epicsExport.h>
#ifdef __cplusplus
extern "C" {
#endif
static int sscanRecordSizeOffset(dbRecordType *prt)
{
    sscanRecord *prec = 0;

    assert(prt->no_fields == 998);
    prt->papFldDes[sscanRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[sscanRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[sscanRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[sscanRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[sscanRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[sscanRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[sscanRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[sscanRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[sscanRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[sscanRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[sscanRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[sscanRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[sscanRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[sscanRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[sscanRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[sscanRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[sscanRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[sscanRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[sscanRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[sscanRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[sscanRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[sscanRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[sscanRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[sscanRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[sscanRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[sscanRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[sscanRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[sscanRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[sscanRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[sscanRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[sscanRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[sscanRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[sscanRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[sscanRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[sscanRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[sscanRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[sscanRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[sscanRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[sscanRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[sscanRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[sscanRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[sscanRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[sscanRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[sscanRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[sscanRecordVERS]->size = sizeof(prec->vers);
    prt->papFldDes[sscanRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[sscanRecordSMSG]->size = sizeof(prec->smsg);
    prt->papFldDes[sscanRecordCMND]->size = sizeof(prec->cmnd);
    prt->papFldDes[sscanRecordALRT]->size = sizeof(prec->alrt);
    prt->papFldDes[sscanRecordRPVT]->size = sizeof(prec->rpvt);
    prt->papFldDes[sscanRecordMPTS]->size = sizeof(prec->mpts);
    prt->papFldDes[sscanRecordEXSC]->size = sizeof(prec->exsc);
    prt->papFldDes[sscanRecordXSC]->size = sizeof(prec->xsc);
    prt->papFldDes[sscanRecordPXSC]->size = sizeof(prec->pxsc);
    prt->papFldDes[sscanRecordBUSY]->size = sizeof(prec->busy);
    prt->papFldDes[sscanRecordKILL]->size = sizeof(prec->kill);
    prt->papFldDes[sscanRecordWAIT]->size = sizeof(prec->wait);
    prt->papFldDes[sscanRecordWCNT]->size = sizeof(prec->wcnt);
    prt->papFldDes[sscanRecordAWCT]->size = sizeof(prec->awct);
    prt->papFldDes[sscanRecordWTNG]->size = sizeof(prec->wtng);
    prt->papFldDes[sscanRecordAWAIT]->size = sizeof(prec->await);
    prt->papFldDes[sscanRecordAAWAIT]->size = sizeof(prec->aawait);
    prt->papFldDes[sscanRecordDATA]->size = sizeof(prec->data);
    prt->papFldDes[sscanRecordREFD]->size = sizeof(prec->refd);
    prt->papFldDes[sscanRecordNPTS]->size = sizeof(prec->npts);
    prt->papFldDes[sscanRecordFPTS]->size = sizeof(prec->fpts);
    prt->papFldDes[sscanRecordFFO]->size = sizeof(prec->ffo);
    prt->papFldDes[sscanRecordCPT]->size = sizeof(prec->cpt);
    prt->papFldDes[sscanRecordBCPT]->size = sizeof(prec->bcpt);
    prt->papFldDes[sscanRecordDPT]->size = sizeof(prec->dpt);
    prt->papFldDes[sscanRecordPCPT]->size = sizeof(prec->pcpt);
    prt->papFldDes[sscanRecordPASM]->size = sizeof(prec->pasm);
    prt->papFldDes[sscanRecordTOLP]->size = sizeof(prec->tolp);
    prt->papFldDes[sscanRecordTLAP]->size = sizeof(prec->tlap);
    prt->papFldDes[sscanRecordATIME]->size = sizeof(prec->atime);
    prt->papFldDes[sscanRecordP1PV]->size = sizeof(prec->p1pv);
    prt->papFldDes[sscanRecordP2PV]->size = sizeof(prec->p2pv);
    prt->papFldDes[sscanRecordP3PV]->size = sizeof(prec->p3pv);
    prt->papFldDes[sscanRecordP4PV]->size = sizeof(prec->p4pv);
    prt->papFldDes[sscanRecordR1PV]->size = sizeof(prec->r1pv);
    prt->papFldDes[sscanRecordR2PV]->size = sizeof(prec->r2pv);
    prt->papFldDes[sscanRecordR3PV]->size = sizeof(prec->r3pv);
    prt->papFldDes[sscanRecordR4PV]->size = sizeof(prec->r4pv);
    prt->papFldDes[sscanRecordD01PV]->size = sizeof(prec->d01pv);
    prt->papFldDes[sscanRecordD02PV]->size = sizeof(prec->d02pv);
    prt->papFldDes[sscanRecordD03PV]->size = sizeof(prec->d03pv);
    prt->papFldDes[sscanRecordD04PV]->size = sizeof(prec->d04pv);
    prt->papFldDes[sscanRecordD05PV]->size = sizeof(prec->d05pv);
    prt->papFldDes[sscanRecordD06PV]->size = sizeof(prec->d06pv);
    prt->papFldDes[sscanRecordD07PV]->size = sizeof(prec->d07pv);
    prt->papFldDes[sscanRecordD08PV]->size = sizeof(prec->d08pv);
    prt->papFldDes[sscanRecordD09PV]->size = sizeof(prec->d09pv);
    prt->papFldDes[sscanRecordD10PV]->size = sizeof(prec->d10pv);
    prt->papFldDes[sscanRecordD11PV]->size = sizeof(prec->d11pv);
    prt->papFldDes[sscanRecordD12PV]->size = sizeof(prec->d12pv);
    prt->papFldDes[sscanRecordD13PV]->size = sizeof(prec->d13pv);
    prt->papFldDes[sscanRecordD14PV]->size = sizeof(prec->d14pv);
    prt->papFldDes[sscanRecordD15PV]->size = sizeof(prec->d15pv);
    prt->papFldDes[sscanRecordD16PV]->size = sizeof(prec->d16pv);
    prt->papFldDes[sscanRecordD17PV]->size = sizeof(prec->d17pv);
    prt->papFldDes[sscanRecordD18PV]->size = sizeof(prec->d18pv);
    prt->papFldDes[sscanRecordD19PV]->size = sizeof(prec->d19pv);
    prt->papFldDes[sscanRecordD20PV]->size = sizeof(prec->d20pv);
    prt->papFldDes[sscanRecordD21PV]->size = sizeof(prec->d21pv);
    prt->papFldDes[sscanRecordD22PV]->size = sizeof(prec->d22pv);
    prt->papFldDes[sscanRecordD23PV]->size = sizeof(prec->d23pv);
    prt->papFldDes[sscanRecordD24PV]->size = sizeof(prec->d24pv);
    prt->papFldDes[sscanRecordD25PV]->size = sizeof(prec->d25pv);
    prt->papFldDes[sscanRecordD26PV]->size = sizeof(prec->d26pv);
    prt->papFldDes[sscanRecordD27PV]->size = sizeof(prec->d27pv);
    prt->papFldDes[sscanRecordD28PV]->size = sizeof(prec->d28pv);
    prt->papFldDes[sscanRecordD29PV]->size = sizeof(prec->d29pv);
    prt->papFldDes[sscanRecordD30PV]->size = sizeof(prec->d30pv);
    prt->papFldDes[sscanRecordD31PV]->size = sizeof(prec->d31pv);
    prt->papFldDes[sscanRecordD32PV]->size = sizeof(prec->d32pv);
    prt->papFldDes[sscanRecordD33PV]->size = sizeof(prec->d33pv);
    prt->papFldDes[sscanRecordD34PV]->size = sizeof(prec->d34pv);
    prt->papFldDes[sscanRecordD35PV]->size = sizeof(prec->d35pv);
    prt->papFldDes[sscanRecordD36PV]->size = sizeof(prec->d36pv);
    prt->papFldDes[sscanRecordD37PV]->size = sizeof(prec->d37pv);
    prt->papFldDes[sscanRecordD38PV]->size = sizeof(prec->d38pv);
    prt->papFldDes[sscanRecordD39PV]->size = sizeof(prec->d39pv);
    prt->papFldDes[sscanRecordD40PV]->size = sizeof(prec->d40pv);
    prt->papFldDes[sscanRecordD41PV]->size = sizeof(prec->d41pv);
    prt->papFldDes[sscanRecordD42PV]->size = sizeof(prec->d42pv);
    prt->papFldDes[sscanRecordD43PV]->size = sizeof(prec->d43pv);
    prt->papFldDes[sscanRecordD44PV]->size = sizeof(prec->d44pv);
    prt->papFldDes[sscanRecordD45PV]->size = sizeof(prec->d45pv);
    prt->papFldDes[sscanRecordD46PV]->size = sizeof(prec->d46pv);
    prt->papFldDes[sscanRecordD47PV]->size = sizeof(prec->d47pv);
    prt->papFldDes[sscanRecordD48PV]->size = sizeof(prec->d48pv);
    prt->papFldDes[sscanRecordD49PV]->size = sizeof(prec->d49pv);
    prt->papFldDes[sscanRecordD50PV]->size = sizeof(prec->d50pv);
    prt->papFldDes[sscanRecordD51PV]->size = sizeof(prec->d51pv);
    prt->papFldDes[sscanRecordD52PV]->size = sizeof(prec->d52pv);
    prt->papFldDes[sscanRecordD53PV]->size = sizeof(prec->d53pv);
    prt->papFldDes[sscanRecordD54PV]->size = sizeof(prec->d54pv);
    prt->papFldDes[sscanRecordD55PV]->size = sizeof(prec->d55pv);
    prt->papFldDes[sscanRecordD56PV]->size = sizeof(prec->d56pv);
    prt->papFldDes[sscanRecordD57PV]->size = sizeof(prec->d57pv);
    prt->papFldDes[sscanRecordD58PV]->size = sizeof(prec->d58pv);
    prt->papFldDes[sscanRecordD59PV]->size = sizeof(prec->d59pv);
    prt->papFldDes[sscanRecordD60PV]->size = sizeof(prec->d60pv);
    prt->papFldDes[sscanRecordD61PV]->size = sizeof(prec->d61pv);
    prt->papFldDes[sscanRecordD62PV]->size = sizeof(prec->d62pv);
    prt->papFldDes[sscanRecordD63PV]->size = sizeof(prec->d63pv);
    prt->papFldDes[sscanRecordD64PV]->size = sizeof(prec->d64pv);
    prt->papFldDes[sscanRecordD65PV]->size = sizeof(prec->d65pv);
    prt->papFldDes[sscanRecordD66PV]->size = sizeof(prec->d66pv);
    prt->papFldDes[sscanRecordD67PV]->size = sizeof(prec->d67pv);
    prt->papFldDes[sscanRecordD68PV]->size = sizeof(prec->d68pv);
    prt->papFldDes[sscanRecordD69PV]->size = sizeof(prec->d69pv);
    prt->papFldDes[sscanRecordD70PV]->size = sizeof(prec->d70pv);
    prt->papFldDes[sscanRecordT1PV]->size = sizeof(prec->t1pv);
    prt->papFldDes[sscanRecordT2PV]->size = sizeof(prec->t2pv);
    prt->papFldDes[sscanRecordT3PV]->size = sizeof(prec->t3pv);
    prt->papFldDes[sscanRecordT4PV]->size = sizeof(prec->t4pv);
    prt->papFldDes[sscanRecordA1PV]->size = sizeof(prec->a1pv);
    prt->papFldDes[sscanRecordBSPV]->size = sizeof(prec->bspv);
    prt->papFldDes[sscanRecordASPV]->size = sizeof(prec->aspv);
    prt->papFldDes[sscanRecordBSWAIT]->size = sizeof(prec->bswait);
    prt->papFldDes[sscanRecordASWAIT]->size = sizeof(prec->aswait);
    prt->papFldDes[sscanRecordP1NV]->size = sizeof(prec->p1nv);
    prt->papFldDes[sscanRecordP2NV]->size = sizeof(prec->p2nv);
    prt->papFldDes[sscanRecordP3NV]->size = sizeof(prec->p3nv);
    prt->papFldDes[sscanRecordP4NV]->size = sizeof(prec->p4nv);
    prt->papFldDes[sscanRecordR1NV]->size = sizeof(prec->r1nv);
    prt->papFldDes[sscanRecordR2NV]->size = sizeof(prec->r2nv);
    prt->papFldDes[sscanRecordR3NV]->size = sizeof(prec->r3nv);
    prt->papFldDes[sscanRecordR4NV]->size = sizeof(prec->r4nv);
    prt->papFldDes[sscanRecordD01NV]->size = sizeof(prec->d01nv);
    prt->papFldDes[sscanRecordD02NV]->size = sizeof(prec->d02nv);
    prt->papFldDes[sscanRecordD03NV]->size = sizeof(prec->d03nv);
    prt->papFldDes[sscanRecordD04NV]->size = sizeof(prec->d04nv);
    prt->papFldDes[sscanRecordD05NV]->size = sizeof(prec->d05nv);
    prt->papFldDes[sscanRecordD06NV]->size = sizeof(prec->d06nv);
    prt->papFldDes[sscanRecordD07NV]->size = sizeof(prec->d07nv);
    prt->papFldDes[sscanRecordD08NV]->size = sizeof(prec->d08nv);
    prt->papFldDes[sscanRecordD09NV]->size = sizeof(prec->d09nv);
    prt->papFldDes[sscanRecordD10NV]->size = sizeof(prec->d10nv);
    prt->papFldDes[sscanRecordD11NV]->size = sizeof(prec->d11nv);
    prt->papFldDes[sscanRecordD12NV]->size = sizeof(prec->d12nv);
    prt->papFldDes[sscanRecordD13NV]->size = sizeof(prec->d13nv);
    prt->papFldDes[sscanRecordD14NV]->size = sizeof(prec->d14nv);
    prt->papFldDes[sscanRecordD15NV]->size = sizeof(prec->d15nv);
    prt->papFldDes[sscanRecordD16NV]->size = sizeof(prec->d16nv);
    prt->papFldDes[sscanRecordD17NV]->size = sizeof(prec->d17nv);
    prt->papFldDes[sscanRecordD18NV]->size = sizeof(prec->d18nv);
    prt->papFldDes[sscanRecordD19NV]->size = sizeof(prec->d19nv);
    prt->papFldDes[sscanRecordD20NV]->size = sizeof(prec->d20nv);
    prt->papFldDes[sscanRecordD21NV]->size = sizeof(prec->d21nv);
    prt->papFldDes[sscanRecordD22NV]->size = sizeof(prec->d22nv);
    prt->papFldDes[sscanRecordD23NV]->size = sizeof(prec->d23nv);
    prt->papFldDes[sscanRecordD24NV]->size = sizeof(prec->d24nv);
    prt->papFldDes[sscanRecordD25NV]->size = sizeof(prec->d25nv);
    prt->papFldDes[sscanRecordD26NV]->size = sizeof(prec->d26nv);
    prt->papFldDes[sscanRecordD27NV]->size = sizeof(prec->d27nv);
    prt->papFldDes[sscanRecordD28NV]->size = sizeof(prec->d28nv);
    prt->papFldDes[sscanRecordD29NV]->size = sizeof(prec->d29nv);
    prt->papFldDes[sscanRecordD30NV]->size = sizeof(prec->d30nv);
    prt->papFldDes[sscanRecordD31NV]->size = sizeof(prec->d31nv);
    prt->papFldDes[sscanRecordD32NV]->size = sizeof(prec->d32nv);
    prt->papFldDes[sscanRecordD33NV]->size = sizeof(prec->d33nv);
    prt->papFldDes[sscanRecordD34NV]->size = sizeof(prec->d34nv);
    prt->papFldDes[sscanRecordD35NV]->size = sizeof(prec->d35nv);
    prt->papFldDes[sscanRecordD36NV]->size = sizeof(prec->d36nv);
    prt->papFldDes[sscanRecordD37NV]->size = sizeof(prec->d37nv);
    prt->papFldDes[sscanRecordD38NV]->size = sizeof(prec->d38nv);
    prt->papFldDes[sscanRecordD39NV]->size = sizeof(prec->d39nv);
    prt->papFldDes[sscanRecordD40NV]->size = sizeof(prec->d40nv);
    prt->papFldDes[sscanRecordD41NV]->size = sizeof(prec->d41nv);
    prt->papFldDes[sscanRecordD42NV]->size = sizeof(prec->d42nv);
    prt->papFldDes[sscanRecordD43NV]->size = sizeof(prec->d43nv);
    prt->papFldDes[sscanRecordD44NV]->size = sizeof(prec->d44nv);
    prt->papFldDes[sscanRecordD45NV]->size = sizeof(prec->d45nv);
    prt->papFldDes[sscanRecordD46NV]->size = sizeof(prec->d46nv);
    prt->papFldDes[sscanRecordD47NV]->size = sizeof(prec->d47nv);
    prt->papFldDes[sscanRecordD48NV]->size = sizeof(prec->d48nv);
    prt->papFldDes[sscanRecordD49NV]->size = sizeof(prec->d49nv);
    prt->papFldDes[sscanRecordD50NV]->size = sizeof(prec->d50nv);
    prt->papFldDes[sscanRecordD51NV]->size = sizeof(prec->d51nv);
    prt->papFldDes[sscanRecordD52NV]->size = sizeof(prec->d52nv);
    prt->papFldDes[sscanRecordD53NV]->size = sizeof(prec->d53nv);
    prt->papFldDes[sscanRecordD54NV]->size = sizeof(prec->d54nv);
    prt->papFldDes[sscanRecordD55NV]->size = sizeof(prec->d55nv);
    prt->papFldDes[sscanRecordD56NV]->size = sizeof(prec->d56nv);
    prt->papFldDes[sscanRecordD57NV]->size = sizeof(prec->d57nv);
    prt->papFldDes[sscanRecordD58NV]->size = sizeof(prec->d58nv);
    prt->papFldDes[sscanRecordD59NV]->size = sizeof(prec->d59nv);
    prt->papFldDes[sscanRecordD60NV]->size = sizeof(prec->d60nv);
    prt->papFldDes[sscanRecordD61NV]->size = sizeof(prec->d61nv);
    prt->papFldDes[sscanRecordD62NV]->size = sizeof(prec->d62nv);
    prt->papFldDes[sscanRecordD63NV]->size = sizeof(prec->d63nv);
    prt->papFldDes[sscanRecordD64NV]->size = sizeof(prec->d64nv);
    prt->papFldDes[sscanRecordD65NV]->size = sizeof(prec->d65nv);
    prt->papFldDes[sscanRecordD66NV]->size = sizeof(prec->d66nv);
    prt->papFldDes[sscanRecordD67NV]->size = sizeof(prec->d67nv);
    prt->papFldDes[sscanRecordD68NV]->size = sizeof(prec->d68nv);
    prt->papFldDes[sscanRecordD69NV]->size = sizeof(prec->d69nv);
    prt->papFldDes[sscanRecordD70NV]->size = sizeof(prec->d70nv);
    prt->papFldDes[sscanRecordT1NV]->size = sizeof(prec->t1nv);
    prt->papFldDes[sscanRecordT2NV]->size = sizeof(prec->t2nv);
    prt->papFldDes[sscanRecordT3NV]->size = sizeof(prec->t3nv);
    prt->papFldDes[sscanRecordT4NV]->size = sizeof(prec->t4nv);
    prt->papFldDes[sscanRecordA1NV]->size = sizeof(prec->a1nv);
    prt->papFldDes[sscanRecordBSNV]->size = sizeof(prec->bsnv);
    prt->papFldDes[sscanRecordASNV]->size = sizeof(prec->asnv);
    prt->papFldDes[sscanRecordP1PP]->size = sizeof(prec->p1pp);
    prt->papFldDes[sscanRecordP1CV]->size = sizeof(prec->p1cv);
    prt->papFldDes[sscanRecordP1DV]->size = sizeof(prec->p1dv);
    prt->papFldDes[sscanRecordP1LV]->size = sizeof(prec->p1lv);
    prt->papFldDes[sscanRecordP1SP]->size = sizeof(prec->p1sp);
    prt->papFldDes[sscanRecordP1SI]->size = sizeof(prec->p1si);
    prt->papFldDes[sscanRecordP1EP]->size = sizeof(prec->p1ep);
    prt->papFldDes[sscanRecordP1CP]->size = sizeof(prec->p1cp);
    prt->papFldDes[sscanRecordP1WD]->size = sizeof(prec->p1wd);
    prt->papFldDes[sscanRecordR1CV]->size = sizeof(prec->r1cv);
    prt->papFldDes[sscanRecordR1LV]->size = sizeof(prec->r1lv);
    prt->papFldDes[sscanRecordR1DL]->size = sizeof(prec->r1dl);
    prt->papFldDes[sscanRecordP1HR]->size = sizeof(prec->p1hr);
    prt->papFldDes[sscanRecordP1LR]->size = sizeof(prec->p1lr);
    prt->papFldDes[sscanRecordP1PA]->size = sizeof(prec->p1pa);
    prt->papFldDes[sscanRecordP1RA]->size = sizeof(prec->p1ra);
    prt->papFldDes[sscanRecordP1CA]->size = sizeof(prec->p1ca);
    prt->papFldDes[sscanRecordP1FS]->size = sizeof(prec->p1fs);
    prt->papFldDes[sscanRecordP1FI]->size = sizeof(prec->p1fi);
    prt->papFldDes[sscanRecordP1FE]->size = sizeof(prec->p1fe);
    prt->papFldDes[sscanRecordP1FC]->size = sizeof(prec->p1fc);
    prt->papFldDes[sscanRecordP1FW]->size = sizeof(prec->p1fw);
    prt->papFldDes[sscanRecordP1SM]->size = sizeof(prec->p1sm);
    prt->papFldDes[sscanRecordP1AR]->size = sizeof(prec->p1ar);
    prt->papFldDes[sscanRecordP1EU]->size = sizeof(prec->p1eu);
    prt->papFldDes[sscanRecordP1PR]->size = sizeof(prec->p1pr);
    prt->papFldDes[sscanRecordP2PP]->size = sizeof(prec->p2pp);
    prt->papFldDes[sscanRecordP2CV]->size = sizeof(prec->p2cv);
    prt->papFldDes[sscanRecordP2DV]->size = sizeof(prec->p2dv);
    prt->papFldDes[sscanRecordP2LV]->size = sizeof(prec->p2lv);
    prt->papFldDes[sscanRecordP2SP]->size = sizeof(prec->p2sp);
    prt->papFldDes[sscanRecordP2SI]->size = sizeof(prec->p2si);
    prt->papFldDes[sscanRecordP2EP]->size = sizeof(prec->p2ep);
    prt->papFldDes[sscanRecordP2CP]->size = sizeof(prec->p2cp);
    prt->papFldDes[sscanRecordP2WD]->size = sizeof(prec->p2wd);
    prt->papFldDes[sscanRecordR2CV]->size = sizeof(prec->r2cv);
    prt->papFldDes[sscanRecordR2LV]->size = sizeof(prec->r2lv);
    prt->papFldDes[sscanRecordR2DL]->size = sizeof(prec->r2dl);
    prt->papFldDes[sscanRecordP2HR]->size = sizeof(prec->p2hr);
    prt->papFldDes[sscanRecordP2LR]->size = sizeof(prec->p2lr);
    prt->papFldDes[sscanRecordP2PA]->size = sizeof(prec->p2pa);
    prt->papFldDes[sscanRecordP2RA]->size = sizeof(prec->p2ra);
    prt->papFldDes[sscanRecordP2CA]->size = sizeof(prec->p2ca);
    prt->papFldDes[sscanRecordP2FS]->size = sizeof(prec->p2fs);
    prt->papFldDes[sscanRecordP2FI]->size = sizeof(prec->p2fi);
    prt->papFldDes[sscanRecordP2FE]->size = sizeof(prec->p2fe);
    prt->papFldDes[sscanRecordP2FC]->size = sizeof(prec->p2fc);
    prt->papFldDes[sscanRecordP2FW]->size = sizeof(prec->p2fw);
    prt->papFldDes[sscanRecordP2SM]->size = sizeof(prec->p2sm);
    prt->papFldDes[sscanRecordP2AR]->size = sizeof(prec->p2ar);
    prt->papFldDes[sscanRecordP2EU]->size = sizeof(prec->p2eu);
    prt->papFldDes[sscanRecordP2PR]->size = sizeof(prec->p2pr);
    prt->papFldDes[sscanRecordP3PP]->size = sizeof(prec->p3pp);
    prt->papFldDes[sscanRecordP3CV]->size = sizeof(prec->p3cv);
    prt->papFldDes[sscanRecordP3DV]->size = sizeof(prec->p3dv);
    prt->papFldDes[sscanRecordP3LV]->size = sizeof(prec->p3lv);
    prt->papFldDes[sscanRecordP3SP]->size = sizeof(prec->p3sp);
    prt->papFldDes[sscanRecordP3SI]->size = sizeof(prec->p3si);
    prt->papFldDes[sscanRecordP3EP]->size = sizeof(prec->p3ep);
    prt->papFldDes[sscanRecordP3CP]->size = sizeof(prec->p3cp);
    prt->papFldDes[sscanRecordP3WD]->size = sizeof(prec->p3wd);
    prt->papFldDes[sscanRecordR3CV]->size = sizeof(prec->r3cv);
    prt->papFldDes[sscanRecordR3LV]->size = sizeof(prec->r3lv);
    prt->papFldDes[sscanRecordR3DL]->size = sizeof(prec->r3dl);
    prt->papFldDes[sscanRecordP3HR]->size = sizeof(prec->p3hr);
    prt->papFldDes[sscanRecordP3LR]->size = sizeof(prec->p3lr);
    prt->papFldDes[sscanRecordP3PA]->size = sizeof(prec->p3pa);
    prt->papFldDes[sscanRecordP3RA]->size = sizeof(prec->p3ra);
    prt->papFldDes[sscanRecordP3CA]->size = sizeof(prec->p3ca);
    prt->papFldDes[sscanRecordP3FS]->size = sizeof(prec->p3fs);
    prt->papFldDes[sscanRecordP3FI]->size = sizeof(prec->p3fi);
    prt->papFldDes[sscanRecordP3FE]->size = sizeof(prec->p3fe);
    prt->papFldDes[sscanRecordP3FC]->size = sizeof(prec->p3fc);
    prt->papFldDes[sscanRecordP3FW]->size = sizeof(prec->p3fw);
    prt->papFldDes[sscanRecordP3SM]->size = sizeof(prec->p3sm);
    prt->papFldDes[sscanRecordP3AR]->size = sizeof(prec->p3ar);
    prt->papFldDes[sscanRecordP3EU]->size = sizeof(prec->p3eu);
    prt->papFldDes[sscanRecordP3PR]->size = sizeof(prec->p3pr);
    prt->papFldDes[sscanRecordP4PP]->size = sizeof(prec->p4pp);
    prt->papFldDes[sscanRecordP4CV]->size = sizeof(prec->p4cv);
    prt->papFldDes[sscanRecordP4DV]->size = sizeof(prec->p4dv);
    prt->papFldDes[sscanRecordP4LV]->size = sizeof(prec->p4lv);
    prt->papFldDes[sscanRecordP4SP]->size = sizeof(prec->p4sp);
    prt->papFldDes[sscanRecordP4SI]->size = sizeof(prec->p4si);
    prt->papFldDes[sscanRecordP4EP]->size = sizeof(prec->p4ep);
    prt->papFldDes[sscanRecordP4CP]->size = sizeof(prec->p4cp);
    prt->papFldDes[sscanRecordP4WD]->size = sizeof(prec->p4wd);
    prt->papFldDes[sscanRecordR4CV]->size = sizeof(prec->r4cv);
    prt->papFldDes[sscanRecordR4LV]->size = sizeof(prec->r4lv);
    prt->papFldDes[sscanRecordR4DL]->size = sizeof(prec->r4dl);
    prt->papFldDes[sscanRecordP4HR]->size = sizeof(prec->p4hr);
    prt->papFldDes[sscanRecordP4LR]->size = sizeof(prec->p4lr);
    prt->papFldDes[sscanRecordP4PA]->size = sizeof(prec->p4pa);
    prt->papFldDes[sscanRecordP4RA]->size = sizeof(prec->p4ra);
    prt->papFldDes[sscanRecordP4CA]->size = sizeof(prec->p4ca);
    prt->papFldDes[sscanRecordP4FS]->size = sizeof(prec->p4fs);
    prt->papFldDes[sscanRecordP4FI]->size = sizeof(prec->p4fi);
    prt->papFldDes[sscanRecordP4FE]->size = sizeof(prec->p4fe);
    prt->papFldDes[sscanRecordP4FC]->size = sizeof(prec->p4fc);
    prt->papFldDes[sscanRecordP4FW]->size = sizeof(prec->p4fw);
    prt->papFldDes[sscanRecordP4SM]->size = sizeof(prec->p4sm);
    prt->papFldDes[sscanRecordP4AR]->size = sizeof(prec->p4ar);
    prt->papFldDes[sscanRecordP4EU]->size = sizeof(prec->p4eu);
    prt->papFldDes[sscanRecordP4PR]->size = sizeof(prec->p4pr);
    prt->papFldDes[sscanRecordD01HR]->size = sizeof(prec->d01hr);
    prt->papFldDes[sscanRecordD01LR]->size = sizeof(prec->d01lr);
    prt->papFldDes[sscanRecordD01DA]->size = sizeof(prec->d01da);
    prt->papFldDes[sscanRecordD01CA]->size = sizeof(prec->d01ca);
    prt->papFldDes[sscanRecordD01CV]->size = sizeof(prec->d01cv);
    prt->papFldDes[sscanRecordD01LV]->size = sizeof(prec->d01lv);
    prt->papFldDes[sscanRecordD01NE]->size = sizeof(prec->d01ne);
    prt->papFldDes[sscanRecordD01EU]->size = sizeof(prec->d01eu);
    prt->papFldDes[sscanRecordD01PR]->size = sizeof(prec->d01pr);
    prt->papFldDes[sscanRecordD02HR]->size = sizeof(prec->d02hr);
    prt->papFldDes[sscanRecordD02LR]->size = sizeof(prec->d02lr);
    prt->papFldDes[sscanRecordD02DA]->size = sizeof(prec->d02da);
    prt->papFldDes[sscanRecordD02CA]->size = sizeof(prec->d02ca);
    prt->papFldDes[sscanRecordD02CV]->size = sizeof(prec->d02cv);
    prt->papFldDes[sscanRecordD02LV]->size = sizeof(prec->d02lv);
    prt->papFldDes[sscanRecordD02NE]->size = sizeof(prec->d02ne);
    prt->papFldDes[sscanRecordD02EU]->size = sizeof(prec->d02eu);
    prt->papFldDes[sscanRecordD02PR]->size = sizeof(prec->d02pr);
    prt->papFldDes[sscanRecordD03HR]->size = sizeof(prec->d03hr);
    prt->papFldDes[sscanRecordD03LR]->size = sizeof(prec->d03lr);
    prt->papFldDes[sscanRecordD03DA]->size = sizeof(prec->d03da);
    prt->papFldDes[sscanRecordD03CA]->size = sizeof(prec->d03ca);
    prt->papFldDes[sscanRecordD03CV]->size = sizeof(prec->d03cv);
    prt->papFldDes[sscanRecordD03LV]->size = sizeof(prec->d03lv);
    prt->papFldDes[sscanRecordD03NE]->size = sizeof(prec->d03ne);
    prt->papFldDes[sscanRecordD03EU]->size = sizeof(prec->d03eu);
    prt->papFldDes[sscanRecordD03PR]->size = sizeof(prec->d03pr);
    prt->papFldDes[sscanRecordD04HR]->size = sizeof(prec->d04hr);
    prt->papFldDes[sscanRecordD04LR]->size = sizeof(prec->d04lr);
    prt->papFldDes[sscanRecordD04DA]->size = sizeof(prec->d04da);
    prt->papFldDes[sscanRecordD04CA]->size = sizeof(prec->d04ca);
    prt->papFldDes[sscanRecordD04CV]->size = sizeof(prec->d04cv);
    prt->papFldDes[sscanRecordD04LV]->size = sizeof(prec->d04lv);
    prt->papFldDes[sscanRecordD04NE]->size = sizeof(prec->d04ne);
    prt->papFldDes[sscanRecordD04EU]->size = sizeof(prec->d04eu);
    prt->papFldDes[sscanRecordD04PR]->size = sizeof(prec->d04pr);
    prt->papFldDes[sscanRecordD05HR]->size = sizeof(prec->d05hr);
    prt->papFldDes[sscanRecordD05LR]->size = sizeof(prec->d05lr);
    prt->papFldDes[sscanRecordD05DA]->size = sizeof(prec->d05da);
    prt->papFldDes[sscanRecordD05CA]->size = sizeof(prec->d05ca);
    prt->papFldDes[sscanRecordD05CV]->size = sizeof(prec->d05cv);
    prt->papFldDes[sscanRecordD05LV]->size = sizeof(prec->d05lv);
    prt->papFldDes[sscanRecordD05NE]->size = sizeof(prec->d05ne);
    prt->papFldDes[sscanRecordD05EU]->size = sizeof(prec->d05eu);
    prt->papFldDes[sscanRecordD05PR]->size = sizeof(prec->d05pr);
    prt->papFldDes[sscanRecordD06HR]->size = sizeof(prec->d06hr);
    prt->papFldDes[sscanRecordD06LR]->size = sizeof(prec->d06lr);
    prt->papFldDes[sscanRecordD06DA]->size = sizeof(prec->d06da);
    prt->papFldDes[sscanRecordD06CA]->size = sizeof(prec->d06ca);
    prt->papFldDes[sscanRecordD06CV]->size = sizeof(prec->d06cv);
    prt->papFldDes[sscanRecordD06LV]->size = sizeof(prec->d06lv);
    prt->papFldDes[sscanRecordD06NE]->size = sizeof(prec->d06ne);
    prt->papFldDes[sscanRecordD06EU]->size = sizeof(prec->d06eu);
    prt->papFldDes[sscanRecordD06PR]->size = sizeof(prec->d06pr);
    prt->papFldDes[sscanRecordD07HR]->size = sizeof(prec->d07hr);
    prt->papFldDes[sscanRecordD07LR]->size = sizeof(prec->d07lr);
    prt->papFldDes[sscanRecordD07DA]->size = sizeof(prec->d07da);
    prt->papFldDes[sscanRecordD07CA]->size = sizeof(prec->d07ca);
    prt->papFldDes[sscanRecordD07CV]->size = sizeof(prec->d07cv);
    prt->papFldDes[sscanRecordD07LV]->size = sizeof(prec->d07lv);
    prt->papFldDes[sscanRecordD07NE]->size = sizeof(prec->d07ne);
    prt->papFldDes[sscanRecordD07EU]->size = sizeof(prec->d07eu);
    prt->papFldDes[sscanRecordD07PR]->size = sizeof(prec->d07pr);
    prt->papFldDes[sscanRecordD08HR]->size = sizeof(prec->d08hr);
    prt->papFldDes[sscanRecordD08LR]->size = sizeof(prec->d08lr);
    prt->papFldDes[sscanRecordD08DA]->size = sizeof(prec->d08da);
    prt->papFldDes[sscanRecordD08CA]->size = sizeof(prec->d08ca);
    prt->papFldDes[sscanRecordD08CV]->size = sizeof(prec->d08cv);
    prt->papFldDes[sscanRecordD08LV]->size = sizeof(prec->d08lv);
    prt->papFldDes[sscanRecordD08NE]->size = sizeof(prec->d08ne);
    prt->papFldDes[sscanRecordD08EU]->size = sizeof(prec->d08eu);
    prt->papFldDes[sscanRecordD08PR]->size = sizeof(prec->d08pr);
    prt->papFldDes[sscanRecordD09HR]->size = sizeof(prec->d09hr);
    prt->papFldDes[sscanRecordD09LR]->size = sizeof(prec->d09lr);
    prt->papFldDes[sscanRecordD09DA]->size = sizeof(prec->d09da);
    prt->papFldDes[sscanRecordD09CA]->size = sizeof(prec->d09ca);
    prt->papFldDes[sscanRecordD09CV]->size = sizeof(prec->d09cv);
    prt->papFldDes[sscanRecordD09LV]->size = sizeof(prec->d09lv);
    prt->papFldDes[sscanRecordD09NE]->size = sizeof(prec->d09ne);
    prt->papFldDes[sscanRecordD09EU]->size = sizeof(prec->d09eu);
    prt->papFldDes[sscanRecordD09PR]->size = sizeof(prec->d09pr);
    prt->papFldDes[sscanRecordD10HR]->size = sizeof(prec->d10hr);
    prt->papFldDes[sscanRecordD10LR]->size = sizeof(prec->d10lr);
    prt->papFldDes[sscanRecordD10DA]->size = sizeof(prec->d10da);
    prt->papFldDes[sscanRecordD10CA]->size = sizeof(prec->d10ca);
    prt->papFldDes[sscanRecordD10CV]->size = sizeof(prec->d10cv);
    prt->papFldDes[sscanRecordD10LV]->size = sizeof(prec->d10lv);
    prt->papFldDes[sscanRecordD10NE]->size = sizeof(prec->d10ne);
    prt->papFldDes[sscanRecordD10EU]->size = sizeof(prec->d10eu);
    prt->papFldDes[sscanRecordD10PR]->size = sizeof(prec->d10pr);
    prt->papFldDes[sscanRecordD11HR]->size = sizeof(prec->d11hr);
    prt->papFldDes[sscanRecordD11LR]->size = sizeof(prec->d11lr);
    prt->papFldDes[sscanRecordD11DA]->size = sizeof(prec->d11da);
    prt->papFldDes[sscanRecordD11CA]->size = sizeof(prec->d11ca);
    prt->papFldDes[sscanRecordD11CV]->size = sizeof(prec->d11cv);
    prt->papFldDes[sscanRecordD11LV]->size = sizeof(prec->d11lv);
    prt->papFldDes[sscanRecordD11NE]->size = sizeof(prec->d11ne);
    prt->papFldDes[sscanRecordD11EU]->size = sizeof(prec->d11eu);
    prt->papFldDes[sscanRecordD11PR]->size = sizeof(prec->d11pr);
    prt->papFldDes[sscanRecordD12HR]->size = sizeof(prec->d12hr);
    prt->papFldDes[sscanRecordD12LR]->size = sizeof(prec->d12lr);
    prt->papFldDes[sscanRecordD12DA]->size = sizeof(prec->d12da);
    prt->papFldDes[sscanRecordD12CA]->size = sizeof(prec->d12ca);
    prt->papFldDes[sscanRecordD12CV]->size = sizeof(prec->d12cv);
    prt->papFldDes[sscanRecordD12LV]->size = sizeof(prec->d12lv);
    prt->papFldDes[sscanRecordD12NE]->size = sizeof(prec->d12ne);
    prt->papFldDes[sscanRecordD12EU]->size = sizeof(prec->d12eu);
    prt->papFldDes[sscanRecordD12PR]->size = sizeof(prec->d12pr);
    prt->papFldDes[sscanRecordD13HR]->size = sizeof(prec->d13hr);
    prt->papFldDes[sscanRecordD13LR]->size = sizeof(prec->d13lr);
    prt->papFldDes[sscanRecordD13DA]->size = sizeof(prec->d13da);
    prt->papFldDes[sscanRecordD13CA]->size = sizeof(prec->d13ca);
    prt->papFldDes[sscanRecordD13CV]->size = sizeof(prec->d13cv);
    prt->papFldDes[sscanRecordD13LV]->size = sizeof(prec->d13lv);
    prt->papFldDes[sscanRecordD13NE]->size = sizeof(prec->d13ne);
    prt->papFldDes[sscanRecordD13EU]->size = sizeof(prec->d13eu);
    prt->papFldDes[sscanRecordD13PR]->size = sizeof(prec->d13pr);
    prt->papFldDes[sscanRecordD14HR]->size = sizeof(prec->d14hr);
    prt->papFldDes[sscanRecordD14LR]->size = sizeof(prec->d14lr);
    prt->papFldDes[sscanRecordD14DA]->size = sizeof(prec->d14da);
    prt->papFldDes[sscanRecordD14CA]->size = sizeof(prec->d14ca);
    prt->papFldDes[sscanRecordD14CV]->size = sizeof(prec->d14cv);
    prt->papFldDes[sscanRecordD14LV]->size = sizeof(prec->d14lv);
    prt->papFldDes[sscanRecordD14NE]->size = sizeof(prec->d14ne);
    prt->papFldDes[sscanRecordD14EU]->size = sizeof(prec->d14eu);
    prt->papFldDes[sscanRecordD14PR]->size = sizeof(prec->d14pr);
    prt->papFldDes[sscanRecordD15HR]->size = sizeof(prec->d15hr);
    prt->papFldDes[sscanRecordD15LR]->size = sizeof(prec->d15lr);
    prt->papFldDes[sscanRecordD15DA]->size = sizeof(prec->d15da);
    prt->papFldDes[sscanRecordD15CA]->size = sizeof(prec->d15ca);
    prt->papFldDes[sscanRecordD15CV]->size = sizeof(prec->d15cv);
    prt->papFldDes[sscanRecordD15LV]->size = sizeof(prec->d15lv);
    prt->papFldDes[sscanRecordD15NE]->size = sizeof(prec->d15ne);
    prt->papFldDes[sscanRecordD15EU]->size = sizeof(prec->d15eu);
    prt->papFldDes[sscanRecordD15PR]->size = sizeof(prec->d15pr);
    prt->papFldDes[sscanRecordD16HR]->size = sizeof(prec->d16hr);
    prt->papFldDes[sscanRecordD16LR]->size = sizeof(prec->d16lr);
    prt->papFldDes[sscanRecordD16DA]->size = sizeof(prec->d16da);
    prt->papFldDes[sscanRecordD16CA]->size = sizeof(prec->d16ca);
    prt->papFldDes[sscanRecordD16CV]->size = sizeof(prec->d16cv);
    prt->papFldDes[sscanRecordD16LV]->size = sizeof(prec->d16lv);
    prt->papFldDes[sscanRecordD16NE]->size = sizeof(prec->d16ne);
    prt->papFldDes[sscanRecordD16EU]->size = sizeof(prec->d16eu);
    prt->papFldDes[sscanRecordD16PR]->size = sizeof(prec->d16pr);
    prt->papFldDes[sscanRecordD17HR]->size = sizeof(prec->d17hr);
    prt->papFldDes[sscanRecordD17LR]->size = sizeof(prec->d17lr);
    prt->papFldDes[sscanRecordD17DA]->size = sizeof(prec->d17da);
    prt->papFldDes[sscanRecordD17CA]->size = sizeof(prec->d17ca);
    prt->papFldDes[sscanRecordD17CV]->size = sizeof(prec->d17cv);
    prt->papFldDes[sscanRecordD17LV]->size = sizeof(prec->d17lv);
    prt->papFldDes[sscanRecordD17NE]->size = sizeof(prec->d17ne);
    prt->papFldDes[sscanRecordD17EU]->size = sizeof(prec->d17eu);
    prt->papFldDes[sscanRecordD17PR]->size = sizeof(prec->d17pr);
    prt->papFldDes[sscanRecordD18HR]->size = sizeof(prec->d18hr);
    prt->papFldDes[sscanRecordD18LR]->size = sizeof(prec->d18lr);
    prt->papFldDes[sscanRecordD18DA]->size = sizeof(prec->d18da);
    prt->papFldDes[sscanRecordD18CA]->size = sizeof(prec->d18ca);
    prt->papFldDes[sscanRecordD18CV]->size = sizeof(prec->d18cv);
    prt->papFldDes[sscanRecordD18LV]->size = sizeof(prec->d18lv);
    prt->papFldDes[sscanRecordD18NE]->size = sizeof(prec->d18ne);
    prt->papFldDes[sscanRecordD18EU]->size = sizeof(prec->d18eu);
    prt->papFldDes[sscanRecordD18PR]->size = sizeof(prec->d18pr);
    prt->papFldDes[sscanRecordD19HR]->size = sizeof(prec->d19hr);
    prt->papFldDes[sscanRecordD19LR]->size = sizeof(prec->d19lr);
    prt->papFldDes[sscanRecordD19DA]->size = sizeof(prec->d19da);
    prt->papFldDes[sscanRecordD19CA]->size = sizeof(prec->d19ca);
    prt->papFldDes[sscanRecordD19CV]->size = sizeof(prec->d19cv);
    prt->papFldDes[sscanRecordD19LV]->size = sizeof(prec->d19lv);
    prt->papFldDes[sscanRecordD19NE]->size = sizeof(prec->d19ne);
    prt->papFldDes[sscanRecordD19EU]->size = sizeof(prec->d19eu);
    prt->papFldDes[sscanRecordD19PR]->size = sizeof(prec->d19pr);
    prt->papFldDes[sscanRecordD20HR]->size = sizeof(prec->d20hr);
    prt->papFldDes[sscanRecordD20LR]->size = sizeof(prec->d20lr);
    prt->papFldDes[sscanRecordD20DA]->size = sizeof(prec->d20da);
    prt->papFldDes[sscanRecordD20CA]->size = sizeof(prec->d20ca);
    prt->papFldDes[sscanRecordD20CV]->size = sizeof(prec->d20cv);
    prt->papFldDes[sscanRecordD20LV]->size = sizeof(prec->d20lv);
    prt->papFldDes[sscanRecordD20NE]->size = sizeof(prec->d20ne);
    prt->papFldDes[sscanRecordD20EU]->size = sizeof(prec->d20eu);
    prt->papFldDes[sscanRecordD20PR]->size = sizeof(prec->d20pr);
    prt->papFldDes[sscanRecordD21HR]->size = sizeof(prec->d21hr);
    prt->papFldDes[sscanRecordD21LR]->size = sizeof(prec->d21lr);
    prt->papFldDes[sscanRecordD21DA]->size = sizeof(prec->d21da);
    prt->papFldDes[sscanRecordD21CA]->size = sizeof(prec->d21ca);
    prt->papFldDes[sscanRecordD21CV]->size = sizeof(prec->d21cv);
    prt->papFldDes[sscanRecordD21LV]->size = sizeof(prec->d21lv);
    prt->papFldDes[sscanRecordD21NE]->size = sizeof(prec->d21ne);
    prt->papFldDes[sscanRecordD21EU]->size = sizeof(prec->d21eu);
    prt->papFldDes[sscanRecordD21PR]->size = sizeof(prec->d21pr);
    prt->papFldDes[sscanRecordD22HR]->size = sizeof(prec->d22hr);
    prt->papFldDes[sscanRecordD22LR]->size = sizeof(prec->d22lr);
    prt->papFldDes[sscanRecordD22DA]->size = sizeof(prec->d22da);
    prt->papFldDes[sscanRecordD22CA]->size = sizeof(prec->d22ca);
    prt->papFldDes[sscanRecordD22CV]->size = sizeof(prec->d22cv);
    prt->papFldDes[sscanRecordD22LV]->size = sizeof(prec->d22lv);
    prt->papFldDes[sscanRecordD22NE]->size = sizeof(prec->d22ne);
    prt->papFldDes[sscanRecordD22EU]->size = sizeof(prec->d22eu);
    prt->papFldDes[sscanRecordD22PR]->size = sizeof(prec->d22pr);
    prt->papFldDes[sscanRecordD23HR]->size = sizeof(prec->d23hr);
    prt->papFldDes[sscanRecordD23LR]->size = sizeof(prec->d23lr);
    prt->papFldDes[sscanRecordD23DA]->size = sizeof(prec->d23da);
    prt->papFldDes[sscanRecordD23CA]->size = sizeof(prec->d23ca);
    prt->papFldDes[sscanRecordD23CV]->size = sizeof(prec->d23cv);
    prt->papFldDes[sscanRecordD23LV]->size = sizeof(prec->d23lv);
    prt->papFldDes[sscanRecordD23NE]->size = sizeof(prec->d23ne);
    prt->papFldDes[sscanRecordD23EU]->size = sizeof(prec->d23eu);
    prt->papFldDes[sscanRecordD23PR]->size = sizeof(prec->d23pr);
    prt->papFldDes[sscanRecordD24HR]->size = sizeof(prec->d24hr);
    prt->papFldDes[sscanRecordD24LR]->size = sizeof(prec->d24lr);
    prt->papFldDes[sscanRecordD24DA]->size = sizeof(prec->d24da);
    prt->papFldDes[sscanRecordD24CA]->size = sizeof(prec->d24ca);
    prt->papFldDes[sscanRecordD24CV]->size = sizeof(prec->d24cv);
    prt->papFldDes[sscanRecordD24LV]->size = sizeof(prec->d24lv);
    prt->papFldDes[sscanRecordD24NE]->size = sizeof(prec->d24ne);
    prt->papFldDes[sscanRecordD24EU]->size = sizeof(prec->d24eu);
    prt->papFldDes[sscanRecordD24PR]->size = sizeof(prec->d24pr);
    prt->papFldDes[sscanRecordD25HR]->size = sizeof(prec->d25hr);
    prt->papFldDes[sscanRecordD25LR]->size = sizeof(prec->d25lr);
    prt->papFldDes[sscanRecordD25DA]->size = sizeof(prec->d25da);
    prt->papFldDes[sscanRecordD25CA]->size = sizeof(prec->d25ca);
    prt->papFldDes[sscanRecordD25CV]->size = sizeof(prec->d25cv);
    prt->papFldDes[sscanRecordD25LV]->size = sizeof(prec->d25lv);
    prt->papFldDes[sscanRecordD25NE]->size = sizeof(prec->d25ne);
    prt->papFldDes[sscanRecordD25EU]->size = sizeof(prec->d25eu);
    prt->papFldDes[sscanRecordD25PR]->size = sizeof(prec->d25pr);
    prt->papFldDes[sscanRecordD26HR]->size = sizeof(prec->d26hr);
    prt->papFldDes[sscanRecordD26LR]->size = sizeof(prec->d26lr);
    prt->papFldDes[sscanRecordD26DA]->size = sizeof(prec->d26da);
    prt->papFldDes[sscanRecordD26CA]->size = sizeof(prec->d26ca);
    prt->papFldDes[sscanRecordD26CV]->size = sizeof(prec->d26cv);
    prt->papFldDes[sscanRecordD26LV]->size = sizeof(prec->d26lv);
    prt->papFldDes[sscanRecordD26NE]->size = sizeof(prec->d26ne);
    prt->papFldDes[sscanRecordD26EU]->size = sizeof(prec->d26eu);
    prt->papFldDes[sscanRecordD26PR]->size = sizeof(prec->d26pr);
    prt->papFldDes[sscanRecordD27HR]->size = sizeof(prec->d27hr);
    prt->papFldDes[sscanRecordD27LR]->size = sizeof(prec->d27lr);
    prt->papFldDes[sscanRecordD27DA]->size = sizeof(prec->d27da);
    prt->papFldDes[sscanRecordD27CA]->size = sizeof(prec->d27ca);
    prt->papFldDes[sscanRecordD27CV]->size = sizeof(prec->d27cv);
    prt->papFldDes[sscanRecordD27LV]->size = sizeof(prec->d27lv);
    prt->papFldDes[sscanRecordD27NE]->size = sizeof(prec->d27ne);
    prt->papFldDes[sscanRecordD27EU]->size = sizeof(prec->d27eu);
    prt->papFldDes[sscanRecordD27PR]->size = sizeof(prec->d27pr);
    prt->papFldDes[sscanRecordD28HR]->size = sizeof(prec->d28hr);
    prt->papFldDes[sscanRecordD28LR]->size = sizeof(prec->d28lr);
    prt->papFldDes[sscanRecordD28DA]->size = sizeof(prec->d28da);
    prt->papFldDes[sscanRecordD28CA]->size = sizeof(prec->d28ca);
    prt->papFldDes[sscanRecordD28CV]->size = sizeof(prec->d28cv);
    prt->papFldDes[sscanRecordD28LV]->size = sizeof(prec->d28lv);
    prt->papFldDes[sscanRecordD28NE]->size = sizeof(prec->d28ne);
    prt->papFldDes[sscanRecordD28EU]->size = sizeof(prec->d28eu);
    prt->papFldDes[sscanRecordD28PR]->size = sizeof(prec->d28pr);
    prt->papFldDes[sscanRecordD29HR]->size = sizeof(prec->d29hr);
    prt->papFldDes[sscanRecordD29LR]->size = sizeof(prec->d29lr);
    prt->papFldDes[sscanRecordD29DA]->size = sizeof(prec->d29da);
    prt->papFldDes[sscanRecordD29CA]->size = sizeof(prec->d29ca);
    prt->papFldDes[sscanRecordD29CV]->size = sizeof(prec->d29cv);
    prt->papFldDes[sscanRecordD29LV]->size = sizeof(prec->d29lv);
    prt->papFldDes[sscanRecordD29NE]->size = sizeof(prec->d29ne);
    prt->papFldDes[sscanRecordD29EU]->size = sizeof(prec->d29eu);
    prt->papFldDes[sscanRecordD29PR]->size = sizeof(prec->d29pr);
    prt->papFldDes[sscanRecordD30HR]->size = sizeof(prec->d30hr);
    prt->papFldDes[sscanRecordD30LR]->size = sizeof(prec->d30lr);
    prt->papFldDes[sscanRecordD30DA]->size = sizeof(prec->d30da);
    prt->papFldDes[sscanRecordD30CA]->size = sizeof(prec->d30ca);
    prt->papFldDes[sscanRecordD30CV]->size = sizeof(prec->d30cv);
    prt->papFldDes[sscanRecordD30LV]->size = sizeof(prec->d30lv);
    prt->papFldDes[sscanRecordD30NE]->size = sizeof(prec->d30ne);
    prt->papFldDes[sscanRecordD30EU]->size = sizeof(prec->d30eu);
    prt->papFldDes[sscanRecordD30PR]->size = sizeof(prec->d30pr);
    prt->papFldDes[sscanRecordD31HR]->size = sizeof(prec->d31hr);
    prt->papFldDes[sscanRecordD31LR]->size = sizeof(prec->d31lr);
    prt->papFldDes[sscanRecordD31DA]->size = sizeof(prec->d31da);
    prt->papFldDes[sscanRecordD31CA]->size = sizeof(prec->d31ca);
    prt->papFldDes[sscanRecordD31CV]->size = sizeof(prec->d31cv);
    prt->papFldDes[sscanRecordD31LV]->size = sizeof(prec->d31lv);
    prt->papFldDes[sscanRecordD31NE]->size = sizeof(prec->d31ne);
    prt->papFldDes[sscanRecordD31EU]->size = sizeof(prec->d31eu);
    prt->papFldDes[sscanRecordD31PR]->size = sizeof(prec->d31pr);
    prt->papFldDes[sscanRecordD32HR]->size = sizeof(prec->d32hr);
    prt->papFldDes[sscanRecordD32LR]->size = sizeof(prec->d32lr);
    prt->papFldDes[sscanRecordD32DA]->size = sizeof(prec->d32da);
    prt->papFldDes[sscanRecordD32CA]->size = sizeof(prec->d32ca);
    prt->papFldDes[sscanRecordD32CV]->size = sizeof(prec->d32cv);
    prt->papFldDes[sscanRecordD32LV]->size = sizeof(prec->d32lv);
    prt->papFldDes[sscanRecordD32NE]->size = sizeof(prec->d32ne);
    prt->papFldDes[sscanRecordD32EU]->size = sizeof(prec->d32eu);
    prt->papFldDes[sscanRecordD32PR]->size = sizeof(prec->d32pr);
    prt->papFldDes[sscanRecordD33HR]->size = sizeof(prec->d33hr);
    prt->papFldDes[sscanRecordD33LR]->size = sizeof(prec->d33lr);
    prt->papFldDes[sscanRecordD33DA]->size = sizeof(prec->d33da);
    prt->papFldDes[sscanRecordD33CA]->size = sizeof(prec->d33ca);
    prt->papFldDes[sscanRecordD33CV]->size = sizeof(prec->d33cv);
    prt->papFldDes[sscanRecordD33LV]->size = sizeof(prec->d33lv);
    prt->papFldDes[sscanRecordD33NE]->size = sizeof(prec->d33ne);
    prt->papFldDes[sscanRecordD33EU]->size = sizeof(prec->d33eu);
    prt->papFldDes[sscanRecordD33PR]->size = sizeof(prec->d33pr);
    prt->papFldDes[sscanRecordD34HR]->size = sizeof(prec->d34hr);
    prt->papFldDes[sscanRecordD34LR]->size = sizeof(prec->d34lr);
    prt->papFldDes[sscanRecordD34DA]->size = sizeof(prec->d34da);
    prt->papFldDes[sscanRecordD34CA]->size = sizeof(prec->d34ca);
    prt->papFldDes[sscanRecordD34CV]->size = sizeof(prec->d34cv);
    prt->papFldDes[sscanRecordD34LV]->size = sizeof(prec->d34lv);
    prt->papFldDes[sscanRecordD34NE]->size = sizeof(prec->d34ne);
    prt->papFldDes[sscanRecordD34EU]->size = sizeof(prec->d34eu);
    prt->papFldDes[sscanRecordD34PR]->size = sizeof(prec->d34pr);
    prt->papFldDes[sscanRecordD35HR]->size = sizeof(prec->d35hr);
    prt->papFldDes[sscanRecordD35LR]->size = sizeof(prec->d35lr);
    prt->papFldDes[sscanRecordD35DA]->size = sizeof(prec->d35da);
    prt->papFldDes[sscanRecordD35CA]->size = sizeof(prec->d35ca);
    prt->papFldDes[sscanRecordD35CV]->size = sizeof(prec->d35cv);
    prt->papFldDes[sscanRecordD35LV]->size = sizeof(prec->d35lv);
    prt->papFldDes[sscanRecordD35NE]->size = sizeof(prec->d35ne);
    prt->papFldDes[sscanRecordD35EU]->size = sizeof(prec->d35eu);
    prt->papFldDes[sscanRecordD35PR]->size = sizeof(prec->d35pr);
    prt->papFldDes[sscanRecordD36HR]->size = sizeof(prec->d36hr);
    prt->papFldDes[sscanRecordD36LR]->size = sizeof(prec->d36lr);
    prt->papFldDes[sscanRecordD36DA]->size = sizeof(prec->d36da);
    prt->papFldDes[sscanRecordD36CA]->size = sizeof(prec->d36ca);
    prt->papFldDes[sscanRecordD36CV]->size = sizeof(prec->d36cv);
    prt->papFldDes[sscanRecordD36LV]->size = sizeof(prec->d36lv);
    prt->papFldDes[sscanRecordD36NE]->size = sizeof(prec->d36ne);
    prt->papFldDes[sscanRecordD36EU]->size = sizeof(prec->d36eu);
    prt->papFldDes[sscanRecordD36PR]->size = sizeof(prec->d36pr);
    prt->papFldDes[sscanRecordD37HR]->size = sizeof(prec->d37hr);
    prt->papFldDes[sscanRecordD37LR]->size = sizeof(prec->d37lr);
    prt->papFldDes[sscanRecordD37DA]->size = sizeof(prec->d37da);
    prt->papFldDes[sscanRecordD37CA]->size = sizeof(prec->d37ca);
    prt->papFldDes[sscanRecordD37CV]->size = sizeof(prec->d37cv);
    prt->papFldDes[sscanRecordD37LV]->size = sizeof(prec->d37lv);
    prt->papFldDes[sscanRecordD37NE]->size = sizeof(prec->d37ne);
    prt->papFldDes[sscanRecordD37EU]->size = sizeof(prec->d37eu);
    prt->papFldDes[sscanRecordD37PR]->size = sizeof(prec->d37pr);
    prt->papFldDes[sscanRecordD38HR]->size = sizeof(prec->d38hr);
    prt->papFldDes[sscanRecordD38LR]->size = sizeof(prec->d38lr);
    prt->papFldDes[sscanRecordD38DA]->size = sizeof(prec->d38da);
    prt->papFldDes[sscanRecordD38CA]->size = sizeof(prec->d38ca);
    prt->papFldDes[sscanRecordD38CV]->size = sizeof(prec->d38cv);
    prt->papFldDes[sscanRecordD38LV]->size = sizeof(prec->d38lv);
    prt->papFldDes[sscanRecordD38NE]->size = sizeof(prec->d38ne);
    prt->papFldDes[sscanRecordD38EU]->size = sizeof(prec->d38eu);
    prt->papFldDes[sscanRecordD38PR]->size = sizeof(prec->d38pr);
    prt->papFldDes[sscanRecordD39HR]->size = sizeof(prec->d39hr);
    prt->papFldDes[sscanRecordD39LR]->size = sizeof(prec->d39lr);
    prt->papFldDes[sscanRecordD39DA]->size = sizeof(prec->d39da);
    prt->papFldDes[sscanRecordD39CA]->size = sizeof(prec->d39ca);
    prt->papFldDes[sscanRecordD39CV]->size = sizeof(prec->d39cv);
    prt->papFldDes[sscanRecordD39LV]->size = sizeof(prec->d39lv);
    prt->papFldDes[sscanRecordD39NE]->size = sizeof(prec->d39ne);
    prt->papFldDes[sscanRecordD39EU]->size = sizeof(prec->d39eu);
    prt->papFldDes[sscanRecordD39PR]->size = sizeof(prec->d39pr);
    prt->papFldDes[sscanRecordD40HR]->size = sizeof(prec->d40hr);
    prt->papFldDes[sscanRecordD40LR]->size = sizeof(prec->d40lr);
    prt->papFldDes[sscanRecordD40DA]->size = sizeof(prec->d40da);
    prt->papFldDes[sscanRecordD40CA]->size = sizeof(prec->d40ca);
    prt->papFldDes[sscanRecordD40CV]->size = sizeof(prec->d40cv);
    prt->papFldDes[sscanRecordD40LV]->size = sizeof(prec->d40lv);
    prt->papFldDes[sscanRecordD40NE]->size = sizeof(prec->d40ne);
    prt->papFldDes[sscanRecordD40EU]->size = sizeof(prec->d40eu);
    prt->papFldDes[sscanRecordD40PR]->size = sizeof(prec->d40pr);
    prt->papFldDes[sscanRecordD41HR]->size = sizeof(prec->d41hr);
    prt->papFldDes[sscanRecordD41LR]->size = sizeof(prec->d41lr);
    prt->papFldDes[sscanRecordD41DA]->size = sizeof(prec->d41da);
    prt->papFldDes[sscanRecordD41CA]->size = sizeof(prec->d41ca);
    prt->papFldDes[sscanRecordD41CV]->size = sizeof(prec->d41cv);
    prt->papFldDes[sscanRecordD41LV]->size = sizeof(prec->d41lv);
    prt->papFldDes[sscanRecordD41NE]->size = sizeof(prec->d41ne);
    prt->papFldDes[sscanRecordD41EU]->size = sizeof(prec->d41eu);
    prt->papFldDes[sscanRecordD41PR]->size = sizeof(prec->d41pr);
    prt->papFldDes[sscanRecordD42HR]->size = sizeof(prec->d42hr);
    prt->papFldDes[sscanRecordD42LR]->size = sizeof(prec->d42lr);
    prt->papFldDes[sscanRecordD42DA]->size = sizeof(prec->d42da);
    prt->papFldDes[sscanRecordD42CA]->size = sizeof(prec->d42ca);
    prt->papFldDes[sscanRecordD42CV]->size = sizeof(prec->d42cv);
    prt->papFldDes[sscanRecordD42LV]->size = sizeof(prec->d42lv);
    prt->papFldDes[sscanRecordD42NE]->size = sizeof(prec->d42ne);
    prt->papFldDes[sscanRecordD42EU]->size = sizeof(prec->d42eu);
    prt->papFldDes[sscanRecordD42PR]->size = sizeof(prec->d42pr);
    prt->papFldDes[sscanRecordD43HR]->size = sizeof(prec->d43hr);
    prt->papFldDes[sscanRecordD43LR]->size = sizeof(prec->d43lr);
    prt->papFldDes[sscanRecordD43DA]->size = sizeof(prec->d43da);
    prt->papFldDes[sscanRecordD43CA]->size = sizeof(prec->d43ca);
    prt->papFldDes[sscanRecordD43CV]->size = sizeof(prec->d43cv);
    prt->papFldDes[sscanRecordD43LV]->size = sizeof(prec->d43lv);
    prt->papFldDes[sscanRecordD43NE]->size = sizeof(prec->d43ne);
    prt->papFldDes[sscanRecordD43EU]->size = sizeof(prec->d43eu);
    prt->papFldDes[sscanRecordD43PR]->size = sizeof(prec->d43pr);
    prt->papFldDes[sscanRecordD44HR]->size = sizeof(prec->d44hr);
    prt->papFldDes[sscanRecordD44LR]->size = sizeof(prec->d44lr);
    prt->papFldDes[sscanRecordD44DA]->size = sizeof(prec->d44da);
    prt->papFldDes[sscanRecordD44CA]->size = sizeof(prec->d44ca);
    prt->papFldDes[sscanRecordD44CV]->size = sizeof(prec->d44cv);
    prt->papFldDes[sscanRecordD44LV]->size = sizeof(prec->d44lv);
    prt->papFldDes[sscanRecordD44NE]->size = sizeof(prec->d44ne);
    prt->papFldDes[sscanRecordD44EU]->size = sizeof(prec->d44eu);
    prt->papFldDes[sscanRecordD44PR]->size = sizeof(prec->d44pr);
    prt->papFldDes[sscanRecordD45HR]->size = sizeof(prec->d45hr);
    prt->papFldDes[sscanRecordD45LR]->size = sizeof(prec->d45lr);
    prt->papFldDes[sscanRecordD45DA]->size = sizeof(prec->d45da);
    prt->papFldDes[sscanRecordD45CA]->size = sizeof(prec->d45ca);
    prt->papFldDes[sscanRecordD45CV]->size = sizeof(prec->d45cv);
    prt->papFldDes[sscanRecordD45LV]->size = sizeof(prec->d45lv);
    prt->papFldDes[sscanRecordD45NE]->size = sizeof(prec->d45ne);
    prt->papFldDes[sscanRecordD45EU]->size = sizeof(prec->d45eu);
    prt->papFldDes[sscanRecordD45PR]->size = sizeof(prec->d45pr);
    prt->papFldDes[sscanRecordD46HR]->size = sizeof(prec->d46hr);
    prt->papFldDes[sscanRecordD46LR]->size = sizeof(prec->d46lr);
    prt->papFldDes[sscanRecordD46DA]->size = sizeof(prec->d46da);
    prt->papFldDes[sscanRecordD46CA]->size = sizeof(prec->d46ca);
    prt->papFldDes[sscanRecordD46CV]->size = sizeof(prec->d46cv);
    prt->papFldDes[sscanRecordD46LV]->size = sizeof(prec->d46lv);
    prt->papFldDes[sscanRecordD46NE]->size = sizeof(prec->d46ne);
    prt->papFldDes[sscanRecordD46EU]->size = sizeof(prec->d46eu);
    prt->papFldDes[sscanRecordD46PR]->size = sizeof(prec->d46pr);
    prt->papFldDes[sscanRecordD47HR]->size = sizeof(prec->d47hr);
    prt->papFldDes[sscanRecordD47LR]->size = sizeof(prec->d47lr);
    prt->papFldDes[sscanRecordD47DA]->size = sizeof(prec->d47da);
    prt->papFldDes[sscanRecordD47CA]->size = sizeof(prec->d47ca);
    prt->papFldDes[sscanRecordD47CV]->size = sizeof(prec->d47cv);
    prt->papFldDes[sscanRecordD47LV]->size = sizeof(prec->d47lv);
    prt->papFldDes[sscanRecordD47NE]->size = sizeof(prec->d47ne);
    prt->papFldDes[sscanRecordD47EU]->size = sizeof(prec->d47eu);
    prt->papFldDes[sscanRecordD47PR]->size = sizeof(prec->d47pr);
    prt->papFldDes[sscanRecordD48HR]->size = sizeof(prec->d48hr);
    prt->papFldDes[sscanRecordD48LR]->size = sizeof(prec->d48lr);
    prt->papFldDes[sscanRecordD48DA]->size = sizeof(prec->d48da);
    prt->papFldDes[sscanRecordD48CA]->size = sizeof(prec->d48ca);
    prt->papFldDes[sscanRecordD48CV]->size = sizeof(prec->d48cv);
    prt->papFldDes[sscanRecordD48LV]->size = sizeof(prec->d48lv);
    prt->papFldDes[sscanRecordD48NE]->size = sizeof(prec->d48ne);
    prt->papFldDes[sscanRecordD48EU]->size = sizeof(prec->d48eu);
    prt->papFldDes[sscanRecordD48PR]->size = sizeof(prec->d48pr);
    prt->papFldDes[sscanRecordD49HR]->size = sizeof(prec->d49hr);
    prt->papFldDes[sscanRecordD49LR]->size = sizeof(prec->d49lr);
    prt->papFldDes[sscanRecordD49DA]->size = sizeof(prec->d49da);
    prt->papFldDes[sscanRecordD49CA]->size = sizeof(prec->d49ca);
    prt->papFldDes[sscanRecordD49CV]->size = sizeof(prec->d49cv);
    prt->papFldDes[sscanRecordD49LV]->size = sizeof(prec->d49lv);
    prt->papFldDes[sscanRecordD49NE]->size = sizeof(prec->d49ne);
    prt->papFldDes[sscanRecordD49EU]->size = sizeof(prec->d49eu);
    prt->papFldDes[sscanRecordD49PR]->size = sizeof(prec->d49pr);
    prt->papFldDes[sscanRecordD50HR]->size = sizeof(prec->d50hr);
    prt->papFldDes[sscanRecordD50LR]->size = sizeof(prec->d50lr);
    prt->papFldDes[sscanRecordD50DA]->size = sizeof(prec->d50da);
    prt->papFldDes[sscanRecordD50CA]->size = sizeof(prec->d50ca);
    prt->papFldDes[sscanRecordD50CV]->size = sizeof(prec->d50cv);
    prt->papFldDes[sscanRecordD50LV]->size = sizeof(prec->d50lv);
    prt->papFldDes[sscanRecordD50NE]->size = sizeof(prec->d50ne);
    prt->papFldDes[sscanRecordD50EU]->size = sizeof(prec->d50eu);
    prt->papFldDes[sscanRecordD50PR]->size = sizeof(prec->d50pr);
    prt->papFldDes[sscanRecordD51HR]->size = sizeof(prec->d51hr);
    prt->papFldDes[sscanRecordD51LR]->size = sizeof(prec->d51lr);
    prt->papFldDes[sscanRecordD51DA]->size = sizeof(prec->d51da);
    prt->papFldDes[sscanRecordD51CA]->size = sizeof(prec->d51ca);
    prt->papFldDes[sscanRecordD51CV]->size = sizeof(prec->d51cv);
    prt->papFldDes[sscanRecordD51LV]->size = sizeof(prec->d51lv);
    prt->papFldDes[sscanRecordD51NE]->size = sizeof(prec->d51ne);
    prt->papFldDes[sscanRecordD51EU]->size = sizeof(prec->d51eu);
    prt->papFldDes[sscanRecordD51PR]->size = sizeof(prec->d51pr);
    prt->papFldDes[sscanRecordD52HR]->size = sizeof(prec->d52hr);
    prt->papFldDes[sscanRecordD52LR]->size = sizeof(prec->d52lr);
    prt->papFldDes[sscanRecordD52DA]->size = sizeof(prec->d52da);
    prt->papFldDes[sscanRecordD52CA]->size = sizeof(prec->d52ca);
    prt->papFldDes[sscanRecordD52CV]->size = sizeof(prec->d52cv);
    prt->papFldDes[sscanRecordD52LV]->size = sizeof(prec->d52lv);
    prt->papFldDes[sscanRecordD52NE]->size = sizeof(prec->d52ne);
    prt->papFldDes[sscanRecordD52EU]->size = sizeof(prec->d52eu);
    prt->papFldDes[sscanRecordD52PR]->size = sizeof(prec->d52pr);
    prt->papFldDes[sscanRecordD53HR]->size = sizeof(prec->d53hr);
    prt->papFldDes[sscanRecordD53LR]->size = sizeof(prec->d53lr);
    prt->papFldDes[sscanRecordD53DA]->size = sizeof(prec->d53da);
    prt->papFldDes[sscanRecordD53CA]->size = sizeof(prec->d53ca);
    prt->papFldDes[sscanRecordD53CV]->size = sizeof(prec->d53cv);
    prt->papFldDes[sscanRecordD53LV]->size = sizeof(prec->d53lv);
    prt->papFldDes[sscanRecordD53NE]->size = sizeof(prec->d53ne);
    prt->papFldDes[sscanRecordD53EU]->size = sizeof(prec->d53eu);
    prt->papFldDes[sscanRecordD53PR]->size = sizeof(prec->d53pr);
    prt->papFldDes[sscanRecordD54HR]->size = sizeof(prec->d54hr);
    prt->papFldDes[sscanRecordD54LR]->size = sizeof(prec->d54lr);
    prt->papFldDes[sscanRecordD54DA]->size = sizeof(prec->d54da);
    prt->papFldDes[sscanRecordD54CA]->size = sizeof(prec->d54ca);
    prt->papFldDes[sscanRecordD54CV]->size = sizeof(prec->d54cv);
    prt->papFldDes[sscanRecordD54LV]->size = sizeof(prec->d54lv);
    prt->papFldDes[sscanRecordD54NE]->size = sizeof(prec->d54ne);
    prt->papFldDes[sscanRecordD54EU]->size = sizeof(prec->d54eu);
    prt->papFldDes[sscanRecordD54PR]->size = sizeof(prec->d54pr);
    prt->papFldDes[sscanRecordD55HR]->size = sizeof(prec->d55hr);
    prt->papFldDes[sscanRecordD55LR]->size = sizeof(prec->d55lr);
    prt->papFldDes[sscanRecordD55DA]->size = sizeof(prec->d55da);
    prt->papFldDes[sscanRecordD55CA]->size = sizeof(prec->d55ca);
    prt->papFldDes[sscanRecordD55CV]->size = sizeof(prec->d55cv);
    prt->papFldDes[sscanRecordD55LV]->size = sizeof(prec->d55lv);
    prt->papFldDes[sscanRecordD55NE]->size = sizeof(prec->d55ne);
    prt->papFldDes[sscanRecordD55EU]->size = sizeof(prec->d55eu);
    prt->papFldDes[sscanRecordD55PR]->size = sizeof(prec->d55pr);
    prt->papFldDes[sscanRecordD56HR]->size = sizeof(prec->d56hr);
    prt->papFldDes[sscanRecordD56LR]->size = sizeof(prec->d56lr);
    prt->papFldDes[sscanRecordD56DA]->size = sizeof(prec->d56da);
    prt->papFldDes[sscanRecordD56CA]->size = sizeof(prec->d56ca);
    prt->papFldDes[sscanRecordD56CV]->size = sizeof(prec->d56cv);
    prt->papFldDes[sscanRecordD56LV]->size = sizeof(prec->d56lv);
    prt->papFldDes[sscanRecordD56NE]->size = sizeof(prec->d56ne);
    prt->papFldDes[sscanRecordD56EU]->size = sizeof(prec->d56eu);
    prt->papFldDes[sscanRecordD56PR]->size = sizeof(prec->d56pr);
    prt->papFldDes[sscanRecordD57HR]->size = sizeof(prec->d57hr);
    prt->papFldDes[sscanRecordD57LR]->size = sizeof(prec->d57lr);
    prt->papFldDes[sscanRecordD57DA]->size = sizeof(prec->d57da);
    prt->papFldDes[sscanRecordD57CA]->size = sizeof(prec->d57ca);
    prt->papFldDes[sscanRecordD57CV]->size = sizeof(prec->d57cv);
    prt->papFldDes[sscanRecordD57LV]->size = sizeof(prec->d57lv);
    prt->papFldDes[sscanRecordD57NE]->size = sizeof(prec->d57ne);
    prt->papFldDes[sscanRecordD57EU]->size = sizeof(prec->d57eu);
    prt->papFldDes[sscanRecordD57PR]->size = sizeof(prec->d57pr);
    prt->papFldDes[sscanRecordD58HR]->size = sizeof(prec->d58hr);
    prt->papFldDes[sscanRecordD58LR]->size = sizeof(prec->d58lr);
    prt->papFldDes[sscanRecordD58DA]->size = sizeof(prec->d58da);
    prt->papFldDes[sscanRecordD58CA]->size = sizeof(prec->d58ca);
    prt->papFldDes[sscanRecordD58CV]->size = sizeof(prec->d58cv);
    prt->papFldDes[sscanRecordD58LV]->size = sizeof(prec->d58lv);
    prt->papFldDes[sscanRecordD58NE]->size = sizeof(prec->d58ne);
    prt->papFldDes[sscanRecordD58EU]->size = sizeof(prec->d58eu);
    prt->papFldDes[sscanRecordD58PR]->size = sizeof(prec->d58pr);
    prt->papFldDes[sscanRecordD59HR]->size = sizeof(prec->d59hr);
    prt->papFldDes[sscanRecordD59LR]->size = sizeof(prec->d59lr);
    prt->papFldDes[sscanRecordD59DA]->size = sizeof(prec->d59da);
    prt->papFldDes[sscanRecordD59CA]->size = sizeof(prec->d59ca);
    prt->papFldDes[sscanRecordD59CV]->size = sizeof(prec->d59cv);
    prt->papFldDes[sscanRecordD59LV]->size = sizeof(prec->d59lv);
    prt->papFldDes[sscanRecordD59NE]->size = sizeof(prec->d59ne);
    prt->papFldDes[sscanRecordD59EU]->size = sizeof(prec->d59eu);
    prt->papFldDes[sscanRecordD59PR]->size = sizeof(prec->d59pr);
    prt->papFldDes[sscanRecordD60HR]->size = sizeof(prec->d60hr);
    prt->papFldDes[sscanRecordD60LR]->size = sizeof(prec->d60lr);
    prt->papFldDes[sscanRecordD60DA]->size = sizeof(prec->d60da);
    prt->papFldDes[sscanRecordD60CA]->size = sizeof(prec->d60ca);
    prt->papFldDes[sscanRecordD60CV]->size = sizeof(prec->d60cv);
    prt->papFldDes[sscanRecordD60LV]->size = sizeof(prec->d60lv);
    prt->papFldDes[sscanRecordD60NE]->size = sizeof(prec->d60ne);
    prt->papFldDes[sscanRecordD60EU]->size = sizeof(prec->d60eu);
    prt->papFldDes[sscanRecordD60PR]->size = sizeof(prec->d60pr);
    prt->papFldDes[sscanRecordD61HR]->size = sizeof(prec->d61hr);
    prt->papFldDes[sscanRecordD61LR]->size = sizeof(prec->d61lr);
    prt->papFldDes[sscanRecordD61DA]->size = sizeof(prec->d61da);
    prt->papFldDes[sscanRecordD61CA]->size = sizeof(prec->d61ca);
    prt->papFldDes[sscanRecordD61CV]->size = sizeof(prec->d61cv);
    prt->papFldDes[sscanRecordD61LV]->size = sizeof(prec->d61lv);
    prt->papFldDes[sscanRecordD61NE]->size = sizeof(prec->d61ne);
    prt->papFldDes[sscanRecordD61EU]->size = sizeof(prec->d61eu);
    prt->papFldDes[sscanRecordD61PR]->size = sizeof(prec->d61pr);
    prt->papFldDes[sscanRecordD62HR]->size = sizeof(prec->d62hr);
    prt->papFldDes[sscanRecordD62LR]->size = sizeof(prec->d62lr);
    prt->papFldDes[sscanRecordD62DA]->size = sizeof(prec->d62da);
    prt->papFldDes[sscanRecordD62CA]->size = sizeof(prec->d62ca);
    prt->papFldDes[sscanRecordD62CV]->size = sizeof(prec->d62cv);
    prt->papFldDes[sscanRecordD62LV]->size = sizeof(prec->d62lv);
    prt->papFldDes[sscanRecordD62NE]->size = sizeof(prec->d62ne);
    prt->papFldDes[sscanRecordD62EU]->size = sizeof(prec->d62eu);
    prt->papFldDes[sscanRecordD62PR]->size = sizeof(prec->d62pr);
    prt->papFldDes[sscanRecordD63HR]->size = sizeof(prec->d63hr);
    prt->papFldDes[sscanRecordD63LR]->size = sizeof(prec->d63lr);
    prt->papFldDes[sscanRecordD63DA]->size = sizeof(prec->d63da);
    prt->papFldDes[sscanRecordD63CA]->size = sizeof(prec->d63ca);
    prt->papFldDes[sscanRecordD63CV]->size = sizeof(prec->d63cv);
    prt->papFldDes[sscanRecordD63LV]->size = sizeof(prec->d63lv);
    prt->papFldDes[sscanRecordD63NE]->size = sizeof(prec->d63ne);
    prt->papFldDes[sscanRecordD63EU]->size = sizeof(prec->d63eu);
    prt->papFldDes[sscanRecordD63PR]->size = sizeof(prec->d63pr);
    prt->papFldDes[sscanRecordD64HR]->size = sizeof(prec->d64hr);
    prt->papFldDes[sscanRecordD64LR]->size = sizeof(prec->d64lr);
    prt->papFldDes[sscanRecordD64DA]->size = sizeof(prec->d64da);
    prt->papFldDes[sscanRecordD64CA]->size = sizeof(prec->d64ca);
    prt->papFldDes[sscanRecordD64CV]->size = sizeof(prec->d64cv);
    prt->papFldDes[sscanRecordD64LV]->size = sizeof(prec->d64lv);
    prt->papFldDes[sscanRecordD64NE]->size = sizeof(prec->d64ne);
    prt->papFldDes[sscanRecordD64EU]->size = sizeof(prec->d64eu);
    prt->papFldDes[sscanRecordD64PR]->size = sizeof(prec->d64pr);
    prt->papFldDes[sscanRecordD65HR]->size = sizeof(prec->d65hr);
    prt->papFldDes[sscanRecordD65LR]->size = sizeof(prec->d65lr);
    prt->papFldDes[sscanRecordD65DA]->size = sizeof(prec->d65da);
    prt->papFldDes[sscanRecordD65CA]->size = sizeof(prec->d65ca);
    prt->papFldDes[sscanRecordD65CV]->size = sizeof(prec->d65cv);
    prt->papFldDes[sscanRecordD65LV]->size = sizeof(prec->d65lv);
    prt->papFldDes[sscanRecordD65NE]->size = sizeof(prec->d65ne);
    prt->papFldDes[sscanRecordD65EU]->size = sizeof(prec->d65eu);
    prt->papFldDes[sscanRecordD65PR]->size = sizeof(prec->d65pr);
    prt->papFldDes[sscanRecordD66HR]->size = sizeof(prec->d66hr);
    prt->papFldDes[sscanRecordD66LR]->size = sizeof(prec->d66lr);
    prt->papFldDes[sscanRecordD66DA]->size = sizeof(prec->d66da);
    prt->papFldDes[sscanRecordD66CA]->size = sizeof(prec->d66ca);
    prt->papFldDes[sscanRecordD66CV]->size = sizeof(prec->d66cv);
    prt->papFldDes[sscanRecordD66LV]->size = sizeof(prec->d66lv);
    prt->papFldDes[sscanRecordD66NE]->size = sizeof(prec->d66ne);
    prt->papFldDes[sscanRecordD66EU]->size = sizeof(prec->d66eu);
    prt->papFldDes[sscanRecordD66PR]->size = sizeof(prec->d66pr);
    prt->papFldDes[sscanRecordD67HR]->size = sizeof(prec->d67hr);
    prt->papFldDes[sscanRecordD67LR]->size = sizeof(prec->d67lr);
    prt->papFldDes[sscanRecordD67DA]->size = sizeof(prec->d67da);
    prt->papFldDes[sscanRecordD67CA]->size = sizeof(prec->d67ca);
    prt->papFldDes[sscanRecordD67CV]->size = sizeof(prec->d67cv);
    prt->papFldDes[sscanRecordD67LV]->size = sizeof(prec->d67lv);
    prt->papFldDes[sscanRecordD67NE]->size = sizeof(prec->d67ne);
    prt->papFldDes[sscanRecordD67EU]->size = sizeof(prec->d67eu);
    prt->papFldDes[sscanRecordD67PR]->size = sizeof(prec->d67pr);
    prt->papFldDes[sscanRecordD68HR]->size = sizeof(prec->d68hr);
    prt->papFldDes[sscanRecordD68LR]->size = sizeof(prec->d68lr);
    prt->papFldDes[sscanRecordD68DA]->size = sizeof(prec->d68da);
    prt->papFldDes[sscanRecordD68CA]->size = sizeof(prec->d68ca);
    prt->papFldDes[sscanRecordD68CV]->size = sizeof(prec->d68cv);
    prt->papFldDes[sscanRecordD68LV]->size = sizeof(prec->d68lv);
    prt->papFldDes[sscanRecordD68NE]->size = sizeof(prec->d68ne);
    prt->papFldDes[sscanRecordD68EU]->size = sizeof(prec->d68eu);
    prt->papFldDes[sscanRecordD68PR]->size = sizeof(prec->d68pr);
    prt->papFldDes[sscanRecordD69HR]->size = sizeof(prec->d69hr);
    prt->papFldDes[sscanRecordD69LR]->size = sizeof(prec->d69lr);
    prt->papFldDes[sscanRecordD69DA]->size = sizeof(prec->d69da);
    prt->papFldDes[sscanRecordD69CA]->size = sizeof(prec->d69ca);
    prt->papFldDes[sscanRecordD69CV]->size = sizeof(prec->d69cv);
    prt->papFldDes[sscanRecordD69LV]->size = sizeof(prec->d69lv);
    prt->papFldDes[sscanRecordD69NE]->size = sizeof(prec->d69ne);
    prt->papFldDes[sscanRecordD69EU]->size = sizeof(prec->d69eu);
    prt->papFldDes[sscanRecordD69PR]->size = sizeof(prec->d69pr);
    prt->papFldDes[sscanRecordD70HR]->size = sizeof(prec->d70hr);
    prt->papFldDes[sscanRecordD70LR]->size = sizeof(prec->d70lr);
    prt->papFldDes[sscanRecordD70DA]->size = sizeof(prec->d70da);
    prt->papFldDes[sscanRecordD70CA]->size = sizeof(prec->d70ca);
    prt->papFldDes[sscanRecordD70CV]->size = sizeof(prec->d70cv);
    prt->papFldDes[sscanRecordD70LV]->size = sizeof(prec->d70lv);
    prt->papFldDes[sscanRecordD70NE]->size = sizeof(prec->d70ne);
    prt->papFldDes[sscanRecordD70EU]->size = sizeof(prec->d70eu);
    prt->papFldDes[sscanRecordD70PR]->size = sizeof(prec->d70pr);
    prt->papFldDes[sscanRecordT1CD]->size = sizeof(prec->t1cd);
    prt->papFldDes[sscanRecordT2CD]->size = sizeof(prec->t2cd);
    prt->papFldDes[sscanRecordT3CD]->size = sizeof(prec->t3cd);
    prt->papFldDes[sscanRecordT4CD]->size = sizeof(prec->t4cd);
    prt->papFldDes[sscanRecordA1CD]->size = sizeof(prec->a1cd);
    prt->papFldDes[sscanRecordBSCD]->size = sizeof(prec->bscd);
    prt->papFldDes[sscanRecordASCD]->size = sizeof(prec->ascd);
    prt->papFldDes[sscanRecordPAUS]->size = sizeof(prec->paus);
    prt->papFldDes[sscanRecordLPAU]->size = sizeof(prec->lpau);
    prt->papFldDes[sscanRecordPDLY]->size = sizeof(prec->pdly);
    prt->papFldDes[sscanRecordDDLY]->size = sizeof(prec->ddly);
    prt->papFldDes[sscanRecordRDLY]->size = sizeof(prec->rdly);
    prt->papFldDes[sscanRecordFAZE]->size = sizeof(prec->faze);
    prt->papFldDes[sscanRecordACQM]->size = sizeof(prec->acqm);
    prt->papFldDes[sscanRecordACQT]->size = sizeof(prec->acqt);
    prt->papFldDes[sscanRecordDSTATE]->size = sizeof(prec->dstate);
    prt->papFldDes[sscanRecordCOPYTO]->size = sizeof(prec->copyto);
    prt->papFldDes[sscanRecordNAME]->offset = (unsigned short)((char *)&prec->name - (char *)prec);
    prt->papFldDes[sscanRecordDESC]->offset = (unsigned short)((char *)&prec->desc - (char *)prec);
    prt->papFldDes[sscanRecordASG]->offset = (unsigned short)((char *)&prec->asg - (char *)prec);
    prt->papFldDes[sscanRecordSCAN]->offset = (unsigned short)((char *)&prec->scan - (char *)prec);
    prt->papFldDes[sscanRecordPINI]->offset = (unsigned short)((char *)&prec->pini - (char *)prec);
    prt->papFldDes[sscanRecordPHAS]->offset = (unsigned short)((char *)&prec->phas - (char *)prec);
    prt->papFldDes[sscanRecordEVNT]->offset = (unsigned short)((char *)&prec->evnt - (char *)prec);
    prt->papFldDes[sscanRecordTSE]->offset = (unsigned short)((char *)&prec->tse - (char *)prec);
    prt->papFldDes[sscanRecordTSEL]->offset = (unsigned short)((char *)&prec->tsel - (char *)prec);
    prt->papFldDes[sscanRecordDTYP]->offset = (unsigned short)((char *)&prec->dtyp - (char *)prec);
    prt->papFldDes[sscanRecordDISV]->offset = (unsigned short)((char *)&prec->disv - (char *)prec);
    prt->papFldDes[sscanRecordDISA]->offset = (unsigned short)((char *)&prec->disa - (char *)prec);
    prt->papFldDes[sscanRecordSDIS]->offset = (unsigned short)((char *)&prec->sdis - (char *)prec);
    prt->papFldDes[sscanRecordMLOK]->offset = (unsigned short)((char *)&prec->mlok - (char *)prec);
    prt->papFldDes[sscanRecordMLIS]->offset = (unsigned short)((char *)&prec->mlis - (char *)prec);
    prt->papFldDes[sscanRecordDISP]->offset = (unsigned short)((char *)&prec->disp - (char *)prec);
    prt->papFldDes[sscanRecordPROC]->offset = (unsigned short)((char *)&prec->proc - (char *)prec);
    prt->papFldDes[sscanRecordSTAT]->offset = (unsigned short)((char *)&prec->stat - (char *)prec);
    prt->papFldDes[sscanRecordSEVR]->offset = (unsigned short)((char *)&prec->sevr - (char *)prec);
    prt->papFldDes[sscanRecordNSTA]->offset = (unsigned short)((char *)&prec->nsta - (char *)prec);
    prt->papFldDes[sscanRecordNSEV]->offset = (unsigned short)((char *)&prec->nsev - (char *)prec);
    prt->papFldDes[sscanRecordACKS]->offset = (unsigned short)((char *)&prec->acks - (char *)prec);
    prt->papFldDes[sscanRecordACKT]->offset = (unsigned short)((char *)&prec->ackt - (char *)prec);
    prt->papFldDes[sscanRecordDISS]->offset = (unsigned short)((char *)&prec->diss - (char *)prec);
    prt->papFldDes[sscanRecordLCNT]->offset = (unsigned short)((char *)&prec->lcnt - (char *)prec);
    prt->papFldDes[sscanRecordPACT]->offset = (unsigned short)((char *)&prec->pact - (char *)prec);
    prt->papFldDes[sscanRecordPUTF]->offset = (unsigned short)((char *)&prec->putf - (char *)prec);
    prt->papFldDes[sscanRecordRPRO]->offset = (unsigned short)((char *)&prec->rpro - (char *)prec);
    prt->papFldDes[sscanRecordASP]->offset = (unsigned short)((char *)&prec->asp - (char *)prec);
    prt->papFldDes[sscanRecordPPN]->offset = (unsigned short)((char *)&prec->ppn - (char *)prec);
    prt->papFldDes[sscanRecordPPNR]->offset = (unsigned short)((char *)&prec->ppnr - (char *)prec);
    prt->papFldDes[sscanRecordSPVT]->offset = (unsigned short)((char *)&prec->spvt - (char *)prec);
    prt->papFldDes[sscanRecordRSET]->offset = (unsigned short)((char *)&prec->rset - (char *)prec);
    prt->papFldDes[sscanRecordDSET]->offset = (unsigned short)((char *)&prec->dset - (char *)prec);
    prt->papFldDes[sscanRecordDPVT]->offset = (unsigned short)((char *)&prec->dpvt - (char *)prec);
    prt->papFldDes[sscanRecordRDES]->offset = (unsigned short)((char *)&prec->rdes - (char *)prec);
    prt->papFldDes[sscanRecordLSET]->offset = (unsigned short)((char *)&prec->lset - (char *)prec);
    prt->papFldDes[sscanRecordPRIO]->offset = (unsigned short)((char *)&prec->prio - (char *)prec);
    prt->papFldDes[sscanRecordTPRO]->offset = (unsigned short)((char *)&prec->tpro - (char *)prec);
    prt->papFldDes[sscanRecordBKPT]->offset = (unsigned short)((char *)&prec->bkpt - (char *)prec);
    prt->papFldDes[sscanRecordUDF]->offset = (unsigned short)((char *)&prec->udf - (char *)prec);
    prt->papFldDes[sscanRecordUDFS]->offset = (unsigned short)((char *)&prec->udfs - (char *)prec);
    prt->papFldDes[sscanRecordTIME]->offset = (unsigned short)((char *)&prec->time - (char *)prec);
    prt->papFldDes[sscanRecordFLNK]->offset = (unsigned short)((char *)&prec->flnk - (char *)prec);
    prt->papFldDes[sscanRecordVERS]->offset = (unsigned short)((char *)&prec->vers - (char *)prec);
    prt->papFldDes[sscanRecordVAL]->offset = (unsigned short)((char *)&prec->val - (char *)prec);
    prt->papFldDes[sscanRecordSMSG]->offset = (unsigned short)((char *)&prec->smsg - (char *)prec);
    prt->papFldDes[sscanRecordCMND]->offset = (unsigned short)((char *)&prec->cmnd - (char *)prec);
    prt->papFldDes[sscanRecordALRT]->offset = (unsigned short)((char *)&prec->alrt - (char *)prec);
    prt->papFldDes[sscanRecordRPVT]->offset = (unsigned short)((char *)&prec->rpvt - (char *)prec);
    prt->papFldDes[sscanRecordMPTS]->offset = (unsigned short)((char *)&prec->mpts - (char *)prec);
    prt->papFldDes[sscanRecordEXSC]->offset = (unsigned short)((char *)&prec->exsc - (char *)prec);
    prt->papFldDes[sscanRecordXSC]->offset = (unsigned short)((char *)&prec->xsc - (char *)prec);
    prt->papFldDes[sscanRecordPXSC]->offset = (unsigned short)((char *)&prec->pxsc - (char *)prec);
    prt->papFldDes[sscanRecordBUSY]->offset = (unsigned short)((char *)&prec->busy - (char *)prec);
    prt->papFldDes[sscanRecordKILL]->offset = (unsigned short)((char *)&prec->kill - (char *)prec);
    prt->papFldDes[sscanRecordWAIT]->offset = (unsigned short)((char *)&prec->wait - (char *)prec);
    prt->papFldDes[sscanRecordWCNT]->offset = (unsigned short)((char *)&prec->wcnt - (char *)prec);
    prt->papFldDes[sscanRecordAWCT]->offset = (unsigned short)((char *)&prec->awct - (char *)prec);
    prt->papFldDes[sscanRecordWTNG]->offset = (unsigned short)((char *)&prec->wtng - (char *)prec);
    prt->papFldDes[sscanRecordAWAIT]->offset = (unsigned short)((char *)&prec->await - (char *)prec);
    prt->papFldDes[sscanRecordAAWAIT]->offset = (unsigned short)((char *)&prec->aawait - (char *)prec);
    prt->papFldDes[sscanRecordDATA]->offset = (unsigned short)((char *)&prec->data - (char *)prec);
    prt->papFldDes[sscanRecordREFD]->offset = (unsigned short)((char *)&prec->refd - (char *)prec);
    prt->papFldDes[sscanRecordNPTS]->offset = (unsigned short)((char *)&prec->npts - (char *)prec);
    prt->papFldDes[sscanRecordFPTS]->offset = (unsigned short)((char *)&prec->fpts - (char *)prec);
    prt->papFldDes[sscanRecordFFO]->offset = (unsigned short)((char *)&prec->ffo - (char *)prec);
    prt->papFldDes[sscanRecordCPT]->offset = (unsigned short)((char *)&prec->cpt - (char *)prec);
    prt->papFldDes[sscanRecordBCPT]->offset = (unsigned short)((char *)&prec->bcpt - (char *)prec);
    prt->papFldDes[sscanRecordDPT]->offset = (unsigned short)((char *)&prec->dpt - (char *)prec);
    prt->papFldDes[sscanRecordPCPT]->offset = (unsigned short)((char *)&prec->pcpt - (char *)prec);
    prt->papFldDes[sscanRecordPASM]->offset = (unsigned short)((char *)&prec->pasm - (char *)prec);
    prt->papFldDes[sscanRecordTOLP]->offset = (unsigned short)((char *)&prec->tolp - (char *)prec);
    prt->papFldDes[sscanRecordTLAP]->offset = (unsigned short)((char *)&prec->tlap - (char *)prec);
    prt->papFldDes[sscanRecordATIME]->offset = (unsigned short)((char *)&prec->atime - (char *)prec);
    prt->papFldDes[sscanRecordP1PV]->offset = (unsigned short)((char *)&prec->p1pv - (char *)prec);
    prt->papFldDes[sscanRecordP2PV]->offset = (unsigned short)((char *)&prec->p2pv - (char *)prec);
    prt->papFldDes[sscanRecordP3PV]->offset = (unsigned short)((char *)&prec->p3pv - (char *)prec);
    prt->papFldDes[sscanRecordP4PV]->offset = (unsigned short)((char *)&prec->p4pv - (char *)prec);
    prt->papFldDes[sscanRecordR1PV]->offset = (unsigned short)((char *)&prec->r1pv - (char *)prec);
    prt->papFldDes[sscanRecordR2PV]->offset = (unsigned short)((char *)&prec->r2pv - (char *)prec);
    prt->papFldDes[sscanRecordR3PV]->offset = (unsigned short)((char *)&prec->r3pv - (char *)prec);
    prt->papFldDes[sscanRecordR4PV]->offset = (unsigned short)((char *)&prec->r4pv - (char *)prec);
    prt->papFldDes[sscanRecordD01PV]->offset = (unsigned short)((char *)&prec->d01pv - (char *)prec);
    prt->papFldDes[sscanRecordD02PV]->offset = (unsigned short)((char *)&prec->d02pv - (char *)prec);
    prt->papFldDes[sscanRecordD03PV]->offset = (unsigned short)((char *)&prec->d03pv - (char *)prec);
    prt->papFldDes[sscanRecordD04PV]->offset = (unsigned short)((char *)&prec->d04pv - (char *)prec);
    prt->papFldDes[sscanRecordD05PV]->offset = (unsigned short)((char *)&prec->d05pv - (char *)prec);
    prt->papFldDes[sscanRecordD06PV]->offset = (unsigned short)((char *)&prec->d06pv - (char *)prec);
    prt->papFldDes[sscanRecordD07PV]->offset = (unsigned short)((char *)&prec->d07pv - (char *)prec);
    prt->papFldDes[sscanRecordD08PV]->offset = (unsigned short)((char *)&prec->d08pv - (char *)prec);
    prt->papFldDes[sscanRecordD09PV]->offset = (unsigned short)((char *)&prec->d09pv - (char *)prec);
    prt->papFldDes[sscanRecordD10PV]->offset = (unsigned short)((char *)&prec->d10pv - (char *)prec);
    prt->papFldDes[sscanRecordD11PV]->offset = (unsigned short)((char *)&prec->d11pv - (char *)prec);
    prt->papFldDes[sscanRecordD12PV]->offset = (unsigned short)((char *)&prec->d12pv - (char *)prec);
    prt->papFldDes[sscanRecordD13PV]->offset = (unsigned short)((char *)&prec->d13pv - (char *)prec);
    prt->papFldDes[sscanRecordD14PV]->offset = (unsigned short)((char *)&prec->d14pv - (char *)prec);
    prt->papFldDes[sscanRecordD15PV]->offset = (unsigned short)((char *)&prec->d15pv - (char *)prec);
    prt->papFldDes[sscanRecordD16PV]->offset = (unsigned short)((char *)&prec->d16pv - (char *)prec);
    prt->papFldDes[sscanRecordD17PV]->offset = (unsigned short)((char *)&prec->d17pv - (char *)prec);
    prt->papFldDes[sscanRecordD18PV]->offset = (unsigned short)((char *)&prec->d18pv - (char *)prec);
    prt->papFldDes[sscanRecordD19PV]->offset = (unsigned short)((char *)&prec->d19pv - (char *)prec);
    prt->papFldDes[sscanRecordD20PV]->offset = (unsigned short)((char *)&prec->d20pv - (char *)prec);
    prt->papFldDes[sscanRecordD21PV]->offset = (unsigned short)((char *)&prec->d21pv - (char *)prec);
    prt->papFldDes[sscanRecordD22PV]->offset = (unsigned short)((char *)&prec->d22pv - (char *)prec);
    prt->papFldDes[sscanRecordD23PV]->offset = (unsigned short)((char *)&prec->d23pv - (char *)prec);
    prt->papFldDes[sscanRecordD24PV]->offset = (unsigned short)((char *)&prec->d24pv - (char *)prec);
    prt->papFldDes[sscanRecordD25PV]->offset = (unsigned short)((char *)&prec->d25pv - (char *)prec);
    prt->papFldDes[sscanRecordD26PV]->offset = (unsigned short)((char *)&prec->d26pv - (char *)prec);
    prt->papFldDes[sscanRecordD27PV]->offset = (unsigned short)((char *)&prec->d27pv - (char *)prec);
    prt->papFldDes[sscanRecordD28PV]->offset = (unsigned short)((char *)&prec->d28pv - (char *)prec);
    prt->papFldDes[sscanRecordD29PV]->offset = (unsigned short)((char *)&prec->d29pv - (char *)prec);
    prt->papFldDes[sscanRecordD30PV]->offset = (unsigned short)((char *)&prec->d30pv - (char *)prec);
    prt->papFldDes[sscanRecordD31PV]->offset = (unsigned short)((char *)&prec->d31pv - (char *)prec);
    prt->papFldDes[sscanRecordD32PV]->offset = (unsigned short)((char *)&prec->d32pv - (char *)prec);
    prt->papFldDes[sscanRecordD33PV]->offset = (unsigned short)((char *)&prec->d33pv - (char *)prec);
    prt->papFldDes[sscanRecordD34PV]->offset = (unsigned short)((char *)&prec->d34pv - (char *)prec);
    prt->papFldDes[sscanRecordD35PV]->offset = (unsigned short)((char *)&prec->d35pv - (char *)prec);
    prt->papFldDes[sscanRecordD36PV]->offset = (unsigned short)((char *)&prec->d36pv - (char *)prec);
    prt->papFldDes[sscanRecordD37PV]->offset = (unsigned short)((char *)&prec->d37pv - (char *)prec);
    prt->papFldDes[sscanRecordD38PV]->offset = (unsigned short)((char *)&prec->d38pv - (char *)prec);
    prt->papFldDes[sscanRecordD39PV]->offset = (unsigned short)((char *)&prec->d39pv - (char *)prec);
    prt->papFldDes[sscanRecordD40PV]->offset = (unsigned short)((char *)&prec->d40pv - (char *)prec);
    prt->papFldDes[sscanRecordD41PV]->offset = (unsigned short)((char *)&prec->d41pv - (char *)prec);
    prt->papFldDes[sscanRecordD42PV]->offset = (unsigned short)((char *)&prec->d42pv - (char *)prec);
    prt->papFldDes[sscanRecordD43PV]->offset = (unsigned short)((char *)&prec->d43pv - (char *)prec);
    prt->papFldDes[sscanRecordD44PV]->offset = (unsigned short)((char *)&prec->d44pv - (char *)prec);
    prt->papFldDes[sscanRecordD45PV]->offset = (unsigned short)((char *)&prec->d45pv - (char *)prec);
    prt->papFldDes[sscanRecordD46PV]->offset = (unsigned short)((char *)&prec->d46pv - (char *)prec);
    prt->papFldDes[sscanRecordD47PV]->offset = (unsigned short)((char *)&prec->d47pv - (char *)prec);
    prt->papFldDes[sscanRecordD48PV]->offset = (unsigned short)((char *)&prec->d48pv - (char *)prec);
    prt->papFldDes[sscanRecordD49PV]->offset = (unsigned short)((char *)&prec->d49pv - (char *)prec);
    prt->papFldDes[sscanRecordD50PV]->offset = (unsigned short)((char *)&prec->d50pv - (char *)prec);
    prt->papFldDes[sscanRecordD51PV]->offset = (unsigned short)((char *)&prec->d51pv - (char *)prec);
    prt->papFldDes[sscanRecordD52PV]->offset = (unsigned short)((char *)&prec->d52pv - (char *)prec);
    prt->papFldDes[sscanRecordD53PV]->offset = (unsigned short)((char *)&prec->d53pv - (char *)prec);
    prt->papFldDes[sscanRecordD54PV]->offset = (unsigned short)((char *)&prec->d54pv - (char *)prec);
    prt->papFldDes[sscanRecordD55PV]->offset = (unsigned short)((char *)&prec->d55pv - (char *)prec);
    prt->papFldDes[sscanRecordD56PV]->offset = (unsigned short)((char *)&prec->d56pv - (char *)prec);
    prt->papFldDes[sscanRecordD57PV]->offset = (unsigned short)((char *)&prec->d57pv - (char *)prec);
    prt->papFldDes[sscanRecordD58PV]->offset = (unsigned short)((char *)&prec->d58pv - (char *)prec);
    prt->papFldDes[sscanRecordD59PV]->offset = (unsigned short)((char *)&prec->d59pv - (char *)prec);
    prt->papFldDes[sscanRecordD60PV]->offset = (unsigned short)((char *)&prec->d60pv - (char *)prec);
    prt->papFldDes[sscanRecordD61PV]->offset = (unsigned short)((char *)&prec->d61pv - (char *)prec);
    prt->papFldDes[sscanRecordD62PV]->offset = (unsigned short)((char *)&prec->d62pv - (char *)prec);
    prt->papFldDes[sscanRecordD63PV]->offset = (unsigned short)((char *)&prec->d63pv - (char *)prec);
    prt->papFldDes[sscanRecordD64PV]->offset = (unsigned short)((char *)&prec->d64pv - (char *)prec);
    prt->papFldDes[sscanRecordD65PV]->offset = (unsigned short)((char *)&prec->d65pv - (char *)prec);
    prt->papFldDes[sscanRecordD66PV]->offset = (unsigned short)((char *)&prec->d66pv - (char *)prec);
    prt->papFldDes[sscanRecordD67PV]->offset = (unsigned short)((char *)&prec->d67pv - (char *)prec);
    prt->papFldDes[sscanRecordD68PV]->offset = (unsigned short)((char *)&prec->d68pv - (char *)prec);
    prt->papFldDes[sscanRecordD69PV]->offset = (unsigned short)((char *)&prec->d69pv - (char *)prec);
    prt->papFldDes[sscanRecordD70PV]->offset = (unsigned short)((char *)&prec->d70pv - (char *)prec);
    prt->papFldDes[sscanRecordT1PV]->offset = (unsigned short)((char *)&prec->t1pv - (char *)prec);
    prt->papFldDes[sscanRecordT2PV]->offset = (unsigned short)((char *)&prec->t2pv - (char *)prec);
    prt->papFldDes[sscanRecordT3PV]->offset = (unsigned short)((char *)&prec->t3pv - (char *)prec);
    prt->papFldDes[sscanRecordT4PV]->offset = (unsigned short)((char *)&prec->t4pv - (char *)prec);
    prt->papFldDes[sscanRecordA1PV]->offset = (unsigned short)((char *)&prec->a1pv - (char *)prec);
    prt->papFldDes[sscanRecordBSPV]->offset = (unsigned short)((char *)&prec->bspv - (char *)prec);
    prt->papFldDes[sscanRecordASPV]->offset = (unsigned short)((char *)&prec->aspv - (char *)prec);
    prt->papFldDes[sscanRecordBSWAIT]->offset = (unsigned short)((char *)&prec->bswait - (char *)prec);
    prt->papFldDes[sscanRecordASWAIT]->offset = (unsigned short)((char *)&prec->aswait - (char *)prec);
    prt->papFldDes[sscanRecordP1NV]->offset = (unsigned short)((char *)&prec->p1nv - (char *)prec);
    prt->papFldDes[sscanRecordP2NV]->offset = (unsigned short)((char *)&prec->p2nv - (char *)prec);
    prt->papFldDes[sscanRecordP3NV]->offset = (unsigned short)((char *)&prec->p3nv - (char *)prec);
    prt->papFldDes[sscanRecordP4NV]->offset = (unsigned short)((char *)&prec->p4nv - (char *)prec);
    prt->papFldDes[sscanRecordR1NV]->offset = (unsigned short)((char *)&prec->r1nv - (char *)prec);
    prt->papFldDes[sscanRecordR2NV]->offset = (unsigned short)((char *)&prec->r2nv - (char *)prec);
    prt->papFldDes[sscanRecordR3NV]->offset = (unsigned short)((char *)&prec->r3nv - (char *)prec);
    prt->papFldDes[sscanRecordR4NV]->offset = (unsigned short)((char *)&prec->r4nv - (char *)prec);
    prt->papFldDes[sscanRecordD01NV]->offset = (unsigned short)((char *)&prec->d01nv - (char *)prec);
    prt->papFldDes[sscanRecordD02NV]->offset = (unsigned short)((char *)&prec->d02nv - (char *)prec);
    prt->papFldDes[sscanRecordD03NV]->offset = (unsigned short)((char *)&prec->d03nv - (char *)prec);
    prt->papFldDes[sscanRecordD04NV]->offset = (unsigned short)((char *)&prec->d04nv - (char *)prec);
    prt->papFldDes[sscanRecordD05NV]->offset = (unsigned short)((char *)&prec->d05nv - (char *)prec);
    prt->papFldDes[sscanRecordD06NV]->offset = (unsigned short)((char *)&prec->d06nv - (char *)prec);
    prt->papFldDes[sscanRecordD07NV]->offset = (unsigned short)((char *)&prec->d07nv - (char *)prec);
    prt->papFldDes[sscanRecordD08NV]->offset = (unsigned short)((char *)&prec->d08nv - (char *)prec);
    prt->papFldDes[sscanRecordD09NV]->offset = (unsigned short)((char *)&prec->d09nv - (char *)prec);
    prt->papFldDes[sscanRecordD10NV]->offset = (unsigned short)((char *)&prec->d10nv - (char *)prec);
    prt->papFldDes[sscanRecordD11NV]->offset = (unsigned short)((char *)&prec->d11nv - (char *)prec);
    prt->papFldDes[sscanRecordD12NV]->offset = (unsigned short)((char *)&prec->d12nv - (char *)prec);
    prt->papFldDes[sscanRecordD13NV]->offset = (unsigned short)((char *)&prec->d13nv - (char *)prec);
    prt->papFldDes[sscanRecordD14NV]->offset = (unsigned short)((char *)&prec->d14nv - (char *)prec);
    prt->papFldDes[sscanRecordD15NV]->offset = (unsigned short)((char *)&prec->d15nv - (char *)prec);
    prt->papFldDes[sscanRecordD16NV]->offset = (unsigned short)((char *)&prec->d16nv - (char *)prec);
    prt->papFldDes[sscanRecordD17NV]->offset = (unsigned short)((char *)&prec->d17nv - (char *)prec);
    prt->papFldDes[sscanRecordD18NV]->offset = (unsigned short)((char *)&prec->d18nv - (char *)prec);
    prt->papFldDes[sscanRecordD19NV]->offset = (unsigned short)((char *)&prec->d19nv - (char *)prec);
    prt->papFldDes[sscanRecordD20NV]->offset = (unsigned short)((char *)&prec->d20nv - (char *)prec);
    prt->papFldDes[sscanRecordD21NV]->offset = (unsigned short)((char *)&prec->d21nv - (char *)prec);
    prt->papFldDes[sscanRecordD22NV]->offset = (unsigned short)((char *)&prec->d22nv - (char *)prec);
    prt->papFldDes[sscanRecordD23NV]->offset = (unsigned short)((char *)&prec->d23nv - (char *)prec);
    prt->papFldDes[sscanRecordD24NV]->offset = (unsigned short)((char *)&prec->d24nv - (char *)prec);
    prt->papFldDes[sscanRecordD25NV]->offset = (unsigned short)((char *)&prec->d25nv - (char *)prec);
    prt->papFldDes[sscanRecordD26NV]->offset = (unsigned short)((char *)&prec->d26nv - (char *)prec);
    prt->papFldDes[sscanRecordD27NV]->offset = (unsigned short)((char *)&prec->d27nv - (char *)prec);
    prt->papFldDes[sscanRecordD28NV]->offset = (unsigned short)((char *)&prec->d28nv - (char *)prec);
    prt->papFldDes[sscanRecordD29NV]->offset = (unsigned short)((char *)&prec->d29nv - (char *)prec);
    prt->papFldDes[sscanRecordD30NV]->offset = (unsigned short)((char *)&prec->d30nv - (char *)prec);
    prt->papFldDes[sscanRecordD31NV]->offset = (unsigned short)((char *)&prec->d31nv - (char *)prec);
    prt->papFldDes[sscanRecordD32NV]->offset = (unsigned short)((char *)&prec->d32nv - (char *)prec);
    prt->papFldDes[sscanRecordD33NV]->offset = (unsigned short)((char *)&prec->d33nv - (char *)prec);
    prt->papFldDes[sscanRecordD34NV]->offset = (unsigned short)((char *)&prec->d34nv - (char *)prec);
    prt->papFldDes[sscanRecordD35NV]->offset = (unsigned short)((char *)&prec->d35nv - (char *)prec);
    prt->papFldDes[sscanRecordD36NV]->offset = (unsigned short)((char *)&prec->d36nv - (char *)prec);
    prt->papFldDes[sscanRecordD37NV]->offset = (unsigned short)((char *)&prec->d37nv - (char *)prec);
    prt->papFldDes[sscanRecordD38NV]->offset = (unsigned short)((char *)&prec->d38nv - (char *)prec);
    prt->papFldDes[sscanRecordD39NV]->offset = (unsigned short)((char *)&prec->d39nv - (char *)prec);
    prt->papFldDes[sscanRecordD40NV]->offset = (unsigned short)((char *)&prec->d40nv - (char *)prec);
    prt->papFldDes[sscanRecordD41NV]->offset = (unsigned short)((char *)&prec->d41nv - (char *)prec);
    prt->papFldDes[sscanRecordD42NV]->offset = (unsigned short)((char *)&prec->d42nv - (char *)prec);
    prt->papFldDes[sscanRecordD43NV]->offset = (unsigned short)((char *)&prec->d43nv - (char *)prec);
    prt->papFldDes[sscanRecordD44NV]->offset = (unsigned short)((char *)&prec->d44nv - (char *)prec);
    prt->papFldDes[sscanRecordD45NV]->offset = (unsigned short)((char *)&prec->d45nv - (char *)prec);
    prt->papFldDes[sscanRecordD46NV]->offset = (unsigned short)((char *)&prec->d46nv - (char *)prec);
    prt->papFldDes[sscanRecordD47NV]->offset = (unsigned short)((char *)&prec->d47nv - (char *)prec);
    prt->papFldDes[sscanRecordD48NV]->offset = (unsigned short)((char *)&prec->d48nv - (char *)prec);
    prt->papFldDes[sscanRecordD49NV]->offset = (unsigned short)((char *)&prec->d49nv - (char *)prec);
    prt->papFldDes[sscanRecordD50NV]->offset = (unsigned short)((char *)&prec->d50nv - (char *)prec);
    prt->papFldDes[sscanRecordD51NV]->offset = (unsigned short)((char *)&prec->d51nv - (char *)prec);
    prt->papFldDes[sscanRecordD52NV]->offset = (unsigned short)((char *)&prec->d52nv - (char *)prec);
    prt->papFldDes[sscanRecordD53NV]->offset = (unsigned short)((char *)&prec->d53nv - (char *)prec);
    prt->papFldDes[sscanRecordD54NV]->offset = (unsigned short)((char *)&prec->d54nv - (char *)prec);
    prt->papFldDes[sscanRecordD55NV]->offset = (unsigned short)((char *)&prec->d55nv - (char *)prec);
    prt->papFldDes[sscanRecordD56NV]->offset = (unsigned short)((char *)&prec->d56nv - (char *)prec);
    prt->papFldDes[sscanRecordD57NV]->offset = (unsigned short)((char *)&prec->d57nv - (char *)prec);
    prt->papFldDes[sscanRecordD58NV]->offset = (unsigned short)((char *)&prec->d58nv - (char *)prec);
    prt->papFldDes[sscanRecordD59NV]->offset = (unsigned short)((char *)&prec->d59nv - (char *)prec);
    prt->papFldDes[sscanRecordD60NV]->offset = (unsigned short)((char *)&prec->d60nv - (char *)prec);
    prt->papFldDes[sscanRecordD61NV]->offset = (unsigned short)((char *)&prec->d61nv - (char *)prec);
    prt->papFldDes[sscanRecordD62NV]->offset = (unsigned short)((char *)&prec->d62nv - (char *)prec);
    prt->papFldDes[sscanRecordD63NV]->offset = (unsigned short)((char *)&prec->d63nv - (char *)prec);
    prt->papFldDes[sscanRecordD64NV]->offset = (unsigned short)((char *)&prec->d64nv - (char *)prec);
    prt->papFldDes[sscanRecordD65NV]->offset = (unsigned short)((char *)&prec->d65nv - (char *)prec);
    prt->papFldDes[sscanRecordD66NV]->offset = (unsigned short)((char *)&prec->d66nv - (char *)prec);
    prt->papFldDes[sscanRecordD67NV]->offset = (unsigned short)((char *)&prec->d67nv - (char *)prec);
    prt->papFldDes[sscanRecordD68NV]->offset = (unsigned short)((char *)&prec->d68nv - (char *)prec);
    prt->papFldDes[sscanRecordD69NV]->offset = (unsigned short)((char *)&prec->d69nv - (char *)prec);
    prt->papFldDes[sscanRecordD70NV]->offset = (unsigned short)((char *)&prec->d70nv - (char *)prec);
    prt->papFldDes[sscanRecordT1NV]->offset = (unsigned short)((char *)&prec->t1nv - (char *)prec);
    prt->papFldDes[sscanRecordT2NV]->offset = (unsigned short)((char *)&prec->t2nv - (char *)prec);
    prt->papFldDes[sscanRecordT3NV]->offset = (unsigned short)((char *)&prec->t3nv - (char *)prec);
    prt->papFldDes[sscanRecordT4NV]->offset = (unsigned short)((char *)&prec->t4nv - (char *)prec);
    prt->papFldDes[sscanRecordA1NV]->offset = (unsigned short)((char *)&prec->a1nv - (char *)prec);
    prt->papFldDes[sscanRecordBSNV]->offset = (unsigned short)((char *)&prec->bsnv - (char *)prec);
    prt->papFldDes[sscanRecordASNV]->offset = (unsigned short)((char *)&prec->asnv - (char *)prec);
    prt->papFldDes[sscanRecordP1PP]->offset = (unsigned short)((char *)&prec->p1pp - (char *)prec);
    prt->papFldDes[sscanRecordP1CV]->offset = (unsigned short)((char *)&prec->p1cv - (char *)prec);
    prt->papFldDes[sscanRecordP1DV]->offset = (unsigned short)((char *)&prec->p1dv - (char *)prec);
    prt->papFldDes[sscanRecordP1LV]->offset = (unsigned short)((char *)&prec->p1lv - (char *)prec);
    prt->papFldDes[sscanRecordP1SP]->offset = (unsigned short)((char *)&prec->p1sp - (char *)prec);
    prt->papFldDes[sscanRecordP1SI]->offset = (unsigned short)((char *)&prec->p1si - (char *)prec);
    prt->papFldDes[sscanRecordP1EP]->offset = (unsigned short)((char *)&prec->p1ep - (char *)prec);
    prt->papFldDes[sscanRecordP1CP]->offset = (unsigned short)((char *)&prec->p1cp - (char *)prec);
    prt->papFldDes[sscanRecordP1WD]->offset = (unsigned short)((char *)&prec->p1wd - (char *)prec);
    prt->papFldDes[sscanRecordR1CV]->offset = (unsigned short)((char *)&prec->r1cv - (char *)prec);
    prt->papFldDes[sscanRecordR1LV]->offset = (unsigned short)((char *)&prec->r1lv - (char *)prec);
    prt->papFldDes[sscanRecordR1DL]->offset = (unsigned short)((char *)&prec->r1dl - (char *)prec);
    prt->papFldDes[sscanRecordP1HR]->offset = (unsigned short)((char *)&prec->p1hr - (char *)prec);
    prt->papFldDes[sscanRecordP1LR]->offset = (unsigned short)((char *)&prec->p1lr - (char *)prec);
    prt->papFldDes[sscanRecordP1PA]->offset = (unsigned short)((char *)&prec->p1pa - (char *)prec);
    prt->papFldDes[sscanRecordP1RA]->offset = (unsigned short)((char *)&prec->p1ra - (char *)prec);
    prt->papFldDes[sscanRecordP1CA]->offset = (unsigned short)((char *)&prec->p1ca - (char *)prec);
    prt->papFldDes[sscanRecordP1FS]->offset = (unsigned short)((char *)&prec->p1fs - (char *)prec);
    prt->papFldDes[sscanRecordP1FI]->offset = (unsigned short)((char *)&prec->p1fi - (char *)prec);
    prt->papFldDes[sscanRecordP1FE]->offset = (unsigned short)((char *)&prec->p1fe - (char *)prec);
    prt->papFldDes[sscanRecordP1FC]->offset = (unsigned short)((char *)&prec->p1fc - (char *)prec);
    prt->papFldDes[sscanRecordP1FW]->offset = (unsigned short)((char *)&prec->p1fw - (char *)prec);
    prt->papFldDes[sscanRecordP1SM]->offset = (unsigned short)((char *)&prec->p1sm - (char *)prec);
    prt->papFldDes[sscanRecordP1AR]->offset = (unsigned short)((char *)&prec->p1ar - (char *)prec);
    prt->papFldDes[sscanRecordP1EU]->offset = (unsigned short)((char *)&prec->p1eu - (char *)prec);
    prt->papFldDes[sscanRecordP1PR]->offset = (unsigned short)((char *)&prec->p1pr - (char *)prec);
    prt->papFldDes[sscanRecordP2PP]->offset = (unsigned short)((char *)&prec->p2pp - (char *)prec);
    prt->papFldDes[sscanRecordP2CV]->offset = (unsigned short)((char *)&prec->p2cv - (char *)prec);
    prt->papFldDes[sscanRecordP2DV]->offset = (unsigned short)((char *)&prec->p2dv - (char *)prec);
    prt->papFldDes[sscanRecordP2LV]->offset = (unsigned short)((char *)&prec->p2lv - (char *)prec);
    prt->papFldDes[sscanRecordP2SP]->offset = (unsigned short)((char *)&prec->p2sp - (char *)prec);
    prt->papFldDes[sscanRecordP2SI]->offset = (unsigned short)((char *)&prec->p2si - (char *)prec);
    prt->papFldDes[sscanRecordP2EP]->offset = (unsigned short)((char *)&prec->p2ep - (char *)prec);
    prt->papFldDes[sscanRecordP2CP]->offset = (unsigned short)((char *)&prec->p2cp - (char *)prec);
    prt->papFldDes[sscanRecordP2WD]->offset = (unsigned short)((char *)&prec->p2wd - (char *)prec);
    prt->papFldDes[sscanRecordR2CV]->offset = (unsigned short)((char *)&prec->r2cv - (char *)prec);
    prt->papFldDes[sscanRecordR2LV]->offset = (unsigned short)((char *)&prec->r2lv - (char *)prec);
    prt->papFldDes[sscanRecordR2DL]->offset = (unsigned short)((char *)&prec->r2dl - (char *)prec);
    prt->papFldDes[sscanRecordP2HR]->offset = (unsigned short)((char *)&prec->p2hr - (char *)prec);
    prt->papFldDes[sscanRecordP2LR]->offset = (unsigned short)((char *)&prec->p2lr - (char *)prec);
    prt->papFldDes[sscanRecordP2PA]->offset = (unsigned short)((char *)&prec->p2pa - (char *)prec);
    prt->papFldDes[sscanRecordP2RA]->offset = (unsigned short)((char *)&prec->p2ra - (char *)prec);
    prt->papFldDes[sscanRecordP2CA]->offset = (unsigned short)((char *)&prec->p2ca - (char *)prec);
    prt->papFldDes[sscanRecordP2FS]->offset = (unsigned short)((char *)&prec->p2fs - (char *)prec);
    prt->papFldDes[sscanRecordP2FI]->offset = (unsigned short)((char *)&prec->p2fi - (char *)prec);
    prt->papFldDes[sscanRecordP2FE]->offset = (unsigned short)((char *)&prec->p2fe - (char *)prec);
    prt->papFldDes[sscanRecordP2FC]->offset = (unsigned short)((char *)&prec->p2fc - (char *)prec);
    prt->papFldDes[sscanRecordP2FW]->offset = (unsigned short)((char *)&prec->p2fw - (char *)prec);
    prt->papFldDes[sscanRecordP2SM]->offset = (unsigned short)((char *)&prec->p2sm - (char *)prec);
    prt->papFldDes[sscanRecordP2AR]->offset = (unsigned short)((char *)&prec->p2ar - (char *)prec);
    prt->papFldDes[sscanRecordP2EU]->offset = (unsigned short)((char *)&prec->p2eu - (char *)prec);
    prt->papFldDes[sscanRecordP2PR]->offset = (unsigned short)((char *)&prec->p2pr - (char *)prec);
    prt->papFldDes[sscanRecordP3PP]->offset = (unsigned short)((char *)&prec->p3pp - (char *)prec);
    prt->papFldDes[sscanRecordP3CV]->offset = (unsigned short)((char *)&prec->p3cv - (char *)prec);
    prt->papFldDes[sscanRecordP3DV]->offset = (unsigned short)((char *)&prec->p3dv - (char *)prec);
    prt->papFldDes[sscanRecordP3LV]->offset = (unsigned short)((char *)&prec->p3lv - (char *)prec);
    prt->papFldDes[sscanRecordP3SP]->offset = (unsigned short)((char *)&prec->p3sp - (char *)prec);
    prt->papFldDes[sscanRecordP3SI]->offset = (unsigned short)((char *)&prec->p3si - (char *)prec);
    prt->papFldDes[sscanRecordP3EP]->offset = (unsigned short)((char *)&prec->p3ep - (char *)prec);
    prt->papFldDes[sscanRecordP3CP]->offset = (unsigned short)((char *)&prec->p3cp - (char *)prec);
    prt->papFldDes[sscanRecordP3WD]->offset = (unsigned short)((char *)&prec->p3wd - (char *)prec);
    prt->papFldDes[sscanRecordR3CV]->offset = (unsigned short)((char *)&prec->r3cv - (char *)prec);
    prt->papFldDes[sscanRecordR3LV]->offset = (unsigned short)((char *)&prec->r3lv - (char *)prec);
    prt->papFldDes[sscanRecordR3DL]->offset = (unsigned short)((char *)&prec->r3dl - (char *)prec);
    prt->papFldDes[sscanRecordP3HR]->offset = (unsigned short)((char *)&prec->p3hr - (char *)prec);
    prt->papFldDes[sscanRecordP3LR]->offset = (unsigned short)((char *)&prec->p3lr - (char *)prec);
    prt->papFldDes[sscanRecordP3PA]->offset = (unsigned short)((char *)&prec->p3pa - (char *)prec);
    prt->papFldDes[sscanRecordP3RA]->offset = (unsigned short)((char *)&prec->p3ra - (char *)prec);
    prt->papFldDes[sscanRecordP3CA]->offset = (unsigned short)((char *)&prec->p3ca - (char *)prec);
    prt->papFldDes[sscanRecordP3FS]->offset = (unsigned short)((char *)&prec->p3fs - (char *)prec);
    prt->papFldDes[sscanRecordP3FI]->offset = (unsigned short)((char *)&prec->p3fi - (char *)prec);
    prt->papFldDes[sscanRecordP3FE]->offset = (unsigned short)((char *)&prec->p3fe - (char *)prec);
    prt->papFldDes[sscanRecordP3FC]->offset = (unsigned short)((char *)&prec->p3fc - (char *)prec);
    prt->papFldDes[sscanRecordP3FW]->offset = (unsigned short)((char *)&prec->p3fw - (char *)prec);
    prt->papFldDes[sscanRecordP3SM]->offset = (unsigned short)((char *)&prec->p3sm - (char *)prec);
    prt->papFldDes[sscanRecordP3AR]->offset = (unsigned short)((char *)&prec->p3ar - (char *)prec);
    prt->papFldDes[sscanRecordP3EU]->offset = (unsigned short)((char *)&prec->p3eu - (char *)prec);
    prt->papFldDes[sscanRecordP3PR]->offset = (unsigned short)((char *)&prec->p3pr - (char *)prec);
    prt->papFldDes[sscanRecordP4PP]->offset = (unsigned short)((char *)&prec->p4pp - (char *)prec);
    prt->papFldDes[sscanRecordP4CV]->offset = (unsigned short)((char *)&prec->p4cv - (char *)prec);
    prt->papFldDes[sscanRecordP4DV]->offset = (unsigned short)((char *)&prec->p4dv - (char *)prec);
    prt->papFldDes[sscanRecordP4LV]->offset = (unsigned short)((char *)&prec->p4lv - (char *)prec);
    prt->papFldDes[sscanRecordP4SP]->offset = (unsigned short)((char *)&prec->p4sp - (char *)prec);
    prt->papFldDes[sscanRecordP4SI]->offset = (unsigned short)((char *)&prec->p4si - (char *)prec);
    prt->papFldDes[sscanRecordP4EP]->offset = (unsigned short)((char *)&prec->p4ep - (char *)prec);
    prt->papFldDes[sscanRecordP4CP]->offset = (unsigned short)((char *)&prec->p4cp - (char *)prec);
    prt->papFldDes[sscanRecordP4WD]->offset = (unsigned short)((char *)&prec->p4wd - (char *)prec);
    prt->papFldDes[sscanRecordR4CV]->offset = (unsigned short)((char *)&prec->r4cv - (char *)prec);
    prt->papFldDes[sscanRecordR4LV]->offset = (unsigned short)((char *)&prec->r4lv - (char *)prec);
    prt->papFldDes[sscanRecordR4DL]->offset = (unsigned short)((char *)&prec->r4dl - (char *)prec);
    prt->papFldDes[sscanRecordP4HR]->offset = (unsigned short)((char *)&prec->p4hr - (char *)prec);
    prt->papFldDes[sscanRecordP4LR]->offset = (unsigned short)((char *)&prec->p4lr - (char *)prec);
    prt->papFldDes[sscanRecordP4PA]->offset = (unsigned short)((char *)&prec->p4pa - (char *)prec);
    prt->papFldDes[sscanRecordP4RA]->offset = (unsigned short)((char *)&prec->p4ra - (char *)prec);
    prt->papFldDes[sscanRecordP4CA]->offset = (unsigned short)((char *)&prec->p4ca - (char *)prec);
    prt->papFldDes[sscanRecordP4FS]->offset = (unsigned short)((char *)&prec->p4fs - (char *)prec);
    prt->papFldDes[sscanRecordP4FI]->offset = (unsigned short)((char *)&prec->p4fi - (char *)prec);
    prt->papFldDes[sscanRecordP4FE]->offset = (unsigned short)((char *)&prec->p4fe - (char *)prec);
    prt->papFldDes[sscanRecordP4FC]->offset = (unsigned short)((char *)&prec->p4fc - (char *)prec);
    prt->papFldDes[sscanRecordP4FW]->offset = (unsigned short)((char *)&prec->p4fw - (char *)prec);
    prt->papFldDes[sscanRecordP4SM]->offset = (unsigned short)((char *)&prec->p4sm - (char *)prec);
    prt->papFldDes[sscanRecordP4AR]->offset = (unsigned short)((char *)&prec->p4ar - (char *)prec);
    prt->papFldDes[sscanRecordP4EU]->offset = (unsigned short)((char *)&prec->p4eu - (char *)prec);
    prt->papFldDes[sscanRecordP4PR]->offset = (unsigned short)((char *)&prec->p4pr - (char *)prec);
    prt->papFldDes[sscanRecordD01HR]->offset = (unsigned short)((char *)&prec->d01hr - (char *)prec);
    prt->papFldDes[sscanRecordD01LR]->offset = (unsigned short)((char *)&prec->d01lr - (char *)prec);
    prt->papFldDes[sscanRecordD01DA]->offset = (unsigned short)((char *)&prec->d01da - (char *)prec);
    prt->papFldDes[sscanRecordD01CA]->offset = (unsigned short)((char *)&prec->d01ca - (char *)prec);
    prt->papFldDes[sscanRecordD01CV]->offset = (unsigned short)((char *)&prec->d01cv - (char *)prec);
    prt->papFldDes[sscanRecordD01LV]->offset = (unsigned short)((char *)&prec->d01lv - (char *)prec);
    prt->papFldDes[sscanRecordD01NE]->offset = (unsigned short)((char *)&prec->d01ne - (char *)prec);
    prt->papFldDes[sscanRecordD01EU]->offset = (unsigned short)((char *)&prec->d01eu - (char *)prec);
    prt->papFldDes[sscanRecordD01PR]->offset = (unsigned short)((char *)&prec->d01pr - (char *)prec);
    prt->papFldDes[sscanRecordD02HR]->offset = (unsigned short)((char *)&prec->d02hr - (char *)prec);
    prt->papFldDes[sscanRecordD02LR]->offset = (unsigned short)((char *)&prec->d02lr - (char *)prec);
    prt->papFldDes[sscanRecordD02DA]->offset = (unsigned short)((char *)&prec->d02da - (char *)prec);
    prt->papFldDes[sscanRecordD02CA]->offset = (unsigned short)((char *)&prec->d02ca - (char *)prec);
    prt->papFldDes[sscanRecordD02CV]->offset = (unsigned short)((char *)&prec->d02cv - (char *)prec);
    prt->papFldDes[sscanRecordD02LV]->offset = (unsigned short)((char *)&prec->d02lv - (char *)prec);
    prt->papFldDes[sscanRecordD02NE]->offset = (unsigned short)((char *)&prec->d02ne - (char *)prec);
    prt->papFldDes[sscanRecordD02EU]->offset = (unsigned short)((char *)&prec->d02eu - (char *)prec);
    prt->papFldDes[sscanRecordD02PR]->offset = (unsigned short)((char *)&prec->d02pr - (char *)prec);
    prt->papFldDes[sscanRecordD03HR]->offset = (unsigned short)((char *)&prec->d03hr - (char *)prec);
    prt->papFldDes[sscanRecordD03LR]->offset = (unsigned short)((char *)&prec->d03lr - (char *)prec);
    prt->papFldDes[sscanRecordD03DA]->offset = (unsigned short)((char *)&prec->d03da - (char *)prec);
    prt->papFldDes[sscanRecordD03CA]->offset = (unsigned short)((char *)&prec->d03ca - (char *)prec);
    prt->papFldDes[sscanRecordD03CV]->offset = (unsigned short)((char *)&prec->d03cv - (char *)prec);
    prt->papFldDes[sscanRecordD03LV]->offset = (unsigned short)((char *)&prec->d03lv - (char *)prec);
    prt->papFldDes[sscanRecordD03NE]->offset = (unsigned short)((char *)&prec->d03ne - (char *)prec);
    prt->papFldDes[sscanRecordD03EU]->offset = (unsigned short)((char *)&prec->d03eu - (char *)prec);
    prt->papFldDes[sscanRecordD03PR]->offset = (unsigned short)((char *)&prec->d03pr - (char *)prec);
    prt->papFldDes[sscanRecordD04HR]->offset = (unsigned short)((char *)&prec->d04hr - (char *)prec);
    prt->papFldDes[sscanRecordD04LR]->offset = (unsigned short)((char *)&prec->d04lr - (char *)prec);
    prt->papFldDes[sscanRecordD04DA]->offset = (unsigned short)((char *)&prec->d04da - (char *)prec);
    prt->papFldDes[sscanRecordD04CA]->offset = (unsigned short)((char *)&prec->d04ca - (char *)prec);
    prt->papFldDes[sscanRecordD04CV]->offset = (unsigned short)((char *)&prec->d04cv - (char *)prec);
    prt->papFldDes[sscanRecordD04LV]->offset = (unsigned short)((char *)&prec->d04lv - (char *)prec);
    prt->papFldDes[sscanRecordD04NE]->offset = (unsigned short)((char *)&prec->d04ne - (char *)prec);
    prt->papFldDes[sscanRecordD04EU]->offset = (unsigned short)((char *)&prec->d04eu - (char *)prec);
    prt->papFldDes[sscanRecordD04PR]->offset = (unsigned short)((char *)&prec->d04pr - (char *)prec);
    prt->papFldDes[sscanRecordD05HR]->offset = (unsigned short)((char *)&prec->d05hr - (char *)prec);
    prt->papFldDes[sscanRecordD05LR]->offset = (unsigned short)((char *)&prec->d05lr - (char *)prec);
    prt->papFldDes[sscanRecordD05DA]->offset = (unsigned short)((char *)&prec->d05da - (char *)prec);
    prt->papFldDes[sscanRecordD05CA]->offset = (unsigned short)((char *)&prec->d05ca - (char *)prec);
    prt->papFldDes[sscanRecordD05CV]->offset = (unsigned short)((char *)&prec->d05cv - (char *)prec);
    prt->papFldDes[sscanRecordD05LV]->offset = (unsigned short)((char *)&prec->d05lv - (char *)prec);
    prt->papFldDes[sscanRecordD05NE]->offset = (unsigned short)((char *)&prec->d05ne - (char *)prec);
    prt->papFldDes[sscanRecordD05EU]->offset = (unsigned short)((char *)&prec->d05eu - (char *)prec);
    prt->papFldDes[sscanRecordD05PR]->offset = (unsigned short)((char *)&prec->d05pr - (char *)prec);
    prt->papFldDes[sscanRecordD06HR]->offset = (unsigned short)((char *)&prec->d06hr - (char *)prec);
    prt->papFldDes[sscanRecordD06LR]->offset = (unsigned short)((char *)&prec->d06lr - (char *)prec);
    prt->papFldDes[sscanRecordD06DA]->offset = (unsigned short)((char *)&prec->d06da - (char *)prec);
    prt->papFldDes[sscanRecordD06CA]->offset = (unsigned short)((char *)&prec->d06ca - (char *)prec);
    prt->papFldDes[sscanRecordD06CV]->offset = (unsigned short)((char *)&prec->d06cv - (char *)prec);
    prt->papFldDes[sscanRecordD06LV]->offset = (unsigned short)((char *)&prec->d06lv - (char *)prec);
    prt->papFldDes[sscanRecordD06NE]->offset = (unsigned short)((char *)&prec->d06ne - (char *)prec);
    prt->papFldDes[sscanRecordD06EU]->offset = (unsigned short)((char *)&prec->d06eu - (char *)prec);
    prt->papFldDes[sscanRecordD06PR]->offset = (unsigned short)((char *)&prec->d06pr - (char *)prec);
    prt->papFldDes[sscanRecordD07HR]->offset = (unsigned short)((char *)&prec->d07hr - (char *)prec);
    prt->papFldDes[sscanRecordD07LR]->offset = (unsigned short)((char *)&prec->d07lr - (char *)prec);
    prt->papFldDes[sscanRecordD07DA]->offset = (unsigned short)((char *)&prec->d07da - (char *)prec);
    prt->papFldDes[sscanRecordD07CA]->offset = (unsigned short)((char *)&prec->d07ca - (char *)prec);
    prt->papFldDes[sscanRecordD07CV]->offset = (unsigned short)((char *)&prec->d07cv - (char *)prec);
    prt->papFldDes[sscanRecordD07LV]->offset = (unsigned short)((char *)&prec->d07lv - (char *)prec);
    prt->papFldDes[sscanRecordD07NE]->offset = (unsigned short)((char *)&prec->d07ne - (char *)prec);
    prt->papFldDes[sscanRecordD07EU]->offset = (unsigned short)((char *)&prec->d07eu - (char *)prec);
    prt->papFldDes[sscanRecordD07PR]->offset = (unsigned short)((char *)&prec->d07pr - (char *)prec);
    prt->papFldDes[sscanRecordD08HR]->offset = (unsigned short)((char *)&prec->d08hr - (char *)prec);
    prt->papFldDes[sscanRecordD08LR]->offset = (unsigned short)((char *)&prec->d08lr - (char *)prec);
    prt->papFldDes[sscanRecordD08DA]->offset = (unsigned short)((char *)&prec->d08da - (char *)prec);
    prt->papFldDes[sscanRecordD08CA]->offset = (unsigned short)((char *)&prec->d08ca - (char *)prec);
    prt->papFldDes[sscanRecordD08CV]->offset = (unsigned short)((char *)&prec->d08cv - (char *)prec);
    prt->papFldDes[sscanRecordD08LV]->offset = (unsigned short)((char *)&prec->d08lv - (char *)prec);
    prt->papFldDes[sscanRecordD08NE]->offset = (unsigned short)((char *)&prec->d08ne - (char *)prec);
    prt->papFldDes[sscanRecordD08EU]->offset = (unsigned short)((char *)&prec->d08eu - (char *)prec);
    prt->papFldDes[sscanRecordD08PR]->offset = (unsigned short)((char *)&prec->d08pr - (char *)prec);
    prt->papFldDes[sscanRecordD09HR]->offset = (unsigned short)((char *)&prec->d09hr - (char *)prec);
    prt->papFldDes[sscanRecordD09LR]->offset = (unsigned short)((char *)&prec->d09lr - (char *)prec);
    prt->papFldDes[sscanRecordD09DA]->offset = (unsigned short)((char *)&prec->d09da - (char *)prec);
    prt->papFldDes[sscanRecordD09CA]->offset = (unsigned short)((char *)&prec->d09ca - (char *)prec);
    prt->papFldDes[sscanRecordD09CV]->offset = (unsigned short)((char *)&prec->d09cv - (char *)prec);
    prt->papFldDes[sscanRecordD09LV]->offset = (unsigned short)((char *)&prec->d09lv - (char *)prec);
    prt->papFldDes[sscanRecordD09NE]->offset = (unsigned short)((char *)&prec->d09ne - (char *)prec);
    prt->papFldDes[sscanRecordD09EU]->offset = (unsigned short)((char *)&prec->d09eu - (char *)prec);
    prt->papFldDes[sscanRecordD09PR]->offset = (unsigned short)((char *)&prec->d09pr - (char *)prec);
    prt->papFldDes[sscanRecordD10HR]->offset = (unsigned short)((char *)&prec->d10hr - (char *)prec);
    prt->papFldDes[sscanRecordD10LR]->offset = (unsigned short)((char *)&prec->d10lr - (char *)prec);
    prt->papFldDes[sscanRecordD10DA]->offset = (unsigned short)((char *)&prec->d10da - (char *)prec);
    prt->papFldDes[sscanRecordD10CA]->offset = (unsigned short)((char *)&prec->d10ca - (char *)prec);
    prt->papFldDes[sscanRecordD10CV]->offset = (unsigned short)((char *)&prec->d10cv - (char *)prec);
    prt->papFldDes[sscanRecordD10LV]->offset = (unsigned short)((char *)&prec->d10lv - (char *)prec);
    prt->papFldDes[sscanRecordD10NE]->offset = (unsigned short)((char *)&prec->d10ne - (char *)prec);
    prt->papFldDes[sscanRecordD10EU]->offset = (unsigned short)((char *)&prec->d10eu - (char *)prec);
    prt->papFldDes[sscanRecordD10PR]->offset = (unsigned short)((char *)&prec->d10pr - (char *)prec);
    prt->papFldDes[sscanRecordD11HR]->offset = (unsigned short)((char *)&prec->d11hr - (char *)prec);
    prt->papFldDes[sscanRecordD11LR]->offset = (unsigned short)((char *)&prec->d11lr - (char *)prec);
    prt->papFldDes[sscanRecordD11DA]->offset = (unsigned short)((char *)&prec->d11da - (char *)prec);
    prt->papFldDes[sscanRecordD11CA]->offset = (unsigned short)((char *)&prec->d11ca - (char *)prec);
    prt->papFldDes[sscanRecordD11CV]->offset = (unsigned short)((char *)&prec->d11cv - (char *)prec);
    prt->papFldDes[sscanRecordD11LV]->offset = (unsigned short)((char *)&prec->d11lv - (char *)prec);
    prt->papFldDes[sscanRecordD11NE]->offset = (unsigned short)((char *)&prec->d11ne - (char *)prec);
    prt->papFldDes[sscanRecordD11EU]->offset = (unsigned short)((char *)&prec->d11eu - (char *)prec);
    prt->papFldDes[sscanRecordD11PR]->offset = (unsigned short)((char *)&prec->d11pr - (char *)prec);
    prt->papFldDes[sscanRecordD12HR]->offset = (unsigned short)((char *)&prec->d12hr - (char *)prec);
    prt->papFldDes[sscanRecordD12LR]->offset = (unsigned short)((char *)&prec->d12lr - (char *)prec);
    prt->papFldDes[sscanRecordD12DA]->offset = (unsigned short)((char *)&prec->d12da - (char *)prec);
    prt->papFldDes[sscanRecordD12CA]->offset = (unsigned short)((char *)&prec->d12ca - (char *)prec);
    prt->papFldDes[sscanRecordD12CV]->offset = (unsigned short)((char *)&prec->d12cv - (char *)prec);
    prt->papFldDes[sscanRecordD12LV]->offset = (unsigned short)((char *)&prec->d12lv - (char *)prec);
    prt->papFldDes[sscanRecordD12NE]->offset = (unsigned short)((char *)&prec->d12ne - (char *)prec);
    prt->papFldDes[sscanRecordD12EU]->offset = (unsigned short)((char *)&prec->d12eu - (char *)prec);
    prt->papFldDes[sscanRecordD12PR]->offset = (unsigned short)((char *)&prec->d12pr - (char *)prec);
    prt->papFldDes[sscanRecordD13HR]->offset = (unsigned short)((char *)&prec->d13hr - (char *)prec);
    prt->papFldDes[sscanRecordD13LR]->offset = (unsigned short)((char *)&prec->d13lr - (char *)prec);
    prt->papFldDes[sscanRecordD13DA]->offset = (unsigned short)((char *)&prec->d13da - (char *)prec);
    prt->papFldDes[sscanRecordD13CA]->offset = (unsigned short)((char *)&prec->d13ca - (char *)prec);
    prt->papFldDes[sscanRecordD13CV]->offset = (unsigned short)((char *)&prec->d13cv - (char *)prec);
    prt->papFldDes[sscanRecordD13LV]->offset = (unsigned short)((char *)&prec->d13lv - (char *)prec);
    prt->papFldDes[sscanRecordD13NE]->offset = (unsigned short)((char *)&prec->d13ne - (char *)prec);
    prt->papFldDes[sscanRecordD13EU]->offset = (unsigned short)((char *)&prec->d13eu - (char *)prec);
    prt->papFldDes[sscanRecordD13PR]->offset = (unsigned short)((char *)&prec->d13pr - (char *)prec);
    prt->papFldDes[sscanRecordD14HR]->offset = (unsigned short)((char *)&prec->d14hr - (char *)prec);
    prt->papFldDes[sscanRecordD14LR]->offset = (unsigned short)((char *)&prec->d14lr - (char *)prec);
    prt->papFldDes[sscanRecordD14DA]->offset = (unsigned short)((char *)&prec->d14da - (char *)prec);
    prt->papFldDes[sscanRecordD14CA]->offset = (unsigned short)((char *)&prec->d14ca - (char *)prec);
    prt->papFldDes[sscanRecordD14CV]->offset = (unsigned short)((char *)&prec->d14cv - (char *)prec);
    prt->papFldDes[sscanRecordD14LV]->offset = (unsigned short)((char *)&prec->d14lv - (char *)prec);
    prt->papFldDes[sscanRecordD14NE]->offset = (unsigned short)((char *)&prec->d14ne - (char *)prec);
    prt->papFldDes[sscanRecordD14EU]->offset = (unsigned short)((char *)&prec->d14eu - (char *)prec);
    prt->papFldDes[sscanRecordD14PR]->offset = (unsigned short)((char *)&prec->d14pr - (char *)prec);
    prt->papFldDes[sscanRecordD15HR]->offset = (unsigned short)((char *)&prec->d15hr - (char *)prec);
    prt->papFldDes[sscanRecordD15LR]->offset = (unsigned short)((char *)&prec->d15lr - (char *)prec);
    prt->papFldDes[sscanRecordD15DA]->offset = (unsigned short)((char *)&prec->d15da - (char *)prec);
    prt->papFldDes[sscanRecordD15CA]->offset = (unsigned short)((char *)&prec->d15ca - (char *)prec);
    prt->papFldDes[sscanRecordD15CV]->offset = (unsigned short)((char *)&prec->d15cv - (char *)prec);
    prt->papFldDes[sscanRecordD15LV]->offset = (unsigned short)((char *)&prec->d15lv - (char *)prec);
    prt->papFldDes[sscanRecordD15NE]->offset = (unsigned short)((char *)&prec->d15ne - (char *)prec);
    prt->papFldDes[sscanRecordD15EU]->offset = (unsigned short)((char *)&prec->d15eu - (char *)prec);
    prt->papFldDes[sscanRecordD15PR]->offset = (unsigned short)((char *)&prec->d15pr - (char *)prec);
    prt->papFldDes[sscanRecordD16HR]->offset = (unsigned short)((char *)&prec->d16hr - (char *)prec);
    prt->papFldDes[sscanRecordD16LR]->offset = (unsigned short)((char *)&prec->d16lr - (char *)prec);
    prt->papFldDes[sscanRecordD16DA]->offset = (unsigned short)((char *)&prec->d16da - (char *)prec);
    prt->papFldDes[sscanRecordD16CA]->offset = (unsigned short)((char *)&prec->d16ca - (char *)prec);
    prt->papFldDes[sscanRecordD16CV]->offset = (unsigned short)((char *)&prec->d16cv - (char *)prec);
    prt->papFldDes[sscanRecordD16LV]->offset = (unsigned short)((char *)&prec->d16lv - (char *)prec);
    prt->papFldDes[sscanRecordD16NE]->offset = (unsigned short)((char *)&prec->d16ne - (char *)prec);
    prt->papFldDes[sscanRecordD16EU]->offset = (unsigned short)((char *)&prec->d16eu - (char *)prec);
    prt->papFldDes[sscanRecordD16PR]->offset = (unsigned short)((char *)&prec->d16pr - (char *)prec);
    prt->papFldDes[sscanRecordD17HR]->offset = (unsigned short)((char *)&prec->d17hr - (char *)prec);
    prt->papFldDes[sscanRecordD17LR]->offset = (unsigned short)((char *)&prec->d17lr - (char *)prec);
    prt->papFldDes[sscanRecordD17DA]->offset = (unsigned short)((char *)&prec->d17da - (char *)prec);
    prt->papFldDes[sscanRecordD17CA]->offset = (unsigned short)((char *)&prec->d17ca - (char *)prec);
    prt->papFldDes[sscanRecordD17CV]->offset = (unsigned short)((char *)&prec->d17cv - (char *)prec);
    prt->papFldDes[sscanRecordD17LV]->offset = (unsigned short)((char *)&prec->d17lv - (char *)prec);
    prt->papFldDes[sscanRecordD17NE]->offset = (unsigned short)((char *)&prec->d17ne - (char *)prec);
    prt->papFldDes[sscanRecordD17EU]->offset = (unsigned short)((char *)&prec->d17eu - (char *)prec);
    prt->papFldDes[sscanRecordD17PR]->offset = (unsigned short)((char *)&prec->d17pr - (char *)prec);
    prt->papFldDes[sscanRecordD18HR]->offset = (unsigned short)((char *)&prec->d18hr - (char *)prec);
    prt->papFldDes[sscanRecordD18LR]->offset = (unsigned short)((char *)&prec->d18lr - (char *)prec);
    prt->papFldDes[sscanRecordD18DA]->offset = (unsigned short)((char *)&prec->d18da - (char *)prec);
    prt->papFldDes[sscanRecordD18CA]->offset = (unsigned short)((char *)&prec->d18ca - (char *)prec);
    prt->papFldDes[sscanRecordD18CV]->offset = (unsigned short)((char *)&prec->d18cv - (char *)prec);
    prt->papFldDes[sscanRecordD18LV]->offset = (unsigned short)((char *)&prec->d18lv - (char *)prec);
    prt->papFldDes[sscanRecordD18NE]->offset = (unsigned short)((char *)&prec->d18ne - (char *)prec);
    prt->papFldDes[sscanRecordD18EU]->offset = (unsigned short)((char *)&prec->d18eu - (char *)prec);
    prt->papFldDes[sscanRecordD18PR]->offset = (unsigned short)((char *)&prec->d18pr - (char *)prec);
    prt->papFldDes[sscanRecordD19HR]->offset = (unsigned short)((char *)&prec->d19hr - (char *)prec);
    prt->papFldDes[sscanRecordD19LR]->offset = (unsigned short)((char *)&prec->d19lr - (char *)prec);
    prt->papFldDes[sscanRecordD19DA]->offset = (unsigned short)((char *)&prec->d19da - (char *)prec);
    prt->papFldDes[sscanRecordD19CA]->offset = (unsigned short)((char *)&prec->d19ca - (char *)prec);
    prt->papFldDes[sscanRecordD19CV]->offset = (unsigned short)((char *)&prec->d19cv - (char *)prec);
    prt->papFldDes[sscanRecordD19LV]->offset = (unsigned short)((char *)&prec->d19lv - (char *)prec);
    prt->papFldDes[sscanRecordD19NE]->offset = (unsigned short)((char *)&prec->d19ne - (char *)prec);
    prt->papFldDes[sscanRecordD19EU]->offset = (unsigned short)((char *)&prec->d19eu - (char *)prec);
    prt->papFldDes[sscanRecordD19PR]->offset = (unsigned short)((char *)&prec->d19pr - (char *)prec);
    prt->papFldDes[sscanRecordD20HR]->offset = (unsigned short)((char *)&prec->d20hr - (char *)prec);
    prt->papFldDes[sscanRecordD20LR]->offset = (unsigned short)((char *)&prec->d20lr - (char *)prec);
    prt->papFldDes[sscanRecordD20DA]->offset = (unsigned short)((char *)&prec->d20da - (char *)prec);
    prt->papFldDes[sscanRecordD20CA]->offset = (unsigned short)((char *)&prec->d20ca - (char *)prec);
    prt->papFldDes[sscanRecordD20CV]->offset = (unsigned short)((char *)&prec->d20cv - (char *)prec);
    prt->papFldDes[sscanRecordD20LV]->offset = (unsigned short)((char *)&prec->d20lv - (char *)prec);
    prt->papFldDes[sscanRecordD20NE]->offset = (unsigned short)((char *)&prec->d20ne - (char *)prec);
    prt->papFldDes[sscanRecordD20EU]->offset = (unsigned short)((char *)&prec->d20eu - (char *)prec);
    prt->papFldDes[sscanRecordD20PR]->offset = (unsigned short)((char *)&prec->d20pr - (char *)prec);
    prt->papFldDes[sscanRecordD21HR]->offset = (unsigned short)((char *)&prec->d21hr - (char *)prec);
    prt->papFldDes[sscanRecordD21LR]->offset = (unsigned short)((char *)&prec->d21lr - (char *)prec);
    prt->papFldDes[sscanRecordD21DA]->offset = (unsigned short)((char *)&prec->d21da - (char *)prec);
    prt->papFldDes[sscanRecordD21CA]->offset = (unsigned short)((char *)&prec->d21ca - (char *)prec);
    prt->papFldDes[sscanRecordD21CV]->offset = (unsigned short)((char *)&prec->d21cv - (char *)prec);
    prt->papFldDes[sscanRecordD21LV]->offset = (unsigned short)((char *)&prec->d21lv - (char *)prec);
    prt->papFldDes[sscanRecordD21NE]->offset = (unsigned short)((char *)&prec->d21ne - (char *)prec);
    prt->papFldDes[sscanRecordD21EU]->offset = (unsigned short)((char *)&prec->d21eu - (char *)prec);
    prt->papFldDes[sscanRecordD21PR]->offset = (unsigned short)((char *)&prec->d21pr - (char *)prec);
    prt->papFldDes[sscanRecordD22HR]->offset = (unsigned short)((char *)&prec->d22hr - (char *)prec);
    prt->papFldDes[sscanRecordD22LR]->offset = (unsigned short)((char *)&prec->d22lr - (char *)prec);
    prt->papFldDes[sscanRecordD22DA]->offset = (unsigned short)((char *)&prec->d22da - (char *)prec);
    prt->papFldDes[sscanRecordD22CA]->offset = (unsigned short)((char *)&prec->d22ca - (char *)prec);
    prt->papFldDes[sscanRecordD22CV]->offset = (unsigned short)((char *)&prec->d22cv - (char *)prec);
    prt->papFldDes[sscanRecordD22LV]->offset = (unsigned short)((char *)&prec->d22lv - (char *)prec);
    prt->papFldDes[sscanRecordD22NE]->offset = (unsigned short)((char *)&prec->d22ne - (char *)prec);
    prt->papFldDes[sscanRecordD22EU]->offset = (unsigned short)((char *)&prec->d22eu - (char *)prec);
    prt->papFldDes[sscanRecordD22PR]->offset = (unsigned short)((char *)&prec->d22pr - (char *)prec);
    prt->papFldDes[sscanRecordD23HR]->offset = (unsigned short)((char *)&prec->d23hr - (char *)prec);
    prt->papFldDes[sscanRecordD23LR]->offset = (unsigned short)((char *)&prec->d23lr - (char *)prec);
    prt->papFldDes[sscanRecordD23DA]->offset = (unsigned short)((char *)&prec->d23da - (char *)prec);
    prt->papFldDes[sscanRecordD23CA]->offset = (unsigned short)((char *)&prec->d23ca - (char *)prec);
    prt->papFldDes[sscanRecordD23CV]->offset = (unsigned short)((char *)&prec->d23cv - (char *)prec);
    prt->papFldDes[sscanRecordD23LV]->offset = (unsigned short)((char *)&prec->d23lv - (char *)prec);
    prt->papFldDes[sscanRecordD23NE]->offset = (unsigned short)((char *)&prec->d23ne - (char *)prec);
    prt->papFldDes[sscanRecordD23EU]->offset = (unsigned short)((char *)&prec->d23eu - (char *)prec);
    prt->papFldDes[sscanRecordD23PR]->offset = (unsigned short)((char *)&prec->d23pr - (char *)prec);
    prt->papFldDes[sscanRecordD24HR]->offset = (unsigned short)((char *)&prec->d24hr - (char *)prec);
    prt->papFldDes[sscanRecordD24LR]->offset = (unsigned short)((char *)&prec->d24lr - (char *)prec);
    prt->papFldDes[sscanRecordD24DA]->offset = (unsigned short)((char *)&prec->d24da - (char *)prec);
    prt->papFldDes[sscanRecordD24CA]->offset = (unsigned short)((char *)&prec->d24ca - (char *)prec);
    prt->papFldDes[sscanRecordD24CV]->offset = (unsigned short)((char *)&prec->d24cv - (char *)prec);
    prt->papFldDes[sscanRecordD24LV]->offset = (unsigned short)((char *)&prec->d24lv - (char *)prec);
    prt->papFldDes[sscanRecordD24NE]->offset = (unsigned short)((char *)&prec->d24ne - (char *)prec);
    prt->papFldDes[sscanRecordD24EU]->offset = (unsigned short)((char *)&prec->d24eu - (char *)prec);
    prt->papFldDes[sscanRecordD24PR]->offset = (unsigned short)((char *)&prec->d24pr - (char *)prec);
    prt->papFldDes[sscanRecordD25HR]->offset = (unsigned short)((char *)&prec->d25hr - (char *)prec);
    prt->papFldDes[sscanRecordD25LR]->offset = (unsigned short)((char *)&prec->d25lr - (char *)prec);
    prt->papFldDes[sscanRecordD25DA]->offset = (unsigned short)((char *)&prec->d25da - (char *)prec);
    prt->papFldDes[sscanRecordD25CA]->offset = (unsigned short)((char *)&prec->d25ca - (char *)prec);
    prt->papFldDes[sscanRecordD25CV]->offset = (unsigned short)((char *)&prec->d25cv - (char *)prec);
    prt->papFldDes[sscanRecordD25LV]->offset = (unsigned short)((char *)&prec->d25lv - (char *)prec);
    prt->papFldDes[sscanRecordD25NE]->offset = (unsigned short)((char *)&prec->d25ne - (char *)prec);
    prt->papFldDes[sscanRecordD25EU]->offset = (unsigned short)((char *)&prec->d25eu - (char *)prec);
    prt->papFldDes[sscanRecordD25PR]->offset = (unsigned short)((char *)&prec->d25pr - (char *)prec);
    prt->papFldDes[sscanRecordD26HR]->offset = (unsigned short)((char *)&prec->d26hr - (char *)prec);
    prt->papFldDes[sscanRecordD26LR]->offset = (unsigned short)((char *)&prec->d26lr - (char *)prec);
    prt->papFldDes[sscanRecordD26DA]->offset = (unsigned short)((char *)&prec->d26da - (char *)prec);
    prt->papFldDes[sscanRecordD26CA]->offset = (unsigned short)((char *)&prec->d26ca - (char *)prec);
    prt->papFldDes[sscanRecordD26CV]->offset = (unsigned short)((char *)&prec->d26cv - (char *)prec);
    prt->papFldDes[sscanRecordD26LV]->offset = (unsigned short)((char *)&prec->d26lv - (char *)prec);
    prt->papFldDes[sscanRecordD26NE]->offset = (unsigned short)((char *)&prec->d26ne - (char *)prec);
    prt->papFldDes[sscanRecordD26EU]->offset = (unsigned short)((char *)&prec->d26eu - (char *)prec);
    prt->papFldDes[sscanRecordD26PR]->offset = (unsigned short)((char *)&prec->d26pr - (char *)prec);
    prt->papFldDes[sscanRecordD27HR]->offset = (unsigned short)((char *)&prec->d27hr - (char *)prec);
    prt->papFldDes[sscanRecordD27LR]->offset = (unsigned short)((char *)&prec->d27lr - (char *)prec);
    prt->papFldDes[sscanRecordD27DA]->offset = (unsigned short)((char *)&prec->d27da - (char *)prec);
    prt->papFldDes[sscanRecordD27CA]->offset = (unsigned short)((char *)&prec->d27ca - (char *)prec);
    prt->papFldDes[sscanRecordD27CV]->offset = (unsigned short)((char *)&prec->d27cv - (char *)prec);
    prt->papFldDes[sscanRecordD27LV]->offset = (unsigned short)((char *)&prec->d27lv - (char *)prec);
    prt->papFldDes[sscanRecordD27NE]->offset = (unsigned short)((char *)&prec->d27ne - (char *)prec);
    prt->papFldDes[sscanRecordD27EU]->offset = (unsigned short)((char *)&prec->d27eu - (char *)prec);
    prt->papFldDes[sscanRecordD27PR]->offset = (unsigned short)((char *)&prec->d27pr - (char *)prec);
    prt->papFldDes[sscanRecordD28HR]->offset = (unsigned short)((char *)&prec->d28hr - (char *)prec);
    prt->papFldDes[sscanRecordD28LR]->offset = (unsigned short)((char *)&prec->d28lr - (char *)prec);
    prt->papFldDes[sscanRecordD28DA]->offset = (unsigned short)((char *)&prec->d28da - (char *)prec);
    prt->papFldDes[sscanRecordD28CA]->offset = (unsigned short)((char *)&prec->d28ca - (char *)prec);
    prt->papFldDes[sscanRecordD28CV]->offset = (unsigned short)((char *)&prec->d28cv - (char *)prec);
    prt->papFldDes[sscanRecordD28LV]->offset = (unsigned short)((char *)&prec->d28lv - (char *)prec);
    prt->papFldDes[sscanRecordD28NE]->offset = (unsigned short)((char *)&prec->d28ne - (char *)prec);
    prt->papFldDes[sscanRecordD28EU]->offset = (unsigned short)((char *)&prec->d28eu - (char *)prec);
    prt->papFldDes[sscanRecordD28PR]->offset = (unsigned short)((char *)&prec->d28pr - (char *)prec);
    prt->papFldDes[sscanRecordD29HR]->offset = (unsigned short)((char *)&prec->d29hr - (char *)prec);
    prt->papFldDes[sscanRecordD29LR]->offset = (unsigned short)((char *)&prec->d29lr - (char *)prec);
    prt->papFldDes[sscanRecordD29DA]->offset = (unsigned short)((char *)&prec->d29da - (char *)prec);
    prt->papFldDes[sscanRecordD29CA]->offset = (unsigned short)((char *)&prec->d29ca - (char *)prec);
    prt->papFldDes[sscanRecordD29CV]->offset = (unsigned short)((char *)&prec->d29cv - (char *)prec);
    prt->papFldDes[sscanRecordD29LV]->offset = (unsigned short)((char *)&prec->d29lv - (char *)prec);
    prt->papFldDes[sscanRecordD29NE]->offset = (unsigned short)((char *)&prec->d29ne - (char *)prec);
    prt->papFldDes[sscanRecordD29EU]->offset = (unsigned short)((char *)&prec->d29eu - (char *)prec);
    prt->papFldDes[sscanRecordD29PR]->offset = (unsigned short)((char *)&prec->d29pr - (char *)prec);
    prt->papFldDes[sscanRecordD30HR]->offset = (unsigned short)((char *)&prec->d30hr - (char *)prec);
    prt->papFldDes[sscanRecordD30LR]->offset = (unsigned short)((char *)&prec->d30lr - (char *)prec);
    prt->papFldDes[sscanRecordD30DA]->offset = (unsigned short)((char *)&prec->d30da - (char *)prec);
    prt->papFldDes[sscanRecordD30CA]->offset = (unsigned short)((char *)&prec->d30ca - (char *)prec);
    prt->papFldDes[sscanRecordD30CV]->offset = (unsigned short)((char *)&prec->d30cv - (char *)prec);
    prt->papFldDes[sscanRecordD30LV]->offset = (unsigned short)((char *)&prec->d30lv - (char *)prec);
    prt->papFldDes[sscanRecordD30NE]->offset = (unsigned short)((char *)&prec->d30ne - (char *)prec);
    prt->papFldDes[sscanRecordD30EU]->offset = (unsigned short)((char *)&prec->d30eu - (char *)prec);
    prt->papFldDes[sscanRecordD30PR]->offset = (unsigned short)((char *)&prec->d30pr - (char *)prec);
    prt->papFldDes[sscanRecordD31HR]->offset = (unsigned short)((char *)&prec->d31hr - (char *)prec);
    prt->papFldDes[sscanRecordD31LR]->offset = (unsigned short)((char *)&prec->d31lr - (char *)prec);
    prt->papFldDes[sscanRecordD31DA]->offset = (unsigned short)((char *)&prec->d31da - (char *)prec);
    prt->papFldDes[sscanRecordD31CA]->offset = (unsigned short)((char *)&prec->d31ca - (char *)prec);
    prt->papFldDes[sscanRecordD31CV]->offset = (unsigned short)((char *)&prec->d31cv - (char *)prec);
    prt->papFldDes[sscanRecordD31LV]->offset = (unsigned short)((char *)&prec->d31lv - (char *)prec);
    prt->papFldDes[sscanRecordD31NE]->offset = (unsigned short)((char *)&prec->d31ne - (char *)prec);
    prt->papFldDes[sscanRecordD31EU]->offset = (unsigned short)((char *)&prec->d31eu - (char *)prec);
    prt->papFldDes[sscanRecordD31PR]->offset = (unsigned short)((char *)&prec->d31pr - (char *)prec);
    prt->papFldDes[sscanRecordD32HR]->offset = (unsigned short)((char *)&prec->d32hr - (char *)prec);
    prt->papFldDes[sscanRecordD32LR]->offset = (unsigned short)((char *)&prec->d32lr - (char *)prec);
    prt->papFldDes[sscanRecordD32DA]->offset = (unsigned short)((char *)&prec->d32da - (char *)prec);
    prt->papFldDes[sscanRecordD32CA]->offset = (unsigned short)((char *)&prec->d32ca - (char *)prec);
    prt->papFldDes[sscanRecordD32CV]->offset = (unsigned short)((char *)&prec->d32cv - (char *)prec);
    prt->papFldDes[sscanRecordD32LV]->offset = (unsigned short)((char *)&prec->d32lv - (char *)prec);
    prt->papFldDes[sscanRecordD32NE]->offset = (unsigned short)((char *)&prec->d32ne - (char *)prec);
    prt->papFldDes[sscanRecordD32EU]->offset = (unsigned short)((char *)&prec->d32eu - (char *)prec);
    prt->papFldDes[sscanRecordD32PR]->offset = (unsigned short)((char *)&prec->d32pr - (char *)prec);
    prt->papFldDes[sscanRecordD33HR]->offset = (unsigned short)((char *)&prec->d33hr - (char *)prec);
    prt->papFldDes[sscanRecordD33LR]->offset = (unsigned short)((char *)&prec->d33lr - (char *)prec);
    prt->papFldDes[sscanRecordD33DA]->offset = (unsigned short)((char *)&prec->d33da - (char *)prec);
    prt->papFldDes[sscanRecordD33CA]->offset = (unsigned short)((char *)&prec->d33ca - (char *)prec);
    prt->papFldDes[sscanRecordD33CV]->offset = (unsigned short)((char *)&prec->d33cv - (char *)prec);
    prt->papFldDes[sscanRecordD33LV]->offset = (unsigned short)((char *)&prec->d33lv - (char *)prec);
    prt->papFldDes[sscanRecordD33NE]->offset = (unsigned short)((char *)&prec->d33ne - (char *)prec);
    prt->papFldDes[sscanRecordD33EU]->offset = (unsigned short)((char *)&prec->d33eu - (char *)prec);
    prt->papFldDes[sscanRecordD33PR]->offset = (unsigned short)((char *)&prec->d33pr - (char *)prec);
    prt->papFldDes[sscanRecordD34HR]->offset = (unsigned short)((char *)&prec->d34hr - (char *)prec);
    prt->papFldDes[sscanRecordD34LR]->offset = (unsigned short)((char *)&prec->d34lr - (char *)prec);
    prt->papFldDes[sscanRecordD34DA]->offset = (unsigned short)((char *)&prec->d34da - (char *)prec);
    prt->papFldDes[sscanRecordD34CA]->offset = (unsigned short)((char *)&prec->d34ca - (char *)prec);
    prt->papFldDes[sscanRecordD34CV]->offset = (unsigned short)((char *)&prec->d34cv - (char *)prec);
    prt->papFldDes[sscanRecordD34LV]->offset = (unsigned short)((char *)&prec->d34lv - (char *)prec);
    prt->papFldDes[sscanRecordD34NE]->offset = (unsigned short)((char *)&prec->d34ne - (char *)prec);
    prt->papFldDes[sscanRecordD34EU]->offset = (unsigned short)((char *)&prec->d34eu - (char *)prec);
    prt->papFldDes[sscanRecordD34PR]->offset = (unsigned short)((char *)&prec->d34pr - (char *)prec);
    prt->papFldDes[sscanRecordD35HR]->offset = (unsigned short)((char *)&prec->d35hr - (char *)prec);
    prt->papFldDes[sscanRecordD35LR]->offset = (unsigned short)((char *)&prec->d35lr - (char *)prec);
    prt->papFldDes[sscanRecordD35DA]->offset = (unsigned short)((char *)&prec->d35da - (char *)prec);
    prt->papFldDes[sscanRecordD35CA]->offset = (unsigned short)((char *)&prec->d35ca - (char *)prec);
    prt->papFldDes[sscanRecordD35CV]->offset = (unsigned short)((char *)&prec->d35cv - (char *)prec);
    prt->papFldDes[sscanRecordD35LV]->offset = (unsigned short)((char *)&prec->d35lv - (char *)prec);
    prt->papFldDes[sscanRecordD35NE]->offset = (unsigned short)((char *)&prec->d35ne - (char *)prec);
    prt->papFldDes[sscanRecordD35EU]->offset = (unsigned short)((char *)&prec->d35eu - (char *)prec);
    prt->papFldDes[sscanRecordD35PR]->offset = (unsigned short)((char *)&prec->d35pr - (char *)prec);
    prt->papFldDes[sscanRecordD36HR]->offset = (unsigned short)((char *)&prec->d36hr - (char *)prec);
    prt->papFldDes[sscanRecordD36LR]->offset = (unsigned short)((char *)&prec->d36lr - (char *)prec);
    prt->papFldDes[sscanRecordD36DA]->offset = (unsigned short)((char *)&prec->d36da - (char *)prec);
    prt->papFldDes[sscanRecordD36CA]->offset = (unsigned short)((char *)&prec->d36ca - (char *)prec);
    prt->papFldDes[sscanRecordD36CV]->offset = (unsigned short)((char *)&prec->d36cv - (char *)prec);
    prt->papFldDes[sscanRecordD36LV]->offset = (unsigned short)((char *)&prec->d36lv - (char *)prec);
    prt->papFldDes[sscanRecordD36NE]->offset = (unsigned short)((char *)&prec->d36ne - (char *)prec);
    prt->papFldDes[sscanRecordD36EU]->offset = (unsigned short)((char *)&prec->d36eu - (char *)prec);
    prt->papFldDes[sscanRecordD36PR]->offset = (unsigned short)((char *)&prec->d36pr - (char *)prec);
    prt->papFldDes[sscanRecordD37HR]->offset = (unsigned short)((char *)&prec->d37hr - (char *)prec);
    prt->papFldDes[sscanRecordD37LR]->offset = (unsigned short)((char *)&prec->d37lr - (char *)prec);
    prt->papFldDes[sscanRecordD37DA]->offset = (unsigned short)((char *)&prec->d37da - (char *)prec);
    prt->papFldDes[sscanRecordD37CA]->offset = (unsigned short)((char *)&prec->d37ca - (char *)prec);
    prt->papFldDes[sscanRecordD37CV]->offset = (unsigned short)((char *)&prec->d37cv - (char *)prec);
    prt->papFldDes[sscanRecordD37LV]->offset = (unsigned short)((char *)&prec->d37lv - (char *)prec);
    prt->papFldDes[sscanRecordD37NE]->offset = (unsigned short)((char *)&prec->d37ne - (char *)prec);
    prt->papFldDes[sscanRecordD37EU]->offset = (unsigned short)((char *)&prec->d37eu - (char *)prec);
    prt->papFldDes[sscanRecordD37PR]->offset = (unsigned short)((char *)&prec->d37pr - (char *)prec);
    prt->papFldDes[sscanRecordD38HR]->offset = (unsigned short)((char *)&prec->d38hr - (char *)prec);
    prt->papFldDes[sscanRecordD38LR]->offset = (unsigned short)((char *)&prec->d38lr - (char *)prec);
    prt->papFldDes[sscanRecordD38DA]->offset = (unsigned short)((char *)&prec->d38da - (char *)prec);
    prt->papFldDes[sscanRecordD38CA]->offset = (unsigned short)((char *)&prec->d38ca - (char *)prec);
    prt->papFldDes[sscanRecordD38CV]->offset = (unsigned short)((char *)&prec->d38cv - (char *)prec);
    prt->papFldDes[sscanRecordD38LV]->offset = (unsigned short)((char *)&prec->d38lv - (char *)prec);
    prt->papFldDes[sscanRecordD38NE]->offset = (unsigned short)((char *)&prec->d38ne - (char *)prec);
    prt->papFldDes[sscanRecordD38EU]->offset = (unsigned short)((char *)&prec->d38eu - (char *)prec);
    prt->papFldDes[sscanRecordD38PR]->offset = (unsigned short)((char *)&prec->d38pr - (char *)prec);
    prt->papFldDes[sscanRecordD39HR]->offset = (unsigned short)((char *)&prec->d39hr - (char *)prec);
    prt->papFldDes[sscanRecordD39LR]->offset = (unsigned short)((char *)&prec->d39lr - (char *)prec);
    prt->papFldDes[sscanRecordD39DA]->offset = (unsigned short)((char *)&prec->d39da - (char *)prec);
    prt->papFldDes[sscanRecordD39CA]->offset = (unsigned short)((char *)&prec->d39ca - (char *)prec);
    prt->papFldDes[sscanRecordD39CV]->offset = (unsigned short)((char *)&prec->d39cv - (char *)prec);
    prt->papFldDes[sscanRecordD39LV]->offset = (unsigned short)((char *)&prec->d39lv - (char *)prec);
    prt->papFldDes[sscanRecordD39NE]->offset = (unsigned short)((char *)&prec->d39ne - (char *)prec);
    prt->papFldDes[sscanRecordD39EU]->offset = (unsigned short)((char *)&prec->d39eu - (char *)prec);
    prt->papFldDes[sscanRecordD39PR]->offset = (unsigned short)((char *)&prec->d39pr - (char *)prec);
    prt->papFldDes[sscanRecordD40HR]->offset = (unsigned short)((char *)&prec->d40hr - (char *)prec);
    prt->papFldDes[sscanRecordD40LR]->offset = (unsigned short)((char *)&prec->d40lr - (char *)prec);
    prt->papFldDes[sscanRecordD40DA]->offset = (unsigned short)((char *)&prec->d40da - (char *)prec);
    prt->papFldDes[sscanRecordD40CA]->offset = (unsigned short)((char *)&prec->d40ca - (char *)prec);
    prt->papFldDes[sscanRecordD40CV]->offset = (unsigned short)((char *)&prec->d40cv - (char *)prec);
    prt->papFldDes[sscanRecordD40LV]->offset = (unsigned short)((char *)&prec->d40lv - (char *)prec);
    prt->papFldDes[sscanRecordD40NE]->offset = (unsigned short)((char *)&prec->d40ne - (char *)prec);
    prt->papFldDes[sscanRecordD40EU]->offset = (unsigned short)((char *)&prec->d40eu - (char *)prec);
    prt->papFldDes[sscanRecordD40PR]->offset = (unsigned short)((char *)&prec->d40pr - (char *)prec);
    prt->papFldDes[sscanRecordD41HR]->offset = (unsigned short)((char *)&prec->d41hr - (char *)prec);
    prt->papFldDes[sscanRecordD41LR]->offset = (unsigned short)((char *)&prec->d41lr - (char *)prec);
    prt->papFldDes[sscanRecordD41DA]->offset = (unsigned short)((char *)&prec->d41da - (char *)prec);
    prt->papFldDes[sscanRecordD41CA]->offset = (unsigned short)((char *)&prec->d41ca - (char *)prec);
    prt->papFldDes[sscanRecordD41CV]->offset = (unsigned short)((char *)&prec->d41cv - (char *)prec);
    prt->papFldDes[sscanRecordD41LV]->offset = (unsigned short)((char *)&prec->d41lv - (char *)prec);
    prt->papFldDes[sscanRecordD41NE]->offset = (unsigned short)((char *)&prec->d41ne - (char *)prec);
    prt->papFldDes[sscanRecordD41EU]->offset = (unsigned short)((char *)&prec->d41eu - (char *)prec);
    prt->papFldDes[sscanRecordD41PR]->offset = (unsigned short)((char *)&prec->d41pr - (char *)prec);
    prt->papFldDes[sscanRecordD42HR]->offset = (unsigned short)((char *)&prec->d42hr - (char *)prec);
    prt->papFldDes[sscanRecordD42LR]->offset = (unsigned short)((char *)&prec->d42lr - (char *)prec);
    prt->papFldDes[sscanRecordD42DA]->offset = (unsigned short)((char *)&prec->d42da - (char *)prec);
    prt->papFldDes[sscanRecordD42CA]->offset = (unsigned short)((char *)&prec->d42ca - (char *)prec);
    prt->papFldDes[sscanRecordD42CV]->offset = (unsigned short)((char *)&prec->d42cv - (char *)prec);
    prt->papFldDes[sscanRecordD42LV]->offset = (unsigned short)((char *)&prec->d42lv - (char *)prec);
    prt->papFldDes[sscanRecordD42NE]->offset = (unsigned short)((char *)&prec->d42ne - (char *)prec);
    prt->papFldDes[sscanRecordD42EU]->offset = (unsigned short)((char *)&prec->d42eu - (char *)prec);
    prt->papFldDes[sscanRecordD42PR]->offset = (unsigned short)((char *)&prec->d42pr - (char *)prec);
    prt->papFldDes[sscanRecordD43HR]->offset = (unsigned short)((char *)&prec->d43hr - (char *)prec);
    prt->papFldDes[sscanRecordD43LR]->offset = (unsigned short)((char *)&prec->d43lr - (char *)prec);
    prt->papFldDes[sscanRecordD43DA]->offset = (unsigned short)((char *)&prec->d43da - (char *)prec);
    prt->papFldDes[sscanRecordD43CA]->offset = (unsigned short)((char *)&prec->d43ca - (char *)prec);
    prt->papFldDes[sscanRecordD43CV]->offset = (unsigned short)((char *)&prec->d43cv - (char *)prec);
    prt->papFldDes[sscanRecordD43LV]->offset = (unsigned short)((char *)&prec->d43lv - (char *)prec);
    prt->papFldDes[sscanRecordD43NE]->offset = (unsigned short)((char *)&prec->d43ne - (char *)prec);
    prt->papFldDes[sscanRecordD43EU]->offset = (unsigned short)((char *)&prec->d43eu - (char *)prec);
    prt->papFldDes[sscanRecordD43PR]->offset = (unsigned short)((char *)&prec->d43pr - (char *)prec);
    prt->papFldDes[sscanRecordD44HR]->offset = (unsigned short)((char *)&prec->d44hr - (char *)prec);
    prt->papFldDes[sscanRecordD44LR]->offset = (unsigned short)((char *)&prec->d44lr - (char *)prec);
    prt->papFldDes[sscanRecordD44DA]->offset = (unsigned short)((char *)&prec->d44da - (char *)prec);
    prt->papFldDes[sscanRecordD44CA]->offset = (unsigned short)((char *)&prec->d44ca - (char *)prec);
    prt->papFldDes[sscanRecordD44CV]->offset = (unsigned short)((char *)&prec->d44cv - (char *)prec);
    prt->papFldDes[sscanRecordD44LV]->offset = (unsigned short)((char *)&prec->d44lv - (char *)prec);
    prt->papFldDes[sscanRecordD44NE]->offset = (unsigned short)((char *)&prec->d44ne - (char *)prec);
    prt->papFldDes[sscanRecordD44EU]->offset = (unsigned short)((char *)&prec->d44eu - (char *)prec);
    prt->papFldDes[sscanRecordD44PR]->offset = (unsigned short)((char *)&prec->d44pr - (char *)prec);
    prt->papFldDes[sscanRecordD45HR]->offset = (unsigned short)((char *)&prec->d45hr - (char *)prec);
    prt->papFldDes[sscanRecordD45LR]->offset = (unsigned short)((char *)&prec->d45lr - (char *)prec);
    prt->papFldDes[sscanRecordD45DA]->offset = (unsigned short)((char *)&prec->d45da - (char *)prec);
    prt->papFldDes[sscanRecordD45CA]->offset = (unsigned short)((char *)&prec->d45ca - (char *)prec);
    prt->papFldDes[sscanRecordD45CV]->offset = (unsigned short)((char *)&prec->d45cv - (char *)prec);
    prt->papFldDes[sscanRecordD45LV]->offset = (unsigned short)((char *)&prec->d45lv - (char *)prec);
    prt->papFldDes[sscanRecordD45NE]->offset = (unsigned short)((char *)&prec->d45ne - (char *)prec);
    prt->papFldDes[sscanRecordD45EU]->offset = (unsigned short)((char *)&prec->d45eu - (char *)prec);
    prt->papFldDes[sscanRecordD45PR]->offset = (unsigned short)((char *)&prec->d45pr - (char *)prec);
    prt->papFldDes[sscanRecordD46HR]->offset = (unsigned short)((char *)&prec->d46hr - (char *)prec);
    prt->papFldDes[sscanRecordD46LR]->offset = (unsigned short)((char *)&prec->d46lr - (char *)prec);
    prt->papFldDes[sscanRecordD46DA]->offset = (unsigned short)((char *)&prec->d46da - (char *)prec);
    prt->papFldDes[sscanRecordD46CA]->offset = (unsigned short)((char *)&prec->d46ca - (char *)prec);
    prt->papFldDes[sscanRecordD46CV]->offset = (unsigned short)((char *)&prec->d46cv - (char *)prec);
    prt->papFldDes[sscanRecordD46LV]->offset = (unsigned short)((char *)&prec->d46lv - (char *)prec);
    prt->papFldDes[sscanRecordD46NE]->offset = (unsigned short)((char *)&prec->d46ne - (char *)prec);
    prt->papFldDes[sscanRecordD46EU]->offset = (unsigned short)((char *)&prec->d46eu - (char *)prec);
    prt->papFldDes[sscanRecordD46PR]->offset = (unsigned short)((char *)&prec->d46pr - (char *)prec);
    prt->papFldDes[sscanRecordD47HR]->offset = (unsigned short)((char *)&prec->d47hr - (char *)prec);
    prt->papFldDes[sscanRecordD47LR]->offset = (unsigned short)((char *)&prec->d47lr - (char *)prec);
    prt->papFldDes[sscanRecordD47DA]->offset = (unsigned short)((char *)&prec->d47da - (char *)prec);
    prt->papFldDes[sscanRecordD47CA]->offset = (unsigned short)((char *)&prec->d47ca - (char *)prec);
    prt->papFldDes[sscanRecordD47CV]->offset = (unsigned short)((char *)&prec->d47cv - (char *)prec);
    prt->papFldDes[sscanRecordD47LV]->offset = (unsigned short)((char *)&prec->d47lv - (char *)prec);
    prt->papFldDes[sscanRecordD47NE]->offset = (unsigned short)((char *)&prec->d47ne - (char *)prec);
    prt->papFldDes[sscanRecordD47EU]->offset = (unsigned short)((char *)&prec->d47eu - (char *)prec);
    prt->papFldDes[sscanRecordD47PR]->offset = (unsigned short)((char *)&prec->d47pr - (char *)prec);
    prt->papFldDes[sscanRecordD48HR]->offset = (unsigned short)((char *)&prec->d48hr - (char *)prec);
    prt->papFldDes[sscanRecordD48LR]->offset = (unsigned short)((char *)&prec->d48lr - (char *)prec);
    prt->papFldDes[sscanRecordD48DA]->offset = (unsigned short)((char *)&prec->d48da - (char *)prec);
    prt->papFldDes[sscanRecordD48CA]->offset = (unsigned short)((char *)&prec->d48ca - (char *)prec);
    prt->papFldDes[sscanRecordD48CV]->offset = (unsigned short)((char *)&prec->d48cv - (char *)prec);
    prt->papFldDes[sscanRecordD48LV]->offset = (unsigned short)((char *)&prec->d48lv - (char *)prec);
    prt->papFldDes[sscanRecordD48NE]->offset = (unsigned short)((char *)&prec->d48ne - (char *)prec);
    prt->papFldDes[sscanRecordD48EU]->offset = (unsigned short)((char *)&prec->d48eu - (char *)prec);
    prt->papFldDes[sscanRecordD48PR]->offset = (unsigned short)((char *)&prec->d48pr - (char *)prec);
    prt->papFldDes[sscanRecordD49HR]->offset = (unsigned short)((char *)&prec->d49hr - (char *)prec);
    prt->papFldDes[sscanRecordD49LR]->offset = (unsigned short)((char *)&prec->d49lr - (char *)prec);
    prt->papFldDes[sscanRecordD49DA]->offset = (unsigned short)((char *)&prec->d49da - (char *)prec);
    prt->papFldDes[sscanRecordD49CA]->offset = (unsigned short)((char *)&prec->d49ca - (char *)prec);
    prt->papFldDes[sscanRecordD49CV]->offset = (unsigned short)((char *)&prec->d49cv - (char *)prec);
    prt->papFldDes[sscanRecordD49LV]->offset = (unsigned short)((char *)&prec->d49lv - (char *)prec);
    prt->papFldDes[sscanRecordD49NE]->offset = (unsigned short)((char *)&prec->d49ne - (char *)prec);
    prt->papFldDes[sscanRecordD49EU]->offset = (unsigned short)((char *)&prec->d49eu - (char *)prec);
    prt->papFldDes[sscanRecordD49PR]->offset = (unsigned short)((char *)&prec->d49pr - (char *)prec);
    prt->papFldDes[sscanRecordD50HR]->offset = (unsigned short)((char *)&prec->d50hr - (char *)prec);
    prt->papFldDes[sscanRecordD50LR]->offset = (unsigned short)((char *)&prec->d50lr - (char *)prec);
    prt->papFldDes[sscanRecordD50DA]->offset = (unsigned short)((char *)&prec->d50da - (char *)prec);
    prt->papFldDes[sscanRecordD50CA]->offset = (unsigned short)((char *)&prec->d50ca - (char *)prec);
    prt->papFldDes[sscanRecordD50CV]->offset = (unsigned short)((char *)&prec->d50cv - (char *)prec);
    prt->papFldDes[sscanRecordD50LV]->offset = (unsigned short)((char *)&prec->d50lv - (char *)prec);
    prt->papFldDes[sscanRecordD50NE]->offset = (unsigned short)((char *)&prec->d50ne - (char *)prec);
    prt->papFldDes[sscanRecordD50EU]->offset = (unsigned short)((char *)&prec->d50eu - (char *)prec);
    prt->papFldDes[sscanRecordD50PR]->offset = (unsigned short)((char *)&prec->d50pr - (char *)prec);
    prt->papFldDes[sscanRecordD51HR]->offset = (unsigned short)((char *)&prec->d51hr - (char *)prec);
    prt->papFldDes[sscanRecordD51LR]->offset = (unsigned short)((char *)&prec->d51lr - (char *)prec);
    prt->papFldDes[sscanRecordD51DA]->offset = (unsigned short)((char *)&prec->d51da - (char *)prec);
    prt->papFldDes[sscanRecordD51CA]->offset = (unsigned short)((char *)&prec->d51ca - (char *)prec);
    prt->papFldDes[sscanRecordD51CV]->offset = (unsigned short)((char *)&prec->d51cv - (char *)prec);
    prt->papFldDes[sscanRecordD51LV]->offset = (unsigned short)((char *)&prec->d51lv - (char *)prec);
    prt->papFldDes[sscanRecordD51NE]->offset = (unsigned short)((char *)&prec->d51ne - (char *)prec);
    prt->papFldDes[sscanRecordD51EU]->offset = (unsigned short)((char *)&prec->d51eu - (char *)prec);
    prt->papFldDes[sscanRecordD51PR]->offset = (unsigned short)((char *)&prec->d51pr - (char *)prec);
    prt->papFldDes[sscanRecordD52HR]->offset = (unsigned short)((char *)&prec->d52hr - (char *)prec);
    prt->papFldDes[sscanRecordD52LR]->offset = (unsigned short)((char *)&prec->d52lr - (char *)prec);
    prt->papFldDes[sscanRecordD52DA]->offset = (unsigned short)((char *)&prec->d52da - (char *)prec);
    prt->papFldDes[sscanRecordD52CA]->offset = (unsigned short)((char *)&prec->d52ca - (char *)prec);
    prt->papFldDes[sscanRecordD52CV]->offset = (unsigned short)((char *)&prec->d52cv - (char *)prec);
    prt->papFldDes[sscanRecordD52LV]->offset = (unsigned short)((char *)&prec->d52lv - (char *)prec);
    prt->papFldDes[sscanRecordD52NE]->offset = (unsigned short)((char *)&prec->d52ne - (char *)prec);
    prt->papFldDes[sscanRecordD52EU]->offset = (unsigned short)((char *)&prec->d52eu - (char *)prec);
    prt->papFldDes[sscanRecordD52PR]->offset = (unsigned short)((char *)&prec->d52pr - (char *)prec);
    prt->papFldDes[sscanRecordD53HR]->offset = (unsigned short)((char *)&prec->d53hr - (char *)prec);
    prt->papFldDes[sscanRecordD53LR]->offset = (unsigned short)((char *)&prec->d53lr - (char *)prec);
    prt->papFldDes[sscanRecordD53DA]->offset = (unsigned short)((char *)&prec->d53da - (char *)prec);
    prt->papFldDes[sscanRecordD53CA]->offset = (unsigned short)((char *)&prec->d53ca - (char *)prec);
    prt->papFldDes[sscanRecordD53CV]->offset = (unsigned short)((char *)&prec->d53cv - (char *)prec);
    prt->papFldDes[sscanRecordD53LV]->offset = (unsigned short)((char *)&prec->d53lv - (char *)prec);
    prt->papFldDes[sscanRecordD53NE]->offset = (unsigned short)((char *)&prec->d53ne - (char *)prec);
    prt->papFldDes[sscanRecordD53EU]->offset = (unsigned short)((char *)&prec->d53eu - (char *)prec);
    prt->papFldDes[sscanRecordD53PR]->offset = (unsigned short)((char *)&prec->d53pr - (char *)prec);
    prt->papFldDes[sscanRecordD54HR]->offset = (unsigned short)((char *)&prec->d54hr - (char *)prec);
    prt->papFldDes[sscanRecordD54LR]->offset = (unsigned short)((char *)&prec->d54lr - (char *)prec);
    prt->papFldDes[sscanRecordD54DA]->offset = (unsigned short)((char *)&prec->d54da - (char *)prec);
    prt->papFldDes[sscanRecordD54CA]->offset = (unsigned short)((char *)&prec->d54ca - (char *)prec);
    prt->papFldDes[sscanRecordD54CV]->offset = (unsigned short)((char *)&prec->d54cv - (char *)prec);
    prt->papFldDes[sscanRecordD54LV]->offset = (unsigned short)((char *)&prec->d54lv - (char *)prec);
    prt->papFldDes[sscanRecordD54NE]->offset = (unsigned short)((char *)&prec->d54ne - (char *)prec);
    prt->papFldDes[sscanRecordD54EU]->offset = (unsigned short)((char *)&prec->d54eu - (char *)prec);
    prt->papFldDes[sscanRecordD54PR]->offset = (unsigned short)((char *)&prec->d54pr - (char *)prec);
    prt->papFldDes[sscanRecordD55HR]->offset = (unsigned short)((char *)&prec->d55hr - (char *)prec);
    prt->papFldDes[sscanRecordD55LR]->offset = (unsigned short)((char *)&prec->d55lr - (char *)prec);
    prt->papFldDes[sscanRecordD55DA]->offset = (unsigned short)((char *)&prec->d55da - (char *)prec);
    prt->papFldDes[sscanRecordD55CA]->offset = (unsigned short)((char *)&prec->d55ca - (char *)prec);
    prt->papFldDes[sscanRecordD55CV]->offset = (unsigned short)((char *)&prec->d55cv - (char *)prec);
    prt->papFldDes[sscanRecordD55LV]->offset = (unsigned short)((char *)&prec->d55lv - (char *)prec);
    prt->papFldDes[sscanRecordD55NE]->offset = (unsigned short)((char *)&prec->d55ne - (char *)prec);
    prt->papFldDes[sscanRecordD55EU]->offset = (unsigned short)((char *)&prec->d55eu - (char *)prec);
    prt->papFldDes[sscanRecordD55PR]->offset = (unsigned short)((char *)&prec->d55pr - (char *)prec);
    prt->papFldDes[sscanRecordD56HR]->offset = (unsigned short)((char *)&prec->d56hr - (char *)prec);
    prt->papFldDes[sscanRecordD56LR]->offset = (unsigned short)((char *)&prec->d56lr - (char *)prec);
    prt->papFldDes[sscanRecordD56DA]->offset = (unsigned short)((char *)&prec->d56da - (char *)prec);
    prt->papFldDes[sscanRecordD56CA]->offset = (unsigned short)((char *)&prec->d56ca - (char *)prec);
    prt->papFldDes[sscanRecordD56CV]->offset = (unsigned short)((char *)&prec->d56cv - (char *)prec);
    prt->papFldDes[sscanRecordD56LV]->offset = (unsigned short)((char *)&prec->d56lv - (char *)prec);
    prt->papFldDes[sscanRecordD56NE]->offset = (unsigned short)((char *)&prec->d56ne - (char *)prec);
    prt->papFldDes[sscanRecordD56EU]->offset = (unsigned short)((char *)&prec->d56eu - (char *)prec);
    prt->papFldDes[sscanRecordD56PR]->offset = (unsigned short)((char *)&prec->d56pr - (char *)prec);
    prt->papFldDes[sscanRecordD57HR]->offset = (unsigned short)((char *)&prec->d57hr - (char *)prec);
    prt->papFldDes[sscanRecordD57LR]->offset = (unsigned short)((char *)&prec->d57lr - (char *)prec);
    prt->papFldDes[sscanRecordD57DA]->offset = (unsigned short)((char *)&prec->d57da - (char *)prec);
    prt->papFldDes[sscanRecordD57CA]->offset = (unsigned short)((char *)&prec->d57ca - (char *)prec);
    prt->papFldDes[sscanRecordD57CV]->offset = (unsigned short)((char *)&prec->d57cv - (char *)prec);
    prt->papFldDes[sscanRecordD57LV]->offset = (unsigned short)((char *)&prec->d57lv - (char *)prec);
    prt->papFldDes[sscanRecordD57NE]->offset = (unsigned short)((char *)&prec->d57ne - (char *)prec);
    prt->papFldDes[sscanRecordD57EU]->offset = (unsigned short)((char *)&prec->d57eu - (char *)prec);
    prt->papFldDes[sscanRecordD57PR]->offset = (unsigned short)((char *)&prec->d57pr - (char *)prec);
    prt->papFldDes[sscanRecordD58HR]->offset = (unsigned short)((char *)&prec->d58hr - (char *)prec);
    prt->papFldDes[sscanRecordD58LR]->offset = (unsigned short)((char *)&prec->d58lr - (char *)prec);
    prt->papFldDes[sscanRecordD58DA]->offset = (unsigned short)((char *)&prec->d58da - (char *)prec);
    prt->papFldDes[sscanRecordD58CA]->offset = (unsigned short)((char *)&prec->d58ca - (char *)prec);
    prt->papFldDes[sscanRecordD58CV]->offset = (unsigned short)((char *)&prec->d58cv - (char *)prec);
    prt->papFldDes[sscanRecordD58LV]->offset = (unsigned short)((char *)&prec->d58lv - (char *)prec);
    prt->papFldDes[sscanRecordD58NE]->offset = (unsigned short)((char *)&prec->d58ne - (char *)prec);
    prt->papFldDes[sscanRecordD58EU]->offset = (unsigned short)((char *)&prec->d58eu - (char *)prec);
    prt->papFldDes[sscanRecordD58PR]->offset = (unsigned short)((char *)&prec->d58pr - (char *)prec);
    prt->papFldDes[sscanRecordD59HR]->offset = (unsigned short)((char *)&prec->d59hr - (char *)prec);
    prt->papFldDes[sscanRecordD59LR]->offset = (unsigned short)((char *)&prec->d59lr - (char *)prec);
    prt->papFldDes[sscanRecordD59DA]->offset = (unsigned short)((char *)&prec->d59da - (char *)prec);
    prt->papFldDes[sscanRecordD59CA]->offset = (unsigned short)((char *)&prec->d59ca - (char *)prec);
    prt->papFldDes[sscanRecordD59CV]->offset = (unsigned short)((char *)&prec->d59cv - (char *)prec);
    prt->papFldDes[sscanRecordD59LV]->offset = (unsigned short)((char *)&prec->d59lv - (char *)prec);
    prt->papFldDes[sscanRecordD59NE]->offset = (unsigned short)((char *)&prec->d59ne - (char *)prec);
    prt->papFldDes[sscanRecordD59EU]->offset = (unsigned short)((char *)&prec->d59eu - (char *)prec);
    prt->papFldDes[sscanRecordD59PR]->offset = (unsigned short)((char *)&prec->d59pr - (char *)prec);
    prt->papFldDes[sscanRecordD60HR]->offset = (unsigned short)((char *)&prec->d60hr - (char *)prec);
    prt->papFldDes[sscanRecordD60LR]->offset = (unsigned short)((char *)&prec->d60lr - (char *)prec);
    prt->papFldDes[sscanRecordD60DA]->offset = (unsigned short)((char *)&prec->d60da - (char *)prec);
    prt->papFldDes[sscanRecordD60CA]->offset = (unsigned short)((char *)&prec->d60ca - (char *)prec);
    prt->papFldDes[sscanRecordD60CV]->offset = (unsigned short)((char *)&prec->d60cv - (char *)prec);
    prt->papFldDes[sscanRecordD60LV]->offset = (unsigned short)((char *)&prec->d60lv - (char *)prec);
    prt->papFldDes[sscanRecordD60NE]->offset = (unsigned short)((char *)&prec->d60ne - (char *)prec);
    prt->papFldDes[sscanRecordD60EU]->offset = (unsigned short)((char *)&prec->d60eu - (char *)prec);
    prt->papFldDes[sscanRecordD60PR]->offset = (unsigned short)((char *)&prec->d60pr - (char *)prec);
    prt->papFldDes[sscanRecordD61HR]->offset = (unsigned short)((char *)&prec->d61hr - (char *)prec);
    prt->papFldDes[sscanRecordD61LR]->offset = (unsigned short)((char *)&prec->d61lr - (char *)prec);
    prt->papFldDes[sscanRecordD61DA]->offset = (unsigned short)((char *)&prec->d61da - (char *)prec);
    prt->papFldDes[sscanRecordD61CA]->offset = (unsigned short)((char *)&prec->d61ca - (char *)prec);
    prt->papFldDes[sscanRecordD61CV]->offset = (unsigned short)((char *)&prec->d61cv - (char *)prec);
    prt->papFldDes[sscanRecordD61LV]->offset = (unsigned short)((char *)&prec->d61lv - (char *)prec);
    prt->papFldDes[sscanRecordD61NE]->offset = (unsigned short)((char *)&prec->d61ne - (char *)prec);
    prt->papFldDes[sscanRecordD61EU]->offset = (unsigned short)((char *)&prec->d61eu - (char *)prec);
    prt->papFldDes[sscanRecordD61PR]->offset = (unsigned short)((char *)&prec->d61pr - (char *)prec);
    prt->papFldDes[sscanRecordD62HR]->offset = (unsigned short)((char *)&prec->d62hr - (char *)prec);
    prt->papFldDes[sscanRecordD62LR]->offset = (unsigned short)((char *)&prec->d62lr - (char *)prec);
    prt->papFldDes[sscanRecordD62DA]->offset = (unsigned short)((char *)&prec->d62da - (char *)prec);
    prt->papFldDes[sscanRecordD62CA]->offset = (unsigned short)((char *)&prec->d62ca - (char *)prec);
    prt->papFldDes[sscanRecordD62CV]->offset = (unsigned short)((char *)&prec->d62cv - (char *)prec);
    prt->papFldDes[sscanRecordD62LV]->offset = (unsigned short)((char *)&prec->d62lv - (char *)prec);
    prt->papFldDes[sscanRecordD62NE]->offset = (unsigned short)((char *)&prec->d62ne - (char *)prec);
    prt->papFldDes[sscanRecordD62EU]->offset = (unsigned short)((char *)&prec->d62eu - (char *)prec);
    prt->papFldDes[sscanRecordD62PR]->offset = (unsigned short)((char *)&prec->d62pr - (char *)prec);
    prt->papFldDes[sscanRecordD63HR]->offset = (unsigned short)((char *)&prec->d63hr - (char *)prec);
    prt->papFldDes[sscanRecordD63LR]->offset = (unsigned short)((char *)&prec->d63lr - (char *)prec);
    prt->papFldDes[sscanRecordD63DA]->offset = (unsigned short)((char *)&prec->d63da - (char *)prec);
    prt->papFldDes[sscanRecordD63CA]->offset = (unsigned short)((char *)&prec->d63ca - (char *)prec);
    prt->papFldDes[sscanRecordD63CV]->offset = (unsigned short)((char *)&prec->d63cv - (char *)prec);
    prt->papFldDes[sscanRecordD63LV]->offset = (unsigned short)((char *)&prec->d63lv - (char *)prec);
    prt->papFldDes[sscanRecordD63NE]->offset = (unsigned short)((char *)&prec->d63ne - (char *)prec);
    prt->papFldDes[sscanRecordD63EU]->offset = (unsigned short)((char *)&prec->d63eu - (char *)prec);
    prt->papFldDes[sscanRecordD63PR]->offset = (unsigned short)((char *)&prec->d63pr - (char *)prec);
    prt->papFldDes[sscanRecordD64HR]->offset = (unsigned short)((char *)&prec->d64hr - (char *)prec);
    prt->papFldDes[sscanRecordD64LR]->offset = (unsigned short)((char *)&prec->d64lr - (char *)prec);
    prt->papFldDes[sscanRecordD64DA]->offset = (unsigned short)((char *)&prec->d64da - (char *)prec);
    prt->papFldDes[sscanRecordD64CA]->offset = (unsigned short)((char *)&prec->d64ca - (char *)prec);
    prt->papFldDes[sscanRecordD64CV]->offset = (unsigned short)((char *)&prec->d64cv - (char *)prec);
    prt->papFldDes[sscanRecordD64LV]->offset = (unsigned short)((char *)&prec->d64lv - (char *)prec);
    prt->papFldDes[sscanRecordD64NE]->offset = (unsigned short)((char *)&prec->d64ne - (char *)prec);
    prt->papFldDes[sscanRecordD64EU]->offset = (unsigned short)((char *)&prec->d64eu - (char *)prec);
    prt->papFldDes[sscanRecordD64PR]->offset = (unsigned short)((char *)&prec->d64pr - (char *)prec);
    prt->papFldDes[sscanRecordD65HR]->offset = (unsigned short)((char *)&prec->d65hr - (char *)prec);
    prt->papFldDes[sscanRecordD65LR]->offset = (unsigned short)((char *)&prec->d65lr - (char *)prec);
    prt->papFldDes[sscanRecordD65DA]->offset = (unsigned short)((char *)&prec->d65da - (char *)prec);
    prt->papFldDes[sscanRecordD65CA]->offset = (unsigned short)((char *)&prec->d65ca - (char *)prec);
    prt->papFldDes[sscanRecordD65CV]->offset = (unsigned short)((char *)&prec->d65cv - (char *)prec);
    prt->papFldDes[sscanRecordD65LV]->offset = (unsigned short)((char *)&prec->d65lv - (char *)prec);
    prt->papFldDes[sscanRecordD65NE]->offset = (unsigned short)((char *)&prec->d65ne - (char *)prec);
    prt->papFldDes[sscanRecordD65EU]->offset = (unsigned short)((char *)&prec->d65eu - (char *)prec);
    prt->papFldDes[sscanRecordD65PR]->offset = (unsigned short)((char *)&prec->d65pr - (char *)prec);
    prt->papFldDes[sscanRecordD66HR]->offset = (unsigned short)((char *)&prec->d66hr - (char *)prec);
    prt->papFldDes[sscanRecordD66LR]->offset = (unsigned short)((char *)&prec->d66lr - (char *)prec);
    prt->papFldDes[sscanRecordD66DA]->offset = (unsigned short)((char *)&prec->d66da - (char *)prec);
    prt->papFldDes[sscanRecordD66CA]->offset = (unsigned short)((char *)&prec->d66ca - (char *)prec);
    prt->papFldDes[sscanRecordD66CV]->offset = (unsigned short)((char *)&prec->d66cv - (char *)prec);
    prt->papFldDes[sscanRecordD66LV]->offset = (unsigned short)((char *)&prec->d66lv - (char *)prec);
    prt->papFldDes[sscanRecordD66NE]->offset = (unsigned short)((char *)&prec->d66ne - (char *)prec);
    prt->papFldDes[sscanRecordD66EU]->offset = (unsigned short)((char *)&prec->d66eu - (char *)prec);
    prt->papFldDes[sscanRecordD66PR]->offset = (unsigned short)((char *)&prec->d66pr - (char *)prec);
    prt->papFldDes[sscanRecordD67HR]->offset = (unsigned short)((char *)&prec->d67hr - (char *)prec);
    prt->papFldDes[sscanRecordD67LR]->offset = (unsigned short)((char *)&prec->d67lr - (char *)prec);
    prt->papFldDes[sscanRecordD67DA]->offset = (unsigned short)((char *)&prec->d67da - (char *)prec);
    prt->papFldDes[sscanRecordD67CA]->offset = (unsigned short)((char *)&prec->d67ca - (char *)prec);
    prt->papFldDes[sscanRecordD67CV]->offset = (unsigned short)((char *)&prec->d67cv - (char *)prec);
    prt->papFldDes[sscanRecordD67LV]->offset = (unsigned short)((char *)&prec->d67lv - (char *)prec);
    prt->papFldDes[sscanRecordD67NE]->offset = (unsigned short)((char *)&prec->d67ne - (char *)prec);
    prt->papFldDes[sscanRecordD67EU]->offset = (unsigned short)((char *)&prec->d67eu - (char *)prec);
    prt->papFldDes[sscanRecordD67PR]->offset = (unsigned short)((char *)&prec->d67pr - (char *)prec);
    prt->papFldDes[sscanRecordD68HR]->offset = (unsigned short)((char *)&prec->d68hr - (char *)prec);
    prt->papFldDes[sscanRecordD68LR]->offset = (unsigned short)((char *)&prec->d68lr - (char *)prec);
    prt->papFldDes[sscanRecordD68DA]->offset = (unsigned short)((char *)&prec->d68da - (char *)prec);
    prt->papFldDes[sscanRecordD68CA]->offset = (unsigned short)((char *)&prec->d68ca - (char *)prec);
    prt->papFldDes[sscanRecordD68CV]->offset = (unsigned short)((char *)&prec->d68cv - (char *)prec);
    prt->papFldDes[sscanRecordD68LV]->offset = (unsigned short)((char *)&prec->d68lv - (char *)prec);
    prt->papFldDes[sscanRecordD68NE]->offset = (unsigned short)((char *)&prec->d68ne - (char *)prec);
    prt->papFldDes[sscanRecordD68EU]->offset = (unsigned short)((char *)&prec->d68eu - (char *)prec);
    prt->papFldDes[sscanRecordD68PR]->offset = (unsigned short)((char *)&prec->d68pr - (char *)prec);
    prt->papFldDes[sscanRecordD69HR]->offset = (unsigned short)((char *)&prec->d69hr - (char *)prec);
    prt->papFldDes[sscanRecordD69LR]->offset = (unsigned short)((char *)&prec->d69lr - (char *)prec);
    prt->papFldDes[sscanRecordD69DA]->offset = (unsigned short)((char *)&prec->d69da - (char *)prec);
    prt->papFldDes[sscanRecordD69CA]->offset = (unsigned short)((char *)&prec->d69ca - (char *)prec);
    prt->papFldDes[sscanRecordD69CV]->offset = (unsigned short)((char *)&prec->d69cv - (char *)prec);
    prt->papFldDes[sscanRecordD69LV]->offset = (unsigned short)((char *)&prec->d69lv - (char *)prec);
    prt->papFldDes[sscanRecordD69NE]->offset = (unsigned short)((char *)&prec->d69ne - (char *)prec);
    prt->papFldDes[sscanRecordD69EU]->offset = (unsigned short)((char *)&prec->d69eu - (char *)prec);
    prt->papFldDes[sscanRecordD69PR]->offset = (unsigned short)((char *)&prec->d69pr - (char *)prec);
    prt->papFldDes[sscanRecordD70HR]->offset = (unsigned short)((char *)&prec->d70hr - (char *)prec);
    prt->papFldDes[sscanRecordD70LR]->offset = (unsigned short)((char *)&prec->d70lr - (char *)prec);
    prt->papFldDes[sscanRecordD70DA]->offset = (unsigned short)((char *)&prec->d70da - (char *)prec);
    prt->papFldDes[sscanRecordD70CA]->offset = (unsigned short)((char *)&prec->d70ca - (char *)prec);
    prt->papFldDes[sscanRecordD70CV]->offset = (unsigned short)((char *)&prec->d70cv - (char *)prec);
    prt->papFldDes[sscanRecordD70LV]->offset = (unsigned short)((char *)&prec->d70lv - (char *)prec);
    prt->papFldDes[sscanRecordD70NE]->offset = (unsigned short)((char *)&prec->d70ne - (char *)prec);
    prt->papFldDes[sscanRecordD70EU]->offset = (unsigned short)((char *)&prec->d70eu - (char *)prec);
    prt->papFldDes[sscanRecordD70PR]->offset = (unsigned short)((char *)&prec->d70pr - (char *)prec);
    prt->papFldDes[sscanRecordT1CD]->offset = (unsigned short)((char *)&prec->t1cd - (char *)prec);
    prt->papFldDes[sscanRecordT2CD]->offset = (unsigned short)((char *)&prec->t2cd - (char *)prec);
    prt->papFldDes[sscanRecordT3CD]->offset = (unsigned short)((char *)&prec->t3cd - (char *)prec);
    prt->papFldDes[sscanRecordT4CD]->offset = (unsigned short)((char *)&prec->t4cd - (char *)prec);
    prt->papFldDes[sscanRecordA1CD]->offset = (unsigned short)((char *)&prec->a1cd - (char *)prec);
    prt->papFldDes[sscanRecordBSCD]->offset = (unsigned short)((char *)&prec->bscd - (char *)prec);
    prt->papFldDes[sscanRecordASCD]->offset = (unsigned short)((char *)&prec->ascd - (char *)prec);
    prt->papFldDes[sscanRecordPAUS]->offset = (unsigned short)((char *)&prec->paus - (char *)prec);
    prt->papFldDes[sscanRecordLPAU]->offset = (unsigned short)((char *)&prec->lpau - (char *)prec);
    prt->papFldDes[sscanRecordPDLY]->offset = (unsigned short)((char *)&prec->pdly - (char *)prec);
    prt->papFldDes[sscanRecordDDLY]->offset = (unsigned short)((char *)&prec->ddly - (char *)prec);
    prt->papFldDes[sscanRecordRDLY]->offset = (unsigned short)((char *)&prec->rdly - (char *)prec);
    prt->papFldDes[sscanRecordFAZE]->offset = (unsigned short)((char *)&prec->faze - (char *)prec);
    prt->papFldDes[sscanRecordACQM]->offset = (unsigned short)((char *)&prec->acqm - (char *)prec);
    prt->papFldDes[sscanRecordACQT]->offset = (unsigned short)((char *)&prec->acqt - (char *)prec);
    prt->papFldDes[sscanRecordDSTATE]->offset = (unsigned short)((char *)&prec->dstate - (char *)prec);
    prt->papFldDes[sscanRecordCOPYTO]->offset = (unsigned short)((char *)&prec->copyto - (char *)prec);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(sscanRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_sscanRecord_H */
