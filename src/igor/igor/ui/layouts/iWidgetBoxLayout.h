//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin Loga
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

#ifndef IGOR_WIDGET_BOX_LAYOUT_H
#define IGOR_WIDGET_BOX_LAYOUT_H

#include <igor/ui/widgets/iWidget.h>
#include <vector>
#include <any>

namespace igor
{

    /*! box layout type
     */
    enum class iWidgetBoxLayoutType
    {
        Vertical,
        Horizontal
    };

    /*! prints the box layout type to a stream

    \param stream the stream to log to
    \param type the type to print
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iWidgetBoxLayoutType &type);

    /*! grid layout widget
     */
    class IGOR_API iWidgetBoxLayout : public iWidget
    {
    public:
        /*! ctor initializes member variables

        \param layoutType the layout type vertical or horizontal
        \param parent optional parent
        */
        iWidgetBoxLayout(iWidgetBoxLayoutType layoutType, const iWidgetPtr parent = nullptr);

        /*! does nothing
         */
        ~iWidgetBoxLayout() = default;

        /*! adds a widget to this widget at the end of the layout

        \param widget the child widget to be added
        */
        void addWidget(iWidgetPtr widget) override;

        /*! removes a child widget from this widget

        \param widget the child widget to be removed
        */
        void removeWidget(iWidgetPtr widget) override;

        /*! sets stretch index

        \param index the stretch index. <0 for no stretch
        */
        void setStretchIndex(int32 index);

        /*! \returns the stretch index
        */
        int32 getStretchIndex() const;

    private:
        /*! layout type
         */
        iWidgetBoxLayoutType _layoutType;

        /*! spacing in pixels
        */
        int32 _spacing = 1; // TODO
 
        /*! the stretch index
        */
        int32 _stretchIndex = -1;

        /*! updates size based on it's content
         */
        void calcMinSize() override;

        /*! calculates childrens offsets relative to their parent

        \param[out] offsets vector to be filled with childrens offsets
        */
        void calcChildOffsets(std::vector<iaRectanglef> &offsets) override;
    };

    /*! widget grid pointer definition
     */
    typedef iWidgetBoxLayout *iWidgetBoxLayoutPtr;

} // namespace igor

#endif // IGOR_WIDGET_BOX_LAYOUT_H