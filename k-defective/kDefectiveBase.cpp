#include "kDefectiveBase.h"

KDefectiveBase::KDefectiveBase(int n): size(n) {
	from = new vector<int>[n];
}

KDefectiveBase::~KDefectiveBase() {
	delete[] from;
}

void KDefectiveBase::AddEdge(int a, int b) {
	from[a].push_back(b);
	from[b].push_back(a);
}

void KDefectiveBase::prework(void *P, void *C) {
	// @todo
}

void KDefectiveBase::init(void *P, void *C) {
	for (int i = 0; i < size; i++) {
		from[i].clear();
	}
	this -> __init__(P, C); 
}

int KDefectiveBase::calcNeedEdge(void *P, void *C, int idx) {
	void *nei = this -> neighborSetOf(idx);
	void *neiInP = this -> setIntersection(P, nei);
	int need = this -> sizeOfSet(P) - this -> sizeOfSet(neiInP);
	this -> deleteSet(nei); this -> deleteSet(neiInP);
	return need;
}

void KDefectiveBase::reductionByEdge(void *P, void *C, int m) {
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		int need = this -> calcNeedEdge(P, C, i);
		if (need > m) this -> removeVertexFromSet(C, i);
	}
}

int KDefectiveBase::calcLimOfDiam(void *P, void *C, int k) {
	int sz = this -> sizeOfSet(P) + 1;
	if (k < sz - 1) return 2;
	// @todo add 周老师提的约束
	return sz - 1; // 大小为sz的图中, 任何简单路径的长度最多为sz-1
}

void KDefectiveBase::calcDisFrom(void *P, void *C, int s) {
	memset(dis, 0x3f, sizeof(int) * size); // 初始化
	queue <int> q; 
	while (!q.empty()) q.pop();
	dis[s] = 0;
	q.push(s);
	while (!q.empty()) {
		int tt = q.front(); q.pop();
		for (int i = 0; i < from[tt].size(); i++) {
			int to = from[tt][i];

			// 保证被删掉的点不会在计算距离时被用到
			if (!this -> existsInSet(C, to) && !this -> existsInSet(P, to)) continue;

			if (dis[to] == 0x3f3f3f3f) {
				dis[to] = dis[tt] + 1;
				q.push(to);
			}
		}
	}
}

void KDefectiveBase::reductionByDiam(void *P, void *C, int k) {
	int maxDiam = this -> calcLimOfDiam(P, C, k);
	for (int i = 0; i < size; i++) if(this -> existsInSet(C, i)) {
		this -> calcDisFrom(P, C, i);
		int diam = 0;
		for (int j = 0; j < size; j++) if(this -> existsInSet(P, j)) diam = max(diam, dis[j]);
		if ( diam > maxDiam ) this -> removeVertexFromSet(C, i);
	}
}

void KDefectiveBase::reductionByConnectToAll(void *P, void *C) {
	int szP = this -> sizeOfSet(P), szC = this -> sizeOfSet(C);
	for (int i = 0; i < size; i++) if(this -> existsInSet(C, i)) {
		void *nei = this -> neighborSetOf(i);
		void *neiP = this -> setIntersection(P, nei);
		void *neiC = this -> setIntersection(C, nei);
		if (this -> sizeOfSet(neiP) + this -> sizeOfSet(neiC) == szP + szC) {
			this -> removeVertexFromSet(C, i);
			this -> addVertexToSet(P, i);
		}
		this -> deleteSet(nei);
		this -> deleteSet(neiP);
		this -> deleteSet(neiC);
	}
}

bool KDefectiveBase::couldRecudeM(void *P, void *C) { 
	bool flag = false;
	int sz = this -> sizeOfSet(P);
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		int need = this -> calcNeedEdge(P, C, i);
		if (need > 0){ flag = true; break; }
	}
	return flag;
}

void KDefectiveBase::branchWhenCouldNotReduceM(void *P, void *C, int k, int m) {
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		this -> removeVertexFromSet(C, i);
		solve(P, C, k, m);
		this -> addVertexToSet(P, i);
		solve(P, C, k, m);

		// recover
		this -> addVertexToSet(C, i);	
		this -> removeVertexFromSet(P, i);

		break;
	}
}

bool KDefectiveBase::cmpForOrder(const pair<int, int> &a, const pair<int, int> &b) {
	return a.second > b.second;
}

void KDefectiveBase::calcBranchOrder(void *P, void *C, vector<pair<int, int> > &order) {
	order.clear();
	for (int i = 0; i < size; i++) if (existsInSet(C, i)) {
		int need = this -> calcNeedEdge(P, C, i);
		order.push_back(make_pair(i, need));
	}
	sort(order.begin(), order.end(), cmpForOrder);
}

void KDefectiveBase::branchWhenCouldReduceM(void *P, void *C, int k, int m) {
	vector<pair<int, int> > order;
	this -> calcBranchOrder(P, C, order);
	this -> removeVertexFromSet(C, order[0].first);
	solve(P, C, k, m);
	for (int i = 0; i < (int)order.size() - 1; i++) {
		int need = order[i].second;
		if (need > m) return; 
		m -= need;
		addVertexToSet(P, order[i].first);

		// 如果是最后一个点, 就不从C中删除了, 不然会少枚举一种情况
		if (i + 1 < (int)order.size() - 1) removeVertexFromSet(C, order[i+1].first);

		solve(P, C, k, m);
	}
}

void KDefectiveBase::solve(void *_P, void *_C, int k, int m) {
	void *P = this -> newSet(), *C = this -> newSet();
	this -> setCopyTo(_P, P); this -> setCopyTo(_C, C);

	// reduction
	this -> reductionByEdge(P, C, m);
	this -> reductionByDiam(P, C, k);
	this -> reductionByConnectToAll(P, C);

	// update ans
	if (sizeOfSet(C) == 0) {
		ans = max(ans, sizeOfSet(P));
		return;
	}

	// branch
	if (couldRecudeM(P, C)) {
		this -> branchWhenCouldReduceM(P, C, k, m);
	} else {
		this -> branchWhenCouldNotReduceM(P, C, k, m);
	}

	// free the menory
	this -> deleteSet(P); this -> deleteSet(C);
}

int KDefectiveBase::Solve(int k) {
	void *P = this -> newSet(), *C = this -> newSet();
	init(P, C);
	prework(P, C);
	solve(P, C, k, k);
	this -> deleteSet(P); this -> deleteSet(C);
	return ans;
}
