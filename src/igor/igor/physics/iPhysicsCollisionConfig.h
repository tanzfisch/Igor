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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_PHYSICSCOLLISIONCONFIG__
#define __IGOR_PHYSICSCOLLISIONCONFIG__

#include <igor/resources/mesh/iMesh.h>
#include <igor/physics/iPhysicsCollision.h>

#include <iaux/data/iaIDGenerator.h>
#include <iaux/math/iaVector3.h>
#include <iaux/math/iaMatrix.h>
#include <iaux/system/iaMutex.h>
using namespace iaux;

#include <memory>
#include <vector>

namespace igor
{

    /*! collision configuration is to be finalized by iPhysics or iTaskPrepareCollision
    */
    class IGOR_API iPhysicsCollisionConfig
    {

        friend class iTaskPrepareCollision;
        friend class iPhysics;

    public:
        /*! definition of invalid collision config id
        */
        static const iaID64 INVALID_COLLISIONCONFIG_ID = IGOR_INVALID_ID;

        /*! \returns collision config id
        */
        uint64 getID();

        /*! copy data from other config

        \param physicsCollisionConfig source to copy data from
        */
        void set(const iPhysicsCollisionConfig *physicsCollisionConfig);

        /*! adds box

        \param width width of box
        \param height height of box
        \param depth depth of box
        \param offset relative position
        */
        void addBox(float32 width, float32 height, float32 depth, const iaMatrixd &offset);

        /*! adds sphere

        \param radius radius of sphere
        \param offset relative position
        */
        void addSphere(float32 radius, const iaMatrixd &offset);

        /*! adds cone

        \param radius radius of cone
        \param height height of cone
        \param offset relative position
        */
        void addCone(float32 radius, float32 height, const iaMatrixd &offset);

        /*! adds capsule

        \param radius radius of capsule
        \param height height of capsule
        \param offset relative position
        */
        void addCapsule(float32 radius0, float32 radius1, float32 height, const iaMatrixd &offset);

        /*! adds cylinder

        \param radius radius of cylinder
        \param height height of cylinder
        \param offset relative position
        */
        void addCylinder(float32 radius0, float32 radius1, float32 height, const iaMatrixd &offset);

        /*! set up mesh for physics mesh

        \param mesh the mesh to make a physics collision from
        \param faceAttribute the face attribute for the whole mesh
        \param offset off the mesh collision object
        */
        void addMesh(iMeshPtr mesh, int64 faceAttribute, const iaMatrixd &offset);

        /*! \returns resulting collision ID

        returns INVALID_PHYSICSCOLLISION_ID if collision config was not finalized yet
        */
        uint64 getCollisionID();

        /*! creates the collisions
        */
        void finalize(uint64 worldID);

        /*! \returns true if collision is finalized
        */
        bool isFinalized();

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
            iaMatrixd _offset;
        };

        /*! structure to hold the data to initialize physics with a sphere
        */
        struct Sphere
        {
            float32 _radius;
            iaMatrixd _offset;
        };

        /*! structure to hold the data to initialize physics with a cone
        */
        struct Cone
        {
            float32 _radius;
            float32 _height;
            iaMatrixd _offset;
        };

        /*! structure to hold the data to initialize physics with a capsule
        */
        struct Capsule
        {
            float32 _radius0;
            float32 _radius1;
            float32 _height;
            iaMatrixd _offset;
        };

        /*! structure to hold the data to initialize physics with a cylinder
        */
        struct Cylinder
        {
            float32 _radius0;
            float32 _radius1;
            float32 _height;
            iaMatrixd _offset;
        };

        /*! structure to hold the data to initialize physics with a mesh
        */
        struct Mesh
        {
            iMeshPtr _mesh = nullptr;
            int64 _faceAttribute;
            iaMatrixd _offset;
        };

        /*! id generator
         */
        static iaIDGenerator64 _idGenerator;

        /*! collision config ID
        */
        iaID64 _id = INVALID_COLLISIONCONFIG_ID;

        /*! saves next id
        */
        iaMutex _mutex;

        /*! lsit of boxes
        */
        std::vector<Box> _boxes;

        /*! lsit of spheres
        */
        std::vector<Sphere> _spheres;

        /*! lsit of cones
        */
        std::vector<Cone> _cones;

        /*! lsit of capsules
        */
        std::vector<Capsule> _capsules;

        /*! lsit of cylinders
        */
        std::vector<Cylinder> _cylinders;

        /*! lsit of meshs
        */
        std::vector<Mesh> _meshs;

        /*! true if data was already finalized
        */
        bool _finalized = false;

        /*! collision id creted after finalisation
        */
        uint64 _collisionID = iPhysicsCollision::INVALID_PHYSICSCOLLISION_ID;

        /*! init id
        */
        iPhysicsCollisionConfig();

        /*! copy ctor
        */
        iPhysicsCollisionConfig(const iPhysicsCollisionConfig *physicsCollisionConfig);

        /*! does nothing
        */
        virtual ~iPhysicsCollisionConfig() = default;
    };

}; // namespace igor

#endif
