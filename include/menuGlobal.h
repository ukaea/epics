/* menuGlobal.h generated from menuGlobal.dbd */

#ifndef INC_menuGlobal_H
#define INC_menuGlobal_H

#ifndef menuPriority_NUM_CHOICES
typedef enum {
    menuPriorityLOW                 /* LOW */,
    menuPriorityMEDIUM              /* MEDIUM */,
    menuPriorityHIGH                /* HIGH */
} menuPriority;
#define menuPriority_NUM_CHOICES 3
#endif

#ifndef menuIvoa_NUM_CHOICES
typedef enum {
    menuIvoaContinue_normally       /* Continue normally */,
    menuIvoaDon_t_drive_outputs     /* Don't drive outputs */,
    menuIvoaSet_output_to_IVOV      /* Set output to IVOV */
} menuIvoa;
#define menuIvoa_NUM_CHOICES 3
#endif

#ifndef menuAlarmSevr_NUM_CHOICES
typedef enum {
    menuAlarmSevrNO_ALARM           /* NO_ALARM */,
    menuAlarmSevrMINOR              /* MINOR */,
    menuAlarmSevrMAJOR              /* MAJOR */,
    menuAlarmSevrINVALID            /* INVALID */
} menuAlarmSevr;
#define menuAlarmSevr_NUM_CHOICES 4
#endif

#ifndef menuOmsl_NUM_CHOICES
typedef enum {
    menuOmslsupervisory             /* supervisory */,
    menuOmslclosed_loop             /* closed_loop */
} menuOmsl;
#define menuOmsl_NUM_CHOICES 2
#endif

#ifndef menuFtype_NUM_CHOICES
typedef enum {
    menuFtypeSTRING                 /* STRING */,
    menuFtypeCHAR                   /* CHAR */,
    menuFtypeUCHAR                  /* UCHAR */,
    menuFtypeSHORT                  /* SHORT */,
    menuFtypeUSHORT                 /* USHORT */,
    menuFtypeLONG                   /* LONG */,
    menuFtypeULONG                  /* ULONG */,
    menuFtypeINT64                  /* INT64 */,
    menuFtypeUINT64                 /* UINT64 */,
    menuFtypeFLOAT                  /* FLOAT */,
    menuFtypeDOUBLE                 /* DOUBLE */,
    menuFtypeENUM                   /* ENUM */
} menuFtype;
#define menuFtype_NUM_CHOICES 12
#endif

#ifndef menuSimm_NUM_CHOICES
typedef enum {
    menuSimmNO                      /* NO */,
    menuSimmYES                     /* YES */,
    menuSimmRAW                     /* RAW */
} menuSimm;
#define menuSimm_NUM_CHOICES 3
#endif

#ifndef menuYesNo_NUM_CHOICES
typedef enum {
    menuYesNoNO                     /* NO */,
    menuYesNoYES                    /* YES */
} menuYesNo;
#define menuYesNo_NUM_CHOICES 2
#endif

#ifndef menuAlarmStat_NUM_CHOICES
typedef enum {
    menuAlarmStatNO_ALARM           /* NO_ALARM */,
    menuAlarmStatREAD               /* READ */,
    menuAlarmStatWRITE              /* WRITE */,
    menuAlarmStatHIHI               /* HIHI */,
    menuAlarmStatHIGH               /* HIGH */,
    menuAlarmStatLOLO               /* LOLO */,
    menuAlarmStatLOW                /* LOW */,
    menuAlarmStatSTATE              /* STATE */,
    menuAlarmStatCOS                /* COS */,
    menuAlarmStatCOMM               /* COMM */,
    menuAlarmStatTIMEOUT            /* TIMEOUT */,
    menuAlarmStatHWLIMIT            /* HWLIMIT */,
    menuAlarmStatCALC               /* CALC */,
    menuAlarmStatSCAN               /* SCAN */,
    menuAlarmStatLINK               /* LINK */,
    menuAlarmStatSOFT               /* SOFT */,
    menuAlarmStatBAD_SUB            /* BAD_SUB */,
    menuAlarmStatUDF                /* UDF */,
    menuAlarmStatDISABLE            /* DISABLE */,
    menuAlarmStatSIMM               /* SIMM */,
    menuAlarmStatREAD_ACCESS        /* READ_ACCESS */,
    menuAlarmStatWRITE_ACCESS       /* WRITE_ACCESS */
} menuAlarmStat;
#define menuAlarmStat_NUM_CHOICES 22
#endif

#ifndef menuPost_NUM_CHOICES
typedef enum {
    menuPost_OnChange               /* On Change */,
    menuPost_Always                 /* Always */
} menuPost;
#define menuPost_NUM_CHOICES 2
#endif

#ifndef menuPini_NUM_CHOICES
typedef enum {
    menuPiniNO                      /* NO */,
    menuPiniYES                     /* YES */,
    menuPiniRUN                     /* RUN */,
    menuPiniRUNNING                 /* RUNNING */,
    menuPiniPAUSE                   /* PAUSE */,
    menuPiniPAUSED                  /* PAUSED */
} menuPini;
#define menuPini_NUM_CHOICES 6
#endif


#endif /* INC_menuGlobal_H */
