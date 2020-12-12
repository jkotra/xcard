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


    std::vector<bool> results[this->max_threads];

    if (this->debug)
    {
        std::cout << "[DEBUG] TotalLines=" << line_counter << std::endl;
        std::cout << "[DEBUG] max_threads=" << this->max_threads << std::endl;
    }


    while(true){

        //set max_threads and per_thread;
        long int per_thread = line_counter / this->max_threads;

        if (per_thread <= this->max_threads){
            this->max_threads = 1;
            per_thread = line_counter;
        }

        std::thread threads[this->max_threads];
        
        //debug
        //std::cout << "per_thread= " << per_thread << " max_threads=" << this->max_threads << std::endl;

        for (size_t thread_id = 0; thread_id < this->max_threads; thread_id++)
        {
            //declare storage
            std::vector<std::string> card_storage;

            int pt_counter = 0;

            //get lines and store in storage (check for eof)
        while (pt_counter < per_thread && !fp.eof())
        {
            getline(fp, buf);
            card_storage.push_back(buf);
            pt_counter++;
        }

        //dispatch
        threads[thread_id] = std::thread(&xcardMT::runner, this, card_storage, &results[thread_id]);
        line_counter -= pt_counter;

        }

        //join and add result to final_result
        for (size_t i = 0; i < this->max_threads; i++)
        {
            threads[i].join();
            final_result.insert(final_result.end(), results[i].begin(), results[i].end());
        }
        

        //if not eof - continue
        if(fp.eof()){
            break;
        }else{
            continue;
        }


    } //end while

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

    std::vector<std::string> results[this->max_threads];
    std::vector<std::string> final_result;

    long int remaining = max_s - card_b;

    while (true){
        long int per_thread = remaining / this->max_threads;

        if (per_thread <= this->max_threads){
            per_thread = remaining;
            this->max_threads = 1;
        }

        std::thread threads[this->max_threads];

        //dispatch
        for (int i = 0; i < this->max_threads && card_b < max_s; i++)
        {
            card_b += per_thread;
            threads[i] = std::thread(&xcardMT::runnerLS, this, card_b-per_thread, card_b, &results[i]);
        }
        

        //join
        for (int i = 0; i < this->max_threads; i++)
        {
            threads[i].join();
            final_result.insert(final_result.end(), results[i].begin(), results[i].end());
        }
        
        remaining -= per_thread * this->max_threads;
        if (remaining == 0){
            break;
        }

    }

    return final_result;
}

