#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void printResult(int loads, int stores, int ldr_hits, int ldr_misses, int str_hits, int str_misses, int total) {
    //edit later to take an array or vector... parse structure and then
    //have the lines below
    std::cout << "Total loads: " << loads << "\n";
    std::cout << "Total stores: " << stores << "\n";
    std::cout << "Load hits: " << ldr_hits << "\n";
    std::cout << "Load misses: " << ldr_misses << "\n";
    std::cout << "Store hits: " << str_hits << "\n"; 
    std::cout << "Store misses: " << str_misses << "\n"; 
    std::cout << "Total cycles: " << total;
}

int * parseTraces(std::string trace_name) {
    int counts[7]; //store information in array
    std::ifstream trace;
    trace.open(trace_name);
    //parse to find number of loads and stores first... get that working
    std::string line;
    std::string lOrS;
    while(std::getline(trace, line)) {
        //something like this below, finish later
        std::stringstream ss(line);
        getline(ss, lOrS, ' ');
        if(lOrS.compare("l") == 0) ? loads++ : stores++; 

        int a, b;

    }

    trace.close();
}

/*
Main function of program --> will manage input of command line arguments as well as initalizing the simulation
    Inputs:
        number of sets in the cache (a positive power-of-2)
        number of blocks in each set (a positive power-of-2)
        number of bytes in each block (a positive power-of-2, at least 4)
        write-allocate or no-write-allocate
        write-through or write-back
        lru (least-recently-used) or fifo evictions
*/
int main(int argc, char *argv[]){ 
    //convert entier input to more easily usable strings
    std::vector<std::string> params(argv, argv+argc);
    //convert first three inputs to ints
    int sets = std::stoi(params[1]);
    int blocks = std::stoi(params[2]);
    int bytes = std::stoi(params[3]);
    //use the next three inputs as bools 
    bool write_alloc = (params[4].compare("write-allocate") == 0) ? true : false; 
    bool write_thr = (params[5].compare("write-through") == 0) ? true : false; 
    bool lru = (params[6].compare("lru")) ? true : false;
    std::string trace_name;
    std::cin >> trace_name; //reading file name
    //std::ifstream trace;
    //trace.open(trace_name);
    //test code
    //std::cout << trace.rdbuf();
    //std::cout << sets << blocks << bytes << write_alloc << write_thr << lru << std::endl;
    //proper result printing format
    parseTraces(trace_name); //maybe put this directly into printResult
    printResult();
    //trace.close();
    return 0;
}