import pickle


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

def printout(results):
	for ind in range(len(results)):
		print("Option #" + str(ind))
		print("	Sets: ", results[ind][1][0])
		print("	Blocks: ", results[ind][1][1])
		print("	Allocation: ", get_alloc(results[ind][1][2]))
		print("	Thr/Bck: ", get_thr(results[ind][1][3]))
		print("	Mode: ", get_LRU(results[ind][1][4]))
		print("	Score: ", results[ind][0])

formatted = pickle.load(open( "results.p", "rb" ) )
printout(formatted)
