// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
void iaKeyFrameGraph<T>::clear()
{
    _values.clear();
}

template <class T>
__IGOR_INLINE__ bool iaKeyFrameGraph<T>::isEmpty() const
{
    return _values.empty();
}

template <class T>
void iaKeyFrameGraph<T>::setValue(float64 at, const T &value)
{
    if (_values.size() == 0)
    {
        _values.push_back(std::pair<float64, T>(at, value));
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
void iaKeyFrameGraph<T>::removeIndex(int32 index)
{
    if (index < _values.size())
    {
        _values.erase(_values.begin() + index);
    }
}

template <class T>
void iaKeyFrameGraph<T>::setValueAtIndex(int32 index, float64 at, const T &value)
{
    con_assert(index < _values.size(), "out of bounds");

    _values[index].first = at;
    _values[index].second = value;

    std::sort(_values.begin(), _values.end(), [](std::pair<float64, T> const &a, std::pair<float64, T> const &b)
              { return a.first < b.first; });
}

template <class T>
void iaKeyFrameGraph<T>::getValueAtIndex(int32 index, float64 &at, T &value)
{
    con_assert(index < _values.size(), "out of bounds");

    at = _values[index].first;
    value = _values[index].second;
}

template <class T>
const std::vector<std::pair<float64, T>> &iaKeyFrameGraph<T>::getValues() const
{
    return _values;
}

template <class T>
uint32 iaKeyFrameGraph<T>::getValueCount() const
{
    return _values.size();
}

template <class T>
T iaKeyFrameGraph<T>::getValue(float64 at) const
{
    T result;
    getValue(at, result);

    return result;
}

template <class T>
void iaKeyFrameGraph<T>::getValue(float64 at, T &value) const
{
    if (at <= _values.front().first)
    {
        value = _values.front().second;
        return;
    }

    if (at >= _values.back().first)
    {
        value = _values.back().second;
        return;
    }

    for (int i = 0; i < _values.size(); ++i)
    {
        const float64 &right = _values[i].first;
        if (at > right)
        {
            continue;
        }

        const float64 &left = _values[i - 1].first;

        const float64 t = (at - left) / (right - left);
        value = iaMath::lerp(_values[i - 1].second, _values[i].second, t);
        return;
    }
}
