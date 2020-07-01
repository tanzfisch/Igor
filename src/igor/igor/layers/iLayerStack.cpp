#include <igor/layers/iLayerStack.h>

#include <algorithm>

namespace igor
{

    iLayerStack::~iLayerStack()
    {
        for (auto layer : _layers)
        {
            delete layer;
        }
    }

    void iLayerStack::addLayer(iLayer *layer)
    {
        _layers.push_back(layer);

        std::sort(_layers.begin(), _layers.end(), [](const iLayer *a, const iLayer *b) -> bool {
            return a->getZIndex() < b->getZIndex();
        });

        layer->onInit();
    }

    void iLayerStack::removeLayer(iLayer *layer)
    {
        auto iter = std::find(_layers.begin(), _layers.end(), layer);
        _layers.erase(iter);

        layer->onDeinit();
    }

    const std::vector<iLayer *> &iLayerStack::getStack() const
    {
        return _layers;
    }

} // namespace igor