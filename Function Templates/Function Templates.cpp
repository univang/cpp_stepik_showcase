#include "pch.h"
#include <iostream>

#include <cstddef>

// This is an Array class for storing data of any type.
template <typename T>
struct Array {

	Array() : data_(nullptr), size_(0) { }

	explicit Array(size_t size, const T& value = T()) : size_(size) {
		if (size_ == 0) {
			data_ = nullptr;
		}
		else {
			data_ = (T*) new char[size * sizeof(T)];
			for (size_t i = 0; i < size_; ++i) {
				new (data_ + i) T(value);
			}
		}
	}

	~Array() {
		if (data_) {
			for (size_t i = 0; i < size_; ++i) {
				(data_ + i)->~T();
			}
			delete[](char *) data_;
		}
	}

	Array(const Array & other) : size_(other.size_) {
		if (!other.data_) {
			data_ = nullptr;
		}
		else {
			data_ = (T*) new char[size_ * sizeof(T)];

			for (size_t i = 0; i < size_; ++i) {
				new (data_ + i) T(*(other.data_ + i));
			}
		}
	}

	Array& operator=(const Array & other) {
		if (this == &other) {
			return *this;
		}

		this->~Array();
		new (this) Array(other); // placement new

		return *this;
	}

	size_t size() const {
		return size_;
	}

	T& operator[](size_t index) {
		return data_[index];
	}

	const T& operator[](size_t index) const {
		return data_[index];
	}

	void prnt()
	{
		for (size_t i = 0; i < size_; ++i)
			std::cout << *(data_ + i);
		std::cout << std::endl;
	}

private:
	T * data_;
	size_t size_;
};




template <typename T, typename U>
void copy_n(T* target, U* source, int amount) {
	for (int i = 0; i < amount; ++i) {
		*target = (T)*source;
		++target;
		++source;
	}
}

template <typename T, typename U>
T minimum(const Array<T> & array, U f) {
	T min = array[0];

	for (int i = 0; i < array.size(); ++i) {
		if (f(array[i], min)) {
			min = array[i];
		}
	}
	return min;
}

bool less(int a, int b) { return a < b; }
struct Greater { bool operator()(int a, int b) const { return b < a; } };

// This is a function to flatten Array class objects (for example: Array< Array<int> >)
template <typename T>
void flatten(const Array<Array<T>>& array, std::ostream& out)
{
	for (int i = 0; i < array.size(); ++i) {
		flatten(array[i], out);
	}
}
template <typename T>
void flatten(const Array<T>& array, std::ostream& out) {
	for (int i = 0; i < array.size(); ++i) {
		out << array[i] << ' ';
	}
}


struct ICloneable
{
	virtual ICloneable* clone() const = 0;
	virtual ~ICloneable() { }
};

// This a structure which holds a single value of any type
template <typename T>
struct ValueHolder : ICloneable {
	explicit ValueHolder(const T & data) : data_(data) { }

	ICloneable* clone() const {
		return new ValueHolder(this->data_);
	}

	T data_;
};

// This is a structure for storing many values of any types (it could be int, double or even user defined Array)
// (simplified version of boost::any)
struct Any
{
	Any() { };

	template<typename T>
	Any(const T& data) : ptr_(new ValueHolder<T>(data)) {}

	~Any() {
		if (ptr_) {
			delete ptr_;
		}
	}

	Any(const Any& other) : ptr_(other.ptr_ ? other.ptr_->clone() : nullptr) {}

	Any& operator=(const Any& other) {
		if (this == &other) {
			return *this;
		}

		this->~Any();
		new (this) Any(other);

		return *this;
	}

	template <typename T>
	T * cast() const {		
		ValueHolder<T> *check_ptr = dynamic_cast<ValueHolder<T> *>(this->ptr_);
		if (check_ptr) {
			return (T*)&((ValueHolder<T>*)this->ptr_)->data_;
		}
		else {
			return nullptr;
		}
	}

	ICloneable * ptr_ = nullptr;
};

int main()
{
	//int ints[] = { 1, 2, 3, 4 };
	//double doubles[4] = {};
	//copy_n(doubles, ints, 4); // doubles has elements: 1.0, 2.0, 3.0 и 4.0
	//
	//for (int i = 0; i < 4; ++i) {
	//	std::cout << doubles[i] << std::endl;
	//}
	//{
	//	Array<int> ints(3);;
	//	ints[0] = 10;
	//	ints[1] = 2;
	//	ints[2] = 15;
	//	int min = minimum(ints, less); // min is 2
	//	std::cout << min << std::endl;
	//	int max = minimum(ints, Greater()); // max is 15
	//	std::cout << max << std::endl;
	//}
	//{
	//	Array<int> ints(2, 0);
	//	ints[0] = 10;
	//	ints[1] = 20;
	//	flatten(ints, std::cout); // prints "10 20"
	//	std::cout << std::endl;

	//	Array< Array<int> > array_of_ints(2, ints);
	//	flatten(array_of_ints, std::cout); // prints "10 20 10 20"
	//	std::cout << std::endl;

	//	Array<double> doubles(10, 0.0);
	//	flatten(doubles, std::cout); // for any type
	//	std::cout << std::endl;
	//}

	{
		//ValueHolder<int> i(10);
		//ICloneable *p = i.clone();
		//std::cout << ((ValueHolder<int>*)p)->data_ << std::endl;
		//delete p;

		Any empty;
		Any a(10);
		Any copy(a);
		Any null_instance;

		null_instance = copy;
		std::cout << *null_instance.cast<int>() << std::endl;
		null_instance = 0;
		std::cout << *null_instance.cast<int>() << std::endl;

		std::cout << *copy.cast<int>() << std::endl;

		int* iptr = null_instance.cast<int>();
		std::cout << iptr << std::endl;
	}

	if (_CrtDumpMemoryLeaks())
		std::cout << "memory leak is detected" << std::endl;
	else
		std::cout << "no memory leaks" << std::endl;
}
