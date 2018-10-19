#include "kDefectiveBase.h"

KDefectiveBase::KDefectiveBase(int n): size(n) {
	from = new vector<int>[n];
	for (int i = 0; i < size; i++) {
		from[i].clear();
	}
	dis = new int[n];
    maxDis = new int[n];
    isInPC = new bool[n];
	neiSet = new void*[n];
	timeLimit = -1;
}

KDefectiveBase::~KDefectiveBase() {
	delete[] from;
	delete[] dis;
    delete[] maxDis;
    delete[] isInPC;
	delete[] neiSet;
}

void KDefectiveBase::exitWhenTimeIsUp() {
	if (timeLimit < 0) return; // 没有设置时间上限
	if (double(clock() - st)/CLOCKS_PER_SEC * 1000 > timeLimit) exit(0);
}

void KDefectiveBase::setTimeLimit(int ti) { timeLimit = ti; }

KDefectiveBase::State::State(int n):size(n) {
	neiP = new int[n];
	neiC = new int[n];
}

KDefectiveBase::State::~State() {
	delete[] neiP;
	delete[] neiC;
}

KDefectiveBase::State::State(const State &other) {
	size = other.size;
	sizeP = other.sizeP;
	sizeC = other.sizeC;
	neiP = new int[size];
	neiC = new int[size];
	memcpy(neiP, other.neiP, sizeof(int) * size);
	memcpy(neiC, other.neiC, sizeof(int) * size);
}

void KDefectiveBase::modifyState(char kind, int idx, int delta) {
	State &now = state.top();
	if (kind == 'C') {
		now.sizeC += delta;
		for (auto to: from[idx]) now.neiC[to] += delta;
	}
	if (kind == 'P') {
		now.sizeP += delta;
		for (auto to: from[idx]) now.neiP[to] += delta;
	}
}

void KDefectiveBase::pushState() {
	state.push(state.top());
}

void KDefectiveBase::popState() {
	state.pop();
}

void KDefectiveBase::addVertexToSetSync(void *ptr, int idx, char kind) {
	if (this -> existsInSet(ptr, idx)) return; // 已经在集合中了, 不需要进行任何操作
	this -> addVertexToSet(ptr, idx);
	modifyState(kind, idx, +1);
}

void KDefectiveBase::removeVertexFromSetSync(void *ptr, int idx, char kind) {
	if (!this -> existsInSet(ptr, idx)) return; // 已经不在集合中了, 不需要进行任何操作 
	this -> removeVertexFromSet(ptr, idx);
	modifyState(kind ,idx, -1);
}

size_t KDefectiveBase::GetCount() {
    return count;
}

void KDefectiveBase::AddEdge(int a, int b) {
	from[a].push_back(b);
	from[b].push_back(a);
}

// 目前使用这个函数可能会导致得到的答案是不联通的
void KDefectiveBase::prework(void *_P, void *_C, int k) {
	void *P = this -> newSet(), *C = this -> newSet();
	this -> setCopyTo(_P, P); this -> setCopyTo(_C, C);
	this -> pushState();

	vector <int> vec; vec.clear();
	for (int i = 0; i < size; i++) vec.push_back(i);
	random_shuffle(vec.begin(), vec.end());
	for (int i = 0; i < size; i++) {
		int need = this -> calcNeedEdge(P, C, i);
		if (need > k) continue;
		k -= need;
		this -> addVertexToSetSync(P, i, 'P');
	}
	ans = max(ans, this -> sizeOfSet(P));

	this -> popState();
	this -> deleteSet(P);
	this -> deleteSet(C);
}

void KDefectiveBase::init(void *P, void *C) {
	this -> __init__(P, C); 

	// 初始化邻居集合
	for (int i = 0; i < size; i++) 
		neiSet[i] = this -> neighborSetOf(i);

	// 初始化栈
	while(!state.empty()) state.pop();
	State s(size);
	s.sizeP = 0; s.sizeC = size;
	for (int i = 0; i < size; i++) {
		s.neiC[i] = from[i].size();
		s.neiP[i] = 0;
	}
	state.push(s);
}

int KDefectiveBase::calcNeedEdge(void *P, void *C, int idx) {
	/*void *nei = this -> neighborSetOf(idx);
	void *neiInP = this -> setIntersection(P, nei);
	int need = this -> sizeOfSet(P) - this -> sizeOfSet(neiInP);
	this -> deleteSet(nei); this -> deleteSet(neiInP);
	return need;*/
	return state.top().sizeP - state.top().neiP[idx];
}

void KDefectiveBase::reductionByEdge(void *P, void *C, int m) {
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		int need = this -> calcNeedEdge(P, C, i);
		if (need > m) this -> removeVertexFromSetSync(C, i, 'C');
	}
}

