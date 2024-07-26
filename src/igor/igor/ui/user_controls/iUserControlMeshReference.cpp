// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
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

    void iUserControlMeshReference::setReference(const iResourceID &modelID, const iaString &meshPath)
    {
        _modelID = modelID;

        if (!_modelID.isValid())
        {
            _labelID->setText("");
            _labelAlias->setText("");
            return;
        }

        _labelID->setText(_modelID.toString());
        _labelAlias->setText(iResourceManager::getInstance().getAlias(_modelID));
        _labelAlias->setVisible(!_labelAlias->getText().isEmpty());
        _meshPath->setText(meshPath);

        _change(this);
    }

    iResourceID iUserControlMeshReference::getModelID() const
    {
        return _modelID;
    }

    const iaString &iUserControlMeshReference::getMeshPath() const
    {
        return _meshPath->getText();
    }

    void iUserControlMeshReference::onDragMove(iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID() ||
            !mimeData.hasText())
        {
            drag.reject();
            return;
        }

        iResourceID id = mimeData.getResourceID();
        iaString meshPath = mimeData.getText();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType != IGOR_RESOURCE_MODEL ||
            meshPath.isEmpty())
        {
            drag.reject();
            return;
        }

        drag.accept();
    }

    void iUserControlMeshReference::onDrop(const iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID() ||
            !mimeData.hasText())
        {
            return;
        }

        iResourceID id = mimeData.getResourceID();
        iaString meshPath = mimeData.getText();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType != IGOR_RESOURCE_MODEL)
        {
            return;
        }

        setReference(id, meshPath);
    }

} // namespace igor