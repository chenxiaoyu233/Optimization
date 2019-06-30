# Function: 通过读取数据文件, 生成bash命令 (for KDefective)

# Librarys:
import sys
import os

# Parameters: 用于控制生成的指令

# k值的取值范围(n的倍数)
kList = [1, 4]

# 需要测试的算法类型
algoList = ['Base', 'RDS', 'Simple']

# 时间上限
timeLimit = 18000 # = 2 * 3600

# 数据集的位置
#dataDir = "./graph"
dataDir = "../../msbundle/benchmark/benchmark/selected"

# 存储结果的位置
targetDir = "./result"

# 可执行文件的存储位置
execDir = "./KDefective"

# 命令的模板
#cmd = "{0} -O solve -a {1} -t {2} -D {3} -k {4} -r {5} -G {6} -M {7} -p {8} > {9}" # @todo
cmd = "sbatch task.slurm {0} {1} {2} {3} {4} {5} {6} {7} {8} {9}"

# Result
command = ['#!/bin/bash']


def FindNInDataFile(data):
    f = open(data, 'r')
    buff = f.read()
    lines = buff.split('\n')
    for line in lines:
        line = line.split(' ')
        if line[0] == 'p':
            for s in line:
                if s.isdigit():
                    return int(s)


def AddCommand(algorithm, k, dataFile, resultDir, cliqueSize, fileType, dataStructure, cnt):
    global execDir, timeLimit
    command.append(
        cmd.format(
            execDir, algorithm, timeLimit, dataStructure, k, dataFile,
            fileType, cliqueSize,
            ResultFile(
                resultDir,
                dataFile,
                algorithm,
                k
            ),
            cnt
        )
    )

def ResultFile(resultDir, dataFile, algorithm, k):
    return os.path.join(
        resultDir, 
        '-'.join([os.path.basename(dataFile), algorithm, str(k)])
    )

def main(argv = sys.argv):
    fin = open("TestList_10th.txt", "r")
    cases = fin.read().split('\n')
    for item in cases:
        item = item.split(' ')
        if len(item) < 4:
            break
        fileName = item[0]
        # 数据文件
        dataFile = os.path.join(dataDir, fileName)

        # 结果文件夹
        resultDir = os.path.join(targetDir, fileName)

        # 创建结果文件夹
        if not os.path.exists(resultDir):
            os.mkdir(resultDir)

        # 生成指令
        cnt = 0
        for t in kList:
            for algo in algoList:
                cnt += 1
                AddCommand(algo, t, dataFile, resultDir, item[1], item[2], item[3], cnt)
    
    # 将指令输出
    for line in command:
        print(line)




if __name__ == "__main__":
    sys.exit(main())
