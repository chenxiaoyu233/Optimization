#include <iostream>
#include <cstring>
#include <cstdio>
#include "kDefective.h"
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
			if (solverT == "algo") solver = new MyBitSetImplement<MyBitSet<1000>, KDefectiveBase>(n);
			else if (solverT == "bao") solver = new BaoSolver(n);
            else if (solverT == "RDS") solver = new BitSetImplement<bitset<1000>, KDefectiveRDS>(n);
            else if (solverT == "Simple") solver = new MyBitSetImplement<MyBitSet<1000>, KDefectiveBase>(n);
			else if (solverT == "Prework") solver = new STLSetImplement<set<int>, PreWorker>(n);
		}else if(flag == 'e') { // edge
			int a, b; fscanf(in, " %d%d", &a, &b);
			--a; --b; // 从 0 开始编号
			if (solverT == "algo") {
				((KDefectiveBase*) solver) -> AddEdge(a, b);
			} else if (solverT == "bao") {
				((BaoSolver*) solver) -> AddEdge(a, b);
            } else if (solverT == "RDS") {
                ((KDefectiveRDS*) solver) -> AddEdge(a, b);
            } else if (solverT == "Simple") {
                ((KDefectiveSimple*) solver) -> AddEdge(a, b);
            } else if (solverT == "Prework") {
				((PreWorker*) solver) -> AddEdge(a, b);
			}
		}
	}
    fclose(in);
	int ans = 0;
	if (solverT == "algo")
		ans = ((KDefectiveBase*) solver) -> Solve(k);
	else if (solverT == "bao")
		ans = ((BaoSolver*) solver) -> Solve(k);
    else if (solverT == "RDS")
        ans = ((KDefectiveRDS*) solver) -> Solve(k);
    else if (solverT == "Simple")
        ans = ((KDefectiveSimple*) solver) -> Solve(k);
    else if (solverT == "Prework")
		((PreWorker*) solver) -> Process(k);
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
	} else if (
               string(argv[1]) == "algo" ||
               string(argv[1]) == "bao" ||
               string(argv[1]) == "RDS" ||
               string(argv[1]) == "Simple" ||
			   string(argv[1]) == "Prework"
    ) {
		solve(string(argv[1]), string(argv[2]), atoi(argv[3]));
	} else if (string(argv[1]) == "draw") {
		Compare *compare = new Compare(string(argv[2]));
		compare -> Run(30, atof(argv[3]));
	} else {
		puts("invalid arguments");
	}
	return 0;
}

