#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

#include <utility>
#include <cstddef>

// Moving Algorithms and Iterators
namespace chapter_14
{
	// Moving Algorithms
	namespace sec_14_1
	{
		template <typename T>
		void print(const std::string& name, const T& coll)
		{
			std::cout << name << " (" << coll.size() << "elems): ";
			for (const auto& elem : coll) {
				std::cout << " '" << elem << "'";
			}
			std::cout << "\n";
		}

		void run()
		{
			std::cout << "\n";

			std::list<std::string> coll1 { "love", "is", "all", "you", "need" };
			std::vector<std::string> coll2;

			// ensure coll2 has enough elements to overwrite thier values:
			coll2.resize(coll1.size());

			// print out size and values
			print("coll1", coll1);
			print("coll2", coll2);

			// move assign the values from coll1 to coll2
			// not changing any size
			std::move(coll1.begin(), coll1.end(),		// source range
					coll2.begin());						// destination range

			// print out size and values
			print("coll1", coll1);
			print("coll2", coll2);

			// move assign the first three values inside coll2 to the end
			// - not changing any size
			std::move_backward(coll2.begin(), coll2.begin()+3,		// source range
							coll2.end());							// destination range

			// print out size and values
			print("coll1", coll1);
			print("coll2", coll2);
		}
	}

	// Removing Algorithms
	namespace sec_14_2
	{
		// class for email addresses
		// - asserts that each email address has a @
		// - except when in a moved-from state
		class Email
		{
		private:
			std::string m_value;
			bool		m_moved_from{false};
		public:
			Email(const std::string& val)
				: m_value{ val }
			{
				assert(m_value.find('@') != std::string::npos);
			}
			Email(const char* val)	// enable implicit conversions for string literals
				:Email{ std::string(val) }
			{}

			std::string get_value() const {
				assert(!m_moved_from);	// or throw
				return m_value;
			}

			// implement move operations to signal a moved-from state:
			Email(Email&& e) noexcept
				: m_value{ std::move(e.m_value) }, m_moved_from{ e.m_moved_from }
			{
				e.m_moved_from = true;
			}
			Email& operator=(Email&& e) noexcept				
			{
				m_value = std::move(e.m_value);
				m_moved_from = e.m_moved_from;
				e.m_moved_from = true;

				return *this;
			}

			// enable copying:
			Email(const Email&) = default;
			Email& operator=(const Email&) = default;

			// declare output stream operator
			friend std::ostream& operator<< (std::ostream& strm, const Email& e);
		};

		// print out the current state (even if it is a moved-from state)
		std::ostream& operator<< (std::ostream& strm, const Email& e) {
			return strm << (e.m_moved_from ? "MOVED-FROM" : e.m_value);
		}

		void run()
		{
			std::vector<Email> coll { "tom@domain.de", "jill@company.com",
									  "sarah@domain.de", "hana@company.com"};

			std::cout << "-- original elements: \n";
			for (const auto& elem : coll) {
				std::cout << " " << elem << "\n";
			}

			// remove all email address ending with ".de"
			auto new_end = std::remove_if(coll.begin(), coll.end(),
											[](const Email& e) {
												auto&& val = e.get_value();
												return val.size() > 2 &&
													   val.substr(val.size()-3) == ".de";
											});

			// print elements up to the new end:
			std::cout << "-- remaining elements: \n";
			for (auto pos = coll.begin(); pos != new_end; ++pos) {
				std::cout << " " << *pos << "\n";
			}

			std::cout << "-- all elements: \n";
			// print all elements in the constainer:
			for (const auto& elem : coll) {
				std::cout << " " << elem << "\n";
			}
		}
	}

	// Move Iterators
	namespace sec_14_3
	{
		// You can only access an element once using move iterators
	}

	// Move Iterators in Algorithms
	namespace sec_14_3_1
	{
		template <typename T>
		void print(const std::string& name, const T& coll)
		{
			std::cout << name << " (" << coll.size() << " elems): ";
			for (const auto& elem : coll) {
				std::cout << " \"" << elem << "\"";
			}
			std::cout << "\n";
		}

		void process(std::string s)		// gets moved value from rvalues
		{
			std::cout << "- process(" << s << ")\n";
		}

		void run()
		{
			std::cout << "\n";

			std::list<std::string> coll{ "don't", "vote", "for", "liars" };
			print("coll", coll);

			// move away only the elements processed:
			std::for_each(std::make_move_iterator(coll.begin()),
						  std::make_move_iterator(coll.end()),
							[](auto&& elem) {
								if (elem.size() != 4) {
									process(std::move(elem));
								}
							});

			print("coll", coll);
		}
	}

	// Move Iterators in Constructors and Member Functions
	namespace sec_14_3_2
	{
		template <typename T>
		void print(const std::string& name, const T& coll)
		{
			std::cout << name << " (" << coll.size() << " elems): ";
			for (const auto& elem : coll) {
				std::cout << " \"" << elem << "\"";
			}
			std::cout << "\n";
		}

		void run()
		{
			std::cout << "\n";

			std::list<std::string> src{ "don't", "vote", "for", "liars" };

			// move all elements from the list to the vector
			std::vector<std::string> vec {std::make_move_iterator(src.begin()),
										  std::make_move_iterator(src.end())};

			print("src", src);
			print("vec", vec);
		}
	}
}
