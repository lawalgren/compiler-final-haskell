cmake_minimum_required(VERSION 3.15)
project(compiler_final_haskell)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(compiler_final_haskell
        Token.cpp Tokenizer.cpp Parser.cpp HFunction.cpp HCondition.cpp HExpression.cpp HLogical.cpp generator.cpp)
