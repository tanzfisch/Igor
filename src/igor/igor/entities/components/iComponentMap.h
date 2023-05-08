//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2023 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_COMPONENT_MAP__
#define __IGOR_COMPONENT_MAP__

#include <igor/entities/components/iComponents.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <unordered_map>
#include <vector>
#include <deque>

namespace igor
{

    /*! entity to component map

    used for handling component types unknown to Igor
    */
    template <typename T>
    class iComponentMap
    {
    public:

        /*! adds component for given entity

        \param entityID the given entity
        \param component the component to be added
        */
        T &add(iEntityID entityID, const T &component)
        {
            uint32 index;

            if (!_freeIndices.empty())
            {
                index = _freeIndices.front();
                _freeIndices.pop_front();
                _components[index] = component;
            }
            else
            {
                index = _components.size();
                _components.push_back(component);
            }

            _entityToIndex[entityID] = index;

            return _components[index];
        }

        /*! removes component for given entity id

        \param entityID the given entity id
        */
        void remove(iEntityID entityID)
        {
            auto iter = _entityToIndex.find(entityID);
            if(iter == _entityToIndex.end())
            {
                return;
            }
            
            _freeIndices.push_back((*iter).second);
            _entityToIndex.erase(entityID);
        }

        /*! \returns a pointer to a component if available of given entity

        \param entityID the given entity id
        */
        T *tryGet(iEntityID entityID)
        {
            auto iter = _entityToIndex.find(entityID);
            if(iter == _entityToIndex.end())
            {
                return nullptr;
            }

            return &(_components[iter->second]);
        }

        /*! \returns a reference to a component of given entity

        critical error if entity does not exist

        \param entityID the given entity id
        */
        T &get(iEntityID entityID)
        {
            auto iter = _entityToIndex.find(entityID);
            con_assert(iter != _entityToIndex.end(), "component does not exist");

            return _components[iter->second];
        }

        /*! \returns a const reference to a component of given entity

        critical error if entity does not exist

        \param entityID the given entity id
        */
        const T &get(iEntityID entityID) const
        {
            auto iter = _entityToIndex.find(entityID);
            if(iter == _entityToIndex.end())
            {
                con_crit("accessed non existing entity");
                return T();
            }

            return _components[iter->second];
        }

        void clear()
        {
            _components.clear();
            _entityToIndex.clear();
            _freeIndices.clear();
        }

    private:

        /*! list of component data
        */
        std::vector<T> _components;

        /*! lookup table to map entity IDs to indices
        */
        std::unordered_map<iEntityID, uint32> _entityToIndex;

        /*! queue of free indices
        */
        std::deque<uint32> _freeIndices;
    };

}

#endif // __IGOR_COMPONENT_MAP__
