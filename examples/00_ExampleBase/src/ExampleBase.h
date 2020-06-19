//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2014-2020 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: igorgameengine@protonmail.com

#ifndef __EXAMPLEBASE_H__
#define __EXAMPLEBASE_H__

#include <igor/system/iWindow.h>
#include <igor/graphics/iView.h>
#include <igor/threading/tasks/iTask.h>
#include <igor/resources/profiler/iProfilerVisualizer.h>
#include <igor/resources/texture/iTexture.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/scene/iScene.h>
using namespace igor;

#include <iaux/data/iaString.h>
using namespace iaux;

/*! Examples base class
*/
class ExampleBase
{

public:
    /*! init example
    */
    ExampleBase(const iaString &name, bool createBaseSetup = true, bool createSkyBox = true);

    /*! release resources
    */
    virtual ~ExampleBase();

    /*! \returns name of example
    */
    const iaString &getName() const;

    /*! \returns the example's window
    */
    iWindow &getWindow();

    /*! \retruns the example's view
    */
    iView &getView();

    /*! \returns the examples scene
    */
    iScenePtr getScene();

    /*! \returns font
    */
    iTextureFontPtr getFont() const;

    /*! \returns font material
    */
    iMaterialID getFontMaterial() const;

    /*! run example
    */
    virtual void run();

protected:
    /*! initialize example
    */
    virtual void init();

    /*! deinitialize example
    */
    virtual void deinit();

    /*! called every frame before draw

    override if you need to work with it
    */
    virtual void onPreDraw();

    /*! called every frame after draw

    override if you need to work with it
    */
    virtual void onPostDraw();

    /*! called by orthogonal view
    */
    virtual void onRenderOrtho();

    /*! called on key pressed event

    \param key the key code of the pressed key
    */
    virtual void onKeyDown(iKeyCode key);

    /*! called when key was released

    \param key the keycode of the released key
    */
    virtual void onKeyReleased(iKeyCode key);

    /*! called when any mouse key was pressed

    \pram key the key code of the key that was pressed
    */
    virtual void onMouseKeyDown(iKeyCode key);

    /*! called when any mouse key was released

    \param key the key code of the key that was pressed
    */
    virtual void onMouseKeyUp(iKeyCode key);

    /*! called when mouse was moved
    \param from last mouse position
    \param to current mouse position
    \param window the window the coordinates are related to
    */
    virtual void onMouseMovedFull(const iaVector2i &from, const iaVector2i &to, iWindow *window);

    /*! called when mouse has moved

    \param pos the new mouse position
    */
    virtual void onMouseMoved(const iaVector2i &pos);

    /*! called when mouse was double clicked

    \param key the key that was double clicked
    */
    virtual void onMouseDoubleClick(iKeyCode key);

    /*! called when mouse wheel was turned

    \param d mouse wheel delta
    */
    virtual void onMouseWheel(int32 d);

    /*! called when window was resized

    \param clientWidth the client rectangle width
    \param clientHeight the client rectangle height
    */
    virtual void onWindowResized(int32 clientWidth, int32 clientHeight);

private:
    /*! the window that receives the input messages
    */
    iWindow _window;

    /*! the view we render 2D to
    */
    iView _viewOrtho;

    /*! the view we render 3D to
    */
    iView _view;

    /*! the scene holding our 3d nodes
    */
    iScenePtr _scene = nullptr;

    /*! async loading of models
    */
    iTaskID _taskFlushModels = iTask::INVALID_TASK_ID;

    /*! async loading of textures
    */
    iTaskID _taskFlushTextures = iTask::INVALID_TASK_ID;

    /*! example name
    */
    iaString _name;

    /*! displays profiler
    */
    iProfilerVisualizer _profilerVisualizer;

    /*! igor logo
    */
    iTexturePtr _igorLogo = nullptr;

    /*! texture font we use to display the profiler
    */
    iTextureFontPtr _font = nullptr;

    /*! material definition for the sky box
    */
    iMaterialID _materialSkyBox = iMaterial::INVALID_MATERIAL_ID;

    /*! material for igor logo
    */
    iMaterialID _materialWithTextureAndBlending = iMaterial::INVALID_MATERIAL_ID;

    /*! draw igor logo
    */
    void drawLogo();

    /*! called when window got closed
    */
    void onCloseWindow();
};

#endif // __EXAMPLEBASE_H__
