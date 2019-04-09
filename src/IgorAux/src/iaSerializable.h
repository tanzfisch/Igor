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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __IGOR_AUX_SERIALIZABLE__
#define __IGOR_AUX_SERIALIZABLE__

#include <iaDefines.h>
#include <iaConsole.h>

#include <iaColor3.h>
#include <iaVector4.h>
#include <iaGradient.h>

#include <iostream>
using namespace std;

namespace IgorAux
{

    class iaString;

    /*! helper class for serializing / deserializing any data from and to a stream
    */
    class IgorAux_API iaSerializable
    {

    public:
        
        /*! writes uint8 value to stream

        \param stream output stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool writeUInt8(ofstream& stream, uint8 value);

        /*! reads uint8 value from stream

        \param stream input stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool readUInt8(ifstream& stream, uint8& value);

        /*! writes int8 value to stream

        \param stream output stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool writeInt8(ofstream& stream, int8 value);

        /*! reads int8 value from stream

        \param stream input stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool readInt8(ifstream& stream, int8& value);

        /*! writes uint16 value to stream

        \param stream output stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool writeUInt16(ofstream& stream, uint16 value);

        /*! reads uint16 value from stream

        \param stream input stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool readUInt16(ifstream& stream, uint16& value);

        /*! writes int16 value to stream

        \param stream output stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool writeInt16(ofstream& stream, int16 value);

        /*! reads int16 value from stream

        \param stream input stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool readInt16(ifstream& stream, int16& value);

        /*! writes uint32 value to stream

        \param stream output stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool writeUInt32(ofstream& stream, uint32 value);

        /*! reads uint32 value from stream

        \param stream input stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool readUInt32(ifstream& stream, uint32& value);

        /*! writes int32 value to stream

        \param stream output stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool writeInt32(ofstream& stream, int32 value);

        /*! reads int32 value from stream

        \param stream input stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool readInt32(ifstream& stream, int32& value);

        /*! writes uint64 value to stream

        \param stream output stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool writeUInt64(ofstream& stream, uint64 value);

        /*! reads uint64 value from stream

        \param stream input stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool readUInt64(ifstream& stream, uint64& value);

        /*! writes int64 value to stream

        \param stream output stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool writeInt64(ofstream& stream, int64 value);

        /*! reads int64 value from stream

        \param stream input stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool readInt64(ifstream& stream, int64& value);

        /*! writes float32 value to stream

        \param stream output stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool writeFloat32(ofstream& stream, float32 value);

        /*! reads float32 value from stream

        \param stream input stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool readFloat32(ifstream& stream, float32& value);

        /*! writes float64 value to stream

        \param stream output stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool writeFloat64(ofstream& stream, float64 value);

        /*! reads float64 value from stream

        \param stream input stream
        \param value value to write
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool readFloat64(ifstream& stream, float64& value);

        /*! writes unsigned integer value with specified lenght to stream

        \param stream output stream
        \param value value to write
        \param bytes bytes to write count
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool writeUInt(ofstream& stream, uint64 value, uint8 bytes);

        /*! reads unsigned integer value with specified lenght from stream

        \param stream input stream
        \param value value to read
        \param bytes bytes to read count
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool readUInt(ifstream& stream, uint64& value, uint8 bytes);

        /*! writes integer value with specified lenght to stream

        \param stream output stream
        \param value value to write
        \param bytes bytes to write count
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool writeInt(ofstream& stream, int64 value, uint8 bytes);

        /*! reads integer value with specified lenght from stream

        \param stream input stream
        \param value value to read
        \param bytes bytes to read count
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool readInt(ifstream& stream, int64& value, uint8 bytes);

        /*! writes buffer with specified lenght to stream

        \param stream output stream
        \param buffer buffer to write
        \param bytes bytes to write count
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool write(ofstream& stream, const char* buffer, uint32 bytes);

        /*! reads buffer with specified lenght from stream

        \param stream input stream
        \param buffer buffer to read
        \param bytes bytes to read count
        \returns true if successfull
        */
        static __IGOR_INLINE__ bool read(ifstream& stream, char* buffer, uint32 bytes);

