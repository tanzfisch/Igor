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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __iWIDGETMENUBAR__
#define __iWIDGETMENUBAR__

#include <iWidgetGrid.h>

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iAction;
    typedef iAction* iActionPtr;
    class iMenu;

    /*! menu widget
    */
	class Igor_API iMenuBar : public iWidget
	{

	public:

        iMenuBar(iWidgetPtr parent = nullptr);
		virtual ~iMenuBar();

        /*! adds action to menu

        only adds actions that are registered to the action manager
        actions are not owned by the menu

        \param action the action to be added
        */
        void addAction(const iActionPtr action);

        /*! same as add actions just by action name

        requires that the action we are searchign for was already registered to the action manager

        \param actionName name of the action to be added
        */
        void addAction(const iaString& actionName);

    private:

        /*! menu grid
        */
        iWidgetGridPtr _grid;

        /*! action hash codes
        */
        std::vector<int64> _actions;

        /*! updates size based on widgets content

        all widgets have to derive from this
        */
        void calcMinSize() override;

        /*! initializes ui elements
        */
        void init();

        /*! handles on click event. as in menu entry was clicked
        */
        void onClick(iWidgetPtr source);

        /*! handle on mouse over event
        */
        void onMouseOver(iWidgetPtr source);

	};

	/*! menu widget pointer definition
	*/
	typedef iMenuBar* iMenuBarPtr;
}

#endif
