// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
iaFlushVector<T>::iaFlushVector()
{
}

template <class T>
iaFlushVector<T>::~iaFlushVector()
{
	_queue.clear();
	_data.clear();
}

template <class T>
vector<T>& iaFlushVector<T>::getList()
{
	return _data;
}

template <class T>
void iaFlushVector<T>::add(T element)
{
	_queue.push_back(pair<T, bool>(element, true));
}

template <class T>
void iaFlushVector<T>::remove(T element)
{
	_queue.push_back(pair<T, bool>(element, false));
}

template <class T>
void iaFlushVector<T>::flush()
{
	auto iter = _queue.begin();

	while (iter != _queue.end())
	{
		auto found = find(_data.begin(), _data.end(), (*iter).first);

		if ((*iter).second)
		{
			if (found == _data.end())
			{
				_data.push_back((*iter).first);
			}
            else
            {
                con_err("element alredy added");
            }
		}
		else
		{
			if (found != _data.end())
			{
				_data.erase(found);
			}
            else
            {
                con_err("element to remove not found");
            }
		}

		iter++;
	}

	_queue.clear();
}
