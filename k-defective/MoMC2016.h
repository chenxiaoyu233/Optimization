#ifndef __MoMC2016_H__
#define __MoMC2016_H__

#include "kDefectiveR2MC.h"

extern KDefectiveR2MC *shell;
void MOMCSolver();

#define TIME_CUTTER do {\
    if (shell -> timeIsUp()) {\
        shell -> notFinish = true;\
        return 0;\
    }\
} while (0)

#endif
