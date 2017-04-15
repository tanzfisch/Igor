// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include "iCelestialNode.h"

#include <iRenderer.h>

namespace Igor
{

	iCelestialNode::iCelestialNode()
		: iNodeRender()
	{
		setName(L"iCelestialNode: no texture");
		_nodeType = iNodeType::iCelestialNode;
	}

	iCelestialNode::~iCelestialNode()
	{
	}

	void iCelestialNode::setTexture(iaString textureFileName)
	{
		texture = iTextureResourceFactory::getInstance().loadFile(textureFileName, iResourceCacheMode::Free, iTextureBuildMode::Normal);
		iaString name = L"iCelestialNode: ";
		name += textureFileName;
		setName(name);
	}

    void iCelestialNode::onPostCopyLink(map<uint32, uint32>& nodeIDMap)
    {

    }

/*	void iCelestialNode::transformDown(iaMatrixf& matrix)
	{
		iNodeRender::transformDown(matrix);
	}*/

	void iCelestialNode::draw()
	{
/*		iRenderer::getInstance().setModelMatrix(_matrix);

		iMaterial* material = iRenderer::getInstance().getCurrentMaterial();
		if(material)
		{
			material->processMatrices();
		}

		for(int i=0;i<model->getMeshCount();++i)
		{
			iMeshBuilder* mesh = model->getMesh(i);

			if(material)
			{
				material->processAppearance(mesh->getEmissive(), mesh->getAmbient(), mesh->getDiffuse(), mesh->getSpecular(), mesh->getShininess());
			}
			
			iRenderer::getInstance().drawMesh(*mesh);
		}*/
	}

}