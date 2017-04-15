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
// (c) Copyright 2012-2017 by Martin Loga
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

#ifndef __CHECKBOXWIDGET__
#define __CHECKBOXWIDGET__

#include <iWidget.h>

#include <vector>
using namespace std;

namespace Igor
{

	class iTextureFont;

    /*! check box widget

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    */
	class Igor_API iWidgetCheckBox : public iWidget
	{

        /*! needs to be friend because it's the factory that creates this widget
        */
		friend class iWidgetManager;

	public:

        /*! beginns a radio button group

        every check box that is created afterwards will be added to the same radio button group
        */
		static void beginRadioButtonGroup();

        /*! ends the radio button group
        */
		static void endRadioButtonGroup();

        /*! set text of check box
        */
		void setText(const iaString& buttontext);

        /*! \retruns text of check box
        */
		const iaString& getText() const;

        /*! sets the checked flag

        \param check bolean value to set if check box is checked
        */
		void setChecked(bool check = true);

        /*! \returns true: if checked
        */
        bool isChecked();

	protected:

        /*! check box text
        */
		iaString _text;

        /*! boolean if check box is checked or not
        */
		bool _checked = false;

        /*! handles incomming mouse up event

        \parma key released mouse key 
        \returns true: if event was consumed and therefore ignored by the parent
        */
		virtual bool handleMouseKeyUp(iKeyCode key);

        /*! if check box is part of a radio button group. this is the group
        */
		vector<iWidgetCheckBox*> _radioButtons;

        /*! flag to determine if the next check box created is part of a radio button group
        */
		static bool _generatingRadioButtonGroup;

        /*! radio button stack while creating a radio button group
        */
		static vector<iWidgetCheckBox*> _currentRadioButtons;

        /*! updates size based on it's content
        */
        void calcMinSize();

		/*! draws the widget
		*/
		void draw();

	private:

        /*! ctor initializes member variables
        */
		iWidgetCheckBox();

        /*! does nothing
        */
		virtual ~iWidgetCheckBox() = default;

        /*! creates instance of this widget type
        */
        static iWidget* createInstance();
	};
}

#endif
