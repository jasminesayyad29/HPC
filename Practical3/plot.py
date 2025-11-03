import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('result.csv')

for size in df['Matrix Size'].unique():
    subset = df[df['Matrix Size'] == size]
    plt.plot(subset['Threads'], subset['Speedup'], 'o-', label=f'{size}x{size}')

plt.xlabel('Number of Threads')
plt.ylabel('Speedup (Relative to 1 thread)')
plt.title('OpenMP Matrix Addition Scaling')
plt.legend()
plt.grid(True)
plt.savefig('speedup_plot.png')
plt.show()