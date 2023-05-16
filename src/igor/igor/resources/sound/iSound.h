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

#ifndef __IGOR_SOUND__
#define __IGOR_SOUND__

#include <igor/resources/iResource.h>

namespace igor
{

    /*! audio buffer
     */
    struct IGOR_API iAudioBuffer
    {
        uint32 _id = 0;
    };

    /*! audio source
     */
    struct IGOR_API iAudioSource
    {
        uint32 _id = 0;
    };

    /*! represents a sound resource
     */
    class IGOR_API iSound : public iResource
    {

        friend class iSoundFactory;
        friend class iAudio;

    public:
        /*! does nothing
         */
        ~iSound() = default;

        /*! \returns number of channels
         */
        int16 getNumberOfChannels() const;

        /*! \returns sample rate
         */
        int32 getSampleRate() const;

        /*! \returns byte rate
         */
        int32 getByteRate() const;

        /*! \returns bits per sample
         */
        int16 getBitsPerSample() const;

    private:
        /*! sound buffer
         */
        iAudioBuffer _buffer;

        /*! number of channels
         */
        int16 _numChannels = 0;

        /*! sample rate
         */
        int32 _sampleRate = 0;

        /*! byte rate
         */
        int32 _byteRate = 0;

        /*! bits per sample
         */
        int16 _bitsPerSample = 0;

        /*! bytes per sample
         */
        int32 _bytesPerSample = 0;

        /*! sample count
         */
        int32 _sampleCount = 0;

        /*! initializes members

        \param parameters the parameters which define the resource
        */
        iSound(const iParameters &parameters);
    };

    /*! definition of sound pointer
     */
    typedef std::shared_ptr<iSound> iSoundPtr;

}; // namespace igor

#endif // __IGOR_SOUND__