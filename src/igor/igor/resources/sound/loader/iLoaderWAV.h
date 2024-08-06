//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
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

#ifndef __LOADER_WAV__
#define __LOADER_WAV__

#include <igor/iDefines.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{
    /*! wav file header 
    */
    struct iWAVHeader
    {
        char _chunkID[4];
        int32 _chunkSize;
        char _format[4];
        char _subchunk1ID[4];
        int32 _subchunk1Size;
        int16 _audioFormat;
        int16 _numChannels;
        int32 _sampleRate;
        int32 _byteRate;
        int16 _blockAlign;
        int16 _bitsPerSample;
        int16 _extraParamSize;
        char _subchunk2ID[4];
        int32 _subchunk2Size;
    };

    /*! loads a wav file

    \param filename the file to load
    \param[out] header the returned header information of the wav file
    \param[out] buffer contains the audio data
    \param[out] bufferSize size of the audio data
    */
    bool loadWav(const iaString &filename, iWAVHeader &header, char** buffer, int32& bufferSize);

} // namespace igor

#endif // __LOADER_WAV__