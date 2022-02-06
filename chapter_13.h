#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <cstddef>

// Move-Only Types
namespace chapter_13
{
	// Declaring Move-Only Types
	namespace sec_13_1_1
	{
		class MoveOnly {
		public:
			// constructors
			MoveOnly();

			// copying disabled:
			MoveOnly(const MoveOnly&) = delete;
			MoveOnly& operator=(const MoveOnly&) = delete;

			// moving enabled
			MoveOnly(MoveOnly&&) noexcept;
			MoveOnly& operator=(MoveOnly&&) noexcept;
		};
		void run()
		{
		}
	}
	// Using Move-Only Types
	namespace sec_13_1_2
	{
		class MoveOnly {
		public:
			// constructors
			MoveOnly() = default;

			// copying disabled:
			MoveOnly(const MoveOnly&) = delete;
			MoveOnly& operator=(const MoveOnly&) = delete;

			// moving enabled
			MoveOnly(MoveOnly&&) noexcept = default;
			MoveOnly& operator=(MoveOnly&&) noexcept = default;
		};

		void run()
		{
			std::vector<MoveOnly> coll;

			coll.push_back(MoveOnly{});		// creates a temporary object, which is moved into coll
		}
	}
	// Using Move-Only Types
	namespace sec_13_1_2b
	{
		class MoveOnly {
		public:
			// constructors
			MoveOnly() = default;

			// copying disabled:
			MoveOnly(const MoveOnly&) = delete;
			MoveOnly& operator=(const MoveOnly&) = delete;

			// moving enabled
			MoveOnly(MoveOnly&&) noexcept = default;
			MoveOnly& operator=(MoveOnly&&) noexcept = default;
		};

 		void run()
		{
			std::vector<MoveOnly> coll;

			MoveOnly mo;

			//coll.push_back(mo);			// ERROR: can't copy mo into coll
			
			std::cout << "size before move insertion = " << coll.size() << '\n';
			coll.push_back(std::move(mo));	// OK: moves mo into coll
			std::cout << "size after move insertion = " << coll.size() << '\n';

			//mo = coll[0];					// ERROR: can't copy from coll[i]
			mo = std::move(coll[0]);		// ...but we can move from coll[i]. coll[i] is left in an unuable state 
			std::cout << "size after move extraction = " << coll.size() << '\n';
		}
	}
	// Using Move-Only Types
	namespace sec_13_1_2c
	{
		class MoveOnly {
		public:
			// constructors
			MoveOnly() = default;

			// copying disabled:
			MoveOnly(const MoveOnly&) = delete;
			MoveOnly& operator=(const MoveOnly&) = delete;

			// moving enabled
			MoveOnly(MoveOnly&&) noexcept = default;
			MoveOnly& operator=(MoveOnly&&) noexcept = default;
		};

		void run()
		{
			std::vector<MoveOnly> coll1;
			std::vector<MoveOnly> coll2;

			coll1.push_back(MoveOnly{});
			coll1.push_back(MoveOnly{});
			coll1.push_back(MoveOnly{});
			coll1.push_back(MoveOnly{});			
			
			std::cout << "coll1 size before loop move insertion = " << coll1.size() << '\n';
			std::cout << "coll2 size before loop move insertion = " << coll2.size() << '\n';

			for (auto& elem : coll1) {		// note: non-const reference
				coll2.push_back(std::move(elem));
			}

			std::cout << "col11 size after loop move insertion = " << coll1.size() << '\n';
			std::cout << "col12 size after loop move insertion = " << coll2.size() << '\n';

			// NOTE: You cannot use std::initializer list because they are passed by value
			// std::vector<MoveOnly> coll3{ MoveOnly{}, MoveOnly{}, MoveOnly{}, MoveOnly{} };	// ERROR

			// You can only iterate by reference over all move-only elements of a container
			for (auto& elem : coll1);			// OK
			for (const auto& elem : coll1);		// OK
			// ... but not by value
			//for (auto elem : coll1);			// ERROR: iteration by value copies elements
		}
	}
	// Passing Move-Only Objects as Arguments
	namespace sec_13_1_3a
	{
		class MoveOnly {
		public:
			// constructors
			MoveOnly() = default;