int KDefectiveBase::calcLimOfDiam(void *P, void *C, int k) {
	int sz = ans;
	if (k < sz - 1) return 2;

	// 根据保证连通这个前提计算出的直径大小 -> 待验算
	float delta = 4 * sz * sz - 12 * sz + 17 - 8 * k;
	if (delta >= 0 && 2 * sz >= 3){
			return ((2 * (float)sz + 1) - sqrt(delta))/(float)2;
	}

	return sizeOfSet(P) + sizeOfSet(C) - 1; // 大小为sz的图中, 任何简单路径的长度最多为sz-1
}

/*void KDefectiveBase::calcDisFrom(void *P, void *C, int s) {
	memset(dis, 0x3f, sizeof(int) * size); // 初始化
	queue <int> q; 
	while (!q.empty()) q.pop();
	dis[s] = 0;
	q.push(s);
	while (!q.empty()) {
		int tt = q.front(); q.pop();
        for (auto to: from[tt]) {

			// 保证被删掉的点不会在计算距离时被用到
			if (!isInPC[to]) continue;

			if (dis[to] == 0x3f3f3f3f) {
				dis[to] = dis[tt] + 1;
				q.push(to);
			}
		}
	}
}*/

// 和原来的版本相比, 带来了3倍的速度提升
void KDefectiveBase::calcDisFrom(void *P, void *C, int s) {
	memset(dis, 0x3f, sizeof(int) * size);
	int *q = new int[size], l = -1, r = -1;
	dis[s] = 0;
	q[++r] = s;
	while(l != r) {
		int tt = q[++l];
		for (auto to: from[tt]){
			if (!isInPC[to]) continue;
			if (dis[to] == 0x3f3f3f3f) {
				dis[to] = dis[tt] + 1;
				q[++r] = to;
			}
		}
	}
	delete[] q;
}

// 使用位运算优化了最短路, (目前是负优化)
/*void KDefectiveBase::calcDisFrom(void *P, void *C, int s) {
	void *PC = this -> setUnion(P, C);
	memset(dis, 0x3f, sizeof(int) * size);
	int *q = new int[size], l = -1, r = -1;
	dis[s] = 0;
	q[++r] = s;
	while(l != r) {
		int tt = q[++l];
		void *Nei = this -> setIntersection(neiSet[tt], PC);
		for (int to = this -> nextBitPos(Nei); to != -1; to = this -> nextBitPos(Nei)) {
			if (dis[to] == 0x3f3f3f3f) {
				dis[to] = dis[tt] + 1;
				q[++r] = to;
				this -> removeVertexFromSet(PC, to);
			}
		}
		this -> deleteSet(Nei);
	}
	this -> deleteSet(PC);
	delete[] q;
}*/

/*void KDefectiveBase::reductionByDiam(void *P, void *C, int k) {
	int maxDiam = this -> calcLimOfDiam(P, C, k);
	for (int i = 0; i < size; i++) if(this -> existsInSet(C, i)) {
		this -> calcDisFrom(P, C, i);
		int diam = 0;
		for (int j = 0; j < size; j++) if(this -> existsInSet(P, j)) diam = max(diam, dis[j]);
		if ( diam > maxDiam ) this -> removeVertexFromSet(C, i);
	}
}*/
void KDefectiveBase::reductionByDiam(void *P, void *C, int k) {
    int maxDiam = this -> calcLimOfDiam(P, C, k) + 1;
	//fprintf(stderr, "sz: %d, maxDiam: %d, k: %d\n", ans, maxDiam, k);
    for (int i = 0; i < size; i++) isInPC[i] = this -> existsInSet(P, i) | this -> existsInSet(C, i);
    list <int> inC; inC.clear();
    for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) inC.push_back(i);
	bool flag = true;
	while(flag) {
		flag = false;
		for (int i = 0; i < size; i++) if (this -> existsInSet(P, i)) {
			this -> calcDisFrom(P, C, i);
			for (list<int>::iterator it = inC.begin(); it != inC.end(); ) {
				if (dis[*it] > maxDiam) {
					this -> removeVertexFromSetSync(C, *it, 'C');
					isInPC[*it] = false;
					it = inC.erase(it);
					flag = true;
				} else it++;
			}
		}
	}
}

void KDefectiveBase::reductionByConnectToAll(void *P, void *C) {
	int szP = this -> sizeOfSet(P), szC = this -> sizeOfSet(C);
	for (int i = 0; i < size; i++) if(this -> existsInSet(C, i)) {
		if (state.top().neiC[i] + state.top().neiP[i] == szP + szC - 1) { 
			// 减去1, 因为在计算的时候没有包含自己
			this -> removeVertexFromSetSync(C, i, 'C');
			this -> addVertexToSetSync(P, i, 'P');
		}
	}
}

