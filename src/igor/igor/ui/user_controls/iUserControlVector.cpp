// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlVector.h>

#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/ui/widgets/iWidgetSpacer.h>

namespace igor
{

    iUserControlVector::iUserControlVector(uint32 dimensions, const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlVector, parent), _dimensions(dimensions)
    {
        initGUI();
    }

    void iUserControlVector::initGUI()
    {
        iWidgetBoxLayoutPtr layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, this);
        layout->setHorizontalAlignment(iHorizontalAlignment::Stretch);

        for (uint32 i = 0; i < _dimensions; ++i)
        {
            if (i != 0)
            {
                iWidgetSpacerPtr spacer = new iWidgetSpacer(5, 0, false, layout);
            }

            iWidgetLineTextEditPtr textField = new iWidgetLineTextEdit(layout);
            textField->setHorizontalTextAlignment(iHorizontalAlignment::Right);
            textField->setHorizontalAlignment(iHorizontalAlignment::Stretch);
            textField->setMinWidth(80);
            textField->registerOnChangeEvent(iChangeDelegate(this, &iUserControlVector::onValueChanged));
            _textValues.push_back(textField);
        }
    }

    void iUserControlVector::onValueChanged(iWidgetPtr source)
    {
        _change(this);
    }

    float64 iUserControlVector::getValue(uint32 index) const
    {
        con_assert(index < _textValues.size(), "out of bounds");

        return iaString::toFloat(_textValues[index]->getText());
    }

    void iUserControlVector::setValue(uint32 index, float32 value)
    {
        con_assert(index < _textValues.size(), "out of bounds");

        _textValues[index]->setText(iaString::toString(value, 4));
    }

} // namespace igor