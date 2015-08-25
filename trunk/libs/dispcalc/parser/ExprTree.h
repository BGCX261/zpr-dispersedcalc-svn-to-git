#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <string>
#include <sstream>


using namespace std;
using namespace boost;

#ifndef _DISPCALC_EXPRTREE_H_
#define _DISPCALC_EXPRTREE_H_

// nth to n-ty element liczac od 1
//! Klasa reprezentujaca drzewo, potrafi wygenerowac siebie ze stosu
//	i transformowac siebie do stosu
struct ExprTree {
	typedef list<string> T_stack;

	ExprTree() : null_("N") {
	}
	//! konstruktor tworzacy obiekt na podstawie wyrazenia-stosu
	ExprTree(const T_stack& p_stack) : null_("N") {
		regenerate(p_stack);
	}
	//! konstruktor kopiujący tworzący obiekt na podstawie wyrazenia -stosu
	ExprTree(const ExprTree& p) : null_("N") {
		clear();
		values_ = p.values_;
		null_ = p.null_;
	}
	ExprTree(const string& a) : null_("N") {
		add(1, a);
	}
	void add(int nth, const string& v) {
		if(nth > values_.size()) {
			while(values_.size()<nth-1) values_.push_back(null_);
			values_.push_back(v);
		} else {
			values_[nth-1]=v;
		}
	}
	void addLeft(int nth, const string& v) {
		return add(leftN(nth), v);
	}
	void addRight(int nth, const string& v) {
		return add(rightN(nth), v);
	}
	string get(int nth) const {
		if(values_.size() < nth)
			return null_;
		return values_.at(nth-1);
	}
	string getParent() const {
		return get(1);
	}
	string operator[](int nth) const {
		return get(nth);
	}
	int parentN(int nth) const {
		return (nth-nth%2)/2;
	}
	int leftN(int nth) const {
		return nth*2;
	}
	int rightN(int nth) const {
			return (nth*2)+1;
	}
	int rootN() { return 1; }
	string left(int nth) const {
		return get(leftN(nth));
	}
	string right(int nth) const {
		return get(rightN(nth));
	}
	string parent(int nth) const {
		return get(parentN(nth));
	}
	bool isNull(int nth) const {
		return (null_ == get(nth));
	}
	bool isOp(string x) const {
		return (string("ADD") == x || string("SUBTRACT") == x || string("MULTIPLY") == x || string("DIVIDE") == x);
	}
	void clear() {
		values_.clear();
	}
	vector<string> getValues() {
			return values_;
	}
	int height(int nth = 1) const {
		return log2(values_.size()) - log2(nth);
	}
	//! generacja drzewa na podstawie stosu
	void regenerate(const T_stack& p_stack) {
		stack_ = p_stack;
		clear();
		T_stack::const_reverse_iterator topIt = p_stack.rbegin();

		string v;
		std::stack<int> emptyBranch;
		unsigned wasOp = -1;
		int parent = 1;

		if(p_stack.size() > 0) {
			v = *topIt;
			add(parent, v);
			if(!isOp(v) && 1 != p_stack.size()) cout << "Error - no op at end" << endl;
			wasOp = 1;
			++topIt;
		}

		for(;topIt!=p_stack.rend();++topIt) {
			v= *topIt;

			if(wasOp) {
				addRight(parent, v);
				emptyBranch.push(parent);
				if(isOp(v)) {
					wasOp = 1;
					parent = rightN(parent);
				} else { //!isOp(v)
					wasOp = 0;
				}
			} else { //!wasOp
				parent = emptyBranch.top();
				emptyBranch.pop();

				addLeft(parent, v);
				if(isOp(v)) {
					parent = leftN(parent);
					wasOp = 1;
				} else {
					parent = parentN(parent);
					wasOp = 0;
				}
			}
		}
	}
	//! zwroc stos na podstawie drzewa
	T_stack toStack(int nth = 1) const {
		//post order visitation actually
		T_stack stack_;
		if(isOp(get(nth)) && left(nth) != null_) {
			T_stack leftStack = toStack(leftN(nth));
			while(leftStack.size()) {
				stack_.push_back(leftStack.front());
				leftStack.pop_front();
			}
		}
		if(isOp(get(nth)) &&  right(nth) != null_) {
			T_stack rightStack = toStack(rightN(nth));
			while(rightStack.size()) {
				stack_.push_back(rightStack.front());
				rightStack.pop_front();
			}
		}
		stack_.push_back(get(nth));
		return stack_;
	}
	//returns a stack of trees saving operations between them in Polish notation
	list<ExprTree> chunk(int maxH) {
		list<ExprTree> vec_;
		if(height() <= maxH) {
			vec_.push_back(*this);
		} else {
			ExprTree e1( toStack(leftN(rootN())) );
			list<ExprTree> l1 = e1.chunk(maxH);
			while(l1.size() > 0) {
				vec_.push_back(l1.front());
				l1.pop_front();
			}

			ExprTree e2( toStack(rightN(rootN())) );
			list<ExprTree> l2 = e2.chunk(maxH);
			while(l2.size() > 0) {
				vec_.push_back(l2.front());
				l2.pop_front();
			}
			vec_.push_back(ExprTree(get(rootN())));
		}
		return vec_;
	}
	void print(ostringstream& os) const {
		vector<string>::const_iterator it = values_.begin();
		os << "tree:H(" << height(1) << ") ";
		int i = 1;
		while(it != values_.end()) {
			if(*it != null_)
				os << i<< "{" << *it << "} ";
			++it; ++i;
		}
		os << endl;
	}
private:
	T_stack stack_;
	vector<string> values_;
	string null_;
};

#endif //_DISPCALC_EXPRTREE_H_
