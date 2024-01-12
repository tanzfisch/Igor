//
//  ###
//   #
//   #  #### #### ##
//   #  #  # #  # #
//  ### #### #### #
//         #
//       ###
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

#ifndef __iLOADER3DS__
#define __iLOADER3DS__

#include <iLoader.h>
#include <iModelManager.h>
#include <iTextureOld.h>

#include <vector>


namespace Igor
{

	class iModelData;
	
    /*! 
    */
	class iLoader3DS : public iLoader
	{
		struct Chunk
		{
			unsigned short id;
			uint32 start;
			uint32 end;
		};

		struct Material
		{
			char name[100];
			iRefTexture texture;
		};

		struct Vertex
		{
			float32 posx,posy,posz;
			float32 texx,texy;
			float32 colr,colg,colb;
			float32 norx,nory,norz;
		};

	private:

		char path[256];

		std::vector<iMesh*> meshes;

		unsigned char *buffer;
		uint32 buffersize;
		uint32 bufferpos;
		uint32 vertexcount;
		uint32 trianglecount;
		bool eof;

		std::vector<ShaderMaterial*> materials;
		std::vector<Vertex*> vertexes;

		void readMesh(iLoader3DS::Chunk parent);
		iLoader3DS::Chunk readChunk();

		bool findChunk(iLoader3DS::Chunk &target, iLoader3DS::Chunk parent);
		void skipChunk(iLoader3DS::Chunk chunk);
		void seek(uint32 offset, uint32 origin);
		uint32 pos();

		unsigned short readShort();
		uint32 readLong();
		char readChar();
		float32 readfloat32();
		void readASCIIString(char *buf, unsigned short max_count);

		bool analyze3DS();

	public:

		iModelData* loadFile(string filename);
		void saveFile(string filename, iModel_Old* model);

		iLoader3DS();
		~iLoader3DS();
	};

}

#endif
