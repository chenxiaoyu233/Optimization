#ifndef __K_DEFECTIVE_GUROBI_H__
#define __K_DEFECTIVE_GUROBI_H__

#include "Common.h"
#include "kDefectiveBase.h"
/* Gurobi header */

#include "gurobi_c++.h"

class KDefectiveGurobi: public KDefectiveBase {
    private:
        vector<pair<int, int> > edges;
        /* varable holder */
        vector<GRBVar> x; // for V
        vector<GRBVar> z; // for E

        void edgePreprocessingForGurobi();
        void solve(int k);
    public: 
        KDefectiveGurobi(int n);
        virtual void AddEdgeByVector(const vector<pair<int ,int> > &edges);
        virtual int Solve(int k);
};

#endif
