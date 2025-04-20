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

#ifndef __IGOR_JOINT__
#define __IGOR_JOINT__

#include <igor/iDefines.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

#include <vector>
#include <any>

namespace igor
{

    class iBone;

    /*! joining bones togehter
    */
    class IGOR_API iJoint
    {

        friend class iSkeleton;

    public:
        /*! sets base bone of this joint

        \param bone pointer to bone
        */
        void setBaseBone(iBone *bone);

        /*! \returns the base bone
        */
        iBone *getBaseBone() const;

        /*! adds cild bone to joint
        */
        void addChildBone(iBone *bone);

        /*! removes child bone from joint
        */
        void removeChildBone(iBone *bone);

        /*! \returns reference of child bones list
        */
        std::vector<iBone *> getChildren() const;

        /*! sets custom data

        \param data pointer to custom data
        */
        void setCustomData(const std::any &data);

        /*! \returns pointer to custom data
        */
        std::any getCustomData() const;

    private:
        /*! custom data
        */
        std::any _customData;

        /*! base bone
        */
        iBone *_baseBone = nullptr;

        /*! ids of bones connected to the base bone in this joint
        */
        std::vector<iBone *> _childBones;

        /*! init id
        */
        iJoint();

        /*! does nothing
        */
        virtual ~iJoint() = default;
    };

} // namespace igor

#endif