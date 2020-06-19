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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __iLIGHTNODE__
#define __iLIGHTNODE__

#include <igor/scene/nodes/iNode.h>
#include <iaux/math/iaMatrix.h>
#include <iaux/data/iaColor4.h>

namespace igor
{

    /*! type of light
    */
    enum class iLightType
    {
        Directional,
        Point,
        Undefined
    };

    /*! this node implementation controls a light within the scene
	
	\todo implement switch on/off
    \bug orientation of light in world and model space is diffrent or wrong
    \todo does it realy need to be color4f?
	*/
    class Igor_API iNodeLight : public iNode
    {

        friend class iNodeVisitorUpdateTransform;
        friend class iNodeManager;

    public:
        /*! returns world matrix

        \param[out] matrix returned world matrix
        */
        void getWorldMatrix(iaMatrixd &matrix);

        /*! returns ambient material color
        */
        iaColor4f &getAmbient();

        /*! set ambient material color

        \param color ambient color
        */
        void setAmbient(const iaColor4f &color);

        /*! returns diffuse material color
        */
        iaColor4f &getDiffuse();

        /*! set diffuse material color

        \param color diffuse color
        */
        void setDiffuse(const iaColor4f &color);

        /*! returns specular material color
        */
        iaColor4f &getSpecular();

        /*! set speculer material color

        \param color speculer color
        */
        void setSpecular(const iaColor4f &color);

        /*! set type of light

        \param the type of light
        */
        void setType(iLightType type);

        /*! \returns type of light
        */
        iLightType getType();

        /*! \returns position of light
        */
        iaVector4d getPosition();

        /*! \returns direction of light
        */
        iaVector4d getDirection() const;

    protected:
        /*! resulting world matrix of light
        */
        iaMatrixd _worldMatrix;

        /*! ambient material color
        */
        iaColor4f _ambient = {0.7f, 0.7f, 0.7f, 1.0f};

        /*! diffuse material color
        */
        iaColor4f _diffuse = {0.9f, 0.9f, 0.9f, 1.0f};

        /*! specular material color
        */
        iaColor4f _specular = {1.0f, 1.0f, 1.0f, 1.0f};

        /*! type of light
        */
        iLightType _type = iLightType::Directional;

        /*! unregister from scene
        */
        virtual void onPreSetScene();

        /*! register to scene
        */
        virtual void onPostSetScene();

        /*! set world matrix

        \param matrix matrix to set
        */
        void onUpdateTransform(iaMatrixd &matrix);

    private:
        /*! initializes member variables
        */
        iNodeLight();

        /*! copy ctor
        */
        iNodeLight(iNodeLight *node);

        /*! does nothing
        */
        virtual ~iNodeLight();
    };

}; // namespace igor

#endif
