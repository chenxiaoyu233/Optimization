// 输入输出, 内存操作工具
#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;

// k-defective 相关算法实现
#include "kDefective.h"

// 命令行参数解析工具
#include <unistd.h> 

// 全局参数存储
struct globalArgs_t {
	/* 读写文件相关信息 */
	string readFileName;  // -r 读文件名
	string writeFileName; // -w 写文件名

	/* 操作类型 */
	string op;            // -O 需要运行那种类型的操作

	/* 求解类操作 */
	bool needPrework;     // -p 是否需要对图进行预处理
	int timeLimit;        // -t 算法运行的时间上限
	string algoType;      // -a 需要运行的算法类型
	string dataStructure; // -D 使用的数据结构的类型
	int k;                // -k 设置 k-defective 中的 k 值

	/* 生成数据类操作 */
	int vertexNum;        // -n 图中点的数量
	float edgeDensity;    // -d 图中边出现的密度

	/* 对比类操作 */

	/* 求助类操作 */
	bool needHelp;        // -h 表示需要帮助

	/* 输入图的文件类型 */
	string graphFileType; // -G 设置图文件的类型 (.clq, .graph)

	/* 最大团的大小 */
	int maxKDefective;    // -M 设置图的最大团的大小
} globalArgs;

static const char *optString = "f:r:w:O:pt:a:D:n:d:hk:G:M:";

void InitGlobalArgs() {
	globalArgs.readFileName = "";
	globalArgs.writeFileName = "";

	globalArgs.op = "";

	globalArgs.needPrework = false;
	globalArgs.timeLimit = -1;
	globalArgs.algoType = "";
	globalArgs.dataStructure = "";
	globalArgs.k = -1;

	globalArgs.vertexNum = 0;
	globalArgs.edgeDensity = 0.0;

	globalArgs.needHelp = false;

	globalArgs.graphFileType = "clq";

	globalArgs.maxKDefective = 0;
}

void DisplayUsage() {
}

bool HaveEnoughArgs(string needArgs) {
	for (size_t i = 0; i < needArgs.length(); i++) {
		switch (needArgs[i]) {
			case 'r':
				if (globalArgs.readFileName == "") {
					puts("need -r for readFileName");
					return false;
				}
                break;
                
			case 'w':
				if (globalArgs.writeFileName == "") {
					puts("need -w for writeFileName");
				   	return false;
				}
                break;
                
			case 'O':
				if (globalArgs.op == "") {
					puts("need -O for operation type");
				   	return false;
				}
                break;
                
			case 'a':
				if (globalArgs.algoType == "") {
					puts("need -a for algorithm type");
					return false;
				}
                break;
                
			case 'D':
				if (globalArgs.dataStructure == "") {
					puts("need -D for dataStructure type");
					return false;
				}
                break;
                
			case 'k':
				if (globalArgs.k < 0) {
					puts("need -k for k");
					return false;
				}
                break;
                
			case 'n':
				if (globalArgs.vertexNum <= 0) {
					puts("need -n for vertexNum");
					return false;
				}
                break;
                
			case 'd':
				if (globalArgs.edgeDensity < 0) {
					puts("need -d for edgeDensity");
					return false;
				}
                break;
		}
	}
	return true;
}

int N, M;
vector<pair<int, int> > edges;
// 读取.clq类型的图
void ReadGraphClq() {
	FILE *in = fopen(globalArgs.readFileName.c_str(), "r");
	char buffer[1005];
	char flag;
	while(fscanf(in, " %c", &flag) != EOF){
		if(flag == 'c') {
			fgets(buffer, 1000, in);
		}else if(flag == 'p') { //  info
			fscanf(in, " %*s%d%d", &N, &M);
		}else if(flag == 'e') { // edge
			int a, b; fscanf(in, " %d%d", &a, &b);
			--a; --b; // 从 0 开始编号
			edges.push_back(make_pair(a, b));
		}
	}
    fclose(in);
}

// 读取.graph类型的图
#include <sstream>
#include <fstream>
void ReadGraphGraph() {
	ifstream fin(globalArgs.readFileName.c_str());
	string buffer;
	getline(fin, buffer);
	istringstream ss(buffer);
	ss >> N >> M;
	for (int i = 1; i <= N; i++) {
		getline(fin, buffer);
		istringstream ss(buffer);
		int tmp;
		while (!(ss >> tmp).eof()) {
			int a = i - 1, b = tmp - 1;
			edges.push_back(make_pair(a, b));
		}
	}
}

// 读取SNAP类型的图(txt)
void ReadGraphSnap() {
	ifstream fin(globalArgs.readFileName.c_str());
	string buffer;
	while (!getline(fin, buffer).eof()) {
		// 判断是否当前行是注释
		size_t idx = 0; 
		while (idx < buffer.length() && buffer[idx] == ' ') ++idx;
		if (idx >= buffer.length() || buffer[idx] == '#') continue;
		// 读取本行数据
		istringstream ss(buffer);
		int a, b; ss >> a >> b;
		--a; --b;
		edges.push_back(make_pair(a, b));
	}
	// 设置N, M
	M = edges.size();
	for (auto e: edges) {
		N = max(N, e.first + 1);
		N = max(N, e.second + 1);
	}
}

void dealDoubleEdge() {
	fprintf(stderr, "number of edges before dealDoubleEdge: %lu\n", edges.size());
	// 排序
	for (size_t i = 0; i < edges.size(); i++) {
		if (edges[i].first > edges[i].second) swap(edges[i].first, edges[i].second);
	}
	sort(edges.begin(), edges.end());
	// 去重
	size_t cur = 1;
	for (size_t last = 0, i = 1; i < edges.size(); ++i, last = i-1) {
		if (edges[i] != edges[last]) {
			edges[cur++] = edges[i];
		}
	}
	edges.resize(cur);
	fprintf(stderr, "number of edges after dealDoubleEdge: %lu\n", edges.size());
}

