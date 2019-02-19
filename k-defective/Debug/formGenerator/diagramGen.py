
import numpy as np
import formGenerator
import matplotlib.pyplot as plt
import os
import sys
import math

class Ploter:
    def __init__(self):
        self.index = np.arange(4)
        self.form = formGenerator.GetFormDataBase()
        self.barWidth = 0.16
        self.opacity = 0.8
        self.barList = []

    def Plot(self, datafiles, rowN, colN, algoList, y, toFloat):
        def nameFix(str):
            if str == 'Base':
                return "$\mbox{MADEC}^+$"
            elif str == 'Base-noColor':
                return "MADEC+diam"
            elif str == 'Base-noDiam':
                return "MADEC+color"
            elif str == 'Base-noImprove':
                return "MADEC"
            else:
                return str
        def nameListFix(lst):
            for i in range(len(lst)):
                lst[i] = nameFix(lst[i])
            return lst

        fig, axs = plt.subplots(rowN, colN)
        cnt = 0
        for i in range(rowN):
            for j in range(colN):
                datafile = datafiles[i * colN + j]
                self.barList = []
                for algo in algoList:
                    DB = self.form.DB.Find({"datafile": datafile, "algorithm": algo}).Sort('k', '<')
                    dataList = []
                    for row in DB.Rows:
                        dataList.append(toFloat(row.dic[y]))
                    print(dataList)
                    bar = axs[i, j].bar(self.index + (len(self.barList) * self.barWidth), dataList, self.barWidth, log = True)
                    self.barList.append(bar)
                axs[i, j].set_xticks(self.index + (len(self.barList) - 1) * self.barWidth / 2)
                axs[i, j].set_xticklabels(('1', '2', '3', '4'))
                if j == 0:
                    axs[i, j].set_ylabel(y + " (log)")
                #if i == rowN - 1:
                #    axs[i, j].set_xlabel('k')
                axs[i, j].set_xlabel('k')
                axs[i, j].set_title("$" + datafile + "$")
                #axs[i, j].legend(self.barList, algoList)
        plt.rc('text', usetex=True)
        plt.rc('text.latex', preamble=r'\usepackage[normalem]{ulem}')
        axs[1, 1].legend(self.barList, nameListFix(algoList), ncol = len(algoList), loc = 'upper center', bbox_to_anchor = (-0.2, 2.9))
        plt.tight_layout()
        plt.subplots_adjust(top = 0.86)
        plt.show()

    def PlotPrework(self, y1, y2, lim, toFloat):
        datafiles = self.form.DB.ValueSetOfKey('datafile')
        fig, ax = plt.subplots()
        before = []
        after = []
        self.index = []
        for datafile in datafiles:
            lim -= 1
            if lim < 0:
                break
            DB = self.form.DB.Find({"datafile": datafile})
            if toFloat(DB.Rows[0].dic[y1]) < 1e4:
                lim += 1
                continue
            self.index.append(datafile.replace('\\', ''))
            before.append(toFloat(DB.Rows[0].dic[y1]))
            after.append(toFloat(DB.Rows[0].dic[y2]))
        self.barList.append(plt.bar(self.index, before, self.barWidth, log = True))
        self.barList.append(plt.bar(self.index, after, self.barWidth, log = True))
        plt.legend(self.barList, ['before', 'after'])
        ax.set_ylabel('Vertex Number (log)')
        labels = ax.get_xticklabels()
        plt.setp(labels, rotation=45, horizontalalignment='right')
        plt.tight_layout()
        plt.show()

    
def sizeToFloat(str):
    return float(str)

def timeToFloat(str):
    return float(str[0:-1])


def main():
    #ploterSize = Ploter()
    #ploterSize.Plot(
    #        ['web-Google.txt', 'rgg\_n\_2\_19\_s0.graph', 'astro-ph.graph', 'email.graph'], 
    #        2, 2, ['RDS', 'Base-noImprove', 'Base-noColor', 'Base-noDiam', 'Base'], 'size of search tree', sizeToFloat)

    ploterTime = Ploter()
    ploterTime.Plot(
            ['web-Google.txt', 'rgg\_n\_2\_19\_s0.graph', 'astro-ph.graph', 'email.graph'], 
            2, 2, ['RDS', 'Base-noImprove', 'Base-noColor', 'Base-noDiam', 'Base'], 'cost of time', timeToFloat)

    #ploter = Ploter()
    #ploter.PlotPrework('number of vertex before prework', 'number of vertex after prework', 25, sizeToFloat)
    pass

if __name__ == '__main__':
    sys.exit(main())
