// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

#ifndef _MB_H_
#define _MB_H_

#include <shareLib.h>

#ifdef WITH_MICROBENCH

#include <string>
#include <vector>

#include <epicsTypes.h>
#include <epicsVersion.h>

#include <boost/atomic.hpp>

#ifndef EPICS_VERSION_INT
// Base-3.14 doesn't provide these:
typedef long long epicsInt64;
typedef unsigned long long epicsUInt64;
#endif

struct MBPoint
{
    std::ptrdiff_t id;
    epicsUInt8 stage;
    epicsUInt64 time;
    
    MBPoint() {}
    MBPoint(std::ptrdiff_t _id, epicsUInt8 _stage) : id(_id), stage(_stage) {}
};

struct MBEntity;

epicsShareFunc void MBEntityExport(MBEntity *e);

#if defined(_WIN32) && !defined (_MINGW)
// Visual C++ complains without these decorations:
#define MBCLASS class epicsShareClass
#else
#define MBCLASS
#endif

typedef MBCLASS std::allocator<MBPoint> MBPointAllocType;
typedef MBCLASS std::vector<MBPoint, MBPointAllocType> MBPointType;
typedef MBCLASS boost::atomic<std::size_t> MBPosType;
typedef MBCLASS boost::atomic<std::ptrdiff_t> MBAutoIdType;

struct epicsShareClass MBEntity
{
    const char * name;
    MBPointType points;
    MBPosType pos;
    MBAutoIdType auto_id;

    MBEntity(const char * name_, std::size_t size) : name(name_)
    {
        // init vector at the beginning
        points.resize(size);
        pos.store(0);
        auto_id.store(0);
    }
    ~MBEntity();
};

epicsShareFunc epicsUInt64 MBTime();

epicsShareFunc void MBPointAdd(MBEntity &e, std::ptrdiff_t id, epicsUInt8 stage);

epicsShareFunc void MBCSVExport(MBEntity &e, epicsUInt8 stageOnly, std::size_t skipFirstNSamples, std::ostream &o);
epicsShareFunc void MBCSVImport(MBEntity &e, std::istream &i);

epicsShareFunc void MBStats(MBEntity &e, epicsUInt8 stageOnly, std::size_t skipFirstNSamples, std::ostream &o);

epicsShareFunc void MBNormalize(MBEntity &e);


#define MB_NAME(NAME) g_MB_##NAME

#define MB_DECLARE(NAME, SIZE) MBEntity MB_NAME(NAME)(#NAME, SIZE)
#define MB_DECLARE_EXTERN(NAME) extern MBEntity MB_NAME(NAME)

#define MB_POINT_ID(NAME, STAGE, STAGE_DESC, ID) MBPointAdd(MB_NAME(NAME), ID, STAGE)

#define MB_INC_AUTO_ID(NAME) MB_NAME(NAME).auto_id++
#define MB_POINT(NAME, STAGE, STAGE_DESC) MBPointAdd(MB_NAME(NAME), MB_NAME(NAME).auto_id, STAGE)
#define MB_POINT_CONDITIONAL(NAME, STAGE, STAGE_DESC, COND) if (COND) MBPointAdd(MB_NAME(NAME), MB_NAME(NAME).auto_id, STAGE)

#define MB_NORMALIZE(NAME) MBNormalize(MB_NAME(NAME))

#define MB_STATS(NAME, STREAM) MBStats(MB_NAME(NAME), 0, 0, STREAM)
#define MB_STATS_OPT(NAME, STAGE_ONLY, SKIP_FIRST_N_SAMPLES, STREAM) MBStats(MB_NAME(NAME), STAGE_ONLY, SKIP_FIRST_N_SAMPLES, STREAM)

#define MB_CSV_EXPORT(NAME, STREAM) MBCSVExport(MB_NAME(NAME), 0, 0, STREAM)
#define MB_CSV_EXPORT_OPT(NAME, STAGE_ONLY, SKIP_FIRST_N_SAMPLES, STREAM) MBCSVExport(MB_NAME(NAME), STAGE_ONLY, SKIP_FIRST_N_SAMPLES, STREAM)
#define MB_CSV_IMPORT(NAME, STREAM) MBCSVImport(MB_NAME(NAME), STREAM)

#define MB_PRINT(NAME, STREAM) MB_CSV_EXPORT(NAME, STREAM)
#define MB_PRINT_OPT(NAME, STAGE_ONLY, SKIP_FIRST_N_SAMPLES, STREAM) MB_CSV_EXPORT_OPT(NAME, STAGE_ONLY, SKIP_FIRST_N_SAMPLES, STREAM)

#define MB_INIT MBInit()

#endif // WITH_MICROBENCH

epicsShareFunc void MBInit();

#endif
