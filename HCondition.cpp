//
// Created by lucas on 11/20/19.
//
#pragma once
#include <string>
#include "HExpression.cpp"

using namespace std;

struct HCondition {
    HExpression *left;
    string op;
    HExpression *right;
    HCondition operator!() {
        HCondition hc;
        hc.left = left;
        hc.right = right;
        if (op == ">")
            hc.op = "<=";
        else if (op == "<")
            hc.op = ">=";
        else if (op == "==")
            hc.op = "/=";
        else if (op == "/=")
            hc.op = "==";
        else if (op == ">=")
            hc.op = "<";
        else if (op == "<=")
            hc.op = ">";
        return hc;
    }
};
