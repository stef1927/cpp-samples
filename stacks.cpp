#include <iostream>
#include <algorithm>
#include <functional>
#include <stack>
#include <vector>
#include <string>
 
using namespace std;
 
/**
       A set of stacks that behaves like a stack:
              if one stack becomes too high then a new one is added to the set
              and if a stack becomes empty and it is not the first stack,
              then it is removed.
*/
template <class T>
class MultipleStacks {
public:
       explicit MultipleStacks(int inMaxHeight) : maxHeight(inMaxHeight), currentIndex(0) {
              stacks.push_back(stack<T>());
       }
 
       bool empty() {
              return size() == 0;
       }
 
       int size() {
              int ret = 0;
 
              for_each(begin(stacks), end(stacks), [this, &ret](const stack<T>& stack) {
                     ret += stack.size();
              });
 
              return ret;
       }
 
       const T& top() const {
              return stacks[currentIndex].top();
       }
 
       T& top() {
              return stacks[currentIndex].top();
       }
 
       void push(const T& item) {
              if (stacks[currentIndex].size() > maxHeight) {
                     stacks.push_back(stack<T>());
                     currentIndex++;
              }
              stacks[currentIndex].push(item);
       }
 
       void pop() {
              stacks[currentIndex].pop();
 
              if (currentIndex > 0 && stacks[currentIndex].size() == 0) {
                     stacks.pop_back();
                     currentIndex--;
              }
       }
 
private:
       unsigned int maxHeight;
       unsigned int currentIndex;
 
       vector<stack<T>> stacks;  
};
 
int main() {
       MultipleStacks<int> stacks(10);
 
       for (auto i = 0; i < 100; ++i)
              stacks.push(i);
 
       while(!stacks.empty()) {
              int top = stacks.top();
              stacks.pop();
              cout << top << endl;
       }
 
       cout << "Press enter to continue..." << endl;
       string line;
       getline(cin, line);
 
       return 0;
}