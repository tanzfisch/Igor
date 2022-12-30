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

void OBJ2OMPF::setMaterialRecursive(iNodePtr node, uint64 materialID)
{
    if (node->getType() == iNodeType::iNodeMesh)
    {
        iNodeMesh *meshNode = static_cast<iNodeMesh *>(node);
        meshNode->setMaterial(materialID);
    }

    if (node->hasChildren())
    {
        auto children = node->getChildren();
        auto childIter = children.begin();
        while (childIter != children.end())
        {
            setMaterialRecursive((*childIter), materialID);
            childIter++;
        }
    }
}

void OBJ2OMPF::convert(int argc, char *argv[])
{
    if (analyzeParam(argc, argv))
    {
        uint64 materialID = iMaterialResourceFactory::getInstance().createMaterial("Textured");
        iMaterialResourceFactory::getInstance().getMaterial(materialID)->addShaderSource("textured.vert", iShaderObjectType::Vertex);
        iMaterialResourceFactory::getInstance().getMaterial(materialID)->addShaderSource("textured_directional_light.frag", iShaderObjectType::Fragment);
        iMaterialResourceFactory::getInstance().getMaterial(materialID)->setOrder(iMaterial::RENDER_ORDER_DEFAULT);

        iModelDataInputParameter *parameters = new iModelDataInputParameter();
        parameters->_joinVertexes = _joinVertexes;
        parameters->_identifier = "obj";
        parameters->_modelSourceType = iModelSourceType::File;
        parameters->_needsRenderContext = false;
        parameters->_keepMesh = true;

        iNodeModel *modelNode = iNodeManager::getInstance().createNode<iNodeModel>();
        modelNode->setModel(_src, iResourceCacheMode::Keep, parameters);

        iScenePtr scene = iSceneFactory::getInstance().createScene();
        scene->getRoot()->insertNode(modelNode);

        while (modelNode->getChild("obj_root") == nullptr)
        {
            iModelResourceFactory::getInstance().flush(nullptr);
            scene->handle();
        }

        // force him to use the textured material
        setMaterialRecursive(modelNode, materialID);

        auto materials = iMaterialResourceFactory::getInstance().getSortedMaterials();
        for (auto material : materials)
        {
            con_endl("material " << material->getName() << " with id " << material->getID());
        }

        iNodePtr objRoot = modelNode->getChild("obj_root");
        if (objRoot && objRoot->getChildren().size() == 1)
        {
            iModelResourceFactory::getInstance().exportModelData(_dst, (*objRoot->getChildren().begin()), "ompf", iSaveMode::EmbedExternals);
        }
        else
        {
            iModelResourceFactory::getInstance().exportModelData(_dst, objRoot, "ompf", iSaveMode::EmbedExternals);
        }

        iSceneFactory::getInstance().destroyScene(scene);
    }
}