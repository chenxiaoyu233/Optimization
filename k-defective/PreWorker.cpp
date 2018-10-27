#include "PreWorker.h"

PreWorker::PreWorker(int n):KDefectiveBase(n) { 
	edges.clear();
	neiSet = new set<int>*[n];
}

PreWorker::~PreWorker() {
	delete[] neiSet;
}

void PreWorker::deleteIllegalVertex(int k) {
	queue<int> q;
	bool *inq = new bool[size];
	memset(inq, 0, sizeof(bool) * size); // 一开始都没有在队列中

	/* Debug */
	fprintf(stderr, "prework ans: %d\n", ans);
	//for (int i = 0; i < size; i++) if (this -> existsInSet(selected, i)) {
		//fprintf(stderr, "size of neighbor set: %d\n", this -> sizeOfSet(neiSet[i]));
	//}
	fprintf(stderr, "prework ans - k - 1: %d\n", ans - k - 1);
	/* Debug */

	// 队列初始化
	for (int i = 0; i < size; i++) 
		if (this -> existsInSet(selected, i) && this -> sizeOfSet(neiSet[i]) < ans - k - 1) {
			q.push(i);
			inq[i] = true;
		}

	// 利用队列在图中进行删点
	while (!q.empty()) {
		int tt = q.front(); q.pop();
		inq[tt] = false;
		this -> removeVertexFromSet(selected, tt);

		for (auto to: *neiSet[tt]) {
			neiSet[to] -> erase(tt);
			if (this -> existsInSet(selected, to) &&
			    this -> sizeOfSet(neiSet[to]) < ans - k - 1 && !inq[to]) {
				q.push(to);
			}
		}
	}

	delete[] inq;
}

void PreWorker::mallocMem() {
    // 申请每个点的邻居集合需要的空间
    for (int i = 0; i < size; i++) {
        neiSet[i] = (set<int>*) this -> neighborSetOf(i);
    }
    
    // 申请selected集合的空间
    selected = (set<int>*) this -> newSet();
    for (int i = 0; i < size; i++) this -> addVertexToSet(selected, i);
}

void PreWorker::freeMem() {
    // 清理为每个点申请的邻居集合
    for (int i = 0; i < size; i++) {
        this -> deleteSet(neiSet[i]);
    }
    
    // 清理selected集合的空间
    this -> deleteSet(selected);
}

void PreWorker::AddEdgeByVector(const vector<pair<int, int> > &edges) {
	// 保证基类加边正常
	KDefectiveBase::AddEdgeByVector(edges);

	this -> edges = edges;
}

pair<vector<pair<int, int> >, int> PreWorker::Process(int k) {
    // 初始化
    mallocMem();
	void *P = this -> newSet(), *C = this -> newSet();
    ans = 0;
	init(P, C);

	// 计算下界 (目前这个计算还是有问题的, 需要注意)
	for (int i = 0; i < 100; i++) prework(P, C, k); 

	// 删除一定不包含在答案中的点
	deleteIllegalVertex(k);

	// 输出预处理之后, 图的规模
	fprintf(stderr, "After prework, we have %d vertces in the new graph.\n", sizeOfSet(selected));
    
	auto ret = make_pair(getEdges(), getVertexNum());
    
    // 释放内存空间
    freeMem();

	return ret;
}

vector<pair<int, int> > PreWorker::getEdges() {
	// 将现在还没有被删掉的点统计出来
	vector<int> vec; vec.clear();
	for (int i = 0; i < size; i++) 
		if (this -> existsInSet(selected, i))
			vec.push_back(i);

	// 对原来图中的点进行重新编号
	int *newID = new int[size];
	memset(newID, -1, sizeof(int) * size);
	for (size_t i = 0; i < vec.size(); i++)
		newID[vec[i]] = i;

	// 构造新的边集合
	vector<pair<int, int> >newEdges; newEdges.clear();
	for (auto e: edges) if (newID[e.first] != -1 && newID[e.second] != -1) {
		newEdges.push_back(make_pair(newID[e.first], newID[e.second]));
	}

	return newEdges;
}

int PreWorker::getVertexNum() {
	return this -> sizeOfSet(selected);
}
