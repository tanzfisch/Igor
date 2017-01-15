#ifndef __ASCENT__
#define __ASCENT__

#include <Igor.h>
#include <iWindow.h>
#include <iView.h>
#include <iTimerHandle.h>
#include <iModelResourceFactory.h>
#include <iKeyboard.h>
using namespace Igor;

#include <iaMatrix.h>
using namespace IgorAux;

#include "VoxelTerrainGenerator.h"

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
    class iNodeTransformControl;
    class iNodeLODTrigger;
}

class Ascent
{

public:

    Ascent();
	virtual ~Ascent();

	void run();

private:

    bool _loading = true;
    bool _activeControls = false;

    iWindow _window;
    iView _view;
    iView _viewOrtho;

    uint64 _playerID = 0;
    uint64 _bossID = 0;

    iTextureFont* _font = nullptr;

    iScene* _scene = nullptr;

    uint64 _toolSize = 10;
    uint8 _toolDensity = 0;

    iaVector2f _mouseDelta;
    iaVector3f _weaponPos;

    iNodeTransform* _lightTranslate = nullptr;
    iNodeTransform* _lightRotate = nullptr;
    iNodeLight* _lightNode = nullptr;

    uint32 _materialSolid = 0;

    float64 _startTime;
    
    uint32 _materialWithTextureAndBlending = 0;
    uint32 _octreeMaterial = 0;
    int32 _materialSkyBox = 0;

    uint64 _taskFlushModels = 0; 
    uint64 _taskFlushTextures = 0;
    
    void onKeyPressed(iKeyCode key);
    void onKeyReleased(iKeyCode key);

    void onWindowClosed();
    void onWindowResized(int32 clientWidth, int32 clientHeight);

    void onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window);

    void onVoxelDataGenerated(const iaVector3I& min, const iaVector3I& max);
    
    void onMouseUp(iKeyCode key);
    void onMouseDown(iKeyCode key);
    void onMouseWheel(int d);

    void handleMouse();

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
    void initVoxelData();
    void drawReticle();

    void registerEntityTypes();
    void unregisterEntityTypes();

};

#endif
