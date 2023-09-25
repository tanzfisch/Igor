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

#ifndef __RESOURCE__
#define __RESOURCE__

#include <igor/data/iParameters.h>

#include <iaux/data/iaString.h>
#include <iaux/data/iaUUID.h>
using namespace iaux;

#include <memory>
#include <any>

namespace igor
{
    /*! resource id definition
     */
    typedef iaUUID iResourceID;

    /*! represents a resource

    available parameters for loading data:

    - id: the unique id of the resource (type: iResourceID)
    - alias: the alias of the resource (type: iaString) Internally this doubles as filename and id in string form.
    - type: the type of resource (type: iaString)
    - cacheMode: the cache mode of this resource (type: iResourceCacheMode)
    - quiet: if true this resource will cause less verbose output in logging. Helpful when generating stuff
    - generate: if true the data is assumed to be generated and not loaded. This implies that there is no uuid for it and therefore will also be generated

    Cache mode of a resource can be increased by requesting the same resource with a higher level of cache mode.

    */
    class IGOR_API iResource
    {
        friend class iResourceManager;

    public:
        /*! does nothing
         */
        virtual ~iResource() = default;

        /*! \returns true if there is valid data present
         */
        bool isValid() const;

        /*! \returns true if the resource was processed

        processed does not mean that it was loaded correctly
        it can also mean that we are done trying to loading it
        */
        bool isProcessed() const;

        /*! \returns true if this resource will not be logged
         */
        bool isQuiet() const;

        /*! \returns the source of this resource

        ie filename
        */
        const iaString &getSource() const;

        /*! \returns info string
        */
        const iaString getInfo() const;

        /*! \returns resource id
         */
        const iResourceID &getID() const;

        /*! \returns cache mode
         */
        iResourceCacheMode getCacheMode() const;

        /*! \returns resource parameters
         */
        const iParameters &getParameters() const;

        /*! \returns the resource type
         */
        const iaString &getType() const;

        /*! \returns extracted resource id from parameters

        \param parameters the given parameters
        */
        static bool extractID(const iParameters &parameters, iResourceID &id, bool quiet = false);

    protected:
        /*! initializes members

        \param parameters the parameters which define the resource
        */
        iResource(const iParameters &parameters);

    private:
        /*! true if there was actually a resource loaded
         */
        bool _valid = false;

        /*! if true the resource is considered loaded regardless if it was a success or not
         */
        bool _processed = false;

        /*! if true resource will not be logged
         */
        bool _quiet = true;

        /*! parameters of this resource
         */
        iParameters _parameters;

        /*! name of resource
         */
        iaString _alias;

        /*! type of the resource
         */
        iaString _type;

        /*! source name (ie filename)
        */
        iaString _source;

        /*! id of this resource
         */
        iResourceID _id;

        /*! the resources cache mode
         */
        iResourceCacheMode _cacheMode;

        /*! sets processed flag on resource

        \param processed the flag to set
        */
        void setProcessed(bool processed);

        /*! sets valid flag on resource

        \param valid the flag to set
        */
        void setValid(bool valid);

        /*! does nothing
         */
        iResource() = default;
    };

    /*! definition of resource pointer
     */
    typedef std::shared_ptr<iResource> iResourcePtr;

}; // namespace igor

#endif // __RESOURCE__