#include <iostream>
#include <vector>
#include <assert.h>
#include "../include/xcard.hpp"

int main()
{

    std::string prefix_input;
    std::string _dbg_option;

    std::cout << "Enter Prefix (Upto 15 digits): ";
    std::cin >> prefix_input;
    assert(prefix_input.length() < 16);
    
    std::cout << "Enable Debug Messages [Y/N]: ";
    std::cin >> _dbg_option;

    xcard xc(prefix_input, _dbg_option == "Y" ? true : false);
    std::cout << xc.VERSION << std::endl;

    for (size_t i = 0; i < 10; i++)
    {
        std::cout << xc.generateCard() << std::endl; 
    }

    return 0;
}
