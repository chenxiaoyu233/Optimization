#ifndef __K_DEFECTIVE_RDS_H__
#define __K_DEFECTIVE_RDS_H__

#include "Common.h"

class KDefectiveRDS {
	protected:
    size_t count;
    clock_t st, ed; // 用于计时
	int size; // 点数
	vector <int> *from; // 邻接链表
	vector <int> order;
	int *LB; // 下界
	int ans; // 当前答案

	// virtual API for set operation
	virtual int sizeOfSet(void *ptr) = 0;
	virtual void addVertexToSet(void *ptr, int idx) = 0;
	virtual void removeVertexFromSet(void *ptr, int idx) = 0;
	virtual void* newSet() = 0;
	virtual void deleteSet(void *ptr) = 0;
	virtual void setCopyTo(void *src, void *dst) = 0;
	virtual bool existsInSet(void *ptr, int idx) = 0;
	virtual void* neighborSetOf(int idx) = 0;
	virtual void* setIntersection(void *A, void *B) = 0;

	void getOrder(vector<int> &vec);
	void solve(void *P, void *C, int k);
	int minInSet(void *C);
	int calcNeedEdge(void *P, void *C, int idx);

	public:
	KDefectiveRDS(int n);
	virtual ~KDefectiveRDS();

	int Solve(int k);
	void AddEdge(int a, int b);
    
    // API for read the Protected var
    virtual size_t GetCount();
};

#endif
