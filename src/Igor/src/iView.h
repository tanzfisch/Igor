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
// (c) Copyright 2014-2016 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iVIEW__
#define __iVIEW__

#include <iDefines.h>
#include <iRectangle.h>
#include <iRenderEngine.h>

#include <iaColor4.h>
#include <iaEvent.h>
#include <iaString.h>
#include <iaMatrix.h>
using namespace IgorAux;

namespace Igor
{

    class iWindow;
	class iPixmap;
    class iScene;
    class iNodeCamera;
    class iOctreeObject;

    /*! render event triggered for every frame a view is rendered
    */
	iaEVENT(RenderEvent, RenderDelegate, void, (), ());

    /*! represents a view rectangle within a window and projection of the scene

    similar to what the combination of glViewport and glFrustum do

    \todo would be nice to have that again -> iPixmap* makeScreenshot(bool alphachannel=false);
    */
	class Igor_API iView
	{
		friend class iWindow;

    public:

        /*! sets the scene to be rendered with this view

        \param scene scene to be rendered
        */
        void setScene(iScene* scene);

        /*! \returns scene to be rendered with this view
        */
        iScene* getScene();

        /*! sets name of view

        \param name name of the view
        */
        void setName(const iaString& name);

        /*! \returns name of the view
        */
        const iaString& getName() const;

        /*! \returns aspect ratio
        */
        float32 getAspectRatio() const;

        /*! registers delegate to render event

        \param renderDelegate delegate to register
        */
        void registerRenderDelegate(RenderDelegate renderDelegate);

        /*! unregister delegate from render event

        \param renderDelegate delegate to unregister
        */
        void unregisterRenderDelegate(RenderDelegate renderDelegate);

        /*! sets the view port within a window.

        values have to be from 0.0 to 1.0 and represent a resolution independent unit.

        \param rect rectangle with view port boundings usnig relative values 0.0 - 1.0
        */
        void setViewport(iRectanglef rect);

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

        \param nearPlane near clipping plane
        \param farPlane far clipping plane
        */
        void setClipPlanes(float32 nearPlane, float32 farPlane);

        /*! sets the clear color bit.

        \param active true: color buffer will be cleared every frame; false: it will not
        */
        void setClearColor(bool active);

        /*! specifies the the color the color buffer will be cleared with

        \param color the clear color
        */
        void setClearColor(iaColor4f& color);

        /*! specifies the the color the color buffer will be cleared with

        \param r red channel of clear color
        \param g green channel of clear color
        \param b blue channel of clear color
        \param a alpha channel of clear color
        */
        void setClearColor(float32 r, float32 g, float32 b, float32 a);

        /*! sets the clear buffer bit.

        \param active true: depth buffer will be cleared every frame; false: it will not
        */
        void setClearDepth(bool active);

        /*! sets the clear depth value

        \param depth the clear depth value
        */
        void setClearDepth(float32 depth);

        iaVector3f unProject(const iaVector3f& screenpos, const iaMatrixf& modelMatrix);

        void updateWindowRect(const iRectanglei& windowRect);

        /*! init
        */
        iView();

        /*! checks consistency
        */
        virtual ~iView();

	private:

        uint32 _scenePreparationSectionID = 0;
        uint32 _postRenderSectionID = 0;

        /*! views name
        */
        iaString _name = "iView";

        /*! scene that is currently bound with this view
        */
        iScene* _scene = nullptr;

        iRectanglei _windowRect;

        iRectanglei _resultingRectangle;
		
        /*! the viewing rectangle with values from 0.0-1.0 so it is independent form screen resolution
        */
        iRectanglef	_viewRect = { 0.0f, 0.0f, 1.0f, 1.0f };

        /*! if true the color buffer will be cleared with _clearColor before every frame
        */
		bool _clearColorActive = true;

        /*! the color definition of the clear color step
        */
        iaColor4f _clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

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
		float32 _left = -1.0f;

        /*! right value used for orthogonal projection
        */
        float32 _right = 1.0f;

        /*! top value used for orthogonal projection
        */
		float32 _top = 1.0f;

        /*! bottom value used for orthogonal projection
        */
        float32 _bottom = -1.0f;
		
        /*! field of view
        */
        float32 _viewAngel = 45.0f;

        /*! distance from camera to near clipping plane
        */
        float32 _nearPlaneDistance = 1.0f;

        /*! distance from camera to far clipping plane
        */
        float32 _farPlaneDistance = 10000.0f;

        /*! event called one per render frame
        */
		RenderEvent _renderEvent;

        /*! render engine that turns a scene in to something visible
        */
        iRenderEngine _renderEngine;

        /*! called every render frame by the parenting window
        */
        void draw();

	};

};

#endif
