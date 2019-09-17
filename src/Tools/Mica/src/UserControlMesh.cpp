// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlMesh.h"

#include <iWidgetSelectBox.h>
#include <iWidgetManager.h>
#include <iWidgetGrid.h>
#include <iWidgetTextEdit.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetSlider.h>
#include <iWidgetNumberChooser.h>
#include <iNodeMesh.h>
#include <iMesh.h>
#include <iNodeManager.h>
#include <iTargetMaterial.h>
#include <iTextureResourceFactory.h>
#include <iResourceManager.h>
#include <iMaterialResourceFactory.h>
#include <iMaterial.h>
#include <iUserControlFileChooser.h>
#include <iMeshBuilderUtils.h>
#include <iScene.h>
using namespace Igor;

#include "MicaDefines.h"

UserControlMesh::UserControlMesh()
{
	con_endl("UserControlMesh id " << getID());	
	initGUI();
}

UserControlMesh::~UserControlMesh()
{
	deinitGUI();
}

void UserControlMesh::onAmbientChange(const iaColor4f& color)
{
	_ambient = color;
	updateNode();
}

void UserControlMesh::onDiffuseChange(const iaColor4f& color)
{
	_diffuse = color;
	updateNode();
}

void UserControlMesh::onSpecularChange(const iaColor4f& color)
{
	_specular = color;
	updateNode();
}

void UserControlMesh::onEmissiveChange(const iaColor4f& color)
{
	_emissive = color;
	updateNode();
}

void UserControlMesh::onTextChangedShininess(iWidgetPtr source)
{
	_shininess = _textShininess->getValue();
	_sliderShininess->setValue(_shininess);
	updateNode();
}

void UserControlMesh::onSliderChangedShininess(iWidgetPtr source)
{
	_shininess = _sliderShininess->getValue();
	_textShininess->setValue(_shininess);
	updateNode();
}

void UserControlMesh::updateNode()
{
	if (!_ignoreNodeUpdate)
	{
		iNodeMesh* node = static_cast<iNodeMesh*>(iNodeManager::getInstance().getNode(_nodeId));

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

			node->getTargetMaterial()->setTexture(iTextureResourceFactory::getInstance().loadFile(_textureChooser0->getFileName()), 0);
			node->getTargetMaterial()->setTexture(iTextureResourceFactory::getInstance().loadFile(_textureChooser1->getFileName()), 1);
			node->getTargetMaterial()->setTexture(iTextureResourceFactory::getInstance().loadFile(_textureChooser2->getFileName()), 2);
			node->getTargetMaterial()->setTexture(iTextureResourceFactory::getInstance().loadFile(_textureChooser3->getFileName()), 3);

			if (_selectMaterial->getSelectedUserData() != nullptr)
			{
				uint32 materialID = *(static_cast<uint32*>(_selectMaterial->getSelectedUserData()));
				node->setMaterial(materialID);
			}
		}
	}
}

