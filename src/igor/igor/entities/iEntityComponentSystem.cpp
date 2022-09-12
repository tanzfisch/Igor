#include <igor/entities/iEntityComponentSystem.h>

#include <cstring>

namespace igor
{

	iEntityComponentSystem::~iEntityComponentSystem()
	{
		for (std::map<uint32, std::vector<uint8>>::iterator it = _components.begin(); it != _components.end(); ++it)
		{
			size_t typeSize = BaseComponent::getTypeSize(it->first);
			ComponentFreeFunction freefn = BaseComponent::getTypeFreeFunction(it->first);
			for (uint32 i = 0; i < it->second.size(); i += typeSize)
			{
				freefn((BaseComponent *)&it->second[i]);
			}
		}

		for (uint32 i = 0; i < _entities.size(); i++)
		{
			delete _entities[i];
		}
	}

	iEntityHandle iEntityComponentSystem::makeEntity(BaseComponent **entityComponents, const iEntityComponentID *componentIDs, size_t numComponents)
	{
		std::pair<uint32, std::vector<std::pair<uint32, uint32>>> *newEntity = new std::pair<uint32, std::vector<std::pair<uint32, uint32>>>();
		iEntityHandle handle = (iEntityHandle)newEntity;
		for (uint32 i = 0; i < numComponents; i++)
		{
			if (!BaseComponent::isTypeValid(componentIDs[i]))
			{
				con_err(componentIDs[i] << " is not a valid component type.");
				delete newEntity;
				return nullptr;
			}

			addComponentInternal(handle, newEntity->second, componentIDs[i], entityComponents[i]);
		}

		newEntity->first = _entities.size();
		_entities.push_back(newEntity);
		return handle;
	}

	void iEntityComponentSystem::removeEntity(iEntityHandle handle)
	{
		std::vector<std::pair<uint32, uint32>> &entity = handleToEntity(handle);
		for (uint32 i = 0; i < entity.size(); i++)
		{
			deleteComponent(entity[i].first, entity[i].second);
		}

		uint32 destIndex = handleToEntityIndex(handle);
		uint32 srcIndex = _entities.size() - 1;
		delete _entities[destIndex];
		_entities[destIndex] = _entities[srcIndex];
		_entities.pop_back();
	}

	void iEntityComponentSystem::addComponentInternal(iEntityHandle handle, std::vector<std::pair<uint32, uint32>> &entity, uint32 componentID, BaseComponent *component)
	{
		ComponentCreateFunction createfn = BaseComponent::getTypeCreateFunction(componentID);
		std::pair<uint32, uint32> newPair;
		newPair.first = componentID;
		newPair.second = createfn(_components[componentID], handle, component);
		entity.push_back(newPair);
	}

	void iEntityComponentSystem::deleteComponent(uint32 componentID, uint32 index)
	{
		std::vector<uint8> &array = _components[componentID];
		ComponentFreeFunction freefn = BaseComponent::getTypeFreeFunction(componentID);
		size_t typeSize = BaseComponent::getTypeSize(componentID);
		uint32 srcIndex = array.size() - typeSize;

		BaseComponent *destComponent = (BaseComponent *)&array[index];
		BaseComponent *srcComponent = (BaseComponent *)&array[srcIndex];
		freefn(destComponent);

		if (index == srcIndex)
		{
			array.resize(srcIndex);
			return;
		}

		memcpy(destComponent, srcComponent, typeSize);

		std::vector<std::pair<uint32, uint32>> &srcComponents = handleToEntity(srcComponent->_entity);
		for (uint32 i = 0; i < srcComponents.size(); i++)
		{
			if (componentID == srcComponents[i].first && srcIndex == srcComponents[i].second)
			{
				srcComponents[i].second = index;
				break;
			}
		}

		array.resize(srcIndex);
	}

	bool iEntityComponentSystem::removeComponentInternal(iEntityHandle handle, uint32 componentID)
	{
		std::vector<std::pair<uint32, uint32>> &entityComponents = handleToEntity(handle);
		for (uint32 i = 0; i < entityComponents.size(); i++)
		{
			if (componentID == entityComponents[i].first)
			{
				deleteComponent(entityComponents[i].first, entityComponents[i].second);
				uint32 srcIndex = entityComponents.size() - 1;
				uint32 destIndex = i;
				entityComponents[destIndex] = entityComponents[srcIndex];
				entityComponents.pop_back();
				return true;
			}
		}
		return false;
	}

	BaseComponent *iEntityComponentSystem::getComponentInternal(std::vector<std::pair<uint32, uint32>> &entityComponents, std::vector<uint8> &array, uint32 componentID)
	{
		for (uint32 i = 0; i < entityComponents.size(); i++)
		{
			if (componentID == entityComponents[i].first)
			{
				return (BaseComponent *)&array[entityComponents[i].second];
			}
		}
		return nullptr;
	}

	void iEntityComponentSystem::updateSystems(iEntitySystemList &systems)
	{
		std::vector<BaseComponent *> componentParam;
		std::vector<std::vector<uint8> *> componentArrays;
		for (uint32 i = 0; i < systems.size(); i++)
		{
			const std::vector<uint32> &componentTypes = systems[i]->getComponentTypes();
			if (componentTypes.size() == 1)
			{
				size_t typeSize = BaseComponent::getTypeSize(componentTypes[0]);
				std::vector<uint8> &array = _components[componentTypes[0]];
				for (uint32 j = 0; j < array.size(); j += typeSize)
				{
					BaseComponent *component = (BaseComponent *)&array[j];
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
			size_t typeSize = BaseComponent::getTypeSize(componentTypes[i]);
			uint32 size = _components[componentTypes[i]].size() / typeSize;
			if (size <= minSize)
			{
				minSize = size;
				minIndex = i;
			}
		}

		return minIndex;
	}

	void iEntityComponentSystem::updateSystemWithMultipleComponents(uint32 index, iEntitySystemList &systems, const std::vector<uint32> &componentTypes, std::vector<BaseComponent *> &componentParam, std::vector<std::vector<uint8> *> &componentArrays)
	{
		const std::vector<uint32> &componentFlags = systems[index]->getComponentFlags();

		componentParam.resize(std::max(componentParam.size(), componentTypes.size()));
		componentArrays.resize(std::max(componentArrays.size(), componentTypes.size()));
		for (uint32 i = 0; i < componentTypes.size(); i++)
		{
			componentArrays[i] = &_components[componentTypes[i]];
		}
		uint32 minSizeIndex = findLeastCommonComponent(componentTypes, componentFlags);

		size_t typeSize = BaseComponent::getTypeSize(componentTypes[minSizeIndex]);
		std::vector<uint8> &array = *componentArrays[minSizeIndex];
		for (uint32 i = 0; i < array.size(); i += typeSize)
		{
			componentParam[minSizeIndex] = (BaseComponent *)&array[i];
			std::vector<std::pair<uint32, uint32>> &entityComponents =
				handleToEntity(componentParam[minSizeIndex]->_entity);

			bool isValid = true;
			for (uint32 j = 0; j < componentTypes.size(); j++)
			{
				if (j == minSizeIndex)
				{
					continue;
				}

				componentParam[j] = getComponentInternal(entityComponents,
														 *componentArrays[j], componentTypes[j]);
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