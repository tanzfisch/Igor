#include "OBJ2OMPF.h"

#include <iNodeVisitorPrintTree.h>
#include <iNodeModel.h> 
#include <iModelResourceFactory.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeFactory.h>
#include <iMaterialResourceFactory.h>
#include <iMaterialGroup.h>
#include <iShader.h>
#include <iNodeMesh.h>
using namespace Igor;

#include <iaConsole.h>
#include <iaDirectory.h>
using namespace IgorAux;

#include <list>
using namespace std;

bool OBJ2OMPF::analyzeParam(int argc, char* argv[])
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

void OBJ2OMPF::setMaterialRecursive(iNode* node, int32 materialID)
{
    if (node->getType() == iNodeType::iNodeMesh)
    {
        iNodeMesh* meshNode = static_cast<iNodeMesh*>(node);
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

void OBJ2OMPF::convert(int argc, char* argv[])
{
    if (analyzeParam(argc, argv))
    {
        int32 materialID = iMaterialResourceFactory::getInstance().createMaterial("Textured");
        iMaterialResourceFactory::getInstance().getMaterial(materialID)->addShaderSource(L"textured.vert", iShaderObjectType::Vertex);
        iMaterialResourceFactory::getInstance().getMaterial(materialID)->addShaderSource(L"textured_directional_light.frag", iShaderObjectType::Fragment);
        iMaterialResourceFactory::getInstance().getMaterial(materialID)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
        iMaterialResourceFactory::getInstance().getMaterialGroup(materialID)->setOrder(100);

        iModelDataInputParameter* parameters = new iModelDataInputParameter();
        parameters->_joinVertexes = _joinVertexes;
        parameters->_identifier = "obj";
        parameters->_modelSourceType = iModelSourceType::File;
        parameters->_needsRenderContext = false;

        iNodeModel* modelNode = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        modelNode->setModel(_src, iResourceCacheMode::Free, parameters); // force him to use the textured material

        iScene* scene = iSceneFactory::getInstance().createScene();
        scene->getRoot()->insertNode(modelNode);

        while(modelNode->getChild("obj_root") == nullptr)
        {
            iModelResourceFactory::getInstance().flush(nullptr);
            scene->handle();
        }

        setMaterialRecursive(modelNode, materialID);

        list<iMaterialGroup*>* materials = iMaterialResourceFactory::getInstance().getMaterialGroups();
        auto iter = materials->begin();
        while (iter != materials->end())
        {
            con_endl("material " << (*iter)->getMaterial()->getName() << " with id " << (*iter)->getID());
            iter++;
        }

        iNode* objRoot = modelNode->getChild("obj_root");
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
    else
    {
        con_err("invalid parameters");
    }
}