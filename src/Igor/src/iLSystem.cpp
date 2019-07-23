// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iLSystem.h>

namespace Igor
{

    void iLSystem::setRule(wchar_t input, iaString output)
    {
        iLSystemRule rule;
        rule._output.push_back(std::pair<float64, iaString>(1.0, output));

        _rules[input] = rule;
    }

    void iLSystem::clearRule(wchar_t input)
    {
        auto iter = _rules.find(input);
        if (iter != _rules.end())
        {
            _rules.erase(iter);
        }
    }

    void iLSystem::clear()
    {
        _rules.clear();
    }

    void iLSystem::setRule(wchar_t input, std::vector<std::pair<float64, iaString>> output)
    {
        iLSystemRule rule;
        rule._output = output;
        _rules[input] = rule;
    }

    void iLSystem::setAgeFilter(wchar_t input, iLSystemAgeFunction ageFunction, int32 ageFilter)
    {
        auto iter = _rules.find(input);
        if (iter != _rules.end())
        {
            (*iter).second._ageFilter = ageFilter;
            (*iter).second._ageFunction = ageFunction;
        }
    }

    iaString iLSystem::generate(iaString in, int32 iterations, uint32 seed)
    {
        _rand.setSeed(seed);

        iaString sentence = in;
        
        for (int i = 0; i < iterations; ++i)
        {
            sentence = generateIteration(sentence, i);
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
            if (iter != _rules.end())
            {
                bool validAge = true;
                if ((*iter).second._ageFunction != iLSystemAgeFunction::None)
                {
                    switch ((*iter).second._ageFunction)
                    {
                    case iLSystemAgeFunction::Less:
                        if (age >= (*iter).second._ageFilter - 1)
                        {
                            validAge = false;
                        }
                        break;

                    case iLSystemAgeFunction::LessOrEqual:
                        if (age > (*iter).second._ageFilter - 1)
                        {
                            validAge = false;
                        }
                        break;

                    case iLSystemAgeFunction::Equal:
                        if (age != (*iter).second._ageFilter - 1)
                        {
                            validAge = false;
                        }
                        break;

                    case iLSystemAgeFunction::GreaterOrEqual:
                        if (age < (*iter).second._ageFilter - 1)
                        {
                            validAge = false;
                        }
                        break;

                    case iLSystemAgeFunction::Greater:
                        if (age <= (*iter).second._ageFilter - 1)
                        {
                            validAge = false;
                        }
                        break;
                    }
                }

                if (validAge)
                {
                    if ((*iter).second._output.size() > 1)
                    {
                        float32 value = _rand.getNext() % 100 / 100.0f;
                        float32 threashold = 0.0f;
                        for (auto iter : (*iter).second._output)
                        {
                            threashold += iter.first;
                            if (value < threashold)
                            {
                                result += iter.second;
                                break;
                            }
                        }

                        skiped = false;
                    }
                    else
                    {
                        result += (*iter).second._output[0].second;

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
}