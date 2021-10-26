#pragma once

#include <type_traits>

// Section 7.3
// I DID NOT UNDERSTAND THIS SECTION
// noexcept Declarations in Class Heirarchies

template <typename Base>
struct Wrapper : Base {
	using Base::Base;
	// implement all possibly wrapped pure virtual functions
	void print() const {}
};

template <typename T>
static constexpr inline bool is_nothrow_movable_v
	= std::is_nothrow_move_constructible_v<Wrapper<T>>;
