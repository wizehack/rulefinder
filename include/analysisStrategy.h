#ifndef LOG_ANALYSIS_H_
#define LOG_ANALYSIS_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "rule.h"

class LogAnalysisStrategy {
    public:
        LogAnalysisStrategy(){}
        virtual ~LogAnalysisStrategy(){}
        virtual std::shared_ptr<Report> execute() = 0;
        virtual void setData(ConfData* conf) = 0;
};

class Context {
    public:
        Context(LogAnalysisStrategy* strategy):
            m_strategy(strategy){}
        virtual ~Context() {
            if(m_strategy)
            {
                //std::cout << __PRETTY_FUNCTION__ << std::endl;
                delete m_strategy;
            }
        }

        bool analyze(ConfData* conf) {
            if(m_strategy && conf)
            {
                std::shared_ptr<Report> report;
                m_strategy->setData(conf);

                report = m_strategy->execute();
                conf->setReport(report);
                return true;
            }

            return false;
        }

    private:
        LogAnalysisStrategy* m_strategy;

    protected:
        Context(const Context&);
        Context& operator=(const Context&);
};

#endif /*LOG_ANALYSIS_H_*/
