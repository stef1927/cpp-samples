#include <iostream>
#include <string>
#include <memory>
 
using namespace std;
 
class Node : public std::enable_shared_from_this<Node> {
public:
       explicit Node(int data) : data_(data) { }
 
public:
       void AddToTail(int data) {
              if (!next_) {
                     next_ = shared_ptr<Node>(new Node(data));
                     return;
              }
 
              next_->AddToTail(data);
              //auto node = next_;
              //while(node->next_) {
              //     node = node->next_;
              //}
              //node->next_ =  shared_ptr<Node>(new Node(data));
       }
 
      
       shared_ptr<Node> Reverse(shared_ptr<Node> prev) {
              if (!next_) {
                     next_ = prev;
                     return shared_from_this();
              }
 
              auto node = next_;
              next_ = prev;
              return node->Reverse(shared_from_this());
       }
 
       shared_ptr<Node> Reverse() {
              return Reverse(shared_ptr<Node>());
       }
       /*
       shared_ptr<Node> Reverse() {
              if (!next_) {
                     return shared_from_this();
              }
 
              auto prev = shared_from_this();
              auto node = next_;
              next_ = shared_ptr<Node>();
              while (node) {
                     auto temp = node->next_;
                     node->next_ = prev;
                     prev = node;
                     node = temp;
              }
 
              return prev;
       }*/
 
       void Print() const {
              cout << data() << " ";
              if(next_) {
                     next_->Print();
              }
       }
 
public:
       int data() const { return data_; }
 
private:
       int data_;
       shared_ptr<Node> next_;
};
 
 
int main() {
       shared_ptr<Node> head(new Node(3));
       head->AddToTail(5);
       head->AddToTail(6);
       head->AddToTail(7);
       head->AddToTail(8);
       head->AddToTail(9);
 
       head = head->Reverse();
 
       head->Print();
 
      // string c;
      // cin >> c;
      
       return 0;
}