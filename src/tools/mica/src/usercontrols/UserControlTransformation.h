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

#ifndef __USERCONTROLTRANSFORMATION__
#define __USERCONTROLTRANSFORMATION__

#include <igor/igor.h>
using namespace igor;
using namespace iaux;

#include <vector>

class UserControlTransformation : public iUserControl
{
public:
    UserControlTransformation();
    ~UserControlTransformation();

    void setNode(uint32 id);
    uint32 getNode();

private:
    iWidgetGrid *_grid = nullptr;

    std::vector<iWidgetLineTextEdit *> _matrixText;

    uint32 _nodeId = iNode::INVALID_NODE_ID;

    void onTransformationChanged(iNode *node);

    iWidgetLineTextEdit *createTextEdit();

    void onChange(const iWidgetPtr source);

    void updateGUI(iNodeTransform *transformNode);

    void initGUI();
    void deinitGUI();
};

#endif // __USERCONTROLTRANSFORMATION__