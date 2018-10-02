#include <iostream>
#include <cstring>
#include <cstdio>
#include "kDefective.h"
using namespace std;

KDefectiveBase *solver;

void read_input(string filePath) {
	FILE *in = fopen(filePath.c_str(), "r");
	char buffer[1005];
	char flag;
	while(fscanf(in, " %c", &flag) != EOF){
		if(flag == 'c') {
			fgets(buffer, 1000, in);
		}else if(flag == 'p') { //  info
			int n, m;
			fscanf(in, " %*s%d%d", &n, &m);
			solver = new KDefectiveBitset<bitset<1000> >(n);
		}else if(flag == 'e') { // edge
			int a, b; fscanf(in, " %d%d", &a, &b);
			solver -> AddEdge(a, b);
		}
	}
}

int main(int argc, char** argv) {
	if (argc != 3) {
		puts("wrong number of arguments");
		return 1;
	}
	read_input(string(argv[1]));
	solver -> Solve(atoi(argv[2]));
	return 0;
}

