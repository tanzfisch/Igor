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

#ifndef __NODE_AUDIO_LISTENER_H__
#define __NODE_AUDIO_LISTENER_H__

#include <igor/scene/nodes/iNode.h>

namespace igor
{

    /*! audio listener node
    */
    class Igor_API iNodeAudioListener : public iNode
    {

        friend class iNodeManager;

    public:

        /*! returns world matrix of audio listener

        \param[out] matrix returns the world matrix
        */
        void getWorldMatrix(iaMatrixd &matrix);

        /*! sets this audio listener to be the active one
        */
        void setActiveListener();

        /*! \returns true if this audio listener is the active one
        */
        bool isActiveListener() const;

    protected:

        /*! matrix including position an orientation of this audio listeners in world coordinates
        */
        iaMatrixd _worldMatrix;

        /*! set the world matrix of the audio listener

        \param matrix world matrix for the audio listener
        */
        void onUpdateTransform(iaMatrixd &matrix);

    private:

        /*! stores which audio listener is the active one
        */
        static uint64 _activeListener;

        /*! init member variables
        */
        iNodeAudioListener();

        /*! copy ctor
        */
        iNodeAudioListener(iNodeAudioListener *node);

        /*! dtor
		*/
        virtual ~iNodeAudioListener();
    };

}; // namespace igor

#endif // __NODE_AUDIO_LISTENER_H__
