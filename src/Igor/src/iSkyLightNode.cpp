// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iSkyLightNode.h>

#include <iaConsole.h>
#include <iRenderer.h>
#include <iShader.h>
#include <iScene.h>
#include <iNodeCamera.h>
#include <iMaterial.h>

namespace Igor
{

	iSkyLightNode::iSkyLightNode()
		: iNodeRender()
	{
		setName(L"SkyLight");
		initMesh();
		_nodeType = iNodeType::iSkyLightNode;
	}

	iSkyLightNode::~iSkyLightNode()
	{
	    _dome = 0;
	}

	void iSkyLightNode::draw()
	{
	/*	iNodeCamera* cameraNode = _scene->getCamera();

		if(cameraNode)
		{
            iaMatrixf model;
            iaMatrixf camMatrix;

            cameraNode->getMatrix(camMatrix);
            model.pos = camMatrix.pos;

			iRenderer::getInstance().setModelMatrix(model);

			iMaterial* material = iRenderer::getInstance().getCurrentMaterial();
			if(material)
			{
				iShader* shader = material->getShader();
                if (shader)
				{
                    shader->setVector3f(shader->getUniformLocation("LightDirection"), lightdirection);
				}

				iRenderer::getInstance().drawMesh(*_dome);
			}
		}
		else
		{
			con_err("no camera node was set");
		}*/
	}

	void iSkyLightNode::initMesh()
	{
        iMeshBuilder* domeBuilder = new iMeshBuilder();
		float s = 10.0f;

		iaVector3f vec[8];
		vec[0] = iaVector3f(-s, -s, s);
		vec[1] = iaVector3f(s, -s, s);
		vec[2] = iaVector3f(s, s, s);
		vec[3] = iaVector3f(-s, s, s);

		vec[4] = iaVector3f(-s, -s, -s);
		vec[5] = iaVector3f(s, -s, -s);
		vec[6] = iaVector3f(s, s, -s);
		vec[7] = iaVector3f(-s, s, -s);

/*        domeBuilder->setTriangle(vec[2], vec[1], vec[0]);
        domeBuilder->setTriangle(vec[2], vec[0], vec[3]);

        domeBuilder->setTriangle(vec[6], vec[5], vec[1]);
        domeBuilder->setTriangle(vec[1], vec[2], vec[6]);

        domeBuilder->setTriangle(vec[2], vec[3], vec[7]);
        domeBuilder->setTriangle(vec[7], vec[6], vec[2]);

        domeBuilder->setTriangle(vec[0], vec[4], vec[7]);
        domeBuilder->setTriangle(vec[7], vec[3], vec[0]);

        domeBuilder->setTriangle(vec[4], vec[5], vec[6]);
        domeBuilder->setTriangle(vec[6], vec[7], vec[4]);

        domeBuilder->setTriangle(vec[1], vec[5], vec[4]);
        domeBuilder->setTriangle(vec[4], vec[0], vec[1]);*/

        _dome = domeBuilder->createMesh();

        delete domeBuilder;
	}

	void iSkyLightNode::setLightDirection(iaVector3f lightdir)
	{
		lightdirection = lightdir;
	}

}