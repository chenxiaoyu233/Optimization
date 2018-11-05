# Function: 通过读取数据文件, 生成bash命令 (for KDefective)

# Librarys:
import sys
import os

# Parameters: 用于控制生成的指令

# k值的取值范围(n的倍数)
kList = [1, 2, 3, 4, 5]

# 需要测试的算法类型
algoList = ['Base', 'RDS', 'Simple']

# 需要测试的图的规模
# 点数
vertexNum = [20, 30]
# 边密度
edgeDensity = [0.1, 0.2]

# 测试组数
caseNum = 5

# 时间上限
timeLimit = 172800 # = 2 * 24 * 3600

# 数据集的位置
dataDir = "./graph"

# 存储结果的位置
targetDir = "./result"

# 可执行文件的存储位置
execDir = "./KDefective"

# 命令的模板
solveCmd = "{} -O solve -a {} -t {} -D Bitset -k {} -r {} > {}" 
generateCmd = "{} -O generate -n {} -d {} -w {}"

# Result:
command = []

def ResultFile(resultDir, dataFile, algorithm, k):
    return os.path.join(
        resultDir, 
        '-'.join([os.path.basename(dataFile), algorithm, str(k)])
    )

def AddSolveCommand(algorithm, k, dataFile, resultDir):
    global execDir, timeLimit
    command.append(
        solveCmd.format(
            execDir, algorithm, timeLimit, k, dataFile,
            ResultFile(
                resultDir,
                dataFile,
                algorithm,
                k
            )
        )
    )

def AddGenerateCommand(n, d, graphPath):
    global execDir
    command.append(
        generateCmd.format(
            execDir, n, d, graphPath
        )
    )

def main(argv = sys.argv):
    for n in vertexNum:
        for d in edgeDensity:
            for case in range(0, caseNum):
                # 生成随机图
                graphPath = os.path.join(dataDir, '-'.join([str(n), str(d), str(case)]))
                AddGenerateCommand(n, d, graphPath);
    
    # 将指令输出
    for line in command:
        print(line, end  = '\n')


if __name__ == "__main__":
    sys.exit(main())
