#read lines from file and create a graph
import numpy as np
import matplotlib.pyplot as plt
import sys;
import math;
if(len(sys.argv) != 2):
    print("Usage: python3 graph.py <filename>")
    exit()
data_rn = np.empty((2, 12, 4))
data_th = np.empty((12, 2, 4))

labels = []
ranges = [10, 1400]
for rn in range(0,2):
    f = open('.\output_final{0}.out'.format(rn+2), "r")
    x = f.readline()
    while x:
        x = x.split('\n')
        x = x[0].split(',')
        [v, th] = [int(x[0].split()[1]), int(x[1].split()[1])]
        labels.append([v, th, '{:.2%}'.format(1.0/ranges[rn])])
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

data_times = data_th.transpose()
# assign - 0
# sort - 1
# merge - 2

# X = np.arange(1, 13, 1) 
# for i in range(-2, 4):
#     plt.figure()
#     Y = data_rn[i+2]
#     plt.stackplot(X, Y.T, baseline="zero")
#     plt.title('Bucket amount (2^{0})*sqrt(111000000)'.format(i))#(int(2**(i)*math.sqrt(111000000))))
#     plt.axis('tight')
#     plt.xlabel('Threads')
#     plt.ylabel('Time (s)')
#     plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
#     plt.grid()
#     plt.xlim(1, 12)
    
#     plt.legend(['Assign bucket', 'Quick sort buckets', 'Merge results'])

# X = np.arange(-2, 4, 1) 
# for i in range(0, 12):
#     plt.figure()
#     Y = data_th[i]
#     plt.stackplot(X, Y.T, baseline="zero")
#     plt.title('Thread amount {0}'.format(i+1))
#     plt.axis('tight')
#     plt.xlabel('Bucket amount (2^x)*sqrt(n)')
#     plt.ylabel('Time (s)')
#     plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
#     plt.grid()  
#     plt.xlim(-2, 3)
    
#     plt.legend(['Assign bucket', 'Quick sort buckets', 'Merge results'])




speedup_plot_titles=['Assign bucket', 'Merge buckets of the same range', 'Quick sort buckets', 'Merge results']

for i in range(0,4):
    plt.figure()
    plt.title(speedup_plot_titles[i])
    plt.xlabel('Threads')
    plt.ylabel('Efficiency')
    j = 0
    for bucket_data in data_times[i]:
        speedup = [bucket_data[0]/time_n for time_n in bucket_data]
        efficiency = [bucket_data[0]/(bucket_data[t]*(t+1)) for t in range(0,12)]
        plt.plot(np.linspace(1, 12), np.linspace(1, 1), 'r--')
        plt.xlim(.9, 12)
        plt.xticks(np.arange(1, 13, 1))

        plt.errorbar(x=list(range(1,13)), y=efficiency, label='{:.2%}'.format(1.0/ranges[j]), fmt='--.')
        j += 1
    
    plt.legend()


















plt.show()

