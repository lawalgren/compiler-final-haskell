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
    string op = "";
    HExpression *right;

    HCondition() {
       left = nullptr;
       op = "";
       right = nullptr;
    }

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
