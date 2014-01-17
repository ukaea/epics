#include <iostream>
#include <fstream>
#include <cstring>

#include <epicsGetopt.h>
#include <epicsTypes.h>
#include "mb.h"

// NOTE: this can be replaced with epicsScanLong in 3.15
#include <errno.h>
int
parseLong(const char *str, long *to, int base)
{
    int c;
    char *endp;
    long value;

    while ((c = *str) && isspace(c))
        ++str;

    errno = 0;
    value = strtol(str, &endp, base);

    if (endp == str)
        return 1;
    if (errno == EINVAL)    /* Not universally supported */
        return 2;
    if (errno == ERANGE)
        return 3;

    while ((c = *endp) && isspace(c))
        ++endp;
    if (c)
        return 4;

    *to = value;
    return 0;
}


void usage (void)
{
    fprintf (stderr, "\nUsage: mb_stat [options] <micro-benchmark .csv filename>\n\n"
    "  -h: Help: Print this message\n"
    "\noptions:\n"
    "  -s <stage>:          Process only selected stage, valid range: (0..255].\n"
    "  -i <# of samples>:   Ignore first n samples, defaults to 0.\n"
    "  -n:                  Normalize timestamp (convert timestamps to time spent per stage).\n"
    "\n"
    );
}

MB_DECLARE(importCSV, 64000);

int main(int argc, char** argv)
{
    int opt;                    /* getopt() current option */
    epicsInt8 stageOnly = 0; /* default is all, 0 stage is not valid option */
    std::size_t ignoreFirstNSamples = 0; /* default is not to ignore anything */
    bool normalizeOnly = false; /* default is to point statistics */

    while ((opt = getopt(argc, argv, ":hs:i:n")) != -1) {
        switch (opt) {
        case 'h':               /* Print usage */
            usage();
            return 0;
        case 's':               /* Select stage */
            long sv;
            if(parseLong(optarg, &sv, 10) != 0 || sv <= 0)
            {
                fprintf(stderr, "'%s' is not a valid stage value "
                        "- ignored. ('mb_stat -h' for help.)\n", optarg);
            }
            else
                stageOnly = static_cast<std::size_t>(sv);
            break;
        case 'i':               /* Ignore first N samples */
            long iv;
            if(parseLong(optarg, &iv, 10) != 0 || iv < 0)
            {
                fprintf(stderr, "'%s' is not a valid non-negative integer value "
                        "- ignored. ('mb_stat -h' for help.)\n", optarg);
            }
            else
                ignoreFirstNSamples = static_cast<std::size_t>(iv);
            break;
        case 'n':               /* Service name */
            normalizeOnly = true;
            break;
        case '?':
            fprintf(stderr,
                    "Unrecognized option: '-%c'. ('mb_stat -h' for help.)\n",
                    optopt);
            return 1;
        case ':':
            fprintf(stderr,
                    "Option '-%c' requires an argument. ('mb_stat -h' for help.)\n",
                    optopt);
            return 1;
        default :
            usage();
            return 1;
        }
    }

    int nFiles = argc - optind;       /* Remaining arg list are list of files */
    if (nFiles != 1)
    {
        fprintf(stderr, "One micro-benchmark .csv filename needs to be specified. ('mb_stat -h' for help.)\n");
        return 1;
    }

    
    char * fileName = argv[optind];
    
    std::ifstream in(fileName);
    if (in.is_open())
    {
        MB_CSV_IMPORT(importCSV, in);
        in.close();

        if (normalizeOnly)
        {
            MB_NORMALIZE(importCSV);
            MB_PRINT_OPT(importCSV, stageOnly, ignoreFirstNSamples, std::cout);
        }
        else
            MB_STATS_OPT(importCSV, stageOnly, ignoreFirstNSamples, std::cout);
    }
    else
    {
        std::cerr << "failed to open a file " << fileName << ", skipping..." << std::endl;
        return -1;
    }
    
    return 0;
}