			// copying disabled:
			MoveOnly(const MoveOnly&) = delete;
			MoveOnly& operator=(const MoveOnly&) = delete;

			// moving enabled
			MoveOnly(MoveOnly&&) noexcept = default;
			MoveOnly& operator=(MoveOnly&&) noexcept = default;
		};

		void sink(MoveOnly arg){}		// sink() takes ownership of the passed argument
		void run()
		{
			sink(MoveOnly{});			// OK, moves temporary objects to arg
			MoveOnly mo;
			//sink(mo);					// ERROR: cannot copy mo into arg
			sink(std::move(mo));		// OK: moves mo into arg becaused it is pass by value
		}
	}
	// Passing Move-Only Objects as Arguments
	namespace sec_13_1_3b
	{
		class MoveOnly {
		public:
			// constructors
			MoveOnly() = default;

			// copying disabled:
			MoveOnly(const MoveOnly&) = delete;
			MoveOnly& operator=(const MoveOnly&) = delete;

			// moving enabled
			MoveOnly(MoveOnly&&) noexcept = default;
			MoveOnly& operator=(MoveOnly&&) noexcept = default;
		};

		void sink(MoveOnly&& arg) {}	// sink() might take ownership of the passed argument
		void run()
		{
			sink(MoveOnly{});			// OK, moves temporary objects to arg
			MoveOnly mo;
			//sink(mo);					// ERROR: can't pass lvalue mo to arg
			sink(std::move(mo));		// OK: might move mo to something inside sink
		}
	}
	// Passing Move-Only Objects as Arguments
	namespace sec_13_1_3c
	{
		// design move only classes with a resource modifiying function like close(), reset() or release()
		class MoveOnly {
			int m_val;
		public:
			// constructors
			MoveOnly() = default;

			// copying disabled:
			MoveOnly(const MoveOnly&) = delete;
			MoveOnly& operator=(const MoveOnly&) = delete;

			// moving enabled
			MoveOnly(MoveOnly&&) noexcept = default;
			MoveOnly& operator=(MoveOnly&&) noexcept = default;

			// close the object
			void close()
			{
				m_val = 0;
			}
			// reset the state of the object
			void reset()
			{
				m_val = 0;
			}	
			// release the state of the object
			void release()
			{
				m_val = 0;
			}
		};

		void sink(MoveOnly&& arg) {}	// sink() might take ownership of the passed argument
		void run()
		{
			sink(MoveOnly{});			// OK, moves temporary objects to arg
			MoveOnly mo;
			//sink(mo);					// ERROR: can't pass lvalue mo to arg
			sink(std::move(mo));		// OK: might move mo to something inside sink

			mo.close();
			mo.reset();
			mo.release();
		}
	}
	// Returning Move-Only Objects by Value
	// ... move only objects can be return by value effectively
	namespace sec_13_1_4
	{
		class MoveOnly {
		public:
			// constructors
			MoveOnly() = default;

			// copying disabled:
			MoveOnly(const MoveOnly&) = delete;
			MoveOnly& operator=(const MoveOnly&) = delete;

			// moving enabled
			MoveOnly(MoveOnly&&) noexcept = default;
			MoveOnly& operator=(MoveOnly&&) noexcept = default;
		};

		MoveOnly source()
		{
			MoveOnly mo;
			// do stuff
			return mo;		// moves mo into the caller
		}
		void run()
		{
			MoveOnly mo{source()};		// takes direct ownership of the associated resource
		}
	}
	// Move-From states of Move-Only Objects
	namespace sec_13_1_5
	{
	}
}

