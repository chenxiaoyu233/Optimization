#include "kDefectiveIP.h"

KDefectiveIP::KDefectiveIP(int n): KDefectiveBase(n) {
	ampl = new ampl::AMPL(ampl::Environment("../ampl.macosx64"));
	ampl -> setOption("solver", "cbc");
	ampl -> read("../IPmodel/kdefective.mod");
}

void KDefectiveIP::assignData() {
}

int KDefectiveIP::Solve(int k) {
    count = 0;
    st = clock();
	notFinish = false;

	assignData();
	ampl -> solve();

    ed = clock();
    fprintf(stderr, "time: %fms\n", float(ed-st)/CLOCKS_PER_SEC * 1000);
    fprintf(stderr, "%lu\n", count);
	return ans;
}
