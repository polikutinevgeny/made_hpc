from io import StringIO

import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

time_str = """1,59.81
2,31.11
3,21.46
4,16.19
5,11.46
6,9.90
7,8.94
8,8.08
"""

df = pd.read_csv(StringIO(time_str), header=None, names=["processes", "time"])

plt.figure(figsize=(10, 10))
sns.lineplot(data=df, x="processes", y="time")
plt.savefig("plotted_time.png")
