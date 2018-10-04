#ifndef __GRAPH_GENERATOR_H__
#define __GRAPH_GENERATOR_H__

#include "Common.h"

class GraphGenerator {
    private:
	string fileName; // 输出路径
	vector<pair<int, int> > edges; // 使用内部形式存储边

	size_t randRange(int l, int r);
	// default for DIMACS dataset
	virtual void output(int n); 
    public:
    GraphGenerator(string filePath);
	void Generate(int n, double density, double eps);
};

#endif
