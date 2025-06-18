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

#ifndef IGOR_ENTITY_PRINT_TRAVERSER_H
#define IGOR_ENTITY_PRINT_TRAVERSER_H

#include <igor/entities/traversal/iEntityTraverser.h>

namespace igor
{

    /*! entity to item traverser
     */
    class IGOR_API iEntityPrintTraverser : public iEntityTraverser
    {

    public:
        /*! does nothing
         */
        iEntityPrintTraverser(bool verbose = false);

        /*! does nothing
         */
        ~iEntityPrintTraverser() = default;

    private:      
        /*! keeps track of indentation
        */
        uint32 _indentation = 0;

        /*! if true output is less verbose
        */
        bool _verbose;

        /*! is called before traversal

        has to be implemented by deriving class
        */
        void preTraverse() override;

        /*! is called before every entity visited

        has to be implemented by deriving class
        */
        bool preOrderVisit(iEntityPtr entity) override;

        /*! is called after every entity visited

        has to be implemented by deriving class
        */
        void postOrderVisit(iEntityPtr entity) override;

        /*! is called after the whole traversal

        has to be implemented by deriving class
        */
        void postTraverse() override;
    };

}; // namespace igor

#endif // IGOR_ENTITY_PRINT_TRAVERSER_H