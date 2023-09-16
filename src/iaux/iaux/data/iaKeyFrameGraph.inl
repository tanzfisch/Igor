// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <typename T>
void iaKeyFrameGraph<T>::clear()
{
    _values.clear();
}

template <typename T>
bool iaKeyFrameGraph<T>::isEmpty() const
{
    return _values.empty();
}

template <typename T>
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

template <typename T>
void iaKeyFrameGraph<T>::removeIndex(int32 index)
{
    if (index < _values.size())
    {
        _values.erase(_values.begin() + index);
    }
}

template <typename T>
void iaKeyFrameGraph<T>::setValueAtIndex(int32 index, float64 at, const T &value)
{
    con_assert(index < _values.size(), "out of bounds");

    _values[index].first = at;
    _values[index].second = value;

    std::sort(_values.begin(), _values.end(), [](std::pair<float64, T> const &a, std::pair<float64, T> const &b)
              { return a.first < b.first; });
}

template <typename T>
void iaKeyFrameGraph<T>::getValueAtIndex(int32 index, float64 &at, T &value)
{
    con_assert(index < _values.size(), "out of bounds");

    at = _values[index].first;
    value = _values[index].second;
}

template <typename T>
const std::vector<std::pair<float64, T>> &iaKeyFrameGraph<T>::getValues() const
{
    return _values;
}

template <typename T>
uint32 iaKeyFrameGraph<T>::getValueCount() const
{
    return _values.size();
}

template <typename T>
void iaKeyFrameGraph<T>::setInterpolationMode(iInterpolationMode mode)
{
    _interpolationMode = mode;
}

template <typename T>
iInterpolationMode iaKeyFrameGraph<T>::getInterpolationMode() const
{
    return _interpolationMode;
}

template <typename T>
T iaKeyFrameGraph<T>::getValue(float64 at) const
{
    if (at <= _values.front().first)
    {
        return _values.front().second;
    }

    if (at >= _values.back().first)
    {
        return _values.back().second;
    }

    for (int i = 0; i < _values.size(); ++i)
    {
        const float64 &right = _values[i].first;
        if (at > right)
        {
            continue;
        }

        switch(_interpolationMode)
        {
            case iInterpolationMode::Linear:
            default:
            {
                const float64 &left = _values[i - 1].first;
                const float64 t = (at - left) / (right - left);
                return iaMath::lerp(_values[i - 1].second, _values[i].second, t);
            }

            case iInterpolationMode::None:
                return _values[i - 1].second;
        };
    }

    con_crit("can't happen");
    return T();
}
