// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iModelDataIOOBJ.h>

#include <iTextureResourceFactory.h>
#include <iResourceManager.h>
#include <iaConsole.h>
#include <iModel.h>
#include <iMesh.h>
#include <iMeshBuilder.h>
#include <iTargetMaterial.h>

#include <iNode.h>
#include <iNodeMesh.h>
#include <iNodeFactory.h>

#include <IgorAux.h>
#include <iaFile.h>
using namespace IgorAux;

#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

namespace Igor
{

	iModelDataIOOBJ::iModelDataIOOBJ()
	{
        _name = "Wavefront";
        _identifier = "obj";

		OBJGroup defaultgroup;
		defaultgroup._name = "defaultgroup";
		_currentGroups.push_back(0);
		_groups.push_back(defaultgroup);

		_currentMaterial = -1;
	}

    iModelDataIO* iModelDataIOOBJ::createInstance()
    {
        iModelDataIOOBJ* result = new iModelDataIOOBJ();
        return static_cast<iModelDataIO*>(result);
    }

    iNode* iModelDataIOOBJ::importData(const iaString& filename, iModelDataInputParameter* parameter)
	{
        iNode* result = iNodeFactory::getInstance().createNode(iNodeType::iNode);
        result->setName("obj_root");

        if (!readFile(filename))
        {
            return 0;
        }

		if(getMaterialCount()==0)
		{
            con_debug_endl("no materials loaded");
            vector<iMeshBuilder*> meshes;
            iMeshBuilder* meshBuilder = new iMeshBuilder();
            if (parameter != nullptr)
            {
                meshBuilder->setJoinVertexes(parameter->_joinVertexes);
            }
            meshes.push_back(meshBuilder);

            transferToMeshBuilder(meshes);

            iNodeMesh* mesh = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
            mesh->setName("mesh");
            mesh->setMesh(meshes[0]->createMesh());

			result->insertNode(mesh);

            delete meshes[0];
            meshes.clear();
		}
		else
		{
			vector<iMeshBuilder*> meshBuilders;

			for(uint32 i=0;i<getMaterialCount();i++)
			{
                iMeshBuilder* meshBuilder = new iMeshBuilder();
                if (parameter != nullptr)
                {
                    meshBuilder->setJoinVertexes(parameter->_joinVertexes);
                }
                meshBuilders.push_back(meshBuilder);
			}

            transferToMeshBuilder(meshBuilders);

            for(uint32 i=0;i<meshBuilders.size();++i)
			{
                wstringstream stream;
                stream << "mesh" << setfill(L'0') << setw(4) << i+1;

                iNodeMesh* mesh = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh)); 
                mesh->setName(stream.str().data());
                if (parameter != nullptr)
                {
                    mesh->setKeepMesh(parameter->_keepMesh);
                }
                mesh->setMesh(meshBuilders[i]->createMesh());
				result->insertNode(mesh);

                mesh->getTargetMaterial()->setAmbient(getMaterial(i)->_ambient);
                mesh->getTargetMaterial()->setDiffuse(getMaterial(i)->_diffuse);
                mesh->getTargetMaterial()->setSpecular(getMaterial(i)->_specular);
                mesh->getTargetMaterial()->setShininess(getMaterial(i)->_shininess);

                if(getMaterial(i)->_texture != "")
                {
                    mesh->getTargetMaterial()->setTexture(iTextureResourceFactory::getInstance().requestFile(getMaterial(i)->_texture.getData()), 0);
                }

                delete meshBuilders[i];
			}

