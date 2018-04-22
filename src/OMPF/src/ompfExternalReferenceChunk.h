//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #  3d model file format
//
// (c) Copyright 2012-2018 by Martin Loga
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

#ifndef __EXTERNALREFERENCECHUNK__
#define __EXTERNALREFERENCECHUNK__

#include <ompfBaseChunk.h>

namespace OMPF
{

    /*! chunk that contains an external reference to an other file
    */
	class OMPF_API ompfExternalReferenceChunk : public ompfBaseChunk
	{

	public:

        /*! does nothing
        */
		ompfExternalReferenceChunk();

        /*! does nothing
        */
		virtual ~ompfExternalReferenceChunk() = default;

        /*! \returns size of chunk in bytes

        \param settings the settings the chunk's size depends on
        */
        virtual uint32 getSize(const ompfSettings& settings);

        /*! sets file name of external reference

        \param filename the file name
        */
		void setFilename(const iaString& filename);

        /*! \returns file name of external reference
        */
		iaString getFilename() const;

    private:

        /*! file name of external reference
        */
        iaString _filename;

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