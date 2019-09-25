#include "Common.h"

const int SIZE_V = 1000;
const int SIZE_E = 10000;

typedef bitset<SIZE_V> Vset;

static int size;
static Vset N[SIZE_V]; // Neighbor of each vertex in G
static int CE_P_[SIZE_V];  // number of cost edges between P and v
static int CE_Cp_[SIZE_V]; // number of cost edges between Cp and v
static int CE_P_P, CE_Cp_P, CE_Cp_Cp; // number of cost edges between X and Y

static int sizeP, sizeC, sizeN[SIZE_V];
static Vset P;
static Vset Cm, Cp; // C+, C-
static int lP[SIZE_V], cnt_lP;

int LB; // the lower bound of the maximum k-defective
int TREE_SIZE; // the size of the search tree;
static int k; // the k in k-defective

static int AdjB[SIZE_E*2]; // total memory buffer to store adjacent list
static int AdjP[SIZE_V+1]; // pointer of each adjacent list

static int NAdjB[SIZE_E*2]; // total memory buffer to store non-adjacent list
static int NAdjP[SIZE_V+1]; // pointer of each non-adjacent list

/* macro to iterate the adjacent table */
#define FOR_ADJ_TABLE(v, u, AdjP, AdjB, ...)\
    for (int _p_ = AdjP[(v)], u = AdjB[_p_]; _p_ < AdjP[(v)+1]; ++_p_, u = AdjB[_p_])\
        __VA_ARGS__

/* macro for loop on the neighbor in original graph */
#define FOR_N(v, u)  FOR_ADJ_TABLE(v, u, AdjP, AdjB, if(Cp[u] || Cm[u] || P[u]))
#define FOR_NC(v, u) FOR_ADJ_TABLE(v, u, AdjP, AdjB, if(Cp[u] || Cm[u]))
#define FOR_NP(v, u) FOR_ADJ_TABLE(v, u, AdjP, AdjB, if(P[u]))
#define FOR_NA(v, u) FOR_ADJ_TABLE(v, u, AdjP, AdjB)

/* macro for loop on the neighbor in complement graph */
#define FOR_C_N(v, u)  FOR_ADJ_TABLE(v, u, NAdjP, NAdjB, if(Cp[u] || Cm[u] || P[u]))
#define FOR_C_NC(v, u) FOR_ADJ_TABLE(v, u, NAdjP, NAdjB, if(Cp[u] || Cm[u]))
#define FOR_C_NP(v, u) FOR_ADJ_TABLE(v, u, NAdjP, NAdjB, if(P[u]))
#define FOR_C_NA(v, u) FOR_ADJ_TABLE(v, u, NAdjP, NAdjB)

/* aux function to sort the edges */
int cmp_edges(const pair<int, int> &a, const pair<int, int> &b) {
    if (a.first == b.first) return a.second < b.second;
    return a.first < b.first;
}

/* build the adjacant table for a given edge set
 * @param edges: edge set
 * @param AdjB : buffer to store all the edges
 * @param AdjP : pointer to access neighbors of a given vertex
 */
void build_adjacent_table(const vector<pair<int, int> > &edges, int *AdjB, int *AdjP) {
    vector<pair<int, int> > edges_alter;
    for (auto &e : edges) {
        edges_alter.push_back(e);
        edges_alter.push_back(make_pair(e.second, e.first));
    }
    sort(edges_alter.begin(), edges_alter.end(), cmp_edges);
    int curP = 0, curV = -1;
    for (auto &e: edges_alter) {
        if (e.first != curV) {
            //curV = e.first;
            for (curV += 1; curV <= e.first; ++curV)
                AdjP[curV] = curP;
            --curV;
        }
        AdjB[curP++] = e.second;
    }
    for (curV += 1; curV <= size; ++curV)
        AdjP[curV] = curP;
}

void build_non_adjacent_table(vector<pair<int, int> > edges) {
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
    build_adjacent_table(edges, NAdjB, NAdjP);
}

/* use a list of edges to build the problem in our manner
 * @param size : the number of vertices in the graph
 * @param edges: vector of all edges in the graph
 * @param _LB: the lower bound of answer
 * @param _k: the k in k-defective
 * @note: the graph should not contain isolated point
 * @note: the graph should be simple
 */
void setup_instance(int _size, const vector<pair<int, int> > &edges, int _LB, int _k) {
    /* build the bitset of the whole graph */
    size = _size;
    TREE_SIZE = 0;
    P.reset();
    Cm.set(); Cp.reset();
    CE_Cp_Cp = CE_Cp_P = CE_P_P = 0;
    for (int i = 0; i < size; ++i) {
        N[i].reset();
    }
    for (auto &e : edges) {
        N[e.first].set(e.second);
        N[e.second].set(e.first);
    }

    /* build the adjacant list */
    build_adjacent_table(edges, AdjB, AdjP);
    build_non_adjacent_table(edges);

    /* init some aux parameters */
    sizeP = 0; sizeC = size;
    for (int i = 0; i < size; ++i)
        sizeN[i] = AdjP[i+1] - AdjP[i];
    LB = _LB;
    k = _k;
}

