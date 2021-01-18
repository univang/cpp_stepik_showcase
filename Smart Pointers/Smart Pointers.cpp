#include "pch.h"
#include <iostream>

struct Number;
struct BinaryOperation;

// This is a virtual class featuring the Visitor design pattern
struct Visitor {
	virtual void visitNumber(Number const * number) = 0;
	virtual void visitBinaryOperation(BinaryOperation const * operation) = 0;
	virtual ~Visitor() { }
};

// This is a base class for representing arithmetic expressions
struct Expression {
	virtual double evaluate() const = 0;
	virtual void visit(Visitor * visitor) const = 0;
	virtual ~Expression() { }
};

// This is a class for representing numbers
// The class has a field of type "double"
struct Number : Expression {

	Number(double value)
		: value(value)
	{}

	double evaluate() const
	{
		return value;
	}

	void visit(Visitor * visitor) const { visitor->visitNumber(this); }
	double get_value() const { return value; }

	~Number() {}

private:
	double value;
};

// This is a class for representing binary operations: '+', '-', '*', '/'
// This class has three fields: pointers to left and right elements and a "char" value corresponding to the operation performed.
struct BinaryOperation : Expression {

	BinaryOperation(Expression const * left, char op, Expression const * right)
		: left(left), op(op), right(right)
	{ }

	double evaluate() const
	{
		double result = 0.0;
		switch (op) {
		case '+':
			result = left->evaluate() + right->evaluate();
			return result;
		case '-':
			result = left->evaluate() - right->evaluate();
			return result;
		case '*':
			result = left->evaluate() * right->evaluate();
			return result;
		case '/':
			if (right->evaluate() != 0) {
				result = left->evaluate() / right->evaluate();
			}
			return result;
		default:
			return -1.0;
		}
	}

	void visit(Visitor * visitor) const { visitor->visitBinaryOperation(this); }

	Expression const * get_left()  const { return left; }

	Expression const * get_right() const { return right; }

	char get_op() const { return op; }

	~BinaryOperation()
	{
		delete left;
		delete right;
	}

private:
	Expression const * left;
	Expression const * right;
	char op;
};

// This is a class for printing binary operation (placing brackets according to the correct order of evaluation)
struct PrintVisitor : Visitor {

	void visitNumber(Number const * number)
	{
		std::cout << number->get_value();
	}

	void visitBinaryOperation(BinaryOperation const * bop)
	{
		std::cout << "(";
		bop->get_left()->visit(this);
		if (bop->get_op() == '*' || bop->get_op() == '/')
			std::cout << bop->get_op();
		else
			std::cout << bop->get_op();
		bop->get_right()->visit(this);
		std::cout << ")";
	}
};

// Simple Smart Pointer Class — Resource Acquisition Is Initialization	(RAII)
//
// This is a class for working with pointers to Expression class.
// This class provides a way to automatically allocate and deallocate memory for dynamic objects.
// It frees memory once a block statement completes (in which an oject was created).
// It has methods to "get" pointer, "release" — to return a pointer without freeing memory,
//   "reset" — to delete memory and set a new pointer.
// It also has overriden operators "*", "->" to access values by pointer.
struct ScopedPtr {

	explicit ScopedPtr(Expression *ptr = 0) {
		this->ptr_ = ptr;
	}

	~ScopedPtr() {
		delete this->ptr_;
	}

	Expression* get() const {
		return this->ptr_;
	}

	Expression* release() {
		Expression* temp = this->ptr_;

		this->ptr_ = nullptr;
		return temp;
	}

	void reset(Expression *ptr = 0) {
		delete this->ptr_;
		this->ptr_ = ptr;
	}

	Expression& operator*() const {
		return *ptr_;
	}

	Expression* operator->() const {
		return ptr_;
	}

private:
	// Featuring the Singleton pattern: private methods restrict copying of ScopedPtr
	ScopedPtr(const ScopedPtr&);
	ScopedPtr& operator=(const ScopedPtr&);

	Expression *ptr_;
};

// SharedPtr is a more advanced than the ScopedPtr Smart Pointer Class
//
// This is an allied class to SharedPtr for counting Pointers to the same objects
class Counter {
public:
	void add() {
		++counter_;
	}

	int release() {
		return --counter_;
	}

private:
	friend class SharedPtr;
	int counter_;
};

