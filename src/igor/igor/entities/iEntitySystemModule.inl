// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

template <typename T>
void iEntitySystemModule::registerComponentType()
{
    con_assert_sticky(_registeredComponentTypes.size() < IGOR_MAX_ENTITY_COMPONENT_TYPES, "only " << IGOR_MAX_ENTITY_COMPONENT_TYPES << " types supported");

    const std::type_index typeID = typeid(T);

    auto iter = _registeredComponentTypes.find(typeID);
    con_assert(iter == _registeredComponentTypes.end(), "component type already registered " << typeID.name());    

    iEntityComponentMask id = 0b1;
    id <<= _registeredComponentTypes.size();

    _registeredComponentTypes[typeID] = id;
}
