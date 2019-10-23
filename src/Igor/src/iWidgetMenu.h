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

#ifndef __iWIDGETMENU__
#define __iWIDGETMENU__

#include <iWidget.h>

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iAction; typedef iAction* iActionPtr;
    class iWidgetLabel; typedef iWidgetLabel* iWidgetLabelPtr;
    class iDialogMenu; typedef iDialogMenu* iDialogMenuPtr;
    class iWidgetMenu; typedef iWidgetMenu* iWidgetMenuPtr;
    class iWidgetSpacer; typedef iWidgetSpacer* iWidgetSpacerPtr;
    class iWidgetPicture; typedef iWidgetPicture* iWidgetPicturePtr;
    class iDialog; typedef iDialog* iDialogPtr;

    /*! menu widget
    */
	class Igor_API iWidgetMenu : public iWidget
	{

        friend class iWidgetMenuBar;

	public:

        /*! initializes gui elements
        */
        iWidgetMenu(const iWidgetPtr parent = nullptr);

        /*! clean up
        */
		virtual ~iWidgetMenu();

        /*! \returns the widgets type
        */
        virtual iWidgetType getWidgetType() const override;

        /*! sets the menu title

        \param title the menu title
        */
        void setTitle(const iaString& title);

        /*! \returns the menu's title
        */
        const iaString& getTitle() const;

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

        /*! adds a menu to the menu

        \param menu the menu to add
        */
        void addMenu(const iWidgetMenuPtr menu);

    private:

        /*! menu title
        */
        iWidgetLabelPtr _title = nullptr;

        /*! spacer
        */
        iWidgetSpacerPtr _spacer = nullptr;

        /*! picture
        */
        iWidgetPicturePtr _picture = nullptr;

        /*! menu body
        */
        iDialogMenuPtr _menu = nullptr;

        /*! ID of parent in menu hierarchy
        */
        uint64 _menuParent = IGOR_INVALID_ID;

        /*! called when the parent of this widget changed
        */
        void onParentChanged() override;

        /*! called after sub menu was closed
        */
        void onDialogClose(iDialogPtr dialog);

        /*! initialize GUI elements
        */
        void init();

        /*! updates the widget
        */
        void update();

        /*! handles when menu was clicked on
        */
        void onClick(const iWidgetPtr source);

	};

	/*! menu widget pointer definition
	*/
	typedef iWidgetMenu* iMenuPtr;
}

#endif

