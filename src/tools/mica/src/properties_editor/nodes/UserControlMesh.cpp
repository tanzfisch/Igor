// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlMesh.h"

#include "../../MicaDefines.h"

#include <igor/igor.h>
using namespace igor;

UserControlMesh::UserControlMesh(iNodeID nodeID, const iWidgetPtr parent)
    : UserControlNode(nodeID, parent)
{
}

void UserControlMesh::onAmbientChange(const iaColor4f &color)
{
    _ambient = color;
    updateNode();
}

void UserControlMesh::onDiffuseChange(const iaColor4f &color)
{
    _diffuse = color;
    updateNode();
}

void UserControlMesh::onSpecularChange(const iaColor4f &color)
{
    _specular = color;
    updateNode();
}

void UserControlMesh::onEmissiveChange(const iaColor4f &color)
{
    _emissive = color;
    updateNode();
}

void UserControlMesh::onTextChangedShininess(const iWidgetPtr source)
{
    _shininess = _textShininess->getValue();
    _sliderShininess->setValue(_shininess);
    updateNode();
}

void UserControlMesh::onSliderChangedShininess(const iWidgetPtr source)
{
    _shininess = _sliderShininess->getValue();
    _textShininess->setValue(_shininess);
    updateNode();
}

void UserControlMesh::updateNode()
{
    if (_ignoreNodeUpdate)
    {
        return;
    }

    iNodeMesh *node = static_cast<iNodeMesh *>(iNodeManager::getInstance().getNode(getNodeID()));

    if (node != nullptr)
    {
        iaColor3f ambient(_ambient._r, _ambient._g, _ambient._b);
        iaColor3f diffuse(_diffuse._r, _diffuse._g, _diffuse._b);
        iaColor3f specular(_specular._r, _specular._g, _specular._b);
        iaColor3f emissive(_emissive._r, _emissive._g, _emissive._b);

        node->getTargetMaterial()->setAmbient(ambient);
        node->getTargetMaterial()->setDiffuse(diffuse);
        node->getTargetMaterial()->setSpecular(specular);
        node->getTargetMaterial()->setEmissive(emissive);
        node->getTargetMaterial()->setShininess(_shininess);

        for (int i = 0; i < 4; ++i)
        {
            node->getTargetMaterial()->setTexture(iResourceManager::getInstance().loadResource<iTexture>(_textureChooser[i]->getTextureID()), i);
        }

        if (_materialSelector->getSelectedUserData().has_value())
        {
            std::any userData = _materialSelector->getSelectedUserData();
            iShaderMaterialID materialID(std::any_cast<iShaderMaterialID>(userData));
            node->setMaterial(iResourceManager::getInstance().getResource<iShaderMaterial>(materialID));
        }
    }
}

void UserControlMesh::update()
{
    UserControlNode::update();

    iNodeMesh *node = static_cast<iNodeMesh *>(iNodeManager::getInstance().getNode(getNodeID()));
    _ignoreNodeUpdate = true;

    iaColor3f ambient = node->getAmbient();
    _ambient.set(ambient._r, ambient._g, ambient._b, 1.0f);

    iaColor3f diffuse = node->getDiffuse();
    _diffuse.set(diffuse._r, diffuse._g, diffuse._b, 1.0f);

    iaColor3f specular = node->getSpecular();
    _specular.set(specular._r, specular._g, specular._b, 1.0f);

    iaColor3f emissive = node->getEmissive();
    _emissive.set(emissive._r, emissive._g, emissive._b, 1.0f);

    _shininess = node->getShininess();

    _ambientColorChooser->setColor(_ambient);
    _diffuseColorChooser->setColor(_diffuse);
    _specularColorChooser->setColor(_specular);
    _emissiveColorChooser->setColor(_emissive);
    _sliderShininess->setValue(_shininess);
    _textShininess->setValue(_shininess);

    iMeshPtr mesh = node->getMesh();

    _textVertices->setText(iaString::toString(mesh->getVertexCount()));
    _textTriangles->setText(iaString::toString(mesh->getTrianglesCount()));
    _textIndexes->setText(iaString::toString(mesh->getIndexCount()));

    for (int i = 0; i < 4; ++i)
    {
        if (node->getTargetMaterial()->hasTextureUnit(i))
        {
            _textureChooser[i]->setTextureID(node->getTargetMaterial()->getTexture(i)->getID());
        }
    }

    _materialSelector->clear();

/* TODO    std::vector<iShaderMaterialPtr> materials;
    iResourceManager::getInstance().getMaterials(materials);
    for (auto material : materials)
    {
        if (material->isValid())
        {
            const iShaderMaterialID &materialID = material->getID();
            const iaString &materialName = material->getName();

            _materialSelector->addItem(materialName, materialID);

            if (node->getMaterial() != nullptr &&
                materialID == node->getMaterial()->getID())
            {
                _materialSelector->setSelection(_materialSelector->getItemCount() - 1);
            }
        }
    }*/

    _ignoreNodeUpdate = false;
}

