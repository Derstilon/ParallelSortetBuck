import os
import numpy as np
from pathlib import Path
import matplotlib.pyplot as plt

def get_arrays():
    result = {}
    current_dir = os.path.dirname(os.path.realpath(__file__))
    filepath = Path(current_dir, "slurm.txt")
    with open(filepath, 'r') as reader:
        while True:
            line = reader.readline()
            if not line:
                break
            line = line.split(' ')
            if result.get(line[0]) == None:
                result[line[0]] = {line[1] : [(line[2], float(line[3]))]}
            elif result[line[0]].get(line[1]) == None:
                result[line[0]][line[1]] = [(line[2], float(line[3]))]
            else:
                result[line[0]][line[1]].append((line[2], float(line[3])))

    for key in result:
        for key2 in result[key]:
            pis = []
            times = []
            for tuple in result[key][key2]:
                pi, time = tuple
                pis.append(float(pi))
                times.append(float(time))
            result[key][key2] = {"pis": pis, "times": times}
    
    transposed = {}

    for key in result:
        for key2 in result[key]:
            try:
                transposed[key2][key] = result[key][key2]
            except:
                transposed[key2] = {}
                transposed[key2][key] = result[key][key2]
    result = transposed
    return result

def time_plot(target):
    for prob_size in target:
        procs = []
        times = []
        stds = []
        for proc_n in target[prob_size]:
            procs.append(int(proc_n))
            times.append(np.average(target[prob_size][proc_n]["times"]))
            stds.append(np.std(target[prob_size][proc_n]["times"]))

        plt.errorbar(x=procs, y=times, yerr=stds, label=prob_size, fmt='--.')

    plt.plot(np.linspace(1, 12), np.linspace(0, 0), 'r--')
    plt.xlabel("Processors N")
    plt.ylabel("Time")
    plt.title("Average Time per N Processors")
    plt.legend()
    plt.show()

def speedup_plot(target):
    for prob_size in target:
        procs = []
        speedups = []
        stds = []
        for proc_n in target[prob_size]:
            procs.append(int(proc_n))
            speedups.append(np.average([np.average(target[prob_size]['1']["times"])/time for time in target[prob_size][proc_n]["times"]]))
            stds.append(np.std([np.average(target[prob_size]['1']["times"])/time for time in target[prob_size][proc_n]["times"]]))

        plt.errorbar(x=procs, y=speedups, yerr=stds, label=prob_size, fmt='--.')

    plt.plot(np.linspace(1, 12), np.linspace(1, 12), 'r--')
    plt.xlabel("Processors N")
    plt.ylabel("Speedup")
    plt.title("Average Speedup per N Processors")
    plt.legend()
    plt.show()


def efficiency_plot(target):
    for prob_size in target:
        procs = []
        efficiencies = []
        stds = []
        for proc_n in target[prob_size]:
            int_proc_n = int(proc_n)
            procs.append(int_proc_n)
            efficiencies.append(np.average([np.average(target[prob_size]['1']["times"])/time for time in target[prob_size][proc_n]["times"]])/int_proc_n)
            stds.append(np.std([np.average(target[prob_size]['1']["times"])/time for time in target[prob_size][proc_n]["times"]])/int_proc_n)

        plt.errorbar(x=procs, y=efficiencies, yerr=stds, label=prob_size, fmt='--.')

    plt.plot(np.linspace(1, 12), np.linspace(1, 1), 'r--')
    plt.xlabel("Processors N")
    plt.ylabel("Efficiency")
    plt.title("Average Efficiency per N Processors")
    plt.legend()
    plt.show()


def serial_fraction_plot(target):
    for prob_size in target:
        procs = []
        speedups = []
        stds = []
        for proc_n in target[prob_size]:
            int_proc_n = int(proc_n)
            if int_proc_n != 1:
                procs.append(int_proc_n)
                speedups.append(np.average([(1/np.average(target[prob_size]['1']["times"])/time - 1/int_proc_n)/(1 - 1/int_proc_n) for time in target[prob_size][proc_n]["times"]]))
                stds.append(np.std([(1/np.average(target[prob_size]['1']["times"])/time - 1/int_proc_n)/(1 - 1/int_proc_n) for time in target[prob_size][proc_n]["times"]]))

        plt.errorbar(x=procs, y=speedups, yerr=stds, label=prob_size, fmt='--.')

    plt.plot(np.linspace(1, 12), np.linspace(0, 0), 'r--')
    plt.xlabel("Processors N")
    plt.ylabel("Serial Fraction")
    plt.title("Average Serial Fraction per N Processors")
    plt.legend()
    plt.show()

arrays = get_arrays()

time_plot(arrays)
speedup_plot(arrays)
efficiency_plot(arrays)
serial_fraction_plot(arrays)