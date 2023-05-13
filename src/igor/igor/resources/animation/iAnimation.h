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

#ifndef __IGOR_ANIMATION__
#define __IGOR_ANIMATION__

#include <igor/resources/iResource.h>
#include <iaux/system/iaTime.h>
using namespace iaux;

#include <memory>

namespace igor
{

    /*! animation base class
    */
    class IGOR_API iAnimation : public iResource
    {
    public:

        /*! does nothing
        */
        virtual ~iAnimation() = default;

        /*! \returns duration of animation
        */
        virtual iaTime getDuration() const = 0;

        /*! evaluates animation

        \param t the scale form 0 to 1 from start to stop
        */
        virtual void evaluate(float64 t) = 0;

    private:

        /*! initializes members

        \param parameters the parameters which define the animation
        */
        iAnimation(const iResourceParameters &parameters);            
    };

    /*! evaluation pointer definition
    */
    typedef std::shared_ptr<iAnimation> iAnimationPtr;

} // namespace igor

#endif
