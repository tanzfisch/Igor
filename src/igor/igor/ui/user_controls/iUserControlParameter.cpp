// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlParameter.h>

#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetCheckBox.h>
#include <igor/ui/user_controls/iUserControlVector.h>

namespace igor
{

    iUserControlParameter::iUserControlParameter(const iAnyType &type, const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlParameter, parent)
    {
        onInitUI(type);
    }

    void iUserControlParameter::onInitUI(const iAnyType &type)
    {
        auto layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, this);

        auto nameTextEdit = new iWidgetLineTextEdit(layout);
        nameTextEdit->setMinWidth(100);
        nameTextEdit->getChangeEvent().add(iChangeDelegate(this, &iUserControlParameter::onKeyChanged));

        auto typeLabel = new iWidgetLabel(layout);
        typeLabel->setText(toString(type));
        typeLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
        typeLabel->setMinWidth(100);

        iWidgetPtr dataWidget = nullptr;

        switch (type)
        {
        case iAnyType::Bool:
        {
            auto widget = new iWidgetCheckBox(layout);
            dataWidget = widget;
            break;
        }

        case iAnyType::int64:
        {
            auto widget = new iWidgetLineTextEdit(layout);
            dataWidget = widget;
            break;
        }

        case iAnyType::float64:
        {
            auto widget = new iWidgetLineTextEdit(layout);
            dataWidget = widget;
            break;
        }

        case iAnyType::iaString:
        {
            auto widget = new iWidgetLineTextEdit(layout);
            dataWidget = widget;
            break;
        }

        case iAnyType::iaVector2d:
        {
            auto widget = new iUserControlVector(2, layout);
            dataWidget = widget;
            break;
        }

        case iAnyType::iaVector3d:
        {
            auto widget = new iUserControlVector(3, layout);
            dataWidget = widget;
            break;
        }

        case iAnyType::iaVector4d:
        {
            auto widget = new iUserControlVector(4, layout);
            dataWidget = widget;
            break;
        }
        };

        if (dataWidget != nullptr)
        {
            dataWidget->setUserData(pair.second.getType());
            dataWidget->getChangeEvent().add(iChangeDelegate(this, &iUserControlParameter::onValueChanged));
        }
    }

    void iUserControlParameter::onKeyChanged(iWidgetPtr source)
    {
        _change(this);
    }

    void iUserControlParameter::onValueChanged(iWidgetPtr source)
    {
        _change(this);
    }

    const iAny &iUserControlParameter::getValue() const
    {
    }

    void iUserControlParameter::setValue(const iAny &value)
    {
    }

    const iaString &iUserControlParameter::getName() const
    {
    }

    void iUserControlParameter::setName(const iaString &name)
    {
    }

} // namespace igor