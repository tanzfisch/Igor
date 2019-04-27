2D Rendering Example                                    {#example03}
====================

The 2D example shows how to load textures, use the draw callback and draw some simple 2D primitives on the screen. Keep in mind that Igor is primarily a 3D engine so there is really not much more to 2D than in this example.

Build
=====

The code for this example can be found in the folder examples/03 2D. In a vs[ver] sub folder you will find a visual studio solution which contains example project but also all dependent projects. So you just open this one and than build and start. If everything went right you gonna see the following screen. If not please don't hesitate to contact [tanzfisch](https://github.com/tanzfisch).

![2D Rendering Example](/images/Example03_Pic1.png)

If you like to see it moving. Here a link to a short you tube clip [2D Example](https://www.youtube.com/watch?v=N5gWTnX0B6c).

Code
====

Let's directly jump to the init() method. When it comes to rendering we need to set up our window with a view to render to.

    // define a view
    // switch of the clear color step because we will use a tiled background so there is no need to clear the framebuffer every frame
    _view.setClearColor(false);
    // set up an orthogonal projection with the dimensions of the windows client rectangle
    // the client rectangle is the size of the actual rendering area
    _view.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
    // register callback to the rendering event of this view
    _view.registerRenderDelegate(RenderDelegate(this, &Example2D::onRender));
    // add the view to the window
    _window.addView(&_view);

And because we want to render 2D we setup the view using setOrthogonal using the same resolution as the window client rectangle has. What we want to render happens all in the onRender callback we register here to the view and implement later in this exmaple.

Let's skip the window opening - you should know that by now from the examples before - and go to the first call of the iTextureResourceFactory. This factory is where we get our texture from. To just load a texture we call loadFile like this.

    // load the background tile texture
    _backgroundTexture = iTextureResourceFactory::getInstance().loadFile("WidgetThemePattern.png");

It will load the texture synchronously and return after doing so with a shared pointer. The path to the texture by the way is relative to a set of search paths we can define via the iResourceManager.
In the next lines we try to load a texture that does not exist to see what happens.

    // let's see what happens if we want Igor to load a texture that does not exist
    con_endl("!!! next statement will fail with an error message about a missing texture !!!");
    con_endl("");
    _dummyTexture = iTextureResourceFactory::getInstance().loadFile("doesnotexist.foobar");
    // by default Igor generates a dummy texture that will be returned instead
    // it is a checker texture with some gradients in color and alpha channels

Igor will print an error message to the console and use a generated texture from memory instead. As you have noticed it's the checker texture on the left of the screen shot.

Next we gonna configure a particle system that also uses a texture. But this time we gonna load it asynchronously using requestFile. It will return immediately and all it does is basically putting that load request in to a queue that will be processed later in our example. 

    // set up particle system
    // load a texture for our particle system
    _particleTexture = iTextureResourceFactory::getInstance().requestFile("simpleParticle.png");

Let's continue with the particle system. 

    // set the range of particle sizes at spawn
    _particleSystem.setParticleSize(10.0f, 70.0f);
    // set how much the particles should grow each frame
    _particleSystem.setParticleSizeDelta(0.5f, 3.0f);
    // set initial velocity vector of particles at spawn
    _particleSystem.setInitialVelocity(iaVector2f(40, 0));
    // make the animation endless. this causes the particles to be reused after their lifetime is over
    _particleSystem.setLoopable(true);
    // sets particle life time in frames
    _particleSystem.setParticleLifetime(80);
    // set the emission rate per frame
    _particleSystem.setEmitRate(5);
    // set maximal particle count. since the particle system is in loop mode we need at least as 
    // much particles as the life time times the emission rate to have a constant stream of particles
    _particleSystem.setMaxParticleCount(_particleSystem.getParticleLifetime() * _particleSystem.getEmitRate());
    // set the spread factor to 5% of the distribution circle
    // you can also interpret it as opening angle in percent
    _particleSystem.setSpreadFactor(0.05f);
    // don't want air drag
    _particleSystem.setAirDrag(0.0f);
    // apply external force. in this case something like gravity but positive because the y coordinate axis goes down the screen
    _particleSystem.setExternalForce(iaVector2f(0, 0.2f));

For a nice look of the particles we set up a color gradient using the iRainbow class to define the actual rainbow colors.

    // define a rainbow multi color gradient for our particles
    _rainbow.insertColor(iaColor4f(1, 0, 1, 0.0), 0.0f);
    _rainbow.insertColor(iaColor4f(0, 0, 1, 0.2), 0.2f);
    _rainbow.insertColor(iaColor4f(0, 1, 1, 0.4), 0.4f);
    _rainbow.insertColor(iaColor4f(0, 1, 0, 0.6), 0.6f);
    _rainbow.insertColor(iaColor4f(1, 1, 0, 0.8), 0.8f);
    _rainbow.insertColor(iaColor4f(1, 0, 0, 1.0), 1.0f);

So now the particle system is set up (we gonna have a look at how it's rendered later) there is two more ways of using a texture with Igor. One is in form of a texture based font and one is a sprite.

    // load a texture font
    _font = new iTextureFont("StandardFont.png");

    // load a texture as a sprite
    // sprites are basically textures that have some additional meta data that help you to place and orientate them
    _logo = new iSprite(iTextureResourceFactory::getInstance().loadFile("OpenGL-Logo.jpg"));
    // set the center as the origin of the sprite
    _logo->setOrigin(iaVector2f(_logo->getTexture()->getWidth() * 0.5, _logo->getTexture()->getHeight() * 0.5));

The font implicitly loads a texture synchronously so it can analyse the texture for where the single letters are in order to later nicely print them on screen.
The Sprite is basically a texture plus some meta information including the origin (e.g. to rotate around) and with and height in screen space independent of the textures resolution.

An other feature I'd like to introduce is the iaBSpline class. Well it defines a B-Spline that we can use to look up 3D values along the spline.

    // initalize a spline loop
    _spline.addSupportPoint(iaVector3f(100, 100, 0));
    _spline.addSupportPoint(iaVector3f(150, 80, 0));
    _spline.addSupportPoint(iaVector3f(900, 600, 0));
    _spline.addSupportPoint(iaVector3f(700, 500, 0));
    _spline.addSupportPoint(iaVector3f(100, 700, 0));
    _spline.addSupportPoint(iaVector3f(900, 300, 0));
    _spline.addSupportPoint(iaVector3f(50, 150, 0));
    // close the loop by having the end point at the same position as the start point
    _spline.addSupportPoint(iaVector3f(100, 100, 0));
    // defines the resolution of the output line strip if we later call getSpline
    _spline.setResolution(20);

For rendering anything in Igor we need to set up a couple of materials. Basically a material is a set of OpenGL based render states and or some shaders. Right now most of Igor still uses old school fix function pipeline due to laziness of the programmer ;-) but given enough time this will change.

    // create some materials
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _materialWithTexture = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTexture)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTexture)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _materialWithoutDepthTest = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithoutDepthTest)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

