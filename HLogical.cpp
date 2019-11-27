//
// Created by lucas on 11/20/19.
//

#include "HCondition.cpp"
#include <string>
using namespace std;

struct HLogical {
    HCondition *left = nullptr;
    string op;
    HCondition *right = nullptr;
};

HLogical operator!(HLogical &hl) {
    HLogical _hl;
    (*_hl.left) = !(*hl.left);
    (*_hl.right) = !(*hl.right);
    _hl.op = "";
    if(hl.op == "&&")
        _hl.op = "||";
    else if (hl.op == "||")
        _hl.op = "&&";
    return _hl;
}
