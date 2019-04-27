3D Rendering Example                                     {#example04}
====================

The 3D example shows how to load models and place them in to a 3D scene. It also introduces light, lod and switch nodes.

Build
=====

The code for this example can be found in the folder examples/04 3D. In a vs[ver] sub folder you will find a visual studio solution which contains example project but also all dependent projects. So you just open this one and than build and start. If everything went right you gonna see the following screen. If not please don't hesitate to contact [tanzfisch](https://github.com/tanzfisch).

![3D Rendering Example](/images/Example04_Pic1.png)

There is also a short youtube clip [3D Example](https://www.youtube.com/watch?v=UQ7iFGFOv10).

Interaction
===========

In this example we have some user inputs. Following a list.
* hold left mouse button and move to rotate the camera around the origin of the scene
* hold right mouse button and move to rotate a group of models around the origin
* use mouse wheel to change distance of camera to origin (so you can see the change in LOD)
* press **Space** to toggle the switch node
* press **F1** for print the current scene to the console
* press **ESC** to quit

Code
====

Let's start with initializing the scene inside the init method. Igor can handle multiple scenes but most of the time one is enough. In order to make Igot display a scene on screen we need to bind it to a view which was added to a window.

    // init scene
    _scene = iSceneFactory::getInstance().createScene();
    // bind scene to perspective view
    _view.setScene(_scene);

First thing we add to the scene is a camera. To position the camera somewhere else then in the origin it must be child of at least one transformation node. In Igor any combination of node types as children of other nodes is valid.

    // setup camera
    // we want a camera which can be rotated arround the origin
    // we will acchive that with 3 transform nodes
    // one is for the heading
    iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    // give the transform node a name. naming is optional and ist jus for helping to debug. 
    // Names do not have to be unique but since it is possible to find nodes by name they better are
    cameraHeading->setName("camera heading");
    _cameraHeading = cameraHeading->getID();
    // one is for the pitch
    iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    cameraPitch->setName("camera pitch");
    _cameraPitch = cameraPitch->getID();
    // and one is for translation or distance from the origin
    iNodeTransform* cameraTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    cameraTranslation->setName("camera translation");
    // translate away from origin
    cameraTranslation->translate(0, 0, 10);
    _cameraTranslation = cameraTranslation->getID();
    // from all nodes that we want to control later we save the node ID
    // and last but not least we create a camera node
    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    camera->setName("camera");
    // and build everything together
    // first we add the heading to the root node
    _scene->getRoot()->insertNode(cameraHeading);
    // than the pitch to the heading node
    cameraHeading->insertNode(cameraPitch);
    // then the translation to the pitch node
    cameraPitch->insertNode(cameraTranslation);
    // and than we add the camera to the translation
    cameraTranslation->insertNode(camera);
    // and finally we set the camera active. for this to work a camera must be part of a scene 
    // wich we achived by adding all those nodes on to an other starting with the root node
    camera->makeCurrent();

Only one camera per scene can be active at a time. If an other camera was active already it will be deactivated.
Next we have a look at how to add a model node.

    // create a single cat model
    iNodeTransform* justCatTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    justCatTransform->setName("just a cat transform");
    justCatTransform->translate(0, 1, 0);
    // create a cat model
    iNodeModel* justCatModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    // Node model names can be altered but they also are generated based on the file name
    justCatModel->setModel("cat.ompf");
    // building the created nodes together and insert them in the scene
    _scene->getRoot()->insertNode(justCatTransform);
    justCatTransform->insertNode(justCatModel);

The *.ompf model will be located by Igor within the predefined search paths.
Now we are going to take a look at the switch node. It is a node that can set on of it's children active while all the others are inactive. One scenario for this could be a model of a vehicle and a model of a vehicle that is broken or burned out. And when ever the vehicle is broken you can just flip the switch. In the example we add 3 nodes to the switch node.

    // creating the switch node
    iNodeSwitch* switchNode = static_cast<iNodeSwitch*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSwitch));
    _switchNode = switchNode->getID();
    // add the switch node to the all objects pitch / group
    allObjectsPitch->insertNode(switchNode);
    // than add the transform nodes to the switch node
    switchNode->insertNode(crateTransform);
    switchNode->insertNode(teapotTransform);
    switchNode->insertNode(catTransform);
    // set the current activated child
    switchNode->setActiveChild("crate transform");

Similar to the switch node. Is the LOD switch node. Again it has multiple children and again it controls where a child is active or not. But this time it depends on the distance to an LOD trigger node which we attach in this case to the camera.

    // creating the LOD switch node
    iNodeLODSwitch* lodswitch = static_cast<iNodeLODSwitch*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLODSwitch));
    // add it to the scene
    allObjectsPitch->insertNode(lodswitch);
    // and add the nodes to it
    lodswitch->insertNode(lod0Transform);
    lodswitch->insertNode(lod1Transform);
    lodswitch->insertNode(lod2Transform);
    // set up the nodes switch distances. the switch distances are allowed to have gaps or overlap
    lodswitch->setThresholds(lod0Transform, 0.0f, 5.0f);
    lodswitch->setThresholds(lod1Transform, 5.0f, 11.0f);
    lodswitch->setThresholds(lod2Transform, 11.0f, 22.0f);
    // in order to causing the LOD switch to change the active child node it needs to be triggered by a LOD trigger node
    // creating the lod trigger node
    iNodeLODTrigger* lodtrigger = static_cast<iNodeLODTrigger*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLODTrigger));
    // bind the lod switch node with the lod trigger node
    lodswitch->addTrigger(lodtrigger);
    // and add the lod trigger to the scene by attaching it to the camera. there can be multiple LOD triggers and any lod switch can react on any lod trigger
    camera->insertNode(lodtrigger);

Let's add also a sky box as background for better orientation.

    // create a skybox
    iNodeSkyBox* skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
    // set it up with the default skybox texture
    skyBoxNode->setTextures(
        "skybox_default/front.png",
        "skybox_default/back.png",
        "skybox_default/left.png",
        "skybox_default/right.png",
        "skybox_default/top.png",
        "skybox_default/bottom.png");
    // manipulate the texture scale so we can see the repeating pattern of the textures
    skyBoxNode->setTextureScale(10);
    // create a material for the sky box because the default material for all iNodeRender and deriving classes has no textures and uses depth test
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->setOrder(iMaterial::RENDER_ORDER_EARLY);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->getMaterial()->setName("SkyBox");
    // set that material
    skyBoxNode->setMaterial(_materialSkyBox);
    // and add it to the scene
    _scene->getRoot()->insertNode(skyBoxNode);

And also switch the light on.

    // setup light
    // transform node for the lights orientation
    iNodeTransform* directionalLightRotate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    // keep transform node id so we can manipulate the light's position later
    _directionalLightRotate = directionalLightRotate->getID();
    // transform node for the lights distance to the origin
    iNodeTransform* directionalLightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    directionalLightTranslate->translate(100, 100, 100);
    // the light node
    iNodeLight* lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    lightNode->setAmbient(iaColor4f(0.3f, 0.3f, 0.3f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    // insert light to scene
    _scene->getRoot()->insertNode(directionalLightRotate);
    directionalLightRotate->insertNode(directionalLightTranslate);
    directionalLightTranslate->insertNode(lightNode);

For Manipulating the transform nodes have a look at onMouseWheel and onMouseMoved.
And that's it. For further questions please contact [tanzfisch](https://github.com/tanzfisch).
