template <class T>
void iGradient<T>::clear()
{
    _values.clear();
}

template <class T>
void iGradient<T>::setValue(float at, const T& value)
{
    if (_values.size() == 0)
    {
        _values.push_back(pair<float, T>(at, value));
        return;
    }

    auto iter = _values.begin();

    while (iter != _values.end())
    {
        if ((*iter).first == at)
        {
            (*iter).second = value;
            return;
        }

        if ((*iter).first > at)
        {
            _values.insert(iter, pair<float, T>(at, value));
            return;
        }

        iter++;
    }

    _values.push_back(pair<float, T>(at, value));
}

template <class T>
const vector<pair<float, T>>& iGradient<T>::getValues() const
{
    return _values;
}

template <class T>
void iGradient<T>::getValue(float at, T& value) const
{
    if (at >= _values[_values.size() - 1].first)
    {
        value = _values[_values.size() - 1].second;
    }
    else if (at <= _values[0].first)
    {
        value = _values[0].second;
    }
    else
    {
        value = _values[0].second;

        for (int i = 0; i < _values.size(); ++i)
        {
            if (_values[i].first > at)
            {
                float t = (at - _values[i - 1].first) / (_values[i].first - _values[i - 1].first);
                value = _values[i - 1].second;
                value *= (1.0f - t);
                T b = _values[i].second;
                b *= t;
                value += b;

                break;
            }
        }
    }
}
