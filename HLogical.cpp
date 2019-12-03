//
// Created by lucas on 11/20/19.
//
#pragma once
#include "HCondition.cpp"
#include <string>
using namespace std;

class HLogical {
public:
    HCondition *left;
    string op;
    HCondition *right;

    HLogical() {
        left = nullptr;
        op = "";
        right = nullptr;
    }
};

inline HLogical operator!(const HLogical &hl) {
    HLogical _hl;
    if (hl.left != nullptr)
        _hl.left = new HCondition(!(*hl.left));
    if (hl.right != nullptr)
        _hl.right = new HCondition(!(*hl.right));
    _hl.op = "";
    if(hl.op == "&&")
        _hl.op = "||";
    else if (hl.op == "||")
        _hl.op = "&&";
    return _hl;
}

inline bool operator<(const HLogical &c, const HLogical &d) {
    return (d.op.length() > c.op.length());
}
