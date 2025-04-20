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
// (c) Copyright 2012-2025 by Martin A. Loga
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

struct UpgradeConfiguration
{
    iaString _name;
    iaString _description;
    iaString _icon;

    float64 _damageFactor;
    float64 _attackIntervalFactor;
    float64 _criticalHitChanceFactor;
    float64 _criticalHitDamageFactor;
    float64 _splashDamageRangeFactor;
    float64 _walkSpeedFactor;
    float64 _projectileSpeedFactor;
    float64 _projectileRangeFactor;
};

class UpgradeDialog : public iDialog
{
public:
    UpgradeDialog(const iWidgetPtr parent = nullptr);
    ~UpgradeDialog() = default;
    void open(iDialogCloseDelegate dialogCloseDelegate, const std::vector<UpgradeConfiguration> &upgrades);
    const UpgradeConfiguration& getSelection() const;

private:

    std::vector<UpgradeConfiguration> _upgrades;

    uint32 _selection;
    uint32 _option1;
    uint32 _option2;
    uint32 _option3;

	iWidgetLabel *_labelName1;
	iWidgetLabel *_labelName2;
	iWidgetLabel *_labelName3;
	iWidgetLabel *_labelDescription1;
	iWidgetLabel *_labelDescription2;
	iWidgetLabel *_labelDescription3;
    iWidgetButton *_button1;
    iWidgetButton *_button2;
    iWidgetButton *_button3;

    void onSelect1(const iWidgetPtr source);
    void onSelect2(const iWidgetPtr source);
    void onSelect3(const iWidgetPtr source);

    void initGUI();
    void updateGUI();
};

#endif // __SUPREMACY_UPGRADE_DIALOG__