#ifndef __K_DEFECTIVE_IP_H__
#define __K_DEFECTIVE_IP_H__

#include "Common.h"
#include "kDefectiveBase.h"
#include "ampl/ampl.h" // AMPL建模语言API

class KDefectiveIP: public KDefectiveBase {
	private: 
	ampl::AMPL *ampl;
	void assignData();
	public:
	KDefectiveIP(int n);
	virtual int Solve(int k);
};

#endif
