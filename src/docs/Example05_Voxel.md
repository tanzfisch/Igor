Voxel Terrain Example                                            {#example05}
=====================

So in the voxel example we basically learn two things. One is how to use the class iVoxelData and the other is how to write and register a model data loader that derives from iModelDataIO ([youtube](https://www.youtube.com/watch?v=EHBsmCx6gBM)).

Build
=====

The code for this example can be found in the folder examples/05 Voxels. In a vs[ver] sub folder you will find a visual studio solution which contains example project but also all dependent projects. So you just open this one and than build and start. If everything went right you gonna see the following screen. If not please don't hesitate to contact [tanzfisch](https://github.com/tanzfisch).

![Voxel Example](/images/Example05_Pic1.png)

There is also a youtube clip [Voxel Example](https://youtu.be/EHBsmCx6gBM).

# Interaction
* click left and drag to rotate the camera around the point of interest
* press space to generate new voxel data

Code
====

The most interesting parts in this example happen in the method generateVoxelData() and the class VoxelTerrainMeshGenerator. We start with generateVoxelData() because it initiates everything.

So first thing is to alloc some memory for the voxel data by creating a iVoxelData obejct and initializing it with the size we need.

        // if there is none create it
        if (_voxelData == nullptr)
        {
            _voxelData = new iVoxelData();
            // all voxels have a full density as default. so afterwars we need to cut holes in it
            _voxelData->setClearValue(255);
            _voxelData->initData(120, 120, 120);
        }

We generate a new random base everytime so every time we hit the space button we get a diffrent result.

        // generate new random base with time based seed
        _perlinNoise.generateBase(static_cast<uint32>(iTimer::getInstance().getTime()));

Clear the voxel data and switch to uncompressed mode for faster access.

        // clear the voxel data
        _voxelData->clear();

Define some thresholds for metaballs and caves.

        // define some threasholds to describe the surface of caves
        const float64 from = 0.444;
        const float64 to = 0.45;
        float64 factor = 1.0 / (to - from);

        // define some threasholds to describe the surface of metaballs
        const float64 fromMeta = 0.017;
        const float64 toMeta = 0.0175;
        float64 factorMeta = 1.0 / (toMeta - fromMeta);

Define some metaballs for the base structure

        // define some metaballs
        std::vector<std::pair<iaVector3f, float32>> metaballs;
        for (int i = 0; i < 20; ++i)
        {
            metaballs.push_back(std::pair<iaVector3f, float32>(iaVector3f(rand() % static_cast<int>(_voxelData->getWidth() * 0.6) + (_voxelData->getWidth() * 0.2),
                rand() % static_cast<int>(_voxelData->getHeight()*0.6) + (_voxelData->getHeight()* 0.2),
                rand() % static_cast<int>(_voxelData->getDepth()*0.6) + (_voxelData->getDepth()*0.2)), (((rand() % 90) + 10) / 100.0) + 0.6));
        }

Now we itereate through and figure out if they are inside the metaballs and if that is true also if they are inside a cave. 

        // now iterate through all the voxels and define their density
        for (int64 x = 0; x < _voxelData->getWidth(); ++x)
        {
            for (int64 y = 0; y < _voxelData->getHeight(); ++y)
            {
                for (int64 z = 0; z < _voxelData->getDepth(); ++z)
                {
                    // first figure out if a voxel is outside the sphere
                    iaVector3f pos(x, y, z);

                    float32 distance = 0;
                    for (auto metaball : metaballs)
                    {
                        distance += metaballFunction(metaball.first, pos) * metaball.second;
                    }

                    if (distance <= toMeta)
                    {
                        if (distance >= fromMeta)
                        {
                            // at the edge of the sphere.
                            // now we can use the fractional part of the distance to determine how much more than a full voxel we are away from the center
                            // and use this to set the density. this way we get a smooth sphere.
                            float32 denstity = ((distance - fromMeta) * factorMeta);

                            // the density by the way goes from 0-255 but the zero is interpreted as outside ans the 1 is inside but with zero density
                            // so to calculate a propper density we need to multiply the density with 254 and to make it alwasy beein "inside" we add one
                            _voxelData->setVoxelDensity(iaVector3I(x, y, z), (denstity * 254) + 1);
                        }
                        else
                        {
                            // outside metaball
                            _voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                        }
                    }

                    // using some perline noise to cut holes in the sphere. this time we skip the smoothing part due to much effort and cluttering the tutorial 
                    // with bad understandable code. Ask the author if you'd like to know about smoothing the values
                    float64 onoff = _perlinNoise.getValue(iaVector3d(pos._x * 0.03, pos._y * 0.03, pos._z * 0.03), 4, 0.5);

                    // we do the same here as with the metaball surface so it a pears a little smoother
                    if (onoff <= from)
                    {
                        if (onoff >= to)
                        {
                            float64 gradient = 1.0 - ((onoff - from) * factor);
                            _voxelData->setVoxelDensity(iaVector3I(x, y, z), (gradient * 254) + 1);
                        }
                        else
                        {
                            _voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                        }
                    }
                }
            }
        }

The next major part is to turn the voxel data in to a mesh. We could just use the class iContouringCubes to do that but fore more comfort and to understand how to use it we go for implementing the iModelDataIO interface. The iModelDataIO interface is kind of a hock within the engine's loading process. We can define basically our own model data type, register it to the loading process and later just initialize an iNodeModel with our voxel data instead of a filename.
So let's have a look at the key part of the VoxelTerrainMeshGenerator which implements that interface.

    iNode* VoxelTerrainMeshGenerator::importData(const iaString& sectionName, iModelDataInputParameter* parameter)
    {

As you see we get besides the filename, which in our case is just any string, some extra input parameter that contain our voxel data information. See further down to initialize it with custom data.

        TileInformation* tileInformation = reinterpret_cast<TileInformation*>(parameter->_parameters.getDataPointer());
        iVoxelData* voxelData = tileInformation->_voxelData;
        int64 width = voxelData->getWidth() - 1;
        int64 depth = voxelData->getDepth() - 1;
        int64 height = voxelData->getHeight() - 1;

We create a resulting group node because the engine expects this from us.
    
        iNode* result = iNodeManager::getInstance().createNode(iNodeType::iNode);

Now we use the iContouringCubes class to turn our voxel data in to a mesh.

        iContouringCubes contouringCubes;
        contouringCubes.setVoxelData(voxelData);
        iMeshPtr mesh = contouringCubes.compile(iaVector3I(), iaVector3I(width, height, depth));

Theoretically the voxel data could result in a mesh that does not exists so we have to check that here.

        if (mesh.get() != nullptr)
        {

But if it does exist we create a iNodeMesh and add it to the resulting node.

            iNodeMesh* meshNode = static_cast<iNodeMesh*>(iNodeManager::getInstance().createNode(iNodeType::iNodeMesh));
            meshNode->setMesh(mesh);
            meshNode->setMaterial(tileInformation->_materialID);

We also set up the target material of our mesh for tri planar texture mapping as defined earlier in material _voxelMeshMaterialID.

            iTargetMaterial* targetMaterial = meshNode->getTargetMaterial();
            targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("dirt.png"), 0);
            targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("grass.png"), 1);
            targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("rock.png"), 2);
            targetMaterial->setAmbient(iaColor3f(0.7f, 0.7f, 0.7f));
            targetMaterial->setDiffuse(iaColor3f(0.9f, 0.9f, 0.9f));
            targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
            targetMaterial->setEmissive(iaColor3f(0.01f, 0.01f, 0.01f));
            targetMaterial->setShininess(100.0f);

            result->insertNode(meshNode);
        }

        return result;
    }

And last but not least here is how we initialize our custom input parameters for loading our voxel data as a mesh iModelDataInputParameter.

    // prepar special tile information for the VoxelTerrainMeshGenerator
    TileInformation tileInformation;
    tileInformation._materialID = _voxelMeshMaterialID;
    tileInformation._voxelData = _voxelData;
    // define the input parameter so Igor knows which iModelDataIO implementation to use and how
    iModelDataInputParameter* inputParam = new iModelDataInputParameter();
    inputParam->_identifier = "vtg";
    inputParam->_joinVertexes = true;
    inputParam->_needsRenderContext = false;
    inputParam->_modelSourceType = iModelSourceType::Generated;
    inputParam->_loadPriority = 0;
    inputParam->_parameters.setData(reinterpret_cast<const char*>(&tileInformation), sizeof(TileInformation));
    // create a model node
    iNodeModel* voxelMeshModel = static_cast<iNodeModel*>(iNodeManager::getInstance().createNode(iNodeType::iNodeModel));
    _voxelMeshModel = voxelMeshModel->getID();
    // tell the model node to load data with specified identifier ans the above defined parameter
    // it is important to have a unique identifier each time we generate a mesh otherwhise the cache system would return us a prvious generated mesh
    voxelMeshModel->setModel(iaString("VoxelMesh") + iaString::toString(_incarnation++), inputParam);
    // create a transform node to center the mesh to the origin
    iNodeTransform* voxelMeshTransform = static_cast<iNodeTransform*>(iNodeManager::getInstance().createNode(iNodeType::iNodeTransform));
    _voxelMeshTransform = voxelMeshTransform->getID();
    voxelMeshTransform->translate(-_voxelData->getWidth() / 2, -_voxelData->getHeight() / 2, -_voxelData->getDepth() / 2);
    // and add to scene
    voxelMeshTransform->insertNode(voxelMeshModel);
    _scene->getRoot()->insertNode(voxelMeshTransform);

Additional Information
======================

Since the engine is capable of double precision size worlds we need to do an additional step that is kind of a workaround. At the bottom of the init method you can find the following lines.

    // ok this one is kind of a workaround. In order to handle huge worlds beyond float precision we internally mess around with world positions.
    // in consequence the world positions that end up in the shader are not valid. They are relative to the camera position. To compensate we can 
    // set the world grid resolution here. It basically makes a modulo on the world coordinates so they never exceed float precision and are sort of usable again within a shader.
    iRenderer::getInstance().setWorldGridResolution(1000.0);

And that's it. For further questions please contact [tanzfisch](https://github.com/tanzfisch).