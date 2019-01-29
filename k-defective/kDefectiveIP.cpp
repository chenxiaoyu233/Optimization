#include "kDefectiveIP.h"

KDefectiveIP::KDefectiveIP(int n): KDefectiveBase(n) {
	edges.clear();
	/*
	ampl = new ampl::AMPL(ampl::Environment("../ampl.macosx64"));
	ampl -> setOption("solver", "cbc");
	ampl -> read("../IPmodel/kdefective.mod");
	*/
	lp = glp_create_prob();
	glp_init_iocp(&parm);
	parm.presolve = GLP_ON;
	cnt = 0;
}

KDefectiveIP::~KDefectiveIP() {
	glp_delete_prob(lp);
	delete[] ia;
	delete[] ja;
	delete[] mat;
}

void KDefectiveIP::SetTimeLimit(int ti) {
    KDefectiveBase::SetTimeLimit(ti); // 设置父类的时间限制
	if (ti != -1) parm.tm_lim = ti * 1000;
}

void KDefectiveIP::AddEdgeByVector(const vector<pair<int, int> > &edges) {
	// 保证基类加边正常
	KDefectiveBase::AddEdgeByVector(edges);
	this -> edges = edges;
	edgePreprocessingForGLPK();
	total = (this -> edges.size() + 1) * (size + this -> edges.size()) + 1;
    ia = new int[total];
    ja = new int[total];
    mat = new double[total];
	//cerr << total << endl;
}

void KDefectiveIP::edgePreprocessingForGLPK() {
    bool *graph = new bool[size * size];
	for (auto &e: edges) {
        graph[e.first * size + e.second] = true;
        graph[e.second * size + e.first] = true;
	}
    edges.clear();
    for (int i = 0; i < size; i++)
        for (int j = i+1; j < size; j++) if(!graph[i * size + j]) {
            edges.push_back(make_pair(i + 1, j + 1));
        }
}

/*
void KDefectiveIP::assignData(int _k) {
	// 设置V
	ampl::Set V = ampl -> getSet("V");
	ampl::Tuple *tupleV = new ampl::Tuple[sizeof(ampl::Tuple) * size];
	for (int i = 0; i < size; i++) tupleV[i] = ampl::Tuple(i);
	V.setValues(tupleV, size);
	//delete[] tupleV;

	// 设置k
	ampl::Parameter k = ampl -> getParameter("k");
	k.set(_k);

	// 设置e
	ampl::Parameter e = ampl -> getParameter("e");
	double *mat = new double[size * size];
	ampl::Tuple *index = new ampl::Tuple[sizeof(ampl::Tuple) * size * size];
	for (int i = 0; i < size; i++) 
		for (int j = 0; j < size; j++) {
			index[i * size + j] = ampl::Tuple(i, j);
			mat[i * size + j] = 1;
		}
	for (auto &edge: edges) {
		mat[edge.first * size + edge.second] = 0;
	}
	e.setValues(index, mat, size * size);
	//delete[] mat;
}
*/

void KDefectiveIP::addCoefficient(int row, int col, double val) {
	++cnt;
    if (cnt <= total) {
        ia[cnt] = row;
        ja[cnt] = col;
        mat[cnt] = val;
    } else {
        cerr << total << " " << cnt << " " << row << " " << col << " " << val << endl;
        exit(0);
    }
}

void KDefectiveIP::buildModel(int k) {
	glp_set_obj_dir(lp, GLP_MAX); // 设置问题为最大化问题
	// 添加列
	for (int i = 0; i < size; i++) {  // x {V} binary
        if (timeIsUp()) return;
        //cerr << timeLimit << " " << clock() - st << endl;
		int idx = glp_add_cols(lp, 1);
		glp_set_col_kind(lp, idx, GLP_BV);
		glp_set_obj_coef(lp, idx, 1);
	}
	for (size_t i = 0; i < edges.size(); i++) {
        if (timeIsUp()) return;
        //cerr << timeLimit << " " << clock() - st << endl;
		int idx = glp_add_cols(lp, 1);
		glp_set_col_kind(lp, idx, GLP_IV);
		glp_set_col_bnds(lp, idx, GLP_LO, 0, 0); // 0 <= Z[i, j]
		glp_set_obj_coef(lp, idx, 0);
	}
	// 添加行
	for (size_t i = 0; i < edges.size(); i++) {
		int idx = glp_add_rows(lp, 1);
		glp_set_row_bnds(lp, idx, GLP_LO, -1, 0);
		for (int j = 0; j < size + (int)edges.size(); j++) {
            if (timeIsUp()) return;
            //cerr << timeLimit << " " << clock() - st << endl;
			bool isZ = j >= size; 
			int whe = isZ ? j - size + 1 : j + 1;
			double val = 0;
			if (isZ && whe == (int)i + 1)  val = 1;
			if (!isZ && (whe == edges[i].first || whe == edges[i].second)) val = -1;
			addCoefficient(idx, j+1, val);
		}
	}
	int idx = glp_add_rows(lp, 1);
	glp_set_row_bnds(lp, idx, GLP_UP, 0, k);
	for (int i = 0; i < size + (int)edges.size(); i++) {
        if (timeIsUp()) return;
        //cerr << timeLimit << " " << clock() - st << endl;
		addCoefficient(idx, i+1, double(i >= size));
	}
	glp_load_matrix(lp, cnt, ia, ja, mat);
}

int KDefectiveIP::Solve(int k) {
    count = 0;
    st = clock();
	notFinish = false;

	/*
	assignData(k);
	ampl -> solve();
	ans = ampl -> getObjective("Maximum_KDefective").value();
	*/
	buildModel(k);
    if (!timeIsUp()) {
        int flag = glp_intopt(lp, &parm);
        //notFinish = (flag == GLP_ETMLIM);
        notFinish = flag != 0;
        ans = glp_mip_obj_val(lp);
    }

	/* Debug
	for (int i = 1; i <= 10; i++) {
		cout << glp_mip_col_val(lp, i) << endl;
	}*/

    ed = clock();
    fprintf(stderr, "time: %fms\n", float(ed-st)/CLOCKS_PER_SEC * 1000);
    fprintf(stderr, "%lu\n", count);
	return ans;
}
