#include "kDefectiveR2MC.h"
#include "MoMC2016.h"

KDefectiveR2MC::KDefectiveR2MC(int n): KDefectiveBase(n) {
    edges.clear();
}

void KDefectiveR2MC::AddEdgeByVector(const vector<pair<int, int> > &edges) {
	// 保证基类加边正常
	KDefectiveBase::AddEdgeByVector(edges);
	this -> edges = edges;
    edgesSizeOri = edges.size();
}

void KDefectiveR2MC::encodeGraph() {
    ss << size << " " << edges.size() + edge_stack.size() << "\n";
    for (auto &e: edges) ss << e.first << " " << e.second << "\n";
    for (auto &e: edge_stack) ss << e.first << " " << e.second << "\n";
}

void KDefectiveR2MC::callMaximumCliqueSolver() {
    encodeGraph();
    shell = this;
    MOMCSolver();
}

void KDefectiveR2MC::search(int idx, int k) {
    if (timeIsUp()) {
        notFinish = true;
        return;
    }
    if (idx > k) {
        callMaximumCliqueSolver();
        return;
    }
    int u = 0, v = 1;
    if (!edge_stack.empty()) {
        u = edges.first;
        v = edges.second;
    }
    for (; u < size; ++u, v = u + 1) {
        for (; v < size; ++v) if (!graph[u * size + v]) {
            graph[u * size + v] = true;
            graph[v * size + u] = true;
            edge_stack.push_back(make_pair(u, v));
            search(idx + 1, k);
            edge_stack.pop_back();
            graph[u * size + v] = false;
            graph[v * size + u] = false;
        }
    }
}

void KDefectiveR2MC::solve(int k) {
    graph = new bool[size * size];
    memset(graph, 0, sizeof(bool) * size * size);
    for (auto &e: edges) {
        graph[e.first * size + e.second] = true;
        graph[e.second * size + e.first] = true;
    }
    search(1, k);
    delete[] graph;
}

int KDefectiveR2MC::Solve(int k) {
    count = 0;
    st = clock();
    notFinish = false;

    solve(k);

    ed = clock();
    fprintf(stderr, "time: %fms\n", float(ed-st)/CLOCKS_PER_SEC * 1000);
    fprintf(stderr, "%lu\n", count);
    return ans;
}

bool KDefectiveR2MC::TimeIsUp() { return this -> timeIsUp(); }
void KDefectiveR2MC::SetNotFinish(bool val) { notFinish = val; }
void KDefectiveR2MC::UpdateAns(int val) { ans = max(ans, val); }
