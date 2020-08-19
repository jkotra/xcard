#include <iostream>
#include <vector>
#include <threads.h>
#include <fstream>
#include "../include/main.hpp"

int main()
{

    //initialize
    xcard xc("4", false);

    std::cout << "xcard version: " << xc.VERSION << std::endl;

    std::cout << "Generate 10 random valid cards with prefix 4" << std::endl;

    for (int i = 0; i < 10; i++)
    {
        std::cout << xc.generateCard() << std::endl;
    }

    return 0;
}
