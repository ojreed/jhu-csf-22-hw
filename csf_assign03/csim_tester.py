import numpy as np
import os
import subprocess
import pickle
total_size = 16000
byte_size = 4
set_range = [1,500,1]
files = ["gcc.trace","swim.trace"]


def get_alloc(inp):
	if inp == 0:
		return "write-allocate"
	else:
		return "no-write-allocate"

def get_thr(inp):
	if inp == 0:
		return "write-through"
	else:
		return "write-back"

def get_LRU(inp):
	if inp == 0:
		return "lru"
	else:
		return "fifo"


results = np.zeros(shape = (set_range[1],set_range[1],2,2,2))
print("Total Size: " + str(total_size) + "bytes")
for file in files:
	with open(file, 'rb') as f:
		data = f.read()
	print("File: " + file)
	for Set in range(set_range[0],set_range[1],set_range[2]):
		Block = total_size/(Set*byte_size)
		print("Num Sets: " + str(Set))
		print("Num Blocks: " + str(Block))
		for LRU in range(0,1):
			for write_alloc in range(0,1):
				for write_thr in range(0,1):
					return_data = subprocess.run(["./csim", str(Set), str(Block), str(byte_size), str(get_alloc(write_alloc)), str(get_thr(write_thr)), str(get_LRU(LRU))], input = data, capture_output = True, shell=False, check=False)
					output = return_data.stdout.split()
					if len(output) > 0:
						# print(1/int(output[-1]))
						results[Set][Block][write_alloc][write_thr][LRU] += (int(output[-1]))
print(results)
k = 10
np.argpartition(arr, len(arr) - k)[-k:]
pickle.dump(results , open( "results.p", "wb" ) )