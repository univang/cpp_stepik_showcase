#include "pch.h"
#include <iostream>


struct Dummy { };
typedef int type;

template <typename T, typename U>
struct SameType { static const bool value = false; };

template <typename T> struct
SameType<T, T>
{ static const bool value = true; };

template <typename T, size_t N>
size_t array_size(const T(&array)[N]) {
	return N;
}


int main()
{
	{
		//std::cout << SameType<int, int>::value << std::endl; // 1, i.e. true
		//std::cout << SameType<int, type>::value << std::endl; // 1, type == int
		//std::cout << SameType<int, int&>::value << std::endl; // 0, int and reference to int - are different types
		//std::cout << SameType<Dummy, Dummy>::value << std::endl; // 1
		//std::cout << SameType<int, const int>::value << std::endl; // 0, const int - is a specific int
	}


	{
		int ints[] = { 1, 2, 3, 4 };
		int *iptr = ints;
		double doubles[] = { 3.14 };
		array_size(ints); // вернет 4
		array_size(doubles); // вернет 1
		//array_size(iptr); // тут должна произойти ошибка компиляции
	}
}