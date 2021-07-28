#include <iostream>
#include <vector>
#include <assert.h>
#include<thread>
#include "../include/xcard.hpp"

int main()
{

    std::string prefix_input;
    std::string _dbg_option;
    int n_threads;

    std::cout << "Enter Prefix (Upto 15 digits): ";
    std::cin >> prefix_input;
    assert(prefix_input.length() < 16);
    
    std::cout << "Enable Debug Messages [Y/N]: ";
    std::cin >> _dbg_option;


    std::cout << "No of threads to use (Max: " << std::thread::hardware_concurrency() << "): ";
    std::cin >> n_threads;
    assert(n_threads <= std::thread::hardware_concurrency());

    xcardMT xcmt(prefix_input, n_threads, _dbg_option == "Y" ? true : false);
    xcmt.LinearSearch();

    return 0;

}