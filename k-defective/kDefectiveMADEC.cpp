#include "kDefectiveMADEC.h"
#include "MADEC-CORE.h"

KDefectiveMADEC::KDefectiveMADEC(int n): KDefectiveBase(n) {
    edges.clear();
}

void KDefectiveMADEC::AddEdgeByVector(const vector<pair<int ,int> > &edges) {
    KDefectiveBase::AddEdgeByVector(edges);
    this -> edges = edges;
}

int KDefectiveMADEC::Solve(int k) {
    setup_instance(size, edges, ans, k);
    count = 0;
    st = clock();
    notFinish = false;

    MADEC();
    
    count = TREE_SIZE;
    ed = clock();
    fprintf(stderr, "time: %fms\n", float(ed-st)/CLOCKS_PER_SEC * 1000);
    fprintf(stderr, "%lu\n", count);
    return ans = LB;
}
