#include "boolEval.h"
#include <iostream>
#include <string>

int main() {
    std::string err;
    /*
    std::string expr1 = "0 | c";
    std::string expr2 = "0 | F";
    std::string expr3 = "0 | (ABC | 0)";
    */
    std::string expr4 = "(1 & & 1)";
    /*
    BoolExpr boolean1(expr1);
    bool b = boolean1.eval(err);
    std::cout << expr1 << " = " << b << " err: " << err << std::endl;

    err.clear();

    BoolExpr boolean2(expr2);
    b = boolean2.eval(err);
    std::cout << expr2 << " = " << b << " err: " << err << std::endl;

    BoolExpr boolean3(expr3);
    b = boolean3.eval(err);
    std::cout << expr3 << " = " << b << " err: " << err << std::endl;
    */

    BoolExpr boolean4(expr4);
    bool b = boolean4.eval(err);
    std::cout << expr4 << " = " << b << " err: " << err << std::endl;

    return 0;
}
