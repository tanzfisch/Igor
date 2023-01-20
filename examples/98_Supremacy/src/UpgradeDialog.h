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

#ifndef __SUPREMACY_UPGRADE_DIALOG__
#define __SUPREMACY_UPGRADE_DIALOG__

#include <igor/igor.h>
using namespace igor;

enum class UpgradeType
{
    IncreaseWalkingSpeed1,
    IncreaseWalkingSpeed2,
    IncreaseWalkingSpeed3,
    IncreaseFireFrequency1,
    IncreaseFireFrequency2,
    IncreaseFireFrequency3,
};

struct UpgradeConfiguration
{
    UpgradeType _type;
    iaString _texture;
    iaString _name;
    iaString _description;
};

class UpgradeDialog : public iDialog
{
public:
    UpgradeDialog(const iWidgetPtr parent = nullptr);
    ~UpgradeDialog() = default;
    void open(iDialogCloseDelegate dialogCloseDelegate, const UpgradeConfiguration &config1, const UpgradeConfiguration &config2, const UpgradeConfiguration &config3);
    UpgradeType getSelection() const;

private:

    UpgradeType _selection;

    UpgradeType _option1;
    UpgradeType _option2;
    UpgradeType _option3;

    void onSelect1(const iWidgetPtr source);
    void onSelect2(const iWidgetPtr source);
    void onSelect3(const iWidgetPtr source);

    void initGUI(const UpgradeConfiguration &config1, const UpgradeConfiguration &config2, const UpgradeConfiguration &config3);
};

#endif // __SUPREMACY_UPGRADE_DIALOG__