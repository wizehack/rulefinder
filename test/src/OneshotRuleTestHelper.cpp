#include <unistd.h>

#include "testHelper.h"
#include "analysisStrategy.h"
#include "ruleBasedLogAnalysis.h"
#include "stub.h"
#include "driver.h"

#define SDX_SEND_FAILED "sdx_send_failed"
#define SDX_LUNA_HANDLE_ERR "sdx_luna_handle_err"
#define SDX_REQ_CN_COUNTRY "sdx_req_cn_country"
#define SDX_SET_CN_COUNTRY "sdx_set_cn_country"
#define SDX_SET_CHECK_WASU "sdx_set_check_wasu"

OneshotRuleTestHelper::OneshotRuleTestHelper():
    m_rootdir(get_current_dir_name()),
    m_ruleVec()
{}
OneshotRuleTestHelper::~OneshotRuleTestHelper(){}

void OneshotRuleTestHelper::setUp()
{
    std::vector<std::string> filter1;
    filter1.push_back("(.*)(sdx.logging)(.*)");
    filter1.push_back("(.*)(message send failed:)(.*)");
    std::shared_ptr<OneShotRule> rule1 = std::make_shared<OneShotRule>(SDX_SEND_FAILED, filter1);

    std::vector<std::string> filter2;
    filter2.push_back("(.*)(user.err)(.*)");
    filter2.push_back("(.*)(sdx)(.*)");
    filter2.push_back("(.*)(default-lib)(.*)");
    filter2.push_back("(.*)(LS_INVALID_HANDLE)(.*)");
    std::shared_ptr<OneShotRule> rule2 = std::make_shared<OneShotRule>(SDX_LUNA_HANDLE_ERR, filter2);

    std::vector<std::string> filter3;
    filter3.push_back("(.*)(sdx.logging)(.*)");
    filter3.push_back("(.*)(set_serviceCountry)(.*)");
    filter3.push_back("(.*)(country_code_alpha_2: CN)(.*)");
    std::shared_ptr<OneShotRule> rule3 = std::make_shared<OneShotRule>(SDX_REQ_CN_COUNTRY, filter3);

    std::vector<std::string> filter4;
    filter4.push_back("(.*)(sdx.logging)(.*)");
    filter4.push_back("(.*)(is_valid)(.*)");
    filter4.push_back("(.*)(countrycode2: CN)(.*)");
    std::shared_ptr<OneShotRule> rule4 = std::make_shared<OneShotRule>(SDX_SET_CN_COUNTRY, filter4);

    std::vector<std::string> filter5;
    filter5.push_back("(.*)(sdx.logging)(.*)");
    filter5.push_back("(.*)(chinaCP transformed_xml: wasu)(.*)");
    std::shared_ptr<OneShotRule> rule5 = std::make_shared<OneShotRule>(SDX_SET_CHECK_WASU, filter5);

    m_ruleVec.push_back(rule1);
    m_ruleVec.push_back(rule2);
    m_ruleVec.push_back(rule3);
    m_ruleVec.push_back(rule4);
    m_ruleVec.push_back(rule5);

    OneshotRuleStub::_getInstance().setRule(m_ruleVec);
}

bool OneshotRuleTestHelper::test()
{
    std::string logPath = m_rootdir + "/test/res/sdx_send_failed.log";
    std::string oneshotRulePath = m_rootdir + "/test/res/oneshotconf.json";
    bool bFindMatchedRule = this->find_matched_single_rule(logPath, oneshotRulePath, SDX_SEND_FAILED);
    this->print(bFindMatchedRule);

    if(false == bFindMatchedRule)
        return false;

    logPath = m_rootdir = "test/res/sdx_ls_invalid_handle.log";
    bFindMatchedRule = this->find_matched_single_rule(logPath, oneshotRulePath, SDX_LUNA_HANDLE_ERR);
    this->print(bFindMatchedRule);

    if(false == bFindMatchedRule)
        return false;

    logPath = m_rootdir = "test/res/invalid_area_opt.log";

    std::vector<std::string> ids;
    ids.push_back(SDX_REQ_CN_COUNTRY);
    ids.push_back(SDX_SET_CN_COUNTRY);
    ids.push_back(SDX_SET_CHECK_WASU);
    bFindMatchedRule = this->find_matched_multi_rules(logPath, oneshotRulePath, ids);

    return bFindMatchedRule;
}

bool OneshotRuleTestHelper::find_matched_single_rule(std::string logfile, std::string rulefile, std::string matchedId)
{
    std::cout << __FUNCTION__ << " " << matchedId;
    std::shared_ptr<Report> report;
    ConfData confData;

    confData.setLogFile(logfile);
    confData.setOneshotRuleFile(""); //dummy data

    Context* cont = new Context(new FakeOneShotRuleAnalysis());

    if(true == cont->analyze(&confData))
    {
        report = confData.getReport();

        if(report == nullptr)
        {
            delete cont;
            return false;
        }

        std::vector<std::shared_ptr<OneShotRule>> rule = report->getOneshotRuleVec();
        std::vector<std::shared_ptr<OneShotRule>>::iterator iter;

        for (iter = rule.begin(); iter != rule.end(); iter++)
        {
            if((*iter)->getId() == matchedId && (*iter)->isMatched())
            {
                delete cont;
                return true;
            }
        }
    }

    delete cont;
    return false;
}


bool OneshotRuleTestHelper::find_matched_multi_rules(std::string logfile, std::string rulefile, std::vector<std::string> ids)
{
    std::vector<std::string>::iterator iter;

    for (iter = ids.begin(); iter != ids.end(); iter++)
    {
        bool b = this->find_matched_single_rule(logfile, rulefile, (*iter));
        if(false == b)
            return false;
        else {
            this->print(b);
        }
    }

    return true;
}
