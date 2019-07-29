#ifndef __MoMC2016_H__
#define __MoMC2016_H__

extern KDefectiveR2MC *shell;
void MOMCSolver();

#define TIME_CUTTER do {\
    if (shell -> timeIsUp()) {\
        shell -> notFinish = true;\
        return;\
    }\
} while (0)

#endif
