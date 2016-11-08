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
// (c) Copyright 2014-2016 by Martin Loga
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

    /*! color has changed event
    */
    iaEVENT(iColorChanged, iColorChangedDelegate, void, (const iaColor4f& color), (color));

    /*! color chooser mode
    */
    enum class iColorChooserMode
    {
        RGB,
        RGBA
    };

    /*! color chooser user control

    \bug color chooser disapperas when unfolded and folded again
    */
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

        /*! returns color
        */
        const iaColor4f& getColor() const;

        /*! set color

        \param color color
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

        /*! sets the expand mode

        \param expand if true expand mode is on
        */
        void setExpand(bool expand = true);

        /*! \returns expand mode state
        */
        bool getExpand() const;

        void setExpandButtonVisible(bool showExpand = true);

        bool isExpandButtonVisible() const;

    private:

        /*! current color on RGBA format
        */
        iaColor4f _colorRGBA;

        /*! current color in HSV format
        */
        iaColor4f _colorHSV;
        
        /*! if true color chooser is displayed in expanded mode
        */
        bool _expanded = false;

        /*! if true expand button will be shown
        */
        bool _showExpand = true;

        /*! amount of components based on the mode the color chooser was started with
        */
        int32 _components = 0;

        /*! headline text
        */
        iaString _text;

        /*! color changed event
        */
        iColorChanged _colorChanged;

        /*! collect all widgets created
        */
        vector<iWidget*> _allWidgets;

        /*! root widget
        */
        iWidgetGrid* _grid = nullptr;

        /*! head line grid
        */
        iWidgetGrid* _headlineGrid = nullptr;

        /*! label that holds the title
        */
        iWidgetLabel* _titleLabel = nullptr;

        /*! button to expand the color chooser
        */
        iWidgetButton* _expandButton = nullptr;

        /*! some spacer
        */
        iWidgetSpacer* _collapsedSpacer = nullptr;

        /*! collapsed grid
        */
        iWidgetGrid* _collapsedGrid = nullptr;

        /*! expanded grid
        */
        iWidgetGrid* _expandedGrid = nullptr;

        /*! expanded slider grid
        */
        iWidgetGrid* _expandedSliderGrid = nullptr;

        /*! number chooser for hue component
        */
        iWidgetNumberChooser* _valueChooserH = nullptr;

        /*! number chooser for saturation component
        */
        iWidgetNumberChooser* _valueChooserS = nullptr;

        /*! number chooser for value component
        */
        iWidgetNumberChooser* _valueChooserV = nullptr;

        /*! number chooser for red component
        */
        iWidgetNumberChooser* _valueChooserR = nullptr;

        /*! number chooser for green component
        */
        iWidgetNumberChooser* _valueChooserG = nullptr;

        /*! number chooser for blue component
        */
        iWidgetNumberChooser* _valueChooserB = nullptr;

        /*! number chooser for alpha component
        */
        iWidgetNumberChooser* _valueChooserA = nullptr;

        /*! number chooser for red component used in expanded mode
        */
        iWidgetNumberChooser* _valueChooserRExpanded = nullptr;
        
        /*! number chooser for green component used in expanded mode
        */
        iWidgetNumberChooser* _valueChooserGExpanded = nullptr;

        /*! number chooser for blue component used in expanded mode
        */
        iWidgetNumberChooser* _valueChooserBExpanded = nullptr;

        /*! number chooser for alpha component used in expanded mode
        */
        iWidgetNumberChooser* _valueChooserAExpanded = nullptr;

        /*! label for hue component
        */
        iWidgetLabel* _labelH = nullptr;

        /*! label for saturation component
        */
        iWidgetLabel* _labelS = nullptr;

        /*! label for value component
        */
        iWidgetLabel* _labelV = nullptr;

        /*! label for red component
        */
        iWidgetLabel* _labelR = nullptr;
        
        /*! label for green component
        */
        iWidgetLabel* _labelG = nullptr;
        
        /*! label for blue component
        */
        iWidgetLabel* _labelB = nullptr;
        
        /*! label for alpha component
        */
        iWidgetLabel* _labelA = nullptr;

        /*! label for red component
        */
        iWidgetLabel* _labelRExpanded = nullptr;

        /*! label for green component
        */
        iWidgetLabel* _labelGExpanded = nullptr;

        /*! label for blue component
        */
        iWidgetLabel* _labelBExpanded = nullptr;

        /*! label for alpha component
        */
        iWidgetLabel* _labelAExpanded = nullptr;

        /*! slider for hue component
        */
        iWidgetSlider* _sliderH = nullptr;
        
        /*! slider for saturation component
        */
        iWidgetSlider* _sliderS = nullptr;
        
        /*! slider for value component
        */
        iWidgetSlider* _sliderV = nullptr;
        
        /*! slider for red component
        */
        iWidgetSlider* _sliderR = nullptr;
        
        /*! slider for green component
        */
        iWidgetSlider* _sliderG = nullptr;
        
        /*! slider for blue component
        */
        iWidgetSlider* _sliderB = nullptr;

        /*! slider for alpha component
        */
        iWidgetSlider* _sliderA = nullptr;

        /*! color viewer used in expanded mode
        */
        iWidgetColorView* _colorViewExpanded = nullptr;

        /*! color viewer used in collapsed mode
        */
        iWidgetColorView* _colorViewCollapsed = nullptr;

        /*! triggered when hue component changed

        \param source source widget
        */
        void onValueChangedH(iWidget* source);

        /*! triggered when saturation component changed

        \param source source widget
        */
        void onValueChangedS(iWidget* source);
        
        /*! triggered when value component changed

        \param source source widget
        */
        void onValueChangedV(iWidget* source);
        
        /*! triggered when red component changed

        \param source source widget
        */
        void onValueChangedR(iWidget* source);
        
        /*! triggered when green component changed

        \param source source widget
        */
        void onValueChangedG(iWidget* source);
        
        /*! triggered when blue component changed

        \param source source widget
        */
        void onValueChangedB(iWidget* source);
        
        /*! triggered when alpha component changed

        \param source source widget
        */
        void onValueChangedA(iWidget* source);

        /*! triggered when red component changed from expanded mode widget

        \param source source widget
        */
        void onValueChangedRExpanded(iWidget* source);

        /*! triggered when green component changed from expanded mode widget

        \param source source widget
        */
        void onValueChangedGExpanded(iWidget* source);

        /*! triggered when blue component changed from expanded mode widget

        \param source source widget
        */
        void onValueChangedBExpanded(iWidget* source);

        /*! triggered when alpha component changed from expanded mode widget

        \param source source widget
        */
        void onValueChangedAExpanded(iWidget* source);

        /*! triggered when hue component changed by slider

        \param source source widget
        */
        void onSliderChangedH(iWidget* source);

        /*! triggered when saturation component changed by slider

        \param source source widget
        */
        void onSliderChangedS(iWidget* source);
        
        /*! triggered when value component changed by slider

        \param source source widget
        */
        void onSliderChangedV(iWidget* source);
        
        /*! triggered when red component changed by slider

        \param source source widget
        */
        void onSliderChangedR(iWidget* source);
        
        /*! triggered when green component changed by slider

        \param source source widget
        */
        void onSliderChangedG(iWidget* source);
        
        /*! triggered when blue component changed by slider

        \param source source widget
        */
        void onSliderChangedB(iWidget* source);
        
        /*! triggered when alpha component changed by slider

        \param source source widget
        */
        void onSliderChangedA(iWidget* source);

        /*! updates color view widgets based on current color
        */
        void updateColorViews();

        /*! updates all the sliders and number choosers based on current color
        */
        void updateWidgets();

        /*! updates the HSV colors based on the RGB colors
        */
        void updateColorHSV();

        /*! updates the RGB colors based on the HSV colors
        */
        void updateColorRGB();

        /*! creates a label widget

        \param text the text of the label
        */
        iWidgetLabel* createLabel(const iaString& text);

        /*! creates a slider widget

        \param textureFileName file name of background texture
        \param changeDelegate delegate to register to value change event
        */
        iWidgetSlider* createSlider(iaString textureFileName, iChangeDelegate changeDelegate);

        /*! creates a number chooser

        \param changeDelegate delegate to register to value change event
        */
        iWidgetNumberChooser* createNumberChooser(iChangeDelegate changeDelegate);

        /*! switches between collapsed or expanded mode
        */
        void setExpanded(bool expanded);

        /*! initialize expanded part of user control
        */
        void initExpanded();

        /*! initialize collapsed part of user control
        */
        void initCollapsed();

        /*! called when expand button was pressed

        \param source the source widget
        */
        void onExpandButtonPressed(iWidget* source);

        /*! initializes the gui
        */
        void initGUI();

        /*! releases resources
        */
        void deinitGUI();

    };
}
#endif