#include "pch.h"
#include <iostream>

#include <algorithm> // std::swap
#include <cstddef>   // size_t
#include <cstring>   // strlen, strcpy

#pragma warning(disable:4996)

// This class overrides double squared operator [][].
// So that using it as my_str[i][j] returns a substring of my_str starting from i-th and ending with j - 1 character.
// String const hello("hello");
// String const ell = hello[1][4]; // now ell contains string "ell"
struct String {
	class Proxy {

		String const *_obj;
		int _initial_index;

	public:
		Proxy(String const *obj, int initial_index) : _obj(obj), _initial_index(initial_index) { }

		String operator[](int index) {
			char * temp_str = new char[index - _initial_index + 1];
			for (int i = 0; i < index - _initial_index; ++i) {
				temp_str[i] = _obj->str[i + _initial_index];
			}
			temp_str[index - _initial_index] = '\0';
			String const result(temp_str);

			delete[] temp_str;
			//std::cout << temp_str << std::endl;

			return result;
		}
	};

	String(const char *in_str = "") {
		size = strlen(in_str);
		str = new char[size + 1]; // + 1 for keeping the null character
		strncpy_s(str, size + 1, in_str, size);
	};

	String(size_t n, char c) {};

	String(const String &other) {
		size = strlen(other.str);
		str = new char[other.size + 1]; // + 1 for keeping the null character
		strncpy_s(str, other.size + 1, other.str, other.size);
	};

	~String() {};

	void swap(String &other)
	{
		std::swap(size, other.size);
		std::swap(str, other.str);
	}

	String &operator=(const String &other)
	{
		std::cout << "in operatror=" << std::endl;
		if (this != &other)
			String(other).swap(*this);
		return *this;
	}

	void append(const String &other) {};

	size_t size;
	char *str;

	Proxy operator[](int index) const {
		return Proxy(this, index);
	}
};

std::ostream& operator<<(std::ostream &os, String const& entity) {
	os << entity.str;
	return os;
}



int main()
{
	String const hello("hello");

    std::cout << hello << std::endl;

	//std::cout << std::endl;
	//String hell("hello");
	//std::cout << hell[0][4] << std::endl;

	//std::cout << hell << std::endl;

	//String he = hello[1][2];
	//std::cout << he << std::endl;
	////std::cout << he.size << std::endl;
	//std::cout << hello[1][hello.size] << std::endl;
	//std::cout << he[1][2] << std::endl;

	String const hell = hello[0][4];
	std::cout << hell << std::endl;
}