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
const int maxn = 400;

typedef bitset<maxn> vset;

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
	for(int i = 0; i < n; i++) if(Q[node_rk[i].idx]) return node_rk[i].idx;
}

vset neighbor;
void getNeighbor(int idx) {
	neighbor.reset();
	for(int i = 0; i < from[idx].size(); i++) {
		neighbor.set(from[idx][i]);
	}
}

pair<vector<int>*, vector<int>*> cliqueSort(bitset<maxn> P) {
	vector<int> *bounds = new vector<int>();
	vector<int> *order = new vector<int>();
	int k = 1;
	while(P.any()) {
		bitset<maxn> Q = P;
		while(Q.any()) {
			int v = firstElementOfBitset(Q);
			P.set(v, 0);
			getNeighbor(v);
			Q &= neighbor;
			bounds -> push_back(k);
			order -> push_back(v);
		}
		++k;
	}
	return pair<vector<int>*, vector<int>*>(bounds, order);
}

int cnt = 0;
int A_cnt, B_cnt, Am_cnt, Bm_cnt; // 维护集合中的元素个数, 避免重复计算
void expand(vset *A, vset *B, vset *Pa, vset *Pb, vset *Am, vset *Bm) {
	++cnt;
	pair<vector<int>*, vector<int>*> ret = cliqueSort(*Pa);
	vector<int> *bounds = ret.first, *order = ret.second;
	int sz = Pa -> count();
	for(int i = sz-1; i >= 0; i--) {
		if((*bounds)[i] + A_cnt > Am_cnt && Pb -> count() + B_cnt > Bm_cnt) {
			int v = (*order)[i];
			A -> set(v); Pa -> set(v, 0);
			++A_cnt;

			vset *nPa = new vset(), *nPb = new vset();
			getNeighbor(v); (*nPb) = *Pb & neighbor;
			neighbor.flip(); (*nPa) = *Pa & neighbor;

			if(A_cnt == B_cnt && A_cnt > Am_cnt) { // update the ans
				*Am = *A; *Bm = *B;
				Am_cnt = Bm_cnt = A_cnt;
			}

			swap(A_cnt, B_cnt);
			if(nPb -> count()) expand(B, A, nPb, nPa, Bm, Am);
			swap(A_cnt, B_cnt);

			delete nPa; delete nPb; // 回收内存空间
			A -> set(v, 0);
			--A_cnt;

			if(B_cnt == 0) Pb -> set(v, 0);
		}else {
			return;
		}
	}
}

vset *A, *B, *Pa, *Pb, *Am, *Bm;

void improvedBiclique() {
	A = new vset(); B = new vset();
	Pa = new vset(); Pb = new vset();
	Am = new vset(); Bm = new vset();
	for(int i = 0; i < n; i++) {
		Pa -> set(i); Pb -> set(i);
	}
	int st = clock();
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
	printf("%d\n", Am -> count() * 2);
	printf("cnt: %d\n", cnt);
}

int main(int argc, char **argv) {
	read_input(string(argv[1]));
	improvedBiclique();
	print_ans();
	return 0;
}
