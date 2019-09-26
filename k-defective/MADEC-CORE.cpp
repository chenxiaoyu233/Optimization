#include "Common.h"
#include "kDefectiveMADEC.h"

KDefectiveMADEC *controller;

const int SIZE_V = 20000;
const int SIZE_E = SIZE_V * SIZE_V;

typedef bitset<SIZE_V> Vset;

static int size;
static Vset N[SIZE_V];  // Neighbor of each vertex in G
static Vset NC[SIZE_V]; // Neighbor of each vertex in G^c
static Vset N2[SIZE_V]; // $N^2[v] \cup N[v]$ of each vertex v in G
static int CE_P_[SIZE_V];  // number of cost edges between P and v
static int CE_Cp_[SIZE_V]; // number of cost edges between Cp and v
static int CE_Cm_[SIZE_V]; // number of cost edges between Cm and v
static int CE_P_P, CE_Cp_P, CE_Cp_Cp, CE_Cm_Cm, CE_Cm_Cp;// number of cost edges between X and Y

static int sizeP, sizeC, sizeN[SIZE_V], sizeNC[SIZE_V];
static int neiP[SIZE_V], neiC[SIZE_V]; // number of neighbors in P and C
static Vset P;
static Vset Cm, Cp; // C+, C-
static int lP[SIZE_V], cnt_lP;

int LB; // the lower bound of the maximum k-defective
int TREE_SIZE; // the size of the search tree;
static int k; // the k in k-defective

static bool two_hop_flag; // do we need the 2-hop reduction
const int TWO_HOP_THRESHOLD = 0;

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

pair<int, int> order[SIZE_V];
int ord_map[SIZE_V];
int cmp_order(const pair<int, int> &a, const pair<int, int> &b) {
    return a.second > b.second;
}
/* find an order to accumulate the algorithm, the remap each vertex by the order */
void get_order_then_remap(int size, const vector<pair<int, int> > &_edges, vector<pair<int, int> > &edges) {
    /* get the order by degree */
    for (auto &e: _edges) {
        ++order[e.first].second;
        ++order[e.second].second;
    }
    for (int i = 0; i < size; ++i) order[i].first = i;
    sort(order, order + size, cmp_order);

    /* remap vertex */
    for (int i = 0; i < size; ++i) ord_map[order[i].first] = i;
    for (auto &e: _edges) {
        edges.push_back(make_pair(ord_map[e.first], ord_map[e.second]));
    }
}

/* use a list of edges to build the problem in our manner
 * @param size : the number of vertices in the graph
 * @param edges: vector of all edges in the graph
 * @param _LB: the lower bound of answer
 * @param _k: the k in k-defective
 * @note: the graph should not contain isolated point
 * @note: the graph should be simple
 */
void setup_instance(int _size, const vector<pair<int, int> > &_edges, int _LB, int _k) {
    /* reorder the graph */
    vector<pair<int, int> > edges;
    get_order_then_remap(_size, _edges, edges);

    /* build the bitset of the whole graph */
    size = _size;
    TREE_SIZE = 0;
    P.reset();
    for (int v = 0; v < size; ++v) Cm.set(v);
    Cp.reset();
    for (int i = 0; i < size; ++i) {
        N[i].reset();
        for (int v = 0; v < size; ++v) NC[i].set(v);
        NC[i].reset(i);
    }
    for (auto &e : edges) {
        N[e.first].set(e.second);
        NC[e.first].reset(e.second);
        N[e.second].set(e.first);
        NC[e.second].reset(e.first);
    }

    /* build the adjacant list */
    build_adjacent_table(edges, AdjB, AdjP);
    build_non_adjacent_table(edges);

    /* build the $N^2[v] \cup N[v]$ for each vertex */
    for (int v = 0; v < size; ++v) {
        N2[v] |= N[v];
        FOR_NA(v, u) N2[v] |= N[u];
    }

    /* init some aux parameters */
    sizeP = 0; sizeC = size;
    for (int i = 0; i < size; ++i) {
        neiC[i] = sizeN[i] = AdjP[i+1] - AdjP[i];
        sizeNC[i] = NAdjP[i+1] - NAdjP[i];
    }
    LB = _LB;
    k = _k;
    two_hop_flag = true;

    /* init CE (number of cost edges */
    CE_Cp_Cp = CE_Cp_P = CE_P_P = CE_Cm_Cp = CE_Cm_Cm = 0;
    for (int v = 0; v < size; ++v) {
        CE_Cm_Cm += sizeNC[v];
        CE_Cm_[v] = sizeNC[v];
    }
    CE_Cm_Cm >>= 1;
}

