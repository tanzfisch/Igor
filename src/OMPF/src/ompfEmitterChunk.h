//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #
//
//  (c) Copyright by Martin Loga
//
// This library is free software; you can redistribute it and or modify it   
// under the terms of the GNU Lesser General Public License as published by  
// the Free Software Foundation; either version 2.1 of the License, or (at   
// your option) any later version.                                           
// 
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
// 
// You should have received a copy of the GNU Lesser General Public          
// License along with this library; if not, write to the Free Software       
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA 
// 
// contact: martinloga@gmx.de  

#ifndef __EMITTERCHUNK__
#define __EMITTERCHUNK__

#include <ompfBaseChunk.h>

namespace OMPF
{

    /*! type of emitter
    */
    enum class OMPF_API OMPFEmitterType
    {
        Mesh,
        Point,
        Disc,
        Circle,
        Sphere,
        Square,
        Cube
    };

    /*! ompf emitter chunk
    */
	class OMPF_API ompfEmitterChunk : public ompfBaseChunk
	{

	public:

        /*! init base class
        */
        ompfEmitterChunk();

        /*! does nothing
        */
		virtual ~ompfEmitterChunk() = default;

        /*! \returns size of chunk in bytes

        \param settings the settings the size calculation is based on
        */
        virtual uint32 getSize(const ompfSettings& settings);

        /*! sets emitter size

        \param size the emitter size
        */
        void setSize(float32 size);

        /*! \returns emitter size
        */
        const float32 getSize() const;

        /*! sets type of emitter

        \param emitterType emitter type
        */
        void setType(OMPFEmitterType emitterType);

        /*! \returns type of emitter
        */
        OMPFEmitterType getType() const;

    private:

        /*! size of emitter
        */
        float32 _size;

        /*! type of emitter
        */
        OMPFEmitterType _type;

        /*! writes chunk to stream

        \param stream destination stream
        \param settings the settings how to write the chunk
        */
        virtual bool write(ofstream& stream, const ompfSettings& settings);

        /*! reads chunk from stream

        \param stream source stream
        \param settings the settings how to read the chunk
        */
        virtual bool read(ifstream& stream, ompfSettings& settings);

	};

}

#endif