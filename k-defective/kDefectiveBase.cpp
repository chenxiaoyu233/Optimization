#include "kDefectiveBase.h"

KDefectiveBase::KDefectiveBase(int n): size(n) {
	from = new vector<int>[n];
	for (int i = 0; i < size; i++) {
		from[i].clear();
	}
	dis = new int[n];
}

KDefectiveBase::~KDefectiveBase() {
	delete[] from;
	delete[] dis;
}

void KDefectiveBase::AddEdge(int a, int b) {
	from[a].push_back(b);
	from[b].push_back(a);
}

void KDefectiveBase::prework(void *_P, void *_C, int k) {
	void *P = this -> newSet(), *C = this -> newSet();
	this -> setCopyTo(_P, P); this -> setCopyTo(_C, C);

	vector <int> vec; vec.clear();
	for (int i = 0; i < size; i++) vec.push_back(i);
	random_shuffle(vec.begin(), vec.end());
	for (int i = 0; i < size; i++) {
		int need = this -> calcNeedEdge(P, C, i);
		if (need > k) continue;
		k -= need;
		this -> addVertexToSet(P, i);
	}
	ans = max(ans, this -> sizeOfSet(P));

	this -> deleteSet(P);
	this -> deleteSet(C);
}

void KDefectiveBase::init(void *P, void *C) {
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
	int sz = this -> sizeOfSet(P) + this -> sizeOfSet(C);
	if (k < sz - 1) return 2;

	// 根据保证连通这个前提计算出的直径大小 -> 待验算
	float delta = 4 * sz * sz - 12 * sz + 17 - 8 * k;
	if (delta >= 0) {
		return ((2 * (float)sz + 1) - sqrt(delta))/(float)2;
	}

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
		if (this -> sizeOfSet(neiP) + this -> sizeOfSet(neiC) == szP + szC - 1) { 
			// 减去1, 因为在计算的时候没有包含自己
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
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		int need = this -> calcNeedEdge(P, C, i);
		if (need > 0){ flag = true; break; }
	}
	return flag;
}

/*void KDefectiveBase::branchWhenCouldNotReduceM(void *P, void *C, int k, int m) {
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		this -> removeVertexFromSet(C, i);
		this -> addVertexToSet(P, i);
		solve(P, C, k, m);
		this -> removeVertexFromSet(P, i);
		solve(P, C, k, m);

		// recover
		this -> addVertexToSet(C, i);	
		this -> removeVertexFromSet(P, i);

		break;
	}
}*/
void KDefectiveBase::branchWhenCouldNotReduceM(void *P, void *C, int k, int m) {
	int whe = -1, maxv = -1;
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		void *nei = this -> neighborSetOf(i);
		void *neiC = this -> setIntersection(C, nei);
		int sz = this -> sizeOfSet(neiC);
		if (sz > maxv) {
			maxv = sz; whe = i;
		}
		this -> deleteSet(nei);
		this -> deleteSet(neiC);
	}
	this -> removeVertexFromSet(C, whe);
	this -> addVertexToSet(P, whe);
	solve(P, C, k, m);
	this -> removeVertexFromSet(P, whe);
	solve(P, C, k, m);

	this -> addVertexToSet(C, whe);
}

bool KDefectiveBase::cmpForOrder(const pair<int, int> &a, const pair<int, int> &b) {
	return a.second > b.second;
}

void KDefectiveBase::calcBranchOrder(void *P, void *C, vector<pair<int, int> > &order) {
	order.clear();
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		int need = this -> calcNeedEdge(P, C, i);
		order.push_back(make_pair(i, need));
	}
	sort(order.begin(), order.end(), cmpForOrder);
}

/*void KDefectiveBase::branchWhenCouldReduceM(void *P, void *C, int k, int m) {
	vector<pair<int, int> > order;
	this -> calcBranchOrder(P, C, order);
	this -> removeVertexFromSet(C, order[0].first);
	solve(P, C, k, m);
	for (int i = 0; i < (int)order.size() - 1; i++) {
		int need = this -> calcNeedEdge(P, C, order[i].first); // 这里需要重新计算
		if (need > m) return; 
		m -= need;
		addVertexToSet(P, order[i].first);
		removeVertexFromSet(C, order[i+1].first);
		solve(P, C, k, m);
	}
	int last = (int)order.size() - 1;
	int need = this -> calcNeedEdge(P, C, order[last].first);
	if (need > m) return;
	m -= need;
	addVertexToSet(P, order[last].first);
	solve(P, C, k, m);
}*/
void KDefectiveBase::branchWhenCouldReduceM(void *P, void *C, int k, int m) {
	vector <pair<int, int> > order;
	this -> calcBranchOrder(P, C, order);

	// 计算需要边数的前缀和并存在 order[i].second 中
	for (int i = 0, sum = 0; i < order.size(); i++) {
		int need = this -> calcNeedEdge(P, C, order[i].first);
		sum += need; 
		order[i].second = sum;
		this -> addVertexToSet(P, order[i].first);
	}

	// 将之前的计算还原
	for (int i = 0; i < order.size(); i++)
		this -> removeVertexFromSet(P, order[i].first);

	order.push_back(make_pair(-1, m + 1)); // 放一个标兵
	// 进行最后一个分支, 比较特殊, 需要专门写
	int endPos = -1;
	for (int i = 0; i < order.size(); i++) {
		if (order[i].second > m) { endPos = i; break; }
		this -> addVertexToSet(P, order[i].first);
		this -> removeVertexFromSet(C, order[i].first);
	}
	if (endPos == -1) return;
	endPos -= 1;
	solve(P, C, k, m - order[endPos].second);

	//解决其他分支
	for (int i = endPos; i >= 0; i--) {
		if (i < endPos) this -> addVertexToSet(C, order[i+1].first);
		this -> removeVertexFromSet(P, order[i].first);
		if (i > 0) solve(P, C, k, m - order[i-1].second);
		else solve(P, C, k, m);
	}
}

void KDefectiveBase::solve(void *_P, void *_C, int k, int m) {
	void *P = this -> newSet(), *C = this -> newSet();
	this -> setCopyTo(_P, P); this -> setCopyTo(_C, C);

	// reduction
	this -> reductionByEdge(P, C, m);
	this -> reductionByDiam(P, C, k);
	this -> reductionByConnectToAll(P, C);

	// cut brunch
    //printf("sizeof(P): %d, sizeof(C): %d\n", sizeOfSet(P), sizeOfSet(C));
	if (sizeOfSet(P) + sizeOfSet(C) <= ans) return;
	
	// update ans
	if (sizeOfSet(C) == 0) {
		ans = max(ans, sizeOfSet(P));
		printf("new ans: %d\n", ans);
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
    clock_t st, ed;
    st = clock();
    ans = 0; // init the value of the ans
	void *P = this -> newSet(), *C = this -> newSet();
	init(P, C);
	for (int i = 0; i < 500; i++) prework(P, C, k);
    printf("new ans: %d\n", ans);
	solve(P, C, k, k);
	this -> deleteSet(P); this -> deleteSet(C);
    ed = clock();
    printf("time: %fms\n", float(ed-st)/CLOCKS_PER_SEC * 1000);
	return ans;
}
