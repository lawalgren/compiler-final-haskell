//
// Created by lucas on 11/20/19.
//

#include <string>

using namespace std;

struct HExpression {
    HExpression *left;
    string op;
    HExpression *right;
};
