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

#ifndef __IGOR_LIGHTNODE__
#define __IGOR_LIGHTNODE__

#include <igor/scene/nodes/iNode.h>
#include <iaux/math/iaMatrix.h>
#include <iaux/data/iaColor3.h>

namespace igor
{

    /*! this node implementation controls a light within the scene
	
	\todo implement switch on/off
    \bug orientation of light in world and model space is diffrent or wrong
    \todo does it realy need to be color4f?
	*/
    class IGOR_API iNodeLight : public iNode
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
        iaColor3f &getAmbient();

        /*! set ambient material color

        \param color ambient color
        */
        void setAmbient(const iaColor3f &color);

        /*! set ambient material color

        \param r red
        \param g green
        \param b blue
        */
        void setAmbient(float32 r, float32 g, float32 b);

        /*! returns diffuse material color
        */
        iaColor3f &getDiffuse();

        /*! set diffuse material color

        \param color diffuse color
        */
        void setDiffuse(const iaColor3f &color);

        /*! set diffuse material color

        \param r red
        \param g green
        \param b blue
        */
        void setDiffuse(float32 r, float32 g, float32 b);        

        /*! returns specular material color
        */
        iaColor3f &getSpecular();

        /*! set specular material color

        \param color specular color
        */
        void setSpecular(const iaColor3f &color);

        /*! set specular material color

        \param r red
        \param g green
        \param b blue
        */
        void setSpecular(float32 r, float32 g, float32 b);          

        /*! set type of light

        \param the type of light
        */
        void setType(iLightType type);

        /*! \returns type of light
        */
        iLightType getType() const;

        /*! \returns position of light
        */
        const iaVector3d& getPosition() const;

        /*! \returns direction of light
        */
        const iaVector3d& getDirection() const;

    protected:
        /*! resulting world matrix of light
        */
        iaMatrixd _worldMatrix;

        /*! ambient material color
        */
        iaColor3f _ambient = {0.7f, 0.7f, 0.7f};

        /*! diffuse material color
        */
        iaColor3f _diffuse = {0.9f, 0.9f, 0.9f};

        /*! specular material color
        */
        iaColor3f _specular = {1.0f, 1.0f, 1.0f};

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

    /*! light node pointer definition
    */
    typedef iNodeLight *iNodeLightPtr;

}; // namespace igor

#endif
