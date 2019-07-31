#include "kDefectiveR2MC.h"

KDefectiveR2MC::KDefectiveR2MC(int n): KDefectiveBase(n) {
    edges.clear();
}

void KDefectiveR2MC::AddEdgeByVector(const vector<pair<int, int> > &edges) {
	// 保证基类加边正常
	KDefectiveBase::AddEdgeByVector(edges);
	this -> edges = edges;
}

void KDefectiveR2MC::encodeGraph() {
    FILE *out = fopen(gf.c_str(), "w");
    fprintf(out, "p word %d %lu\n", size, edges.size() + edge_stack.size());
    for (auto &e: edges) 
        fprintf(out, "e %d %d\n", e.first, e.second);
    for (auto &e: edge_stack) 
        fprintf(out, "e %d %d\n", e.first, e.second);
    fclose(out);
}

void KDefectiveR2MC::callMaximumCliqueSolver() {
    // ask OS for two temp file
    char midFile[] = "mid.XXXXXX";
    int mf_id = mkstemp(midFile);
    mf = string(midFile);
    char graphFile[] = "graph.XXXXXX";
    int gf_id = mkstemp(graphFile);
    gf = string(graphFile);

    // output the graph to a file
    encodeGraph();

    // collect the answer from a file
    system((string("./MOMC ") + gf + string(" > ") + mf).c_str());
    system((string("tail -n 1 ") + mf + string(" | tee ") + mf).c_str());
    FILE *in = fopen(mf.c_str(), "r");
    int cur = 0; fscanf(in, "%*s%*s%*s%*s%d", &cur);
    ans = max(ans, cur);
    fprintf("update ans: %d\n", ans);
    fclose(in);

    // 释放文件
    unlink(midFile);
    unlink(graphFile);
    close(mf_id);
    close(gf_id);
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
    if (edges.size() + edge_stack.size() == size * (size - 1) / 2) {
        callMaximumCliqueSolver();
        return;
    }
    int u = 0, v = 1;
    if (!edge_stack.empty()) {
        u = edge_stack.back().first;
        v = edge_stack.back().second;
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
