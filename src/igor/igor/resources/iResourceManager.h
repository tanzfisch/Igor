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
// (c) Copyright 2012-2020 by Martin Loga
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
// contact: martinloga@gmx.de

#ifndef __RESOURCEMANAGER__
#define __RESOURCEMANAGER__

#include <igor/resources/iFactory.h>

#include <iaux/system/iaSingleton.h>
#include <iaux/system/iaDirectory.h>
#include <iaux/data/iaString.h>
#include <iaux/system/iaMutex.h>
using namespace iaux;

#include <unordered_map>
#include <map>
#include <vector>

namespace igor
{

    /*! manages resoruces and their factories
    */
    class Igor_API iResourceManager : public iaSingleton<iResourceManager>
    {

        friend class iaSingleton<iResourceManager>;

    public:
        /*! adds search path to list

        \param folder search path to add
        */
        void addSearchPath(const iaString &folder);

        /*! removes search path from list

        \param folder search path to remove
        */
        void removeSearchPath(const iaString &folder);

        /*! \returns list of search paths
        */
        const std::vector<iaString>& getSearchPaths() const;

        /*! clears search path list
        */
        void clearSearchPaths();

        /*! \returns full path to a file if it exists

        \param filename file name to search for in search paths
        */
        iaString getPath(const iaString &filename);

        /*! \returns relative path to search path. returns full path if no matching search path found

        \param filename full path to file
        */
        iaString getRelativePath(const iaString &filename);

        /*! requests a resource to be loaded asynchronously.

        \param param parameters for loading resource
        \returns shared pointer to resource
        */
        iResourcePtr requestResource(const iResourceParameters& parameters);

        /*! loads a resource synchronously.

        \param param parameters for loading resource
        \returns shared pointer to resource
        */
        iResourcePtr loadResource(const iResourceParameters& parameters);

        /*! works like a garbage collector.

        Interrates through all textures and checks how many references every texture has. If reference count
        goes down to 1 then the texture will be released. If reference count is greater 1 and the texture was
        not loaded yet and the texture will be loaded.

        \param cacheModeLevel level of cache mode to be released
        */
        void flush(iResourceCacheMode cacheModeLevel = iResourceCacheMode::Free);

        /*! if a flush in a different thread is currently running. this will prevent loading new data from disk and in consequence make it stop earlier
        */
        void interruptFlush();

        /*! registers factory to resource manager

        \param factory the given factory
        */
        void registerFactory(iFactoryPtr factory);

        /*! unregisters factory from resource manager

        \param factory the given factory
        */
        void unregisterFactory(iFactoryPtr factory);

    private:
        /*! mutex to manage access to internal data
        */
        iaMutex _mutex;

        /*! list of search paths
        */
        std::vector<iaString> _searchPaths;

        /*! map of registered factories
        */
        std::map<iaString, iFactoryPtr> _factories;

        /*! flag to interrupt flush cross threads
        */
        bool _interrupLoading = false;

        /*! map of resources
        */
        std::unordered_map<int64, iResourcePtr> _resources;

        /*! \returns resource for given parameters

        \param parameters the resource parameters
        \param factory the factory used to create the resource if not found
        */
        iResourcePtr getResource(const iResourceParameters& parameters, iFactoryPtr factory);

        /*! \returns factory for given resource parameters

        \param parameters given resource parameters
        */
        iFactoryPtr getFactory(const igor::iResourceParameters& parameters);

        /*! calculates hash value based on resource parameters

        \param parameters the resource parameters
        \param factory the factory that will be used to load this resource
        */
        int64 calcHashValue(const iResourceParameters& parameters, iFactoryPtr factory);

        /*! does nothing
        */
        iResourceManager();

        /*! free resources
        */
        ~iResourceManager();
    };

} // namespace igor

#endif