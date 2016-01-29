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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iENTITY__
#define __iENTITY__

#include <iDefines.h>

#include <iaMatrix.h>
#include <iaVector3.h>
using namespace IgorAux;

#include <memory>
#include <map>
using namespace std;

namespace Igor
{

    class iPhysicsBody;
    class iNodeTransform;

    /*! an entity is something you want to play with. like a character or vehicle or intercative object
    */
    class Igor_API iEntity
    {

        friend class iEntityFactory;
        friend class iEntityControl;
        friend void PhysicsNodeSetTransform(const void* body, const float* matrix, int threadIndex);

    public:

        /*! the invalid entity id
        */
        static const uint32 INVALID_ENTITY_ID = 0;

        /*! sets the matrix of the entity

        \param matrix matrix to be set
        */
        void setMatrix(const iaMatrixf& matrix);

        /*! returns the matrix of this entity

        \param[out] matrix to be returned
        */
        void getMatrix(iaMatrixf& matrix);

        /*! set position of entity

        \param positin position to be set
        */
        void setPosition(const iaVector3f& position);

        /*! \returns position of entity
        */
        iaVector3f getPosition();

        /*! set oriantation with euler angles in radians

        \param orientation euler angles in radians
        */
        void setOrientation(const iaVector3f& orientation);

        /*! \returns orientation in euler angles in radians
        */
        iaVector3f getOrientation();

        /*! sets velocity of this entity

        \param velocity velocity to be set
        */
        void setVelocity(const iaVector3f& velocity);

        /*! \returns velocity of the entity
        */
        iaVector3f getVelocity();

        /*! sets force to this entity

        \param force force to be set
        */
        void setForce(const iaVector3f& force);

        /*! \returns force to the entity
        */
        iaVector3f getForce();

        /*! sets angular velocity of this entity

        \param angularVelocity the angular velocity to be set
        */
        void setAngularVelocity(const iaVector3f& angularVelocity);

        /*! \returns the angular velocity of this entity
        */
        iaVector3f getAngularVelocity();

        /*! sets angular force of this entity

        \param angularForce the angular force to be set
        */
        void setAngularForce(const iaVector3f& angularForce);

        /*! \returns the angular force of this entity
        */
        iaVector3f getAngularForce();

        /*! binds entity to phsical body

        \param body physical body
        */
        void setBody(iPhysicsBody* body);

        /*! \returns newton body
        */
        iPhysicsBody* getBody() const;

        /*! bind transform node to entity

        \param node the transform node
        */
        void setTransformNode(iNodeTransform* node);

        /*! \returns transform node of entity
        */
        iNodeTransform* getTransformNode();

        /*! \retruns the id of this entity
        */
        uint32 getID();

        /*! set custom value of type bool

        \param index index of velue to set
        \param value value to set
        */
        void setBool(uint32 index, bool value);

        /*! set custom value of type float

        \param index index of velue to set
        \param value value to set
        */
        void setFloat(uint32 index, float32 value);

        /*! set custom value of type int

        \param index index of velue to set
        \param value value to set
        */
        void setInt(uint32 index, int32 value);

        /*! \returns custom value of type bool

        \param index index of value to return
        */
        bool getBool(uint32 index);

        /*! \returns custom value of type float

        \param index index of value to return
        */
        float32 getFloat(uint32 index);

        /*! \returns custom value of type int

        \param index index of value to return
        */
        int32 getInt(uint32 index);

        /*! \returns dirty velocity flag
        */
        bool isDirtyVelocity();

        /*! \returns dirty angular velocity flag
        */
        bool isDirtyAngularVelocity();

        /*! \returns dirty force flag
        */
        bool isDirtyForce();

        /*! \returns dirty angular force flag
        */
        bool isDirtyAngularForce();

        /*! \returns dirty matrix flag

        \todo might be interessing to differ between position and orientation
        */
        bool isDirtyMatrix();

        /*! resets the dirty flag
        */
        void resetDirty();

        /*! \returns true if entity contains body
        */
        bool hasBody();

        /*! \returns true if entity contains node
        */
        bool hasNode();

    private:

        /*! entity unique id
        */
        uint32 _id = 0;

        /*! matrix of entity. contains orientation and position of entity
        */
        iaMatrixf _matrix;

        /*! current velocity of entity in m/s
        */
        iaVector3f _velocity;

        /*! current angular velocity in radians/s
        */
        iaVector3f _angularVelocity;

        /*! force or acceleration to apply to entity in m/(s*s)
        */
        iaVector3f _force;

        /*! current angular force
        */
        iaVector3f _angularForce;
        
        /*! dirty flag velocity
        */
        bool _dirtyVelocity = true;

        /*! dirty flag angular velocity
        */
        bool _dirtyAngularVelocity = true;

        /*! dirty flag force
        */
        bool _dirtyForce = true;

        /*! dirty flag angular force
        */
        bool _dirtyAngularForce = true;

        /*! dirty flag matrix
        */
        bool _dirtyMatrix = true;

        /*! dirty physics flag. true if updateMatrix was called
        */
        bool _dirtyPhysics = true;

        /*! shared pointer to optional physical body
        */
        iPhysicsBody* _body = nullptr;

        /*! transformation node
		*/
        iNodeTransform* _node = nullptr;
        
        /*! custom data bools
        */
        map<uint32, bool> _customBools;

        /*! custom data integers
        */
        map<uint32, int32> _customInts;

        /*! custom data floats
        */
        map<uint32, float32> _customFloats;

        /*! matrix updated by physics
        */
        void updateMatrix(const iaMatrixf& matrix);

        /*! \todo
        */
        void onHandle();

        /*! updates physics if body is connected
        */
        void updatePhysics();

        /*! updates scene if node is connected
        */
        void updateScene();

        /*! does nothing
        */
        iEntity() = default;

        /*! does nothing
        */
        virtual ~iEntity() = default;

    };

}

#endif