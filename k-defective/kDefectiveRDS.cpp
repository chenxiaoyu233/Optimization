#include "kDefectiveRDS.h"

KDefectiveRDS::KDefectiveRDS(int n):KDefectiveBase(n) {
	LB = new int[n];
}

KDefectiveRDS::~KDefectiveRDS() {
	delete[] LB;
}

void KDefectiveRDS::getOrder(vector<int> &vec) {
	// sort
	vector <pair<int, int> > order_pair; order_pair.clear();
	for (int i = 0; i < size; i++) 
		order_pair.push_back(make_pair(from[i].size(), i));
	sort(order_pair.begin(), order_pair.end());

	vec.clear(); // initial
	for (int i = 0; i < size; i++) vec.push_back(order_pair[i].second);
}

int KDefectiveRDS::minInSet(void *C) {
	for (int i = 0; i < size; i++) 
		if (this -> existsInSet(C, order[i])) 
			return i;
    return -1;
}

void KDefectiveRDS::reductionByConnective(void *P, void *C) {
	int *q = new int[size], l = -1, r = -1;
	bool *vis = new bool[size];
	memset(q, 0, sizeof(int) * size);
	memset(vis, 0, sizeof(bool) * size);
	// P集合中的点进入队列
	for (int i = 0; i < size; i++) if (this -> existsInSet(P, i)) {
		vis[i] = true; q[++r] = i;
	}
	// bfs维护可达性
	while (l != r) {
		int tt = q[++l];
		for (auto to: from[tt]) {
			if (!vis[to]) {
				vis[to] = true;
				q[++r] = to;
			}
		}
	}
	// 删除C中不可达的点
	for (int i = 0; i < size; i++) if (!vis[i] && this -> existsInSet(C, i)) 
		this -> removeVertexFromSetSync(C, i, 'C');
	delete[] q;
	delete[] vis;
}

void KDefectiveRDS::solve(void *_P, void *_C, int k) {
    //if ((clock() - st)/CLOCKS_PER_SEC > 600) exit(2); // 卡时间
	if (this -> timeIsUp()) return;
    
	void *P = this -> newSet(), *C = this -> newSet();
	this -> setCopyTo(_P, P); this -> setCopyTo(_C, C);
	this -> pushState();
    
    count++;
    
	// reduction
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		int need = calcNeedEdge(P, C, i);
		if (need > k) this -> removeVertexFromSetSync(C, i, 'C');
	}
	this -> reductionByConnective(P, C); // 删除不可达的边, 保证最后的答案一定是联通的

	// update ans
	if (this -> sizeOfSet(C) == 0) {
		int sz = this -> sizeOfSet(P);
        if (sz > ans) {
            ans = sz;
            fprintf(stderr, "new ans: %d\n", ans);
        }
	}

	// branch
	while (this -> sizeOfSet(C) != 0) {
		if (this -> sizeOfSet(C) + this -> sizeOfSet(P) <= ans) break;
		int idx = minInSet(C);
		if (LB[idx] + this -> sizeOfSet(P) <= ans) break;
		this -> removeVertexFromSetSync(C, order[idx], 'C');
		int need = calcNeedEdge(P, C, order[idx]);
		this -> addVertexToSetSync(P, order[idx], 'P');
		solve(P, C, k - need);
		this -> removeVertexFromSetSync(P, order[idx], 'P');
	}

	this -> popState();
	this -> deleteSet(P); this -> deleteSet(C);
}

int KDefectiveRDS::Solve(int k) {
    count = 0;
    ans = 0;
	notFinish = false;
    st = clock();
	memset(LB, 0, sizeof(int) * size);
	getOrder(order);
	for (int i = size-1; i >= 0; i--) {
		void *P = this -> newSet(), *C = this -> newSet();
		state.push(State(size));
		memset(state.top().neiC, 0, sizeof(int) * size);
		memset(state.top().neiP, 0, sizeof(int) * size);
		state.top().sizeC = state.top().sizeP = 0;

		this -> addVertexToSetSync(P, order[i], 'P'); // i 一定在当前的答案中
		for (int j = i + 1; j < size; j++) this -> addVertexToSetSync(C, order[j], 'C');
		solve(P, C, k);
		LB[i] = ans;

		state.pop();
		this -> deleteSet(P); this -> deleteSet(C);
	}
    ed = clock();
    fprintf(stderr, "time: %fms\n", float(ed-st)/CLOCKS_PER_SEC * 1000);
    fprintf(stderr, "%lu\n", count);
    return ans;
}