No it's time to load the requested textures by doing a flush in the iTextureResourceFactory. So technically this is not asynchronous loading of textures. And that is correct but this way you can choose whether to just make a flush between level loading or run it in a separate thread to continuously loading/unloading textures in the background. The last scenario is easiest by just creating iTaskFlushTextures and add it in to the iTaskManager.

    // load requested textures
    iTextureResourceFactory::getInstance().flush();

On the deinit end we destroy the created materials ...

    // release materials (optional)
    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTextureAndBlending);
    _materialWithTextureAndBlending = 0;
    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTexture);
    _materialWithTexture = 0;
    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithoutDepthTest);
    _materialWithoutDepthTest = 0;

... and destroy the font and the sprite and release the textures.

    // release some textures. otherwhise you will get a reminder of possible mem leak
    if (_logo != nullptr)
    {
        delete _logo;
        _logo = nullptr;
    }

    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }

    _particleTexture = nullptr;
    _backgroundTexture = nullptr;
    _dummyTexture = nullptr;

Next interessting part in the example is the onHandle implementation which is called before every render frame. And inside of that the updateParticles method. Basically we manipulate the start velocity of the particles over time based on a 2D perlin noise function and than we call the particle systems handle which updates the particles position.

    // manipulates particles initial velocity over time
    iaVector2f velocity(12, 0);
    float32 emitangle = _perlinNoise.getValue(_particleAnimatioValue * 0.05, 3) + 0.1;
    
    velocity.rotateXY(emitangle);
    _particleSystem.setInitialVelocity(velocity);
    _particleAnimatioValue += 1.0f;

    // calls particles iteration
    _particleSystem.handle();

And now we have a look at the rendering which happens in the onRender method. Since Igor is based on OpenGL we have to setup the OpenGL matrices first in order to see anything we are going to render.

    // since the model matrix is by default an identity matrix which would cause all our 2d rendering end up at depth zero
    // and the near clipping plane of our frustum can't be zero we have to push the scene a bit away from zero (e.g. -30 just a random number with no meaning)
    iaMatrixf matrix;
    iRenderer::getInstance().setViewMatrix(matrix);
    matrix.translate(iaVector3f(0, 0, -30));
    iRenderer::getInstance().setModelMatrix(matrix);

