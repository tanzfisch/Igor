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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_DIALOGCOLORCHOOSER_H__
#define __IGOR_DIALOGCOLORCHOOSER_H__

#include <igor/ui/dialogs/iDialog.h>

#include <iaux/system/iaEvent.h>
#include <iaux/data/iaString.h>
#include <iaux/data/iaColor4.h>
using namespace iaux;

namespace igor
{

    class iUserControlColorChooser;
    typedef iUserControlColorChooser *iUserControlColorChooserPtr;

    /*! the color chooser dialog
	*/
    class IGOR_API iDialogColorChooser : public iDialog
    {

    public:
        /*! does nothing
		*/
        iDialogColorChooser(const iWidgetPtr parent = nullptr);

        /*! does nothing
        */
        ~iDialogColorChooser() = default;

        /*! show/open the decision box

		\param dialogCloseDelegate the closing delegate
		\param color the color to start with
		\param useAlpha if true also use the alpha channel
		*/
        void open(iDialogCloseDelegate dialogCloseDelegate, const iaColor4f &color, bool useAlpha = true);

        /*! \returns color from color chooser
        */
        const iaColor4f &getColor() const;

        /*! \returns color that was set previously
        */
        const iaColor4f &getResetColor() const;

    private:
        /*! old color
		*/
        iaColor4f _oldColor;

        /*! actual color chooser
		*/
        iUserControlColorChooserPtr _userControlColorChooser = nullptr;

        /*! all widgets
		*/
        std::vector<iWidgetPtr> _allWidgets;

        /*! handles ok button clicked event

		\param source the ok button it self
		*/
        void onOK(const iWidgetPtr source);

        /*! handles cancel button clicked event

		\param source the cancel button it self
		*/
        void onCancel(const iWidgetPtr source);

        /*! handles reset button click event

		\param source the reset button it self
		*/
        void onReset(const iWidgetPtr source);

        /*! initializes gui elements

		\param color the color to init with
		\param useAlpha if true alpha value will be edited too
		*/
        void initGUI(const iaColor4f &color, bool useAlpha);
    };

    /*! dialog color chooser pointer definition
    */
    typedef iDialogColorChooser *iDialogColorChooserPtr;

} // namespace igor

#endif // __IGOR_DIALOGCOLORCHOOSER_H__