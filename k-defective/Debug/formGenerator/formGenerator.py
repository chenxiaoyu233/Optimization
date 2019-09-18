
import sys
import os
import functools

# 一条测试记录
class Row:
    # 读文件, 并将文件中的信息打包成为一个字典
    def __readFile(self, filename):
        f = open(filename, 'r')
        lines = f.read().split('\n')
        for line in lines:
            pair = line.split(' ')
            if len(pair) < 2:
                continue
            if len(pair) > 2:
                tmp1 = '-'.join(pair[0:-1])
                pair = [tmp1, pair[-1]]
            pair[0] = pair[0].replace('-', ' ')
            pair[0] = pair[0][0:-1] # 去掉最后的冒号
            self.dic[pair[0]] = pair[1]


    # 分析文件名, 形成主键
    def __parseFileName(self, filename):
        filename = os.path.basename(filename)
        keys = filename.split('-')
        self.dic['datafile'] = '-'.join(keys[0:-2])
        self.dic['datafile'] = self.dic['datafile'].replace('_', '\\_')
        self.dic['algorithm'] = keys[-2]
        self.dic['k'] = keys[-1]

    def __init__(self, filename, outerDic = None):
        self.dic = {'ans': 'X',
                    'size of search tree': 'X',
                    'cost of time': 'X',
                    'time out flag': 'X',
                    'number of vertex before prework': 'X',
                    'number of vertex after prework': 'X',
                    'ans lower bound': 'X'}
        self.__readFile(filename)
        self.__parseFileName(filename)
        # 优先使用参数传递的键值对
        if outerDic != None:
            for k, v in outerDic.items():
                self.dic[k] = v
        #print(self.dic)


# 所有测试集合构成的数据库
class DataBase:
    def __init__(self, initRows = []):
        self.Rows = initRows

    def AddRow(self, filename, outerDic = None):
        self.Rows.append(Row(filename, outerDic))

    def __genCompareFuncByKey(self, keys, kind = '<'):
        def cmp1(a, b):
            # 放在前面的key优先级比较高
            for key in keys:
                if a.dic[key] != b.dic[key]:
                    return -1 if a.dic[key] < b.dic[key] else 1
            return 0

        def cmp2(a, b):
            return -cmp1(a, b)

        return cmp1 if kind == '<' else cmp2

    def Sort(self, keys, kind = '<'):
        # 拷贝原来的序列
        tmp = []
        for row in self.Rows:
            tmp.append(row)
        tmp.sort(key = functools.cmp_to_key(self.__genCompareFuncByKey(keys, kind)))
        return DataBase(tmp)

    def Find(self, dic):
        tmp = []
        for row in self.Rows:
            flag = True
            for k, v in dic.items():
                if v != row.dic[k]:
                    flag = False
                    break
            if flag:
                tmp.append(row)
        return DataBase(tmp)

    def ValueSetOfKey(self, key):
        dic = {}
        for row in self.Rows:
            dic[row.dic[key]] = -1
        tmp = []
        for k in dic.keys():
            tmp.append(k)
        return tmp

