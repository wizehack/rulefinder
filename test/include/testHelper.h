#ifndef HELPER_H_
#define HELPER_H_

#include <iostream>
#include <memory>
#include "rule.h"

class Helper {
    public:
        Helper(){}
        virtual ~Helper(){}
        virtual void setUp()=0;
        virtual bool test()=0;

    protected:
        virtual void print(bool b) {
            if(false == b)
            {
                std::cout << '\t' << "[FAILED]" << std::endl;
            }
            std::cout << '\t' << "[SUCCESS]" << std::endl;
        }
};

class OneshotRuleTestHelper : public Helper{
    public:
        OneshotRuleTestHelper();
        virtual ~OneshotRuleTestHelper();
        void setUp();
        bool test();

    private:
        bool find_matched_single_rule(std::string logfile, std::string rulefile, std::string matchedId);
        bool find_matched_multi_rules(std::string logfile, std::string rulefile, std::vector<std::string> ids);

        std::string m_rootdir;
        std::vector<std::shared_ptr<OneShotRule>> m_ruleVec;
};

class CompositeRuleTestHelper : public Helper{
    public:
        CompositeRuleTestHelper();
        virtual ~CompositeRuleTestHelper();
        void setUp();
        bool test();

    private:
        std::shared_ptr<OneShotRule> createOneshotRule(std::string id, std::string filter, bool bMatched);
        std::shared_ptr<CompositeRule> createRegexCompositeRule(std::string id, std::string ruleType, std::string regex);
        std::shared_ptr<CompositeRule> createSeqCompositeRule(std::string id, std::string ruleType, std::vector<std::string> seq);
        bool find_regexpr_rule(std::string ruleId);
        bool find_circular_rule(std::string ruleId);

        std::string m_rootdir;
        std::vector<std::shared_ptr<OneShotRule>> m_oneShotVec;
        std::shared_ptr<ConfData> m_confData;
};

class BoolExprTestHelper : public Helper{
    public:
        BoolExprTestHelper();
        ~BoolExprTestHelper();

        void setUp();
        bool test();

    private:
        bool test_true_return();
        bool test_false_return();
        bool test_error_return();
        std::vector<std::string> m_expr;
};
#endif /*HELPER_H_*/