void SolveWork() {
	if (!HaveEnoughArgs("rptaDk")) return;
	KDefectiveBase *solver = NULL;
   	PreWorker* preworker = NULL;
	int ans = -1;

	// 从文件输入图
	if (globalArgs.graphFileType == "clq") {
		ReadGraphClq();
	} else if (globalArgs.graphFileType == "graph") {
		ReadGraphGraph();
	} else if (globalArgs.graphFileType == "SNAP") {
		ReadGraphSnap();
	} else {
		fprintf(stderr, "error graph file type");
		exit(2333);
	}

	// 去重边
	dealDoubleEdge();

	// 暴力输出解(用于Debug其他的实现, 当然也包括预处理器)
	if (globalArgs.algoType == "Bao") { 
		BaoSolver* baoSolver;
		baoSolver = new BaoSolver(N);
		baoSolver -> AddEdgeByVector(edges);
		ans = baoSolver -> Solve(globalArgs.k);
		printf("ans: %d\n", ans);
		return;
	}

	// 预处理图
	if (globalArgs.needPrework) {
		preworker = new STLSetImplement<set<int>, PreWorker>(N);
		preworker -> SetAns(globalArgs.maxKDefective);
		preworker -> AddEdgeByVector(edges);
		auto ret = preworker -> Process(globalArgs.k);

		// 更新点集和边集
		edges = ret.first;
		N = ret.second;

		// 更新当前答案
		globalArgs.maxKDefective = preworker -> GetAns();
		
        // 回收内存
		delete preworker;
		fprintf(stderr, "finish prework");
	}

	// 创建求解器
	if (globalArgs.dataStructure == "Set") {
		if (globalArgs.algoType == "Base") solver = new STLSetImplement<set<int>, KDefectiveBase>(N);
		else if (globalArgs.algoType == "RDS") solver = new STLSetImplement<set<int>, KDefectiveRDS>(N);
		else if (globalArgs.algoType == "Simple") solver = new STLSetImplement<set<int>, KDefectiveSimple>(N);
	} else if (globalArgs.dataStructure == "Bitset" && N <= 5000) {
		if (globalArgs.algoType == "Base") solver = new BitSetImplement<bitset<5000>, KDefectiveBase>(N);
		else if (globalArgs.algoType == "RDS") solver = new BitSetImplement<bitset<5000>, KDefectiveRDS>(N);
		else if (globalArgs.algoType == "Simple") solver = new BitSetImplement<bitset<5000>, KDefectiveSimple>(N);
	}

	// 判断solver是否构造成功
	if (solver == NULL) {
		puts("can not initialize the solver");
		return;
	}

	// 设置答案下界
	solver -> SetAns(globalArgs.maxKDefective);

	// 设置时间上限
	solver -> SetTimeLimit(globalArgs.timeLimit);

	// 输入图
	solver -> AddEdgeByVector(edges);

	// 求解
	solver -> Solve(globalArgs.k);

	// 输出答案
	printf("ans: %d\n", solver -> GetAns());
	printf("size of search tree: %lu\n", solver -> GetCount());
	printf("cost of time: %lfs\n", solver -> GetCostTime());
	printf("time out flag: %d\n", solver -> GetNotFinishFlag());

	// 回收内存
	delete solver;
}

void GenerateWork() {
	if (!HaveEnoughArgs("wnd")) return;
	GraphGenerator *generator = new GraphGenerator(globalArgs.writeFileName);
	generator -> Generate(globalArgs.vertexNum, globalArgs.edgeDensity, 0.00001);
}

void CompareWork() {
	if (!HaveEnoughArgs("ptDnd")) return;
	Compare *compare = new Compare("main.in");
	compare -> Run(globalArgs.vertexNum, globalArgs.edgeDensity);
}

void Work() {
	// 显示帮助信息
	if (globalArgs.needHelp) {
		DisplayUsage();
		return;
	}

	// 根据需要做的工作进行分流
	if (globalArgs.op == "solve") {
		SolveWork();
	} else if (globalArgs.op == "generate") {
		GenerateWork();
	} else if (globalArgs.op == "compare") {
		CompareWork();
	} else {
		puts ("需要指定操作类型");
	}
}

int main(int argc, char** argv) {
	// 初始化各项参数
	InitGlobalArgs();

	// 从命令行获取参数
	char opt;
	while ( (opt = getopt( argc, argv, optString )) != -1 ) {
		switch (opt) {
			case 'f':
				globalArgs.readFileName = globalArgs.writeFileName = string(optarg);
				break;

			case 'r':
				globalArgs.readFileName = string(optarg);
				break;

			case 'w':
				globalArgs.writeFileName = string(optarg);
				break;

			case 'O':
				globalArgs.op = string(optarg);
				break;

			case 'p':
				globalArgs.needPrework = true;
				break;

			case 't':
				globalArgs.timeLimit = atoi(optarg);
				break;

			case 'a':
				globalArgs.algoType = string(optarg);
				break;

			case 'D':
				globalArgs.dataStructure = string(optarg);
				break;

			case 'k':
				globalArgs.k = atoi(optarg);
				break;

			case 'n':
				globalArgs.vertexNum = atoi(optarg);
				break;

			case 'd':
				globalArgs.edgeDensity = atof(optarg);
				break;

			case 'h':
				globalArgs.needHelp = true;
				break;

			case 'G':
				globalArgs.graphFileType = string(optarg);
				break;

			case 'M':
				globalArgs.maxKDefective = atoi(optarg);
				break;
		}
	}

	// 开始工作
	Work();

	return 0;
}

