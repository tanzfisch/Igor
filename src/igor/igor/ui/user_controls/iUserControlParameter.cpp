// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlParameter.h>

#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/ui/widgets/iWidgetSpacer.h>

namespace igor
{

    iUserControlParameter::iUserControlParameter(const iAnyType &type, const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlParameter, parent)
    {
        onInitUI(type);
    }

    void iUserControlParameter::onInitUI(const iAnyType &type)
    {
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