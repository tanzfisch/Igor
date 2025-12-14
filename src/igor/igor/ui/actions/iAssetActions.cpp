#include <igor/ui/actions/iAssetActions.h>

#include <igor/ui/actions/context/iFilesystemActionContext.h>
#include <iaux/system/iaFile.h>

#include <igor/resources/iResourceManager.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/prefab/iPrefab.h>
#include <igor/resources/shader/iShader.h>
#include <igor/resources/project/iProject.h>
#include <igor/resources/sprite/iSprite.h>

namespace igor
{

    iActionCreateScene::iActionCreateScene()
        : iAction("igor:create_scene")
    {
        setIcon("igor_icon_scene");
        setDescription("Create Scene", "Create a new scene");
    }

    bool iActionCreateScene::isCompatible(const iActionContext &context)
    {
        const iFilesystemActionContext *actionContext = dynamic_cast<const iFilesystemActionContext *>(&context);
        if (actionContext == nullptr)
        {
            return false;
        }        

        return true;
    }

    void iActionCreateScene::execute(const iActionContext &context)
    {
        const iFilesystemActionContext *actionContext = static_cast<const iFilesystemActionContext *>(&context);

        iPrefabPtr resource = iResourceManager::getInstance().createResource<iPrefab>();
        auto path = actionContext->getPath() + IGOR_PATHSEPARATOR + "scene.scene";
        path = iaPath::generateUniqueFilename(path);        
        iResourceManager::getInstance().saveResource(resource, path);

        path = iaPath::getRelativePath(iProject::getInstance().getProjectPath(), path);
        iResourceManager::getInstance().addToDictionary(path, "", resource->getID());
    }    

    iActionCreateMaterial::iActionCreateMaterial()
        : iAction("igor:create_material")
    {
        setIcon("igor_icon_material");
        setDescription("Create material", "Create a new material");
    }

    bool iActionCreateMaterial::isCompatible(const iActionContext &context)
    {
        const iFilesystemActionContext *actionContext = dynamic_cast<const iFilesystemActionContext *>(&context);
        if (actionContext == nullptr)
        {
            return false;
        }

        return true;
    }

    void iActionCreateMaterial::execute(const iActionContext &context)
    {
        const iFilesystemActionContext *actionContext = static_cast<const iFilesystemActionContext *>(&context);

        iMaterialPtr resource = iResourceManager::getInstance().createResource<iMaterial>();
        auto path = actionContext->getPath() + IGOR_PATHSEPARATOR + "material.mat";
        path = iaPath::generateUniqueFilename(path);
        iResourceManager::getInstance().saveResource(resource, path);
        iResourceManager::getInstance().addToDictionary(path, "", resource->getID());
    }    
    
    iActionCreateShader::iActionCreateShader()
        : iAction("igor:create_shader")
    {
        setIcon("igor_icon_shader");
        setDescription("Create shader", "Create a new shader");
    }

    bool iActionCreateShader::isCompatible(const iActionContext &context)
    {
        const iFilesystemActionContext *actionContext = dynamic_cast<const iFilesystemActionContext *>(&context);
        if (actionContext == nullptr)
        {
            return false;
        }

        return true;
    }

    void iActionCreateShader::execute(const iActionContext &context)
    {
        const iFilesystemActionContext *actionContext = static_cast<const iFilesystemActionContext *>(&context);

        iShaderPtr resource = iResourceManager::getInstance().createResource<iShader>();
        auto path = actionContext->getPath() + IGOR_PATHSEPARATOR + "shader.shader";
        path = iaPath::generateUniqueFilename(path);
        iResourceManager::getInstance().saveResource(resource, path);
        iResourceManager::getInstance().addToDictionary(path, "", resource->getID());
    }        

    iActionLoadProject::iActionLoadProject()
        : iAction("igor:load_project")
    {
        setDescription("...", "Loads given project");
    }

    bool iActionLoadProject::isCompatible(const iActionContext &context)
    {
        const iFilesystemActionContext *actionContext = dynamic_cast<const iFilesystemActionContext *>(&context);
        if (actionContext == nullptr)
        {
            return false;
        }

        setDescription(actionContext->getPath());

        return true;
    }

    void iActionLoadProject::execute(const iActionContext &context)
    {
        const iFilesystemActionContext *actionContext = static_cast<const iFilesystemActionContext *>(&context);
        iProject::getInstance().load(actionContext->getPath(), iProject::iMode::Edit);
    }   

   iActionCreateSprite::iActionCreateSprite()
        : iAction("igor:create_sprite")
    {
        setIcon("igor_icon_sprite");
        setDescription("Create sprite", "Create a new sprite");
    }

    bool iActionCreateSprite::isCompatible(const iActionContext &context)
    {
        const iFilesystemActionContext *actionContext = dynamic_cast<const iFilesystemActionContext *>(&context);
        if (actionContext == nullptr)
        {
            return false;
        }

        return true;
    }

    void iActionCreateSprite::execute(const iActionContext &context)
    {
        const iFilesystemActionContext *actionContext = static_cast<const iFilesystemActionContext *>(&context);

        iSpritePtr resource = iResourceManager::getInstance().createResource<iSprite>();
        auto path = actionContext->getPath() + IGOR_PATHSEPARATOR + "sprite.sprite";
        path = iaPath::generateUniqueFilename(path);
        iResourceManager::getInstance().saveResource(resource, path);
        iResourceManager::getInstance().addToDictionary(path, "", resource->getID());
    }        

    iActionCreateScript::iActionCreateScript()
        : iAction("igor:create_script")
    {
        setIcon("igor_icon_script");
        setDescription("Create script", "Create a new script");
    }

    bool iActionCreateScript::isCompatible(const iActionContext &context)
    {
        const iFilesystemActionContext *actionContext = dynamic_cast<const iFilesystemActionContext *>(&context);
        if (actionContext == nullptr)
        {
            return false;
        }

        return true;
    }

    void iActionCreateScript::execute(const iActionContext &context)
    {
        const iFilesystemActionContext *actionContext = static_cast<const iFilesystemActionContext *>(&context);

        iScriptPtr resource = iResourceManager::getInstance().createResource<iScript>();
        auto path = actionContext->getPath() + IGOR_PATHSEPARATOR + "script.chai";
        path = iaPath::generateUniqueFilename(path);
        iResourceManager::getInstance().saveResource(resource, path);
        iResourceManager::getInstance().addToDictionary(path, "", resource->getID());
    }       

} // namespace igor