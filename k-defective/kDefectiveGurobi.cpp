#include "kDefectiveGurobi.h"
#include "gurobi_c++.h"
using namespace std;

KDefectiveGurobi::KDefectiveGurobi(int n): KDefectiveBase(n) {
    edges.clear();
    x.clear(); z.clear(); 
    /* start the environment for gurobi */
}

void KDefectiveGurobi::AddEdgeByVector(const vector<pair<int, int> > &edges) {
    /* add edges for the base class */
    KDefectiveBase::AddEdgeByVector(edges);
    this -> edges = edges;
    edgePreprocessingForGurobi();
}

void KDefectiveGurobi::edgePreprocessingForGurobi() {
    bool *graph = new bool[size * size];
    memset(graph, 0, sizeof(bool) * size * size);
    for (auto &e: edges) {
        graph[e.first * size + e.second] = true;
        graph[e.second * size + e.first] = true;
    }
    edges.clear();
    for (int i = 0; i < size; ++i) 
        for (int j = i+1; j < size; ++j) if (!graph[i * size + j]) {
            edges.push_back(make_pair(i, j));
        }
    delete[] graph;
}

void KDefectiveGurobi::solve(int k) {
    try{
        GRBEnv env = GRBEnv(true);
        env.start();
        GRBModel model = GRBModel(env);
        /* set time limit */
        if (timeLimit != -1) model.set(GRB_DoubleParam_TimeLimit, timeLimit);
        /* declare varibles for verteces */
        for (int i = 0; i < size; ++i) 
            x.push_back(model.addVar(0, 1, 1, GRB_BINARY));
        /* declare varibles for edges */
        for (auto &e: edges)
            z.push_back(model.addVar(0, 1, 0, GRB_BINARY));
        /* set objective function */
        model.set(GRB_IntAttr_ModelSense, -1); // maximize
        /* set constraint for edges */
        for (int i = 0; i < edges.size(); ++i)
            model.addConstr(z[i] >= x[edges[i].first] + x[edges[i].second] - 1);
        GRBLinExpr exp(z[0]);
        for (int i = 1; i < edges.size(); ++i) exp += z[i];
        model.addConstr(exp <= k);
        /* solve the model */
        model.optimize();
        /* collect answer and status */
        ans = model.get(GRB_DoubleAttr_ObjVal);
        notFinish = model.get(GRB_IntAttr_Status) != GRB_OPTIMAL;
    } catch (GRBException e) {
        cerr << "Error code =" << e.getErrorCode() << endl ;
        cerr << e.getMessage() << endl ;
    } catch (...) {
        cerr << "Exception during optimization" << endl ;
    }
}

int KDefectiveGurobi::Solve(int k) {
    count = 0;
    st = clock();
    notFinish = false;

    solve(k);

    ed = clock();
    fprintf(stderr, "time: %fms\n", float(ed-st)/CLOCKS_PER_SEC * 1000);
    fprintf(stderr, "%lu\n", count);
	return ans;
}
