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

			return cust;	// return value uses move semantics if not optimized away
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

	namespace sec_3_2
	{
		class customer_t
		{
		private:
			std::string			m_name;		// name of the customer
			std::vector<int>	m_values;	// some values of the customer

		public:
			// constructor:
			customer_t(const std::string& n)
				: m_name{ n }
			{
				assert(!m_name.empty());
				std::cout << "CONSTRUCT " << m_name << '\n';
			}

			std::string get_name() const
			{
				return m_name;
			}

			void add_value(int val)
			{				
				m_values.push_back(val);
			}

			friend std::ostream& operator<<(std::ostream& strm, const customer_t cust)
			{
				strm << '[' << cust.m_name << ": ";
				for (int val : cust.m_values) {
					strm << val << ' ';
				}
				strm << ']';

				return strm;
			}

			// copy constructor (copy all members):
			customer_t(const customer_t& cust)
				: m_name{cust.m_name}, m_values {cust.m_values}
			{
				std::cout << "COPY " << cust.m_name << '\n';
			}

			// move constructor (move all members):
			customer_t(customer_t&& cust)		// noexcept declaraton missing
				: m_name{std::move(cust.m_name)}, m_values{std::move(cust.m_values)}
			{
				std::cout << "MOVE " << m_name << '\n';
			}

			// copy assignment (assign all members):
			customer_t& operator= (const customer_t& cust)
			{
				std::cout << "COPY ASSIGN " << cust.m_name << '\n';
				m_name = cust.m_name;
				m_values = cust.m_values;
				return *this;
			}

			// move assignment (move all members):
			customer_t& operator= (customer_t&& cust)
			{
				std::cout << "MOVE ASSIGN " << cust.m_name << '\n';
				m_name = std::move(cust.m_name);
				m_values = std::move(cust.m_values);
				/*
				* Note: Because satd::move is just simply a static cast to T&&
				* i.e. std::move(T t) is equivalent to static_cast<T&&>(t),
				* we can right std::move(cust.m_name) as std::move(cust).m_name
				* 
				* The following are all equivalent
				* std::move(cust.m_name)
				* std::move(cust).m_name
				* static_cast<customer_t&&>(cust.m_name)
				* static_cast<customer_t&&>(cust).m_name
				*/
				return *this;
			}

		};

		void run(void)
		{
			// Dealing with Move Assignments of Objects to Themselves
			customer_t c{ "GNU's Not Unix" };
			c = std::move(c);

			std::vector<customer_t> coll;
			coll.emplace_back("GNU's Not Unix");	// coll has one element
			coll[0] = std::move(coll.back());		// move assigns the only element to itself
		}

		void run_2(void)
		{
			std::vector<customer_t> coll;
			for (int i{ 0 }; i < 12; ++i) {
				coll.push_back(customer_t{"TestCustomer " + std::to_string(i-5)});
			}

			std::cout << "---- sort():\n";
			std::sort(coll.begin(), coll.end(),
				[](const customer_t& c1, const customer_t& c2) {
					return c1.get_name() < c2.get_name();
				});
		}
	}

	// By Default, We Have Copying and Moving
	namespace sec_3_3_2
	{
		class person_t
		{
		private:
			std::string m_fname;
			std::string m_lname;
		public:
			person_t(std::string fname, std::string lname)
				:m_fname(fname), m_lname(lname)
			{}
			// NO copy constructor/assignment declared
			// NO move constructor/assignment declared
			// NO destructor
		};

		void run()
		{
			std::vector<person_t> coll;
			person_t p{ "Tina", "Fox" };
			coll.push_back(p);				// OK, copies p
			coll.push_back(std::move(p));	// OK, moves p
		}
	}

	// Declared Copying Disables Moving (Fallback Enabled)
	namespace sec_3_3_3
	{
		class person_t
		{
		private:
			std::string m_fname;
			std::string m_lname;
		public:
			person_t(std::string fname, std::string lname)
				:m_fname(fname), m_lname(lname)
			{}

			// copy constructor/assignment explicitly declared
			person_t(const person_t&) = default;
			person_t& operator=(const person_t&) = default;

			// NO move constructor/assignment declared
			// NO destructor
		};

		void run()
		{
			std::vector<person_t> coll;
			person_t p{ "Tina", "Fox" };
			coll.push_back(p);				// OK, copies p
			coll.push_back(std::move(p));	// OK, copies p due to fallback
		}
	}
	// Declared Moving Disables Copying
	namespace sec_3_3_4
	{
		class person_t
		{
		private:
			std::string m_fname;
			std::string m_lname;
		public:
			person_t(std::string fname, std::string lname)
				:m_fname(fname), m_lname(lname)
			{}

			// NO copy constructor declared

			// move constructor/assignment declared
			person_t(person_t&&) = default;
			person_t& operator=(person_t&&) = default;

			// NO destructor
		};

		void run()
		{
			std::vector<person_t> coll;
			person_t p{ "Tina", "Fox" };
			//coll.push_back(p);						// ERROR, coping disabled
			coll.push_back(std::move(p));				// OK, moves p
			coll.push_back(person_t{ "Ben","Cook" });	// OK, moves temporary into coll
		}
	}

	// Deleting Move Makes No Sense
	namespace sec_3_3_5
	{
		class person_t
		{
		private:
			std::string m_fname;
			std::string m_lname;
		public:
			person_t(std::string fname, std::string lname)
				:m_fname(fname), m_lname(lname)
			{}

			// NO copy constructor declared

			// move constructor/assignment deleted (declared as deleted)
			// therefore no default copy constructor/assignment
			person_t(person_t&&) = delete;
			person_t& operator=(person_t&&) = delete;
			
			// NO destructor
		};

		void run()
		{
			std::vector<person_t> coll;
			person_t p{ "Tina", "Fox" };
			//coll.push_back(p);						// ERROR, copying disabled
			//coll.push_back(std::move(p));				// ERROR, moving disabled
			//coll.push_back(person_t{ "Ben","Cook" });	// ERROR, no moving temporaries
		}
	}

	// Explicitly Deleting Move And Enabling Copy Semantics Makes No Sense Either
	// General guideline: NEVER DELETE MOVE CONSTRUCTOR/MOVE ASSIGNMENT
	namespace sec_3_3_5b
	{
		class person_t
		{
		private:
			std::string m_fname;
			std::string m_lname;
		public:
			person_t(std::string fname, std::string lname)
				:m_fname(fname), m_lname(lname)
			{}

			// copy constructor/assignment explicitly declared
			person_t(const person_t&) = default;
			person_t& operator=(const person_t&) = default;

			// move constructor/assignment deleted (declared as deleted)
			person_t(person_t&&) = delete;
			person_t& operator=(person_t&&) = delete;

			// NO destructor
		};

		void run()
		{
			std::vector<person_t> coll;
			person_t p{ "Tina", "Fox" };
			//coll.push_back(p);						// ERROR, copying disabled
			//coll.push_back(std::move(p));				// ERROR, moving disabled
			//coll.push_back(person_t{ "Ben","Cook" });	// ERROR, no moving temporaries
		}
	}

	// Disabling Move with Enabled Copy Semantics
	// This is the default position before C++11
	namespace sec_3_3_6
	{
		class person_t
		{
		private:
			std::string m_fname;
			std::string m_lname;
		public:
			person_t(std::string fname, std::string lname)
				:m_fname(fname), m_lname(lname)
			{}

			// copy constructor/assignment explicitly declared
			person_t(const person_t&) = default;
			person_t& operator=(const person_t&) = default;

			// NO explicit move constructor/assignment declared

			// NO destructor
		};
		class customer_t
		{
		private:
			std::string			m_name;		// name of the customer
			std::vector<int>	m_values;	// some values of the customer

		public:
			// constructor:
			customer_t(const std::string& n)
				: m_name{ n }
			{
				assert(!m_name.empty());
				std::cout << "CONSTRUCT " << m_name << '\n';
			}

			std::string get_name() const
			{
				return m_name;
			}

			void add_value(int val)
			{
				m_values.push_back(val);
			}

			friend std::ostream& operator<<(std::ostream& strm, const customer_t cust)
			{
				strm << '[' << cust.m_name << ": ";
				for (int val : cust.m_values) {
					strm << val << ' ';
				}
				strm << ']';

				return strm;
			}

			// copy constructor (copy all members):
			customer_t(const customer_t& cust) = default;	// disable move semantics
			// copy assignment (assign all members):
			customer_t& operator= (const customer_t& cust) = default; // disable move semantics
			// delete move constructor (move all members):
			//customer_t(customer_t&& cust) = delete;	// noexcept declaraton missing
			//// delete move assignment (assign all members):
			//customer_t& operator= (customer_t&& cust) = delete;	// noexcept declaraton missing

		//	customer_t(const customer_t& cust)
			//	: m_name{ cust.m_name }, m_values{ cust.m_values }
			//{
			//	std::cout << "COPY " << cust.m_name << '\n';
			//}

			//// move constructor (move all members):
			//customer_t(customer_t&& cust)		// noexcept declaraton missing
			//	: m_name{ std::move(cust.m_name) }, m_values{ std::move(cust.m_values) }
			//{
			//	std::cout << "MOVE " << m_name << '\n';
			//}

			//// copy assignment (assign all members):
			//customer_t& operator= (const customer_t& cust)
			//{
			//	std::cout << "COPY ASSIGN " << cust.m_name << '\n';
			//	m_name = cust.m_name;
			//	m_values = cust.m_values;
			//	return *this;
			//}

			//// move assignment (move all members):
			//customer_t& operator= (customer_t&& cust)
			//{
			//	std::cout << "MOVE ASSIGN " << cust.m_name << '\n';
			//	m_name = std::move(cust.m_name);
			//	m_values = std::move(cust.m_values);
			//	/*
			//	* Note: Because satd::move is just simply a static cast to T&&
			//	* i.e. std::move(T t) is equivalent to static_cast<T&&>(t),
			//	* we can right std::move(cust.m_name) as std::move(cust).m_name
			//	*
			//	* The following are all equivalent
			//	* std::move(cust.m_name)
			//	* std::move(cust).m_name
			//	* static_cast<customer_t&&>(cust.m_name)
			//	* static_cast<customer_t&&>(cust).m_name
			//	*/
			//	return *this;
			//}
		};

		customer_t create_customer()
		{
			// initialize random-number generator for values from 0 to 999
			static std::default_random_engine rnd_engine;
			static std::uniform_int_distribution<int> distr{ 0, 999 };

			// create customers with unique name and 10 random values
			static int no{ 0 };
			customer_t cust{ " my test customer " + std::to_string(++no) };
			for (int i = 0; i < 10; ++i) {
				cust.add_value(distr(rnd_engine));
			}

			return cust;	// return value uses copy semantics because move is deleted
		}

		void run()
		{
			std::vector<customer_t> customers;
			customer_t cust{"customer one"};
			customers.push_back(cust);

			customers.push_back(create_customer());						// OK, falls back to coping
			customers.push_back(std::move(customers[0]));				// OK, falls back to coping
		}
	}

	// Moving for Members with Disabled Move Semantics
	namespace sec_3_3_7
	{
		class customer_t
		{
		private:
			std::string m_fname;
			std::string m_lname;
		public:
			customer_t(std::string fname, std::string lname)
				:m_fname(fname), m_lname(lname)
			{}

			// copy constructor/assignment explicitly declared
			customer_t(const customer_t&) = default;
			customer_t& operator=(const customer_t&) = default;

			// move constructor/assignment deleted (declared as deleted)
			customer_t(customer_t&&) = delete;
			customer_t& operator=(customer_t&&) = delete;

			// NO destructor
		};

		class invoice_t
		{
		private:
			std::string	m_id;
			customer_t	m_cust;
		public:
			// using defaults
			invoice_t(std::string id, customer_t p)
				: m_id{id}, m_cust{p}
			{}
			// the generated move constructor will move m_id string but copy the
			//  person_t cust because move semantics is disabled for person_t
		};

		void run()
		{
			invoice_t inv{ std::string{"inv01" }, customer_t{ "Tina", "Fox" } };
			invoice_t inv2{ std::move(inv) };	// OK, moves id, copies invoice
		}
	}

	// Exact Rules for Generated Special Member Functions
	namespace sec_3_3_8
	{
		class mytype_t {};
		class base_t {};
		class my_class_t : public base_t
		{
		private:
			mytype_t value;
		};

	}
}
