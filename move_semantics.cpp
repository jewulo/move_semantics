// move_semantics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// C++ Move Semantics : The Complete Guide, Nicolai M. Josuttis. 
using namespace std;

#include <iostream>
#include <vector>

#include "chapter_1.h"
#include "chapter_2.h"
#include "chapter_3.h"
#include "chapter_4.h"
#include "chapter_5.h"
#include "chapter_6.h"
#include "chapter_7.h"
#include "chapter_8.h"
#include "chapter_9.h"
#include "chapter_10.h"
#include "chapter_11.h"
#include "chapter_12.h"
#include "chapter_13.h"
#include "chapter_14.h"
#include "chapter_15.h"

void chapter_1_run()
{
    chapter_1::sec_1_1_1::run();
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
    chapter_3::sec_3_2::run();
    chapter_3::sec_3_2::run_2();
    chapter_3::sec_3_3_2::run();
    chapter_3::sec_3_3_3::run();
    chapter_3::sec_3_3_4::run();
    chapter_3::sec_3_3_5::run();
    chapter_3::sec_3_3_5b::run();
    chapter_3::sec_3_3_6::run();
    chapter_3::sec_3_3_7::run();
}

void chapter_4_run()
{
    //chapter_4::sec_4_1::run();
    //chapter_4::sec_4_1_1::run();
    //chapter_4::sec_4_1_1::run2();
    //chapter_4::sec_4_2::run();
    //chapter_4::sec_4_3_1::run();
    //chapter_4::sec_4_3_1b::run();
    //chapter_4::sec_4_3_2::run();
    //chapter_4::sec_4_3_3::run();
    //chapter_4::sec_4_3_3::run_2();
    //chapter_4::sec_4_3_3b::run();
    //chapter_4::sec_4_3_3c::run();
    chapter_4::sec_4_3_4::run();
    chapter_4::sec_4_3_6::run();
    chapter_4::sec_4_3_6::run2();
    chapter_4::sec_4_3_6::run3();

    chapter_4::sec_4_4::sec_4_4_1::slicing_problem::run();
    chapter_4::sec_4_4::sec_4_4_1::solve_slicing_problem::run();
    chapter_4::sec_4_4::sec_4_4_2a::run();
    chapter_4::sec_4_4::sec_4_4_2b::run();
}

void chapter_5_run()
{
    //chapter_5::sec_5_1_1::run();
    //chapter_5::sec_5_1_2::run();
    chapter_5::sec_5_1_3::run();
    chapter_5::sec_5_1_3::run2();
    chapter_5::sec_5_1_3::run3();
    chapter_5::sec_5_1_3::run4();
    chapter_5::sec_5_1_3::run5();
    chapter_5::sec_5_2::run();
    chapter_5::sec_5_3::run();
}

void chapter_6_run()
{
    //chapter_6::sec_6_2_1::run();
    //chapter_6::sec_6_2_2::run();
    //chapter_6::sec_6_2_2::run2();
    //chapter_6::sec_6_3_2::run();
    //chapter_6::sec_6_3_2::run();
    //chapter_6::sec_6_3_3::run();
    chapter_6::sec_6_3_3b::run();
    std::cout << std::endl;
}

void chapter_7_run()
{
    chapter_7::sec_7_1_1::run();    
    chapter_7::sec_7_1_2::run();
    chapter_7::sec_7_1_2b::run();
    chapter_7::sec_7_1_2c::run();
    chapter_7::sec_7_1_3::run();
    chapter_7::sec_7_2_2::run();
    chapter_7::sec_7_2_2b::run();
    chapter_7::sec_7_2_2c::run();
    chapter_7::sec_7_2_2d::run();
    chapter_7::sec_7_3a::run();
    chapter_7::sec_7_3b::run();
    std::cout << std::endl;
}

void chapter_8_run()
{
    chapter_8::sec_8_1_1::run();
    chapter_8::sec_8_1_2::run();
    chapter_8::sec_8_1_3::run();
    chapter_8::sec_8_2_2::run();
    chapter_8::sec_8_2_2b::run();
    //chapter_8::sec_8_2_2c::run(); // crash
    //chapter_8::sec_8_2_2d::run(); // crash
    chapter_8::sec_8_3::run();
    chapter_8::sec_8_3_1::run();
    chapter_8::sec_8_4::run();
    chapter_8::sec_8_5::run();
    chapter_8::sec_8_6_1::run();
    chapter_8::sec_8_6_2::run();

    std::cout << std::endl;
}

