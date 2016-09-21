// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iLSystem.h>

namespace Igor
{

    void iLSystem::addRule(wchar_t input, iLSystemRule rule)
    {
        vector<iLSystemRule> ruleWeighting;
        ruleWeighting.push_back(rule);
        addRule(input, ruleWeighting);
    }

    void iLSystem::addRule(wchar_t input, iaString output)
    {
        iLSystemRule rule;
        rule._output = output;
        vector<iLSystemRule> ruleWeighting;
        ruleWeighting.push_back(rule);
        addRule(input, ruleWeighting);
    }

    void iLSystem::addRule(wchar_t input, vector<iLSystemRule> rules)
    {
        // TODO check weighting
        _rules[input] = rules;
    }

    iaString iLSystem::generate(iaString in, int32 iterations)
    {
        iaString sentence = in;
        iaString next;

        for (int i = 0; i < iterations; ++i)
        {
            next.clear();

            for (int c = 0; c < sentence.getSize(); ++c)
            {
                auto iter = _rules.find(sentence[c]);
                if (iter != _rules.end())
                {
                    if ((*iter).second.size() > 1)
                    {
                        float32 value = rand() % 100 / 100.0f;
                        float32 threashold = 0.0f;
                        for (auto iter : (*iter).second)
                        {
                            threashold += iter._likelihood;
                            if (value < threashold)
                            {
                                next += iter._output;
                                break;
                            }
                        }
                    }
                    else
                    {
                        next += (*iter).second[0]._output;
                    }
                }
                else
                {
                    next += sentence[c];
                }
            }

            sentence = next;
        }

        return sentence;
    }

    void iLSystem::clear()
    {
        _rules.clear();
    }
}