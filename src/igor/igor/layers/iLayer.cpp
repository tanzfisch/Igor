#include <igor/layers/iLayer.h>

namespace igor
{
    iLayer::iLayer(iWindow *window, const iaString &name, int32 zIndex)
        : _name(name), _zIndex(zIndex), _window(window)
    {
    }

    const iaString &iLayer::getName() const
    {
        return _name;
    }

    int32 iLayer::getZIndex() const
    {
        return _zIndex;
    }

    iWindow *iLayer::getWindow() const
    {
        return _window;
    }

} // namespace igor