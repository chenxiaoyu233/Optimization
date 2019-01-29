#ifndef __K_DEFECTIVE_IP_H__
#define __K_DEFECTIVE_IP_H__

#include "Common.h"
#include "kDefectiveBase.h"
//#include "ampl/ampl.h" // AMPL建模语言API
#include <glpk.h>

class KDefectiveIP: public KDefectiveBase {
	private: 
	vector<pair<int, int> > edges;
	//ampl::AMPL *ampl;
	//void assignData(int k);

	glp_prob *lp;
	int *ia, *ja;
	double *mat;
	int cnt;
	// GLPK 库所有的元素从1开始编号
	// 为了迎合这个特点, 这里将所有的点进行了重新编号
	void edgePreprocessingForGLPK();

	// 使用GLPK的接口建立模型
	void buildModel(int k);
	void addCoefficient(int row, int col, double val);

	public:
	KDefectiveIP(int n);
	~KDefectiveIP();
	virtual void AddEdgeByVector(const vector<pair<int, int> > &edges);
	virtual int Solve(int k);
};

#endif
