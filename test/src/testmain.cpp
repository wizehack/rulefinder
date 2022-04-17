#include <iostream>
#include "testHelper.h"

int main()
{
    OneshotRuleTestHelper oneshothelper;
    CompositeRuleTestHelper compositeHelper;
    BoolExprTestHelper boolHelper;

    oneshothelper.setUp();
    bool b1 = oneshothelper.test();
    compositeHelper.setUp();
    bool b2 = compositeHelper.test();
    bool b3 = boolHelper.test();

    if( b1 && b2 && b3 )
    {
        std::cout << "Test success" << std::endl;
    }
    else
    {
        std::cout << "Test failed" << std::endl;
    }

    return 0;
}
