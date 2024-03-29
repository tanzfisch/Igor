// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlMaterialChooser.h>

#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/resources/iResourceManager.h>
#include <igor/resources/texture/iThumbnailCache.h>
#include <igor/data/iMimeData.h>
#include <igor/ui/iDrag.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

namespace igor
{

    iUserControlMaterialChooser::iUserControlMaterialChooser(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlMaterialChooser, parent)
    {
        setAcceptDrop(true);

        initGUI();
    }

    iUserControlMaterialChooser::~iUserControlMaterialChooser()
    {
    }

    void iUserControlMaterialChooser::initGUI()
    {
        iWidgetBoxLayoutPtr layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, this);
        iWidgetBoxLayoutPtr labelLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, layout);

        _labelID = new iWidgetLabel(labelLayout);
        _labelID->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelID->setVerticalAlignment(iVerticalAlignment::Top);
        _labelID->setText("---");

        _labelAlias = new iWidgetLabel(labelLayout);
        _labelAlias->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelAlias->setVerticalAlignment(iVerticalAlignment::Top);
        _labelAlias->setText("---");

        _picture = new iWidgetPicture(layout);
        _picture->setMaxSize(64, 64);
        _picture->setMinSize(64, 64);
    }

    void iUserControlMaterialChooser::setID(iResourceID materialID)
    {
        _materialID = materialID;

        if (!_materialID.isValid())
        {
            _picture->setTexture(iTexturePtr());
            _labelID->setText("---");
            _labelAlias->setText("---");
            return;
        }

        // TODO 
        /*const iaString filename = iResourceManager::getInstance().getFilename(_materialID);
        iaFile file(iResourceManager::getInstance().resolvePath(filename));
        _picture->setTexture(iThumbnailCache::getInstance().getThumbnail(file.getFullFileName()));*/

        _labelID->setText(_materialID.toString());
        _labelAlias->setText(iResourceManager::getInstance().getAlias(_materialID));

        _change(this);
    }

    iResourceID iUserControlMaterialChooser::getID() const
    {
        return _materialID;
    }

    void iUserControlMaterialChooser::onDragMove(iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID())
        {
            drag.reject();
            return;
        }

        iResourceID id = mimeData.getResourceID();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType != IGOR_RESOURCE_MATERIAL)
        {
            drag.reject();
            return;
        }

        drag.accept();
    }

    void iUserControlMaterialChooser::onDrop(const iDrag &drag)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID())
        {
            return;
        }

        iResourceID id = mimeData.getResourceID();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType != IGOR_RESOURCE_MATERIAL)
        {
            return;
        }

        setID(id);
    }

} // namespace igor