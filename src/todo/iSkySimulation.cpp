// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <iSkySimulation.h>

#include <igor/graphics/scene/iScene.h>
#include <igor/graphics/scene/nodes/iNode.h>
#include <iaConsole.h>
#include <iSkyLightNode.h>
#include <iNodeSkyBox.h>
#include <iaMatrix.h>
#include <iDirectionalLightNode.h>

namespace Igor
{

	iSkySimulation::iSkySimulation(iScenePtr scene)
	{
		this->scene = scene;

		// a nice place
		latitude = 20.817272f;
		longitude = 107.226653f;

		time = 9.0f;
		day = 1;
		month = 1;
		year = 2000;

		init();
	}

	iSkySimulation::~iSkySimulation()
	{
	}

	void iSkySimulation::init()
	{
		skySimRoot = new iNode();
		skySimRoot->setName("SkySimulationRoot");
		scene->getRoot()->insertNode(skySimRoot);

		skyLightNode = new iSkyLightNode();
		skySimRoot->insertNode(skyLightNode);
		skyLightNode->setName("SkyLight");

		directionalLightNode = new iDirectionalLightNode();
		directionalLightNode->setColor(1, 1, 1);
		skySimRoot->insertNode(directionalLightNode);

		skyBoxNode = new iNodeSkyBox("skybox_stars/front.jpg", "skybox_stars/back.jpg", "skybox_stars/left.jpg", "skybox_stars/right.jpg", "skybox_stars/top.jpg", "skybox_stars/bottom.jpg");
		skyBoxNode->setAlpha(0.0f);
		skyBoxNode->setUseMatrix(true);
		skySimRoot->insertNode(skyBoxNode);

		update();
	}

	void iSkySimulation::update()
	{
		iaMatrixf rotate;
		iaVector3f lightDirection(0, 1, 0);

		rotate.rotate(latitude / 90.0f * 0.5f * M_PI, iaAxis::X);
		rotate.rotate((time / 24.0f - 0.5f) * 2.0f * M_PI, iaAxis::Z);
		lightDirection = rotate * lightDirection;

		skyLightNode->setLightDirection(lightDirection);

		iaVector3f up(0, 1, 0);
		float32 elevation = up * lightDirection;

		iaColor3f color;
		color.lerp(iaColor3f(1, 1, 1), iaColor3f(0.95, 0.5, 0), elevation);
		directionalLightNode->setColor(color);
		directionalLightNode->setDirection(lightDirection);

		float32 alpha = -elevation + 0.1f * 0.2f; //! \todo
		if (alpha < 0.0f)
			alpha = 0.0f;
		if (alpha > 1.0f)
			alpha = 1.0f;
		skyBoxNode->setAlpha(alpha);

		skyBoxNode->setMatrix(rotate);
	}

	void iSkySimulation::setGeoPos(float32 latitude, float32 longitude)
	{
		this->latitude = latitude;
		this->longitude = longitude;
		update();
	}

	void iSkySimulation::setTime(float32 time)
	{
		this->time = time;
		update();
	}

	void iSkySimulation::setDate(int32 day, int32 month, int32 year)
	{
		this->day = day;
		this->month = month;
		this->year = year;
		con_err("not implemented");
	}

} // namespace Igor