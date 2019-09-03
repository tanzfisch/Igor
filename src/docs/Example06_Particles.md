Particle Systems Example                                       {#example06}
========================

The particles example shows how to set up particle systems and their corresponding emitters ([youtube](https://www.youtube.com/watch?v=pXI5tWLcBKQ)).

Build
=====

The code for this example can be found in the folder examples/06 Particles. In a vs[ver] sub folder you will find a visual studio solution which contains example project but also all dependent projects. So you just open this one and than build and start. If everything went right you gonna see the following screen. If not please don't hesitate to contact [tanzfisch](https://github.com/tanzfisch).

![Particles Example](/images/Example06_Pic1.png)

Interaction
===========

* Press left mouse button and drag to rotate the camera position around center of interest.
* Press **Space** to pause and continue the particle systems

Code
====

Particles is something you really don't want to configure in code. Therefore we can configure it using Mica and load it like model data in to the scene. 
But we have the flexibility to do so in code.

First we define the color gradient of the particles during their life time. Independent from their real lifetime this gradient goes from 0.0 to 1.0. 0.0 defines birth of a particle and 1.0 the dead. So in this case we fade in from white transparent to white opaque and than fade out to black transparent. We don't need any other colors here because the color is already inside the texture.

    iGradientColor4f colors;
    colors.insertValue(0.0, iaColor4f(1.0, 1.0, 1.0, 0.0));
    colors.insertValue(0.2, iaColor4f(1.0, 1.0, 1.0, 1.0));
    colors.insertValue(0.4, iaColor4f(1.0, 1.0, 1.0, 1.0));
    colors.insertValue(1.0, iaColor4f(0.0, 0.0, 0.0, 0.0));

Now we define the life time of particles. Since this gradient has only one entry it means all particles emitted at any time will have a life time between 0.2 and 0.5 seconds.

    iGradientVector2f visibility;
    visibility.insertValue(0.0, iaVector2f(0.2, 0.5));

Next gradient defines that at any time the emission rate will be 4 particles per frame (this might change to something like particles per second in later versions).

    iGradientui emission;
    emission.insertValue(0.0, 4);

Now we define the starting velocity of the particles that varies between 0.4 and 0.7.

    iGradientVector2f velocity;
    velocity.insertValue(0.0, iaVector2f(0.4, 0.7));

And finally the size of the particles between 3.0 and 4.0.

    iGradientVector2f startSize;
    startSize.insertValue(0.0, iaVector2f(3.0, 4.0));

So the data is set now we create the particle system it self and set a couple of parameters and put in the gradients we just defined.

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeManager::getInstance().createNode(iNodeType::iNodeParticleSystem));
    _particleSystemIDs.push_back(particleSystem->getID());
    particleSystem->setMaterial(_particlesMaterial);
    particleSystem->setTextureA("particleFire.png");
    particleSystem->setFirstTextureTiling(4, 4);
    particleSystem->setColorGradient(colors);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setStartVisibleTimeGradient(visibility);
    particleSystem->setStartSizeGradient(startSize);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setPeriodTime(2.0);
    particleSystem->setVortexTorque(0.2, 0.5);
    particleSystem->setVorticityConfinement(0.05);
    particleSystem->setVortexRange(10.0, 15.0);
    particleSystem->setVortexApperanceIntervall(5);

Put it in the scene root and start it (it does not have to be under the root node).

    _scene->getRoot()->insertNode(particleSystem);
    particleSystem->start();

We also need an emitter otherwise nothing will happen and tell the particle system to use it. It is allowed for multiple particle systems to use the same emitter. In this case we define the emitter as a disc of size 3 which by default points up.

    iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeManager::getInstance().createNode(iNodeType::iNodeEmitter));
    particleSystem->setEmitter(emitter->getID());
    emitter->setType(iEmitterType::Disc);
    emitter->setSize(3);

Using a transform to position the emitter where it suits us. and put all together in to the scene.

    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeManager::getInstance().createNode(iNodeType::iNodeTransform));
    transform->translate(-2, -4, 40);
    transform->insertNode(emitter);
    _scene->getRoot()->insertNode(transform);

And that's it. For further questions please contact [tanzfisch](https://github.com/tanzfisch).