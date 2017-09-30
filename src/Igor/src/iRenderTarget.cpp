// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iRenderTarget.h>

#include <iView.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iRenderTarget::iRenderTarget()
    {

    }    

    iRenderTarget::~iRenderTarget()
    {
        if (!_views.empty())
        {
            con_err("possible mem leak not all views are detached");
        }
    }

    void iRenderTarget::drawViews()
    {
        for (auto view : _views)
        {
            view->draw();
        }
    }

    void iRenderTarget::updateViews()
    {
        for (auto view : _views)
        {
            view->updateParentSize(getTargetSize());
        }
    }

    void iRenderTarget::addView(iView* view)
    {
        con_assert_sticky(view != nullptr, "zero pointer");

        if (view != nullptr)
        {
            _views.push_back(view);
            view->updateParentSize(getTargetSize());
        }
    }

    void iRenderTarget::removeView(iView* view)
    {
        con_assert_sticky(view != nullptr, "zero pointer");

        auto iter = find(_views.begin(), _views.end(), view);
        if (iter != _views.end())
        {
            _views.erase(iter);
        }
        else
        {
            con_err("view not found");
        }
    }

};
