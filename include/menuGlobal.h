/** @file menuGlobal.h
 * @brief Declarations generated from menuGlobal.dbd
 */

#ifndef INC_menuGlobal_H
#define INC_menuGlobal_H

#ifndef menuPriority_NUM_CHOICES
/** @brief Enumerated type from menu menuPriority */
typedef enum {
    menuPriorityLOW                 /**< @brief State string "LOW" */,
    menuPriorityMEDIUM              /**< @brief State string "MEDIUM" */,
    menuPriorityHIGH                /**< @brief State string "HIGH" */
} menuPriority;
/** @brief Number of states defined for menu menuPriority */
#define menuPriority_NUM_CHOICES 3
#endif

#ifndef menuIvoa_NUM_CHOICES
/** @brief Enumerated type from menu menuIvoa */
typedef enum {
    menuIvoaContinue_normally       /**< @brief State string "Continue normally" */,
    menuIvoaDon_t_drive_outputs     /**< @brief State string "Don't drive outputs" */,
    menuIvoaSet_output_to_IVOV      /**< @brief State string "Set output to IVOV" */
} menuIvoa;
/** @brief Number of states defined for menu menuIvoa */
#define menuIvoa_NUM_CHOICES 3
#endif

#ifndef menuFtype_NUM_CHOICES
/** @brief Enumerated type from menu menuFtype */
typedef enum {
    menuFtypeSTRING                 /**< @brief State string "STRING" */,
    menuFtypeCHAR                   /**< @brief State string "CHAR" */,
    menuFtypeUCHAR                  /**< @brief State string "UCHAR" */,
    menuFtypeSHORT                  /**< @brief State string "SHORT" */,
    menuFtypeUSHORT                 /**< @brief State string "USHORT" */,
    menuFtypeLONG                   /**< @brief State string "LONG" */,
    menuFtypeULONG                  /**< @brief State string "ULONG" */,
    menuFtypeINT64                  /**< @brief State string "INT64" */,
    menuFtypeUINT64                 /**< @brief State string "UINT64" */,
    menuFtypeFLOAT                  /**< @brief State string "FLOAT" */,
    menuFtypeDOUBLE                 /**< @brief State string "DOUBLE" */,
    menuFtypeENUM                   /**< @brief State string "ENUM" */
} menuFtype;
/** @brief Number of states defined for menu menuFtype */
#define menuFtype_NUM_CHOICES 12
#endif

#ifndef menuPost_NUM_CHOICES
/** @brief Enumerated type from menu menuPost */
typedef enum {
    menuPost_OnChange               /**< @brief State string "On Change" */,
    menuPost_Always                 /**< @brief State string "Always" */
} menuPost;
/** @brief Number of states defined for menu menuPost */
#define menuPost_NUM_CHOICES 2
#endif

#ifndef menuOmsl_NUM_CHOICES
/** @brief Enumerated type from menu menuOmsl */
typedef enum {
    menuOmslsupervisory             /**< @brief State string "supervisory" */,
    menuOmslclosed_loop             /**< @brief State string "closed_loop" */
} menuOmsl;
/** @brief Number of states defined for menu menuOmsl */
#define menuOmsl_NUM_CHOICES 2
#endif

#ifndef menuPini_NUM_CHOICES
/** @brief Enumerated type from menu menuPini */
typedef enum {
    menuPiniNO                      /**< @brief State string "NO" */,
    menuPiniYES                     /**< @brief State string "YES" */,
    menuPiniRUN                     /**< @brief State string "RUN" */,
    menuPiniRUNNING                 /**< @brief State string "RUNNING" */,
    menuPiniPAUSE                   /**< @brief State string "PAUSE" */,
    menuPiniPAUSED                  /**< @brief State string "PAUSED" */
} menuPini;
/** @brief Number of states defined for menu menuPini */
#define menuPini_NUM_CHOICES 6
#endif

#ifndef menuSimm_NUM_CHOICES
/** @brief Enumerated type from menu menuSimm */
typedef enum {
    menuSimmNO                      /**< @brief State string "NO" */,
    menuSimmYES                     /**< @brief State string "YES" */,
    menuSimmRAW                     /**< @brief State string "RAW" */
} menuSimm;
/** @brief Number of states defined for menu menuSimm */
#define menuSimm_NUM_CHOICES 3
#endif

#ifndef menuAlarmSevr_NUM_CHOICES
/** @brief Enumerated type from menu menuAlarmSevr */
typedef enum {
    menuAlarmSevrNO_ALARM           /**< @brief State string "NO_ALARM" */,
    menuAlarmSevrMINOR              /**< @brief State string "MINOR" */,
    menuAlarmSevrMAJOR              /**< @brief State string "MAJOR" */,
    menuAlarmSevrINVALID            /**< @brief State string "INVALID" */
} menuAlarmSevr;
/** @brief Number of states defined for menu menuAlarmSevr */
#define menuAlarmSevr_NUM_CHOICES 4
#endif

#ifndef menuYesNo_NUM_CHOICES
/** @brief Enumerated type from menu menuYesNo */
typedef enum {
    menuYesNoNO                     /**< @brief State string "NO" */,
    menuYesNoYES                    /**< @brief State string "YES" */
} menuYesNo;
/** @brief Number of states defined for menu menuYesNo */
#define menuYesNo_NUM_CHOICES 2
#endif

#ifndef menuAlarmStat_NUM_CHOICES
/** @brief Enumerated type from menu menuAlarmStat */
typedef enum {
    menuAlarmStatNO_ALARM           /**< @brief State string "NO_ALARM" */,
    menuAlarmStatREAD               /**< @brief State string "READ" */,
    menuAlarmStatWRITE              /**< @brief State string "WRITE" */,
    menuAlarmStatHIHI               /**< @brief State string "HIHI" */,
    menuAlarmStatHIGH               /**< @brief State string "HIGH" */,
    menuAlarmStatLOLO               /**< @brief State string "LOLO" */,
    menuAlarmStatLOW                /**< @brief State string "LOW" */,
    menuAlarmStatSTATE              /**< @brief State string "STATE" */,
    menuAlarmStatCOS                /**< @brief State string "COS" */,
    menuAlarmStatCOMM               /**< @brief State string "COMM" */,
    menuAlarmStatTIMEOUT            /**< @brief State string "TIMEOUT" */,
    menuAlarmStatHWLIMIT            /**< @brief State string "HWLIMIT" */,
    menuAlarmStatCALC               /**< @brief State string "CALC" */,
    menuAlarmStatSCAN               /**< @brief State string "SCAN" */,
    menuAlarmStatLINK               /**< @brief State string "LINK" */,
    menuAlarmStatSOFT               /**< @brief State string "SOFT" */,
    menuAlarmStatBAD_SUB            /**< @brief State string "BAD_SUB" */,
    menuAlarmStatUDF                /**< @brief State string "UDF" */,
    menuAlarmStatDISABLE            /**< @brief State string "DISABLE" */,
    menuAlarmStatSIMM               /**< @brief State string "SIMM" */,
    menuAlarmStatREAD_ACCESS        /**< @brief State string "READ_ACCESS" */,
    menuAlarmStatWRITE_ACCESS       /**< @brief State string "WRITE_ACCESS" */
} menuAlarmStat;
/** @brief Number of states defined for menu menuAlarmStat */
#define menuAlarmStat_NUM_CHOICES 22
#endif


#endif /* INC_menuGlobal_H */
