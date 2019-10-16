#ifdef INTEGER_PROGRAMMING_SOLVER
#include "kDefectiveCPLEX.h"

KDefectiveCPLEX::KDefectiveCPLEX(int n): KDefectiveBase(n) {
    edges.clear();
}

void KDefectiveCPLEX::AddEdgeByVector(const vector<pair<int, int> > &edges) {
	// 保证基类加边正常
	KDefectiveBase::AddEdgeByVector(edges);
	this -> edges = edges;
    edgePreprocessingForCPLEX();
}

void KDefectiveCPLEX::edgePreprocessingForCPLEX() {
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

void KDefectiveCPLEX::solve(int k) {
    IloEnv env;
    try {
        IloModel model(env);
        IloBoolVarArray x(env, size), z(env, edges.size());
        // constraints
        for (int i = 0; i < edges.size(); ++i) {
            model.add(z[i] >= x[edges[i].first] + x[edges[i].second] - 1);
        }
        model.add(IloSum(z) <= k);

        // Objective function
        model.add(IloMaximize(env, IloSum(x)));

        // solve
        IloCplex cplex(model);
        if (timeLimit >= 0) cplex.setParam(IloCplex::TiLim, timeLimit);
        if (cplex.solve()) {
            notFinish = cplex.getStatus() != IloAlgorithm::Status::Optimal;
            IloNum ret = cplex.getObjValue();
            cerr << "CPLEX OPTIMUM: " << ret << endl;
            ans = int(ret + 1e-4);
        } else {
            notFinish = true;
            cerr << "no solution found" << endl;
        }
    } catch (IloException& ex) {
        cerr << "Error (CXY): " << ex << endl;
        notFinish = true;
    } catch (...) {
        cerr << "Error (CXY)" << endl;
        notFinish = true;
    }
    env.end();
}

int KDefectiveCPLEX::Solve(int k) {
    count = 0;
    st = clock();
    notFinish = false;

    solve(k);

    ed = clock();
    fprintf(stderr, "time: %fms\n", float(ed-st)/CLOCKS_PER_SEC * 1000);
    fprintf(stderr, "%lu\n", count);
    return ans;
}

#endif
