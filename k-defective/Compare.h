#ifndef __COMPARE_H__
#define __COMPARE_H__

#include "Common.h"
#include "kDefectiveBase.h"
#include "kDefectiveSimple.h"
#include "kDefectiveRDS.h"
#include "BitSetImplement.h"
#include "GraphGenerator.h"

class Compare: public GraphGenerator {
	protected:
	// solver pointers
	BitSetImplement<bitset<128>, KDefectiveBase> *solverBase;
	BitSetImplement<bitset<128>, KDefectiveSimple> *solverSimple;
	BitSetImplement<bitset<128>, KDefectiveRDS> *solverRDS;

	// time result container
	vector <double> timeBase;
	vector <double> timeSimple;
	vector <double> timeRDS;
    
    // search tree size result container
    vector <size_t> countBase;
    vector <size_t> countSimple;
    vector <size_t> countRDS;

	void buildGraphInSolver();
	void run(int n);

	public:
	Compare(string fileName);
	void Run(int n, double density);
};

#endif
