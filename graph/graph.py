#read lines from file and create a graph
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import sys;
import math;
if(len(sys.argv) != 2):
    print("Usage: python3 graph.py <filename>")
    exit()
counter = 0
data_rn = np.empty((5, 12, 4))
data_th = np.empty((12, 5, 4))
labels = []
old_th = 0

for i in range(0, 1):
    f = open('.\output_sequential.out', "r")
    x = f.readline()
    while x:
        x = x.split('\n')
        x = x[0].split(',')
        [v, th] = [int(x[0].split()[1]), int(x[1].split()[1])]
        if th != old_th:
            rg = 0
        else:
            rg += 1
        labels.append([v, th, rg])
        t0 = float(f.readline().split('\n')[0])
        t1 = float(f.readline().split('\n')[0])
        t2 = float(f.readline().split('\n')[0])
        t3 = float(f.readline().split('\n')[0])
        data_rn[rg-1][th-1] = [t0, t1, t2, t3]
        data_th[th-1][rg-1] = [t0, t1, t2, t3]
        x = f.readline()
        x = f.readline()
        x = f.readline()
        old_th = th
    
print(labels)
print(data_rn[0])

# X = np.arange(1, 13, 1) 
# for i in range(0, 36):
#     plt.figure()
#     Y = data_rn[i]
#     plt.stackplot(X, Y.T, baseline="zero")
#     plt.title('Bucket amount {:.2%}'.format(1/(2000 - 50*i)))
#     plt.axis('tight')
#     plt.xlabel('Threads')
#     plt.ylabel('Time (s)')
#     plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
#     plt.xticks(np.arange(1, 13, 1))
#     plt.grid()
#     plt.xlim(1, 12)
    
#     plt.legend(['Assign bucket', 'Merge buckets of the same range', 'Quick sort buckets', 'Merge results'])
# plt.show()

X = np.arange(3, 8, 1)
for i in range(0, 1):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    Y = data_th[i]
    ax.stackplot(X, Y.T, baseline="zero")
    plt.title('Thread amount {0}'.format(i+1))
    plt.axis('tight')
    plt.xlabel('Problem size 10^(n)')
    
    plt.ylabel('Time (s)')
    plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    # set style for x axis to be integer
    
    plt.grid()  
    plt.xlim(2.9, 7)
    plt.xticks(X)
    
    
    plt.legend(['Assign bucket', 'Merge buckets of the same range', 'Quick sort buckets', 'Merge results'])
plt.show()


    

