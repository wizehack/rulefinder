#ifndef DRIVER_H_
#define DRIVER_H_

#include "ruleBasedLogAnalysis.h"
#include "stub.h"
#include <iostream>

class FakeOneShotRuleAnalysis : public OneshotRuleAnalysis {
    public:
        FakeOneShotRuleAnalysis(){}
        ~FakeOneShotRuleAnalysis(){}

    protected:
        void loadRule(std::string rulePath) {
            //std::cout << __PRETTY_FUNCTION__ << std::endl;
            m_ruleVector = OneshotRuleStub::_getInstance().getRule();
            /*
            std::vector<std::shared_ptr<OneShotRule>> v = m_ruleVector;
            std::vector<std::shared_ptr<OneShotRule>>::iterator iter;
            for (iter = v.begin(); iter != v.end(); iter++)
            {
                std::cout << (*iter)->getId() << std::endl;
                std::cout << (*iter)->isMatched() << std::endl;
            }
            */
        }
};

class FakeCompositeRuleAnalysis : public CompositeRuleAnalysis {
    public:
        FakeCompositeRuleAnalysis(){}
        ~FakeCompositeRuleAnalysis(){}

    protected:
        void loadRule(std::string rulePath) {
            m_compRuleVec = CompositeRuleStub::_getInstance().getRule();

            /*
            std::vector<std::shared_ptr<CompositeRule>> v = m_compRuleVec;
            std::vector<std::shared_ptr<CompositeRule>>::iterator iter;
            for (iter = v.begin(); iter != v.end(); iter++)
            {
                std::cout << __PRETTY_FUNCTION__ << " " << (*iter)->getId() << std::endl;
                std::cout << __PRETTY_FUNCTION__ << " " << (*iter)->isMatched() << std::endl;
            }
            */
        }
};
#endif /*DRIVER_H_*/
