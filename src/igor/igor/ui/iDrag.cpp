// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/iDrag.h>

#include <igor/ui/iWidgetManager.h>

namespace igor
{

    iDrag::iDrag(iWidgetPtr source)
    {
        if (source == nullptr)
        {
            return;
        }

        _source = source->getID();
    }

    iWidgetID iDrag::getSource() const
    {
        return _source;
    }

    void iDrag::setMimeData(const iMimeData &mimeData)
    {
        _mimeData = mimeData;
    }

    const iMimeData &iDrag::getMimeData() const
    {
        return _mimeData;
    }

    void iDrag::execute()
    {
        iWidgetManager::getInstance().beginDrag(*this);
    }

    void iDrag::setTexture(iTexturePtr texture)
    {
        _texture = texture;
    }

    iTexturePtr iDrag::getTexture() const
    {
        return _texture;
    }

    iDragState iDrag::getDragState() const
    {
        return _dragState;
    }

    void iDrag::accept()
    {
        _dragState = iDragState::Accepted;
    }

    void iDrag::reject()
    {
        _dragState = iDragState::Rejected;
    }

    void iDrag::clear()
    {
        _dragState = iDragState::Neutral;
    }
}