Now that the matrices are set up we can besically just switch between materials and colors and directly use all the 2d rendering methods the iRenderer provides. So for rendering our tiled background texture we call ...

    // set a textured material and draw the tiles texture as background
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTexture);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
    iRenderer::getInstance().drawTextureTiled(0, 0, _window.getClientWidth(), _window.getClientHeight(), _backgroundTexture);

... and to draw some non textured primitives we call ...

    // set non textured material and draw some primitves
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithoutDepthTest);
    iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 1));

    iRenderer::getInstance().drawRectangle(10, 10, 200, 150);
    iRenderer::getInstance().drawRectangle(220, 10, 200, 150);

    iRenderer::getInstance().setColor(iaColor4f(1, 1, 0, 1));
    iRenderer::getInstance().setLineWidth(3);

    iRenderer::getInstance().drawLine(20, 20, 200, 150);
    iRenderer::getInstance().drawLine(20, 150, 200, 20);
    iRenderer::getInstance().drawLine(110, 20, 110, 150);
    iRenderer::getInstance().drawLine(20, 85, 200, 85);

    for (int x = 0; x < 19; ++x)
    {
        for (int y = 0; y < 14; ++y)
        {
            iRenderer::getInstance().setPointSize(rand() % 5 + 1);
            iRenderer::getInstance().drawPoint(230 + x * 10, 20 + y * 10);
        }
    }

... and this is how we draw a sprite ...

    // change material again to textured an draw the logo
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
    iRenderer::getInstance().drawSprite(_logo, _logoPosition._x, _logoPosition._y, _logoRotationAngle, 1.5f, 1.5f);

... and a regular texture ...

    // draw the texture that we could not load at startup
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
    iRenderer::getInstance().drawTexture(10, 170, 410, 150, _dummyTexture);

... and the particle system (please notice the rainbow at the end)...

    // draw the particles
    iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 0.5));
    iRenderer::getInstance().bindTexture(_particleTexture, 0);
    iRenderer::getInstance().drawParticles(-10, _window.getClientHeight() - 150, 0, _particleSystem.getParticles(), _particleSystem.getParticleCount(), &_rainbow);

.. draw some text from wikipedia about OpenGL ...

    // draw some text from wikipedia
    iaString wikipediaOpenGL = "OpenGL (Open Graphics Library) ist eine Spezifikation fuer eine plattform- und programmiersprachenunabhaengige Programmierschnittstelle zur Entwicklung von 2D- und 3D-Computergrafik. Der OpenGL-Standard beschreibt etwa 250 Befehle, die die Darstellung komplexer 3D-Szenen in Echtzeit erlauben. Zudem koennen andere Organisationen (zumeist Hersteller von Grafikkarten) proprietaere Erweiterungen definieren. Wikipedia";
    iRenderer::getInstance().setFont(_font);
    iRenderer::getInstance().setFontSize(15.0f);
    iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 1));
    iRenderer::getInstance().drawString(600, 100, wikipediaOpenGL, -30, 400);

... and render the spline ...

    // draw spline
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithoutDepthTest);
    iRenderer::getInstance().setColor(iaColor4f(1, 0, 0.5, 1));
    iRenderer::getInstance().drawLineStrip(_spline.getSpline());

.. and some perlin nioise based 2D graph.

    // draw random graph in the upper right corner
    iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 1));
    iRenderer::getInstance().drawRectangle(_window.getClientWidth() - 260, 10, 250, 150);

    static float32 offset = 0.0f;
    iRenderer::getInstance().setLineWidth(1);
    iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 1));

    float64 lastValue = _perlinNoise.getValue(offset * 0.01, 6) * 150;
    for (int x = 1; x < 250; ++x)
    {
        float64 value = _perlinNoise.getValue((offset + x) * 0.01, 6) * 150;
        iRenderer::getInstance().drawLine(_window.getClientWidth() - 260 + x - 1, 10 + lastValue, _window.getClientWidth() - 260 + x, 10 + value);
        lastValue = value;
    }

    offset += 1.0f;

Last but not least there is a class alled iStatistics in Igor that can measure for you all about timings. We just use it here to draw the frame rate on the screen.

    // draw frame rate in lower right corner
    iStatistics::getInstance().drawStatistics(&_window, _font, iaColor4f(0, 1, 0, 1));

And that's all to it. For further questions please contact [tanzfisch](https://github.com/tanzfisch).