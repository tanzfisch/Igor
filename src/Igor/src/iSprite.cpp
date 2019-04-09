// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iSprite.h>

#include <iTextureResourceFactory.h>

namespace Igor
{

    iSprite::iSprite(shared_ptr<iTexture> texture)
    {
        _texture = texture;

        _texCoord[0] = iaVector2f(0.0f, 0.0f);
        _texCoord[1] = iaVector2f(0.0f, 1.0f);
        _texCoord[2] = iaVector2f(1.0f, 1.0f);
        _texCoord[3] = iaVector2f(1.0f, 0.0f);

        _width = static_cast<float32>(_texture->getWidth());
        _height = static_cast<float32>(_texture->getHeight());
    }

    void iSprite::setTexcoord(const iaVector2f& tex0, const iaVector2f& tex1, const iaVector2f& tex2, const iaVector2f& tex3)
    {
        _texCoord[0] = tex0;
        _texCoord[1] = tex1;
        _texCoord[2] = tex2;
        _texCoord[3] = tex3;
    }

    void iSprite::setOrigin(const iaVector2f& pos)
    {
        _origin = pos;
    }

};
