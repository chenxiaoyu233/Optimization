#include <iostream>
#include <cstring>
#include <cstdio>
#include "kDefective.h"
#include "GraphGenerator.h"
#include "BaoSolver.h"
using namespace std;

void *solver;

void solve(string solverT, string filePath, int k) {
	FILE *in = fopen(filePath.c_str(), "r");
	char buffer[1005];
	char flag;
	while(fscanf(in, " %c", &flag) != EOF){
		if(flag == 'c') {
			fgets(buffer, 1000, in);
		}else if(flag == 'p') { //  info
			int n, m;
			fscanf(in, " %*s%d%d", &n, &m);
			if (solverT == "algo") solver = new KDefectiveBitset<bitset<1000> >(n);
			else if (solverT == "bao") solver = new BaoSolver(n);
		}else if(flag == 'e') { // edge
			int a, b; fscanf(in, " %d%d", &a, &b);
			--a; --b; // 从 0 开始编号
			if (solverT == "algo") {
				((KDefectiveBase*) solver) -> AddEdge(a, b);
			} else if(solverT == "bao") {
				((BaoSolver*) solver) -> AddEdge(a, b);
			}
		}
	}
    fclose(in);
	int ans = 0;
	if (solverT == "algo")
		ans = ((KDefectiveBase*) solver) -> Solve(k);
	else if (solverT == "bao")
		ans = ((BaoSolver*) solver) -> Solve(k);
	printf("%d\n", ans);
}

GraphGenerator gen("main.in");

int main(int argc, char** argv) {
	if (argc != 4) {
		puts("wrong number of arguments");
		return 1;
	}
	if (string(argv[1]) == "gen") {
		gen.Generate(atoi(argv[2]), atof(argv[3]), 0.0001);
	} else if (string(argv[1]) == "algo" || string(argv[1]) == "bao") {
		solve(string(argv[1]), string(argv[2]), atoi(argv[3]));
	} else {
		puts("invalid arguments");
	}
	return 0;
}

