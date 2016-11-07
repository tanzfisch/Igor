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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iDIALOGCOLORCHOOSER__
#define __iDIALOGCOLORCHOOSER__

#include <iWidgetUserDialog.h>

#include <iaEvent.h>
#include <iaString.h>
#include <iaColor4.h>
using namespace IgorAux;

namespace Igor
{

    class iWidgetGrid;
    class iWidgetButton;
    class iUserControlColorChooser;

    /*! event triggered when color chooser was closed
    */
    iaEVENT(iColorChooserCloseEvent, iColorChooserCloseDelegate, void, (bool ok, const iaColor4f& color), (ok, color));

    /*! the color chooser dialog
    */
    class Igor_API iDialogColorChooser : public iWidgetUserDialog
    {
    public:

        /*! does nothing
        */
        iDialogColorChooser() = default;

        /*! deinitializes gui
        */
        ~iDialogColorChooser();

        /*! show/open the decision box

        \param message the message
        \param closeDelegate delegate to handle the close desicion box event
        \param radioButtonTexts besides the message you can add a selection of radio buttons
        \param preSelection optional preselection of radio buttons
        */
        void show(iColorChooserCloseDelegate closeDelegate, iaColor4f color, bool useAlpha = true);

    private:

        /*! the close event
        */
        iColorChooserCloseEvent _closeEvent;

        /*! selected color
        */
        iaColor4f _color;

        /*! old color
        */
        iaColor4f _oldColor;

        /*! over all grid
        */
        iWidgetGrid* _grid = nullptr;

        iUserControlColorChooser* _userControlColorChooser = nullptr;

        iWidgetButton* _okButton = nullptr;
        iWidgetButton* _cancelButton = nullptr;
        iWidgetButton* _resetButton = nullptr;
        
        vector<iWidget*> _allWidgets;

        /*! handles ok button clicked event

        \param source the ok button it self
        */
        void onOK(iWidget* source);

        /*! handles cancel button clicked event

        \param source the cancel button it self
        */
        void onCancel(iWidget* source);

        /*! closes the dialog and sends closed event

        will be triggered by any button
        */
        void close();

        /*! deinitializes the gui elements
        */
        void deinitGUI();

    };

}

#endif