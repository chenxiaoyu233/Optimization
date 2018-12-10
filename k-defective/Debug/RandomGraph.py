# Function: 通过读取数据文件, 生成bash命令 (for KDefective)

# Librarys:
import sys
import os

# Parameters: 用于控制生成的指令

# range for k
kList = [1, 2, 3, 4, 5]

# algorithm type for test
algoList = ['Base', 'RDS']

# the scale of the graph
# vertex number
vertexNum = [20, 30, 40, 50, 60, 70, 80, 90, 100]
# edge density
edgeDensity = [0.1]

# test case
caseNum = 1

# time limit
timeLimit = 172800 # = 2 * 24 * 3600

# data set location
dataDir = "./graph"

# result location
targetDir = "./result"

# location for exec file
execDir = "./KDefective"

# template for instruction
solveCmd = "{0} -O solve -a {1} -t {2} -D Bitset -k {3} -r {4} > {5}" 
generateCmd = "{0} -O generate -n {1} -d {2} -w {3}"

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
                # generate random graph
                graphPath = os.path.join(dataDir, '-'.join([str(n), str(d), str(case)]))
                AddGenerateCommand(n, d, graphPath);
    
    # print the command
    for line in command:
        print(line)


if __name__ == "__main__":
    sys.exit(main())
