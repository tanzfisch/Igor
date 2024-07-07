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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef __IGOR_ANIMATION_FACTORY__
#define __IGOR_ANIMATION_FACTORY__

#include <igor/resources/iFactory.h>
#include <igor/resources/animation/iAnimation.h>

class TiXmlElement;

namespace igor
{

    /*! this factory creates animation resources
     */
    class iAnimationFactory : public iFactory
    {
        friend class iResourceManager;

    private:
        /*! init members
         */
        iAnimationFactory();

        /*! \returns resource type specific hash data
         */
        iaString getHashData(const iParameters &parameters) const override;

        /*! creates a resource object

        \param parameters the resource parameters
        \returns loaded or created new resource
        */
        iResourcePtr createResource(const iParameters &parameters) override;

        /*! loads the resource

        \param resource the resource to load
        \returns true if loading the resource was successful
        */
        bool loadResource(iResourcePtr resource) override;

        /*! unloads the resource

        \param resource the resource to unload
        */
        void unloadResource(iResourcePtr resource) override;

        /*! loads animation from file

        \param filename the file to load the animation from
        \param animation the target animation
        */
        bool loadAnimation(const iaString &filename, iAnimationPtr animation);

        /*! reads animation element from xml file

        \param animationElement the animation element to read
        \param animation the target animation
        */
        void readAnimationElement(TiXmlElement *animationElement, iAnimationPtr animation);

        /*! reads index key frames from element

        \returns key frames
        \param animationElement the animation element
        */
        iaKeyFrameGraphui readIndexAnimation(TiXmlElement *animationElement);

        /*! reads vector key frames from element

        \returns key frames
        \param animationElement the animation element
        */
        iaKeyFrameGraphVector3d readVector3Animation(TiXmlElement *animationElement);
    };

}; // namespace igor

#endif // __IGOR_ANIMATION_FACTORY__