// This class is similar to the ScopedPtr class.
// Though this class, as opposed to the ScopedPtr class, allows having multiple Smart Pointers to the same objects.
// It counts a number of Pointers and frees memory if there are no pointers left.
struct SharedPtr {

	explicit SharedPtr(Expression *ptr = 0) : ptr_(ptr), reference_(nullptr) {
		if (ptr_) {
			reference_ = new Counter();
			reference_->add();
			//std::cout << "counter adding = " << reference_->counter_ << std::endl;
		}
	}

	~SharedPtr() {
		if (this->ptr_) {
			--reference_->counter_;
			//std::cout << "counter substracting = " << reference_->counter_ << std::endl;
			if (reference_->counter_ == 0) {
				//std::cout << "... pointer and reference deleting = " << reference_->counter_ << std::endl;
				delete reference_;
				delete ptr_;
			}
		}
	}

	SharedPtr(const SharedPtr & other) {
		if (other.ptr_) {
			this->ptr_ = other.ptr_;
			this->reference_ = other.reference_;
			++this->reference_->counter_;
			//std::cout << "counter adding when copying = " << reference_->counter_ << std::endl;
		}
	}

	SharedPtr& operator=(const SharedPtr & rhs) {
		if (this->ptr_ == rhs.ptr_) {
			// check so reference_ was the same aswell
			//std::cout << "assignment to itself = " << reference_->counter_ << std::endl;
			return *this;
		}
		if (this->ptr_) {
			--this->reference_->counter_;
			if (this->reference_->counter_ == 0) {
				//std::cout << "... pointer and reference deleting = " << reference_->counter_ << std::endl;
				delete reference_;
				delete ptr_;
			}
		}
		if (rhs.ptr_) {
			++rhs.reference_->counter_;
		}
		this->ptr_ = rhs.ptr_;
		this->reference_ = rhs.reference_;
		return *this;
	}

	Expression* get() const {
		return this->ptr_;
	}

	void reset(Expression *ptr = 0) {
		if (this->ptr_) {
			--reference_->counter_;
			if (reference_->counter_ == 0) {
				delete ptr_;
				delete reference_;
			}
		}
		this->ptr_ = ptr;
		if (ptr) {
			reference_ = new Counter();
			reference_->add();
		}
		else {
			reference_ = nullptr;
		}
	}

	Expression& operator*() const {
		return *ptr_;
	}

	Expression* operator->() const {
		return ptr_;
	}

private:
	Expression *ptr_;
	Counter *reference_;
};

// There are some use cases/tests below.
int main() {

	//std::cout << "hwello worlds" << std::endl;


	//Expression * sub1 = new BinaryOperation(new Number(5), '-', new Number(3));
	//Expression * sub2 = new BinaryOperation(new Number(2), '-', new Number(1.5));
	//Expression const * expr = new BinaryOperation(sub1, '/', sub2);

	////// evaluating and printing the result: 25.5
	////std::cout << expr->evaluate() << std::endl;

	////PrintVisitor visitor;
	////expr->visit(&visitor);


	//SharedPtr p1(sub1);
	//SharedPtr p2(p1);
	//SharedPtr p3(sub2);
	//std::cout << p3->evaluate() << std::endl;
	//p3 = p2;
	//std::cout << p3->evaluate() << std::endl;

	////std::cout << p1->evaluate() << std::endl;
	{
		SharedPtr p1;
		{
			SharedPtr p2(new BinaryOperation(new Number(5), '-', new Number(3)));
			SharedPtr p3(new BinaryOperation(new Number(2), '-', new Number(1.5)));
			SharedPtr p4(p2);
			SharedPtr p5;
			p5 = p2;
			p5 = p4;
			p1 = p5;
			p3.reset(NULL);
			p3 = p5;
			p5.reset(NULL);
			SharedPtr p6;
			SharedPtr p7;
			p7 = p7;
			p7.reset(NULL);
			p7.reset(new BinaryOperation(new Number(27), '/', new Number(3)));
			SharedPtr p8(new BinaryOperation(new Number(1000), '-', new Number(2000)));
			p8.reset(NULL);
		}
		p1 = p1;
	}
	
	if (_CrtDumpMemoryLeaks())
		std::cout << "memory leak is detected" << std::endl;
	else
		std::cout << "no memory leaks" << std::endl;

	return 0;
}
