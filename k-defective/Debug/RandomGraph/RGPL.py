import sys
import networkx as netx
import functools

G = netx.powerlaw_cluster_graph(int(sys.argv[1]), int(sys.argv[2]), float(sys.argv[3]))

def graph_output():
    print ('p edge {} {}'.format(G.number_of_nodes(), G.number_of_edges()))
    for e in G.edges():
        print ('e {} {}'.format(e[0] + 1, e[1] + 1))
graph_output()

# check if the graph satisfies the power law
def power_law_checker ():
    du = [0] * G.number_of_nodes()
    for e in G.edges:
        du[e[0]] += 1
        du[e[1]] += 1

    dic = {}
    for i in range(G.number_of_nodes()):
        if dic.get(du[i]) == None:
            dic[du[i]] = 0
        dic[du[i]] += 1

    to_sort = []
    for k in dic.keys():
        to_sort.append((k, dic[k]))

    def my_cmp(a, b):
        if a[0] < b[0]:
            return -1
        if b[0] < a[0]:
            return 1
        return 0

    to_sort.sort(key = functools.cmp_to_key(my_cmp), reverse=True)

    for pr in to_sort:
        print('degree: {} --- num: {}'.format(pr[0], pr[1]))
#power_law_checker()
