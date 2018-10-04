#include "BaoSolver.h"

bool BaoSolver::checkIfConnect(int v, uint32_t s) {
    memset(dis, 0, sizeof(dis));
    queue <int> q;
    while(!q.empty()) q.pop();
    q.push(v); dis[v] = 1;
    while(!q.empty()) {
        int tt = q.front(); q.pop();
        for (int i = 0; i < n; i++) if (edge[tt][i] && ((s >> i) & 1)) {
            if (dis[i] != 1) {
                dis[i] = 1;
                q.push(i);
            }
        }
    }
    bool flag = true;
    for (int i = 0; i < n; i++) if ((s >> i) & 1) {
        flag &= dis[i];
    }
    return flag;
}

int BaoSolver::check(uint32_t s, int k) {
	vector <int> vec; vec.clear();
    if (s == 0) return -1;
	for (int i = 0; i < n; i++) if ((s >> i) & 1) vec.push_back(i);
    if (!checkIfConnect(vec[0], s)) return -1;
	int ret = 0;
	for (int i = 0; i < vec.size(); i++)
		for (int j = i + 1; j < vec.size(); j++) {
			ret += 1 - edge[vec[i]][vec[j]];
		}
	if (ret > k) return -1;
	else return vec.size();
}

int BaoSolver::Solve(int k) {
	int ans = 0;
	for (uint32_t s = 0; s < (1 << n); s++)
		ans = max(ans, check(s, k));
	return ans;
}

BaoSolver::BaoSolver(int n):n(n) {
	memset(edge, 0, sizeof(edge));
}

void BaoSolver::AddEdge(int a, int b) {
	edge[a][b] = edge[b][a] = 1;
}
