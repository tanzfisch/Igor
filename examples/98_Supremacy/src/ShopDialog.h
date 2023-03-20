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

#ifndef __SUPREMACY_SHOP_DIALOG__
#define __SUPREMACY_SHOP_DIALOG__

#include <igor/igor.h>
using namespace igor;

enum class ShopItemType
{
    None,
    Weapon
};

struct ShopItem
{
    iaString _name;
    iaString _description;
    iaString _icon;    
    ShopItemType _type;
    uint32 _price;
};

class ShopDialog : public iDialog
{
public:
    ShopDialog(const iWidgetPtr parent = nullptr);
    ~ShopDialog() = default;
    void open(iDialogCloseDelegate dialogCloseDelegate, int coins, const std::vector<ShopItem> &shopItems);
    const ShopItem& getSelection() const;
    bool bought() const;

private:

    std::vector<ShopItem> _shopItems;

    bool _buy;
    uint32 _selection;
    uint32 _option1;
    uint32 _option2;
    uint32 _option3;

    iWidgetLabel *_labelCoins;
	iWidgetLabel *_labelName1;
	iWidgetLabel *_labelName2;
	iWidgetLabel *_labelName3;
	iWidgetLabel *_labelPrice1;
	iWidgetLabel *_labelPrice2;
	iWidgetLabel *_labelPrice3;
	iWidgetLabel *_labelDescription1;
	iWidgetLabel *_labelDescription2;
	iWidgetLabel *_labelDescription3;
    iWidgetPicture *_picture1;
    iWidgetPicture *_picture2;
    iWidgetPicture *_picture3;
    iWidgetGrid *_itemGrid;

    void onSelectionChanged(const iWidgetPtr source);

    void onBuy(const iWidgetPtr source);
    void onCancel(const iWidgetPtr source);

    void initGUI();
    void updateGUI(int coins);
};

#endif // __SUPREMACY_SHOP_DIALOG__