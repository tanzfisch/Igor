#include <igor/entities/iEntityComponent.h>

namespace igor
{
	std::vector<ComponentMetrics> *BaseComponent::_componentTypes;

	iEntityComponentID BaseComponent::registerComponentType(ComponentCreateFunction createfn, ComponentFreeFunction freefn, size_t size)
	{
		if (_componentTypes == nullptr)
		{
			_componentTypes = new std::vector<ComponentMetrics>();
		}

		iEntityComponentID componentID = _componentTypes->size();
		ComponentMetrics componentMetrics = {createfn, freefn, size};
		_componentTypes->push_back(componentMetrics);
		return componentID;
	}
}