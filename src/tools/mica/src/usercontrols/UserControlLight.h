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

#ifndef __USERCONTROLLIGHT__
#define __USERCONTROLLIGHT__

#include <igor/igor.h>
using namespace iaux;
using namespace igor;

class UserControlLight : public iUserControl
{
public:
    UserControlLight();
    ~UserControlLight() = default;

    void setNode(uint32 id);
    uint32 getNode();

private:
    iWidgetGridLayout *_grid = nullptr;

    iUserControlColorChooser *_ambientColorChooser = nullptr;
    iUserControlColorChooser *_specularColorChooser = nullptr;
    iUserControlColorChooser *_diffuseColorChooser = nullptr;

    iaColor3f _ambient;
    iaColor3f _diffuse;
    iaColor3f _specular;

    uint32 _lightNodeId = 0;

    void onAmbientChange(const iaColor4f &color);
    void onDiffuseChange(const iaColor4f &color);
    void onSpecularChange(const iaColor4f &color);

    void updateGUI();
    void updateNode();

    void initGUI();
};

#endif // __USERCONTROLLIGHT__