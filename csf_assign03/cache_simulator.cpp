#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

/*
TODO:
    Makefile
*/


void printResult(std::vector<int> values) {
    //edit later to take an array or vector... parse structure and then
    //loads, stores, ldr_hits, ldr_misses, str_hits, str_misses, total
    //have the lines below
    std::cout << "Total loads: " << values[0] << "\n";
    std::cout << "Total stores: " << values[1] << "\n";
    std::cout << "Load hits: " << values[2] << "\n";
    std::cout << "Load misses: " << values[3] << "\n";
    std::cout << "Store hits: " << values[4] << "\n"; 
    std::cout << "Store misses: " << values[5] << "\n"; 
    std::cout << "Total cycles: " << values[6];
}

std::vector<int> parseTraces(std::string trace_name) {
    std::vector<int> vec; //store info in vector
    int loads, stores, ldr_hits, ldr_misses, str_hits, str_misses, total; 
    loads = 0;
    stores = 0;
    ldr_hits = 0;
    ldr_misses = 0; 
    str_hits = 0;
    str_misses = 0;
    total = 0;

    std::ifstream trace;
    std::cout << trace_name << std::endl;

    trace.open(trace_name);
    //parse to find number of loads and stores first... get that working
    std::string line;
    std::string lOrS;
    while(std::getline(trace, line)) {
        std::cout << line << std::endl;
        //something like this below, finish later
        std::istringstream ss(line);
        //std::getline(ss, lOrS, ' ');
        ss >> lOrS; //maybe will store first char
        if(lOrS.compare("l") == 0){
            loads = loads + 1;
        } else {
            stores = stores + 1; 
        }

    }

    trace.close();
    
    vec.push_back(loads);
    vec.push_back(stores);
    vec.push_back(ldr_hits);
    vec.push_back(ldr_misses);
    vec.push_back(str_hits);
    vec.push_back(str_misses);
    vec.push_back(total);

    return vec;
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
    if (argc != 7) { //invalid number of arguments
        return 0; //process cant run
    }
    std::string trace_name;
    std::cin >> trace_name; //reading file name
    std::cout << trace_name <<std::endl;
    try
    {
        std::ifstream trace;
        trace.open(trace_name);
        trace.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << 'Bad Trace File\n';
        return 0;
    }
    
    std::string current_exec_name = argv[0]; // Name of the current exec program
    std::vector<std::string> params;
    if (argc > 1) {
        params.assign(argv, argv + argc);
    }
    //convert first three inputs to ints
    std::cout << params[1] << std::endl;
    std::cout << params[2] << std::endl;
    std::cout << params[3] << std::endl;
    int sets = std::stoi(params[1]);
    int blocks = std::stoi(params[2]);
    int bytes = std::stoi(params[3]);
    //use the next three inputs as bools 
    bool write_alloc = (params[4].compare("write-allocate") == 0) ? true : false; 
    bool write_thr = (params[5].compare("write-through") == 0) ? true : false; 
    bool lru = (params[6].compare("lru")) ? true : false;
    //std::ifstream trace;
    //trace.open(trace_name);
    //test code
    //std::cout << trace.rdbuf();
    //std::cout << sets << blocks << bytes << write_alloc << write_thr << lru << std::endl;
    //proper result printing format
    //maybe put this directly into printResult
    printResult(parseTraces(trace_name));
    //trace.close();
    return 1;
}