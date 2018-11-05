# 读取数据并绘图

# library
import numpy as np
import matplotlib.pyplot as plt
import sys
import os

# 获取每个Case对应的文件夹的路径
def getCasePath(dir):
    dirNames = os.listdir(dir)
    dirs = []
    for name in dirNames:
        path = os.path.join(dirNames, name)
        dirs.append(path)

# 为每个Case单独作图
maxt = 172800
maxc = 100000000
kl = [1, 2, 3, 4, 5]
def drawForCase(caseDir):
    caseName = os.path.basename(caseDir)
    # 数据池
    data = {}
    data["k"] = [caseName.split('-')[0] * i for i in kl]
    data["Base"]  = {"time":[maxt] * len(kl), "count":[maxc] * len(kl)}
    data["RDS"]   = {"time":[maxt] * len(kl), "count":[maxc] * len(kl)}
    data["Simple"]= {"time":[maxt] * len(kl), "count":[maxc] * len(kl)}

    # 读数据
    for f in listdir(caseDir):
        filepath = os.path.join(caseDir, f)
        fin = open(filepath, 'r')
        raw = fin.read()
        raw = raw.split('\n')
        cnt = int(raw[1].split(' ')[4])
        tim = float(raw[2].split(' ')[3][:-1])
        flg = int(raw[3].split(' ')[3])

        idx = int(f.split('-')[4]) // int(f.split('-')[0]) - 1
        if flg == 1:
            data[f.split('-')[3]]["time"] = tim
            data[f.split('-')[3]]["count"] = cnt
        pass
    pass

    # 绘图
    plt.figure(1)
    # 绘制时间大小
    plt.subplot(211)
    plt.plot(data['k'], data['Base']['time'],
             data['k'], data['RDS']['time'],
             data['k'], data['Simple']['time'])

    # 绘制搜索树大小
    plt.subplot(212)
    plt.plot(data['k'], data['Base']['count'],
             data['k'], data['RDS']['count'],
             data['k'], data['Simple']['count'])


# 主函数
def main(argv = sys.argv):
    dirs = getCasePath(argv[1])
    for case in dirs:
        drawForCase(case)

if __name__ == "__main__":
    sys.exit(main())
