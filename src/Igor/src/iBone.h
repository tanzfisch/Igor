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

#ifndef __iBONE__
#define __iBONE__

#include <iDefines.h>

#include <iaMatrix.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{

    class Igor_API iBone
    {

        friend class iBoneFactory;
        friend class iSkeleton;
        friend class iJoint;

    public:

        /*! invalid bone ID
        */
        static const uint64 INVALID_BONE_ID = 0;

        /*! sets matrix of bone

        \param matrix the new matrix
        */
        void setMatrix(const iaMatrixf& matrix);

        /*! returns matrix of bone

        \param[out] matrix destination matrix to write the bone matrix in to
        */
        void getMatrix(iaMatrixf& matrix) const;

        /*! sets lenght of bone

        \param lenght the lenght of the bone
        */
        void setLenght(float64 lenght);

        /*! \returns the lenght of the bone
        */
        float64 getLenght()const;

        /*! \returns bone id
        */
        uint64 getID() const;

        /*! \returns top joint id (aka child joint)
        */
        uint64 getTopJoint() const;

        /*! \returns bottom joint id (aka parenting joint)
        */
        uint64 getBottomJoint() const;

        /*! sets custom data

        \param data pointer to custom data
        */
        void setCustomData(void* data);

        /*! \returns pointer to custom data
        */
        void* getCustomData() const;

    private:

        /*! unique bone id
        */
        uint64 _id = INVALID_BONE_ID;

        /*! holds next bone id 
        */
        static uint64 _nextID;

        /*! pointer to custom data
        */
        void* _customData = nullptr;

        /*! the top joint id
        */
        uint64 _jointTop = 0;

        /*! the bottom joint id
        */
        uint64 _jointBottom = 0;

        /*! position and oriantation to parenting bone
        */
        iaMatrixf _matrix;

        /*! lenght of bone
        */
        float64 _lenght = 0.0;

        /*! init id
        */
        iBone();

        /*! does nothing
        */
        virtual ~iBone() = default;

    };

}


#endif