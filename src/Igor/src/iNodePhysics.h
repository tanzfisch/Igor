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

#ifndef __iNODEPHYSICS__
#define __iNODEPHYSICS__

#include <iNodeVolume.h>
#include <iaMatrix.h>
#include <iPhysicsBody.h>
#include <iMesh.h>

#include <iaVector3.h>
#include <iaMatrix.h>
using namespace IgorAux;

#include <memory>
using namespace std;

namespace Igor
{
    /*! node that handles physics collisions boudn to one body

    \todo also need to remove things after adding them. maybe using IDs for that
    */
    class Igor_API iNodePhysics : public iNodeVolume
	{
		
		friend class iNodeVisitorUpdateTransform;
        friend class iNodeFactory;
        friend class iPhysicsManager;
        friend class iPhysicsManagerTask;

    public:

        /*! clears all lists and removes all primitives
        */
        void clear();

        /*! adds box

        \param width width of box
        \param height height of box
        \param depth depth of box
        \param offset relative position
        */
        void addBox(float32 width, float32 height, float32 depth, const iaMatrixf& offset);

        /*! adds sphere

        \param radius radius of sphere
        \param offset relative position
        */
        void addSphere(float32 radius, const iaMatrixf& offset);

        /*! adds cone

        \param radius radius of cone
        \param height height of cone
        \param offset relative position
        */
        void addCone(float32 radius, float32 height, const iaMatrixf& offset);

        /*! adds capsule

        \param radius radius of capsule
        \param height height of capsule
        \param offset relative position
        */
        void addCapsule(float32 radius, float32 height, const iaMatrixf& offset);

        /*! adds cylinder

        \param radius radius of cylinder
        \param height height of cylinder
        \param offset relative position
        */
        void addCylinder(float32 radius, float32 height, const iaMatrixf& offset);

        /*! set up mesh for physics mesh

        \param mesh the mesh to make a physics collision from
        \param faceAttribute the faceattribute for the whole mesh
        \param offset off the mesh collision object
        */
        void addMesh(shared_ptr<iMesh> mesh, int64 faceAttribute, const iaMatrixf& offset);

        /*! \returns mass of body
        */
        float32 getMass() const;

        /*! sets mass of body

        if body is initialized the actual bodys mass will also be updated

        \param mass new mass
        */
        void setMass(float32 mass);

        /*! sets a delegate to handle the ApplyForceAndTorque event

        \param applyForceAndTorqueDelegate the delegate to register
        */
        void setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate applyForceAndTorqueDelegate);

        /*! resets the delegate to handle the ApplyForceAndTorque event
        */
        void resetForceAndTorqueDelegate();

        /*! sets material id

        \param materialID the material id
        */
        void setMaterial(int64 materialID);

        /*! \returns material id
        */
        int64 getMaterial() const;

        /*! \returns the ID of the body if exists
        */
        uint64 getBodyID() const;

	private:

        /*! structure to hold the data to initialize physics with a box
        */
        struct Box
        {
            /*! width of box
            */
            float32 _width;
            float32 _height;
            float32 _depth;
            iaMatrixf _offset;
        };

        /*! structure to hold the data to initialize physics with a sphere
        */
        struct Sphere
        {
            float32 _radius;
            iaMatrixf _offset;
        };

        /*! structure to hold the data to initialize physics with a cone
        */
        struct Cone
        {
            float32 _radius;
            float32 _height;
            iaMatrixf _offset;
        };

        /*! structure to hold the data to initialize physics with a capsule
        */
        struct Capsule
        {
            float32 _radius;
            float32 _height;
            iaMatrixf _offset;
        };

        /*! structure to hold the data to initialize physics with a cylinder
        */
        struct Cylinder
        {
            float32 _radius;
            float32 _height;
            iaMatrixf _offset;
        };

        /*! structure to hold the data to initialize physics with a mesh
        */
        struct Mesh
        {
            shared_ptr<iMesh> _mesh = nullptr;
            int64 _faceAttribute;
            iaMatrixf _offset;
        };

        /*! lsit of boxes
        */
        vector<Box> _boxes;

        /*! lsit of spheres
        */
        vector<Sphere> _spheres;

        /*! lsit of cones
        */
        vector<Cone> _cones;
        
        /*! lsit of capsules
        */
        vector<Capsule> _capsules;

        /*! lsit of cylinders
        */
        vector<Cylinder> _cylinders;

        /*! lsit of meshs
        */
        vector<Mesh> _meshs;

        /*! the bodys mass
        */
        float32 _mass = 0;

        /*! material id
        */
        int64 _materialID = 0;

        /*! handles ApplyForceAndTorque event
        */
        iApplyForceAndTorqueDelegate* _applyForceAndTorqueDelegate = nullptr;

        /*! true if the physics was initialized
        */
        bool _physicsInitialized = false;

        /*! physics body
        */
        uint64 _bodyID = iPhysicsBody::INVALID_PHYSICSBODY_ID;

        virtual void initPhysics();

        /*! this is called just before setScene and gives the class the chance to unregister from the current scene if set.
        */
        virtual void onPreSetScene();

        /*! this is called just after setScene and gives the class the chance to register it self to the new scene.
        */
        virtual void onPostSetScene();

        /*! set world matrix

        \param matrix matrix to set
        */
        virtual void onUpdateTransform(iaMatrixf& matrix);

        /*! checks if physics was created and forces tree to update

        \returns true if physics data was present
        */
        bool onUpdateData();

        /*! does nothing
        */
        void draw();

        /*! \returns true if physics was already initialized
        */
        bool isInitialized();

        /*! initializes member variables
        */
        iNodePhysics();

        /*! copy ctor
        */
        iNodePhysics(iNodePhysics* node);

        /*! does nothing
        */
        virtual ~iNodePhysics();

		
	};

};

#endif

