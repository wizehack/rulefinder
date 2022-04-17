#ifndef STUB_H_
#define STUB_H_

#include <iostream>
#include <thread>
#include <mutex>

template <class T>
class Stub {
    protected:
        Stub() {};
        virtual ~Stub() {};
        Stub(const Stub& other) {};

    private:
        static std::once_flag _chk;
        static std::shared_ptr<T> _inst;
        static void destroy() {
            if (_inst != nullptr) {
                _inst.reset();
                _inst = nullptr;
            }
        }

    public:
        static T& _getInstance(){
            std::call_once(_chk, [](){
                    _inst.reset(new T());
                    std::atexit(destroy);
                    });
            return *(_inst.get());
        }
};

template <class T> std::shared_ptr<T> Stub <T>::_inst = nullptr;
template <class T> std::once_flag Stub <T>::_chk;

class OneshotRuleStub final : public Stub<OneshotRuleStub> {
    public:
        OneshotRuleStub():m_oRule(){}
        ~OneshotRuleStub(){}
        void init() {
            m_oRule.clear();
        }
        void setRule(std::vector<std::shared_ptr<OneShotRule>> rule) {
            m_oRule = rule;
        }
        std::vector<std::shared_ptr<OneShotRule>> getRule() {
            return m_oRule;
        }

    private:
        std::vector<std::shared_ptr<OneShotRule>> m_oRule;
};

class CompositeRuleStub final : public Stub<CompositeRuleStub> {
    public:
        CompositeRuleStub():m_cRule(){}
        ~CompositeRuleStub(){}
        void init() {
            m_cRule.clear();
        }
        void setRule(std::vector<std::shared_ptr<CompositeRule>> rule) {
            m_cRule = rule;
        }
        std::vector<std::shared_ptr<CompositeRule>> getRule() {
            return m_cRule;
        }

    private:
        std::vector<std::shared_ptr<CompositeRule>> m_cRule;
};

#endif
