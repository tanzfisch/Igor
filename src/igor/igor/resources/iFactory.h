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

#ifndef __FACTORY_H__
#define __FACTORY_H__

#include <igor/resources/iResource.h>

#include <memory>

namespace igor
{

    /*! represents a factory that can process resources
    */
    class IGOR_API iFactory
    {
        friend class iResourceManager;

    public:
        /*! does nothing
        */
        iFactory() = default;

        /*! does nothing
        */
        virtual ~iFactory() = default;

    protected:
        /*! \returns the fatory type

        this type is used to register with the resource manager
        */
        virtual const iaString &getType() const = 0;

        /*! \returns true if resource parameters are supported by this factory

        \param parameters the given resource parameters
        */
        virtual bool matchingType(const iResourceParameters &parameters) const = 0;

        /*! \returns resource type specific hash data
        */
        virtual iaString getHashData(const iResourceParameters &parameters) const = 0;

        /*! creates a resource object

        \param parameters the resource parameters
        \returns loaded or created new resource
        */
        virtual iResourcePtr createResource(const iResourceParameters &parameters) = 0;

        /*! loads the resource

        \param resource the resource to load
        \returns true if loading the resource was successful
        */
        virtual bool loadResource(iResourcePtr resource) = 0;

        /*! unloads the resource

        \param resource the resource to unload
        */
        virtual void unloadResource(iResourcePtr resource) = 0;

        /*! called once after registration to resource manager
        */
        virtual void init() {};

        /*! called once before unregistration from resource manager
        */
        virtual void deinit() {};
    };

    /*! definition of texture shared pointer
    */
    typedef std::shared_ptr<iFactory> iFactoryPtr;

}; // namespace igor

#endif // __SOUND_H__