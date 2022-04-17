#include "testHelper.h"
#include "boolEval.h"

BoolExprTestHelper::BoolExprTestHelper():m_expr() {}
BoolExprTestHelper::~BoolExprTestHelper() {}

void BoolExprTestHelper::setUp(){}

bool BoolExprTestHelper::test()
{
    bool b1 = this->test_true_return();
    this->print(b1);

    bool b2 = this->test_false_return();
    this->print(b2);

    bool b3 = this->test_error_return();
    this->print(b3);

    return b1 && b2 && b3;
}

bool BoolExprTestHelper::test_true_return()
{
    std::cout << __FUNCTION__ ;

    bool bRet = true;
    std::string expr0 = "(1 | 0) & (!0 & 1)";
    std::string expr1 = "!0 & !0 | 0";
    std::string expr2 = "(!!0 | !!1)";

    m_expr.push_back(expr0);
    m_expr.push_back(expr1);
    m_expr.push_back(expr2);

    for(unsigned int i=0; i<m_expr.size(); i++)
    {
        std::string err;
        BoolExpr boolean(m_expr[i]);
        bool b = boolean.eval(err);

        if(false == b)
        {
            std::cout << "[FAILED] [" << i << "] " << m_expr[i] << ": " << b << std::endl;
            bRet = false;
        }
    }
    m_expr.clear();

    return bRet;
}

bool BoolExprTestHelper::test_false_return()
{
    std::cout << __FUNCTION__ ;
    bool bRet = true;
    std::string expr0 = "(0 | (1 & 0 & 1 & 0))";
    std::string expr1 = "0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0";
    std::string expr2 = "0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0";
    std::string expr3 = "(   1 & 1    )    & 0";

    m_expr.push_back(expr0);
    m_expr.push_back(expr1);
    m_expr.push_back(expr2);
    m_expr.push_back(expr3);

    for(unsigned int i=0; i<m_expr.size(); i++)
    {
        std::string err;
        BoolExpr boolean(m_expr[i]);
        bool b = boolean.eval(err);

        if(true == b)
        {
            std::cout << "[FAILED] [" << i << "] " << m_expr[i] << ": " << b << std::endl;
            bRet = false;
        }
    }
    m_expr.clear();

    return bRet;
}

bool BoolExprTestHelper::test_error_return()
{
    std::cout << __FUNCTION__ ;
    bool bRet = true;

    std::string expr0 = "(1 & 1 | 0";
    std::string expr1 = "(1 & & 1)";
    std::string expr2 = "1 \\0 0";
    std::string expr3 = "(1 & 1) & 0)";
    std::string expr4 = "(1 & text) & 1";
    std::string expr5 = "(1 & ! text) & 1";
    std::string expr6 = "ab | DE | 1";
    std::string expr7 = "abc | DEF | 1";
    std::string expr8 = "(   a & b    )    & c";
    std::string expr9 = "1 * 1";

    m_expr.push_back(expr0);
    m_expr.push_back(expr1);
    m_expr.push_back(expr2);
    m_expr.push_back(expr3);
    m_expr.push_back(expr4);
    m_expr.push_back(expr5);
    m_expr.push_back(expr6);
    m_expr.push_back(expr7);
    m_expr.push_back(expr8);
    m_expr.push_back(expr9);


    for(unsigned int i=0; i<m_expr.size(); i++)
    {
        std::string err;
        BoolExpr boolean(m_expr[i]);
        bool b = boolean.eval(err);

        if(b == true || err.empty())
        {
            std::cout << "[FAILED] [" << i << "] " << m_expr[i] << ": " << b << std::endl;
            bRet = false;
        }
    }
    m_expr.clear();

    return bRet;
}

