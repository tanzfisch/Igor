#include <igor/entities/iEntityComponentSystem.h>

#include <cstring>

namespace igor
{

	iEntityComponentSystem::~iEntityComponentSystem()
	{
		for (std::map<uint32, std::vector<uint8>>::iterator it = _components.begin(); it != _components.end(); ++it)
		{
			size_t typeSize = iBaseComponent::getTypeSize(it->first);
			iComponentFreeFunction freefn = iBaseComponent::getTypeFreeFunction(it->first);
			for (uint32 i = 0; i < it->second.size(); i += typeSize)
			{
				freefn((iBaseComponent *)&it->second[i]);
			}
		}

		for (uint32 i = 0; i < _entities.size(); i++)
		{
			delete _entities[i];
		}
	}

	iEntityPtr iEntityComponentSystem::makeEntity(iBaseComponent **components, const iComponentID *componentIDs, size_t numComponents)
	{
		iEntityPtr entity = new iEntity();
		for (uint32 i = 0; i < numComponents; i++)
		{
			if (!iBaseComponent::isTypeValid(componentIDs[i]))
			{
				con_err(componentIDs[i] << " is not a valid component type.");
				delete entity;
				return nullptr;
			}

			addComponentInternal(entity, components[i], componentIDs[i]);
		}

		entity->first = _entities.size();
		_entities.push_back(entity);
		return entity;
	}

	void iEntityComponentSystem::removeEntity(iEntityPtr entity)
	{
		iComponentData &componentData = getComponentData(entity);
		for (uint32 i = 0; i < componentData.size(); i++)
		{
			deleteComponent(entity[i].first, componentData[i].second);
		}

		uint32 destIndex = getEntityIndex(entity);
		uint32 srcIndex = _entities.size() - 1;
		delete _entities[destIndex];
		_entities[destIndex] = _entities[srcIndex];
		_entities.pop_back();
	}

	void iEntityComponentSystem::addComponentInternal(iEntityPtr entity, iBaseComponent *component, uint32 componentID)
	{
		iComponentCreateFunction createfn = iBaseComponent::getTypeCreateFunction(componentID);
		std::pair<uint32, uint32> newPair = {componentID, createfn(_components[componentID], entity, component)};
		getComponentData(entity).emplace_back(newPair);
	}

	void iEntityComponentSystem::deleteComponent(uint32 componentID, uint32 index)
	{
		std::vector<uint8> &array = _components[componentID];
		iComponentFreeFunction freefn = iBaseComponent::getTypeFreeFunction(componentID);
		size_t typeSize = iBaseComponent::getTypeSize(componentID);
		uint32 srcIndex = array.size() - typeSize;

		iBaseComponent *destComponent = (iBaseComponent *)&array[index];
		iBaseComponent *srcComponent = (iBaseComponent *)&array[srcIndex];
		freefn(destComponent);

		if (index == srcIndex)
		{
			array.resize(srcIndex);
			return;
		}

		memcpy(destComponent, srcComponent, typeSize);

		iComponentData &componentData = getComponentData(srcComponent->_entity);
		for (uint32 i = 0; i < componentData.size(); i++)
		{
			if (componentID == componentData[i].first && srcIndex == componentData[i].second)
			{
				componentData[i].second = index;
				break;
			}
		}

		array.resize(srcIndex);
	}

	bool iEntityComponentSystem::removeComponentInternal(iEntityPtr entity, uint32 componentID)
	{
		iComponentData &componentData = getComponentData(entity);
		for (uint32 i = 0; i < componentData.size(); i++)
		{
			if (componentID == componentData[i].first)
			{
				deleteComponent(componentData[i].first, componentData[i].second);
				uint32 srcIndex = componentData.size() - 1;
				uint32 destIndex = i;
				componentData[destIndex] = componentData[srcIndex];
				componentData.pop_back();
				return true;
			}
		}
		return false;
	}

	iBaseComponent *iEntityComponentSystem::getComponentInternal(iComponentData &componentData, std::vector<uint8> &componentRawData, uint32 componentID)
	{
		for (uint32 i = 0; i < componentData.size(); i++)
		{
			if (componentID == componentData[i].first)
			{
				return (iBaseComponent *)&componentRawData[componentData[i].second];
			}
		}
		return nullptr;
	}

	void iEntityComponentSystem::updateSystems(iEntitySystemList &systems)
	{
		std::vector<iBaseComponent *> componentParam;
		std::vector<std::vector<uint8> *> componentArrays;
		for (uint32 i = 0; i < systems.size(); i++)
		{
			const std::vector<uint32> &componentTypes = systems[i]->getComponentTypes();
			if (componentTypes.size() == 1)
			{
				size_t typeSize = iBaseComponent::getTypeSize(componentTypes[0]);
				std::vector<uint8> &componentRawData = _components[componentTypes[0]];
				for (uint32 j = 0; j < componentRawData.size(); j += typeSize)
				{
					iBaseComponent *component = (iBaseComponent *)&componentRawData[j];
					systems[i]->updateComponents(&component);
				}
			}
			else
			{
				updateSystemWithMultipleComponents(i, systems, componentTypes, componentParam, componentArrays);
			}
		}
	}

	uint32 iEntityComponentSystem::findLeastCommonComponent(const std::vector<uint32> &componentTypes, const std::vector<uint32> &componentFlags)
	{
		uint32 minSize = (uint32)-1;
		uint32 minIndex = (uint32)-1;
		for (uint32 i = 0; i < componentTypes.size(); i++)
		{
			if ((componentFlags[i] & static_cast<uint32>(iComponentFlag::Optional)) != 0)
			{
				continue;
			}
			size_t typeSize = iBaseComponent::getTypeSize(componentTypes[i]);
			uint32 size = _components[componentTypes[i]].size() / typeSize;
			if (size <= minSize)
			{
				minSize = size;
				minIndex = i;
			}
		}

		return minIndex;
	}

	void iEntityComponentSystem::updateSystemWithMultipleComponents(uint32 index, iEntitySystemList &systems, const std::vector<uint32> &componentTypes, std::vector<iBaseComponent *> &componentParam, std::vector<std::vector<uint8> *> &componentArrays)
	{
		const std::vector<uint32> &componentFlags = systems[index]->getComponentFlags();

		componentParam.resize(std::max(componentParam.size(), componentTypes.size()));
		componentArrays.resize(std::max(componentArrays.size(), componentTypes.size()));
		for (uint32 i = 0; i < componentTypes.size(); i++)
		{
			componentArrays[i] = &_components[componentTypes[i]];
		}
		uint32 minSizeIndex = findLeastCommonComponent(componentTypes, componentFlags);

		size_t typeSize = iBaseComponent::getTypeSize(componentTypes[minSizeIndex]);
		std::vector<uint8> &array = *componentArrays[minSizeIndex];
		for (uint32 i = 0; i < array.size(); i += typeSize)
		{
			componentParam[minSizeIndex] = (iBaseComponent *)&array[i];
			iComponentData &componentData = getComponentData(componentParam[minSizeIndex]->_entity);

			bool isValid = true;
			for (uint32 j = 0; j < componentTypes.size(); j++)
			{
				if (j == minSizeIndex)
				{
					continue;
				}

				componentParam[j] = getComponentInternal(componentData, *componentArrays[j], componentTypes[j]);
				if (componentParam[j] == nullptr && (componentFlags[j] & static_cast<uint32>(iComponentFlag::Optional)) == 0)
				{
					isValid = false;
					break;
				}
			}

			if (isValid)
			{
				systems[index]->updateComponents(&componentParam[0]);
			}
		}
	}
}