#ifndef __ASCENT__
#define __ASCENT__

#include <igor/igor.h>

class Enemy;

class Ascent : public iLayer
{

public:
    // TODO replace later with data from loaded model
    static uint64 _terrainMaterialID;
    static uint64 _entityMaterialID;
    static uint64 _bulletMaterialID;

    /*! init members

    \param window the given window
    */
    Ascent(iWindow *window);
    ~Ascent() = default;

private:
    bool _captureMouse = true;

    iaRandomNumberGeneratoru rand;

    iProfilerVisualizer _profilerVisualizer;

    bool _loading = true;
    bool _activeControls = false;

    iView _view;
    iView _viewOrtho;

    uint64 _playerID = 0;
    uint64 _bossID = 0;

    iPerlinNoise _perlinNoise;

    iTextureFont *_font = nullptr;

    iScene *_scene = nullptr;

    uint64 _toolSize = 3;
    uint8 _toolDensity = 0;

    iaVector2f _mouseDelta;
    iaVector3f _weaponPos;

    iNodeTransform *_lightTranslate = nullptr;
    iNodeTransform *_lightRotate = nullptr;
    iNodeLight *_lightNode = nullptr;

    float64 _startTime;

    uint32 _materialWithTextureAndBlending = 0;
    uint32 _octreeMaterial = 0;
    int32 _materialSkyBox = 0;

    uint64 _taskFlushModels = 0;
    uint64 _taskFlushTextures = 0;

    iaMutex _hitListMutex;
    std::vector<std::pair<uint64, uint64>> _hitList;

    std::vector<iSphered> _metaballs;
    std::vector<iSphered> _holes;
    iVoxelTerrain *_voxelTerrain = nullptr;

    int _enemyCount = 0;

    void oulineLevelStructure();
    void deinitVoxelData();
    void initVoxelData();
    void onGenerateVoxelData(iVoxelBlockInfo *voxelBlockInfo);
    void onVoxelDataGenerated(iVoxelBlockPropsInfo voxelBlockInfo);

    bool getTerrainIntersectionPoint(iaVector3I &intersection);
    void dig(iaVector3I position, uint64 toolSize, uint8 toolDensity);

    void handleMouse();
    void handleHitList();

    void onRender();
    void onRenderOrtho();

    void initViews();
    void initScene();
    void initPlayer();

    void initPhysics();
    void onContactTerrainBullet(iPhysicsBody *body0, iPhysicsBody *body1);
    void onContact(iPhysicsBody *body0, iPhysicsBody *body1);

    /*! called when added to layer stack
        */
    void onInit() override;

    /*! called when removed from layer stack
        */
    void onDeinit() override;

    /*! called on application pre draw event
        */
    void onPreDraw() override;

    /*! called on any other event
        */
    void onEvent(iEvent &event) override;

    /*! handles mouse key down event

    \param event the mouse key down event
    \returns true if consumed
    */
    bool onMouseKeyDownEvent(iEventMouseKeyDown &event);

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    bool onMouseMoveEvent(iMouseMoveEvent_TMP &event);

    /*! handles mouse wheel event

    \param event the mouse wheel event
    \returns true if consumed
    */
    bool onMouseWheelEvent(iMouseWheelEvent_TMP &event);

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iKeyDownEvent_TMP &event);

    /*! called when key was released

    \param event the event to handle
    */
    bool onKeyUp(iKeyUpEvent_TMP &event);

    /*! handle window resize event

    \param event the window resize event
    \returns true if consumed
    */
    bool onWindowResize(iWindowResizeEvent_TMP &event);
};

#endif
