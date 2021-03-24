#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
import sys

if __name__ == '__main__':
  # load data from given file
  filename = "filter.txt" if len(sys.argv)<2 else sys.argv[1]
  header = np.loadtxt(filename, dtype=str, max_rows=1)
  data = np.loadtxt(filename, skiprows=1)
  # time signal
  t = data[:,0]
  # prepare plot
  fig,ax = plt.subplots()
  # ax.set_xticks([])
  # ax.set_yticks([])
  ax.grid()
  # ax.set_facecolor('k')
  fig.subplots_adjust(left=0, bottom=0, right=1, top=1, wspace=0, hspace=0)

  # plot signals
  for i in range(1,np.size(data,1)):
    ax.plot(t, data[:,i], label=header[i], linewidth=2)
  ax.legend()

  outname = sys.argv[2] if len(sys.argv)>2 else ".".join(filename.split(".")[:-1]) + ".png"
  fig.savefig(outname)
