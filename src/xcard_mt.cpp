#include <iostream>
#include <thread>
#include <thread>
#include <vector>
#include <fstream>
#include <cmath>
#include <mutex>
#include "../include/xcard.hpp"

std::mutex mutex;

void xcardMT::runnerLS(long long int start, long long int end, std::vector<std::string> *result)
{

    int countdown = 10;

    for (long long int i = start; i < end; i++)
    {

        std::string card = std::to_string(i);

        if (LuhnCheck(card) == true)
        {
            result->push_back(card);
            if (this->debug){
                mutex.lock();
                std::cout << "[VALID] " << card << std::endl;
                mutex.unlock();
            }
        }
        else{
            result->push_back(std::to_string(false));
        }

        countdown--;

        if (countdown == 0)
        {
            //reset
            countdown = 10;
        }
    }
}

void xcardMT::runner(std::vector<std::string> cards, std::vector<bool> *result)
{

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
    
    if (threads > 0){
    this->max_threads = threads;
    }
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

    //if max_threads < 0 set to no of cpu of system.
    if (this->max_threads <= 0){
        this->max_threads = std::thread::hardware_concurrency();
    }

    std::thread threads[this->max_threads];
    std::vector<bool> results[this->max_threads];

    if (this->debug)
    {
        std::cout << "[DEBUG] TotalLines=" << line_counter << std::endl;
        std::cout << "[DEBUG] max_threads=" << this->max_threads << std::endl;
    }

    //calculate how many for each thread!
    int per_thread = std::floor(line_counter / this->max_threads);

    if (this->debug){
        std::cout << "[DEBUG] PerThread=" << per_thread << std::endl;
    }

    //loop and dispatch to threads.
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
        if (this->debug){
            std::cout << "[DEBUG] Thread " << i << " joined!" << std::endl;
        }
        final_result.insert(final_result.end(), results[i].begin(), results[i].end());
    }

    //leftover cards
    if (this->debug){
        std::cout << "[DEBUG] Leftover=" << this->max_threads * per_thread << std::endl;
    }

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
    if (this->max_threads <= 0){
        this->max_threads = std::thread::hardware_concurrency();
    }
    std::thread threads[this->max_threads];
    std::vector<std::string> results[this->max_threads];
    std::vector<std::string> final_result;

    long int per_thread = std::floor((max_s - card_b) / this->max_threads);

    if (this->debug){
        std::cout << "[DEBUG] max_threads=" << this->max_threads << std::endl;
        std::cout << "[DEBUG] PerThread=" << per_thread << std::endl;
    }

    for (int i = 0; i < this->max_threads && card_b < max_s; i++)
    {
        threads[i] = std::thread(&xcardMT::runnerLS, this, card_b, (card_b + per_thread), &results[i]);
        card_b += per_thread;
    }

    for (int i = 0; i < this->max_threads; i++)
    {
        if (this->debug){
            std::cout << "[DEBUG] Thread " << i << " joined!" << std::endl;
        }
        threads[i].join();
        final_result.insert(final_result.end(), results[i].begin(), results[i].end());
    }

    long long int leftovers = this->max_threads * per_thread - (card_b - max_s) - final_result.size();
    
    if (this->debug){
    std::cout << "[DEBUG] Leftovers="<< leftovers << std::endl << "[DEBUG] At=" << (max_s - leftovers) << std::endl;
    }

    for (long long int i = max_s - leftovers; i < leftovers; i++)
    {
        if (LuhnCheck(std::to_string(i)) == true){
            final_result.push_back(std::to_string(i));
        }
    }


    return final_result;
}

