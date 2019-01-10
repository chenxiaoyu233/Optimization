#ifndef __PRE_WORKER_H__
#define __PRE_WORKER_H__

// 用于对大规模稀疏图进行预处理
// 需要保证:
// 		1. 处理之后没有去掉最优解
// 		2. 处理之后图的规模变小
// 		3. 处理之后对图中的点进行重新编号, 加快求解器中数据结构的速度
// 		4. 因为处理目标为大图且为了方便实现, 直接使用了STL SET实现

#include "Common.h"
#include "kDefectiveBase.h"

class PreWorker: public KDefectiveBase {
	private:
	vector<pair<int, int> > edges;
	set<int> **neiSet;
	set<int> *selected;
    
    void mallocMem();
    void freeMem();
	void cliqueHeu(void *U, int curSize);
	void maxCliqueHeu();

	// 将预处理完之后的边集合返回
	vector<pair<int, int> > getEdges();

	// 将预处理完之后的点的数量返回
	int getVertexNum();

	public:
	PreWorker(int n);
	~PreWorker();

	virtual void AddEdgeByVector(const vector<pair<int, int> > &edges);
	pair<vector<pair<int, int> >, int> Process(int k);
	void deleteIllegalVertex(int k);
};


#endif
