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

#ifndef __USERCONTROL_EMITTER_H__
#define __USERCONTROL_EMITTER_H__

#include <igor/scene/nodes/iNodeModel.h>
#include <igor/ui/user_controls/iUserControl.h>
using namespace igor;

namespace igor
{
    class iWidgetGrid;
    class iWidgetTextEdit;
    class iWidgetLabel;
    class iWidgetButton;
    class iUserControlColorChooser;
    class iWidgetSlider;
    class iWidgetNumberChooser;
    class iWidgetSelectBox;
} // namespace igor

/*! user control to configure an emitter node
*/
class UserControlEmitter : public iUserControl
{
public:
    /*! ctor initializes gui
	*/
    UserControlEmitter();

    /*! nothing todo
	*/
    ~UserControlEmitter() = default;

    /*! set node id of emitter to configure
    */
    void setNode(uint32 id);

    /*! \returns node id
    */
    uint32 getNode();

private:
    /*! base grid and root widget
    */
    iWidgetGrid *_grid = nullptr;

    /*! select box for emitter type
    */
    iWidgetSelectBox *_selectType = nullptr;

    /*! text box for emitter size
    */
    iWidgetTextEdit *_textSize = nullptr;

    /*! current selected node
    */
    uint32 _nodeId = 0;

    /*! triggered when type of emitter changes

    \param source the source widget
    */
    void onTypeChanged(const iWidgetPtr source);

    /*! triggered when size of emitter changes

    \param source the source widget
    */
    void onSizeChanged(const iWidgetPtr source);

    /*! updates gui based on settings from node
    */
    void updateGUI();

    /*! updates node based on settings from gui
    */
    void updateNode();

    /*! initialize gui elements
    */
    void initGUI();
};

#endif // __USERCONTROL_EMITTER_H__