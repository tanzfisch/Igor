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

#ifndef __USERCONTROLEMITTER__
#define __USERCONTROLEMITTER__

#include <iNodeModel.h>
#include <iUserControl.h>
using namespace Igor;

namespace Igor
{
    class iWidgetGrid;
    class iWidgetTextEdit;
    class iWidgetLabel;
    class iWidgetButton;
    class iUserControlColorChooser;
    class iWidgetSlider;
    class iWidgetNumberChooser;
    class iWidgetSelectBox;
}

/*! user control to configure an emitter node
*/
class UserControlEmitter : public iUserControl
{
public:

    /*! ctor initializes gui
    */
    UserControlEmitter();

    /*! dtor deinitializes gui
    */
    ~UserControlEmitter();

    /*! \returns root widget
    */
    iWidget* getWidget();

    /*! set node id of emitter to configure
    */
    void setNode(uint32 id);

    /*! \returns node id
    */
    uint32 getNode();

private:

    /*! base grid and root widget
    */
    iWidgetGrid* _grid = nullptr;

    /*! select box for emitter type
    */
    iWidgetSelectBox* _selectType = nullptr;

    /*! text box for emitter size
    */
    iWidgetTextEdit* _textSize = nullptr;

    /*! list of all widgets for later release
    */
    vector<iWidget*> _allWidgets;

    /*! current selected node
    */
    uint32 _nodeId = 0;

    /*! triggered when type of emitter changes

    \param source the source widget
    */
    void onTypeChanged(iWidget* source);

    /*! triggered when size of emitter changes

    \param source the source widget
    */
    void onSizeChanged(iWidget* source);

    /*! updates gui based on settings from node
    */
    void updateGUI();

    /*! updates node based on settings from gui
    */
    void updateNode();

    /*! initialize gui elements
    */
    void initGUI();

    /*! release gui elements
    */
    void deinitGUI();

};

#endif