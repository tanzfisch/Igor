//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #  3d model file format
//
// (c) Copyright 2014-2015 by Martin Loga
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

	class OMPF_API ompfExternalReferenceChunk : public ompfBaseChunk
	{

	public:

		ompfExternalReferenceChunk();
		virtual ~ompfExternalReferenceChunk() = default;

        virtual uint32 getSize(const ompfSettings& settings);

		void setFilename(const iaString& filename);
		iaString getFilename() const;

    private:

        virtual bool write(ofstream& file, const ompfSettings& settings);
        virtual bool read(ifstream& file, ompfSettings& settings);

		iaString _filename;

	};

}

#endif