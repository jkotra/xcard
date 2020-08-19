#include<iostream>
#include<fstream>
#include <gtest/gtest.h>

#include "../include/main.hpp"

TEST(xcard, random_card){
    
    xcard xc("4", false);
    EXPECT_EQ(true, xc.validateCard(xc.generateCard()));

}

TEST(xcard, validate_card){

    xcard xc("4", false);
    EXPECT_EQ(xc.validateCard("4593764338713585"), true);
    EXPECT_EQ(xc.validateCard("4593764338713580"), false);

}

TEST(xcard, validate_card_from_file_mt){

    xcardMT xcmt("4", 6, false);
    std::ifstream f;
    f.open("cc.txt");

    std::vector<bool> res = xcmt.validateFromFile(f);
    std::vector<bool> exp = { true, true, true, true };

    EXPECT_EQ(res, exp);

}

TEST(xcard, linear_search_mt){

    xcardMT xcmt("5460430594063", 1, false);

    std::vector<std::string> res = xcmt.LinearSearch();
    std::vector<std::string>::iterator it = std::find(res.begin(), res.end(), "5460430594063308" );
    EXPECT_EQ(true, it != res.end());

}

#include <gtest/gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}