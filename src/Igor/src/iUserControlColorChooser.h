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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iUSERCONTROLCOLORCHOOSER__
#define __iUSERCONTROLCOLORCHOOSER__

#include <iWidgetUserControl.h>
#include <iWidget.h>

#include <iaEvent.h>
#include <iaColor4.h>
#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iWidgetGrid;
    class iWidgetLabel;
    class iWidgetTextEdit;
    class iWidgetButton;
    class iWidgetSpacer;
    class iWidgetColorView;
    class iWidgetSlider;
    class iWidgetNumberChooser;

    iaEVENT(iColorChanged, iColorChangedDelegate, void, (const iaColor4f& color), (color));

    enum class iColorChooserMode
    {
        RGB,
        RGBA
    };

    class Igor_API iUserControlColorChooser : public iWidgetUserControl
    {
    public:

        /*! ctor initializes member variables

        \param mode the mode this color chooser is configured for
        */
        iUserControlColorChooser(iColorChooserMode mode);

        /*! clean up
        */
        ~iUserControlColorChooser();

        /*! sets the headline text

        \param text the text
        */
        void setText(const iaString& text);

        /*! \returns the headline text
        */
        const iaString& getText() const;

        /*! returns ambient material color
        */
        const iaColor4f& getColor() const;

        /*! set ambient material color

        \param color ambient color
        */
        void setColor(const iaColor4f& color);

        /*! \returns root widget of user control
        */
        iWidget* getWidget();

        /*! register on color change event

        \param colorChangedDelegate the delegate to register
        */
        void registerOnColorChangedEvent(iColorChangedDelegate colorChangedDelegate);

        /*! unregister from color change event

        \param colorChangedDelegate the delegate to unregister
        */
        void unregisterOnColorChangedEvent(iColorChangedDelegate colorChangedDelegate);

    private:

        iaColor4f _colorRGBA;
        iaColor4f _colorHSV;
        
        bool _expanded = false;

        iColorChooserMode _mode = iColorChooserMode::RGBA;
        int32 _components = 0;

        /*! headline text
        */
        iaString _text;

        /*! root widget
        */
        iWidgetGrid* _grid = nullptr;

        /*! color changed event
        */
        iColorChanged _colorChanged;

        iWidgetGrid* _headlineGrid = nullptr;
        iWidgetLabel* _label = nullptr;
        iWidgetButton* _expandButton = nullptr;

        iWidgetSpacer* _collapsedSpacer = nullptr;

        iWidgetGrid* _collapsedGrid = nullptr;
        iWidgetGrid* _expandedGrid = nullptr;
        iWidgetGrid* _expandedSliderGrid = nullptr;

        iWidgetNumberChooser* _valueChooserH = nullptr;
        iWidgetNumberChooser* _valueChooserS = nullptr;
        iWidgetNumberChooser* _valueChooserV = nullptr;
        iWidgetNumberChooser* _valueChooserR = nullptr;
        iWidgetNumberChooser* _valueChooserG = nullptr;
        iWidgetNumberChooser* _valueChooserB = nullptr;
        iWidgetNumberChooser* _valueChooserA = nullptr;

        iWidgetLabel* _labelH = nullptr;
        iWidgetLabel* _labelS = nullptr;
        iWidgetLabel* _labelV = nullptr;
        iWidgetLabel* _labelR = nullptr;
        iWidgetLabel* _labelG = nullptr;
        iWidgetLabel* _labelB = nullptr;
        iWidgetLabel* _labelA = nullptr;

        iWidgetSlider* _sliderH = nullptr;
        iWidgetSlider* _sliderS = nullptr;
        iWidgetSlider* _sliderV = nullptr;
        iWidgetSlider* _sliderR = nullptr;
        iWidgetSlider* _sliderG = nullptr;
        iWidgetSlider* _sliderB = nullptr;
        iWidgetSlider* _sliderA = nullptr;

        iWidgetColorView* _colorViewExpanded = nullptr;
        iWidgetColorView* _colorViewCollapsed = nullptr;

        void onTextChangedH(iWidget* source);
        void onTextChangedS(iWidget* source);
        void onTextChangedV(iWidget* source);
        void onTextChangedR(iWidget* source);
        void onTextChangedG(iWidget* source);
        void onTextChangedB(iWidget* source);
        void onTextChangedA(iWidget* source);

        void onSliderChangedH(iWidget* source);
        void onSliderChangedS(iWidget* source);
        void onSliderChangedV(iWidget* source);
        void onSliderChangedR(iWidget* source);
        void onSliderChangedG(iWidget* source);
        void onSliderChangedB(iWidget* source);
        void onSliderChangedA(iWidget* source);

        void updateColorViews();
        void updateWidgets();
        void updateColorRGB();
        void updateColorHSV();

        iWidgetLabel* createLabel(const iaString& text);
        iWidgetSlider* createSlider(iaString textureFileName, iChangeDelegate changeDelegate);
        iWidgetNumberChooser* createTextBox(iChangeDelegate changeDelegate);

        void setExpanded(bool expanded);

        void initExpanded();
        void initCollapsed();

        void onExpandButtonPressed(iWidget* source);
        void onTextEditChange(iWidget* source);

        void initGUI();
        void deinitGUI();

    };
}
#endif