void UserControlMesh::updateGUI()
{
	iNodeMesh* node = static_cast<iNodeMesh*>(iNodeManager::getInstance().getNode(_nodeId));

	if (node != nullptr)
	{
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

		_textVertices->setText(iaString::toString(node->getVertexCount()));
		_textTriangles->setText(iaString::toString(node->getTrianglesCount()));
		_textIndexes->setText(iaString::toString(node->getIndexesCount()));

		if (node->getTargetMaterial()->hasTextureUnit(0))
		{
			iaString filename = node->getTargetMaterial()->getTexture(0)->getFilename();
			iaString shortName = iResourceManager::getInstance().getRelativePath(filename);
			if (!shortName.isEmpty())
			{
				filename = shortName;
			}

			_textureChooser0->setFileName(filename);
		}

		if (node->getTargetMaterial()->hasTextureUnit(1))
		{
			iaString filename = node->getTargetMaterial()->getTexture(1)->getFilename();
			iaString shortName = iResourceManager::getInstance().getRelativePath(filename);
			if (!shortName.isEmpty())
			{
				filename = shortName;
			}

			_textureChooser1->setFileName(filename);
		}

		if (node->getTargetMaterial()->hasTextureUnit(2))
		{
			iaString filename = node->getTargetMaterial()->getTexture(2)->getFilename();
			iaString shortName = iResourceManager::getInstance().getRelativePath(filename);
			if (!shortName.isEmpty())
			{
				filename = shortName;
			}

			_textureChooser2->setFileName(filename);
		}

		if (node->getTargetMaterial()->hasTextureUnit(3))
		{
			iaString filename = node->getTargetMaterial()->getTexture(3)->getFilename();
			iaString shortName = iResourceManager::getInstance().getRelativePath(filename);
			if (!shortName.isEmpty())
			{
				filename = shortName;
			}

			_textureChooser3->setFileName(filename);
		}

		for (auto entry : _userDataMaterialID)
		{
			delete entry;
		}
		_userDataMaterialID.clear();
		_selectMaterial->clear();

		auto materials = iMaterialResourceFactory::getInstance().getSortedMaterials();
		for (auto material : materials)
		{
			if (material->isValid())
			{
				uint32 materialID = material->getID();
				iaString materialName = material->getName();

				uint32* ptrmaterialID = new uint32(materialID);
				_selectMaterial->addSelectionEntry(materialName, ptrmaterialID);
				_userDataMaterialID.push_back(ptrmaterialID);

				if (materialID == node->getMaterial())
				{
					_selectMaterial->setSelection(_selectMaterial->getSelectionEntryCount() - 1);
				}
			}
		}

		_ignoreNodeUpdate = false;
	}
}

void UserControlMesh::draw()
{
	iWidget::draw();
}

void UserControlMesh::setNode(uint32 id)
{
	_nodeId = id;
	updateGUI();
}

uint32 UserControlMesh::getNode()
{
	return _nodeId;
}

void UserControlMesh::deinitGUI()
{
	if (!_initialized)
	{
		return;
	}

	clearChildren();

	_ambientColorChooser = nullptr;
	_diffuseColorChooser = nullptr;
	_specularColorChooser = nullptr;
	_emissiveColorChooser = nullptr;
	_textVertices = nullptr;
	_textTriangles = nullptr;
	_textIndexes = nullptr;
	_sliderShininess = nullptr;
	_textShininess = nullptr;
	_textureChooser0 = nullptr;
	_textureChooser1 = nullptr;
	_textureChooser2 = nullptr;
	_textureChooser3 = nullptr;
	_selectMaterial = nullptr;

	_initialized = false;
}

