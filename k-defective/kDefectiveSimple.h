#ifndef __K_DEFECTOVE_SIMPLE_H__
#define __K_DEFECTOVE_SIMPLE_H__

#include "Common.h"
#include "kDefectiveBase.h"

class KDefectiveSimple: public KDefectiveBase {
	protected:
	//virtual void branch(void *P, void *C, int k, int m);
	virtual void reductionByDiam(void *P, void *C, int k);
	
	public:
	KDefectiveSimple(int n);
};

#endif
