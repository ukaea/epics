#include <iostream>
#include <pv/mb.h>

MB_DECLARE_EXTERN(test);
MB_DECLARE(test, 10000);

int main()
{
    MB_INIT;
    
    for (int id = 0; id < 1000; id++)
        for (int stage = 0; stage < 10; stage++)
            MB_POINT_ID(test, stage, "stage desc", id);

    return 0;
}

