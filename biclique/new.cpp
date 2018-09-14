#include <iostream>
#include <cstring>
#include <cstdio>
#include <bitset>
#include <utility>
#include <vector>
#include <algorithm>
#include <ctime>
#include <set>
using namespace std;
const int maxn = 5000;

typedef set<int> vset;

int n; // number of vertex
int m; // number of edge

struct Node{
	int idx, du, rank;
}node[maxn], node_rk[maxn]; // node 编号顺序, node_rk 按度数排名之后
bool cmp_by_du(const Node &a, const Node &b) { return a.du > b.du; }
bool cmp_by_id(const Node &a, const Node &b) { return a.idx < b.idx; }


vector<int> from[maxn];
vector<pair<int, int> > edges;

void read_input(string filePath) {
	FILE *in = fopen(filePath.c_str(), "r");
	char buffer[1005];
	char flag;
	while(fscanf(in, " %c", &flag) != EOF){
		if(flag == 'c') {
			fgets(buffer, 1000, in);
		}else if(flag == 'p') { //  info
			fscanf(in, " %*s%d%d", &n, &m);
			for(int i = 0; i < n; i++) {
				node[i].idx = i;
				node[i].du = 0;
			}
		}else if(flag == 'e') { // edge
			int a, b; fscanf(in, " %d%d", &a, &b);
			--a; --b;
			++ node[a].du; ++ node[b].du;
			edges.push_back(make_pair(a, b));
			from[a].push_back(b); from[b].push_back(a);
		}
	}
}

int firstElementOfBitset(vset &Q) {
	//return *Q.begin();
	for(int i = 0; i < n; i++) {
		if(Q.count(node_rk[i].idx)) return node_rk[i].idx;
	}
}

vset neighbor;
void getNeighbor(int idx) {
	neighbor.clear();
	for(int i = 0; i < from[idx].size(); i++) {
		neighbor.insert(from[idx][i]);
	}
}

vset intersect(const vset *tar, const vset *other) {
	vset ret; ret.clear();
	if(tar -> size() > other -> size()) swap(tar, other);
	for(auto &p: *tar) {
		if(other -> count(p)) {
			ret.insert(p);
		}
	}
	return ret;
}

vset filp_intersect(const vset *tar, const vset *fp) {
	vset ret; ret.clear();
	for(auto &p: *tar) {
		if(!fp -> count(p)) {
			ret.insert(p);
		}
	}
	return ret;
}

pair<vector<int>*, vector<int>*> cliqueSort(vset P) {
	vector<int> *bounds = new vector<int>();
	vector<int> *order = new vector<int>();
	int k = 1;
	while(!P.empty()) {
		vset Q = P;
		while(!Q.empty()) {
			int v = firstElementOfBitset(Q);
			if(P.count(v)) P.erase(v);
			getNeighbor(v);
			Q = intersect(&Q, &neighbor);
			bounds -> push_back(k);
			order -> push_back(v);
		}
		++k;
	}
	return pair<vector<int>*, vector<int>*>(bounds, order);
}

int cnt = 0;

void expand(vset *A, vset *B, vset *Pa, vset *Pb, vset *Am, vset *Bm) {
	++cnt;
	pair<vector<int>*, vector<int>*> ret = cliqueSort(*Pa);
	vector<int> *bounds = ret.first, *order = ret.second;
	int sz = Pa -> size();
	for(int i = sz-1; i >= 0; i--) {
		if((*bounds)[i] + A -> size() > Am -> size() && Pb -> size() + B -> size() > Bm -> size()) {
			int v = (*order)[i];
			if(!A -> count(v)) A -> insert(v); 
			if(Pa -> count(v)) Pa -> erase(v);

			vset *nPa = new vset(), *nPb = new vset();
			getNeighbor(v); 
			(*nPa) = filp_intersect(Pa, &neighbor);
			(*nPb) = intersect(Pb, &neighbor);

			if(A -> size() == B -> size() && A -> size() > Am -> size()) { // update the ans
				*Am = *A; *Bm = *B;
			}

			if(nPb -> size()) expand(B, A, nPb, nPa, Bm, Am);

			delete nPa; delete nPb; // 回收内存空间
			if(A -> count(v)) A -> erase(v);

			if(B -> size() == 0) Pb -> erase(v);
		}
	}
}

vset *A, *B, *Pa, *Pb, *Am, *Bm;

void improvedBiclique() {
	int st = clock();
	A = new vset(); B = new vset();
	Pa = new vset(); Pb = new vset();
	Am = new vset(); Bm = new vset();
	for(int i = 0; i < n; i++) {
		Pa -> insert(i); Pb -> insert(i);
	}
	sort(node, node + n, cmp_by_du);
	for(int i = 0; i < n; i++) {
		node[i].rank = i;
		node_rk[i] = node[i];
	}
	sort(node, node + n, cmp_by_id);

	expand(A, B, Pa, Pb, Am, Bm);
	int ed = clock();
	printf("time: %fs\n", float(ed-st)/CLOCKS_PER_SEC);
}

void print_ans() {
	printf("%d\n", Am -> size() * 2);
	printf("cnt: %d\n", cnt);
}

int main(int argc, char **argv) {
	read_input(string(argv[1]));
	improvedBiclique();
	print_ans();
	return 0;
}
