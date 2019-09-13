// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

__IGOR_INLINE__ iWidgetPtr iWidgetManager::getWidget(uint64 id)
{
	auto iter = _widgets.find(id);

	if (iter != _widgets.end())
	{
		return (*iter).second;
	}
	return nullptr;
}
