// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlMeshReference.h>

#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/resources/iResourceManager.h>
#include <igor/resources/texture/iThumbnailCache.h>
#include <igor/data/iMimeData.h>
#include <igor/ui/iDrag.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

namespace igor
{

    iUserControlMeshReference::iUserControlMeshReference(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlMeshReference, parent)
    {
        setAcceptDrop(true);

        initGUI();
    }

    iUserControlMeshReference::~iUserControlMeshReference()
    {
    }

    void iUserControlMeshReference::initGUI()
    {
        iWidgetBoxLayoutPtr layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, this);
        // TODO thumbnail picture would be great

        iWidgetBoxLayoutPtr labelLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, layout);
        _labelID = new iWidgetLabel(labelLayout);
        _labelID->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelID->setVerticalAlignment(iVerticalAlignment::Top);

        _labelAlias = new iWidgetLabel(labelLayout);
        _labelAlias->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelAlias->setVerticalAlignment(iVerticalAlignment::Top);

        _meshPath = new iWidgetLabel(labelLayout);
        _meshPath->setHorizontalAlignment(iHorizontalAlignment::Left);
        _meshPath->setVerticalAlignment(iVerticalAlignment::Top);
    }

    void iUserControlMeshReference::setReference(const iResourceID &modelID, const std::vector<iaString> &meshPaths)
    {
        if (!modelID.isValid())
        {
            con_err("tried to set invalid mesh reference");
            return;
        }

        _modelID = modelID;

        _labelID->setText(_modelID.toString());
        iaString alias = iResourceManager::getInstance().getAlias(_modelID);
        if (alias.isEmpty())
        {
            _labelAlias->setText("---");
        }
        else
        {
            _labelAlias->setText(alias);
        }
        _meshPaths = meshPaths;

        if (_meshPaths.empty())
        {
            _meshPath->setText("all meshs are referenced");
        }
        else
        {
            iaString text;

            for (const auto &path : _meshPaths)
            {
                if (!text.isEmpty())
                {
                    text += "\n";
                }

                text += path;
            }

            if (!text.isEmpty())
            {
                _meshPath->setText(text);
            }
            else
            {
                _meshPath->setText("all meshs are referenced");
            }
        }

        _change(this);
    }

    iResourceID iUserControlMeshReference::getModelID() const
    {
        return _modelID;
    }

    const std::vector<iaString> &iUserControlMeshReference::getMeshPaths() const
    {
        return _meshPaths;
    }

    void iUserControlMeshReference::onDragMove(iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID())
        {
            drag.reject();
            return;
        }

        iResourceID id = mimeData.getResourceID();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType != IGOR_RESOURCE_MODEL)
        {
            drag.reject();
            return;
        }

        drag.accept();
    }

    void iUserControlMeshReference::onDrop(const iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID())
        {
            return;
        }

        const iResourceID id = mimeData.getResourceID();
        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType != IGOR_RESOURCE_MODEL)
        {
            return;
        }

        if (mimeData.hasText() &&
            !mimeData.getText().isEmpty())
        {
            setReference(id, {mimeData.getText()});
        }
        else
        {
            setReference(id, {});
        }
    }

} // namespace igor