/*
 * 通过读取实验结果的原始txt生成latex表格
 */
#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include <fstream>
using namespace std;
ifstream in; // 输入文件的指针

void open_file(string file_name) {
	in.open(file_name.c_str(), std::ifstream::in);
}

void close_file() {
	in.close();
}

int state;
char buffer[1000];

void before() {
	for(int i = 0; buffer[i]; i++) if (buffer[i] == ':') {
		buffer[i] = '\0';
		return;
	}
}

void after() {
	int i = 0;
	for (; buffer[i] && buffer[i] != ':'; i++) {}
	i += 2;
	int pt = 0;
	for (; buffer[i]; i++) buffer[pt++] = buffer[i];
	buffer[pt] = '\0';
}

char midBuffer[2000];
void dealUnderLine() {
	int pt = 0;
	for (int i = 0; buffer[i]; i++) {
		if (buffer[i] == '_') {
			midBuffer[pt++] = '\\';
		}
		midBuffer[pt++] = buffer[i];
	}
	midBuffer[pt] = '\0';
	strcpy(buffer, midBuffer);
}

string head = \
"\\documentclass{article}\n"
"\\usepackage{amsmath}\n"
"\\usepackage{multirow}\n"
"\\usepackage{graphicx}\n"
"\\usepackage{geometry}\n"
"\n"
"\\geometry{a4paper,left=2cm,right=2cm,top=1cm,bottom=1cm}\n"
"\n"
"\\begin{document}\n"
"\\resizebox{16cm}{10cm}{\n"
"\\begin{tabular}{|c|c|c|c|c|c|c|c|c|c|c|c|c|c|c|c|c|c|}\n"
"\\hline\n"
"\\multirow{2}{*}{test bench} & \\multirow{2}{*}{Algorithm} & \\multicolumn{4}{|c|}{1-defective} & \\multicolumn{4}{|c|}{2-defective} & \\multicolumn{4}{|c|}{3-defective} & \\multicolumn{4}{|c|}{4-defective} \\\\\n"
"\\cline{3-18}\n"
"& & ans & tree-size & time & flag& ans & tree-size & time & flag& ans & tree-size & time & flag& ans & tree-size & time & flag \\\\\n"
"\\hline\n";

string tail = \
"\\hline\n"
"\\end{tabular}\n"
"}\n"
"\\end{document}\n";

void process() {
	cout << head;
	while(!in.eof()) {
		in.getline(buffer, 1000);
		if(strlen(buffer) == 1) {
			state = 0;
			printf("\\hline\n");
			continue;
		}
		dealUnderLine();
		if (state == 0) {
			before();
			printf("\\multirow{2}{*}{%s}", buffer);
		} else if (state == 1 || state == 18) {
			before();
			printf(" & %s", buffer);
		} else {
			after();
			printf(" & %s", buffer);
			if (state == 17 || state == 34) printf("\\\\\n");
		}
		//printf("\n%d\n", state);
		state++;
	}
	cout << tail;
}

int main(int argc, char **argv) {
	if(argc != 2) return 233;
	open_file(string(argv[1]));
	process();
	close_file();
	return 0;
}
