// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
void iaGradient<T>::clear()
{
    _values.clear();
}

template <class T>
__IGOR_INLINE__ bool iaGradient<T>::isEmpty() const
{
    return _values.empty();
}

template <class T>
void iaGradient<T>::setValue(float32 at, const T &value)
{
    if (_values.size() == 0)
    {
        _values.push_back(std::pair<float32, T>(at, value));
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
            _values.insert(iter, std::pair<float32, T>(at, value));
            return;
        }

        iter++;
    }

    _values.push_back(std::pair<float32, T>(at, value));
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
void iaGradient<T>::setValueAtIndex(int32 index, float32 at, const T &value)
{
    con_assert(index < _values.size(), "out of bounds");

    _values[index].first = at;
    _values[index].second = value;

    std::sort(_values.begin(), _values.end(), [](std::pair<float32, T> const &a, std::pair<float32, T> const &b)
              { return a.first < b.first; });
}

template <class T>
void iaGradient<T>::getValueAtIndex(int32 index, float32 &at, T &value)
{
    con_assert(index < _values.size(), "out of bounds");

    at = _values[index].first;
    value = _values[index].second;
}

template <class T>
const std::vector<std::pair<float32, T>> &iaGradient<T>::getValues() const
{
    return _values;
}

template <class T>
uint32 iaGradient<T>::getValueCount() const
{
    return _values.size();
}

template <class T>
T iaGradient<T>::getValue(float32 at) const
{
    T result;
    getValue(at, result);

    return result;
}

template <class T>
void iaGradient<T>::getValue(float32 at, T &value) const
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
                float32 t = (at - _values[i - 1].first) / (_values[i].first - _values[i - 1].first);
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
