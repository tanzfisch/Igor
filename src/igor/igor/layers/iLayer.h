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
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_LAYER_H__
#define __IGOR_LAYER_H__

#include <igor/system/events/iEvent.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{
    /*! layer base class
    */
    class Igor_API iLayer
    {
    public:
        /*! init members
        */
        iLayer(const iaString &name = "Layer", int32 zIndex = 0);

        /*! called when added to layer stack
        */
        virtual void onInit() = 0;

        /*! called when removed from layer stack
        */
        virtual void onDeinit() = 0;

        /*! called on application pre draw event
        */
        virtual void onPreDraw() = 0;

        /*! called on application post draw event
        */
        virtual void onPostDraw() = 0;

        /*! called on any other event
        */
        virtual void onEvent(iEvent &event) = 0;

        /*! \returns layer name
        */
        const iaString &getName() const;

        /*! \returns z index
        */
        int32 getZIndex() const;

    private:
        /*! the layer name
        */
        iaString _name;

        /*! the z index
        */
        int32 _zIndex = 0;
    };

}; // namespace igor

#endif // __IGOR_LAYER_H__