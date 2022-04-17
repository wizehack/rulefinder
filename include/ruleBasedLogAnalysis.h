#ifndef RULE_BASED_LOG_ANALYSIS_H_
#define RULE_BASED_LOG_ANALYSIS_H_

#include <string>
#include <vector>
#include <memory>
#include "analysisStrategy.h"
#include "rule.h"

class OneshotRuleAnalysis : public LogAnalysisStrategy {
    public:
        OneshotRuleAnalysis();
        virtual ~OneshotRuleAnalysis();

        void setData(ConfData* conf);
        std::shared_ptr<Report> execute();

    protected:
        virtual void loadRule(std::string rulePath); 

        std::vector<std::shared_ptr<OneShotRule>> m_ruleVector;
        std::string m_logFileName;
        std::string m_ruleFileName;
};

class CompositeRuleAnalysis : public LogAnalysisStrategy {
    public:
        CompositeRuleAnalysis();
        virtual ~CompositeRuleAnalysis();

        void setData(ConfData* conf);
        std::shared_ptr<Report> execute();

    protected:
        virtual void loadRule(std::string rulePath); 
        bool replaceAll(std::string& str, const std::string& from, const std::string& to);
        bool convert(std::string& cond);
        bool compareSeqRule(std::vector<std::string> seq);

        std::vector<std::shared_ptr<CompositeRule>> m_compRuleVec;
        std::vector<std::shared_ptr<OneShotRule>> m_oneshotRuleVec;
        std::string m_ruleFileName;
        std::shared_ptr<Report> m_report;
};

#endif /*RULE_BASED_LOG_ANALYSIS_H_*/
