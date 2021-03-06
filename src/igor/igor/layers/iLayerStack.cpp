#include <igor/layers/iLayerStack.h>

#include <algorithm>

namespace igor
{

    iLayerStack::~iLayerStack()
    {
        clear();
    }

    void iLayerStack::clear()
    {
        for (auto layer : _layers)
        {
            layer->onDeinit();
            delete layer;
        }
        _layers.clear();
    }

    void iLayerStack::addLayer(iLayer *layer)
    {
        con_assert(layer != nullptr, "zero pointer");
        if (layer == nullptr)
        {
            return;
        }

        _layers.push_back(layer);

        std::sort(_layers.begin(), _layers.end(), [](const iLayer *a, const iLayer *b) -> bool {
            return a->getZIndex() < b->getZIndex();
        });

        layer->onInit();
    }

    void iLayerStack::removeLayer(iLayer *layer)
    {
        con_assert(layer != nullptr, "zero pointer");
        if (layer == nullptr)
        {
            return;
        }

        auto iter = std::find(_layers.begin(), _layers.end(), layer);
        _layers.erase(iter);

        layer->onDeinit();
    }

    const std::vector<iLayer *> &iLayerStack::getStack() const
    {
        return _layers;
    }

} // namespace igor