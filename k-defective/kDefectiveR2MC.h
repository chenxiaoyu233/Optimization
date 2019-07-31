#ifndef __K_DEFECTIVE_R2MC_H__
#define __K_DEFECTIVE_R2MC_H__

#include "Common.h"
#include "kDefectiveBase.h"

class KDefectiveR2MC: public KDefectiveBase {
    private:
        string mf /*middle file*/, gf /*graph file*/;
        vector<pair<int, int> > edges;
        list<pair<int, int> > edge_stack;
        bool *graph;
        void solve(int k); 
        void encodeGraph();
        void search(int idx, int k);
        void callMaximumCliqueSolver();
    public:
        //stringstream ss;
        KDefectiveR2MC(int n);
        virtual void AddEdgeByVector(const vector<pair<int, int> > &edges);
        virtual int Solve(int k);
        bool TimeIsUp();
        void SetNotFinish(bool val);
        void UpdateAns(int val);
};

#endif
