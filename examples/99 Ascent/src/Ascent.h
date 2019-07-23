#ifndef __ASCENT__
#define __ASCENT__

#include <Igor.h>
#include <iWindow.h>
#include <iView.h>
#include <iTimerHandle.h>
#include <iModelResourceFactory.h>
#include <iKeyboard.h>
#include <iStatisticsVisualizer.h>
#include <iSphere.h>
#include <iPerlinNoise.h>
using namespace Igor;

#include <iaMatrix.h>
#include <iaMutex.h>
#include <iaRandomNumberGenerator.h>
using namespace IgorAux;

namespace Igor
{
    class iScene;
    class iNodeTransform;
    class iNodeLight;
    class iTextureFont;
    class iVoxelData;
    class iContouringCubes;
    class iMeshBuilder;
    class iMesh;
    class iTargetMaterial;
    class iNodeLODTrigger;
    class iPhysicsBody;
    class iVoxelTerrain;
    class iVoxelBlockInfo;
    class iVoxelBlockPropsInfo;
}

class Enemy;

class Ascent
{

public:

    // TODO replace later with data from loaded model
    static uint64 _terrainMaterialID;
    static uint64 _entityMaterialID;
    static uint64 _bulletMaterialID;

    Ascent();
    virtual ~Ascent();

    void run();

private:

	bool _captureMouse = true;

    iaRandomNumberGeneratoru rand;

    iStatisticsVisualizer _statisticsVisualizer;

    bool _loading = true;
    bool _activeControls = false;

    iWindow _window;
    iView _view;
    iView _viewOrtho;

    uint64 _playerID = 0;
    uint64 _bossID = 0;

    iPerlinNoise _perlinNoise;

    iTextureFont* _font = nullptr;

    iScene* _scene = nullptr;

    uint64 _toolSize = 3;
    uint8 _toolDensity = 0;

    iaVector2f _mouseDelta;
    iaVector3f _weaponPos;

    iNodeTransform* _lightTranslate = nullptr;
    iNodeTransform* _lightRotate = nullptr;
    iNodeLight* _lightNode = nullptr;

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
    iVoxelTerrain* _voxelTerrain = nullptr;

    int _enemyCount = 0;

    void oulineLevelStructure();
    void deinitVoxelData();
    void initVoxelData();
    void onGenerateVoxelData(iVoxelBlockInfo* voxelBlockInfo);
    void onVoxelDataGenerated(iVoxelBlockPropsInfo voxelBlockInfo);

    bool getTerrainIntersectionPoint(iaVector3I& intersection);
    void dig(iaVector3I position, uint64 toolSize, uint8 toolDensity);

    void onKeyPressed(iKeyCode key);
    void onKeyReleased(iKeyCode key);

    void onWindowClosed();
    void onWindowResized(int32 clientWidth, int32 clientHeight);

    void onMouseMoved(const iaVector2i& from, const iaVector2i& to, iWindow* _window);

    void onMouseUp(iKeyCode key);
    void onMouseDown(iKeyCode key);
    void onMouseWheel(int d);

    void handleMouse();
    void handleHitList();

    void deinit();
    void init();

    void onRender();
    void onHandle();
    void onRenderOrtho();

    void registerHandles();
    void unregisterHandles();

    void initViews();
    void initScene();
    void initPlayer();

    void initPhysics();
    void onContactTerrainBullet(iPhysicsBody* body0, iPhysicsBody* body1);
    void onContact(iPhysicsBody* body0, iPhysicsBody* body1);

};

#endif
