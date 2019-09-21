
import os

#cmd_t = './KDefective -O generate -n \"{}\" -d \"{}\" -w {}'
cmd_t = 'python3 RGPL.py {} {} \"0.99\" > {}'
filename_t = 'RandomGraph-{}-{}'
#                    size  density

testCase = {
    'sizes' : [100, 200],
    'densities' : [0.02, 0.04, 0.06, 0.08, 0.10]
}


# @gen means generate
# @param tc: testCase
# @ret testList
def gen(tc):
    testList = []
    for size in tc['sizes']:
        for density in tc['densities']:
            filename = filename_t.format(size, density)
            cmd = cmd_t.format(size, int(density * size / 2), filename)
            os.system(cmd)
            print(cmd)
            testList.append([filename, 0, "clq", "Bitset"])
    return testList

def printTestList(testList):
    for line in testList:
        for item in line:
            print(item, end = " ")
        print()

#testList = gen(testCase1)
#testList = testList + gen(testCase2)
testList = gen(testCase)
printTestList(testList)


