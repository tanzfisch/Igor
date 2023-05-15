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

        /*! key value parameters
        */
        std::map<iaString, std::any> _parameters;
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
        bool isValid() const;

        /*! \returns true if the resource was processed

        processed does not mean that it was loaded correctly 
        it can also mean that we are done trying to loading it
        */
        bool isProcessed() const;

        /*! \returns the resource name
        */
        const iaString &getName() const;

        /*! \returns cache mode
        */
        iResourceCacheMode getCacheMode() const;

        /*! \returns resource parameters
        */
        const iResourceParameters &getParameters() const;

        /*! \returns the resource type
        */
        const iaString &getType() const;

    protected:
        /*! initializes members

		\param parameters the parameters which define the resource
		*/
        iResource(const iResourceParameters &parameters)
            : _parameters(parameters)
        {
        }

    private:
        /*! true if there was actually a resource loaded
		*/
        bool _valid = false;

        /*! if true the resource is considered loaded regardless if it was a success or not
        */
        bool _processed = false;

        /*! parameters of this resource
        */
        iResourceParameters _parameters;

        /*! sets processed flag on resource
        */
        void setProcessed(bool processed);

        /*! sets valid flag on resource
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