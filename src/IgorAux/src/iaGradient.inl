template <class T>
void iaGradient<T>::clear()
{
    _values.clear();
}

template <class T>
void iaGradient<T>::setValue(float at, const T& value)
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
void iaGradient<T>::removeIndex(int32 index)
{
    if (index < _values.size())
    {
        _values.erase(_values.begin() + index);
    }
}

template <class T>
void iaGradient<T>::setValueAtIndex(int32 index, const T& value)
{
    if (index < _values.size())
    {
        _values[index] = pair<float, T>(_values[index].first, value);
    }
}

template <class T>
void iaGradient<T>::getValueAtIndex(int32 index, float& at, T& value)
{
    if (index < _values.size())
    {
        at = _values[index].first;
        value = _values[index].second;
    }
}

template <class T>
const vector<pair<float, T>>& iaGradient<T>::getValues() const
{
    return _values;
}

template <class T>
void iaGradient<T>::getValue(float at, T& value) const
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