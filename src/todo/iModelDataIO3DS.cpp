#include <iLoader3DS.h>
#include <iTextureManager.h>
#include <iModelManager.h>
#include <igor/resources/model/iModel.h>

#include <string.h>

namespace Igor
{

#define SEEK_START 1900
#define SEEK_CURSOR 1901

#define MAIN3DS 0x4D4D
#define EDIT3DS 0x3D3D

#define EDIT_MATERIAL 0xAFFF
#define MAT_NAME 0xA000
#define MAT_TEXTURE 0xA200
#define TEX_NAME 0xA300
	//#define TEX_PERCENTAGE		0x0030

#define EDIT_OBJECT 0x4000
#define OBJ_TRIMESH 0x4100
#define OBJ_LIGHT 0x4600
#define OBJ_CAMERA 0x4700

#define LIT_OFF 0x4620
#define LIT_SPOT 0x4610

#define TRI_VERTEXLIST 0x4110
#define TRI_VERTEXOPTIONS 0x4111
#define TRI_FACELIST 0x4120
#define TRI_FACEMAPPING 0x4140
#define TRI_MATRIX 0x4160
#define TRI_MATERIAL 0x4130

#define RGB_COLOR 0x0010
#define COLOR24 0x0011
#define SPOTLIGHT 0x4610

	iLoader3DS::iLoader3DS()
	{
	}

	iLoader3DS::~iLoader3DS()
	{
		if (buffer)
			free(buffer);

		uint32 i;
		for (i = 0; i < vertexes.size(); ++i)
		{
			delete vertexes[i];
		}

		vertexes.clear();

		for (i = 0; i < materials.size(); ++i)
		{
			delete materials[i];
		}

		materials.clear();
	}

