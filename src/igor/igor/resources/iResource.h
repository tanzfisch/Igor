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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <iaux/data/iaString.h>
using namespace iaux;

#include <igor/iDefines.h>

#include <memory>
#include <any>

namespace igor
{

    /*! resource parameters that define how to load given resource
    */
    struct IGOR_API iResourceParameters
    {
        /*! name of the resource 
        
        this can be a filename or anything really
        */
        iaString _name;

        /*! type of this resource (optional)

        if set the resource manager will use a factory of this type to create this resource
        if not set the resource manager will use other information to figure out which factory to use
        */
        iaString _type;

        /*! resource cache mode. default Free
        */
        iResourceCacheMode _cacheMode = iResourceCacheMode::Free;

        /*! type specific parameters
        */
        std::any _data;
    };

    /*! represents a resource
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
        __IGOR_INLINE__ bool isValid() const
        {
            return _valid;
        }

        /*! \returns true if the resource was processed

        processed does not mean that it was loaded correctly 
        it can also mean that we are done trying to loading it
        */
        __IGOR_INLINE__ bool isProcessed() const
        {
            return _processed;
        }

        /*! \returns the resource name
        */
        __IGOR_INLINE__ const iaString &getName() const
        {
            return _parameters._name;
        }

        /*! \returns cache mode
        */
        __IGOR_INLINE__ iResourceCacheMode getCacheMode() const
        {
            return _parameters._cacheMode;
        }

        /*! \returns resource parameters
        */
        __IGOR_INLINE__ const iResourceParameters &getParameters() const
        {
            return _parameters;
        }

        /*! \returns the resource type
        */
        __IGOR_INLINE__ const iaString &getType() const
        {
            return _parameters._type;
        }

    protected:
        /*! initializes members

		\param parameters the parameters which define the resource
		*/
        iResource(const iResourceParameters &parameters)
            : _parameters(parameters)
        {
        }

    private:
        /*! true if there was actually a texture loaded
		*/
        bool _valid = false;

        /*! if true the texture is considered loaded regardless if it was a success or not
        */
        bool _processed = false;

        /*! parameters of this resource
        */
        iResourceParameters _parameters;

        /*! does nothing
        */
        iResource() = default;
    };

    /*! definition of resource pointer
	*/
    typedef std::shared_ptr<iResource> iResourcePtr;

}; // namespace igor

#endif // __RESOURCE_H__