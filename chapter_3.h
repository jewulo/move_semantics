#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cassert>
#include <random>

namespace chapter_3
{
	namespace sec_3_1
	{
		class customer_t {
		private:
			std::string			m_name;		// name of the customer
			std::vector<int>	m_values;	// some values of the customer
		public:
			customer_t(const std::string& n)
				: m_name(n)
			{
				assert(!m_name.empty());
			}

			std::string get_name() const
			{
				return m_name;
			}

			void add_value(int val)
			{
				m_values.push_back(val);
			}

			friend std::ostream& operator<< (std::ostream& strm, const customer_t& cust)
			{
				strm << '[' << cust.m_name << ": ";
				for (int val : cust.m_values) {
					strm << val << ' ';
				}
				strm << ']';
				return strm;
			}
		};
		
#include <utility>	// for std::move
		void run()
		{
			// create a customer with some initial values:
			customer_t cust { "Wolfgang Amadeus Mozart" };
			for (int val : {0, 8, 15}) {
				cust.add_value(val);
			}
			std::cout << "customer: " << cust << '\n';	// print value of initialised cust

			// insert the customer twice into a collection of customers:
			std::vector<customer_t> customers;
			customers.push_back(cust);					// copy into the vector
			customers.push_back(std::move(cust));		// move into the vector
			std::cout << "customer: " << cust << '\n';	// print value of moved-from cust

			// print all customers in the collection:
			std::cout << "customers:\n";
			for (const customer_t& cust : customers) {
				std::cout << "customer: " << cust << '\n';
			}
		}

		// http://www.cppmove.com/code/basics/customer2.cpp.html

		customer_t create_customer()
		{
			// initialize random-number generator for values from 0 to 999
			static std::default_random_engine rnd_engine;
			static std::uniform_int_distribution<int> distr{0, 999};

			// create customers with unique name and 10 random values
			static int no{ 0 };
			customer_t cust{ " my test customer " + std::to_string(++no) };
			for (int i = 0; i < 10; ++i) {
				cust.add_value(distr(rnd_engine));
			}

			return cust;	// usues move semantics if not optimized away
		}

		void run_2()
		{
			std::vector<customer_t> customers;
			for (int num = 0; num < 8; ++num) {
				customers.push_back(create_customer());
			}

			std::cout << "customers:\n";
			for (const customer_t& cust : customers) {
				std::cout << "customer: " << cust << '\n';
			}
		}
	}
	namespace sec_3_2 {
	}
}
