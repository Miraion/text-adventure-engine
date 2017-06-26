#ifndef stack_h
#define stack_h

#include <string>
#include <vector>

using namespace std;

template <class T> 
class Stack {
protected:
	vector<T> arr;

public:
	Stack<T> () {
		this->arr = vector<T>{};
	}

	int size () const {
		return arr.size();
	}

	void push (T const &item) {
		arr.push_back(item);
	}

	T pop () {
		if (arr.empty()) {
			throw out_of_range("Stack<>::pop(): empty stack");
		}
		T last = arr.back();
		arr.pop_back();
		return last;
	}

	T peak () const {
		if (arr.empty()) {
			throw out_of_range("Stack<>::peak(): empty stack");
		}
		return arr.back();
	}

	vector<T> peak (int distance) const {
		if (arr.empty()) {
			throw out_of_range("Stack<>::peak(int): empty stack");
		}
		vector<T> elements = vector<T>{};
		for (int i = 1; i <= distance; i++) {
			elements.push_back(arr[arr.size() - i]);
		}
		return elements;
	}

	T peak_bottom () const {
		if (arr.empty()) {
			throw out_of_range("Stack<>::peak_bottom(): empty stack");
		}
		return arr.front();
	}

	void invert () {
		Stack<T> newStack = Stack<T>{};
		while (!empty()) {
			newStack.push(this->pop());
		}
		this->arr = newStack.arr;
	}

	bool empty () {
		return arr.empty();
	}
};


class CharStack : public Stack<char> {
public:

	CharStack () {
		this->arr = vector<char>{};
	}
	
	CharStack (string const &str) {
		this->arr = vector<char>{};
		for (int i = str.length() - 1; i >= 0; i--) {
			arr.push_back(str[i]);
		}
	}

	string peak_string (int distance) const {
		if (arr.empty()) {
			throw out_of_range("Stack<>::peak_string(int): empty stack");
		}
		string str = "";
		for (int i = 1; i <= distance; i++) {
			str.push_back(arr[arr.size() - i]);
		}
		return str;
	}

	string consolidate () {
		string str = "";
		while (!empty()) {
			str.push_back(this->pop());
		}
		return str;
	}

	void push_string (string const &s) {
		for (int i = s.length() - 1; i >= 0; i--) {
			push(s[i]);
		}
	}

};

#endif