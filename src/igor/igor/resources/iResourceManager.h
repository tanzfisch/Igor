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

#ifndef __IGOR_RESOURCEMANAGER__
#define __IGOR_RESOURCEMANAGER__

#include <igor/resources/iFactory.h>
#include <igor/resources/module/iModule.h>
#include <igor/resources/texture/iTexture.h>
#include <igor/resources/sound/iSound.h>
#include <igor/resources/sprite/iSprite.h>
#include <igor/resources/animation/iAnimation.h>
#include <igor/resources/model/iModel.h>
#include <igor/resources/shader_material/iShaderMaterial.h>
#include <igor/resources/iResourceDictionary.h>

#include <iaux/system/iaDirectory.h>
#include <iaux/data/iaString.h>
#include <iaux/system/iaMutex.h>
using namespace iaux;

#include <unordered_map>
#include <map>
#include <vector>
#include <deque>

namespace igor
{

    /*! manages resources and their factories
     */
    class IGOR_API iResourceManager : public iModule<iResourceManager>
    {

        friend class iModule<iResourceManager>;

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
        const std::vector<iaString> &getSearchPaths() const;

        /*! clears search path list
         */
        void clearSearchPaths();

        /*! \returns full path to a file

        If the file does not exist it returns an empty string

        It only resolves for files that are within one of the given search paths

        \param filepath relative or absolute path to file
        */
        const iaString resolvePath(const iaString &filepath);

        /*! \returns filename for given resource id

        \param id the given resource id
        */
        const iaString getFilename(const iResourceID &id);

        /*! \returns alias for given resource id (if available)

        \param id the given resource id
        */
        const iaString getAlias(const iResourceID &id);

        /*! sets alias for given resource id

        \param id the given resource id
        \param alias the given alias
        */
        void setAlias(const iResourceID &id, const iaString& alias);

        /*! \returns relative path to search path. returns full path if no matching search path found

        \param filename full path to file
        */
        iaString getRelativePath(const iaString &filename);

        /*! \returns true if filename is a file in our search paths or an absolute path to an existing file

        \param filename the file we are looking for
        */
        bool fileExists(const iaString &filename);

        /*! requests a resource to be loaded asynchronously.

        \param param parameters for loading resource
        \returns shared pointer to resource
        */
        iResourcePtr requestResource(const iParameters &parameters);

        /*! loads a resource synchronously.

        \param param parameters for loading resource
        \returns shared pointer to resource
        */
        iResourcePtr loadResource(const iParameters &parameters);

        /*! creates a new resource

        \param param parameters for creating resource
        \returns shared pointer to resource
        */
        iResourcePtr createResource(const iParameters &parameters);

        /*! requests a resource to be loaded asynchronously.

        template version for more convenience

        \param param parameters for loading resource
        \returns shared pointer to resource
        */
        template <typename T>
        std::shared_ptr<T> requestResource(const iParameters &parameters);

        /*! loads a resource synchronously.

        template version for more convenience

        \param param parameters for loading resource
        \returns shared pointer to resource
        */
        template <typename T>
        std::shared_ptr<T> loadResource(const iParameters &parameters);

        /*! creates a new resource of given type

        template version for more convenience

        \returns shared pointer to resource
        */
        template <typename T>
        std::shared_ptr<T> createResource();

        /*! requests a resource to be loaded asynchronously

        template version for more convenience

        \param alias the name of the resource (this can also be a filename)
        \returns shared pointer to resource
        */
        template <typename T>
        std::shared_ptr<T> requestResource(const iaString &alias, iResourceCacheMode cacheMode = iResourceCacheMode::Cache);

        /*! loads a resource synchronously

        template version for more convenience

        \param alias the name of the resource (this can also be a filename)
        \returns shared pointer to resource
        */
        template <typename T>
        std::shared_ptr<T> loadResource(const iaString &alias, iResourceCacheMode cacheMode = iResourceCacheMode::Cache);

        /*! requests a resource to be loaded asynchronously by id

        template version for more convenience

        \param resourceID the id of the resource
        \returns shared pointer to resource
        */
        template <typename T>
        std::shared_ptr<T> requestResource(const iResourceID &resourceID, iResourceCacheMode cacheMode = iResourceCacheMode::Cache);

        /*! loads a resource synchronously by id

        template version for more convenience

        \param resourceID the id of the resource
        \returns shared pointer to resource
        */
        template <typename T>
        std::shared_ptr<T> loadResource(const iResourceID &resourceID, iResourceCacheMode cacheMode = iResourceCacheMode::Cache);

        /*! \returns resource by id

        \param id the id of the resource
        */
        iResourcePtr getResource(const iResourceID &id);

