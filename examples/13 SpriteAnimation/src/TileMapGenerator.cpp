#include "TileMapGenerator.h"

#include <iTextureResourceFactory.h>
#include <iMeshBuilder.h>
#include <iNodeTransform.h>
#include <iNodeFactory.h>
#include <iNodeMesh.h>
#include <iTargetMaterial.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

#include <map>

void TileMapGenerator::addTile(iMeshBuilder& meshBuilder, float32 x, float32 y, float32 sizeX, float32 sizeY, const iAtlas::Frame& frame)
{
	const uint32 offsetIndex = meshBuilder.getVertexCount();

	meshBuilder.addVertex(iaVector3f(x, y, 0));
	meshBuilder.setTexCoord(iaVector2f(frame._pos._x, frame._pos._y), 0);
	meshBuilder.addVertex(iaVector3f(x, y + sizeY, 0));
	meshBuilder.setTexCoord(iaVector2f(frame._pos._x, frame._pos._y + frame._size._y), 0);
	meshBuilder.addVertex(iaVector3f(x + sizeX, y + sizeY, 0));
	meshBuilder.setTexCoord(iaVector2f(frame._pos._x + frame._size._x, frame._pos._y + frame._size._y), 0);
	meshBuilder.addVertex(iaVector3f(x + sizeX, y, 0));
	meshBuilder.setTexCoord(iaVector2f(frame._pos._x + frame._size._x, frame._pos._y), 0);
	
	meshBuilder.addTriangle(0, 1, 2, offsetIndex);
	meshBuilder.addTriangle(2, 3, 0, offsetIndex);
}

void TileMapGenerator::setAtlas(iAtlasPtr atlas)
{
	_atlas = atlas;
}

// pixel
static const int32 gridSize = 16;
static const iaVector2i xdir(2 * gridSize, -1 * gridSize);
static const iaVector2i ydir(-2 * gridSize, -1 * gridSize);
static const iaVector2i patchSize(32, 32);

// world
static const iaVector2i gridScale(gridSize * 4.0, gridSize * 2.0);

iMeshPtr TileMapGenerator::generateMesh(const iPixmapPtr pixmap, const iaVector2i& pos, const iaVector2i& size)
{
	iMeshBuilder meshBuilder;
	meshBuilder.setJoinVertexes(false);

	// map color to tile type
	std::map<uint32, uint32> colorToType;
	colorToType[0xff006400] = 0;
	colorToType[0xff000064] = 4;
	colorToType[0xff009600] = 5;
	colorToType[0xff000096] = 9;
	colorToType[0xff00c800] = 10;

	int wy, wx;

	for (int y = pos._y, wy = 0; y < pos._y + size._y; ++y, ++wy)
	{
		for (int x = pos._x, wx = 0; x < pos._x + size._x; ++x, ++wx)
		{
			uint32 tileType = 0xffffffff;
			auto iter = colorToType.find(pixmap->getPixel(x, y));
			if (iter != colorToType.end())
			{
				tileType = iter->second;
			}

			if (tileType < _atlas->getFrameCount())
			{
				iAtlas::Frame frame = _atlas->getFrame(tileType);
				iaVector2i tilePos = xdir * wx + ydir * wy;
				addTile(meshBuilder, tilePos._x, -tilePos._y, gridSize * 4.0, gridSize * 2.0, frame);
			}
		}
	}

	return meshBuilder.createMesh();
}

iNodePtr TileMapGenerator::generateFromBitmap(const iaString& filename)
{
	con_assert(_atlas != nullptr, "zero pointer");
	con_assert(_material != iMaterial::INVALID_MATERIAL_ID, "no material defined");

	if (_atlas == nullptr)
	{
		return nullptr;
	}

	iPixmapPtr pixmap = iTextureResourceFactory::getInstance().loadPixmap(filename);

	iNode* result = iNodeFactory::getInstance().createNode(iNodeType::iNode);
	result->setName("Terrain");

	iaVector2i patchPos;
	for (int y = 0; y < (pixmap->getHeight() / patchSize._y); ++y)
	{
		for (int x = 0; x < (pixmap->getWidth() / patchSize._x); ++x)
		{
			patchPos.set(y * patchSize._y, x * patchSize._x);
			iMeshPtr terrainMesh = generateMesh(pixmap, patchPos, patchSize);

			iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));

			iaVector2i patchWorldPos = (xdir * patchPos._x) + (ydir * patchPos._y);
			transformNode->translate(patchWorldPos._x, -patchWorldPos._y, 0);
			transformNode->setName(iaString::toString(x) + ":" + iaString::toString(y));

			iNodeMesh* meshNode = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
			meshNode->setMesh(terrainMesh);
			meshNode->setMaterial(_material);
			iTargetMaterial* targetMaterial = meshNode->getTargetMaterial();
			targetMaterial->setTexture(_atlas->getTexture(), 0);

			transformNode->insertNode(meshNode);
			result->insertNode(transformNode);
		}
	}

	delete pixmap;

	return result;
}

void TileMapGenerator::setMaterial(uint64 material)
{
	_material = material;
}