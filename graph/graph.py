#read lines from file and create a graph
import numpy as np
import matplotlib.pyplot as plt
import sys;
import math;
if(len(sys.argv) != 2):
    print("Usage: python3 graph.py <filename>")
    exit()
f = open(sys.argv[1], "r")
counter = 0
x = f.readline()
data_rn = np.empty((10, 12, 4))
data_th = np.empty((12, 10, 4))
labels = []
while x:
    x = x.split('\n')
    x = x[0].split(',')
    [v, th, rn] = [int(x[0].split()[1]), int(x[1].split()[1]), int(math.log2(int(x[2].split()[1])))]
    labels.append([v, th, rn])
    t0 = float(f.readline().split('\n')[0])
    t1 = float(f.readline().split('\n')[0])
    t2 = float(f.readline().split('\n')[0])
    t3 = float(f.readline().split('\n')[0])
    data_rn[rn-1][th-1] = [t0, t1, t2, t3]
    data_th[th-1][rn-1] = [t0, t1, t2, t3]
    x = f.readline()
    x = f.readline()
    x = f.readline()
    
print(labels)
print(data_rn[0])

X = np.arange(1, 13, 1) 
for i in range(0, 10):
    plt.figure()
    Y = data_rn[i]
    plt.stackplot(X, Y.T, baseline="zero")
    plt.title('Bucket amount {0}'.format(2**(i+1)))
    plt.axis('tight')
    plt.xlabel('Threads')
    plt.ylabel('Time (s)')
    plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    plt.grid()
    plt.xlim(1, 12)
    
    plt.legend(['Assign bucket', 'Merge buckets of the same range', 'Quick sort buckets', 'Merge results'])
# plt.show()

X = np.arange(1, 11, 1) 
for i in range(0, 12):
    plt.figure()
    Y = data_th[i]
    plt.stackplot(X, Y.T, baseline="zero")
    plt.title('Thread amount {0}'.format(i+1))
    plt.axis('tight')
    plt.xlabel('Bucket amount 2^n')
    plt.ylabel('Time (s)')
    plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    plt.grid()  
    plt.xlim(1, 10)
    
    plt.legend(['Assign bucket', 'Merge buckets of the same range', 'Quick sort buckets', 'Merge results'])
plt.show()


    

