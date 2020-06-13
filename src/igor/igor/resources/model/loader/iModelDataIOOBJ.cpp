// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/model/loader/iModelDataIOOBJ.h>

#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/resources/iResourceManager.h>
#include <iaux/system/iaConsole.h>
#include <igor/resources/model/iModel.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/material/iTargetMaterial.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/scene/nodes/iNodeManager.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace igor
{

	iModelDataIOOBJ::iModelDataIOOBJ()
	{
		_name = iaString("Wavefront");
		_identifier = iaString("obj");

		iaString groupName = "mesh";
		groupName += iaString::toString(_nextID++);

		// create a valid section in case there is no groups defined in the obj file
		_currentGroups.push_back(groupName);
		_sections[groupName] = Section();
		_currentSections.push_back(groupName);

		// also create the default material which is expected by obj files
		_currentMaterial = "default";
		_materials[_currentMaterial] = OBJMaterial();
	}

	iModelDataIO *iModelDataIOOBJ::createInstance()
	{
		iModelDataIOOBJ *result = new iModelDataIOOBJ();
		return static_cast<iModelDataIO *>(result);
	}

	iNodePtr iModelDataIOOBJ::importData(const iaString &filename, iModelDataInputParameter *parameter)
	{
		iNodePtr result = iNodeManager::getInstance().createNode<iNode>();
		result->setName("obj_root");

		if (!readFile(filename))
		{
			return 0;
		}

		std::vector<iMeshBuilder *> meshBuilders;

		for (auto section : _sections)
		{
			auto &meshBuilder = section.second._meshBuilder;

			if (parameter != nullptr)
			{
				meshBuilder.setJoinVertexes(parameter->_joinVertexes);
			}

			// transfer polygons to mesh builder
			transferToMeshBuilder(section.second);

			// calc normals if needed
			if (!meshBuilder.hasNormals())
			{
				meshBuilder.calcNormals(true);
			}

			auto mesh = meshBuilder.createMesh();
			if (mesh == nullptr)
			{
				continue;
			}

			// create mesh node
			iNodeMesh *meshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
			meshNode->setName(section.first);

			if (parameter != nullptr)
			{
				meshNode->setKeepMesh(parameter->_keepMesh);
			}

			meshNode->setMesh(mesh);
			result->insertNode(meshNode);

			auto material = getMaterial(section.second._materialName);
			if (material == nullptr)
			{
				continue;
			}

			meshNode->getTargetMaterial()->setAmbient(material->_ambient);
			meshNode->getTargetMaterial()->setDiffuse(material->_diffuse);
			meshNode->getTargetMaterial()->setSpecular(material->_specular);
			meshNode->getTargetMaterial()->setShininess(material->_shininess);

			if (material->_texture != "")
			{
				meshNode->getTargetMaterial()->setTexture(iTextureResourceFactory::getInstance().requestFile(material->_texture.getData()), 0);
			}
		}

		if (result != nullptr)
		{
			con_info("loaded obj \"" << filename << "\"");
		}

		return result;
	}

	void iModelDataIOOBJ::transferToMeshBuilder(Section &section)
	{
		auto polygons = section._polygons;
		auto &meshBuilder = section._meshBuilder;

		for (auto polygon : polygons)
		{
			bool hasNormals = polygon._vertexes[0]._vn != -1 ? true : false;
			bool hasTexCoord = polygon._vertexes[0]._t != -1 ? true : false;

			// asuming poly is a triangle fan
			for (uint32 j = 0; j < polygon._vertexes.size() - 2; j++)
			{
				uint32 a = meshBuilder.addVertex(_vertexes[polygon._vertexes[0]._v]);
				uint32 b = meshBuilder.addVertex(_vertexes[polygon._vertexes[j + 1]._v]);
				uint32 c = meshBuilder.addVertex(_vertexes[polygon._vertexes[j + 2]._v]);

				if (hasNormals)
				{
					meshBuilder.setNormal(a, _normals[polygon._vertexes[0]._vn]);
					meshBuilder.setNormal(b, _normals[polygon._vertexes[j + 1]._vn]);
					meshBuilder.setNormal(c, _normals[polygon._vertexes[j + 2]._vn]);
				}

				if (hasTexCoord)
				{
					meshBuilder.setTexCoord(a, _texcoord[polygon._vertexes[0]._t], 0);
					meshBuilder.setTexCoord(b, _texcoord[polygon._vertexes[j + 1]._t], 0);
					meshBuilder.setTexCoord(c, _texcoord[polygon._vertexes[j + 2]._t], 0);
				}

				meshBuilder.addTriangle(a, b, c);
			}
		}
	}

	bool iModelDataIOOBJ::analyseAttributes(std::vector<iaString> &attributes)
	{
		if (attributes.size() == 0)
		{
			return true;
		}

		if (attributes[0] == "v") // vertex
		{
			return readVertex(attributes);
		}
		else if (attributes[0] == "vt") // texcoord
		{
			return readTexcoord(attributes);
		}
		else if (attributes[0] == "vn") // normal
		{
			return readNormal(attributes);
		}
		else if (attributes[0] == "f") // face
		{
			return readFace(attributes);
		}
		else if (attributes[0] == "g") // use groups
		{
			return readGroup(attributes);
		}
		else if (attributes[0] == "o") // interpret objects as groups
		{
			return readGroup(attributes);
		}
		else if (attributes[0] == "usemtl") // use material
		{
			return readUseMaterial(attributes);
		}
		else if (attributes[0] == "mtllib") // load material lib
		{
			if (attributes.size() != 2)
				return false;
			return readMaterialFile(_pathOfModel + attributes[1]);
		}
		else if (attributes[0] == "newmtl") // new material
		{
			return readMaterial(attributes);
		}
		else if (attributes[0] == "Ns") //?
		{
			return readShininess(attributes);
		}
		else if (attributes[0] == "d") //?
		{
		}
		else if (attributes[0] == "Ni") //?
		{
		}
		else if (attributes[0] == "illum") //?
		{
		}
		else if (attributes[0] == "Ka") // ambient
		{
			return readAmbient(attributes);
		}
		else if (attributes[0] == "Kd") // diffuse
		{
			return readDiffuse(attributes);
		}
		else if (attributes[0] == "Ks") // specular
		{
			return readSpecular(attributes);
		}
		else if (attributes[0] == "map_Kd") // diffuse texture
		{
			return readTexture(attributes);
		}

		return true;
	}

	bool iModelDataIOOBJ::readShininess(std::vector<iaString> &attributes)
	{
		con_assert(attributes.size() == 2, "invalid count of attributes");

		if (_materials.size() <= 0)
		{
			return false;
		}

		_materials[_currentMaterial]._shininess = (float32)iaString::toFloat(attributes[1]);

		return true;
	}

	bool iModelDataIOOBJ::readAmbient(std::vector<iaString> &attributes)
	{
		con_assert(attributes.size() == 4, "invalid count of attributes");

		if (_materials.size() <= 0)
		{
			return false;
		}

		_materials[_currentMaterial]._ambient._r = (float32)iaString::toFloat(attributes[1]);
		_materials[_currentMaterial]._ambient._g = (float32)iaString::toFloat(attributes[2]);
		_materials[_currentMaterial]._ambient._b = (float32)iaString::toFloat(attributes[3]);

		return true;
	}

	bool iModelDataIOOBJ::readDiffuse(std::vector<iaString> &attributes)
	{
		con_assert(attributes.size() == 4, "invalid count of attributes");

		if (_materials.size() <= 0)
		{
			return false;
		}

		_materials[_currentMaterial]._diffuse._r = (float32)iaString::toFloat(attributes[1]);
		_materials[_currentMaterial]._diffuse._g = (float32)iaString::toFloat(attributes[2]);
		_materials[_currentMaterial]._diffuse._b = (float32)iaString::toFloat(attributes[3]);
		return true;
	}

	bool iModelDataIOOBJ::readSpecular(std::vector<iaString> &attributes)
	{
		con_assert(attributes.size() == 4, "invalid count of attributes");

		if (_materials.size() <= 0)
		{
			return false;
		}

		_materials[_currentMaterial]._specular._r = (float32)iaString::toFloat(attributes[1]);
		_materials[_currentMaterial]._specular._g = (float32)iaString::toFloat(attributes[2]);
		_materials[_currentMaterial]._specular._b = (float32)iaString::toFloat(attributes[3]);
		return true;
	}

	bool iModelDataIOOBJ::readTexture(std::vector<iaString> &attributes)
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
		std::ifstream file;
		if (iaFile::exist(filename))
		{
			iaString path = iResourceManager::getInstance().getPath(filename);
			char temp[1024];
			path.getData(temp, 1024);
			file.open(temp, std::ifstream::in);
			std::vector<iaString> attributes;

			if (file.is_open())
			{
				std::string line;

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

	bool iModelDataIOOBJ::readMaterial(std::vector<iaString> &attributes)
	{
		con_assert(attributes.size() >= 2, "invalid count of attributes");

		_currentMaterial = attributes[1];
		_materials[_currentMaterial] = OBJMaterial();

		return true;
	}

	bool iModelDataIOOBJ::readGroup(std::vector<iaString> &attributes)
	{
		_currentSections.clear();
		_currentGroups.clear();
		_currentMaterial.clear();

		auto iterAttribute = attributes.begin();
		iterAttribute++;

		while (iterAttribute != attributes.end())
		{
			iaString groupName = (*iterAttribute);

			if (groupName == "(null)")
			{
				groupName = "mesh";
				groupName += iaString::toString(_nextID++);
			}

			auto iter = _sections.find(groupName);
			if (iter == _sections.end())
			{
				_sections[groupName] = Section();
			}

			_currentGroups.push_back(groupName);
			_currentSections.push_back(groupName);

			iterAttribute++;
		}

		return true;
	}

	bool iModelDataIOOBJ::readUseMaterial(std::vector<iaString> &attributes)
	{
		con_assert(attributes.size() == 2, "invalid count of attributes");

		if (attributes.size() != 2)
			return false;

		auto iter = _materials.find(attributes[1]);
		if (iter != _materials.end())
		{
			if (_currentMaterial != iter->first)
			{
				_currentMaterial = iter->first;
				_currentSections.clear();

				for (auto groupName : _currentGroups)
				{
					iaString sectionName = groupName;
					sectionName += "_";
					sectionName += _currentMaterial;

					auto iter = _sections.find(sectionName);
					if (iter == _sections.end())
					{
						_sections[sectionName] = Section();
						_sections[sectionName]._materialName = _currentMaterial;
					}

					_currentSections.push_back(sectionName);
				}
			}

			return true;
		}

		return false;
	}

	bool iModelDataIOOBJ::readFace(std::vector<iaString> &attributes)
	{
		OBJPolygon polygon;

		// create polygon
		for (uint32 i = 1; i < attributes.size(); i++)
		{
			polygon._vertexes.push_back(readVertex(attributes[i]));
		}

		// add polygon to current groups
		for (auto sectionName : _currentSections)
		{
			_sections[sectionName]._polygons.push_back(polygon);
		}

		return true;
	}

	iModelDataIOOBJ::OBJVertex iModelDataIOOBJ::readVertex(iaString attributes)
	{
		OBJVertex result;
		result._v = 0;
		result._t = -1;
		result._vn = -1;

		std::vector<iaString> values;
		attributes.split("/", values, iaStringSplitMode::RetriveAllEmpties);

		con_assert(values.size() < 4, "corrupt value count");
		con_assert(values.size() > 0, "corrupt value count");

		if (values[0] != "")
		{
			result._v = iaString::toInt(values[0]) - 1;
			con_assert(result._v >= 0, "out of range");
			con_assert(result._v < _vertexes.size(), "out of range");
		}

		if (values.size() > 1 &&
			values[1] != "")
		{
			result._t = iaString::toInt(values[1]) - 1;
			con_assert(result._t >= 0, "out of range");
			con_assert(result._t < _texcoord.size(), "out of range");
		}

		if (values.size() > 2 &&
			values[2] != "")
		{
			result._vn = iaString::toInt(values[2]) - 1;
			con_assert(result._vn >= 0, "out of range");
			con_assert(result._vn < _normals.size(), "out of range");
		}

		return result;
	}

	bool iModelDataIOOBJ::readVertex(std::vector<iaString> &attributes)
	{

		con_assert(attributes.size() >= 4, "invalid attributes count");

		iaVector3f result;
		result[0] = (float32)iaString::toFloat(attributes[1]);
		result[1] = (float32)iaString::toFloat(attributes[2]);
		result[2] = (float32)iaString::toFloat(attributes[3]);
		_vertexes.push_back(result);
		return true;
	}

	bool iModelDataIOOBJ::readTexcoord(std::vector<iaString> &attributes)
	{
		con_assert(attributes.size() >= 3, "invalid attributes count");

		iaVector2f result;
		result[0] = (float32)iaString::toFloat(attributes[1]);
		result[1] = (float32)iaString::toFloat(attributes[2]);
		_texcoord.push_back(result);
		return true;
	}

	bool iModelDataIOOBJ::readNormal(std::vector<iaString> &attributes)
	{
		con_assert(attributes.size() >= 4, "invalid attributes count");

		iaVector3f result;
		result[0] = (float32)iaString::toFloat(attributes[1]);
		result[1] = (float32)iaString::toFloat(attributes[2]);
		result[2] = (float32)iaString::toFloat(attributes[3]);
		_normals.push_back(result);
		return true;
	}

	bool iModelDataIOOBJ::readFile(const iaString &filename)
	{
		iaFile file(filename);
		_pathOfModel = file.getPath();

		std::ifstream fileStream;
		char temp[1024];
		filename.getData(temp, 1024);
		fileStream.open(temp, std::ifstream::in);
		std::vector<iaString> attributes;

		if (fileStream.is_open())
		{
			std::string line;

			while (getline(fileStream, line, '\n'))
			{
				attributes.clear();
				iaString result(line.c_str());
				result.split(L" \n\r\t", attributes);
				analyseAttributes(attributes);
			}

			fileStream.close();
		}
		else
			return false;

		return true;
	}

	iModelDataIOOBJ::OBJMaterial *iModelDataIOOBJ::getMaterial(const iaString &materialName)
	{
		if (materialName.isEmpty())
		{
			return nullptr;
		}

		auto iter = _materials.find(materialName);
		if (iter == _materials.end())
		{
			return nullptr;
		}

		return &(iter->second);
	}

	iaVector3f *iModelDataIOOBJ::getVertex(uint32 index)
	{
		if (index >= _vertexes.size())
			return 0;
		return &_vertexes[index];
	}

	iaVector3f *iModelDataIOOBJ::getNormal(uint32 index)
	{
		if (index >= _normals.size())
			return 0;
		return &_normals[index];
	}

	iaVector2f *iModelDataIOOBJ::getTexCoord(uint32 index)
	{
		if (index >= _texcoord.size())
			return 0;
		return &_texcoord[index];
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

} // namespace igor
