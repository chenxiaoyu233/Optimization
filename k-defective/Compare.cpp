#include "Compare.h"

Compare::Compare(string fileName): GraphGenerator(fileName) { }

void Compare::buildGraphInSolver() {
	for (int i = 0; i < edges.size(); i++) {
		solverBase -> AddEdge(edges[i].first, edges[i].second);
		solverSimple -> AddEdge(edges[i].first, edges[i].second);
		solverRDS -> AddEdge(edges[i].first, edges[i].second);
	}
}


void Compare::run(int n) {
	clock_t st, ed;
#define RunLoop(solver, vecTime, vecCnt, st, ed) do {\
    vecTime.clear(); vecCnt.clear();\
	for (int i = 1; i <= 5; i++) {\
		st = clock();\
		if (i * n <= n * (n-1) / 2) solver -> Solve(i * n);\
		ed = clock();\
		vecTime.push_back( double(ed - st) / CLOCKS_PER_SEC * 1000);\
        vecCnt.push_back(solver -> GetCount());\
	}\
    puts("");\
}while(0)
	RunLoop(solverBase, timeBase, countBase, st, ed);
	RunLoop(solverSimple, timeSimple, countSimple, st, ed);
	RunLoop(solverRDS, timeRDS, countRDS, st, ed);

	// output the result
	FILE *out = fopen("draw.raw", "w");
#define PrintLoop(result, format) do {\
	for (int i = 0; i < result.size(); i++) {\
        fprintf(out, ""format" ", result[i]);\
	}\
	fprintf(out, "\n");\
}while(0)
	PrintLoop(timeBase, "%lf");
	PrintLoop(timeSimple, "%lf");
	PrintLoop(timeRDS, "%lf");
    PrintLoop(countBase, "%lu");
    PrintLoop(countSimple, "%lu");
    PrintLoop(countRDS, "%lu");
}

void Compare::Run(int n, double density) {
	solverBase = new BitSetImplement<bitset<1000>, KDefectiveBase>(n);
	solverSimple = new BitSetImplement<bitset<1000>, KDefectiveSimple>(n);
	solverRDS = new BitSetImplement<bitset<1000>, KDefectiveRDS>(n);

	this -> Generate(n, density, 0.0001);
	this -> buildGraphInSolver();
	this -> run(n);

	delete solverBase;
	delete solverSimple;
	delete solverRDS;
}
