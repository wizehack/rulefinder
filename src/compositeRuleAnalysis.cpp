#include "ruleBasedLogAnalysis.h"
#include "rule.h"
#include "boolEval.h"
#include <iostream>
#include <sstream>
#include <algorithm>

CompositeRuleAnalysis::CompositeRuleAnalysis():
    m_compRuleVec(),
    m_oneshotRuleVec(),
    m_ruleFileName(),
    m_report(){}
CompositeRuleAnalysis::~CompositeRuleAnalysis() {}

void CompositeRuleAnalysis::setData(ConfData* conf)
{
    if(conf)
    {
        m_ruleFileName = conf->getCompositeRuleFile();
        m_report = conf->getReport();
        if(m_report != nullptr)
        {
            m_oneshotRuleVec = m_report->getOneshotRuleVec();
            this->loadRule(m_ruleFileName);
        }
        else
        {
            std::cerr << __PRETTY_FUNCTION__ << " can not load rule" << __LINE__ << std::endl;
        }
    }

}

std::shared_ptr<Report> CompositeRuleAnalysis::execute()
{
    std::vector<std::shared_ptr<CompositeRule>> cVec = m_compRuleVec;
    std::vector<std::shared_ptr<CompositeRule>>::iterator cIter;

    for (cIter = cVec.begin(); cIter != cVec.end(); ++cIter)
    {
        std::string type = (*cIter)->getRuleType();
        if(BOOLEANEXPR == type)
        {
            std::string cond = (*cIter)->getCondition();
            if(this->convert(cond))
            {
                BoolExpr boolean(cond);
                std::string err;
                bool b = boolean.eval(err);
                if(err.empty())
                {
                    (*cIter)->setMatched(b);
                }
            }
        }
        else //SEQUENCIAL
        {
            std::vector<std::string> seq = (*cIter)->getSequence();
            bool b = this->compareSeqRule(seq);
            (*cIter)->setMatched(b);
        }
    }

    m_report->setCompRuleVec(cVec);
    return m_report;
}

void CompositeRuleAnalysis::loadRule(std::string rulePath)
{
}

bool CompositeRuleAnalysis::replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty())
        return false;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }

    return true;
}

bool CompositeRuleAnalysis::convert(std::string& cond)
{
    std::transform(cond.begin(), cond.end(), cond.begin(), ::tolower);
    if (true == (this->replaceAll(cond, " and ", " & ") &&
            this->replaceAll(cond, " or ", " | ") &&
            this->replaceAll(cond, " not ", " ! ") &&
            this->replaceAll(cond, "(", " ( ") &&
            this->replaceAll(cond, ")", " ) ")) )
    {
        std::vector<std::shared_ptr<OneShotRule>> rVec = m_oneshotRuleVec;
        std::vector<std::shared_ptr<OneShotRule>>::iterator rIter;

        for (rIter = rVec.begin(); rIter != rVec.end(); ++rIter)
        {
            std::string id = (*rIter)->getId();
            //std::cout << __PRETTY_FUNCTION__ << " " << id << std::endl;
            //id = " " + id + " ";

            if((*rIter)->isMatched())
            {
                this->replaceAll(cond, id, " 1 ");
            }
            else
            {
                this->replaceAll(cond, id, " 0 ");
            }
        }
        return true;
    }
    return false;
}

bool CompositeRuleAnalysis::compareSeqRule(std::vector<std::string> seq)
{
    std::vector<std::string>::iterator sIter;

    std::vector<std::shared_ptr<OneShotRule>> oVec = m_oneshotRuleVec;
    std::vector<std::shared_ptr<OneShotRule>>::iterator oIter;
    bool bFind = false;

    for (sIter = seq.begin(); ; )
    {
        for (oIter = oVec.begin(); oIter != oVec.end(); ++oIter)
        {
            std::string sId = (*sIter);
            std::string oId = (*oIter)->getId();
            //std::cout << "seq Id: " << sId << " oId: " << oId << std::endl;
            if(sId == oId)
            {
                bFind = true;
                if(sIter == seq.end())
                {
                    //std::cout << " end " << std::endl;
                    return true;
                }
                else
                {
                    ++sIter;
                }
            }
            else
            {
                bFind = false;
                continue;
            }
        }

        if(true == bFind)
        {
            return true;
        }

        break;
    }

    return false;
}
