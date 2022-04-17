#ifndef BOOL_EXPR_EVAL_H_
#define BOOL_EXPR_EVAL_H_

#include <string>

class BoolExpr {
    public:
        BoolExpr(std::string expr);
        ~BoolExpr();
        bool eval(std::string& err);

    private:
        BoolExpr();
        bool convertForm(const char* in, char* out, std::string& err);
        int op_preced(const char c);
        bool op_left_assoc(const char c);

        std::string m_expr;
};

#endif /*BOOL_EXPR_EVAL_H_*/
