#include "boolEval.h"
#include <string.h>
#include <stdio.h>
#include <string>
#include <iostream>

#define is_operator(c)  (c == '!' || c == '=' || c == '&' || c == '|')
#define is_function(c)  ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define is_ident(c)     (c >= '0' && c <= '9')

#define MAX_OUTPUT_SIZE 512
#define MAX_PART_SIZE 256

BoolExpr::BoolExpr(std::string expr): m_expr(expr){}
BoolExpr::~BoolExpr(){}

bool BoolExpr::eval(std::string& err)
{
    char output[MAX_OUTPUT_SIZE] = {0};
    char * op;
    bool tmp;
    char part1[MAX_PART_SIZE], part2[MAX_PART_SIZE];

    if(!convertForm(m_expr.c_str(), output, err))
    {
        return false;  //can't convert to postfix form
    }

    //std::cout << "[output ]" << output << std::endl;
    while (strlen(output) > 1) {

        op = &output[0];
        //std::cout << "[op ]" << op << std::endl;
        while (!is_operator(*op) && *op != '\0')
            op++;
        if (*op == '\0') {
            err = "invalid operator";
            return false;
        }
        else if (*op == '!') {
            tmp = !(*(op-1) - 48);
            *(op-1) = '\0';
        }
        else if(*op == '&') {
            tmp = (*(op-1) - 48) && (*(op-2) - 48);
            *(op-2) = '\0';
        }
        else if (*op == '|') {
            tmp = (*(op-1) - 48) || (*(op-2) - 48);
            *(op-2) = '\0';
        }

        memset(part1, 0, sizeof(part1));
        memset(part2, 0, sizeof(part2));
        strcpy(part1, output);
        strcpy(part2, op+1);

        if(strlen(output) > MAX_OUTPUT_SIZE ||
                strlen(part1) > MAX_PART_SIZE ||
                strlen(part2) > MAX_PART_SIZE) {
            err = "stack overflow"; 
            return false;  // stack overflow 
        }

        memset(output, 0, sizeof(output));
        strcat(output, part1);
        strcat(output, ((tmp==false) ? "0" : "1"));
        strcat(output, part2);
    }
    return *output - 48;
}

bool BoolExpr::convertForm(const char* input, char* output, std::string& err)
{
    const char *strpos = input, *strend = input + strlen(input);
    char c, *outpos = output;

    char stack[32];       // operator stack
    unsigned int sl = 0;  // stack length
    char     sc;          // used for record stack element

    while(strpos < strend)   {
        // read one token from the input stream
        c = *strpos;
        if(c != ' ')    {
            //std::cout << "token: " << c << std::endl;
            if(is_ident(c))  {
                //std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << " " << " is ident" << std::endl;
                *outpos = c;
                ++outpos;
            }
            // If the token is an operator, op1, then:
            else if(is_operator(c))  {
                while(sl > 0)    {
                    sc = stack[sl - 1];
                    if(is_operator(sc) &&
                            ((op_left_assoc(c) && (op_preced(c) >= op_preced(sc))) ||
                             (op_preced(c) > op_preced(sc))))   {
                        // Pop op2 off the stack, onto the output queue;
                        *outpos = sc;
                        ++outpos;
                        sl--;
                    }
                    else   {
                        break;
                    }
                }
                // push op1 onto the stack.
                stack[sl] = c;
                ++sl;
            }
            // If the token is a left parenthesis, then push it onto the stack.
            else if(c == '(')   {
                stack[sl] = c;
                ++sl;
            }
            // If the token is a right parenthesis:
            else if(c == ')')    {
                bool pe = false;
                // Until the token at the top of the stack is a left parenthesis,
                // pop operators off the stack onto the output queue
                while(sl > 0)     {
                    sc = stack[sl - 1];
                    if(sc == '(')    {
                        pe = true;
                        break;
                    }
                    else  {
                        *outpos = sc;
                        ++outpos;
                        sl--;
                    }
                }
                // If the stack runs out without finding a left parenthesis, then there are mismatched parentheses.
                if(!pe)  {
                    //printf("Error: parentheses mismatched\n");
                    err = "Error: parentheses mismatched";
                    return false;
                }
                // Pop the left parenthesis from the stack, but not onto the output queue.
                sl--;
                // If the token at the top of the stack is a function token, pop it onto the output queue.
                if(sl > 0)   {
                    sc = stack[sl - 1];
                    if(is_function(sc)) {
                        return false; // Unknown token
                    }
                }
            }
            else  {
                //printf("Unknown token %c\n", c);
                err = "Unknown token "; 
                err = err + c;
                return false; // Unknown token
            }
        }
        ++strpos;
    }
    // When there are no more tokens to read:
    // While there are still operator tokens in the stack:
    while(sl > 0)  {
        sc = stack[sl - 1];
        if(sc == '(' || sc == ')')   {
            //printf("Error: parentheses mismatched\n");
            err = "Error: parentheses mismatched";
            return false;
        }
        *outpos = sc;
        ++outpos;
        --sl;
    }
    *outpos = 0; // Null terminator
    return true;
}

int BoolExpr::op_preced(const char c)
{
    switch(c)    {
        case '|':
            return 6;
        case '&':
            return 5;
        case '!':
            return 4;
    }
    return 0;
}

bool BoolExpr::op_left_assoc(const char c)
{
    switch(c)    {
        // left to right
        case '|':
        case '&':
            return true;
        // right to left
        case '!':
            return false;
    }
    return false;
}


