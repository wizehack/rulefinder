#include "analysisStrategy.h"
#include "rule.h"
#include "ruleBasedLogAnalysis.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>

OneshotRuleAnalysis::OneshotRuleAnalysis():
    m_ruleVector(),
    m_logFileName(),
    m_ruleFileName(){}
OneshotRuleAnalysis::~OneshotRuleAnalysis(){}

void OneshotRuleAnalysis::setData(ConfData* conf)
{
    m_logFileName = conf->getLogFile();
    m_ruleFileName = conf->getOneshotRuleFile();

    this->loadRule(m_ruleFileName);
}

std::shared_ptr<Report> OneshotRuleAnalysis::execute()
{
    std::shared_ptr<Report> report = nullptr;
    std::ifstream logFile(m_logFileName);
    std::string line;
    std::vector<std::shared_ptr<OneShotRule>> v;

    if(false == logFile.is_open())
    {
        std::cerr <<__PRETTY_FUNCTION__ << " " << m_logFileName << ": open error"  << std::endl;
        return report;
    }

    while(std::getline(logFile, line))
    {
        std::vector<std::shared_ptr<OneShotRule>>::iterator iter;

        for (iter = m_ruleVector.begin(); iter != m_ruleVector.end(); ++iter)
        {
            std::string id = (*iter)->getId();
            std::vector<std::string> fv = (*iter)->getFilter();
            std::vector<std::string>::iterator iterFv;

            if(false == (*iter)->isMatched())
            {
                bool b = false;

                for(iterFv = fv.begin(); iterFv != fv.end(); ++iterFv)
                {
                    std::string filter = (*iterFv);
                    std::regex expr(filter);
                    b = regex_match(line, expr);
                    //std::smatch match;
                    //b = regex_search(line, match, expr);
                    //std::cout << line << " " << filter << " : " << b << std::endl;

                    if(false == b)
                    {
                        //std::cout << __PRETTY_FUNCTION__ << " NOT matched " << std::endl;
                        break;
                    }
                }

                //std::cout << __PRETTY_FUNCTION__ << " is matched: " << b << std::endl;
                //(*iter)->setMatched(b);

                if(true == b)
                {
                    std::shared_ptr<OneShotRule> rule = std::make_shared<OneShotRule>(id, fv);
                    rule->setData(line);
                    rule->setMatched(true);
                    v.push_back(rule);
                }
            }
        }

        report = std::make_shared<Report>();
        report->setOneshutRuleVec(v);
    }

    logFile.close();

    return report;
}

void OneshotRuleAnalysis::loadRule(std::string rulePath)
{
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
}
