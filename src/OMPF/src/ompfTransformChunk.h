//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #  3d model file format
//
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

#ifndef __TRANSFORMCHUNK__
#define __TRANSFORMCHUNK__

#include <ompfBaseChunk.h>

#include <iaMatrix.h>
using namespace IgorAux;

namespace OMPF
{

    /*! transformation chunk

    \todo need additional 64bit transform chunk
    */
	class OMPF_API ompfTransformChunk : public ompfBaseChunk
	{

    public:

        /*! does nothing
        */
        ompfTransformChunk();

        /*! does nothing
        */
        virtual ~ompfTransformChunk() = default;

        /*! \returns size of chunk in bytes

        \param settings the settings the size calculation is based on
        */
        virtual uint32 getSize(const ompfSettings& settings);

        /*! sets transformation matrix

        \param matrix transformation matrix
        */
        void setMatrix(const iaMatrixf& matrix);

        /*! returns transformation matrix

        \param[out] matrix returned transformation matrix
        */
        void getMatrix(iaMatrixf& matrix) const;

	private:

        /*! transformation matrix
        */
        iaMatrixf _matrix;

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