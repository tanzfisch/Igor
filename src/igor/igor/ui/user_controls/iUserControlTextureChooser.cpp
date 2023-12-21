// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlTextureChooser.h>

namespace igor
{

    iUserControlTextureChooser::iUserControlTextureChooser(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlTextureChooser, parent)
    {
    }

    iUserControlTextureChooser::~iUserControlTextureChooser()
    {
    }

    void iUserControlTextureChooser::setTexture(iTexturePtr texture)
    {
        _texture = texture;
    }

    iTexturePtr iUserControlTextureChooser::getTexture() const
    {
        return _texture;
    }

} // namespace igor