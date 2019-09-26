#ifndef __K_DEFECTIVE_MADEC_H__
#define __K_DEFECTIVE_MADEC_H__

#include "Common.h"
#include "kDefectiveBase.h"

class KDefectiveMADEC: public KDefectiveBase {
private:
    vector<pair<int, int> > edges;
public:
    KDefectiveMADEC(int n);
    bool TimeIsUp();
    virtual void AddEdgeByVector(const vector<pair<int, int> > &edges);
    virtual int Solve(int k);
};

#endif
