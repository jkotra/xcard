#include <iostream>
#include <thread>
#include <thread>
#include <vector>
#include <fstream>
#include <cmath>
#include "../include/xcard.hpp"

void xcardMT::runnerLS(long long int start, long long int end, std::vector<std::string> *result)
{

    int countdown = 10;

    for (long long int i = start; i < end; i++)
    {
        if (LuhnCheck(std::to_string(i)) == true)
        {
            result->push_back(std::to_string(i));
            i += countdown;
            continue;
        }

        countdown--;

        if (countdown == 0)
        {
            countdown = 10;
        }
    }
}

void xcardMT::runner(std::vector<std::string> cards, std::vector<bool> *result)
{

    if (this->debug)
    {
        std::cout << "cards.size()=" << cards.size() << std::endl;
    }

    for (std::string c : cards)
    {

        if (LuhnCheck(c) == true)
        {
            result->push_back(true);
        }
        else
        {
            result->push_back(false);
        }
    }
}

xcardMT::xcardMT(std::string prefix, int threads, int debug)
{

    this->prefix = prefix;
    this->debug = debug;
    this->max_threads = threads;
}

std::vector<bool> xcardMT::validateFromFile(std::ifstream &fp)
{

    //declare buf to store string from file!
    std::string buf;

    //to join indv. results from threads.
    std::vector<bool> final_result;

    //counter vars
    int line_counter = 0;
    int temp_counter = 0;

    //initial line counting
    while (getline(fp, buf))
    {
        line_counter++;
    }

    //clear and reset file
    fp.clear();
    fp.seekg(0, std::ios::beg);

    //init. threads and results vector.
    std::thread threads[this->max_threads];
    std::vector<bool> results[this->max_threads];

    if (this->debug)
    {
        std::cout << "[DEBUG]TotalLines=" << line_counter << std::endl;
        std::cout << "[DEBUG]max_threads=" << this->max_threads << std::endl;
    }

    //calculate how many for each thread!
    int per_thread = std::ceil(line_counter / this->max_threads);

    //loop and send threads.
    for (int i = 0; i < this->max_threads && !fp.eof(); i++)
    {
        std::vector<std::string> cards;
        while (temp_counter < per_thread)
        {
            getline(fp, buf);
            cards.push_back(buf);
            temp_counter++;
        }

        threads[i] = std::thread(&xcardMT::runner, this, cards, &results[i]);
        temp_counter = 0;
    }

    //join and combine results
    for (int i = 0; i < this->max_threads; i++)
    {
        threads[i].join();
        final_result.insert(final_result.end(), results[i].begin(), results[i].end());
    }

    //leftover cards
    for (int i = this->max_threads * per_thread; i < line_counter; i++)
    {
        getline(fp, buf);
        if (LuhnCheck(buf) == true)
        {
            final_result.push_back(true);
        }
        else
        {
            final_result.push_back(false);
        }
    }

    if (this->debug)
    {
        std::cout << "totalProcessed=" << final_result.size() << std::endl;
    }

    //return result
    return final_result;
}
std::vector<std::string> xcardMT::LinearSearch()
{

    std::string cardbuf;

    if (std::stoll(this->prefix) > -1)
    {
        cardbuf.insert(0, this->prefix);
    }

    int initial_length = cardbuf.length();
    int zreq = 16 - initial_length;

    std::string max_search = cardbuf;

    for (int i = 0; i < zreq; i++)
    {
        max_search.insert(max_search.length(), "9");
        cardbuf.insert(cardbuf.length(), "0");
    }

    //convert to long long int
    long long int max_s = std::stoll(max_search);
    long long int card_b = std::stoll(cardbuf);

    //init. threads and results vector.
    std::thread threads[this->max_threads];
    std::vector<std::string> results[this->max_threads];
    std::vector<std::string> final_result;

    long int per_thread = std::ceil((max_s - card_b) / this->max_threads);

    for (int i = 0; i < this->max_threads && card_b < max_s; i++)
    {
        threads[i] = std::thread(&xcardMT::runnerLS, this, card_b, (card_b + per_thread), &results[i]);
        card_b = card_b + per_thread;
    }

    for (int i = 0; i < this->max_threads; i++)
    {
        threads[i].join();
        final_result.insert(final_result.end(), results[i].begin(), results[i].end());
    }

    long int leftovers = this->max_threads*per_thread - (card_b - max_s);
    long long int leftover_at = (card_b - max_s) - leftovers;

    for (long long int i = leftover_at; i < leftovers; i++)
    {
        if (LuhnCheck(std::to_string(i)) == true){
            final_result.push_back(std::to_string(i));
        }
    }
    

    return final_result;
}

