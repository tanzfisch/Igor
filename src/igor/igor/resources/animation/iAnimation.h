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
#include <iaux/data/iaKeyFrameGraph.h>
using namespace iaux;

#include <memory>

namespace igor
{

    /*! animation

    available parameters:

    - inherits all parameters from iResource
    - translateAnimation: translate animation data (type: iaKeyFrameGraphVector3d)
    - rotateAnimation: rotate animation data (type: iaKeyFrameGraphVector3d)
    - scaleAnimation: scale animation data (type: iaKeyFrameGraphVector3d)
    - spriteAnimation: sprite frame index animation data (type: iaKeyFrameGraphui)

     */
    class IGOR_API iAnimation : public iResource
    {
        friend class iAnimationFactory;

    public:
        /*! does nothing
         */
        virtual ~iAnimation() = default;

        /*! \returns true if translate animation is present
         */
        bool hasTranslateAnimation() const;

        /*! \returns translate value for given t

        \param t normalized time from clip
        */
        const iaVector3d getTranslate(float64 t) const;

        /*! \returns true if rotate animation is present
         */
        bool hasRotateAnimation() const;

        /*! \returns rotation value for given t

        \param t normalized time from clip
        */
        const iaVector3d getRotate(float64 t) const;

        /*! \returns true if scale animation is present
         */
        bool hasScaleAnimation() const;

        /*! \returns scale value for given t

        \param t normalized time from clip
        */
        const iaVector3d getScale(float64 t) const;

        /*! \returns true if frame index animation is present
         */
        bool hasFrameIndexAnimation() const;

        /*! \returns frame index value for given t

        \param t normalized time from clip
        */
        const uint32 getFrameIndex(float64 t) const;

    private:
        // TODO I don't like that we have so many placeholders here

        /*! translate animation
         */
        iaKeyFrameGraphVector3d _translate;

        /*! rotate animation
         */
        iaKeyFrameGraphVector3d _rotate;

        /*! scale animation
         */
        iaKeyFrameGraphVector3d _scale;

        /*! frame index animation
         */
        iaKeyFrameGraphui _frameIndex;

        /*! initializes members

        \param parameters the parameters which define the animation
        */
        iAnimation(const iParameters &parameters);

        /*! overrides translate animation

        \param translate the new translate animation
        */
        void setTranslateAnimation(const iaKeyFrameGraphVector3d &translate);

        /*! overrides rotate animation

        \param rotate the new rotate animation
        */
        void setRotateAnimation(const iaKeyFrameGraphVector3d &rotate);

        /*! overrides scale animation

        \param scale the new scale animation
        */
        void setScaleAnimation(const iaKeyFrameGraphVector3d &scale);

        /*! overrides frame index animation

        \param frameIndex the new frame index animation
        */
        void setFrameIndexAnimation(const iaKeyFrameGraphui &frameIndex);
    };

    /*! evaluation pointer definition
     */
    typedef std::shared_ptr<iAnimation> iAnimationPtr;

} // namespace igor

#endif // __IGOR_ANIMATION__