void UserControlMesh::initGUI()
{
	if (_initialized)
	{
		return;
	}

	iWidgetGridPtr grid = new iWidgetGrid(this);
	grid->appendRows(9);
	grid->setStrechRow(8);
	grid->setStrechColumn(0);
	grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
	grid->setVerticalAlignment(iVerticalAlignment::Strech);

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

	iWidgetGrid* detailsGrid = new iWidgetGrid();
	detailsGrid->appendCollumns(1);
	detailsGrid->appendRows(2);
	detailsGrid->setStrechColumn(1);
	detailsGrid->setHorizontalAlignment(iHorizontalAlignment::Strech);

	iWidgetLabel* labelVertexes = new iWidgetLabel();
	labelVertexes->setText("Vertexes");
	labelVertexes->setWidth(MICA_REGULARBUTTON_SIZE);
	labelVertexes->setHorizontalAlignment(iHorizontalAlignment::Left);

	iWidgetLabel* labelTriangles = new iWidgetLabel();
	labelTriangles->setText("Triangles");
	labelTriangles->setWidth(MICA_REGULARBUTTON_SIZE);
	labelTriangles->setHorizontalAlignment(iHorizontalAlignment::Left);

	iWidgetLabel* labelIndexes = new iWidgetLabel();
	labelIndexes->setText("Indexes");
	labelIndexes->setWidth(MICA_REGULARBUTTON_SIZE);
	labelIndexes->setHorizontalAlignment(iHorizontalAlignment::Left);

	_textVertices = new iWidgetTextEdit();
	_textVertices->setHorizontalAlignment(iHorizontalAlignment::Strech);
	_textVertices->setHorizontalTextAlignment(iHorizontalAlignment::Right);
	_textVertices->setActive(false);
	_textVertices->setText("0");

	_textTriangles = new iWidgetTextEdit();
	_textTriangles->setHorizontalAlignment(iHorizontalAlignment::Strech);
	_textTriangles->setHorizontalTextAlignment(iHorizontalAlignment::Right);
	_textTriangles->setActive(false);
	_textTriangles->setText("0");

	_textIndexes = new iWidgetTextEdit();
	_textIndexes->setHorizontalAlignment(iHorizontalAlignment::Strech);
	_textIndexes->setHorizontalTextAlignment(iHorizontalAlignment::Right);
	_textIndexes->setActive(false);
	_textIndexes->setText("0");

	iWidgetGrid* gridShininess = new iWidgetGrid();
	gridShininess->appendCollumns(2);
	gridShininess->appendRows(1);
	gridShininess->setHorizontalAlignment(iHorizontalAlignment::Left);

	iWidgetLabel* labelShininess = new iWidgetLabel();
	labelShininess->setText("Shininess");
	labelShininess->setHorizontalAlignment(iHorizontalAlignment::Left);

	iWidgetLabel* labelShininessShort = new iWidgetLabel();
	labelShininessShort->setText("S");
	labelShininessShort->setHorizontalAlignment(iHorizontalAlignment::Left);

	_sliderShininess = new iWidgetSlider();
	_sliderShininess->setHorizontalAlignment(iHorizontalAlignment::Left);
	_sliderShininess->setMinValue(0.0f);
	_sliderShininess->setMaxValue(1000.0f);
	_sliderShininess->setSteppingWheel(0.1f, 0.1f);
	_sliderShininess->setValue(0.0f);
	_sliderShininess->setWidth(220);
	_sliderShininess->setHorizontalAlignment(iHorizontalAlignment::Left);
	_sliderShininess->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onSliderChangedShininess));

	_textShininess = new iWidgetNumberChooser();
	_textShininess->setMinMaxNumber(0.0f, 1000.0f);
	_textShininess->setAfterPoint(2);
	_textShininess->setValue(0.0f);
	_textShininess->setWidth(80);
	_textShininess->setSteppingWheel(0.1f, 0.1f);
	_textShininess->setStepping(0.01f, 0.01f);
	_textShininess->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onTextChangedShininess));

	iWidgetGrid* gridTextures = new iWidgetGrid();
	gridTextures->appendRows(3);
	gridTextures->appendCollumns(1);
	gridTextures->setStrechColumn(1);
	gridTextures->setHorizontalAlignment(iHorizontalAlignment::Strech);
	gridTextures->setVerticalAlignment(iVerticalAlignment::Top);

	iWidgetLabel* labelTextureUnit0 = new iWidgetLabel();
	labelTextureUnit0->setText("Texture 0");
	labelTextureUnit0->setWidth(MICA_REGULARBUTTON_SIZE);
	labelTextureUnit0->setHorizontalAlignment(iHorizontalAlignment::Left);

	iWidgetLabel* labelTextureUnit1 = new iWidgetLabel();
	labelTextureUnit1->setText("Texture 1");
	labelTextureUnit1->setWidth(MICA_REGULARBUTTON_SIZE);
	labelTextureUnit1->setHorizontalAlignment(iHorizontalAlignment::Left);

	iWidgetLabel* labelTextureUnit2 = new iWidgetLabel();
	labelTextureUnit2->setText("Texture 2");
	labelTextureUnit2->setWidth(MICA_REGULARBUTTON_SIZE);
	labelTextureUnit2->setHorizontalAlignment(iHorizontalAlignment::Left);

	iWidgetLabel* labelTextureUnit3 = new iWidgetLabel();
	labelTextureUnit3->setText("Texture 3");
	labelTextureUnit3->setWidth(MICA_REGULARBUTTON_SIZE);
	labelTextureUnit3->setHorizontalAlignment(iHorizontalAlignment::Left);

	_textureChooser0 = new iUserControlFileChooser();
	_textureChooser0->setPreselectedPath("..\\data\\textures");
	_textureChooser0->registerOnChangedDelegate(iChangeDelegate(this, &UserControlMesh::onDoUpdateNode));

	_textureChooser1 = new iUserControlFileChooser();
	_textureChooser1->setPreselectedPath("..\\data\\textures");
	_textureChooser1->registerOnChangedDelegate(iChangeDelegate(this, &UserControlMesh::onDoUpdateNode));

	_textureChooser2 = new iUserControlFileChooser();
	_textureChooser2->setPreselectedPath("..\\data\\textures");
	_textureChooser2->registerOnChangedDelegate(iChangeDelegate(this, &UserControlMesh::onDoUpdateNode));

	_textureChooser3 = new iUserControlFileChooser();
	_textureChooser3->setPreselectedPath("..\\data\\textures");
	_textureChooser3->registerOnChangedDelegate(iChangeDelegate(this, &UserControlMesh::onDoUpdateNode));

	iWidgetGrid* gridMaterial = new iWidgetGrid();
	gridMaterial->appendCollumns(1);
	gridMaterial->setHorizontalAlignment(iHorizontalAlignment::Left);
	gridMaterial->setVerticalAlignment(iVerticalAlignment::Top);

	iWidgetLabel* labelMaterial = new iWidgetLabel();
	labelMaterial->setText("Material");
	labelMaterial->setHorizontalAlignment(iHorizontalAlignment::Left);

	iWidgetButton* bakeButton = new iWidgetButton();
	bakeButton->registerOnClickEvent(iClickDelegate(this, &UserControlMesh::onBakeAction));
	bakeButton->setText("Backe to world");
	bakeButton->setHorizontalAlignment(iHorizontalAlignment::Left);

	_selectMaterial = new iWidgetSelectBox();
	_selectMaterial->setHorizontalAlignment(iHorizontalAlignment::Right);
	_selectMaterial->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onMaterialChanged));

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

	gridTextures->addWidget(labelTextureUnit0, 0, 0);
	gridTextures->addWidget(labelTextureUnit1, 0, 1);
	gridTextures->addWidget(labelTextureUnit2, 0, 2);
	gridTextures->addWidget(labelTextureUnit3, 0, 3);
	gridTextures->addWidget(_textureChooser0, 1, 0);
	gridTextures->addWidget(_textureChooser1, 1, 1);
	gridTextures->addWidget(_textureChooser2, 1, 2);
	gridTextures->addWidget(_textureChooser3, 1, 3);

	gridMaterial->addWidget(labelMaterial, 0, 0);
	gridMaterial->addWidget(_selectMaterial, 1, 0);

	grid->addWidget(detailsGrid, 0, 0);
	grid->addWidget(_ambientColorChooser, 0, 1);
	grid->addWidget(_diffuseColorChooser, 0, 2);
	grid->addWidget(_specularColorChooser, 0, 3);
	grid->addWidget(_emissiveColorChooser, 0, 4);
	grid->addWidget(gridShininess, 0, 5);
	grid->addWidget(gridTextures, 0, 6);
	grid->addWidget(gridMaterial, 0, 7);
	grid->addWidget(bakeButton, 0, 8);

	_initialized = true;
}

