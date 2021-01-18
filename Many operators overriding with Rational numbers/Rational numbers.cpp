#include "pch.h"
#include <iostream>

struct Rational {

	Rational(int numerator = 0, int denominator = 1) : numerator_(numerator), denominator_(denominator) {};

	void add(Rational rational) {
		numerator_ = numerator_ * rational.denominator_ + rational.numerator_ * denominator_;
		denominator_ *= rational.denominator_;
	}

	void sub(Rational rational) {
		numerator_ = numerator_ * rational.denominator_ - rational.numerator_ * denominator_;
		denominator_ *= rational.denominator_;
	}

	void mul(Rational rational) {
		numerator_ *= rational.numerator_;
		denominator_ *= rational.denominator_;
	}

	void div(Rational rational) {
		numerator_ *= rational.denominator_;
		denominator_ *= rational.numerator_;
	}

	void neg() {
		numerator_ = -numerator_;
	}

	void inv() {}

	double to_double() const {
		return numerator_ / (double)denominator_;
	}

	Rational & operator+=(const Rational rational) {
		this->add(rational);
		return *this;
	}

	Rational & operator-=(const Rational rational) {
		this->sub(rational);
		return *this;
	}

	Rational & operator*=(const Rational rational) {
		this->mul(rational);
		return *this;
	}


	Rational & operator/=(const Rational rational) {
		this->div(rational);
		return *this;
	}

	Rational operator-() const {
		Rational rational(this->numerator_, this->denominator_);
		rational.neg();
		return rational;
	}

	Rational operator+() const {
		Rational rational(this->numerator_, this->denominator_);
		return rational;
	}

	bool operator==(Rational const& other) const {
		if (numerator_*(int)other.denominator_ == (int)denominator_*other.numerator_) {
			return true;
		}
		return false;
	}

	bool operator<(Rational const& other) const {
		if (numerator_*(int)other.denominator_ < (int)denominator_*other.numerator_) {
			return true;
		}
		return false;
	}

	// when all other operators are disabled
	//operator double() const {
	//	return this->to_double();
	//}

private:
	int numerator_;
	int denominator_;
};

Rational operator+(Rational lhs, Rational const& rhs) {
	return lhs += rhs;
}

Rational operator-(Rational lhs, Rational const& rhs) {
	return lhs -= rhs;
}

Rational operator*(Rational lhs, Rational const& rhs) {
	return lhs *= rhs;
}

Rational operator/(Rational lhs, Rational const& rhs) {
	return lhs /= rhs;
}


// <, <= , >, >= , == , !=
// for left implicit type conversion
bool operator==(int lhs, Rational const& rhs) {
	return (rhs == lhs);
}

bool operator!=(Rational const& lhs, Rational const& rhs) {
	return !(rhs == lhs);
}

bool operator<(int lhs, Rational const& rhs) {
	return (Rational(lhs) < rhs);
}

bool operator>(Rational const& lhs, Rational const& rhs) {
	return (rhs < lhs);
}

bool operator<=(Rational const& lhs, Rational const& rhs) {
	return !(rhs < lhs);
}

bool operator>=(Rational const& lhs, Rational const& rhs) {
	return !(lhs < rhs);
}



int main() {
	Rational r1(1, 2);
	Rational r2(1, 2);
	Rational r3(5);
	Rational r4(1, 4);

	r1 = r1 + 2;
	std::cout << r1.to_double() << std::endl;

	r1 = r2;
	r1 = r1 - 2;
	std::cout << r1.to_double() << std::endl;

	r1 = r2;
	r1 = -r1;
	std::cout << r1.to_double() << std::endl;

	r1 = r2;
	r1 = r1 * 100;
	std::cout << r1.to_double() << std::endl;

	r1 = r2;
	r1 = r1 / 2;
	std::cout << r1.to_double() << std::endl;

	r1 = r2;
	std::cout << (1 >= r4) << std::endl;
}