void UserControlMesh::init()
{
    UserControlNode::init();

    iWidgetGridLayoutPtr grid = new iWidgetGridLayout(getLayout());
    grid->appendRows(9);
    grid->setStretchRow(8);
    grid->setStretchColumn(0);
    grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    grid->setVerticalAlignment(iVerticalAlignment::Stretch);

    _ambientColorChooser = new iUserControlColorChooser();
    _ambientColorChooser->setMode(iColorChooserMode::RGB);
    _ambientColorChooser->setText("Ambient");
    _ambientColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlMesh::onAmbientChange));

    _diffuseColorChooser = new iUserControlColorChooser();
    _diffuseColorChooser->setMode(iColorChooserMode::RGB);
    _diffuseColorChooser->setText("Diffuse");
    _diffuseColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlMesh::onDiffuseChange));

    _specularColorChooser = new iUserControlColorChooser();
    _specularColorChooser->setMode(iColorChooserMode::RGB);
    _specularColorChooser->setText("Specular");
    _specularColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlMesh::onSpecularChange));

    _emissiveColorChooser = new iUserControlColorChooser();
    _emissiveColorChooser->setMode(iColorChooserMode::RGB);
    _emissiveColorChooser->setText("Emissive");
    _emissiveColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlMesh::onEmissiveChange));

    iWidgetGridLayout *detailsGrid = new iWidgetGridLayout();
    detailsGrid->appendColumns(1);
    detailsGrid->appendRows(2);
    detailsGrid->setStretchColumn(1);
    detailsGrid->setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetLabel *labelVertexes = new iWidgetLabel();
    labelVertexes->setText("Vertexes");
    labelVertexes->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVertexes->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelTriangles = new iWidgetLabel();
    labelTriangles->setText("Triangles");
    labelTriangles->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelTriangles->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelIndexes = new iWidgetLabel();
    labelIndexes->setText("Indexes");
    labelIndexes->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelIndexes->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textVertices = new iWidgetLineTextEdit();
    _textVertices->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textVertices->setHorizontalTextAlignment(iHorizontalAlignment::Right);
    _textVertices->setEnabled(false);
    _textVertices->setText("0");

    _textTriangles = new iWidgetLineTextEdit();
    _textTriangles->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textTriangles->setHorizontalTextAlignment(iHorizontalAlignment::Right);
    _textTriangles->setEnabled(false);
    _textTriangles->setText("0");

    _textIndexes = new iWidgetLineTextEdit();
    _textIndexes->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textIndexes->setHorizontalTextAlignment(iHorizontalAlignment::Right);
    _textIndexes->setEnabled(false);
    _textIndexes->setText("0");

    iWidgetGridLayout *gridShininess = new iWidgetGridLayout();
    gridShininess->appendColumns(2);
    gridShininess->appendRows(1);
    gridShininess->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelShininess = new iWidgetLabel();
    labelShininess->setText("Shininess");
    labelShininess->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetLabel *labelShininessShort = new iWidgetLabel();
    labelShininessShort->setText("S");
    labelShininessShort->setHorizontalAlignment(iHorizontalAlignment::Left);

    _sliderShininess = new iWidgetSlider();
    _sliderShininess->setHorizontalAlignment(iHorizontalAlignment::Left);
    _sliderShininess->setMinValue(0.0f);
    _sliderShininess->setMaxValue(1000.0f);
    _sliderShininess->setSteppingWheel(0.1f, 0.1f);
    _sliderShininess->setValue(0.0f);
    _sliderShininess->setMinWidth(220);
    _sliderShininess->setHorizontalAlignment(iHorizontalAlignment::Left);
    _sliderShininess->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onSliderChangedShininess));

    _textShininess = new iWidgetNumberChooser();
    _textShininess->setMinMaxNumber(0.0f, 1000.0f);
    _textShininess->setAfterPoint(2);
    _textShininess->setValue(0.0f);
    _textShininess->setMinWidth(80);
    _textShininess->setSteppingWheel(0.1f, 0.1f);
    _textShininess->setStepping(0.01f, 0.01f);
    _textShininess->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onTextChangedShininess));

    iWidgetGridLayout *gridTextures = new iWidgetGridLayout();
    gridTextures->appendRows(3);
    gridTextures->appendColumns(1);
    gridTextures->setStretchColumn(1);
    gridTextures->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    gridTextures->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabelPtr labelTextureUnit[4] = {nullptr, nullptr, nullptr, nullptr};
    for (int i = 0; i < 4; ++i)
    {
        labelTextureUnit[i] = new iWidgetLabel();
        labelTextureUnit[i]->setText(iaString("Texture ") + iaString::toString(i));
        labelTextureUnit[i]->setMinWidth(MICA_REGULARBUTTON_SIZE);
        labelTextureUnit[i]->setVerticalAlignment(iVerticalAlignment::Top);
        labelTextureUnit[i]->setHorizontalAlignment(iHorizontalAlignment::Left);

        _textureChooser[i] = new iUserControlTextureChooser();
        _textureChooser[i]->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onDoUpdateNode));
    }

    iWidgetGridLayout *gridMaterial = new iWidgetGridLayout();
    gridMaterial->appendColumns(1);
    gridMaterial->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridMaterial->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel *labelMaterial = new iWidgetLabel();
    labelMaterial->setText("Shader Material");
    labelMaterial->setHorizontalAlignment(iHorizontalAlignment::Left);

    _materialSelector = new iWidgetSelectBox();
    _materialSelector->setHorizontalAlignment(iHorizontalAlignment::Right);
    _materialSelector->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onMaterialChanged));

    gridShininess->addWidget(labelShininess, 1, 0);
    gridShininess->addWidget(labelShininessShort, 0, 1);
    gridShininess->addWidget(_sliderShininess, 1, 1);
    gridShininess->addWidget(_textShininess, 2, 1);

    detailsGrid->addWidget(labelVertexes, 0, 0);
    detailsGrid->addWidget(labelTriangles, 0, 1);
    detailsGrid->addWidget(labelIndexes, 0, 2);

    detailsGrid->addWidget(_textVertices, 1, 0);
    detailsGrid->addWidget(_textTriangles, 1, 1);
    detailsGrid->addWidget(_textIndexes, 1, 2);

    for (int i = 0; i < 4; ++i)
    {
        gridTextures->addWidget(labelTextureUnit[i], 0, i);
        gridTextures->addWidget(_textureChooser[i], 1, i);
    }

    gridMaterial->addWidget(labelMaterial, 0, 0);
    gridMaterial->addWidget(_materialSelector, 1, 0);

    grid->addWidget(detailsGrid, 0, 0);
    grid->addWidget(_ambientColorChooser, 0, 1);
    grid->addWidget(_diffuseColorChooser, 0, 2);
    grid->addWidget(_specularColorChooser, 0, 3);
    grid->addWidget(_emissiveColorChooser, 0, 4);
    grid->addWidget(gridShininess, 0, 5);
    grid->addWidget(gridTextures, 0, 6);
    grid->addWidget(gridMaterial, 0, 7);
}

void UserControlMesh::onMaterialChanged(const iWidgetPtr source)
{
    updateNode();
}

void UserControlMesh::onDoUpdateNode(const iWidgetPtr source)
{
    updateNode();
}
