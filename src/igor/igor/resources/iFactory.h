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

#ifndef __FACTORY__
#define __FACTORY__

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
        iFactory(const iaString &type, const std::vector<iaString> &supportedExtensions)
            : _type(type), _supportedExtensions(supportedExtensions)
        {
        }

        /*! does nothing
         */
        virtual ~iFactory() = default;

        /*! \returns the factory type

        this type is used to register with the resource manager
        */
        const iaString &getType() const
        {
            return _type;
        }

        /*! \returns supported file extensions or subtypes
         */
        const std::vector<iaString> &getSupportedExtensions() const
        {
            return _supportedExtensions;
        }

    protected:
        /*! the factory type
         */
        iaString _type;

        /*! supported file extensions or subtypes
         */
        std::vector<iaString> _supportedExtensions;

        /*! \returns resource type specific hash data based on given parameters

        \param parameters the given parameters
        */
        virtual iaString getHashData(const iParameters &parameters) const { return ""; };

        /*! creates a resource object

        \param name the name of the resource
        \param parameters the resource parameters
        \returns new resource
        */
        virtual iResourcePtr createResource(const iParameters &parameters) = 0;

        /*! creates an empty resource

        \returns new resource
        */
        virtual iResourcePtr createResource()
        {
            con_err("not implemented");
            return nullptr;
        }

        /*! loads the resource based on it's parameters

        \param resource the resource to load
        \returns true if loading the resource was successful
        */
        virtual bool loadResource(iResourcePtr resource) = 0;

        /*! unloads the resource

        \param resource the resource to unload
        */
        virtual void unloadResource(iResourcePtr resource) = 0;

        /*! exports the resource based on it's parameters

        this needs to be a valid resource with a valid source parameter

        \param resource the resource to save
        \param filename optional filename if resource is to be saved in a new location
        \returns true if saving the resource was successful
        */
        virtual bool saveResource(iResourcePtr resource, const iaString &filename = "")
        {
            con_err("not implemented");
            return false;
        }

        /*! called once after registration to resource manager
         */
        virtual void init(){};

        /*! called once before unregistration from resource manager
         */
        virtual void deinit(){};
    };

    /*! definition of texture shared pointer
     */
    typedef std::shared_ptr<iFactory> iFactoryPtr;

}; // namespace igor

#endif // __SOUND__