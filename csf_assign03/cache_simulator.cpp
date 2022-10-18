#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "cache_simulator.h"
#include "Cache.h"

cache_simulator::~cache_simulator() {
    delete this->cache;
}

void cache_simulator::printResult(std::vector<int> values) {
    //loads, stores, ldr_hits, ldr_misses, str_hits, str_misses, total
    //have the lines below
    std::cout << "Simulation Results: " << "\n";
    std::cout << "Total loads: " << values[0] << "\n";
    std::cout << "Total stores: " << values[1] << "\n";
    std::cout << "Load hits: " << values[2] << "\n";
    std::cout << "Load misses: " << values[3] << "\n";
    std::cout << "Store hits: " << values[4] << "\n"; 
    std::cout << "Store misses: " << values[5] << "\n"; 
    std::cout << "Total cycles: " << values[6];
}

std::vector<int> cache_simulator::parseTraces() {
    std::vector<int> results; //store info in vector
    //initalizes inputs 
    int loads, stores, ldr_hits, ldr_misses, str_hits, str_misses, total; 
    loads = 0;
    stores = 0;
    ldr_hits = 0;
    ldr_misses = 0; 
    str_hits = 0;
    str_misses = 0;
    total = 0;

    //parse to find number of loads and stores first... get that working
    std::string line;
    std::string lOrS;
    bool hORm;
    uint32_t addr;
    std::string add;
    while(std::getline(std::cin, line)) {
        //something like this below, finish later
        std::istringstream ss(line);
        //std::getline(ss, lOrS, ' ');
        ss >> lOrS; //maybe will store first char
        ss >> add;
        //std::cout<<add<<std::endl;
        addr = std::stoi(add.substr(2,8),nullptr,16);
        if(lOrS.compare("l") == 0){
            loads = loads + 1;
        } else {
            stores = stores + 1; 
        }
        hORm = cache->access(addr,lOrS[0]);

    }
    
    //stores results to vector
    results.push_back(loads);
    results.push_back(stores);
    results.push_back(ldr_hits);
    results.push_back(ldr_misses);
    results.push_back(str_hits);
    results.push_back(str_misses);
    results.push_back(total);

    return results;
}
