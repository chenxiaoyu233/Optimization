import formGenerator as fg
import matplotlib.pyplot as plt
import sys

def plot_sub_random_graph(ax, form, k, n, xlf = False, ylf = False):
    algos = ['Base', 'Base-noImprove', 'RDS', 'CPLEX']
    densities = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
    data = {}
    for algo in algos:
        data[algo] = []
    for density in densities:
        for algo in algos:
            datafile = '-'.join(['RandomGraph', str(n), str(density)])
            res = form.DB.Find({'algorithm': algo, 'datafile': datafile, 'k': str(k)}).Rows
            if len(res) != 0 \
               and res[0].dic['cost of time'] != 'X' \
               and res[0].dic['time out flag'] == '0':
                data[algo].append(float(res[0].dic['cost of time'][0:-1]))
            else:
                data[algo].append(18000)
    for algo in algos:
        ax.plot(densities, data[algo])
    if xlf:
        ax.set_xlabel('density')
    if ylf:
        ax.set_ylabel('cost of time')
    ax.set_title('n = {}, k = {}'.format(n, k))
    ax.set_xmargin(0.5)
    ax.set_ymargin(0.5)
        
def work():
    form = fg.GetFormDataBase()
    fig, axs = plt.subplots(6, 2, constrained_layout=True)

    def get_k(x, y):
        ret = 2 * x + y + 1
        if ret > 6:
            ret -= 6
        return ret

    def get_n(x, y):
        ret = 2 * x + y + 1
        if ret <= 6:
            ret = 100
        else:
            ret = 200
        return ret
        
    for x in range(6):
        for y in range(2):
            ylf = True if y == 0 else False
            xlf = True if x == 5 else False
            plot_sub_random_graph(axs[x][y], form, get_k(x, y), get_n(x, y), xlf, ylf)

    plt.legend(['Base', 'Base-noImprove', 'RDS', 'CPLEX'])
    fig.set_figwidth(8.27)
    fig.set_figheight(12.0)
    fig.savefig(fname = 'random_graph.pdf', format = 'pdf')
    #plt.show()

if __name__ == '__main__':
    sys.exit(work())
