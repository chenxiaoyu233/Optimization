#include "kDefectiveSimple.h"

KDefectiveSimple::KDefectiveSimple(int n): KDefectiveBase(n) { }

void KDefectiveSimple::branch(void *P, void *C, int k, int m) {
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		int need = this -> calcNeedEdge(P, C, i);
		this -> removeVertexFromSet(C, i);
		this -> addVertexToSet(P, i);
		this -> solve(P, C, k, m - need);

		this -> removeVertexFromSet(P, i);
		this -> solve(P, C, k, m);
        
        break;
	}
}
