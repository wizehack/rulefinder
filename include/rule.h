#ifndef RULE_H_
#define RULE_H_

#include <string>
#include <vector>

#define BOOLEANEXPR "BOOLEANEXPR"
#define SEQUENCIAL "SEQUENCIAL"

class OneShotRule {
    public:
        OneShotRule(std::string id, std::vector<std::string> filter):
            m_id(id),
            m_str(),
            m_filter(filter),
            m_bMatched(false){}
        virtual ~OneShotRule(){}

        void setMatched(bool b){
            m_bMatched = b;
        }

        void setData(std::string s) {
            m_str = s;
        }

        bool isMatched(){
            return m_bMatched;
        }

        std::string getId() {
            return m_id;
        }

        std::vector<std::string> getFilter() {
            return m_filter;
        }

        std::string getData() {
            return m_str;
        }

    private:
        std::string m_id;
        std::string m_str;
        std::vector<std::string> m_filter;
        bool m_bMatched;

        OneShotRule();
};

class CompositeRule {
    public:
        CompositeRule(std::string id, std::string ruleType):
            m_id(id),
            m_ruleType(ruleType),
            m_condition(),
            m_sequence(),
            m_bMatched(false){}
        virtual ~CompositeRule(){}

        void setCondition(std::string condition) {
            if(BOOLEANEXPR == m_ruleType)
            {
                m_condition = condition;
            }
        }

        void setSequence(std::vector<std::string> sequence) {
            if(SEQUENCIAL == m_ruleType)
            {
                m_sequence = sequence;
            }
        }

        void setMatched(bool b){
            m_bMatched = b;
        }

        std::string getId() {
            return m_id;
        }

        bool isMatched(){
            return m_bMatched;
        }

        std::string getRuleType() {
            return m_ruleType;
        }

        std::string getCondition() {
            return m_condition;
        }

        std::vector<std::string> getSequence() {
            return m_sequence;
        }

    private:
        std::string m_id;
        std::string m_ruleType;
        std::string m_condition;
        std::vector<std::string> m_sequence;
        bool m_bMatched;

        CompositeRule();
};

class Report {
    public:
        Report():
            m_oneshotRuleVec(),
            m_compRuleVec(){}
        virtual ~Report(){}
        void setOneshutRuleVec(std::vector<std::shared_ptr<OneShotRule>> v) {
            m_oneshotRuleVec = v;
        }

        void setCompRuleVec(std::vector<std::shared_ptr<CompositeRule>> v) {
            m_compRuleVec = v;
        }

        std::vector<std::shared_ptr<OneShotRule>> getOneshotRuleVec() {
            return m_oneshotRuleVec;
        }

        std::vector<std::shared_ptr<CompositeRule>> getCompRuleVec() {
            return m_compRuleVec;
        }

    private:
        std::vector<std::shared_ptr<OneShotRule>> m_oneshotRuleVec;
        std::vector<std::shared_ptr<CompositeRule>> m_compRuleVec;
};

class ConfData {
    public:
        ConfData():
            m_logFile(),
            m_oneshotRuleFile(),
            m_compositeRuleFile(),
            m_report(){}
        virtual ~ConfData(){}

        void setLogFile(std::string filename) {
            m_logFile = filename;
        }

        void setOneshotRuleFile(std::string filename) {
            m_oneshotRuleFile = filename;
        }

        void setCompositeRuleFile(std::string filename) {
            m_compositeRuleFile = filename;
        }

        void setReport(std::shared_ptr<Report> report) {
            m_report = report;
        }

        std::string getLogFile() {
            return m_logFile;
        }

        std::string getOneshotRuleFile() {
            return m_oneshotRuleFile;
        }

        std::string getCompositeRuleFile() {
            return m_compositeRuleFile;
        }

        std::shared_ptr<Report> getReport() {
            return m_report;
        }

    private:
        std::string m_logFile;
        std::string m_oneshotRuleFile;
        std::string m_compositeRuleFile;
        std::shared_ptr<Report> m_report;
};

#endif /*RULE_H_*/
