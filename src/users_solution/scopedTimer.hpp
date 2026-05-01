#pragma once

#include <chrono>
#include <iostream>

class ScopedTimer{
public:
    ScopedTimer(const char* functionName):functionName(functionName){
        start = std::chrono::high_resolution_clock::now();
    }
    ~ScopedTimer(){
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << functionName << " took " << duration.count() << " milliseconds" << std::endl; 
    }

private:
    const char* functionName;
    std::chrono::high_resolution_clock::time_point start;

};
