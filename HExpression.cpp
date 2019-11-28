//
// Created by lucas on 11/20/19.
//

#include <utility>

#include "Token.cpp"

using namespace std;

class HExpression {
public:
    HExpression* left;
    Token data;
    HExpression* right;

    explicit HExpression(Token _data = Token(), HExpression* _left = nullptr, HExpression* _right = nullptr) {
        data = std::move(_data);
        left = _left;
        right = _right;
    }

    explicit HExpression(HExpression *&hc) {
            data = hc->data;
            left = hc->left;
            right = hc->right;
    }
};
