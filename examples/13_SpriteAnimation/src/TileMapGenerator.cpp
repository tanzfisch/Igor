// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "TileMapGenerator.h"

#include <map>

void TileMapGenerator::addTile(iMeshBuilder &meshBuilder, const iaVector2i &pos, const iaVector2i &size, const iAtlas::iFrame &frame)
{
	const uint32 offsetIndex = meshBuilder.getVertexCount();

	float32 x = pos._x - frame._origin._x;
	float32 y = pos._y - frame._origin._y;

	meshBuilder.addVertex(iaVector3f(x, y, 0));
	meshBuilder.setTexCoord(iaVector2f(frame._rect._x, frame._rect._y), 0);

	meshBuilder.addVertex(iaVector3f(x, y + size._y, 0));
	meshBuilder.setTexCoord(iaVector2f(frame._rect._x, frame._rect._y + frame._rect._height), 0);

	meshBuilder.addVertex(iaVector3f(x + size._x, y + size._y, 0));
	meshBuilder.setTexCoord(iaVector2f(frame._rect._x + frame._rect._width, frame._rect._y + frame._rect._height), 0);

	meshBuilder.addVertex(iaVector3f(x + size._x, y, 0));
	meshBuilder.setTexCoord(iaVector2f(frame._rect._x + frame._rect._width, frame._rect._y), 0);

	meshBuilder.addTriangle(0, 1, 2, offsetIndex);
	meshBuilder.addTriangle(2, 3, 0, offsetIndex);
}

void TileMapGenerator::setAtlas(iAtlasPtr atlas)
{
	_atlas = atlas;
}

// pixel
static const int32 gridSize = 64;
static const iaVector2i xdir(gridSize / 2, -gridSize / 4);
static const iaVector2i ydir(-gridSize / 2, -gridSize / 4);
static const iaVector2i patchSize(32, 32);

iMeshPtr TileMapGenerator::generateMesh(uint32 from, uint32 to, const iaVector2i &pos, const iaVector2i &size)
{
	_random.setSeed(pos._x << 8 | pos._y + 42);

	iMeshBuilder meshBuilder;
	meshBuilder.setJoinVertexes(false);

	int wy, wx;

	iTextureOldPtr texture = _atlas->getTexture();
	iaVector2i textureSize(texture->getWidth(), texture->getHeight());

	for (int y = pos._y, wy = 0; y < pos._y + size._y; ++y, ++wy)
	{
		for (int x = pos._x, wx = 0; x < pos._x + size._x; ++x, ++wx)
		{
			uint32 tileType = from + (_random.getNext() % (to - from));

			if (tileType < _atlas->getFrameCount())
			{
				const iAtlas::iFrame &frame = _atlas->getFrame(tileType);
				iaVector2i tilePos = xdir * wx + ydir * wy;
				tilePos._y *= -1;
				iaVector2i tileSize(frame._rect._width * static_cast<float32>(textureSize._x), frame._rect._height * static_cast<float32>(textureSize._y));
				addTile(meshBuilder, tilePos, tileSize, frame);
			}
		}
	}

	return meshBuilder.createMesh();
}

iNodePtr TileMapGenerator::generateFromRandom(const iaVector2i &size, uint32 from, uint32 to)
{
	con_assert(_atlas != nullptr, "zero pointer");
	con_assert(from < to, "invalid parameters");

	if (_atlas == nullptr)
	{
		return nullptr;
	}

	iNode *result = iNodeManager::getInstance().createNode<iNode>();

	iaVector2i patchPos;
	for (int64 y = 0; y < (size._y / patchSize._y); ++y)
	{
		for (int64 x = 0; x < (size._x / patchSize._x); ++x)
		{
			patchPos.set(y * patchSize._y, x * patchSize._x);
			iMeshPtr terrainMesh = generateMesh(from, to, patchPos, patchSize);

			iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();

			iaVector2i patchWorldPos = (xdir * patchPos._x) + (ydir * patchPos._y);
			transformNode->translate(patchWorldPos._x, -patchWorldPos._y, 0);
			transformNode->setName(iaString::toString(x) + ":" + iaString::toString(y));

			iNodeMesh *meshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
			meshNode->setMesh(terrainMesh);
			meshNode->setMaterial(_material);
			iTargetMaterialPtr targetMaterial = meshNode->getTargetMaterial();
			targetMaterial->setTexture(_atlas->getTexture(), 0);

			transformNode->insertNode(meshNode);
			result->insertNode(transformNode);
		}
	}

	return result;
}

