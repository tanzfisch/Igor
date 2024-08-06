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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef __IGOR_WIDGET_FIXED_GRID_LAYOUT__
#define __IGOR_WIDGET_FIXED_GRID_LAYOUT__

#include <igor/ui/widgets/iWidget.h>

namespace igor
{

    /*! fixed grid layout widget
     */
    class IGOR_API iWidgetFixedGridLayout : public iWidget
    {
    public:
        /*! ctor initializes member variables

        \param parent optional parent
        */
        iWidgetFixedGridLayout(const iWidgetPtr parent = nullptr);

        /*! does nothing
         */
        ~iWidgetFixedGridLayout() = default;

        /*! sets the cell size

        \param cellSize the cell size
        */
        void setCellSize(const iaVector2f &cellSize);

        /*! \returns the cell size
         */
        const iaVector2f &getCellSize() const;

        /*! sets the cell spacing

        \param minCellSpacing the cell spacing
        */
        void setMinCellSpacing(float32 minCellSpacing);

        /*! \returns the cell spacing
         */
        float32 getMinCellSpacing() const;

    private:
        /*! cell size
         */
        iaVector2f _cellSize = {100.0f, 100.0f};

        /*! min cell spacing
         */
        float32 _minCellSpacing = 5;

        /*! calculates childrens offsets relative to their parent

        \param[out] offsets vector to be filled with childrens offsets
        */
        void calcChildOffsets(std::vector<iaRectanglef> &offsets) override;

        /*! updates size based on it's content
         */
        void calcMinSize() override;
    };

    /*! widget fixed grid pointer definition
     */
    typedef iWidgetFixedGridLayout *iWidgetFixedGridLayoutPtr;

} // namespace igor

#endif // __IGOR_WIDGET_FIXED_GRID_LAYOUT__
