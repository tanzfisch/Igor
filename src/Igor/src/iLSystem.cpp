// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iLSystem.h>

namespace Igor
{

    void iLSystem::addRule(wchar_t in, iaString out)
    {
        vector<pair<float32, iaString>> ruleWeighting;

        ruleWeighting.push_back(pair<float32, iaString>(1.0f, out));

        addRule(in, ruleWeighting);
    }

    void iLSystem::addRule(wchar_t in, vector<pair<float32, iaString>> out)
    {
        _rules[in] = out;
    }

    iaString iLSystem::generate(iaString in, int iterations)
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
                            threashold += iter.first;
                            if (value < threashold)
                            {
                                next += iter.second;
                                break;
                            }
                        }
                    }
                    else
                    {
                        next += (*iter).second[0].second;
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