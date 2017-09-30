// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iOffScreenBuffer.h>

#include <iView.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iOffScreenBuffer::iOffScreenBuffer(uint32 width, uint32 height, iRenderTargetType targetType, iColorFormat format, bool useDepthBuffer)
    {
        _width = width;
        _height = height;
        _format = format;
        _useDepthBuffer = useDepthBuffer;
        _targetType = targetType;

        if (iRenderer::getInstance().isReady())
        {
            _renderTargetID = iRenderer::getInstance().createRenderTarget(_width, _height, _format, _targetType, _useDepthBuffer);
            activateRenderTarget();
        }
    }

    iOffScreenBuffer::~iOffScreenBuffer()
    {
        if (_renderTargetID != iRenderer::DEFAULT_RENDER_TARGET)
        {
            activateRenderTarget();
            iRenderer::getInstance().destroyRenderTarget(_renderTargetID);
        }
    }

    iaVector2i iOffScreenBuffer::getTargetSize() const
    {
        return iaVector2i(_width, _height);
    }

    void iOffScreenBuffer::handle()
    {

    }

    void iOffScreenBuffer::draw()
    {
        iRenderer::getInstance().setRenderTarget(_renderTargetID);

        drawViews();

        iRenderer::getInstance().setRenderTarget(iRenderer::DEFAULT_RENDER_TARGET);
    }

};
