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

    void iLSystem::addRule(wchar_t input, iaString output, iAgeFunction ageFunction, int32 ageFilter)
    {
        iLSystemRule rule;
        rule._output = output;
        rule._ageFunction = ageFunction;
        rule._ageFilter = ageFilter;
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
        
        for (int age = 0; age < iterations; ++age)
        {
            sentence = generateIteration(sentence, age);
        }

        return sentence;
    }

    iaString iLSystem::generateIteration(iaString sentence, int age)
    {
        iaString result;

        for (int c = 0; c < sentence.getSize(); ++c)
        {
            bool skiped = true;

            auto iter = _rules.find(sentence[c]);
            if (iter != _rules.end() && (*iter).second.size() > 0)
            {
                bool validAge = true;
                if ((*iter).second[0]._ageFunction != iAgeFunction::None)
                {
                    switch ((*iter).second[0]._ageFunction)
                    {
                    case iAgeFunction::Less:
                        if (age >= (*iter).second[0]._ageFilter)
                        {
                            validAge = false;
                        }
                        break;

                    case iAgeFunction::LessOrEqual:
                        if (age > (*iter).second[0]._ageFilter)
                        {
                            validAge = false;
                        }
                        break;

                    case iAgeFunction::Equal:
                        if (age != (*iter).second[0]._ageFilter)
                        {
                            validAge = false;
                        }
                        break;

                    case iAgeFunction::GreaterOrEqual:
                        if (age < (*iter).second[0]._ageFilter)
                        {
                            validAge = false;
                        }
                        break;

                    case iAgeFunction::Greater:
                        if (age <= (*iter).second[0]._ageFilter)
                        {
                            validAge = false;
                        }
                        break;
                    }
                }

                if (validAge)
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
                                result += iter._output;
                                break;
                            }
                        }

                        skiped = false;
                    }
                    else
                    {
                        result += (*iter).second[0]._output;

                        skiped = false;
                    }
                }
            }

            if(skiped)
            {
                result += sentence[c];
            }
        }

        return result;
    }

    void iLSystem::clear()
    {
        _rules.clear();
    }
}