        /*! \returns resource by id

        typed version

        \param id the id of the resource
        */
        template <typename T>
        std::shared_ptr<T> getResource(const iResourceID &id);

        /*! \returns resource by name

        typed version

        \param name the name of the resource
        */
        template <typename T>
        std::shared_ptr<T> getResource(const iaString &alias);

        /*! returns all materials

        \param[out] materials the materials

        TODO how about a more general interface?
        */
        void getMaterials(std::vector<iShaderMaterialPtr> &materials);

        /*! works like a garbage collector.

        Iterates through all resources and checks how many references every resource has. If reference count
        goes down to 1 then the resource will be released. If reference count is greater 1 and the resource was
        not loaded yet then the resource will be loaded.

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

        /*! sets the load mode

        Does not effect load requests that are already in queue

        \param loadMode the load mode to set
        */
        void setLoadMode(iResourceManagerLoadMode loadMode);

        /*! \returns the load mode
         */
        iResourceManagerLoadMode getLoadMode() const;

        /*! loads resource dictionary

        this will append resources to existing dictionary

        \param filename filename to resource dictionary
        */
        void loadResourceDictionary(const iaString &filename);

        /*! write resource dictionary to given filename

        \param filename the file to write the dictionary to
        */
        void saveResourceDictionary(const iaString &filename);

        /*! clear resource dictionary

        (means it will only contain igor internal resources)
        */
        void clearResourceDictionary();

        /*! \returns resource id for given alias or filepath

        If it does not exist yet but it represents a valid file path within one of the search path. We add a new resource.

        \param aliasOrFilename the given alias or filename
        */
        const iResourceID getResourceID(const iaString &aliasOrFilename) const;

        /*! adds new resource to dictionary

        \param filename path to resource (must be relative to search paths)
        \param alias optional alias
        */
        void addResource(const iaString &filename, const iaString &alias = "");

        /*! removes given resource from dictionary

        \param resourceID the given resource id
        */
        void removeResource(const iResourceID &resourceID);

        /*! \returns resource type for given filename
        
        this is purely based on file extension

        \param filename the file name to guess the type of
        */
        const iaString getType(const iaString &filename) const;

        /*! \returns resource type for given resource id

        this is purely based on file extension

        \param resourceID the given resource id
        */
        const iaString getType(const iResourceID &resourceID) const;

        /*! saves a resource to the filesystem        

        \param resource the given resource to save (must be valid and have a valid source/filename)
        \param filename optional filename if resource is to be saved in a new location
        \returns true if successfully saved
        */
        bool saveResource(iResourcePtr resource, const iaString &filename = "");

        /*! saves a resource to the filesystem        

        \param resourceID the given resource ID to save (must be valid and have a valid source/filename)
        \param filename optional filename if resource is to be saved in a new location
        \returns true if successfully saved
        */
        bool saveResource(iResourceID resourceID, const iaString &filename = "");

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
        bool _interruptLoading = false;

        /*! map of resources
         */
        std::unordered_map<iResourceID, iResourcePtr> _resources;

        /*! loading queue
         */
        std::deque<iResourcePtr> _loadingQueue;

        /*! load mode
         */
        iResourceManagerLoadMode _loadMode = iResourceManagerLoadMode::Application;

        /*! resource dictionary
         */
        iResourceDictionary _resourceDictionary;

        /*! \returns factory for given resource parameters

        \param parameters given resource parameters
        */
        iFactoryPtr getFactory(const igor::iParameters &parameters);

        /*! \returns a resource matching the given parameters. If none is matching it will return nullptr

        \param parameters the given parameters
        */
        iResourcePtr getResource(const iParameters &parameters);

        /*! creates a resource

        \param factory the factory to ask for the resource
        \param parameters the parameters to create the resource with
        */
        iResourcePtr createResource(iFactoryPtr factory, const iParameters &parameters);

        /*! applies config settings on resource manager
         */
        void configure();

        /*! helper function to construct parameters

        \param type the resource type
        \param alias the alias (or resource id as string)
        \param cacheMode the cache mode to load the resource with
        */
        iParameters buildParam(const iaString &type, const iaString &alias, iResourceCacheMode cacheMode = iResourceCacheMode::DontCache);

        /*! helper function to construct parameters

        \param type the resource type
        \param resourceID the resource id
        \param cacheMode the cache mode to load the resource with
        */
        iParameters buildParam(const iaString &type, const iResourceID &resourceID, iResourceCacheMode cacheMode = iResourceCacheMode::DontCache);

        /*! does nothing
         */
        iResourceManager();

        /*! free resources
         */
        ~iResourceManager();
    };

#include <igor/resources/iResourceManager.inl>

} // namespace igor

#endif // __IGOR_RESOURCEMANAGER__