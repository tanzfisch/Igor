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

#ifndef __IGOR_AUDIO_H__
#define __IGOR_AUDIO_H__

#include <igor/resources/sound/iSound.h>
#include <igor/resources/module/iModule.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{

    class iAudioImpl;

    /*! represents the audio interface

    in this case wrapping OpenAL
    */
    class IGOR_API iAudio : public iModule<iAudio>
    {

        friend class iModule<iAudio>;

    public:
        /*! creates an audio buffer based on given parameters

        \param[out] audioBuffer the returned audio buffer
        \param numChannels number of audio channels
        \param bitsPerSample bits per sample
        \param sampleRate sample rate
        \param buffer the audio data
        \param bufferSize size of audio data
        \returns true if successful
        */
        bool createBuffer(iAudioBuffer &audioBuffer, int16 numChannels, int16 bitsPerSample, int32 sampleRate, const char *buffer, int32 bufferSize);

        /*! destroys given audio buffer

        \param buffer the audio buffer to destroy
        */
        void destroyBuffer(const iAudioBuffer &buffer);

        /*! creates an audio source

        \param[out] audioSource the created audio source
        \returns true if successful
        */
        bool createSource(iAudioSource &audioSource);

        /*! destroys given audio source

        \param audioSource given audio source
        */
        void destroySource(const iAudioSource &audioSource);

        /*! sets audio source pitch

        \param pitch the given pitch
        */
        void setSourcePitch(const iAudioSource &audioSource, float32 pitch);

        /*! sets audio source gain

        \param gain the given gain
        */
        void setSourceGain(const iAudioSource &audioSource, float32 gain);

        /*! sets audio source loop mode

        \param loop if true loop mode is on
        */
        void setSourceLoop(const iAudioSource &audioSource, bool loop);

        /*! starts playback of source

        \param audioSource given source
        */
        void playSource(const iAudioSource &audioSource);

        /*! stops playback of source

        \param audioSource given source
        */
        void stopSource(const iAudioSource &audioSource);

        /*! updates position and velocoty of audio source

        \param audioSource the audio source to update
        \param position the given position
        \param velocity the given velocity
        */
        void updateSource(const iAudioSource &audioSource, const iaVector3d &position, const iaVector3d velocity);

        /*! binds source to sound

        \param source the given source
        \param resource the given sound resource
        */
        void bindSource(const iAudioSource &source, iResourcePtr resource);

        /*! updates the listeners position, orientation and velocity

        \param matrix the given matrix
        \param velocity the given velocity
        */
        void updateListener(const iaMatrixd &matrix, const iaVector3d velocity);

    private:
        /*! initializes the audio interface
        */
        iAudio();

        /*! shuts doen the audio interface
        */
        ~iAudio();

        /*! pimpl
        */
        iAudioImpl *_impl = nullptr;
    };

}; // namespace igor

#endif // __IGOR_AUDIO_H__