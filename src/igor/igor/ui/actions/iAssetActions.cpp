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
        auto path = actionContext->getPath() + IGOR_PATHSEPARATOR + "my_scene.scene";
        path = iaFile::generateUniqueFilename(path);        
        iResourceManager::getInstance().saveResource(resource, path);

        path = iaDirectory::getRelativePath(iProject::getInstance().getProjectPath(), path);
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
        auto path = actionContext->getPath() + IGOR_PATHSEPARATOR + "my_material.mat";
        path = iaFile::generateUniqueFilename(path);
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
        auto path = actionContext->getPath() + IGOR_PATHSEPARATOR + "my_shader.shader";
        path = iaFile::generateUniqueFilename(path);
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
        iProject::getInstance().load(actionContext->getPath());
    }   

   iActionCreateSprite::iActionCreateSprite()
        : iAction("igor:create_sprite")
    {
        // setIcon("");
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
        auto path = actionContext->getPath() + IGOR_PATHSEPARATOR + "my_sprite.sprite";
        path = iaFile::generateUniqueFilename(path);
        iResourceManager::getInstance().saveResource(resource, path);
        iResourceManager::getInstance().addToDictionary(path, "", resource->getID());
    }        


} // namespace igor