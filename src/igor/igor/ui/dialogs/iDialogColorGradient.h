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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IGOR_DIALOGCOLORGRADIENT_H__
#define __IGOR_DIALOGCOLORGRADIENT_H__

#include <igor/ui/dialogs/iDialog.h>

#include <iaux/data/iaGradient.h>
using namespace iaux;

namespace igor
{

    class iWidgetColorGradient;
    class iUserControlColorChooser;
    class iWidgetNumberChooser;

    /*! the color chooser dialog
	*/
    class IGOR_API iDialogColorGradient : public iDialog
    {

    public:
        /*! does nothing
		*/
        iDialogColorGradient(const iWidgetPtr parent = nullptr);

        /*! does nothing
		*/
        ~iDialogColorGradient() = default;

        /*! show/open the decision box

		\param dialogCloseDelegate closing delegate
		\param gradient to start with
		\param useAlpha if true use alpha channel too
		*/
        void open(iDialogCloseDelegate dialogCloseDelegate) override;

        /*! sets color gradient

		\param gradient the gradient to set
		*/
        void setColorGradient(const iaGradientColor4f &gradient);

        /*! sets weather or not alpha channel is used

		\param useAlpha if true alpha channel is used
		*/
        void setUseAlpha(bool useAlpha);

        /*! \retruns true if alpha channel is used
		*/
        bool isUsingAlpha() const;

        /*! \returns color gradient from dialog
        */
        const iaGradientColor4f &getColorGradient() const;

        /*! \returns previsou color gradient
        */
        const iaGradientColor4f &getResetColorGradient() const;

    private:
        /*! the old gradient
		*/
        iaGradientColor4f _oldGradient;

        /*! the current gradient
		*/
        iaGradientColor4f _gradient;

        /*! current selected color index
		*/
        int32 _selectedColor = 0;

        /*! color chooser
		*/
        iUserControlColorChooser *_colorChooser = nullptr;

        /*! position number chooser
		*/
        iWidgetNumberChooser *_position = nullptr;

        /*! the widget to visualize the gradient
		*/
        iWidgetColorGradient *_gradientWidget = nullptr;

        /*! flag if alpha will be available for edit or not 
        */
        bool _useAlpha = true;

        /*! updates gui part if selection changed
		*/
        void updateSelection();

        /*! triggered by selection changed event
		*/
        void onSelectionChanged(int32 index);

        /*! triggred when position number chooser was changed

		\param source source widget
		*/
        void onPositionChanged(const iWidgetPtr source);

        /*! triggered when color chooser was manipulated
		*/
        void onColorChanged(const iaColor4f &color);

        /*! triggered when color in gradient was created

		\param at position of color
		\param color the value of the color
		*/
        void onColorCreated(float32 at, const iaColor4f &color);

        /*! triggered by clicking delete button
		*/
        void onDelete(const iWidgetPtr source);

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
		*/
        void initUI();

        /*! update UI with current settings
        */
        void update();
    };

    /*! dialog color gradient pointer definition
    */
    typedef iDialogColorGradient *iDialogColorGradientPtr;

} // namespace igor

#endif // __IGOR_DIALOGCOLORGRADIENT_H__