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

#ifndef __iDIALOGDECISIONBOX__
#define __iDIALOGDECISIONBOX__

#include <iWidgetDialog.h>

#include <iaEvent.h>
#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iWidgetButton;
    class iWidgetGrid;
    class iWidgetLabel;
    class iWidget;
    class iWidgetSpacer;
    class iWidgetCheckBox;

    /*! event triggered when decision box was closed
    */
    iaEVENT(iDecisionBoxCloseEvent, iDecisionBoxCloseDelegate, void, (bool ok, int32 selection), (ok, selection));

    /*! the decision box dialog
    */
    class Igor_API iDialogDecisionBox : public iWidgetDialog
    {

        friend class iWidgetManager;

    public:

        /*! show/open the decision box

        \param message the message
        \param closeDelegate delegate to handle the close desicion box event
        \param radioButtonTexts besides the message you can add a selection of radio buttons
        \param preSelection optional preselection of radio buttons
        */
        void show(const iaString& message, iDecisionBoxCloseDelegate closeDelegate, std::initializer_list<iaString> radioButtonTexts, int32 preSelection = -1);

    private:

        /*! the close event
        */
        iDecisionBoxCloseEvent _decisionBoxCloseEvent;

        /*! the selesction made by the user
        */
        int32 _selection = -1;

        /*! the ok button
        */
        iWidgetButton* _okButton = nullptr;

        /*! the cancel button
        */
        iWidgetButton* _cancelButton = nullptr;

        /*! a spacer line
        */
        iWidgetSpacer* _spacerLineTop = nullptr;

        /*! a spacer line
        */
        iWidgetSpacer* _spacerLineBottom = nullptr;

        /*! an other spacer
        */
        iWidgetSpacer* _spacerLittle = nullptr;

        /*! over all grid
        */
        iWidgetGrid* _grid = nullptr;
        
        /*! grid for the buttons
        */
        iWidgetGrid* _buttonGrid = nullptr;

        /*! grid for radio buttons
        */
        iWidgetGrid* _radioGrid = nullptr;
        
        /*! label for the message text
        */
        iWidgetLabel* _messageLabel = nullptr;

        vector<iWidgetCheckBox*> _radioButtons;

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

        /*! does nothing
        */
        iDialogDecisionBox() = default;

        /*! deinitializes gui
        */
        ~iDialogDecisionBox();

        /*! creates instance of this widget type
        */
        static iWidgetDialog* createInstance();

    };

}

#endif