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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_WIDGET_SPLITTER__
#define __IGOR_WIDGET_SPLITTER__

#include <igor/ui/widgets/iWidget.h>
#include <igor/ui/actions/iAction.h>
#include <igor/resources/texture/iTexture.h>

#include <memory>

namespace igor
{

    enum class iSplitterOrientation
    {
        Vertical,
        Horizontal
    };

    /*! splitter widget
    */
    class IGOR_API iWidgetSplitter : public iWidget
    {

    public:
        /*! ctor initializes member variables

		\param parent the optional parent
		*/
        iWidgetSplitter(const iWidgetPtr parent = nullptr);

        /*! does nothing
		*/
        ~iWidgetSplitter() = default;

        /*! sets the split ratio

        \param ratio the new ratio to set
        */
        void setRatio(float32 ratio);

        /*! \returns ratio of this splitter
        */
        float32 getRatio() const;

        /*! sets splitter orientation

        \param orientation if Vertical the children will be left and right from the splitter and vice versa
        */
        void setOrientation(iSplitterOrientation orientation);

        /*! \returns splitter orientation
        */
        iSplitterOrientation getOrientation() const;

        /*! adds a child widget to this widget

        This will add max two widgets. The first one will be left/top and the second one will be right/bottom

        Better to use the index version of addWidget to be more explicit

        \param widget the child widget to be added
        */
        void addWidget(iWidgetPtr widget) override;

        /*! adds a child widget to this widget using index for location

        \param widget the child widget to be added
        \param index 0 == left/top; 1 == right/bottom
        */
        void addWidget(iWidgetPtr widget, uint32 index);

        /*! removes a child widget from this widget

        \param widget the child widget to be removed
        */
        void removeWidget(iWidgetPtr widget) override;

    private:
        /*! splitter ration
        */
        float32 _ratio = 0.5f;

        /*! splitter orientation
        */
        iSplitterOrientation _orientation = iSplitterOrientation::Vertical;

        /*! child a
        */
        iWidgetPtr _childA = nullptr;

        /*! child B
        */
        iWidgetPtr _childB = nullptr;

        /*! updates size based on it's content
        */
        void calcMinSize() override;

        /*! calculates childrens offsets relative to their parent

        \param[out] offsets vector to be filled with childrens offsets
        */
        void calcChildOffsets(std::vector<iaRectanglef> &offsets) override;

        /*! handles incoming mouse key down events

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseKeyDown(iKeyCode key) override;

        /*! handles mouse key up events

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseKeyUp(iKeyCode key) override;

        /*! handles incoming mouse move events

        \param pos mouse position
        */
        void handleMouseMove(const iaVector2f &pos) override;        

        /*! draws the button
        */
        void draw();
    };

    /*! widget button pointer definition
	*/
    typedef iWidgetSplitter *iWidgetSplitterPtr;
} // namespace igor

#endif // __IGOR_WIDGET_SPLITTER__