# 用于生成表格的类型
class FormGenerator:
    def __init__(self):
        self.DB = DataBase()

    # 添加一个目录测试结果
    def AddDirectory(self, dirPath, outerDic = None):
        files = os.listdir(dirPath)
        for fname in files:
            f = os.path.join(dirPath, fname)
            # 忽略隐藏文件
            if fname[0] == '.': 
                continue
            # 读取文件
            if os.path.isdir(f):
                self.AddDirectory(f, outerDic)
            elif os.path.isfile(f):
                self.DB.AddRow(f, outerDic)

    # print the form in the way you like
    def PrintForm(self):
        print("% vim: set filetype=tex :")
        f = open('head.tex', 'r')
        print(f.read())
        f.close()
        datafiles = self.DB.ValueSetOfKey('datafile')
        algorithms = self.DB.ValueSetOfKey('algorithm')
        for datafile in datafiles:
            print("\\hline")
            print("\\multirow{{{}}}{{*}}{{{}}}".format(len(algorithms), datafile), end = " ")
            print("& \\multirow{{{}}}{{*}}{{{}}}".format(len(algorithms), 
                self.DB.Find({'algorithm': 'Base', 'datafile': datafile}).Rows[0].dic['number of vertex before prework']), end = " ")
            print("& \\multirow{{{}}}{{*}}{{{}}}".format(len(algorithms), 
                self.DB.Find({'algorithm': 'Base', 'datafile': datafile}).Rows[0].dic['number of vertex after prework']))
            formatter = "& {}"
            for algorithm in algorithms:
                print(formatter.format(algorithm), end = " ")
                #DB = self.DB.Find({'algorithm': algorithm, 'datafile': datafile}).Sort(['k'], '<')
                for i in range(1, 5):
                    DB = self.DB.Find({'algorithm': algorithm, 'datafile': datafile, 'k': str(i)})
                    if len(DB.Rows) > 0: 
                        row = DB.Rows[0]
                        print("& " + row.dic['ans'], end = " ")
                        print("& " + row.dic['size of search tree'], end = " ")
                        print("& " + row.dic['cost of time'], end = " ")
                        print("& " + row.dic['time out flag'], end = " ")
                    else:
                        print("& X & X & X & X", end = " ")
                print("\\\\") 
                formatter = "& & & {}"
        f = open('tail.tex', 'r')
        print(f.read())
        f.close()

    def timeToFloat(self, str):
        if str == 'X': 
            return 'X'
        else: 
            return float(str[0:-1])

    def DropDataFiles(self, datafiles, algorithms):
        ret = []
        for datafile in datafiles:
            Flag = True
            LFlag = False
            for k in range(1, 5):
                RFlag = False
                for algorithm in algorithms:
                    curDB = self.DB.Find({'algorithm': algorithm, 'datafile': datafile, 'k': str(k)})
                    curT = self.timeToFloat(curDB.Rows[0].dic['cost of time']) if len(curDB.Rows) > 0 and curDB.Rows[0].dic['time out flag'] == '0' else 'X'
                    if curT != 'X' and curT > 5:
                        LFlag = True
                    if curT != 'X' and curT < 18000:
                        RFlag = True
                if not RFlag:
                    Flag = False
                    break
            if Flag and LFlag:
                ret.append(datafile)
        return ret

    def printTex(self, filename):
        f = open(filename, "r")
        print(f.read())
        f.close()
        
    def PrintFormSilm(self):
        """
        \hline
        \multirow{4}{*}{memplus.graph (X,X,X)} & 1 &X & X &X & X\\
                                & 2 &X & X &X & X\\\
                                & 3 &X & X &X & X\\
                                & 4 &X & X &X & X\\
        """
        # print the shhead.tex
        self.printTex('shhead.tex')
        total_cnt = 0
        gt100_cnt = 0
        mean_times_cnt = 0
        mean_times = 0 # GLPK 时间对MADEC+的平均倍数
        self.AddDirectory("Graph-Info/result")
        self.AddDirectory('GraphInfo-DEMACS/')
        datafiles = self.DB.ValueSetOfKey('datafile')
        algorithms = ['Base', 'Base-noImprove', 'RDS', 'CPLEX']
        datafiles = self.DropDataFiles(datafiles, algorithms)
        # print the shead.tex
        self.printTex('shead.tex')
        tabular_line_cnt = 0
        for datafile in datafiles:
            print("\\hline")
            print("\\multirow{{{}}}{{*}}{{{}}}".format(
                2,
                datafile,
                ))
            for k in range(1, 5):
                if k == 3:
                    print("\\multirow{{{}}}{{*}}{{({}, {})}}".format(
                        2,
                        self.DB.Find({'algorithm': 'OnlyRead', 'datafile': datafile}).Rows[0].dic['number of vertex before prework'],
                        self.DB.Find({'algorithm': 'OnlyRead', 'datafile': datafile}).Rows[0].dic['number of edge read in']
                        ))

                print(' & {}'.format(k), end = ' ')
                print(' & {}'.format(self.DB.Find({'algorithm': 'Base-noImprove', 'datafile': datafile, 'k': str(k)}).Rows[0].dic['ans lower bound']), end = ' ')
                #print(' & {}'.format(self.DB.Find({'algorithm': 'Base', 'datafile': datafile, 'k': str(k)}).Rows[0].dic['ans']), end = ' ')
                ans = 'X'
                for algo in algorithms:
                    if len(self.DB.Find({'datafile': datafile, 'k': str(k), 'algorithm': algo}).Rows) == 0:
                        continue
                    row = self.DB.Find({'datafile': datafile, 'k': str(k), 'algorithm': algo}).Rows[0]
                    if row.dic['time out flag'] != 'X' and row.dic['time out flag'] == '0':
                        ans = row.dic['ans']
                print(' & {}'.format(ans), end = ' ')
                
                times = []
                for algorithm in algorithms:
                    tmp = self.DB.Find({'algorithm': algorithm, 'datafile': datafile, 'k': str(k)}).Rows
                    cur = 'X'
                    if len(tmp) > 0:
                        cur = tmp[0].dic['cost of time']
                        if cur != 'X' and tmp[0].dic['time out flag'] == '0':
                            cur = float(cur[0:-1])
                        else: 
                            cur = 'X'
                    #print(' & {}'.format(cur), end = ' ')
                    times.append(cur)

                # 计算倍数
                if (times[0] != 'X' and times[0] != 0 and times[3] != 'X'):
                    mean_times_cnt += 1
                    mean_times += float(times[3]) / float(times[0])
                    if (float(times[3]) / float(times[0]) >= 100):
                        gt100_cnt += 1
                if (times[3] == 'X'):
                    gt100_cnt += 1
                    total_cnt += 1
                    

                minTime = 1e10
                whe = 0
                cnt = 0
                flag = 0
                for i in range(len(times)):
                    if times[i] == 'X':
                        continue
                    if minTime > times[i]:
                        minTime = times[i]
                        whe = i
                    times[i] = float('{:.2f}'.format(times[i]));
                    if times[i] < 0.015:
                        times[i] = '0.0'
                        cnt += 1
                    elif times[i] > 18000:
                        times[i] = '$>$18k'
                    else:
                        flag = 1
                        times[i] = '{:.2f}'.format(times[i]);
                if cnt < 2 and flag == 1:
                    times[whe] = '\\textbf{{{}}}'.format(times[whe])

                # 打印时间
                for time in times:
                   print(' & {}'.format(time), end = ' ')
                print('\\\\')
            tabular_line_cnt = tabular_line_cnt + 1
            if tabular_line_cnt == 15:
                self.printTex('stail.tex')
                self.printTex('shead.tex')
                tabular_line_cnt = 0

        self.printTex('stail.tex')
        self.printTex('sttail.tex')
        
        # 输出GLPK相对于MADEC+的平均倍数
        print(mean_times)
        print(mean_times_cnt)
        print(mean_times / mean_times_cnt)
        total_cnt += mean_times_cnt
        print(total_cnt)
        print(gt100_cnt)
        print(gt100_cnt / total_cnt)


def GetFormDataBase():
    # 初始化
    form = FormGenerator()

    # 读入
    form.AddDirectory('Base/result/')
    form.AddDirectory('Base-noColor/result/', {'algorithm': 'Base-noColor'})
    form.AddDirectory('Base-noDiam/result/', {'algorithm': 'Base-noDiam'})
    form.AddDirectory('RDS/result/')
    form.AddDirectory('Ip/result/')
    form.AddDirectory('Base-noImprove/result/', {'algorithm': 'Base-noImprove'})
    form.AddDirectory('CPLEX/result/')

    # DEMACS 2th
    form.AddDirectory('Base-DEMACS/')
    form.AddDirectory('Base-NoImprove-DEMACS/', {'algorithm': 'Base-noImprove'})
    form.AddDirectory('RDS-DEMACS/')
    form.AddDirectory('CPLEX-DEMACS/')


    return form
    

def main():
    form = GetFormDataBase()

    # 输出
    #form.PrintForm()
    form.PrintFormSilm()

if __name__ == '__main__':
    sys.exit(main())
