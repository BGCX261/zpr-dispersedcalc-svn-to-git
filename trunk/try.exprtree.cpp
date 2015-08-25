#include <iostream>
#include <fstream>

#include <vector>
#include <stack>
#include <list>
#include <cmath>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

// nth to n-ty element liczac od 1
struct ExprTree {
	typedef list<string> T_stack;

	ExprTree() : null_("N") {
	}
	ExprTree(const T_stack& p_stack) : null_("N") {
		regenerate(p_stack);
	}
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
	void print(ostream& os) const {
		vector<string>::const_iterator it = values_.begin();
		os << "tree:H(" << height(1) << ") ";
		int i = 1;
		while(it != values_.end()) {
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

int main() {

	std::list<string> stack_;
	stack_.push_back(string("a"));
	stack_.push_back(string("b"));
	stack_.push_back(string("ADD"));
	stack_.push_back(string("c"));
	stack_.push_back(string("d"));
	stack_.push_back(string("SUBTRACT"));
	stack_.push_back(string("MULTIPLY"));
	stack_.push_back(string("e"));
	stack_.push_back(string("f"));
	stack_.push_back(string("DIVIDE"));
	stack_.push_back(string("SUBTRACT"));
	stack_.push_back(string("x"));
	stack_.push_back(string("y"));
	stack_.push_back(string("ADD"));
	stack_.push_back(string("SUBTRACT"));

	int i = 1;
	ExprTree e;
	e.regenerate(stack_);

	cout << "source stack:";
	std::list<string> ve1 = stack_;
	i=1;
	for(list<string>::iterator it1 = ve1.begin();it1!=ve1.end();++it1) {
		cout << "[" << i++ << "]" << *it1 << " ";
	}
	cout << endl;

	//check if previous source stack is made to stack
	cout << "tree.toStack():";
	std::list<string> ve = e.toStack();
	i=1;
	for(list<string>::iterator it2 = ve.begin();it2!=ve.end();++it2) {
		cout << "[" << i++ << "]" << *it2 << " ";
	}
	cout << endl << endl;

	e.print(cout);

	cout << endl << endl << "chunked" << endl << endl;

	typedef list<ExprTree> T_listTree;
	T_listTree chunks = e.chunk(2);
	T_listTree::iterator i1 = chunks.begin();
	while(i1 != chunks.end()) {
		i1->print(cout);
		++i1;
	}

	return 0;
}
