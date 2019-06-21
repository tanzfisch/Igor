#include "BakeToWorld.h"

#include <iNodeVisitorPrintTree.h>
#include <iNodeModel.h> 
#include <iModelResourceFactory.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeFactory.h>
#include <iMaterialResourceFactory.h>
#include <iShader.h>
#include <iNodeMesh.h>
#include <iRenderer.h>
using namespace Igor;

#include <iaConsole.h>
#include <iaDirectory.h>
using namespace IgorAux;

#include <list>
using namespace std;

bool BakeToWorld::analyzeParam(int argc, char* argv[])
{
    if (argc >= 2)
    {
        for (int i = 1; i < argc; ++i)
        {
            iaString currentParam = argv[i];

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
    else
    {
        con_err("invalid parameters");
        return false;
    }

    return true;
}

void BakeToWorld::bake(int argc, char* argv[])
{
    if (analyzeParam(argc, argv))
    {
        uint64 materialID = iMaterialResourceFactory::getInstance().createMaterial("Textured");
        iMaterialResourceFactory::getInstance().getMaterial(materialID)->addShaderSource("textured.vert", iShaderObjectType::Vertex);
        iMaterialResourceFactory::getInstance().getMaterial(materialID)->addShaderSource("textured_directional_light.frag", iShaderObjectType::Fragment);
        iMaterialResourceFactory::getInstance().getMaterial(materialID)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
        iMaterialResourceFactory::getInstance().getMaterial(materialID)->setOrder(iMaterial::RENDER_ORDER_DEFAULT);

        iModelDataInputParameter* parameters = new iModelDataInputParameter();
        parameters->_joinVertexes = _joinVertexes;
        parameters->_identifier = "obj";
        parameters->_modelSourceType = iModelSourceType::File;
        parameters->_needsRenderContext = false;
        parameters->_keepMesh = true;

        iNodeModel* modelNode = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        modelNode->setModel(_src, iResourceCacheMode::Keep, parameters);

        iScene* scene = iSceneFactory::getInstance().createScene();
        scene->getRoot()->insertNode(modelNode);

        while(modelNode->getChild("obj_root") == nullptr)
        {
            iModelResourceFactory::getInstance().flush(nullptr);
            scene->handle();
        }

        // force him to use the textured material
        
		setMaterialRecursive(modelNode, materialID);

        auto materials = iMaterialResourceFactory::getInstance().getSortedMaterials();
        for(auto material : materials)
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