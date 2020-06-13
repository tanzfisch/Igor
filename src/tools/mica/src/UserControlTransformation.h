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
// contact: martinloga@gmx.de  

#ifndef __USERCONTROLTRANSFORMATION__
#define __USERCONTROLTRANSFORMATION__

#include <iaux/math/iaMatrix.h>
using namespace iaux;

#include <igor/scene/nodes/iNode.h>
#include <igor/ui/user_controls/iUserControl.h>
using namespace igor;

#include <vector>

namespace igor
{
    class iWidgetGrid;
    class iWidgetTextEdit;
    class iWidgetLabel;
    class iWidgetButton;
	class iNodeTransform;
}

class UserControlTransformation : public iUserControl
{
public:


	UserControlTransformation();
	~UserControlTransformation();

    void setNode(uint32 id);
    uint32 getNode();

private:

    iWidgetGrid* _grid = nullptr;

	std::vector<iWidgetTextEdit*> _translateText;
    std::vector<iWidgetTextEdit*> _scaleText;
    std::vector<iWidgetTextEdit*> _rotateText;
	std::vector<iWidgetTextEdit*> _shearText;

	uint32 _nodeId = iNode::INVALID_NODE_ID;

	void onTransformationChanged(iNode* node);

	iWidgetTextEdit* createTextEdit();

    void onChange(const iWidgetPtr source);
   
    void updateGUI(iNodeTransform* transformNode);

    void initGUI();
    void deinitGUI();


};

#endif
