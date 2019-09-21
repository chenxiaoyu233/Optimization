#ifndef __K_DEFECTIVE_H__
#define __K_DEFECTIVE_H__

// Algorithm
#include "kDefectiveBase.h"     // 主算法
#include "kDefectiveSimple.h"   // 主算法简化分支策略之后的版本 
#include "kDefectiveRDS.h"      // 用于对比的 RDS 算法的实现
#include "kDefectiveR2MC.h"     // 用于对比的最大团算法的实现 (Reduce to Maximum Clique)

#ifdef INTEGER_PROGRAMMING_SOLVER
#include "kDefectiveIP.h"       // 用于对比整数规划算法的实现 (GLPK)
#include "kDefectiveCPLEX.h"    // 用于对比整数规划算法的实现 (CPLEX)
//#include "kDefectiveGurobi.h" // 用于对比整数规划算法的实现 (Gurobi)
#endif

// Preworker 
#include "PreWorker.h"

// Data Structure
#include "MyBitSet.h"
#include "BitSetImplement.h"
#include "MyBitSetImplement.h"
#include "STLSetImplement.h"

// Profile and Debug
#include "GraphGenerator.h"
#include "BaoSolver.h"
#include "Compare.h"

#endif
