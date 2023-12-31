#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include<string>

enum NodeType{
  kStmt,  // 普通语句

  kFor,
  kForCondition,
  kForBody,
  kForUpdate,

  kIf,
  kThenBody,  // if语句的thenbody
  kElse,   // if语句的elsebody

  kWhile,
  kWhildBody,  // while()括号中的语句

  kNormal, // 普通语句 

  kReturn,
};

NodeType GetType(const std::string& typeStr);

class Node{
public:
  Node(int id, NodeType type):id_(id),type_(type){
  }
  Node(int id, NodeType type, int lineNumber): id_(id), type_(type), lineNumber_(lineNumber){
  }
  
  Node* AddChild(Node* child){
    child->parent_ = this;
    children_.push_back(child);
    return this;
  }
  Node* AddChild(int id, NodeType type){
    return AddChild(new Node(id, type));
  }
  const NodeType type() {
    return type_;
  }

  const std::vector<Node*> Children(){
    return children_;
  }

  int Id() const{
    return id_;
  }
  int LineNumber() const {
    return lineNumber_;
  }

private:
  int id_;
  std::vector<Node*> children_;
  Node* parent_=nullptr;
  NodeType type_;
  int lineNumber_=-1;
};





#endif // _NODE_H_