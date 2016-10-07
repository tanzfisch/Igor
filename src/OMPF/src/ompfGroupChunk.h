//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #  3d model file format
//
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __GROUPCHUNK__
#define __GROUPCHUNK__

#include <ompfBaseChunk.h>

namespace OMPF
{

    /*! group chunk wich simply groups other chunks
    */
	class OMPF_API ompfGroupChunk : public ompfBaseChunk
	{

    public:

        /*! does nothing
        */
        ompfGroupChunk();

        /*! does nothing
        */
        virtual ~ompfGroupChunk() = default;

        /*! \returns size of chunk

        \param settings the settings to use to calculate the size
        */
        virtual uint32 getSize(const ompfSettings& settings);

    private:

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