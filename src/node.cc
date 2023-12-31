#include"node.h"

#include<cassert>


NodeType GetType(const std::string& type){
    if (type == "if-statement") {
        return kIf;
    }
    else if (type == "then-body") {
        return kThenBody;
    }
    else if (type == "else-body") {
        return kElse;
    }
    else if (type == "for-statement") {
        return kFor;
    }
    else if (type == "for-condition") {
        return kForCondition;
    }
    else if (type == "for-body") {
        return kForBody;
    }
    else if (type == "for-update") {
        return kForUpdate;
    }
    else if (type == "pseudo-return"||type=="return") {
        return kReturn;
    }
    else if (type == "after-branch"||type =="first-statement") {
        return kNormal;
    }else if(type == "while-statement"){
        return kWhile;
    }else if(type == "while-body"){
        return kWhildBody;
    }
    assert(false);
}