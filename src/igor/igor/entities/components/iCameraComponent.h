//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IGOR_CAMERA_COMPONENT_H
#define IGOR_CAMERA_COMPONENT_H

#include <igor/entities/iEntity.h>

namespace igor
{
    /*! projection type definition
     */
    enum class iProjectionType
    {
        Perspective,
        Orthogonal
    };

    /*! camera component
     */
    class iCameraComponent : public iEntityComponent
    {
    public:
        /*! ctor
         */
        iCameraComponent();

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance();

        /*! \returns type name of component
         */
        static const iaString &getTypeName();

        /*! sets the view port within a window.

        values have to be from 0.0 to 1.0 and represent a resolution independent unit.

        TODO need a pixel version of this

        \param rect rectangle with view port boundings using relative values 0.0 - 1.0
        */
        void setViewportRelative(const iaRectangled &rect);

        /*! \returns viewport in pixels
         */
        const iaRectangled &getViewport() const;

        /*! activates perspective mode and sets the view_angle

        \param viewAngel values in degrees 0 < viewAngel < 180
        */
        void setPerspective(float64 viewAngel);

        /*! activates othogonal projection mode

        \param left left value of orthogonal projection
        \param right right value of orthogonal projection
        \param bottom bottom value of orthogonal projection
        \param top top value of orthogonal projection
        */
        void setOrthogonal(float32 left, float32 right, float32 bottom, float32 top);

        /*! \returns field of view
         */
        float64 getFieldOfView() const;

        /*! defines the near and far clipping planes

        \param nearPlain near clipping plane
        \param farPlain far clipping plane
        */
        void setClipPlanes(float32 nearPlain, float32 farPlain);

        /*! \returns near clipping plane
         */
        float64 getNearClipPlane() const;

        /*! \returns far clipping plane
         */
        float64 getFarClipPlane() const;

        /*! sets the clear color bit.

        \param active true: color buffer will be cleared every frame; false: it will not
        */
        void setClearColorActive(bool active);

        /*! \returns true if color buffer will be cleared before render
         */
        bool isClearColorActive() const;

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

        /*! \returns projection type
         */
        iProjectionType getProjectionType() const;

        /*! \returns left edge of orthogonal projection
         */
        float64 getLeftOrtho() const;

        /*! \returns right edge of orthogonal projection
         */
        float64 getRightOrtho() const;

        /*! \returns top edge of orthogonal projection
         */
        float64 getTopOrtho() const;

        /*! \returns bottom edge of orthogonal projection
         */
        float64 getBottomOrtho() const;

    private:
        /*! viewport
         */
        iaRectangled _viewport = {0.0, 0.0, 1.0, 1.0};

        /*! projection type
         */
        iProjectionType _projection = iProjectionType::Perspective;

        /*! field of view
         */
        float64 _fieldOfView = 45.0;

        /*! near clip plane
         */
        float64 _clipNear = 1.0;

        /*! far clip plane
         */
        float64 _clipFar = 10000.0;

        /*! if true clear the color buffer
         */
        bool _clearColorActive = true;

        /*! if true clear the depth buffer
         */
        bool _clearDepthActive = true;

        /*! the clear color
         */
        iaColor4f _clearColor = iaColor4f::gray;

        /*! clear depth value
         */
        float64 _clearDepth = 1.0;

        /*! left value used for orthogonal projection
         */
        float64 _leftOrtho = -1.0;

        /*! right value used for orthogonal projection
         */
        float64 _rightOrtho = 1.0;

        /*! top value used for orthogonal projection
         */
        float64 _topOrtho = 1.0;

        /*! bottom value used for orthogonal projection
         */
        float64 _bottomOrtho = -1.0;

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override;
    };

}

#endif // IGOR_CAMERA_COMPONENT_H