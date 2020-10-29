#include <iostream>
#include <vector>
#include <threads.h>
#include "../include/xcard.hpp"

int main()
{

    xcard xc("456", true);

    std::cout << "xcard version: " << xc.VERSION << std::endl;

    std::cout << "[DEMO] Generate 10 random valid cards with prefix 4" << std::endl;

    for (int i = 0; i < 10; i++)
    {
        std::cout << xc.generateCard() << std::endl;
    }

    std::cout << "enter prefix for linear search!" << std::endl;
    std::string user_input;
    std::cin >> user_input;

    xcardMT xcmt(user_input, -1, true);
    std::vector<std::string> cards;

    cards = xcmt.LinearSearch();


    for (int i = 0; i < cards.size(); i++)
    {
        std::cout << i << " " << cards[i] << std::endl;
    }

    return 0;
}