	void iLoader3DS::saveFile(string filename, iNodePtr model)
	{
		con_err("not implemented" << endl;
	}

	iModel *iLoader3DS::loadFile(string filename)
	{
		FILE *File;
		vertexcount = 0;
		trianglecount = 0;
		buffer = 0;

		File = fopen(filename.c_str(), "rb");
		if (File == 0)
		{
			con_err("can't open file \"" << filename << "\"" << endl;
			return 0;
		}

		buffer = (unsigned char *)malloc(2);

		if (fread(buffer, 2, 1, File) != 1)
		{
			fclose(File);
			if (buffer)
			{
				free(buffer);
				buffer = 0;
			}
			buffersize = 0;
			con_err("can't read file \"" << filename << "\"" << endl;
			return 0;
		}

		uint32 pos = strlen(filename.c_str());
		while ((filename[pos] != '/') && (filename[pos] != '\\') && (pos != 0))
		{
			pos--;
		}

		if (pos)
		{
			path[pos] = 0;
			do
			{
				pos--;
				path[pos] = filename[pos];
			} while (pos > 0);
		}
		else
		{
			path[0] = 0;
		}

		if ((buffer[0] == 0x4D) && (buffer[1] == 0x4D))
		{
			con << "loading 3DS model \"" << filename << "\" ..." << endl;
			fseek(File, 0, SEEK_END);
			buffersize = ftell(File);
			fseek(File, 0, SEEK_SET);
			buffer = (unsigned char *)realloc(buffer, buffersize);

			if (fread(buffer, buffersize, 1, File) != 1)
			{
				fclose(File);
				if (buffer)
				{
					free(buffer);
					buffer = 0;
				}
				buffersize = 0;
				con_err("can't read file \"" << filename << "\"" << endl;
				return 0;
			}
			fclose(File);

			if (!analyze3DS())
			{
				con_err("no valid 3DS file" << endl;
				if(buffer)
				{
					free(buffer);
					buffer = 0;
				}
				buffersize=0;
				return 0;
			}

			if (buffer)
			{
				free(buffer);
				buffer = 0;
				buffersize = 0;
			}

			/*for(uint32 i=0;i<meshes.size();i++)
			{
				meshes[i]->calcNormals();
			}*/
			// not the right place for that

			con << filename << " loaded with: ";
			con << vertexcount << " vertices / " << trianglecount << " triangles / " << meshes.size() << " meshes" << endl;

			return 0;
		}
		else
		{
			fclose(File);
			if (buffer)
				free(buffer);
			return 0;
		}

		iModel *model = createModel(filename);
		for (int i = 0; i < meshes.size(); ++i)
		{
			model->addMesh(meshes[i]);
		}

		return model;
	}

	bool iLoader3DS::analyze3DS()
	{
		char completepath[256];
		bufferpos = 0;
		Chunk mainchunk;
		Chunk editchunk;

		editchunk.id = EDIT3DS;
		mainchunk = readChunk();

		if (mainchunk.id != MAIN3DS)
			return false;

		if (!findChunk(editchunk, mainchunk)) // Sucht editchunk innerhalb mainchunk
			return false;

		Chunk matchunk, matnamechunk, texchunk, texnamechunk;
		char MaterialName[100];
		char ObjectName[100];
		char TextureName[100];

		matchunk.id = EDIT_MATERIAL;

		uint32 pushBuffer = bufferpos; // Position merken fr zweiten Durchlauf

		while (findChunk(matchunk, editchunk)) // Suche nach allen Materialien innerhalb editchunk
		{
			uint32 pushBuffer2 = bufferpos;
			matnamechunk.id = MAT_NAME;
			if (findChunk(matnamechunk, matchunk))
			{
				readASCIIString(MaterialName, 100);
				//Write("\nloading material \"%s\"",MaterialName);
				skipChunk(matchunk);
			}
			bufferpos = pushBuffer2;

			texchunk.id = MAT_TEXTURE;
			if (findChunk(texchunk, matchunk))
			{
				texnamechunk.id = TEX_NAME;
				if (findChunk(texnamechunk, texchunk))
				{
					readASCIIString(TextureName, 100);
					Material *temp = new Material;
					materials.push_back(temp);

					sprintf(completepath, "%s/%s", path, TextureName);
					temp->texture = iTextureManager::getInstance().loadFile(completepath);

					strcpy(temp->name, MaterialName);
					skipChunk(texnamechunk);
				}
				skipChunk(texchunk);
			}
		}

		Chunk objchunk, meshchunk;
		bufferpos = pushBuffer;

		// reading object meshes
		con << "loading Meshs" << endl;
		objchunk.id = EDIT_OBJECT;
		{
			while (findChunk(objchunk, editchunk))
			{
				readASCIIString(ObjectName, 100);
				con << " \"" << ObjectName << "\";";
				meshchunk = readChunk();
				if (meshchunk.id == OBJ_TRIMESH)
					readMesh(objchunk);
				skipChunk(objchunk);
			}
		}

		return true;
	}

	iLoader3DS::Chunk iLoader3DS::readChunk()
	{
		Chunk chunk;
		chunk.id = readShort();
		uint32 length = readLong();
		chunk.start = pos();
		chunk.end = chunk.start + length - 6; //-6 weil start hinter ID und length liegt
		return chunk;
	}

	bool iLoader3DS::findChunk(iLoader3DS::Chunk &target, iLoader3DS::Chunk parent)
	{
		if (pos() >= parent.end)
			return false;
		Chunk chunk;
		chunk = readChunk();
		while ((chunk.id != target.id) && (chunk.end <= parent.end))
		{
			skipChunk(chunk);
			if (chunk.end >= parent.end)
				break;
			if (chunk.id == 0)
				break; // added because of "hanging"-bug.
			chunk = readChunk();
		}
		if (chunk.id == target.id)
		{
			target.start = chunk.start;
			target.end = chunk.end;
			return true;
		}
		return false;
	}

	unsigned short iLoader3DS::readShort()
	{
		if ((buffer != 0) && (buffersize != 0) && ((bufferpos + 2) < buffersize))
		{
			unsigned short *w = (unsigned short *)(buffer + bufferpos);
			unsigned short s = *w;
			bufferpos += 2;
			return s;
		}
		eof = true;
		return 0;
	}

	uint32 iLoader3DS::readLong()
	{
		if ((buffer != 0) && (buffersize != 0) && ((bufferpos + 4) < buffersize))
		{
			uint32 *w = (uint32 *)(buffer + bufferpos);
			uint32 s = *w;
			bufferpos += 4;
			return s;
		}
		eof = true;
		return 0;
	}

	char iLoader3DS::readChar()
	{
		if ((buffer != 0) && (buffersize != 0) && ((bufferpos + 1) < buffersize))
		{
			char s = (char)*(buffer + bufferpos);
			bufferpos += 1;
			return s;
		}
		eof = true;
		return 0;
	}

	float32 iLoader3DS::readfloat32()
	{
		if ((buffer != 0) && (buffersize != 0) && ((bufferpos + 4) < buffersize))
		{
			float32 *w = (float32 *)(buffer + bufferpos);
			float32 s = *w;
			bufferpos += 4;
			return s;
		}
		eof = true;
		return 0.0f;
	}

	void iLoader3DS::readASCIIString(char *buf, unsigned short max_count)
	{
		unsigned short count = 0;
		if ((buffer == 0) || (buffersize == 0) || (bufferpos >= buffersize))
		{
			eof = true;
			return;
		}

		char c = readChar();
		while ((c != 0) && (count < max_count - 1))
		{
			buf[count] = c;
			count++;
			c = readChar();
		}
		buf[count] = 0;
	}

	void iLoader3DS::seek(uint32 offset, uint32 origin)
	{
		if (origin == SEEK_START)
			bufferpos = offset;
		if (origin == SEEK_CURSOR)
			bufferpos += offset;
		if (bufferpos < 0)
			bufferpos = 0;
		if (bufferpos >= buffersize)
			bufferpos = buffersize - 1;
		eof = false;
	}

	uint32 iLoader3DS::pos()
	{
		return bufferpos;
	}

	void iLoader3DS::skipChunk(iLoader3DS::Chunk chunk)
	{
		bufferpos = chunk.end;
	}

	void iLoader3DS::readMesh(iLoader3DS::Chunk parent)
	{
		char MaterialName[100];
		bool fixedvertex = false;
		uint32 Tris = 0, Vers = 0;
		unsigned short count, i, k;

		Chunk mat;
		iVector3f xAxis, yAxis, zAxis, pos;
		iMesh *mesh = new iMesh();
		meshes.push_back(mesh);

		//	CMaterial *temp2;
		Vertex *vertex;

		for (i = 0; i < vertexes.size(); ++i)
		{
			delete vertexes[i];
		}

		vertexes.clear();

		Chunk chunk = readChunk();
		while (chunk.end <= parent.end)
		{
			switch (chunk.id)
			{
			case TRI_VERTEXLIST:
				count = readShort();
				for (i = 0; i < count; i++)
				{
					vertexcount++;
					Vers++;
					vertex = new Vertex;
					vertex->posx = readfloat32();
					vertex->posz = -readfloat32();
					vertex->posy = readfloat32();
					vertexes.push_back(vertex);
				}
				//			Write(".");
				break;
			case TRI_FACEMAPPING:
				count = readShort();
				for (i = 0; i < count; i++)
				{
					if (vertexes.size() > i)
					{
						vertexes[i]->texx = readfloat32();
						vertexes[i]->texy = readfloat32();
					}
					else
					{
						readfloat32();
						readfloat32();
					}
				}
				//			Write(".");
				break;
			case TRI_FACELIST:
				if (!fixedvertex)
				{
					mesh->setNormal3f(0, 1, 0);
					for (i = 0; i < vertexes.size(); i++)
					{
						vertex = vertexes[i];
						mesh->setTexCoord2f(vertex->texx, vertex->texy, 0);
						mesh->setVertex3f(vertex->posx, vertex->posy, vertex->posz);
					}
					fixedvertex = true;
				}

				count = readShort();
				for (k = 0; k < count; k++)
				{
					Tris++;
					unsigned short a = readShort(), b = readShort(), c = readShort();
					mesh->setTriangle(a, b, c);
					trianglecount++;
					readShort(); // dummy
				}
				mat.id = TRI_MATERIAL;
				if (findChunk(mat, chunk))
				{
					readASCIIString(MaterialName, 100);

					for (i = 0; i < materials.size(); i++)
					{
						if (strcmp(materials[i]->name, MaterialName) == 0)
						{
							mesh->bindTexture(materials[i]->texture, 0);
						}
					}
					skipChunk(chunk);
				}
				break;

			default:;
			}
			skipChunk(chunk);
			if (chunk.end >= parent.end)
				break;
			chunk = readChunk();
		}
	}

} // namespace Igor
