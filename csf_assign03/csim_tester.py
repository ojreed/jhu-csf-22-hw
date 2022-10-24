import numpy as np
import os
import subprocess

byte_range = [4,50]
block_range = [4,50]
set_range = [4,50]
files = ["read01.trace"]


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


results = np.zeros(shape = (50,50,50,2,2,2))
# print(results)
for file in files:
	for Byte in range(byte_range[0],byte_range[1]):
		for Block in range(block_range[0],block_range[1]):
			for Set in range(set_range[0],set_range[1]):
				for LRU in range(0,1):
					for write_alloc in range(0,1):
						for write_thr in range(0,1):
							inp = subprocess.run("csim" + str(Set) + " " + str(Block) + " " + str(Byte) + " " + str(get_alloc(write_alloc)) + " " + str(get_thr(write_thr)) + " " + str(get_LRU(LRU)) + " < " + str(file), shell=True, check=True)
							results[Set][Block][Byte][write_alloc][write_thr][LRU] = inp.split()[-1]