        /*! writes UTF8 encoded string in to stream

        \param stream output stream
        \param value the string to write
        \returns true if successfull
        */
        static bool writeUTF8(ofstream& stream, const iaString& value);

        /*! reads UTF8 encoded string from stream

        \param stream input stream
        \param value the string to read
        \returns true if successfull
        */
        static bool readUTF8(ifstream& stream, iaString& value);

        /*! writes a color3f to stream

        \param stream output stream
        \param value output color
        \returns true if successfull
        */
        static bool write(ofstream& stream, const iaColor3f& value);

        /*! reads a color3f from stream

        \param stream input stream
        \param value destination color
        \returns true if successfull
        */
        static bool read(ifstream& stream, iaColor3f& value);

        /*! writes a color3c to stream

        \param stream output stream
        \param value output color
        \returns true if successfull
        */
        static bool write(ofstream& stream, const iaColor3c& value);

        /*! reads a color3c from stream

        \param stream input stream
        \param value destination color
        \returns true if successfull
        */
        static bool read(ifstream& stream, iaColor3c& value);

        /*! writes a color4f to stream

        \param stream output stream
        \param value output color
        \returns true if successfull
        */
        static bool write(ofstream& stream, const iaColor4f& value);

        /*! reads a color4f from stream

        \param stream input stream
        \param value destination color
        \returns true if successfull
        */
        static bool read(ifstream& stream, iaColor4f& value);

        /*! writes a vector2f to stream

        \param stream output stream
        \param value output vector
        \returns true if successfull
        */
        static bool write(ofstream& stream, const iaVector2f& value);

        /*! reads a vector2f from stream

        \param stream input stream
        \param value destination vector
        \returns true if successfull
        */
        static bool read(ifstream& stream, iaVector2f& value);

        /*! writes a vector3f to stream

        \param stream output stream
        \param value output vector
        \returns true if successfull
        */
        static bool write(ofstream& stream, const iaVector3f& value);

        /*! reads a vector3f from stream

        \param stream input stream
        \param value destination vector
        \returns true if successfull
        */
        static bool read(ifstream& stream, iaVector3f& value);

        /*! writes a vector4f to stream

        \param stream output stream
        \param value output vector
        \returns true if successfull
        */
        static bool write(ofstream& stream, const iaVector4f& value);

        /*! reads a vector4f from stream

        \param stream input stream
        \param value destination vector
        \returns true if successfull
        */
        static bool read(ifstream& stream, iaVector4f& value);
        
        /*! write float32 gradient to stream

        \param stream output stream
        \param value gradient to write
        */
        static bool write(ofstream& stream, const iaGradientf& value);

        /*! reads a float32 gradient from stream

        \param stream input stream
        \param value destination gradient
        \returns true if successfull
        */
        static bool read(ifstream& stream, iaGradientf& value);

        /*! write uint32 gradient to stream

        \param stream output stream
        \param value gradient to write
        */
        static bool write(ofstream& stream, const iaGradientui& value);

        /*! reads a uint32 gradient from stream

        \param stream input stream
        \param value destination gradient
        \returns true if successfull
        */
        static bool read(ifstream& stream, iaGradientui& value);

        /*! write float32 3d vector gradient to stream

        \param stream output stream
        \param value gradient to write
        */
        static bool write(ofstream& stream, const iaGradientVector3f& value);

        /*! reads a float32 3d vector gradient from stream

        \param stream input stream
        \param value destination gradient
        \returns true if successfull
        */
        static bool read(ifstream& stream, iaGradientVector3f& value);

        /*! write float32 2d vector gradient to stream

        \param stream output stream
        \param value gradient to write
        */
        static bool write(ofstream& stream, const iaGradientVector2f& value);

        /*! reads a float32 2d vector gradient from stream

        \param stream input stream
        \param value destination gradient
        \returns true if successfull
        */
        static bool read(ifstream& stream, iaGradientVector2f& value);

        /*! write float32 color gradient to stream

        \param stream output stream
        \param value gradient to write
        */
        static bool write(ofstream& stream, const iaGradientColor4f& value);

        /*! reads a float32 color gradient from stream

        \param stream input stream
        \param value destination gradient
        \returns true if successfull
        */
        static bool read(ifstream& stream, iaGradientColor4f& value);

    };

#include <iaSerializable.inl>

};

#endif
