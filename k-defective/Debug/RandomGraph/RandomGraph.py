
import os

cmd_t = 'KDefective -O generate -n \"{}\" -d \"{}\" -w {}'
filename_t = 'RandomGraph-{}-{}'
#                    size  density

# small graph with all kinds of density
testCase1 = {
    'sizes': [50, 100, 200, 300, 400, 500],
    'densities' : [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
}

# large sparse graph
testCase2 = {
    'sizes' : [1000, 2000, 3000, 4000],
    'densities' : [0.02, 0.04, 0.06, 0.08, 0.1]
}


# @gen means generate
# @param tc: testCase
# @ret testList
def gen(tc):
    testList = []
    for size in tc['sizes']:
        for density in tc['densities']:
            filename = filename_t.format(size, density)
            cmd = cmd_t.format(size, density, filename)
            os.system(cmd)
            testList.append([filename, 0, "clq", "Bitset"])
    return testList

def printTestList(testList):
    for line in testList:
        for item in line:
            print(item, end = " ")
        print()

testList = gen(testCase1)
testList = testList + gen(testCase2)
printTestList(testList)


