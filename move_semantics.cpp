// move_semantics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// C++ Move Semantics : The Complete Guide, Nicolai M. Josuttis. 
using namespace std;

#include <iostream>
#include <vector>

#include "chapter_2.h"
#include "chapter_3.h"

std::vector<std::string> create_and_insert()
{
    std::vector<std::string> coll;      // create vector of strings
    coll.reserve(3);                    // reserve memory for 3 elements
    std::string s = "data";             // create string object

    coll.push_back(s);                  // insert string object
    coll.push_back(s+s);                // insert temporary string (move is implicit in C++20 for temporaries)
    coll.push_back(std::move(s));       // move string object

    return coll;                        // return vector of strings
}

void chapter_2_run()
{
    chapter_2::sec_2_1_1::run();
    chapter_2::sec_2_1_2::run();
    chapter_2::sec_2_2::run();
    chapter_2::sec_2_3_1::run();
    chapter_2::sec_2_3_2::run();
    chapter_2::sec_2_3_3::run();
    chapter_2::sec_2_4::run();
    chapter_2::sec_2_5::run();
}

void chapter_3_run()
{
    chapter_3::sec_3_1::run();
    chapter_3::sec_3_1::run_2();
}

int main()
{
    std::vector<std::string> v;         // create empty vector of strings
    v = create_and_insert();            // assign returned vector of strings (move is implicit in C++20)

    //chapter_2_run();
    chapter_3_run();

    cin.get();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
