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
};