void UserControlMesh::onBakeAction(iWidgetPtr source)
{
	// TODO this kind of action needs to be in an action menu

	iNodeMesh* meshNode = static_cast<iNodeMesh*>(iNodeManager::getInstance().getNode(_nodeId));

	if (meshNode == nullptr)
	{
		return;
	}

	iScene* scene = meshNode->getScene();

	if (scene == nullptr)
	{
		return;
	}

	iaMatrixd matrix;
	meshNode->calcWorldTransformation(matrix);
	iMeshBuilder meshBuilder;
	meshBuilder.setJoinVertexes(false);
	meshBuilder.setMatrix(matrix.convert<float32>());
	iMeshPtr mesh = meshNode->getMesh();
	iMeshBuilderUtils::addMesh(meshBuilder, mesh);

	iNodeMesh* newMeshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
	newMeshNode->setKeepMesh();
	newMeshNode->setMesh(meshBuilder.createMesh());
	newMeshNode->setMaterial(meshNode->getMaterial());
	newMeshNode->setTargetMaterial(meshNode->getTargetMaterial());

	scene->getRoot()->getChild("MicaRoot")->insertNode(newMeshNode);
}

void UserControlMesh::onMaterialChanged(iWidgetPtr source)
{
	updateNode();
}

void UserControlMesh::onDoUpdateNode(iWidgetPtr source)
{
	updateNode();
}
