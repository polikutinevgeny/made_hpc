from glob import glob

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm

filenames = sorted(glob("output_*.txt"))
results = []
for filename in filenames:
    with open(filename) as f:
        results.append(np.array([[int(c) for c in line.strip()] for line in f.readlines()]))
results = np.concatenate(results, axis=1)

plt.imsave("plotted_progress.png", 1 - results, cmap=cm.gray)
# plt.figure(figsize=(20, 20))
# plt.imshow(results)
# plt.savefig("plotted_progress.png")