inline void in_Cp(int v) {
    CE_Cp_Cp += CE_Cp_[v];
    CE_Cp_P += CE_P_[v];
    CE_Cm_Cp += CE_Cm_[v];
    FOR_C_NA(v, u) ++CE_Cp_[u];
}

inline void out_Cp(int v) {
    CE_Cp_Cp -= CE_Cp_[v];
    CE_Cp_P -= CE_P_[v];
    CE_Cm_Cp -= CE_Cm_[v];
    FOR_C_NA(v, u) --CE_Cp_[u];
}

inline void in_Cm(int v) {
    CE_Cm_Cm += CE_Cm_[v];
    CE_Cm_Cp += CE_Cp_[v];
    FOR_C_NA(v, u) ++CE_Cm_[u];
}

inline void out_Cm(int v) {
    CE_Cm_Cm -= CE_Cm_[v];
    CE_Cm_Cp -= CE_Cp_[v];
    FOR_C_NA(v, u) --CE_Cm_[u];
}

inline void add_from_Cm_to_Cp(int v) {
    Cm.reset(v); Cp.set(v);
    out_Cm(v); in_Cp(v);
}

inline void add_from_Cp_to_Cm(int v) {
    Cp.reset(v); Cm.set(v);
    out_Cp(v); in_Cm(v);
}

inline void add2P(int v) {
    P.set(v); ++sizeP;
    if(Cp[v]) {Cp.reset(v); out_Cp(v);}
    else {Cm.reset(v); out_Cm(v);}
    --sizeC;

    CE_P_P += CE_P_[v];
    lP[cnt_lP++] = v;

    FOR_NA(v, u) {--neiC[u]; ++neiP[u];}

    FOR_C_NA(v, u) {
        if (Cm[u] || Cp[u]) ++CE_Cp_P;
        if (CE_P_[u] == 0 && Cm[u])
            add_from_Cm_to_Cp(u);
        ++CE_P_[u];
    }
}

inline void add2C(int v) {
    P.reset(v); --sizeP;
    if (CE_P_[v] > 0) {Cp.set(v); in_Cp(v);}
    else {Cm.set(v); in_Cm(v);}
    ++sizeC;

    CE_P_P -= CE_P_[v];
    --cnt_lP;

    FOR_NA(v, u) {++neiC[u]; --neiP[u];}

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
    FOR_NA(v, u) {--sizeN[u]; --neiC[u];}
    if (Cm[v]) {Cm.reset(v); dCm.set(v); out_Cm(v);}
    if (Cp[v]) {Cp.reset(v); dCp.set(v); out_Cp(v);}
}

/* add v to G */
inline void addv (int v) {
    ++sizeC;
    FOR_NA(v, u) {++sizeN[u]; ++neiC[u];}
    if (dCm[v]) {dCm.reset(v); Cm.set(v); in_Cm(v);}
    if (dCp[v]) {dCp.reset(v); Cp.set(v); in_Cp(v);}
}

