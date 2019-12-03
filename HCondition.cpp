//
// Created by lucas on 11/20/19.
//
#pragma once
#include <string>
#include "HExpression.cpp"

using namespace std;

class HCondition {
public:
    HExpression *left;
    string op;
    HExpression *right;

    HCondition() {
       left = nullptr;
       op = "";
       right = nullptr;
    }
};

inline HCondition operator!(const HCondition &og) {
    HCondition hc;
    hc.left = og.left;
    hc.right = og.right;
    if (og.op == ">")
        hc.op = "<=";
    else if (og.op == "<")
        hc.op = ">=";
    else if (og.op == "==")
        hc.op = "/=";
    else if (og.op == "/=")
        hc.op = "==";
    else if (og.op == ">=")
        hc.op = "<";
    else if (og.op == "<=")
        hc.op = ">";
    return hc;
}

inline bool operator<(const HCondition &c, const HCondition &d) {
    return(d.op.length() > c.op.length());
}
