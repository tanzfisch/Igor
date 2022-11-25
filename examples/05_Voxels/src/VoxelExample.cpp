// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "VoxelExample.h"

#include "VoxelTerrainMeshGenerator.h"

VoxelExample::VoxelExample(iWindowPtr window)
    : ExampleBase(window, "Voxel", true, true)
{
}

void VoxelExample::onInit()
{
    initScene();

    // register model data io
    iModelResourceFactory::getInstance().registerModelDataIO("example_vtg", &VoxelTerrainMeshGenerator::createInstance);

    // generating voxels
    generateVoxelData();
}

void VoxelExample::onDeinit()
{
    // unregister vertex mesh generator
    iModelResourceFactory::getInstance().unregisterModelDataIO("example_vtg");
}

void VoxelExample::initScene()
{
    // create camera
    // first heading transformation node
    iNodeTransform *cameraHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraHeading = cameraHeading->getID();
    // then pitch transformation node
    iNodeTransform *cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraPitch = cameraPitch->getID();
    // and distance to origin transformation node
    iNodeTransform *cameraTranslation = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraTranslation->translate(0, 0, 120);
    // anf of corse the camera
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    // add it to the scene
    getScene()->getRoot()->insertNode(cameraHeading);
    cameraHeading->insertNode(cameraPitch);
    cameraPitch->insertNode(cameraTranslation);
    cameraTranslation->insertNode(camera);
    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a
    // scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
    getView().setCurrentCamera(camera->getID());

    // create a directional light
    // transform node
    iNodeTransform *lightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    lightTranslate->translate(100, 100, 100);
    // and light node
    iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor3f(0.6f, 0.6f, 0.6f));
    lightNode->setDiffuse(iaColor3f(0.9f, 0.7f, 0.6f));
    lightNode->setSpecular(iaColor3f(1.0f, 0.9f, 0.87f));
    // and add it to the scene
    getScene()->getRoot()->insertNode(lightTranslate);
    lightTranslate->insertNode(lightNode);

    // set up voxel mesh material
    _voxelMeshMaterial = iMaterialResourceFactory::getInstance().createMaterial("voxel mesh material");
    iShaderProgramPtr program = iShaderProgram::create();
    program->addShader("igor/terrain.vert", iShaderObjectType::Vertex);
    program->addShader("igor/terrain_directional_light.frag", iShaderObjectType::Fragment);
    program->compile();
    _voxelMeshMaterial->setShaderProgram(program);
}

float32 metaballFunction(iaVector3f metaballPos, iaVector3f checkPos)
{
    return 1.0f / ((checkPos._x - metaballPos._x) * (checkPos._x - metaballPos._x) + (checkPos._y - metaballPos._y) * (checkPos._y - metaballPos._y) + (checkPos._z - metaballPos._z) * (checkPos._z - metaballPos._z));
}

