#ifndef _MB_H_
#define _MB_H_

#ifdef WITH_MICROBENCH

#include <string>
#include <vector>

// http://stackoverflow.com/questions/13266868/ffmpeg-inttypes-h-not-found-error
#ifndef HAS_INT_TYPES

#ifdef HAS_C99
  #include <stdint.h>
#endif

#endif

#include <iostream>

#include <boost/atomic.hpp>


#include <shareLib.h>

class epicsShareClass MBMutexInitializer {
  public:
    MBMutexInitializer ();
    ~MBMutexInitializer ();
};

epicsShareExtern MBMutexInitializer mbStaticMutexInitializer; // Note object here in the header.

struct MBPoint
{
    std::ptrdiff_t id;
    uint8_t stage;
    uint64_t time;
    
    MBPoint() {}
    MBPoint(std::ptrdiff_t _id, uint8_t _stage) : id(_id), stage(_stage) {}
};

struct MBEntity;

epicsShareExtern void MBEntityRegister(MBEntity *e);

typedef std::vector<MBPoint> MBPointType;

struct MBEntity
{
    std::string name;
    MBPointType points;
    boost::atomic<std::size_t> pos;
    boost::atomic<std::ptrdiff_t> auto_id;

    MBEntity(const std::string &name_, std::size_t size) : name(name_)
    {
        // init vector at the beginning
        points.resize(size);
        pos.store(0);
        auto_id.store(0);
        
        MBEntityRegister(this);
    }
};

epicsShareExtern uint64_t MBTime();

epicsShareExtern void MBPointAdd(MBEntity &e, std::ptrdiff_t id, uint8_t stage);

epicsShareExtern void MBCSVExport(MBEntity &e, uint8_t stageOnly, std::size_t skipFirstNSamples, std::ostream &o);
epicsShareExtern void MBCSVImport(MBEntity &e, std::istream &i);

epicsShareExtern void MBStats(MBEntity &e, uint8_t stageOnly, std::size_t skipFirstNSamples, std::ostream &o);

epicsShareExtern void MBNormalize(MBEntity &e);


epicsShareExtern void MBInit();


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


#else

#define MB_DECLARE(NAME, SIZE) 
#define MB_DECLARE_EXTERN(NAME)

#define MB_POINT_ID(NAME, STAGE, STAGE_DESC, ID)

#define MB_INC_AUTO_ID(NAME)
#define MB_POINT(NAME, STAGE, STAGE_DESC)

#define MB_POINT_CONDITIONAL(NAME, STAGE, STAGE_DESC, COND)

#define MB_NORMALIZE(NAME)

#define MB_STATS(NAME, STREAM)
#define MB_STATS_OPT(NAME, STAGE_ONLY, SKIP_FIRST_N_SAMPLES, STREAM)

#define MB_CSV_EXPORT(NAME, STREAM)
#define MB_CSV_EXPORT_OPT(NAME, STAGE_ONLY, SKIP_FIRST_N_SAMPLES, STREAM)
#define MB_CSV_IMPORT(NAME, STREAM)

#define MB_PRINT(NAME, STREAM)
#define MB_PRINT_OPT(NAME, STAGE_ONLY, SKIP_FIRST_N_SAMPLES, STREAM)

#define MB_INIT

#endif

#endif
