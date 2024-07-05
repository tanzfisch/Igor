//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_VIEW_H
#define IGOR_VIEW_H

#include <igor/iDefines.h>
#include <igor/renderer/iRenderEngineOld.h>
#include <igor/renderer/iRenderEngine.h>
#include <igor/entities/iEntityScene.h>

#include <iaux/data/iaRectangle.h>
#include <iaux/data/iaColor4.h>
#include <iaux/system/iaEvent.h>
#include <iaux/data/iaString.h>
#include <iaux/math/iaMatrix.h>
using namespace iaux;

#include <vector>

namespace igor
{

    class iWindow;
    class iPixmap;
    class iNodeCamera;
    class iOctreeObject;

    /*! draw event triggered for every frame a view is rendered
     */
    IGOR_EVENT_DEFINITION(iDraw, void);

    /*! represents a view rectangle within a window and projection of the scene

    similar to what the combination of glViewport and glFrustum do

    \todo would be nice to have that again -> iPixmap* makeScreenshot(bool alphachannel=false);
    \todo pre and post render event
    */
    class IGOR_API iView
    {
        friend class iWindow;
        friend class iWidgetViewport;

    public:
        /*! does nothing
         */
        iView() = default;

        /*! checks consistency
         */
        virtual ~iView();

        /*! sets the scene to be rendered with this view

        \param scene scene to be rendered
        */
        void setScene(iScenePtr scene);

        /*! \returns scene to be rendered with this view
         */
        iScenePtr getScene() const;

        /*! sets the entity scene to be rendered with this view

        \param entityScene entity scene to be rendered
        */
        void setEntityScene(iEntityScenePtr entityScene);

        /*! \returns scene to be rendered with this view
         */
        iEntityScenePtr getEntityScene() const;

        /*! sets name of view

        \param name name of the view
        */
        void setName(const iaString &name);

        /*! \returns name of the view
         */
        const iaString &getName() const;

        /*! \returns aspect ratio
         */
        float64 getAspectRatio() const;

        /*! registers delegate to render event

        \param renderDelegate delegate to register
        */
        void registerRenderDelegate(iDrawDelegate renderDelegate);

        /*! unregister delegate from render event

        \param renderDelegate delegate to unregister
        */
        void unregisterRenderDelegate(iDrawDelegate renderDelegate);

        /*! sets the view port within a window.

        values have to be from 0.0 to 1.0 and represent a resolution independent unit.

        TODO need a pixel version of this

        \param rect rectangle with view port boundings using relative values 0.0 - 1.0
        */
        void setViewportRelative(iaRectanglef rect);

        /*! \returns viewport in pixels
         */
        iaRectanglei getViewport() const;

        /*! activates perspective mode and sets the view_angle

        \param viewAngel values in degrees 0 < viewAngel < 180
        */
        void setPerspective(float32 viewAngel);

        /*! activates othogonal projection mode

        \param left left value of orthogonal projection
        \param right right value of orthogonal projection
        \param bottom bottom value of orthogonal projection
        \param top top value of orthogonal projection
        */
        void setOrthogonal(float32 left, float32 right, float32 bottom, float32 top);

        /*! defines the near and far clipping planes

        \param nearPlain near clipping plane
        \param farPlain far clipping plane
        */
        void setClipPlanes(float32 nearPlain, float32 farPlain);

        /*! sets the clear color bit.

        \param active true: color buffer will be cleared every frame; false: it will not
        */
        void setClearColorActive(bool active);

        /*! \returns true if color buffer will be cleared before render
         */
        bool isClearColorActive() const;

        /*! sets if the viewport will be updated during draw call

        \param enabled if true viewport will be updated before drawing (enabled is default)
        */
        void setUpdateViewport(bool enabled);

        /*! \returns true if viewport will be updated before drawing
         */
        bool isUpdatingViewport() const;

        /*! specifies the the color the color buffer will be cleared with

        \param color the clear color
        */
        void setClearColor(const iaColor4f &color);

        /*! specifies the the color the color buffer will be cleared with

        \param r red channel of clear color
        \param g green channel of clear color
        \param b blue channel of clear color
        \param a alpha channel of clear color
        */
        void setClearColor(float32 r, float32 g, float32 b, float32 a);

        /*! \returns current clear color
         */
        const iaColor4f &getClearColor() const;

        /*! sets the clear buffer bit.

        \param active true: depth buffer will be cleared every frame; false: it will not
        */
        void setClearDepthActive(bool active);

        /*! \returns true if clear depth is active
         */
        bool isClearDepthActive() const;

        /*! sets the clear depth value

        \param depth the clear depth value
        */
        void setClearDepth(float32 depth);

        /*! \returns clear depth Value
         */
        float32 getClearDepth() const;

        /*! sets view visible

        \param visible the visible flag
        */
        void setVisible(bool visible = true);

        /*! \returns true if view is visible
         */
        bool isVisible() const;

        /*! shows all rendering in wireframe mode or normal mode

        \param wireframe if true all rendering is using wireframe
        */
        void setWireframeVisible(bool wireframe = true);

