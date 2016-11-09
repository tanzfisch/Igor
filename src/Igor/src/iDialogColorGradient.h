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

#ifndef __iDIALOGCOLORGRADIENT__
#define __iDIALOGCOLORGRADIENT__

#include <iWidgetDialog.h>
#include <iGradient.h>

#include <iaEvent.h>
#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iWidgetGrid;
    class iWidgetButton;
    class iUserControlColorChooser;
    class iWidgetColorGradient;

    /*! event triggered when color chooser was closed
    */
    iaEVENT(iColorGradientCloseEvent, iColorGradientCloseDelegate, void, (bool ok, const iGradientColor4f& gradient), (ok, gradient));

    /*! the color chooser dialog
    */
    class Igor_API iDialogColorGradient : public iWidgetDialog
    {

        friend class iWidgetManager;

    public:

        /*! show/open the decision box

        \param closeDelegate closing delegate
        \param gradient to start with
        \param useAlpha if true use alpha channel too
        */
        void show(iColorGradientCloseDelegate closeDelegate, const iGradientColor4f& gradient, bool useAlpha = true);

    private:

        /*! the close event
        */
        iColorGradientCloseEvent _closeEvent;

        /*! the old gradient
        */
        iGradientColor4f _oldGradient;

        /*! the current gradient
        */
        iGradientColor4f _gradient;

        /*! over all grid
        */
        iWidgetGrid* _grid = nullptr;

        /*! the widget to visualize the gradient
        */
        iWidgetColorGradient* _gradientWidget = nullptr;

        /*! all widgets
        */
        vector<iWidget*> _allWidgets;

        /*! handles ok button clicked event

        \param source the ok button it self
        */
        void onOK(iWidget* source);

        /*! handles cancel button clicked event

        \param source the cancel button it self
        */
        void onCancel(iWidget* source);

        /*! handles reset button click event

        \param source the reset button it self
        */
        void onReset(iWidget* source);

        /*! closes the dialog and sends closed event

        will be triggered by any button
        */
        void close();

        /*! deinitializes the gui elements
        */
        void deinitGUI();

        /*! initializes gui elements

        \param color the color to init with
        \param useAlpha if true alpha value will be edited too
        */
        void initGUI(const iGradientColor4f& gradient, bool useAlpha);

        /*! does nothing
        */
        iDialogColorGradient() = default;

        /*! deinitializes gui
        */
        ~iDialogColorGradient();

        /*! creates instance of this widget type
        */
        static iWidgetDialog* createInstance();

    };

}

#endif