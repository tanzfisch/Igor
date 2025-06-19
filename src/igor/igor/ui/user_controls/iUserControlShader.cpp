// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlShader.h>

#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/resources/iResourceManager.h>
#include <igor/resources/texture/iThumbnailCache.h>
#include <igor/data/iMimeData.h>
#include <igor/ui/iDrag.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

namespace igor
{

    iUserControlShader::iUserControlShader(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlShader, parent)
    {
        setAcceptDrop(true);

        onInitUI();
    }

    iUserControlShader::~iUserControlShader()
    {
    }

    void iUserControlShader::onInitUI()
    {
        iWidgetBoxLayoutPtr layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, this);
        _picture = new iWidgetPicture(layout);
        _picture->setKeepAspectRatio(false);
        _picture->setMaxSize(64, 64);
        _picture->setMinSize(64, 64);

        iWidgetBoxLayoutPtr labelLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, layout);
        _labelID = new iWidgetLabel(labelLayout);
        _labelID->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelID->setVerticalAlignment(iVerticalAlignment::Top);

        _labelAlias = new iWidgetLabel(labelLayout);
        _labelAlias->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelAlias->setVerticalAlignment(iVerticalAlignment::Top);
    }

    void iUserControlShader::setShader(const iResourceID &shaderID)
    {
        _shaderID = shaderID;

        if (!_shaderID.isValid())
        {
            _picture->setTexture(iTexturePtr());
            _labelID->setText("");
            _labelAlias->setText("");
            return;
        }

        /*const iaString filename = iResourceManager::getInstance().getFilename(_shaderID);
        iaFile file(iResourceManager::getInstance().resolvePath(filename));
        _picture->setTexture(iThumbnailCache::getInstance().getThumbnail(file.getFullFileName()));*/

        _labelID->setText(_shaderID.toString());
        _labelAlias->setText(iResourceManager::getInstance().getAlias(_shaderID));

        _change(this);
    }

    const iResourceID& iUserControlShader::getShaderID() const
    {
        return _shaderID;
    }

    void iUserControlShader::onDragMove(iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID())
        {
            drag.reject();
            return;
        }

        iResourceID id = mimeData.getResourceID();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType != IGOR_RESOURCE_SHADER)
        {
            drag.reject();
            return;
        }

        drag.accept();
    }

    void iUserControlShader::onDrop(const iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID())
        {
            return;
        }

        iResourceID id = mimeData.getResourceID();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType != IGOR_RESOURCE_SHADER)
        {
            return;
        }

        setShader(id);
    }

} // namespace igor