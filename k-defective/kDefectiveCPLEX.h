#ifndef __K_DEFECTIVE_CPLEX_H__
#define __K_DEFECTIVE_CPLEX_H__

#include "Common.h"
#include "kDefectiveBase.h"
#include "ilcplex/ilocplex.h"

class KDefectiveCPLEX: public KDefectiveBase {
    private:
        vector<pair<int, int> > edges;
        void edgePreprocessingForCPLEX();
        void solve(int k);
    public:
        KDefectiveCPLEX(int n);
        //~KDefectiveCPLEX();
        virtual void AddEdgeByVector(const vector<pair<int, int> > &edges);
        virtual int Solve(int k);
};

#endif
