#include "OBJ2OMPF.h"

#include <list>

bool OBJ2OMPF::analyzeParam(int argc, char *argv[])
{
    if (argc >= 3)
    {
        for (int i = 1; i < argc; ++i)
        {
            iaString currentParam = argv[i];

            if (currentParam == "-nj")
            {
                _joinVertexes = false;
            }
            else
            {
                if (_src.isEmpty())
                {
                    _src = currentParam;
                }
                else if (_dst.isEmpty())
                {
                    _dst = currentParam;
                }
                else
                {
                    con_err("invalid parameters");
                    return false;
                }
            }
        }
    }
    else
    {
        con_err("invalid parameters");
        return false;
    }

    return true;
}

void OBJ2OMPF::setMaterialRecursive(iNodePtr node, iMaterialPtr material)
{
    if (node->getType() == iNodeType::iNodeMesh)
    {
        iNodeMesh *meshNode = static_cast<iNodeMesh *>(node);
        meshNode->setMaterial(material);
    }

    if (node->hasChildren())
    {
        auto children = node->getChildren();
        auto childIter = children.begin();
        while (childIter != children.end())
        {
            setMaterialRecursive((*childIter), material);
            childIter++;
        }
    }
}

void OBJ2OMPF::convert(int argc, char *argv[])
{
    if (analyzeParam(argc, argv))
    {
        iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>("igor_material_texture_shaded");

        iParameters parameters({
            {IGOR_RESOURCE_PARAM_SOURCE, _src},
            {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MODEL},
            {IGOR_RESOURCE_PARAM_JOIN_VERTICES, true},
            {IGOR_RESOURCE_PARAM_KEEP_MESH, true},
            {IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Keep}
        });
        iModelPtr model = iResourceManager::getInstance().loadResource<iModel>(parameters);
        iNodeModel *modelNode = iNodeManager::getInstance().createNode<iNodeModel>();
        modelNode->setModel(model);

        iScenePtr scene = iSceneFactory::getInstance().createScene();
        scene->getRoot()->insertNode(modelNode);

        while (modelNode->getChild("obj_root") == nullptr)
        {
            iResourceManager::getInstance().flush();
            scene->handle();
        }

        // force him to use the textured material
        setMaterialRecursive(modelNode, material);

        std::vector<iMaterialPtr> materials;
        iResourceManager::getInstance().getMaterials(materials);
        for (auto material : materials)
        {
            con_endl("material " << material->getName() << " with id " << material->getID());
        }

        iNodePtr objRoot = modelNode->getChild("obj_root");
        if (objRoot && objRoot->getChildren().size() == 1)
        {
            iModelFactory::exportToFile(_dst, (*objRoot->getChildren().begin()), iSaveMode::EmbedExternals, "ompf");
        }
        else
        {
            iModelFactory::exportToFile(_dst, objRoot, iSaveMode::EmbedExternals, "ompf");
        }

        iSceneFactory::getInstance().destroyScene(scene);
    }
}