#ifndef __K_DEFECTIVE_MADEC_H__
#define __K_DEFECTIVE_MADEC_H__

#include "Common.h"
#include "kDefectiveBase.h"

class KDefectiveMADEC: public KDefectiveBase {
private:
    vector<pair<int, int> > edges;
public:
    bool diamFlag, colorFlag;
    KDefectiveMADEC(int n);
    bool TimeIsUp();
    virtual void AddEdgeByVector(const vector<pair<int, int> > &edges);
    virtual int Solve(int k);

	// 设置是否使用直径规约 (默认启用)
	virtual void EnableDiamReduction(bool isEnable);
	// 设置是否使用染色规约 (默认启用)
	virtual void EnableColoringReduction(bool isEnable);
};

#endif
