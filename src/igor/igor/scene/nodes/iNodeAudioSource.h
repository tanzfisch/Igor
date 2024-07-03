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

#ifndef __NODE_AUDIO_SOURCE__
#define __NODE_AUDIO_SOURCE__

#include <igor/scene/nodes/iNode.h>
#include <igor/resources/sound/iSound.h>

namespace igor
{

    /*! audio source node
    */
    class IGOR_API iNodeAudioSource : public iNode
    {

        friend class iNodeManager;

    public:
        /*! sets sound that will be played by this source
        */
        void setSound(iSoundPtr sound);

        /*! play back sound
        */
        void play();

        /*! stop playing sound
        */
        void stop();

        /*! sets audio source pitch

        \param pitch the given pitch
        */
        void setPitch(float32 pitch);

        /*! \returns audio source pich
        */
        float32 getPitch() const;

        /*! sets audio source gain

        \param gain the given gain
        */
        void setGain(float32 gain);

        /*! \returns audio source gain
        */
        float32 getGain() const;

        /*! sets audio source loop mode

        \param loop if true loop mode is on
        */
        void setLoop(bool loop);

        /*! \retruns true if audio source is in loop mode
        */
        bool isLooped() const;

    protected:
        /*! last position of source
        */
        iaVector3d _position;

        /*! last velocity of source
        */
        iaVector3d _velocity;

        /*! set the world matrix of the audio listener

        \param matrix world matrix for the audio listener
        */
        void onUpdateTransform(iaMatrixd &matrix);

    private:
        /*! handle to internal source
        */
        iAudioSource _source;

        /*! pitch value
        */
        float32 _pitch = 1.0;

        /*! gain value
        */
        float32 _gain = 1.0;

        /*! if true source is in loop mode
        */
        bool _isLooped = false;

        /*! init member variables
        */
        iNodeAudioSource();

        /*! copy ctor
        */
        iNodeAudioSource(iNodeAudioSource *node);

        /*! dtor
		*/
        virtual ~iNodeAudioSource();
    };

}; // namespace igor

#endif // __NODE_AUDIO_SOURCE__