        /*! \returns true if wireframe mode is active
         */
        bool isWireframeVisible() const;

        /*! defines if bounding boxes are shown or not

        \param boundingBox if true bounding boxes are shown
        */
        void setBoundingBoxVisible(bool boundingBox = true);

        /*! \returns true if bounding boxes are shown
         */
        bool isBoundingBoxVisible() const;

        /*! defines if octree is shown or not

        \param octree if true octree is shown

        \bug currently the octree is not displayed correctly
        */
        void setOctreeVisible(bool octree = true);

        /*! \returns true if octree is shown
         */
        bool isOctreeVisible() const;

        /*! sets current camera by id

        \param cameraID the camery id
        */
        void setCamera(iNodeID cameraID);

        /*! \returns current camera id
         */
        iNodeID getCamera() const;

        /*! unprojects screen position to object space

        \param screenpos screen position in pixels (vertical origin is at top of window)
        \param modelMatrix the camera matrix to create the model view matrix from
        \returns unprojected position in object space
        */
        iaVector3d unProject(const iaVector3d &screenpos, const iaMatrixd &modelMatrix);

        /*! project world position to screen coordinates

        \param worldSpacePos the position in world space to project
        \param cameraMatrix the camera matrix
        \returns projected screen position
        */
        iaVector3d project(const iaVector3d &worldSpacePos, const iaMatrixd &cameraMatrix);

        /*! renders view in an offscreen buffer using the colorID material and returns the color id at given point.

        Top left is origin.

        \param posx horizontal position of point in pixel
        \param posy vertical position of point in pixel
        \returns color id at given point (results are only valid for IDs <= 0xFFFFFF in use)
        */
        uint64 pickColorID(uint32 posx, uint32 posy);

        /*! same as pickColorID but with different parameters

        \param pos the position to pick a color from
        */
        uint64 pickColorID(const iaVector2i &pos);

        /*! renders view in offscreen buffer using the colorID material and returns the color IDs from given rectangle
         */
        void pickColorID(const iaRectanglei &rectangle, std::vector<uint64> &colorIDs);

        /*! \returns the z index of this view
         */
        int32 getZIndex() const;

        /*! \returns true if configure with perspective projection

        else it must be a orthogonal projection
        */
        bool isPerspective() const;

    private:
        /*! z index
         */
        int32 _zIndex = 0;

        /*! visible flag
         */
        bool _visible = true;

        /*! if true this view will be rendered in wireframe mode
         */
        bool _wireframeEnabled = false;

        /*! views name
         */
        iaString _name = "iView";

        /*! scene that is currently bound with this view
         */
        iScenePtr _scene = nullptr;

        /*! entity scene to render
         */
        iEntityScenePtr _entityScene = nullptr;

        /*! size of parenting window in pixel
         */
        iaRectanglei _windowRect;

        /*! the viewingport with values from 0.0-1.0 so it is independent form screen resolution
         */
        iaRectanglef _viewportConfig = {0.0f, 0.0f, 1.0f, 1.0f};

        /*! viewport in pixel
         */
        iaRectanglei _viewport;

        /*! if true the color buffer will be cleared with _clearColor before every frame
         */
        bool _clearColorActive = true;

        /*! if true viewport gets updated before drawing
         */
        bool _updateViewport = true;

        /*! the color definition of the clear color step
         */
        iaColor4f _clearColor = {0.0f, 0.0f, 0.0f, 1.0f};

        /*! if true the depth buffer will be cleared with _clearDepth before every frame
         */
        bool _clearDepthActive = true;

        /*! clear depth value
         */
        float32 _clearDepth = 1.0;

        /*! if true rendering will use a perspective projection.
        if false rendering will use a orthogonal projection
        */
        bool _perspective = true;

        /*! left value used for orthogonal projection
         */
        float64 _left = -1.0;

        /*! right value used for orthogonal projection
         */
        float64 _right = 1.0;

        /*! top value used for orthogonal projection
         */
        float64 _top = 1.0;

        /*! bottom value used for orthogonal projection
         */
        float64 _bottom = -1.0;

        /*! field of view
         */
        float64 _viewAngel = 45.0;

        /*! distance from camera to near clipping plane
         */
        float64 _nearPlaneDistance = 1.0;

        /*! distance from camera to far clipping plane
         */
        float64 _farPlaneDistance = 10000.0;

        /*! event called one per render frame
         */
        iDrawEvent _renderEvent;

        /*! old render engine
         */
        iRenderEngineOld _renderEngineOld;

        /*! render engine
        */
        iRenderEngine _renderEngine;

        /*! sets the z index of this view. will be used by window to determine the render order

        \param zindex the z index to be set
        */
        void setZIndex(int32 zindex);

        /*! called every render frame by the parenting window
         */
        void draw();

        /*! updates window rectangle

        \param windowRect the new window rectangle
        */
        void updateWindowRect(const iaRectanglei &windowRect);
    };

    /*! view pointer definition
     */
    typedef iView *iViewPtr;

}; // namespace igor

#endif // IGOR_VIEW_H
