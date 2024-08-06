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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef __IGOR_SKELETON__
#define __IGOR_SKELETON__

#include <igor/data/iJoint.h>
#include <igor/data/iBone.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

#include <vector>

namespace igor
{

    class iBone;
    class iJoint;

    /*! skeleton that contains joints and bones
    */
    class IGOR_API iSkeleton
    {

    public:
        /*! does nothing
        */
        iSkeleton() = default;

        /*! does nothing
        */
        virtual ~iSkeleton() = default;

        /*! \returns the ID of the root joint
        */
        iJoint *getRootJoint() const;

        /*! adds a bone at current position

        \param matrix the orientation and offset of the bone
        \param length the length of the bone
        */
        void addBone(const iaMatrixf &matrix, float64 length);

        /*! pushes current position in skeleton
        */
        void push();

        /*! pops back to last pushed position in skeleton
        */
        void pop();

        /*! clears all data
        */
        void clear();

        /*! \returns last created bone
        */
        iBone *getLastBone() const;

    private:
        /*! map of all bones
        */
        std::vector<iBone *> _bones;

        /*! map of all joints
        */
        std::vector<iJoint *> _joints;

        /*! internal bone stack. handles push and pop
        */
        std::vector<iBone *> _boneStack;

        /*! root joint id
        */
        iJoint *_rootJoint = nullptr;

        /*! current joint
        */
        iJoint *_currentJoint = nullptr;

        /*! last created bone
        */
        iBone *_lastBone = nullptr;

        /*! creates a bone and returns the bone's id

        \returns pointer to bone
        */
        iBone *createBone();

        /*! destroyes specified bone

        \param bone pointer to bone
        */
        void destroyBone(iBone *bone);

        /*! creates a a joint and returns the pointer

        \returns pointer to joint
        */
        iJoint *createJoint();

        /*! destroyes specified joint

        \param joint pointer to the joint
        */
        void destroyJoint(iJoint *joint);
    };

} // namespace igor

#endif