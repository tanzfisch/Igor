#include <igor/entities/iEntityComponent.h>

namespace igor
{
	std::vector<iComponentMetrics> *iBaseComponent::_componentTypes;

	iEntityComponentID iBaseComponent::registerComponentType(iComponentCreateFunction createfn, iComponentFreeFunction freefn, size_t size)
	{
		if (_componentTypes == nullptr)
		{
			_componentTypes = new std::vector<iComponentMetrics>();
		}

		iEntityComponentID componentID = _componentTypes->size();
		iComponentMetrics componentMetrics = {createfn, freefn, size};
		_componentTypes->push_back(componentMetrics);
		return componentID;
	}
}