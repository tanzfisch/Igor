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

        friend class iEntityControl;
        friend class iEntityFactory;
        friend void PhysicsNodeSetTransform(const void* body, const float* matrix, int threadIndex);

    public:

        /*! the invalid entity id
        */
        static const uint32 INVALID_ENTITY_ID = 0;

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
        
        /*! \returns custom value of type bool

        \param index index of value to return
        */
        bool getBool(uint32 index);

        /*! \returns custom value of type float

        \param index index of value to return
        */
        float32 getFloat(uint32 index);

        /*! set custom value of type int

        \param index index of velue to set
        \param value value to set
        */
        void setInt(uint32 index, int64 value);

        /*! \returns custom value of type int

        \param index index of value to return
        */
        int64 getInt(uint32 index);

        /*! set custom value of type int

        \param index index of velue to set
        \param value value to set
        */
        void setUInt(uint32 index, uint64 value);

        /*! \returns custom value of type int

        \param index index of value to return
        */
        uint64 getUInt(uint32 index);

        /*! sets a vector with a specified index

        \param index the indext to put the vector in
        \param value the vectors value
        */
        void setVector(uint32 index, const iaVector3f& value);

        /*! \returns vector for given index

        \param index the given index
        */
        const iaVector3f& getVector(uint32 index);

        void setMatrix(uint32 index, const iaMatrixf& value);
        const iaMatrixf& getMatrix(uint32 index);

    protected:

        /*! entity unique id
        */
        uint32 _id = 0;

        /*! custom data bools
        */
        map<uint32, bool> _customBools;

        /*! custom data integers
        */
        map<uint32, int32> _sInts;

        /*! custom data unsigned integers
        */
        map<uint32, int32> _uInts;

        /*! custom data floats
        */
        map<uint32, float32> _customFloats;

        /*! custom data vectors
        */
        map<uint32, iaVector3f> _vectors;

        map<uint32, iaMatrixf> _matrices;

        /*! called every frame
        */
        virtual void onHandle() = 0;

        /*! does nothing
        */
        iEntity() = default;

        /*! does nothing
        */
        virtual ~iEntity() = default;

    };

}

#endif