/* shortRecord.c */
/* Example record support module */
  
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "dbEvent.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "devSup.h"
#include "errMdef.h"
#include "recSup.h"
#include "special.h"
#define GEN_SIZE_OFFSET
#include "shortRecord.h"
#undef  GEN_SIZE_OFFSET
#include "epicsExport.h"

/* Create RSET - Record Support Entry Table */
#define report NULL
#define initialize NULL
static long init_record();
static long process();
#define special NULL
#define get_value NULL
#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
static long get_units();
static long get_precision();
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
static long get_graphic_double();
static long get_control_double();
static long get_alarm_double();
 
rset shortRSET={
	RSETNUMBER,
	report,
	initialize,
	init_record,
	process,
	special,
	get_value,
	cvt_dbaddr,
	get_array_info,
	put_array_info,
	get_units,
	get_precision,
	get_enum_str,
	get_enum_strs,
	put_enum_str,
	get_graphic_double,
	get_control_double,
	get_alarm_double
};
epicsExportAddress(rset,shortRSET);

static void checkAlarms(shortRecord *pshort);
static void monitor(shortRecord *pshort);

static long init_record(void *precord,int pass)
{
    return(0);
}

static long process(void *precord)
{
	shortRecord	*pshort = (shortRecord *)precord;

	pshort->pact = TRUE;

	recGblGetTimeStamp(pshort);
	/* check for alarms */
	checkAlarms(pshort);
	/* check event list */
	monitor(pshort);
	/* process the forward scan link record */
        recGblFwdLink(pshort);

	pshort->pact=FALSE;
	return(0);
}

static long get_units(DBADDR *paddr, char *units)
{
    shortRecord	*pshort=(shortRecord *)paddr->precord;

    strncpy(units,pshort->egu,DB_UNITS_SIZE);
    return(0);
}

static long get_precision(DBADDR *paddr, long *precision)
{
    shortRecord	*pshort=(shortRecord *)paddr->precord;

    *precision = pshort->prec;
    if(paddr->pfield == (void *)&pshort->val) return(0);
    recGblGetPrec(paddr,precision);
    return(0);
}

static long get_graphic_double(DBADDR *paddr,struct dbr_grDouble *pgd)
{
    shortRecord	*pshort=(shortRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == shortRecordVAL
    || fieldIndex == shortRecordHIHI
    || fieldIndex == shortRecordHIGH
    || fieldIndex == shortRecordLOW
    || fieldIndex == shortRecordLOLO
    || fieldIndex == shortRecordHOPR
    || fieldIndex == shortRecordLOPR) {
        pgd->upper_disp_limit = pshort->hopr;
        pgd->lower_disp_limit = pshort->lopr;
    } else recGblGetGraphicDouble(paddr,pgd);
    return(0);
}

static long get_control_double(DBADDR *paddr,struct dbr_ctrlDouble *pcd)
{
    shortRecord	*pshort=(shortRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == shortRecordVAL
    || fieldIndex == shortRecordHIHI
    || fieldIndex == shortRecordHIGH
    || fieldIndex == shortRecordLOW
    || fieldIndex == shortRecordLOLO) {
	pcd->upper_ctrl_limit = pshort->hopr;
	pcd->lower_ctrl_limit = pshort->lopr;
    } else recGblGetControlDouble(paddr,pcd);
    return(0);
}

static long get_alarm_double(DBADDR *paddr,struct dbr_alDouble *pad)
{
    shortRecord	*pshort=(shortRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == shortRecordVAL) {
	pad->upper_alarm_limit = pshort->hihi;
	pad->upper_warning_limit = pshort->high;
	pad->lower_warning_limit = pshort->low;
	pad->lower_alarm_limit = pshort->lolo;
    } else recGblGetAlarmDouble(paddr,pad);
    return(0);
}

static void checkAlarms(shortRecord *pshort)
{
	double		val;
	float		hyst, lalm, hihi, high, low, lolo;
	unsigned short	hhsv, llsv, hsv, lsv;

	if(pshort->udf == TRUE ){
		recGblSetSevr(pshort,UDF_ALARM,INVALID_ALARM);
		return;
	}
	hihi = pshort->hihi; lolo = pshort->lolo; high = pshort->high; low = pshort->low;
	hhsv = pshort->hhsv; llsv = pshort->llsv; hsv = pshort->hsv; lsv = pshort->lsv;
	val = pshort->val; hyst = pshort->hyst; lalm = pshort->lalm;

	/* alarm condition hihi */
	if (hhsv && (val >= hihi || ((lalm==hihi) && (val >= hihi-hyst)))){
	        if (recGblSetSevr(pshort,HIHI_ALARM,pshort->hhsv)) pshort->lalm = hihi;
		return;
	}

	/* alarm condition lolo */
	if (llsv && (val <= lolo || ((lalm==lolo) && (val <= lolo+hyst)))){
	        if (recGblSetSevr(pshort,LOLO_ALARM,pshort->llsv)) pshort->lalm = lolo;
		return;
	}

	/* alarm condition high */
	if (hsv && (val >= high || ((lalm==high) && (val >= high-hyst)))){
	        if (recGblSetSevr(pshort,HIGH_ALARM,pshort->hsv)) pshort->lalm = high;
		return;
	}

	/* alarm condition low */
	if (lsv && (val <= low || ((lalm==low) && (val <= low+hyst)))){
	        if (recGblSetSevr(pshort,LOW_ALARM,pshort->lsv)) pshort->lalm = low;
		return;
	}

	/* we get here only if val is out of alarm by at least hyst */
	pshort->lalm = val;
	return;
}

static void monitor(shortRecord *pshort)
{
	unsigned short	monitor_mask;
	double		delta;

        monitor_mask = recGblResetAlarms(pshort);
	/* check for value change */
	delta = pshort->mlst - pshort->val;
	if(delta<0.0) delta = -delta;
	if (delta > pshort->mdel) {
		/* post events for value change */
		monitor_mask |= DBE_VALUE;
		/* update last value monitored */
		pshort->mlst = pshort->val;
	}

	/* check for archive change */
	delta = pshort->alst - pshort->val;
	if(delta<0.0) delta = -delta;
	if (delta > pshort->adel) {
		/* post events on value field for archive change */
		monitor_mask |= DBE_LOG;
		/* update last archive value monitored */
		pshort->alst = pshort->val;
	}

	/* send out monitors connected to the value field */
	if (monitor_mask){
		db_post_events(pshort,&pshort->val,monitor_mask);
	}
	return;
}
