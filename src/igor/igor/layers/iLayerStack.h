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

#ifndef __IGOR_LAYERSTACK_H__
#define __IGOR_LAYERSTACK_H__

#include <igor/layers/iLayer.h>

#include <vector>

namespace igor
{
    /*! event base class
    */
    class iLayerStack
    {
    public:
        /*! clean up
        */
        ~iLayerStack();

        /*! adds layer to stack

        adding a layer to the layer stack passes ownership to the layer stack

        \param layer the layer to be added
        */
        void addLayer(iLayer *layer);

        /*! removes layer from stack

        removing a layer from the layer stack passes ownership back to the caller

        \param layer the layer to be removed
        */
        void removeLayer(iLayer *layer);

        /*! \return the layer stack
        */
        const std::vector<iLayer *> &getStack() const;

        /*! clears layer stack and destoys all layers in the process
        */
        void clear();

    private:
        /*! the layers
        */
        std::vector<iLayer *> _layers;
    };

}; // namespace igor

#endif // __IGOR_LAYERSTACK_H__