
import numpy as np
import matplotlib.pyplot as plt

# Plotting speedup and acled speedup for the problem
# of adding together W numbers on p processors.
def parallel_runtime(n, p):
	return n / p - 1 + 11 * np.log2(p)

if __name__ == '__main__':
	p = np.array([1, 4, 16, 64, 256])
	W = 256

	speedup = parallel_runtime(W, 1) / parallel_runtime(W, p)
	scaled = parallel_runtime(p * W, 1) / parallel_runtime(p * W, p)

	plt.plot(p, speedup, "-b", label = "Speedup");
	plt.plot(p, scaled, "-r", label = "Scaled speedup");
	plt.legend(loc = "upper left")
	plt.show()