inline void in_Cp(int v) {
    CE_Cp_Cp += CE_Cp_[v];
    CE_Cp_P += CE_P_[v];
    FOR_C_NA(v, u) ++CE_Cp_[u];
}

inline void out_Cp(int v) {
    CE_Cp_Cp -= CE_Cp_[v];
    CE_Cp_P -= CE_P_[v];
    FOR_C_NA(v, u) --CE_Cp_[u];
}

inline void add_from_Cm_to_Cp(int v) {
    Cm.reset(v); Cp.set(v);
    in_Cp(v);
}

inline void add_from_Cp_to_Cm(int v) {
    Cp.reset(v); Cm.set(v);
    out_Cp(v);
}

inline void add2P(int v) {
    P.set(v); ++sizeP;
    if(Cp[v]) {
        Cp.reset(v);
        out_Cp(v);
    } else Cm.reset(v);
    --sizeC;

    CE_P_P += CE_P_[v];
    lP[cnt_lP++] = v;

    FOR_C_NA(v, u) {
        if (Cm[u] || Cp[u]) ++CE_Cp_P;
        if (CE_P_[u] == 0 && Cm[u])
            add_from_Cm_to_Cp(u);
        ++CE_P_[u];
    }
}

inline void add2C(int v) {
    P.reset(v); --sizeP;
    if (CE_P_[v] > 0) {
        Cp.set(v);
        in_Cp(v);
    } else Cm.set(v);
    ++sizeC;

    CE_P_P -= CE_P_[v];
    --cnt_lP;

    FOR_C_NA(v, u) {
        --CE_P_[u];
        if (Cp[u] || Cm[u]) --CE_Cp_P;
        if (CE_P_[u] == 0 && Cp[u])
            add_from_Cp_to_Cm(u);
    }
}

/* delete v from G */
static Vset dCm, dCp; // for recover
inline void delv (int v) {
    --sizeC;
    FOR_NA(v, u) --sizeN[u];
    if (Cm[v]) {Cm.reset(v); dCm.set(v);}
    if (Cp[v]) {
        Cp.reset(v); dCp.set(v);
        out_Cp(v);
    }
}

/* add v to G */
inline void addv (int v) {
    ++sizeC;
    FOR_NA(v, u) ++sizeN[u];
    if (dCm[v]) {dCm.reset(v); Cm.set(v);}
    if (dCp[v]) {
        dCp.reset(v); Cp.set(v);
        in_Cp(v);
    }
}

void MADEC() {
    ++TREE_SIZE;
    
    /* reduction 1 */
    if (CE_P_P > k) return;

    /* reduction 2 */
    for (int v = 0; v < size; ++v)
        if ((Cp[v] || Cm[v]) && sizeN[v] == sizeP + sizeC - 1) {
            add2P(v); MADEC(); add2C(v);
            return;
        }

    /* reduction 3 */
    for (int v = 0; v < size; ++v)
        if ((Cp[v] || Cm[v]) && CE_P_[v] > k - CE_P_P) {
            delv(v); MADEC(); addv(v);
            return;
        }

    /* update ans */
    if (LB < sizeP) {
        LB = sizeP;
        fprintf(stderr, "new LB: %d\n", LB);
    }

    if (CE_P_P + CE_Cp_P + CE_Cp_Cp > k) {
        /* branching rule 1 */
        vector<int> vec;
        for (int v = 0; v < size; ++v)
            if (Cp[v]) vec.push_back(v);
        /* currently, we use the random order */
        random_shuffle(vec.begin(), vec.end());
        int pt = 0;
        for (int vec_size = vec.size(); pt < vec_size; ++pt) {
            if (CE_P_P + CE_P_[vec[pt]] > k) break;
            add2P(vec[pt]);
        }
        for (; pt >= 0; --pt) {
            delv(vec[pt]); MADEC(); addv(vec[pt]);
            if (pt > 0) add2C(vec[pt-1]);
        }
    } else {
        /* branching rule 2 */
        for (int v = 0; v < size; ++v)
            if (Cm[v]) {
                add2P(v); MADEC(); add2C(v);
                delv(v); MADEC(); addv(v);
                return;
            }
        for (int v = 0; v < size; ++v)
            if (Cp[v]) {
                add2P(v); MADEC(); add2C(v);
                delv(v); MADEC(); addv(v);
                return;
            }
    }
}