void VoxelExample::generateVoxelData()
{
    _rand.setSeed(static_cast<uint32>(iaTime::getNow().getMicrosenconds()));

    // if there is none create it
    if (_voxelData == nullptr)
    {
        _voxelData = new iVoxelData();
        // all voxels have a full density as default. so afterwars we need to cut holes in it
        _voxelData->setClearValue(0);
        // 255 cubic is the maximum size of a single voxel block. if you need a bigger terrain use iVoxelTerrain see example Ascent
        _voxelData->initData(255, 255, 255);
    }

    // generate new random base with time based seed
    _perlinNoise.generateBase(_rand.getNext());

    // clear the voxel data
    _voxelData->clear();

    // define some threasholds to describe the surface of caves
    const float64 from = 0.444;
    const float64 to = 0.45;
    float64 factor = 1.0 / (to - from);

    // define some threasholds to describe the surface of metaballs
    const float64 fromMeta = 0.017;
    const float64 toMeta = 0.0175;
    float64 factorMeta = 1.0 / (toMeta - fromMeta);

    const float32 coreSize = 0.3f;
    const float32 coreOffset = (1.0f - coreSize) * 0.5f;

    // define some metaballs
    std::vector<std::pair<iaVector3f, float32>> metaballs;
    for (int i = 0; i < 20; ++i)
    {
        metaballs.push_back(std::pair<iaVector3f, float32>(iaVector3f(
                                                               _rand.getNext() % static_cast<int>(_voxelData->getWidth() * coreSize) + (_voxelData->getWidth() * coreOffset),
                                                               _rand.getNext() % static_cast<int>(_voxelData->getHeight() * coreSize) + (_voxelData->getHeight() * coreOffset),
                                                               _rand.getNext() % static_cast<int>(_voxelData->getDepth() * coreSize) + (_voxelData->getDepth() * coreOffset)),
                                                           (((_rand.getNext() % 30) + 70) / 100.0) + 0.4));
    }

    // now iterate through all the voxels and define their density
    for (int64 x = 0; x < _voxelData->getWidth(); ++x)
    {
        for (int64 y = 0; y < _voxelData->getHeight(); ++y)
        {
            for (int64 z = 0; z < _voxelData->getDepth(); ++z)
            {
                float32 density = 0;

                // first figure out if a voxel is outside the sphere
                iaVector3f pos(static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(z));

                float32 noise = (_perlinNoise.getValue(iaVector3d(pos._x * 0.05, pos._y * 0.05, pos._z * 0.05), 3) - 0.5) * 0.017;

                float32 distance = 0;
                for (auto metaball : metaballs)
                {
                    distance += metaballFunction(metaball.first, pos) * metaball.second;
                }

                distance += noise;

                // determine the density of given voxel related to the metaballs
                if (distance >= toMeta)
                {
                    density = 1.0;
                }
                else
                {
                    if (distance >= fromMeta)
                    {
                        density = ((distance - fromMeta) * factorMeta);
                    }
                }

                // using some perline noise to cut holes in the sphere. this time we skip the smoothing part due to much effort and cluttering the tutorial
                // with bad understandable code. Ask the author if you'd like to know about smoothing the values
                float64 onoff = _perlinNoise.getValue(iaVector3d(pos._x * 0.04, pos._y * 0.04, pos._z * 0.04), 3, 0.5);

                // we do the same here as with the metaball surface so it a pears a little smoother
                if (onoff <= from)
                {
                    if (onoff >= to)
                    {
                        density = 1.0 - ((onoff - from) * factor);
                    }
                    else
                    {
                        density = 0.0;
                    }
                }

                if (density > 1.0)
                {
                    density = 1.0;
                }

                if (density < 0.0)
                {
                    density = 0.0;
                }

                if (density > 0.0)
                {
                    _voxelData->setVoxelDensity(iaVector3I(x, y, z), (density * 254) + 1);
                }
            }
        }
    }

    // clean up current scene before we generate a new mesh
    if (_voxelMeshTransform != iNode::INVALID_NODE_ID)
    {
        // this will also kill all the children of that node
        iNodeManager::getInstance().destroyNodeAsync(_voxelMeshTransform);
        _voxelMeshTransform = iNode::INVALID_NODE_ID;
        _voxelMeshModel = iNode::INVALID_NODE_ID;
    }

    // !!!! now you should first have a look at the VoxelTerrainMeshGenerator class before you continue !!!!
    prepareMeshGeneration();

    // set loading flag to true so we can display the loading text on screen
    _loading = true;

    // get current time so we can measure how long it took to generate the mesh
    _time = iaTime::getNow();
}