            meshBuilders.clear();
		}

        if (result != nullptr)
        {
            con_info("loaded obj", "\"" << filename << "\"");
        }

		return result;
	}

    void iModelDataIOOBJ::transferToMeshBuilder(vector<iMeshBuilder*>& meshBuilders)
    {
        con_assert(meshBuilders.size() > 0, "no mesh builder defined");

        iMeshBuilder *meshBuilder = nullptr;

        bool hasNormals = _normals.size() != 0 ? true : false;
        bool hasTexCoord = _texcoord.size() != 0 ? true : false;

        for (uint32 i = 0; i<_polygons.size(); i++)
        {
            OBJPolygon& poly = _polygons[i];

            meshBuilder = meshBuilders[poly._materialNum];

            // asuming poly is a triangle fan
            for (uint32 j = 0; j < poly._vertexes.size() - 2; j++)
            {
                uint32 a = meshBuilder->addVertex(_vertexes[poly._vertexes[0]._v]);
                uint32 b = meshBuilder->addVertex(_vertexes[poly._vertexes[j + 1]._v]);
                uint32 c = meshBuilder->addVertex(_vertexes[poly._vertexes[j + 2]._v]);

                if (hasNormals)
                {
                    meshBuilder->setNormal(a, _normals[poly._vertexes[0]._vn]);
                    meshBuilder->setNormal(b, _normals[poly._vertexes[j+1]._vn]);
                    meshBuilder->setNormal(c, _normals[poly._vertexes[j+2]._vn]);
                }

                if (hasTexCoord)
                {
                    meshBuilder->setTexCoord(a, _texcoord[poly._vertexes[0]._t], 0);
                    meshBuilder->setTexCoord(b, _texcoord[poly._vertexes[j + 1]._t], 0);
                    meshBuilder->setTexCoord(c, _texcoord[poly._vertexes[j + 2]._t], 0);
                }

                meshBuilder->addTriangle(a, b, c);
            }
        }

        for (uint32 i = 0; i < meshBuilders.size(); ++i)
        {
            if (!meshBuilders[i]->hasNormals())
            {
                meshBuilders[i]->calcNormals(true);
            }
        }
    }
    
	bool iModelDataIOOBJ::analyseAttributes(vector<iaString> &attributes)
	{
		if (attributes.size() == 0)
		{
			return true;
		}

		if(attributes[0] == "v")	// vertex
		{
			return readVertex(attributes);
		}
		else if(attributes[0] == "vt") // texcoord
		{
			return readTexcoord(attributes);
		}
		else if(attributes[0] == "vn") // normal
		{
			return readNormal(attributes);
		}
		else if(attributes[0] == "f") // face
		{
			return readFace(attributes);
		}
		else if(attributes[0] == "g")	// use groups
		{
			return readGroup(attributes);
		}
		else if (attributes[0] == "o")	// objects are also groups
		{
			return readGroup(attributes);
		}
		else if(attributes[0] == "usemtl") // use material
		{
			return readUseMaterial(attributes);
		}
		else if(attributes[0] == "mtllib") // load material lib
		{
			if(attributes.size()!=2) return false;
			return readMaterialFile(_pathOfModel+attributes[1]);
		}
		else if(attributes[0] == "newmtl") // new material
		{
			return readMaterial(attributes);
		}
		else if(attributes[0] == "Ns") //?
		{
            return readShininess(attributes);
		}
		else if(attributes[0] == "d") //?
		{
		}
		else if(attributes[0] == "Ni") //?
		{
		}
		else if(attributes[0] == "illum") //?
		{
		}
		else if(attributes[0] == "Ka") // ambient
		{
			return readAmbient(attributes);
		}
		else if(attributes[0] == "Kd") // diffuse
		{
			return readDiffuse(attributes);
		}
		else if(attributes[0] == "Ks") // specular
		{
			return readSpecular(attributes);
		}
		else if(attributes[0] == "map_Kd") // diffuse texture
		{
			return readTexture(attributes);
		}

		return true;
	}

	bool iModelDataIOOBJ::readMaterial(vector<iaString> &attributes)
	{
        con_assert(attributes.size() == 2, "invalid count of attributes");

		OBJMaterial result;
		result._name = attributes[1];
		_materials.push_back(result);
		_currentMaterial = static_cast<int32>(_materials.size()-1);

		return true;
	}

    bool iModelDataIOOBJ::readShininess(vector<iaString> &attributes)
    {
        con_assert(attributes.size() == 2, "invalid count of attributes");

        if (_materials.size() <= 0)
        {
            return false;
        }

        _materials[_currentMaterial]._shininess = (float32)_wtof(attributes[1].getData());

        return true;
    }

	bool iModelDataIOOBJ::readAmbient(vector<iaString> &attributes)
	{
        con_assert(attributes.size() == 4, "invalid count of attributes");

        if (_materials.size() <= 0)
        {
            return false;
        }
		
        _materials[_currentMaterial]._ambient._r = (float32)_wtof(attributes[1].getData());
        _materials[_currentMaterial]._ambient._g = (float32)_wtof(attributes[2].getData());
        _materials[_currentMaterial]._ambient._b = (float32)_wtof(attributes[3].getData());

        return true;
	}

	bool iModelDataIOOBJ::readDiffuse(vector<iaString> &attributes)
	{
        con_assert(attributes.size() == 4, "invalid count of attributes");

        if (_materials.size() <= 0)
        {
            return false;
        }

		_materials[_currentMaterial]._diffuse._r = (float32)_wtof(attributes[1].getData());
		_materials[_currentMaterial]._diffuse._g = (float32)_wtof(attributes[2].getData());
		_materials[_currentMaterial]._diffuse._b = (float32)_wtof(attributes[3].getData());
		return true;
	}

	bool iModelDataIOOBJ::readSpecular(vector<iaString> &attributes)
	{
        con_assert(attributes.size() == 4, "invalid count of attributes");

        if (_materials.size() <= 0)
        {
            return false;
        }

		_materials[_currentMaterial]._specular._r = (float32)_wtof(attributes[1].getData());
		_materials[_currentMaterial]._specular._g = (float32)_wtof(attributes[2].getData());
		_materials[_currentMaterial]._specular._b = (float32)_wtof(attributes[3].getData());
		return true;
	}

	bool iModelDataIOOBJ::readTexture(vector<iaString> &attributes)
	{
        con_assert(attributes.size() == 2, "invalid count of attributes");
		
        if (_materials.size() <= 0)
        {
            return false;
        }

		_materials[_currentMaterial]._texture = attributes[1];
		return true;
	}

	bool iModelDataIOOBJ::readMaterialFile(iaString filename)
	{
		ifstream file;
		if (iaFile::exist(filename))
		{
			file.open(iResourceManager::getInstance().getPath(filename).getData(), ifstream::in);
			vector<iaString> attributes;

			if (file.is_open())
			{
				string line;

				while (getline(file, line, '\n'))
				{
					attributes.clear();

					iaString result(line.c_str());
					result.split(L" \n\r\t", attributes);
					analyseAttributes(attributes);
				}

				file.close();

				return true;
			}
		}
		
		return false;
	}

	bool iModelDataIOOBJ::readGroup(vector<iaString> &attributes)
	{
        con_assert(attributes.size() >= 2, "unexpected attribute count");

		_currentGroups.clear();

		bool found = false;
		for(uint32 j=1;j<attributes.size();j++)
		{
			found = false;
			for(uint32 i=0;i<_groups.size();i++)
			{
                if (_groups[i]._name == attributes[j])
                {
                    found = true;
                }
			}

			if(!found)
			{
				OBJGroup result;
				result._name = attributes[j];
				_groups.push_back(result);
			}

			for(uint32 i=0;i<_groups.size();i++)
			{
				if(_groups[i]._name == attributes[j])
				{
					_currentGroups.push_back(i);
				}
			}
		}

		return true;
	}

	bool iModelDataIOOBJ::readFace(vector<iaString> &attributes)
	{
		OBJPolygon result;

        for (uint32 i = 1; i < attributes.size(); i++)
        {
            result._vertexes.push_back(readVertex(attributes[i]));
        }

        if (_currentMaterial != -1)
        {
            result._materialNum = _currentMaterial;
        }
        else
        {
            result._materialNum = 0;
        }

		_polygons.push_back(result);

		for(uint32 i=0;i<_currentGroups.size();i++)
		{
			_groups[_currentGroups[i]]._polygons.push_back(static_cast<uint32>(_polygons.size()-1));
		}

		return true;
	}

    iModelDataIOOBJ::OBJVertex iModelDataIOOBJ::readVertex(iaString attributes)
    {
        OBJVertex result;
        result._v = 0;
        result._t = -1;
        result._vn = -1;

        vector<iaString> values;
        attributes.split("/", values, iaStringSplitMode::RetriveAllEmpties);

        con_assert(values.size() < 4, "corrupt value count");
        con_assert(values.size() > 0, "corrupt value count");

		if(values[0] != "")
		{
            result._v = _wtoi(values[0].getData()) - 1;
            con_assert(result._v >= 0, "out of range");
            con_assert(result._v < _vertexes.size(), "out of range");
		}

        if (values.size() > 1 &&
            values[1] != "")
		{
            result._t = _wtoi(values[1].getData()) - 1;
            con_assert(result._t >= 0, "out of range");
            con_assert(result._t < _texcoord.size(), "out of range");
		}

        if (values.size() > 2 &&
            values[2] != "")
        {
            result._vn = _wtoi(values[2].getData()) - 1;
            con_assert(result._vn >= 0, "out of range");
            con_assert(result._vn < _normals.size(), "out of range");
        }

		return result;
	}

	bool iModelDataIOOBJ::readUseMaterial(vector<iaString> &attributes)
	{
		if(attributes.size() != 2) return false;

		for(uint32 i=0;i<_materials.size();i++)
		{
			if(_materials[i]._name == attributes[1])
			{
				_currentMaterial = i;
				return true;
			}
		}

		return false;
	}

	bool iModelDataIOOBJ::readVertex(vector<iaString> &attributes)
	{
        con_assert(attributes.size() == 4, "invalid attributes count");

		iaVector3f result;
		result[0] = (float32)_wtof(attributes[1].getData());
		result[1] = (float32)_wtof(attributes[2].getData());
		result[2] = (float32)_wtof(attributes[3].getData());
		_vertexes.push_back(result);
		return true;
	}

	bool iModelDataIOOBJ::readTexcoord(vector<iaString> &attributes)
	{
        con_assert(attributes.size() == 3, "invalid attributes count");

		iaVector2f result;
		result[0] = (float32)_wtof(attributes[1].getData());
		result[1] = (float32)_wtof(attributes[2].getData());
		_texcoord.push_back(result);
		return true;
	}

	bool iModelDataIOOBJ::readNormal(vector<iaString> &attributes)
	{
        con_assert(attributes.size() == 4, "invalid attributes count");

		iaVector3f result;
		result[0] = (float32)_wtof(attributes[1].getData());
		result[1] = (float32)_wtof(attributes[2].getData());
		result[2] = (float32)_wtof(attributes[3].getData());
		_normals.push_back(result);
		return true;
	}

	bool iModelDataIOOBJ::readFile(const iaString& filename)
	{
		iaFile file(filename);
		_pathOfModel = file.getPath();

		ifstream fileStream;
		fileStream.open(filename.getData(), ifstream::in);
		vector<iaString> attributes;

		if(fileStream.is_open())
		{
			string line;

			while(getline(fileStream,line,'\n'))
			{
				attributes.clear();
                iaString result(line.c_str());
                result.split(L" \n\r\t", attributes);
				analyseAttributes(attributes);
			}

			fileStream.close();
		}
		else return false;

		return true;
	}

	iModelDataIOOBJ::OBJMaterial* iModelDataIOOBJ::getMaterial(uint32 materialnum)
	{
		if(materialnum >= _materials.size()) return 0;
		return &_materials[materialnum];
	}

	iaVector3f* iModelDataIOOBJ::getVertex(uint32 index)
	{
		if(index >= _vertexes.size()) return 0;
		return &_vertexes[index];
	}

	iaVector3f* iModelDataIOOBJ::getNormal(uint32 index)
	{
		if(index >= _normals.size()) return 0;
		return &_normals[index];
	}

	iaVector2f* iModelDataIOOBJ::getTexCoord(uint32 index)
	{
		if(index >= _texcoord.size()) return 0;
		return &_texcoord[index];
	}

	iModelDataIOOBJ::OBJPolygon* iModelDataIOOBJ::getPolygon(uint32 index)
	{
		if(index >= _polygons.size()) return 0;
		return &_polygons[index];
	}

	uint32 iModelDataIOOBJ::getPolygonCount()
	{
		return static_cast<uint32>(_polygons.size());
	}

	uint32 iModelDataIOOBJ::getMaterialCount()
	{
		return static_cast<uint32>(_materials.size());
	}

	uint32 iModelDataIOOBJ::getVertexCount()
	{
		return static_cast<uint32>(_vertexes.size());
	}

	uint32 iModelDataIOOBJ::getNormalCount()
	{
		return static_cast<uint32>(_normals.size());
	}

	uint32 iModelDataIOOBJ::getTexCoordCount()
	{
		return static_cast<uint32>(_texcoord.size());
	}

}
