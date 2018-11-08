#include "kDefectiveSimple.h"

KDefectiveSimple::KDefectiveSimple(int n): KDefectiveBase(n) { }

/*void KDefectiveSimple::branch(void *P, void *C, int k, int m) {
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		int need = this -> calcNeedEdge(P, C, i);
		this -> removeVertexFromSetSync(C, i, 'C');
		this -> addVertexToSetSync(P, i, 'P');
		this -> solve(P, C, k, m - need);

		this -> removeVertexFromSetSync(P, i, 'P');
		this -> solve(P, C, k, m);
        
        break;
	}
}*/

void KDefectiveSimple::reductionByDiam(void *P, void *C, int k) {
	// 取消了直径规约, 不能保证解是连通的, 所以必须加上这个规约
	//this -> reductionByConnective(P, C);
}
