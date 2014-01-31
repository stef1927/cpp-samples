#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stack>
#include <array>
#include <exception>
#include <stdexcept>
#include <sstream>
#include <iterator>
#include <memory>
#include <functional>

#include <cstdlib>
#include <cctype>
#include <cmath>
#include <cassert>

using namespace std;

class Calculator {

private:
    template<typename T>
    class Operator {
    public:
        virtual void Eval(stack<T>& stack) = 0;
    };

    template<typename T, int N>
    class OperatorImpl  : public Operator<T> {
    public:
        typedef array<T, N> Values; 

    public:
        explicit OperatorImpl(function <T (Values)> op) : op_(op) { }
      
        virtual void Eval(stack<T>& stack) {
            if (stack.size() < N)
                throw runtime_error("Stack is too small");

            Values values;
            for (auto i = 0; i < N; ++i) {
                values[i] = stack.top();
                stack.pop();
            }

            stack.push(op_(values));
        }

    private:
        function <T (Values)> op_;
    };

    typedef Operator<double> Op;
    typedef OperatorImpl<double, 0> NillOp;     // Push the result of op_ on the stack 
    typedef OperatorImpl<double, 1> UnaryOp;    // Pop one value and push the result of op_ on the stack
    typedef OperatorImpl<double, 2> BinaryOp;   // Pop two values and push the result of op_ on the stack

    /** Add more operators here if required. */
    static unique_ptr<Op> ParseOp(string input) {
        if (input.empty())
            throw invalid_argument("input is empty");
       
        if (input == "+") {
           return unique_ptr<Op>(new BinaryOp([](BinaryOp::Values vals){ return vals[1] + vals[0]; }));
       }
       else if (input == "-") {
           return unique_ptr<Op>(new BinaryOp([](BinaryOp::Values vals){ return vals[1] - vals[0]; }));
       }
       else if (input == "/") {
           return unique_ptr<Op>(new BinaryOp([](BinaryOp::Values vals){ return vals[1] / vals[0]; }));
       }
       else if (input == "*") {
           return unique_ptr<Op>(new BinaryOp([](BinaryOp::Values vals){ return vals[0] * vals[1]; }));
       }
       else if (input == "SIN") {
           return unique_ptr<Op>(new UnaryOp([](UnaryOp::Values vals){ return sin(vals[0]); }));
       }
       else if (input == "COS") {
           return unique_ptr<Op>(new UnaryOp([](UnaryOp::Values vals){ return cos(vals[0]); }));
       }
       else if (find_if(begin(input), end(input), [](char c) {
          return !isdigit(c); }) == input.end()) { // All digits
           return unique_ptr<Op>(new NillOp([input](NillOp::Values){ return atof(input.c_str()); }));
       }
       else {
           throw invalid_argument("input is invalid : " + input);
       }
   }

public:
    Calculator() { }
 
    double Calculate(const string& input) {
        istringstream iss(input);
  
        stack<double> stack;
        for_each(istream_iterator<string>(iss),
            istream_iterator<string>(), [&stack](const string& word){
            ParseOp(word)->Eval(stack);
        });
  
        if (stack.size() == 1) {
            return stack.top();
        }
        else {
            throw runtime_error("Failed to get an answer");
        }
    }
};

void test(Calculator& calculator, const string& input, double expected) {
    double res = calculator.Calculate(input);
    bool passed = fabs(res - expected) < 1e-6;

    cout << "<" << input << ">" 
    << " returned " << res 
    << " expected " << expected 
    << (passed ? " (OK)" : " (FAILED)") 
    << endl;

    if (!passed)
        throw runtime_error(input);
}

/** Some ideas for testing, more tests should be added */ 
bool test(Calculator& calculator) {
    try {
        test(calculator, "3 4 +", 7);
        test(calculator, "5 1 2 + 4 * + 3 - 2 /", 7);
        test(calculator, "9 2 * 9 /", 2);
        test(calculator, "1 SIN", 0.841471);
        test(calculator, "1 COS SIN", 0.514395);
    }
    catch(exception& ex) {
        cout << "Test failed : " << ex.what() << endl;
        return false;
    }
    return true;
}

int main () {
    Calculator calculator;
     
    if (!test(calculator)) {
        cout << "Tests failed, exiting." << endl;
        return -1;
    }

    bool done = false;
    while (!done) {
        cout << "Enter expression : " << flush;
      
        string input;
        getline(cin, input);
      
        transform(input.begin(), input.end(), input.begin(), ::toupper);
        if (input == "EXIT") {
            cout << "Exiting..." << endl;
            done = true;
        }
        else {
            try {
                double ret = calculator.Calculate(input);
                cout << "Result : " << ret << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
        }
    }
    return 0;
}