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
        path = os.path.join(dir, name)
        dirs.append(path)
    return dirs

# 为每个Case单独作图
maxt = 172800
maxc = 100000000
kl = [1, 2, 3, 4, 5]
def drawForCase(caseDir):
    caseName = os.path.basename(caseDir)
    if caseName[0] == '.':
        return
    # 数据池
    data = {}
    data["k"] = kl
    data["Base"]  = {"time":[maxt] * len(kl), "count":[maxc] * len(kl), "ans":[0] * len(kl)}
    data["RDS"]   = {"time":[maxt] * len(kl), "count":[maxc] * len(kl), "ans":[0] * len(kl)}
    data["Simple"]= {"time":[maxt] * len(kl), "count":[maxc] * len(kl), "ans":[0] * len(kl)}

    # 读数据
    for f in os.listdir(caseDir):
        if f[0] == '.':
            continue
        filepath = os.path.join(caseDir, f)
        fin = open(filepath, 'r')
        raw = fin.read()
        raw = raw.split('\n')
        ans = int(raw[0].split(' ')[1])
        cnt = int(raw[1].split(' ')[4])
        tim = float(raw[2].split(' ')[3][:-1]) * 1000.0
        flg = int(raw[3].split(' ')[3])

        idx = int(f.split('-')[4])  - 1

        data[f.split('-')[3]]["ans"][idx] = ans
        if flg == 0:
            data[f.split('-')[3]]["time"][idx] = tim
            data[f.split('-')[3]]["count"][idx] = cnt
        pass
    pass

    # 绘图
    plt.figure(1)
    # 绘制时间大小
    plt.subplot(311)
    plt.title(caseName)
    plt.plot(data['k'], data['Base']['time'], label = 'Base')
    plt.plot(data['k'], data['RDS']['time'], label = 'RDS')
    plt.plot(data['k'], data['Simple']['time'], label = 'Simple')
    plt.legend()

    # 绘制搜索树大小
    plt.subplot(312)
    plt.plot(data['k'], data['Base']['count'], label = 'Base')
    plt.plot(data['k'], data['RDS']['count'], label = 'RDS')
    plt.plot(data['k'], data['Simple']['count'], label = 'Simple')
    plt.legend()

    # 绘制答案大小
    plt.subplot(313)
    plt.plot(data['k'], data['Base']['ans'], label = 'Base')
    plt.plot(data['k'], data['RDS']['ans'], label = 'RDS')
    plt.plot(data['k'], data['Simple']['ans'], label = 'Simple')
    plt.legend()

    plt.show()

# 主函数
def main(argv = sys.argv):
    dirs = getCasePath(argv[1])
    for case in dirs:
        drawForCase(case)

if __name__ == "__main__":
    sys.exit(main())
