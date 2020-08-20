#include <iostream>
#include <random>
#include <string>
#include "../include/xcard.hpp"

xcard::xcard(std::string prefix, int debug)
{
    this->prefix = prefix;
    this->debug = debug;
}

bool xcard::LuhnCheck(const std::string &cardNo)
{

    //Orignally taken from GFG - https://www.geeksforgeeks.org/luhn-algorithm/

    int nDigits = cardNo.length();

    if (nDigits < 1)
    {
        std::cout << "Invalid input. Input must be of lenghth 16. (InputLen=" << nDigits << ")" << std::endl;
        return false;
    }

    int nSum = 0, isSecond = false;
    for (int i = nDigits - 1; i >= 0; i--)
    {

        int d = cardNo[i] - '0';

        if (isSecond == true)
        {
            d = d * 2;
        }

        nSum += d / 10;
        nSum += d % 10;

        isSecond = !isSecond;
    }

    return (nSum % 10 == 0);
}

bool xcard::validateCard(const std::string &cardNo)
{

    return LuhnCheck(cardNo);
}

std::string xcard::generateCard()
{

    std::string cardbuf;

    /* Seed */
    std::random_device rd;

    /* Random number generator */
    std::default_random_engine generator(rd());

    if (std::stoll(this->prefix) > -1)
    {
        cardbuf.insert(0, this->prefix);
    }

    std::uniform_int_distribution<unsigned int> dist(0, 9);

    int initial_length = cardbuf.length();
    int genreq = 16 - initial_length;

    while (true)
    {

        std::string cc = cardbuf;
        
        for (int i = 0; i < genreq; i++)
        {
            cc.insert(cc.length(), std::to_string(dist(generator)));
        }

        if (LuhnCheck(cc) == true)
        {
            return cc;
        }
    }
}