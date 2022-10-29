Madeline (Maddie) Estey and Owen Reed

In the beginning, we worked together in Brody to create the project's general file and class structure. We did some functions together
but then also worked individually on some and worked individually to debug (ex: debugging why our counter amounts were wrong).

One of the biggest challenges we encountered was how to correctly count cycles.

We had most of part 3 of this homework completed by the due date for part 2. All we had to work on after submitting part 2 was input validation and
cleaning up our code in general.


For part (b), you’ll use the memory traces as well as your simulator to determine which cache configuration has the best overall effectiveness. You should take a variety of properties into account: hit rates, miss penalties, total cache size (including overhead), etc. In your README.txt, describe in detail what experiments you ran (and why!), what results you got (and how!), and what, in your opinion, is the best cache configuration of them all.
Finally, you will write a brief summary of how you divided up the work between partners and what each person contributed. This section is not required if you worked alone.

Results:
	    The bulk of our testing was through a python script we wrote to test every set/block number sized cache with a fixed byte size 
    and a fixed total size. This would allow us to compare the relative performance of differently configured caches. We used 
    total_cycles as the primary metric for ranking cache performance but also took into account the different overhead of different 
    modes, such as write-back’s need for a dirty bit. These tests also tested each of the Allocation, Thr/Back and LRU modes for 
    each set/block configuration. The testing code also takes multiple trace files as an input and returns the score for a 
    configuration as the sum of the cycles of each trace. 
        Write-Allocate and Write back was a clear winner with the top 10 outputs all being write-allocate write-back. Now it makes a 
    great deal of sense why this is the case considering the swap from write-back to write-through would immediately cause a huge 
    increase in writes to DRAM which would add up very quickly. Of course this requires us to take on the added weight of storing a 
    dirty bit, but when the worst write-allocate write back performs almost 4 times fewer cycles than the best write-through, it
    becomes worth it. 
	    Since we found the write-back is a definite benefit it makes sense that we also found write-allocate as its consistent pair 
    because write-back doesn't make sense when used in tandem with no-write-allocate.
	    LRU consistently outperformed FiFo which theoretically makes sense in the real world if we assume that the things we use most 
    frequently are going to continue to be used, which by and large is a fair assumption. The only fifo mode that outperformed lru 
    for write-alloc write-back was the fully associative version.
	    Now fully associative caches performed the best overall within their class of configuration, but a close second was a 128 set to 
    64 block set associative cache cache, closely followed by 512:16, 256:32 and 56:128. This suggests that the second best 
    performance cache type includes a number of sets at least double the number of blocks in the set, so a little closer on the 
    spectrum to a direct mapped cache than a fully associative one. Obviously in an ideal world we would prefer the fully associative cache, 
    but due to the hardware requirements we would want something in this approximate range of set associativity. 
        In conclusion our best result was with 128 sets, 64 blocks, 4 bytes, write-allocate, write-back, lru and an overall size of 32kb.  