void VoxelExample::prepareMeshGeneration()
{
    // prepar special tile information for the VoxelTerrainMeshGenerator
    TileInformation tileInformation;
    tileInformation._material = _voxelMeshMaterial;
    tileInformation._voxelData = _voxelData;
    // define the input parameter so Igor knows which iModelDataIO implementation to use and how
    iModelDataInputParameter *inputParam = new iModelDataInputParameter();
    inputParam->_identifier = "example_vtg";
    inputParam->_joinVertexes = true;
    inputParam->_needsRenderContext = false;
    inputParam->_modelSourceType = iModelSourceType::Generated;
    inputParam->_loadPriority = 0;
    inputParam->_keepMesh = true;
    inputParam->_parameters.setData(reinterpret_cast<const char *>(&tileInformation), sizeof(TileInformation));
    // create a model node
    iNodeModel *voxelMeshModel = iNodeManager::getInstance().createNode<iNodeModel>();
    voxelMeshModel->setName("VoxelMeshModel");
    _voxelMeshModel = voxelMeshModel->getID();
    // tell the model node to load data with specified identifier ans the above defined parameter
    // it is important to have a unique identifier each time we generate a mesh otherwhise the cache system would return us a prvious generated mesh
    voxelMeshModel->setModel(iaString("VoxelMesh") + iaString::toString(_incarnation++), iResourceCacheMode::Keep, inputParam);
    // create a transform node to center the mesh to the origin
    iNodeTransform *voxelMeshTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    voxelMeshTransform->setName("VoxelMeshTransform");
    _voxelMeshTransform = voxelMeshTransform->getID();
    voxelMeshTransform->translate(-_voxelData->getWidth() / 2, -_voxelData->getHeight() / 2, -_voxelData->getDepth() / 2);
    // and add to scene
    voxelMeshTransform->insertNode(voxelMeshModel);
    getScene()->getRoot()->insertNode(voxelMeshTransform);
}

void VoxelExample::onRenderOrtho()
{
    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -1);
    iRenderer2::getInstance().setModelMatrix(modelMatrix);

    iRenderer2::getInstance().setFont(getFont());
    iRenderer2::getInstance().setFontSize(25.0f);

    if (_loading)
    {
        iRenderer2::getInstance().drawString(getWindow()->getClientWidth() * 0.5, getWindow()->getClientHeight() * 0.5, "loading ...", iHorizontalAlignment::Center, iVerticalAlignment::Center, iaColor4f::green);
    }
    else
    {
        iRenderer2::getInstance().drawString(getWindow()->getClientWidth() * 0.5, getWindow()->getClientHeight() * 0.1, "press [Space] to recreate", iHorizontalAlignment::Center, iVerticalAlignment::Center, iaColor4f::green);
    }

    ExampleBase::onRenderOrtho();
}

void VoxelExample::onPreDraw()
{
    // detect if loading is done
    if (_voxelMeshModel != iNode::INVALID_NODE_ID)
    {
        iNodeModel *voxelMeshModel = static_cast<iNodeModel *>(iNodeManager::getInstance().getNode(_voxelMeshModel));

        if (voxelMeshModel != nullptr &&
            voxelMeshModel->isValid())
        {
            if (_loading)
            {
                _loading = false;
                con_endl("generation time: " << (iaTime::getNow() - _time));
            }
        }
    }
}

void VoxelExample::onEvent(iEvent &event)
{
    // first call example base
    ExampleBase::onEvent(event);

    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(VoxelExample::onKeyDown));
    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(VoxelExample::onMouseMoveEvent));
}

bool VoxelExample::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::Space:
        generateVoxelData();
        return true;

    case iKeyCode::F4:
        iModelResourceFactory::getInstance().exportModelData("voxelExample.ompf", getScene()->getRoot()->getChild("VoxelMeshTransform")->getChild("VoxelMeshModel"), "ompf", iSaveMode::EmbedExternals);
        return true;
    }

    return false;
}

bool VoxelExample::onMouseMoveEvent(iEventMouseMove &event)
{
    const auto from = event.getLastPosition();
    const auto to = event.getPosition();

    if (iMouse::getInstance().getLeftButton())
    {
        float32 dx = static_cast<float32>(from._x - to._x) * 0.005f;
        float32 dy = static_cast<float32>(from._y - to._y) * 0.005f;
        iNodeTransform *cameraHeading = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraHeading));
        iNodeTransform *cameraPitch = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraPitch));
        if (cameraHeading != nullptr &&
            cameraPitch != nullptr)
        {
            cameraHeading->rotate(dx, iaAxis::Y);
            cameraPitch->rotate(dy, iaAxis::X);
        }

        iMouse::getInstance().setCenter();
        return true;
    }

    return false;
}
