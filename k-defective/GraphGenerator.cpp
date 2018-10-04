#include "GraphGenerator.h"

GraphGenerator::GraphGenerator(string filePath) : fileName(filePath) { 
	srand(time(NULL)); // 随便设置一个种子
}

size_t GraphGenerator::randRange(int l, int r) {
	if (l > r) swap(l, r);
	int del = r - l + 1;
	return rand() % del + l;
}

void GraphGenerator::output(int n) {
	FILE *out = fopen(fileName.c_str(), "w");
	fprintf(out, "p edge %d %d\n", n, (int)edges.size());
	for (int i = 0; i < edges.size(); i++) {
		fprintf(out, "e %d %d\n", edges[i].first + 1, edges[i].second + 1);
	}
	fclose(out);
}

void GraphGenerator::Generate(int n, double density, double eps) {
	edges.clear();
	size_t tot = 1 / eps;
	size_t den = density / eps;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			size_t cur = randRange(0, tot);
			if (cur < den) edges.push_back(make_pair(i, j));
		}
	}
	this -> output(n);
}

