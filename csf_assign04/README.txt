CONTRIBUTIONS

TODO: write a brief summary of how each team member contributed to
the project.

We both met in brody to work on the entierty of this project. 

REPORT

TODO: add your report according to the instructions in the
"Experiments and analysis" section of the assignment description.

In your README.txt, write a brief report which

    Indicates the amount of time that your parsort program took so sort the test data for each threshold value, and
    States a reasonable explanation for why you saw the times you did

For #2, think about how the computation unfolds, and in particular, what parts of the computation are being executed 
in different processes, and thus which parts of the computation could be scheduled by the OS kernel in parallel on different
CPU cores. We don’t expect a completely rigorous and in-depth explanation, but we would like you to give an intuitive 
explanation for the results that you observed.


Threshold: 2097152 1048576 524288 262144 131072 65536 32768 16384
Time:      .409s    .245s  .165s  .127s  .136s  .134s .138s .149s


It makes sence that as we decrease the threshold we limit the amount of time we spend on large sections of slower sorting. 
The more time we spend breaking into forked child proccesses to break the data into a tree structure the more efficent we are.
The fact that there appears to be a diminishing return as we continue to decrease the threshold (and even an increase in time needed
as we continue to decrease the threshold) suggests that at a certain point, the overhead of creating and managing these forks becomes
not worth it. So by using a sort threshold of 65536 on 16 megabytes of data we are able to maximize the bennifits of the parallel 
merge sort while minimizing the overhead of forking on too small an amount of data. This makes sence because at a certain point the 
inefficency of the qsort (relitive to merge) at a low enough N. 

Moral of the story. It is efficent to use a fork based merge sort, but at a certain point, the cost of swapping processes overtakes the bennifits
of spllting in input in half. This point is the correct threshold for calling a regular qsort on the array segment

Test run with threshold 2097152

real    0m0.409s
user    0m0.392s
sys     0m0.014s
Test run with threshold 1048576

real    0m0.245s
user    0m0.396s
sys     0m0.039s
Test run with threshold 524288

real    0m0.165s
user    0m0.422s
sys     0m0.044s
Test run with threshold 262144

real    0m0.127s
user    0m0.531s
sys     0m0.064s
Test run with threshold 131072

real    0m0.136s
user    0m0.510s
sys     0m0.076s
Test run with threshold 65536

real    0m0.134s
user    0m0.507s
sys     0m0.110s
Test run with threshold 32768

real    0m0.138s
user    0m0.518s
sys     0m0.136s
Test run with threshold 16384

real    0m0.149s
user    0m0.519s
sys     0m0.195s