void KDefectiveBase::reductionWhenIsolated(void *P, void *C) {
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		if (state.top().neiC[i] + state.top().neiP[i] == 0) {
			this -> removeVertexFromSetSync(C, i, 'C');
		}
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
		int sz = state.top().neiC[i];
		if (sz > maxv) {
			maxv = sz; whe = i;
		}
	}
	this -> removeVertexFromSetSync(C, whe, 'C');
	this -> addVertexToSetSync(P, whe, 'P');
	solve(P, C, k, m);
	this -> removeVertexFromSetSync(P, whe, 'P');
	solve(P, C, k, m);

	this -> addVertexToSetSync(C, whe, 'C');
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
	for (int i = 0, sum = 0, order_size = order.size(); i < order_size; i++) {
		int need = this -> calcNeedEdge(P, C, order[i].first);
		sum += need; 
		order[i].second = sum;
		this -> addVertexToSetSync(P, order[i].first, 'P');
	}

	// 将之前的计算还原
	for (int i = 0, order_size = order.size(); i < order_size; i++)
		this -> removeVertexFromSetSync(P, order[i].first, 'P');

	order.push_back(make_pair(-1, m + 1)); // 放一个标兵
	// 进行最后一个分支, 比较特殊, 需要专门写
	int endPos = -1;
	for (int i = 0, order_size = order.size(); i < order_size; i++) {
		if (order[i].second > m) { endPos = i; break; }
		this -> addVertexToSetSync(P, order[i].first, 'P');
		this -> removeVertexFromSetSync(C, order[i].first, 'C');
	}
	if (endPos == -1) return;
	endPos -= 1;
	solve(P, C, k, m - order[endPos].second);

	//解决其他分支
	for (int i = endPos; i >= 0; i--) {
		if (i < endPos) this -> addVertexToSetSync(C, order[i+1].first, 'C');
		this -> removeVertexFromSetSync(P, order[i].first, 'P');
		if (i > 0) solve(P, C, k, m - order[i-1].second);
		else solve(P, C, k, m);
	}
}

void KDefectiveBase::branch(void *P, void *C, int k, int m) {
	if (couldRecudeM(P, C)) {
		this -> branchWhenCouldReduceM(P, C, k, m);
	} else {
		this -> branchWhenCouldNotReduceM(P, C, k, m);
	}
}

void KDefectiveBase::solve(void *_P, void *_C, int k, int m) {
    //if ((clock() - st)/CLOCKS_PER_SEC > 600) exit(2); // 卡时间
	this -> exitWhenTimeIsUp();
    
	void *P = this -> newSet(), *C = this -> newSet();
	this -> setCopyTo(_P, P); this -> setCopyTo(_C, C);
	this -> pushState();

	// reduction
	this -> reductionByEdge(P, C, m);
	this -> reductionByConnectToAll(P, C);
	this -> reductionWhenIsolated(P, C);
	this -> reductionByDiam(P, C, k);

	// cut brunch
    //printf("sizeof(P): %d, sizeof(C): %d\n", sizeOfSet(P), sizeOfSet(C));
    if (sizeOfSet(P) + sizeOfSet(C) > ans) {

        count++; // 统计搜索树的节点大小
        
        // update ans
        if (sizeOfSet(C) == 0) {
            ans = max(ans, sizeOfSet(P));
            fprintf(stderr, "new ans: %d\n", ans);
        } else {
            // branch
            this -> branch(P, C, k, m);
        }
    }

	// free the menory
	this -> popState();
	this -> deleteSet(P); this -> deleteSet(C);
}

int KDefectiveBase::Solve(int k) {
    count = 0;
    st = clock();
    ans = 0; // init the value of the ans

	// 申请空间
	void *P = this -> newSet(), *C = this -> newSet();

	init(P, C);
	//for (int i = 0; i < 500; i++) prework(P, C, k); // 现在使用这个, 可能会导致不联通, 暂时停用
    fprintf(stderr, "new ans: %d\n", ans);
	solve(P, C, k, k);

	// 释放空间
	this -> deleteSet(P); this -> deleteSet(C);
	for (int i = 0; i < size; i++) this -> deleteSet(neiSet[i]); //init() 中初始化

    ed = clock();
    fprintf(stderr, "time: %fms\n", float(ed-st)/CLOCKS_PER_SEC * 1000);
    fprintf(stderr, "%lu\n", count);
	return ans;
}
