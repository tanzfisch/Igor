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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_BONE__
#define __IGOR_BONE__

#include <igor/iDefines.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

#include <vector>
#include <any>

namespace igor
{

    class iJoint;

    /*! a bone of a skeleton

    \see iSkeleton
    */
    class IGOR_API iBone
    {

        friend class iSkeleton;
        friend class iJoint;

    public:
        /*! sets matrix of bone

        \param matrix the new matrix
        */
        void setMatrix(const iaMatrixf &matrix);

        /*! returns matrix of bone

        \param[out] matrix destination matrix to write the bone matrix in to
        */
        void getMatrix(iaMatrixf &matrix) const;

        /*! sets lenght of bone

        \param lenght the lenght of the bone
        */
        void setLenght(float64 lenght);

        /*! \returns the lenght of the bone
        */
        float64 getLenght() const;

        /*! \returns top joint id (aka child joint)
        */
        iJoint *getTopJoint() const;

        /*! \returns bottom joint id (aka parenting joint)
        */
        iJoint *getBottomJoint() const;

        /*! sets custom data

        \param data pointer to custom data
        */
        void setCustomData(const std::any &data);

        /*! \returns custom data
        */
        std::any getCustomData() const;

    private:
        /*! pointer to user data
        */
        std::any _userData;

        /*! the top joint id
        */
        iJoint *_jointTop = 0;

        /*! the bottom joint id
        */
        iJoint *_jointBottom = 0;

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

} // namespace igor

#endif