template <class T>
void iGradient<T>::clear()
{
    _values.clear();
}

/* template <typename T>
bool smaller(const pair<float, T>& a, const pair<float, T>& b)
{
return a.first < b.first;
}*/

template <class T>
void iGradient<T>::insertValue(float at, const T& value)
{
    pair<float, T> temp;

    temp.first = at;
    temp.second = value;

    _values.push_back(temp);

    // TODO sort later    sort(_values.begin(), _values.end(), smaller);
}

template <class T>
const vector<pair<float, T>>& iGradient<T>::getValues() const
{
    return _values;
}

template <class T>
void iGradient<T>::getValue(float at, T& value) const
{
    if (at >= _values[_values.size()-1].first)
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
