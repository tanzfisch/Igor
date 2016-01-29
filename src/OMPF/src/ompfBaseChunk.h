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

#ifndef __BASECHUNK__
#define __BASECHUNK__

#include <ompfDefines.h>

#include <iaSerializable.h>
#include <iaMemBlock.h>
#include <iaString.h>
using namespace IgorAux;

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

namespace OMPF
{

	class ompfSettings;

    /*! ompf base chunk. all chunks have to derive from this class
    */
	class OMPF_API ompfBaseChunk
	{
        
	public:

        /*! ctor 
        */
        ompfBaseChunk(OMPFChunkTypes typeID);

        /*! dtor does nothing
        */
        virtual ~ompfBaseChunk() = default;
        
        virtual bool write(ofstream& file, const ompfSettings& settings);
        virtual bool read(ifstream& file, ompfSettings& settings);

		void insertChunk(ompfBaseChunk* chunk);
		void removeChunk(ompfBaseChunk* chunk);

        bool hasParent() const;
        uint32 getParentID() const;
        void setParentID(const uint32 parentID);

		bool hasChildren() const;
		const vector<ompfBaseChunk*>& getChildren() const;

        void setID(const uint32 chunkID);
		uint32 getID() const;

        /*! \returns chunk size in bytes

        size can differ for different settings

        \param settings the settings to take in to account
        */
		virtual uint32 getSize(const ompfSettings& settings);

        OMPFChunkTypes getType() const;

        void setName(const iaString& name);
        iaString getName() const;

    private:

        OMPFChunkTypes _type = OMPFChunkTypes::Invalid;
        uint32 _chunkSize = 0;
        uint32 _ID = OMPFDefaultConfiguration::InvalidChunkID;
        uint32 _parentID = OMPFDefaultConfiguration::InvalidChunkID;
        iaString _name;

        vector<ompfBaseChunk*> _children;

	};

}

#endif