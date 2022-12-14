import numpy as np
import sys
import os
import subprocess
import pickle
total_size = 16384*2
byte_size = 4
set_max = int(total_size/byte_size)
files = ["gcc.trace","swim.trace"]
# files = ["read01.trace"]

def printout(results):
	for ind in range(len(results)):
		print("Option #" + str(ind))
		print("	Sets: ", results[ind][1][0])
		print("	Blocks: ", results[ind][1][1])
		print("	Allocation: ", get_alloc(results[ind][1][2]))
		print("	Thr/Bck: ", get_thr(results[ind][1][3]))
		print("	Mode: ", get_LRU(results[ind][1][4]))
		print("	Score: ", results[ind][0])

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

results = {}
print("Total Size: " + str(total_size) + "bytes")
for file in files:
	with open(file, 'rb') as f:
		data = f.read()
	print("File: " + file)
	Set = set_max
	while Set >= 1:
		Block = round(total_size/(Set*byte_size))
		print("Num Sets: " + str(Set))
		print("Num Blocks: " + str(Block))
		print("Total Space: " + str(Block*Set*byte_size))
		for LRU in range(0,2):
			for write_alloc in range(0,2):
				for write_thr in range(0,2):
					return_data = subprocess.run(["./csim", str(Set), str(Block), str(byte_size), str(get_alloc(write_alloc)), str(get_thr(write_thr)), str(get_LRU(LRU))], input = data, capture_output = True, shell=False, check=False)
					output = return_data.stdout.split()
					if len(output) > 0:
						# print(int(output[-1]))
						if (Set,Block,write_alloc,write_thr,LRU) not in results.keys():
							results[(Set,Block,write_alloc,write_thr,LRU)] = (int(output[-1]))
						else: 
							results[(Set,Block,write_alloc,write_thr,LRU)] += (int(output[-1]))
		Set = Set>>1
results = [list(results.keys()),list(results.values())]
formatted = []
for index in range(len(results[0])):
	formatted.append((results[1][index],results[0][index]))
formatted.sort()
pickle.dump(formatted, open( "results.p", "wb" ) )
printout(formatted)