iMeshPtr TileMapGenerator::generateMesh(const iPixmapPtr pixmap, const iaVector2i &pos, const iaVector2i &size)
{
	iMeshBuilder meshBuilder;
	meshBuilder.setJoinVertexes(false);

	// map color to tile type
	std::map<uint32, uint32> colorToType;
	colorToType[0xff00640a] = 85;
	colorToType[0xff006414] = 86;
	colorToType[0xff00641e] = 87;
	colorToType[0xff006428] = 88;
	colorToType[0xff00c800] = 89;
	colorToType[0xff006432] = 90;
	colorToType[0xff00643c] = 91;

	// dressing
	colorToType[0xff0064c8] = 94;
	colorToType[0xff006ec8] = 95;
	colorToType[0xff0078c8] = 96;

	// stick
	colorToType[0xff0082c8] = 99;
	colorToType[0xff008cc8] = 100;

	// more dressing
	colorToType[0xff0096c8] = 101;
	colorToType[0xff00a0c8] = 102;
	colorToType[0xff00aac8] = 103;
	colorToType[0xff00b4c8] = 104;

	// trees
	colorToType[0xff6400c8] = 97;
	colorToType[0xff6e00c8] = 98;

	colorToType[0xffc800c8] = 109;
	colorToType[0xfffa00c8] = 110;

	// water
	colorToType[0xff641400] = 63;
	colorToType[0xff642800] = 64;

	colorToType[0xff640000] = 66;

	colorToType[0xff643c00] = 69;
	colorToType[0xff645000] = 67;

	int wy, wx;

	iTextureOldPtr texture = _atlas->getTexture();
	iaVector2i textureSize(texture->getWidth(), texture->getHeight());

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
				const iAtlas::iFrame &frame = _atlas->getFrame(tileType);
				iaVector2i tilePos = xdir * wx + ydir * wy;
				tilePos._y *= -1;
				iaVector2i tileSize(frame._rect._width * static_cast<float32>(textureSize._x), frame._rect._height * static_cast<float32>(textureSize._y));
				addTile(meshBuilder, tilePos, tileSize, frame);
			}
		}
	}

	return meshBuilder.createMesh();
}

iNodePtr TileMapGenerator::generateFromTexture(const iaString &filename)
{
	con_assert(_atlas != nullptr, "zero pointer");

	if (_atlas == nullptr)
	{
		return nullptr;
	}

	iPixmapPtr pixmap = iTextureFactory::loadPixmap(filename);

	iNode *result = iNodeManager::getInstance().createNode<iNode>();

	iaVector2i patchPos;
	for (int64 y = 0; y < (pixmap->getHeight() / patchSize._y); ++y)
	{
		for (int64 x = 0; x < (pixmap->getWidth() / patchSize._x); ++x)
		{
			patchPos.set(y * patchSize._y, x * patchSize._x);
			iMeshPtr terrainMesh = generateMesh(pixmap, patchPos, patchSize);

			iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();

			iaVector2i patchWorldPos = (xdir * patchPos._x) + (ydir * patchPos._y);
			transformNode->translate(patchWorldPos._x, -patchWorldPos._y, 0);
			transformNode->setName(iaString::toString(x) + ":" + iaString::toString(y));

			iNodeMesh *meshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
			meshNode->setMesh(terrainMesh);
			meshNode->setMaterial(_material);
			iTargetMaterialPtr targetMaterial = meshNode->getTargetMaterial();
			targetMaterial->setTexture(_atlas->getTexture(), 0);

			transformNode->insertNode(meshNode);
			result->insertNode(transformNode);
		}
	}

	delete pixmap;

	return result;
}

void TileMapGenerator::setMaterial(const iMaterialPtr& material)
{
	_material = material;
}