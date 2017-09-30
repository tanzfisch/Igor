// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iFrameBuffer.h>

#include <iView.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iFrameBuffer::iFrameBuffer(uint32 width, uint32 height, iColorFormat format, bool useDepthBuffer)
    {
        _width = width;
        _height = height;
        _format = format;
        _useDepthBuffer = useDepthBuffer;

        if (iRenderer::getInstance().isReady())
        {
            _renderTargetID = iRenderer::getInstance().createRenderTarget(_width, _height, _format, iRenderTargetType::ToRenderBuffer, _useDepthBuffer);
        }
    }

    iFrameBuffer::~iFrameBuffer()
    {
        if (_renderTargetID != iRenderer::INVALID_ID)
        {
            iRenderer::getInstance().destroyRenderTarget(_renderTargetID);
        }
    }

    iaVector2i iFrameBuffer::getTargetSize() const
    {
        return iaVector2i(_width, _height);
    }

};