inline int color_bound() {
    Vset C = Cm | Cp;
    list<int> lv, part;
    for (int v = 0; v < size; ++v)
        if (C[v]) lv.push_back(v);

    while(!lv.empty()) {
        Vset cur = C;
        int cnt = 0;
        for (list<int>::iterator it = lv.begin(); it != lv.end(); ) {
            if (cur[*it]) {
                ++cnt;
                cur &= NC[*it];
                C.reset(*it);
                it = lv.erase(it);
            } else ++it;
        }
        part.push_back(cnt);
    }
    
    int UB = 0, cnt = 0, m = k - CE_P_P;
    while(m >= cnt) {
        for (auto sz: part) if (sz > 0 && m >= cnt) {
            m -= cnt;
            -- sz;
            ++ UB;
        }
        ++cnt;
    }

    return UB + sizeP;
}

void checker() {
    /* CE_P_P */
    int re_CE_P_P = 0;
    for (int v = 0; v < size; ++v) if (P[v])
        FOR_C_NP(v, u) re_CE_P_P += 1;
    re_CE_P_P >>= 1;
    assert(re_CE_P_P == CE_P_P);

    /* CE_Cp_P */
    int re_CE_Cp_P = 0;
    for (int v = 0; v < size; ++v)
        if (Cp[v]) FOR_C_NP(v, u) re_CE_Cp_P += 1;
    assert(re_CE_Cp_P == CE_Cp_P);

    /* CE_Cp_Cp */
    int re_CE_Cp_Cp = 0;
    for (int v = 0; v < size; ++v)
        if (Cp[v]) FOR_C_NA(v, u) if (Cp[u]) ++re_CE_Cp_Cp;
    re_CE_Cp_Cp >>= 1;
    assert(re_CE_Cp_Cp == CE_Cp_Cp);

    /* CE_Cm_Cp */
    int re_CE_Cm_Cp = 0;
    for (int v = 0; v < size; ++v)
        if (Cm[v]) FOR_C_NA(v, u) if (Cp[u]) ++re_CE_Cm_Cp;
    assert(re_CE_Cm_Cp == CE_Cm_Cp);

    /* CE_Cm_Cm */
    int re_CE_Cm_Cm = 0;
    for (int v = 0; v < size; ++v)
        if (Cm[v]) FOR_C_NA(v, u) if (Cm[u]) ++re_CE_Cm_Cm;
    re_CE_Cm_Cm >>= 1;
    assert(re_CE_Cm_Cm == CE_Cm_Cm);

    /* neiC */
    for (int v = 0; v < size; ++v) {
        int re_neiC = 0;
        FOR_NC(v, u) re_neiC += 1;
        assert(re_neiC == neiC[v]);
    }

    /* neiP */
    for (int v = 0; v < size; ++v) {
        int re_neiP = 0;
        FOR_NP(v, u) re_neiP += 1;
        assert(re_neiP == neiP[v]);
    }

    /* sizeC */
    int re_sizeC = 0;
    for (int v = 0; v < size; ++v) if(Cm[v] || Cp[v]) ++re_sizeC;
    assert(re_sizeC == sizeC);

    /* sizeP */
    int re_sizeP = 0;
    for (int v = 0; v < size; ++v) if (P[v]) ++re_sizeP;
    assert(re_sizeP == sizeP);
}

void bb_color(Vset C, int *U, int *color) {
    int cnt_color = 0, pt = 0;
    list<int> lv;
    for (int v = 0; v < size; ++v)
        if (C[v]) lv.push_back(v);

    while(!lv.empty()) {
        Vset cur = C;
        ++cnt_color;
        for (list<int>::iterator it = lv.begin(); it != lv.end(); ) {
            if (cur[*it]) {
                cur &= NC[*it];
                C.reset(*it);
                U[pt] = *it;
                color[pt++] = cnt_color;
                it = lv.erase(it);
            } else ++it;
        }
    }
}

