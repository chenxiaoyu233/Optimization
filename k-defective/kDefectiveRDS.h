#ifndef __K_DEFECTIVE_RDS_H__
#define __K_DEFECTIVE_RDS_H__

#include "Common.h"
#include "kDefectiveBase.h"

class KDefectiveRDS: public KDefectiveBase {
	protected:
	vector <int> order;
	int *LB; // 下界

	virtual void reductionByConnective(void *P, void *C);
	void getOrder(vector<int> &vec);
	virtual void solve(void *P, void *C, int k);
	int minInSet(void *C);

	public:
	KDefectiveRDS(int n);
	virtual ~KDefectiveRDS();

	virtual int Solve(int k);
};

#endif
