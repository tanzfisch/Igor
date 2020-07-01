#include <igor/layers/iLayer.h>

namespace igor
{
    iLayer::iLayer(const iaString &name, int32 zIndex)
        : _name(name), _zIndex(zIndex)
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

} // namespace igor