void chapter_9_run()
{
    chapter_9::sec_9_1_1a::run();
    chapter_9::sec_9_1_1b::run();
    chapter_9::sec_9_1_1c::run();
    chapter_9::sec_9_2a::run();
    chapter_9::sec_9_2b::run();
    chapter_9::sec_9_2c::run();
    chapter_9::sec_9_2_1a::run();
    //chapter_9::sec_9_2_1b::run();   // const version
    //chapter_9::sec_9_2_1c::run();   // volatile version
    chapter_9::sec_9_2_2::run();
    chapter_9::sec_9_2_3::run();
    chapter_9::sec_9_3_1::run();
    //chapter_9::sec_9_3_2::run();
    chapter_9::sec_9_4_1::run();
    chapter_9::sec_9_4_1a::run();
    chapter_9::sec_9_4_1b::run();
    
    chapter_9::sec_9_5::run();
    chapter_9::sec_9_5::run2();

    std::cout << std::endl;
}

void chapter_10_run()
{
    chapter_10::sec_10_1_1::run();
    chapter_10::sec_10_1_2::run();
    chapter_10::sec_10_1_3a::run();
    chapter_10::sec_10_1_3b::run();
    chapter_10::sec_10_1_3c::run();
    chapter_10::sec_10_1_3d::run();
    chapter_10::sec_10_1_3e::run();
    chapter_10::sec_10_2_2::run();
    chapter_10::sec_10_3::run();
    chapter_10::sec_10_3_1a::run(); 
    chapter_10::sec_10_3_1b::run();
    //chapter_10::sec_10_3_2a::run(); // compiler ERROR
    chapter_10::sec_10_3_2b::run();
    chapter_10::sec_10_3_2c::run();
    chapter_10::sec_10_3_2d::run();
    chapter_10::sec_10_3_3a::run();
    chapter_10::sec_10_3_3b::run();

    std::cout << std::endl;
}

void chapter_11_run()
{
    chapter_11::sec_11_1_1::run();
    chapter_11::sec_11_2::run();
    chapter_11::sec_11_2_1::run();
    chapter_11::sec_11_2_2::run();
    chapter_11::sec_11_3_1a::run();
    chapter_11::sec_11_3_1b::run();
    chapter_11::sec_11_3_1c::run();
    chapter_11::sec_11_3_1d::run();
    // !!!! BUG chapter_11::sec_11_3_1e::run(); // !!!! BUG
    //chapter_11::sec_11_3_1f::run();
    chapter_11::sec_11_3_1g::run();
    //chapter_11::sec_11_3_1h::run();
    chapter_11::sec_11_3_1i::run();
    chapter_11::sec_11_4a::run();
    chapter_11::sec_11_4b::run();
    chapter_11::sec_11_4c::run();
    chapter_11::sec_11_5a::run();
    chapter_11::sec_11_5b::run();

    std::cout << std::endl;
}

void chapter_12_run()
{
    chapter_12::sec_12_1::run();
    chapter_12::sec_12_2a::run();
    chapter_12::sec_12_2b::run();
    chapter_12::sec_12_2c::run();
    chapter_12::sec_12_2d::run();
    chapter_12::sec_12_2e::run();
    chapter_12::sec_12_2_1::run();
}

void chapter_13_run()
{
    chapter_13::sec_13_1_1::run();
    chapter_13::sec_13_1_2::run();
    chapter_13::sec_13_1_2b::run();
    chapter_13::sec_13_1_2c::run();
    chapter_13::sec_13_1_3a::run();
    chapter_13::sec_13_1_3b::run();
    chapter_13::sec_13_1_3c::run();
    chapter_13::sec_13_1_4::run();
}

void chapter_14_run()
{
    chapter_14::sec_14_1::run();
    chapter_14::sec_14_2::run();
    chapter_14::sec_14_3_1::run();
    chapter_14::sec_14_3_2::run();
}

void chapter_15_run()
{
    chapter_15::sec_15_1::run();
}

int main()
{
    //chapter_1_run();
    //chapter_2_run();
    //chapter_3_run();
    //chapter_4_run();
    //chapter_5_run();
    //chapter_6_run();
    //chapter_7_run();
    //chapter_8_run();
    //chapter_9_run();
    //chapter_10_run();
    //chapter_11_run();
    //chapter_12_run();
    //chapter_13_run();
    //chapter_14_run();
    chapter_15_run();

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


