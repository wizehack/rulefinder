#include "testHelper.h"
#include "analysisStrategy.h"
#include "ruleBasedLogAnalysis.h"
#include "driver.h"

#define DUMMY_ONESHOT_1 "dummy_oneshot_1"
#define DUMMY_ONESHOT_2 "dummy_oneshot_2"
#define DUMMY_ONESHOT_3 "dummy_oneshot_3"
#define DUMMY_ONESHOT_4 "dummy_oneshot_4"
#define DUMMY_ONESHOT_5 "dummy_oneshot_5"
#define DUMMY_ONESHOT_6 "dummy_oneshot_6"
#define DUMMY_ONESHOT_7 "dummy_oneshot_7"

#define ID_COMPOSITE_BOOLEXPR "dummy_composite_boolexpr"
#define COND_BOOLEXPR "dummy_oneshot_1 and (dummy_oneshot_3 or dummy_oneshot_6) and not dummy_oneshot_4"
#define ID_COMPOSITE_SEQ "dummy_composite_seq"

CompositeRuleTestHelper::CompositeRuleTestHelper():
    m_rootdir(),
    m_oneShotVec(),
    m_confData(){}
CompositeRuleTestHelper::~CompositeRuleTestHelper(){}

void CompositeRuleTestHelper::setUp()
{
    std::shared_ptr<Report> report = std::make_shared<Report>();
    std::shared_ptr<OneShotRule> rule1 = this->createOneshotRule(DUMMY_ONESHOT_1,"(.*)(dymmy 1)(.*)", true);
    std::shared_ptr<OneShotRule> rule2 = this->createOneshotRule(DUMMY_ONESHOT_2,"(.*)(dymmy 2)(.*)", true);
    std::shared_ptr<OneShotRule> rule3 = this->createOneshotRule(DUMMY_ONESHOT_3,"(.*)(dymmy 3)(.*)", false);
    std::shared_ptr<OneShotRule> rule4 = this->createOneshotRule(DUMMY_ONESHOT_4,"(.*)(dymmy 4)(.*)", false);
    std::shared_ptr<OneShotRule> rule5 = this->createOneshotRule(DUMMY_ONESHOT_5,"(.*)(dymmy 5)(.*)", false);
    std::shared_ptr<OneShotRule> rule6 = this->createOneshotRule(DUMMY_ONESHOT_6,"(.*)(dymmy 6)(.*)", true);
    std::shared_ptr<OneShotRule> rule7 = this->createOneshotRule(DUMMY_ONESHOT_7,"(.*)(dymmy 7)(.*)", true);

    m_oneShotVec.push_back(rule1);
    m_oneShotVec.push_back(rule2);
    m_oneShotVec.push_back(rule3);
    m_oneShotVec.push_back(rule4);
    m_oneShotVec.push_back(rule5);
    m_oneShotVec.push_back(rule6);
    m_oneShotVec.push_back(rule7);

    report->setOneshutRuleVec(m_oneShotVec);

    m_confData = std::make_shared<ConfData>();
    m_confData->setReport(report);

    std::shared_ptr<CompositeRule> compRuleBoolExpr = this->createRegexCompositeRule(ID_COMPOSITE_BOOLEXPR, BOOLEANEXPR, COND_BOOLEXPR);

    std::vector<std::string> sequence;

    sequence.push_back("dummy_oneshot_1");
    sequence.push_back("dummy_oneshot_2");
    sequence.push_back("dummy_oneshot_6");
    sequence.push_back("dummy_oneshot_7");

    std::shared_ptr<CompositeRule> compRuleSeq = this->createSeqCompositeRule(ID_COMPOSITE_SEQ, SEQUENCIAL, sequence);
    std::vector<std::shared_ptr<CompositeRule>> compRuleVec;

    compRuleVec.push_back(compRuleBoolExpr);
    compRuleVec.push_back(compRuleSeq);

    CompositeRuleStub::_getInstance().setRule(compRuleVec);
}

std::shared_ptr<OneShotRule> CompositeRuleTestHelper::createOneshotRule(std::string id, std::string regex, bool bMatched)
{
    std::vector<std::string> filter;
    filter.push_back(regex);
    std::shared_ptr<OneShotRule> rule = std::make_shared<OneShotRule>(id, filter);
    rule->setMatched(bMatched);

    return rule;
}

std::shared_ptr<CompositeRule> CompositeRuleTestHelper::createRegexCompositeRule(std::string id, std::string ruleType, std::string regex)
{
    std::shared_ptr<CompositeRule> cRule = std::make_shared<CompositeRule>(id, ruleType);
    cRule->setCondition(regex);
    return cRule;
}

std::shared_ptr<CompositeRule> CompositeRuleTestHelper::createSeqCompositeRule(std::string id, std::string ruleType, std::vector<std::string> seq)
{
    std::shared_ptr<CompositeRule> cRule = std::make_shared<CompositeRule>(id, ruleType);
    cRule->setSequence(seq);
    return cRule;
}

bool CompositeRuleTestHelper::test()
{
    std::string logPath = m_rootdir + "/test/res/sdx_send_failed.log";
    std::string compositeRulePath = m_rootdir + "/test/res/compositconf.json";

    m_confData->setCompositeRuleFile(compositeRulePath);

    Context* cont = new Context(new FakeCompositeRuleAnalysis());

    if(cont->analyze(m_confData.get()))
    {
        bool bSuccess = this->find_regexpr_rule(ID_COMPOSITE_BOOLEXPR);
        this->print(bSuccess);

        bSuccess = this->find_circular_rule(ID_COMPOSITE_SEQ);
        this->print(bSuccess);

        delete cont;
        return bSuccess;
    }

    delete cont;
    return false;
}

bool CompositeRuleTestHelper::find_regexpr_rule(std::string ruleId)
{
    std::cout << __FUNCTION__ << " " << ID_COMPOSITE_BOOLEXPR;

    std::shared_ptr<Report> report = m_confData->getReport();
    std::vector<std::shared_ptr<CompositeRule>> ruleVec = report->getCompRuleVec();
    std::vector<std::shared_ptr<CompositeRule>>::iterator cIter;
    cIter = ruleVec.begin();
    if(cIter != ruleVec.end())
    {
        if(ID_COMPOSITE_BOOLEXPR == (*cIter)->getId() && (*cIter)->isMatched())
        {
            return true;
        }
    }

    return false;
}

bool CompositeRuleTestHelper::find_circular_rule(std::string ruleId)
{
    std::cout << __FUNCTION__ << " " << ID_COMPOSITE_BOOLEXPR;
    std::shared_ptr<Report> report = m_confData->getReport();
    std::vector<std::shared_ptr<CompositeRule>> ruleVec = report->getCompRuleVec();
    std::vector<std::shared_ptr<CompositeRule>>::iterator cIter;

    for(cIter = ruleVec.begin(); cIter != ruleVec.end(); ++cIter)
    {
        if(ID_COMPOSITE_SEQ == (*cIter)->getId())
        {
            return true;
        }
    }
    return false;
}