void bb_max_clq(Vset P, Vset C) {
    if (controller -> TimeIsUp()) return;

    ++TREE_SIZE;
    int sizeC = C.count();
    int *U = new int[sizeC]; 
    int *color = new int[sizeC];
    bb_color(C, U, color);
    for (int i = sizeC - 1; i >= 0; --i) {
        if (color[i] + P.count() <= LB) return;
        Vset nC = C;
        int v = U[i];
        P.set(v); nC &= N[v];
        if (LB < P.count()) {
            LB = P.count();
            fprintf(stderr, "new LB: %d @ MAX CLIQUE\n", LB);
        }
        if (nC.any()) bb_max_clq(P, nC);
        P.reset(v); C.reset(v);
    }
    delete[] U;
    delete[] color;
}

void MADEC() {
    if (controller -> TimeIsUp()) return;
    //checker();
    
    ++TREE_SIZE;

    /* degenerate to max clique */
    if (k == CE_P_P) {
        Vset NP; NP.set();
        for (int v = 0; v < size; ++v) if (P[v]) NP &= N[v];
        bb_max_clq(P, (Cm | Cp) & NP);
        return;
    }

    /* reduction 1 */
    if (CE_P_P > k) return;

    list<int> del_list, add2P_list;

    /* reduction 2 */
    for (int v = 0; v < size; ++v)
        if ((Cp[v] || Cm[v]) && sizeN[v] == sizeP + sizeC - 1) {
            add2P(v); add2P_list.push_back(v);
        }

    /* reduction 3 */
    for (int v = 0; v < size; ++v)
        if ((Cp[v] || Cm[v]) && CE_P_[v] > k - CE_P_P) {
            delv(v); del_list.push_back(v);
        }

    /* 2-hop reduction */
    if (two_hop_flag && k - CE_P_P && LB > k && sizeP) {
        Vset valid;
        for (int i = 0; i < cnt_lP; ++i)
            valid |= N2[lP[i]];
        for (int v = 0; v < size; ++v)
            if ((Cm[v] || Cp[v]) && !valid[v]) {
                del_list.push_back(v);
                delv(v);
            }
    }

    /* update ans */
    if (LB < sizeP) {
        LB = sizeP;
        fprintf(stderr, "new LB: %d\n", LB);
    }

    if (!sizeC) goto finish;
    if (sizeC + sizeP <= LB) goto finish;

    /* reduction: if we could add C to P directly */
    if (CE_P_P + CE_Cp_P + CE_Cp_Cp + CE_Cm_Cp + CE_Cm_Cm <= k) {
        if (LB < sizeP + sizeC) {
            LB = sizeP + sizeC;
            fprintf(stderr, "new LB: %d @ RC2P\n", LB);
        }
        goto finish;
    }

    /* coloring bound */
    if (color_bound() <= LB) goto finish;

    if (CE_P_P + CE_Cp_P + CE_Cp_Cp > k) {
        /* branching rule 1 */
        vector<pair<int, int> > vec;
        for (int v = 0; v < size; ++v)
            if (Cp[v]) vec.push_back(make_pair(v, CE_P_[v]));
        sort(vec.begin(), vec.end(), cmp_order);
        int pt = 0;
        for (int vec_size = vec.size(); pt < vec_size; ++pt) {
            if (CE_P_P + CE_P_[vec[pt].first] > k) break;
            add2P(vec[pt].first);
        }
        for (; pt >= 0; --pt) {
            delv(vec[pt].first); MADEC(); addv(vec[pt].first);
            if (pt > 0) add2C(vec[pt-1].first);
        }
    } else {
        /* branching rule 2 */
        int whe = -1;
        for (int v = 0; v < size; ++v) if (Cm[v] || Cp[v]) {
            if (whe == -1) whe = v;
            else if (neiC[whe] > neiC[v]) whe = v;
        }
        add2P(whe); MADEC(); add2C(whe);
        delv(whe); MADEC(); addv(whe);
    }

finish:
    while (!del_list.empty()) {
        addv(del_list.back());
        del_list.pop_back();
    }
    while (!add2P_list.empty()) {
        add2C(add2P_list.back());
        add2P_list.pop_back();
    }
}
