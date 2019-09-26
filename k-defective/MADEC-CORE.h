#ifndef __MADEC_CORE_H__
#define __MADEC_CORE_H__

#include "kDefectiveMADEC.h"

extern int LB;
extern int TREE_SIZE;
extern KDefectiveMADEC *controller;
void setup_instance(int size, const vector<pair<int, int> > &edges, int _LB, int _k);
void MADEC();

#endif
