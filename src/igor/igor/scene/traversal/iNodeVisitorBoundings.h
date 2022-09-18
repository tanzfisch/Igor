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
// (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __IGOR_NODEVISITORBOUNDINGS_H__
#define __IGOR_NODEVISITORBOUNDINGS_H__

#include <igor/scene/traversal/iNodeVisitor.h>
#include <igor/iDefines.h>

#include <iaux/data/iaSphere.h>

namespace igor
{

    /*! calculates the combined boundings of a subtree of nodes
    */
    class IGOR_API iNodeVisitorBoundings : public iNodeVisitor
    {

    public:
        /*! \returns merged bounding sphere of all children while traversal
        */
        void getSphere(iSphered &sphere);

        /*! initializes node visitor
        */
        iNodeVisitorBoundings();

        /*! does nothing
        */
        virtual ~iNodeVisitorBoundings() = default;

    protected:
        /*! does nothing
        */
        void preTraverse() override;

        /*! merges bounding spheres from all children while traversal

        \param node current node
        */
        virtual bool preOrderVisit(iNodePtr node, iNodePtr nextSibling) override;

        /*! does nothing

        \param node current node
        */
        virtual void postOrderVisit(iNodePtr node) override;

        /*! does nothing
        */
        virtual void postTraverse() override;

    private:
        /*! flag to mark the first bounding sphere found
        */
        bool _initialSphere = true;

        /*! temp and end result of bounding sphere
        */
        iSphered _sphere;
    };

}; // namespace igor

#endif // __IGOR_NODEVISITORBOUNDINGS_H__
