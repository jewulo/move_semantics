#pragma once

#include <string>
#include <vector>

namespace chapter_1
{
    namespace sec_1_1_1
    {
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

        void run(void)
        {
            std::vector<std::string> v;         // create empty vector of strings
            v = create_and_insert();            // assign returned vector of strings (move is implicit in C++20)
        }
    }
}
