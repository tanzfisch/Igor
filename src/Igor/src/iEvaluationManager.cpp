#include <iEvaluationManager.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    iEvaluationManager::~iEvaluationManager()
    {
        for (auto evaluator : _evaluators)
        {
            delete evaluator;
        }

        _evaluators.clear();
    }

    void iEvaluationManager::registerEvaluator(const iEvaluatorPtr evaluator)
    {
        if (std::find(_evaluators.begin(), _evaluators.end(), evaluator) != _evaluators.end())
        {
            con_warn("evaluator already registered");
            return;
        }

        _evaluators.push_back(evaluator);
    }

    void iEvaluationManager::unregisterEvaluator(const iEvaluatorPtr evaluator)
    {
        auto iter = std::find(_evaluators.begin(), _evaluators.end(), evaluator);

        if (iter == _evaluators.end())
        {
            con_warn("evaluator already unregistered");
            return;
        }

        _evaluators.erase(iter);
    }

    void iEvaluationManager::handle()
    {
        for (auto evaluator : _evaluators)
        {
            evaluator->evaluate();
        }
    }

}