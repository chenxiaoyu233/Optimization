#ifndef __BAO_SOLVER_H__
#define __BAO_SOLVER_H__

#include "Common.h"

class BaoSolver {
	private:
	int n;
	int edge[40][40];
    int dis[40];
    bool checkIfConnect(int v, uint32_t s);
	int check(uint32_t s, int k);

	public:
	BaoSolver(int n);
	int Solve(int k);
	void AddEdge(int a, int b);
	void AddEdgeByVector(const vector<pair<int, int> > &edges);
};

#endif
