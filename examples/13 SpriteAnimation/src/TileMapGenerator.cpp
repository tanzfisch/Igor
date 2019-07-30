#include "TileMapGenerator.h"

#include <iTextureResourceFactory.h>
#include <iMeshBuilder.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

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

void TileMapGenerator::setAtlas(iAtlas* atlas)
{
	_atlas = atlas;
}

iMeshPtr TileMapGenerator::generateFromBitmap(const iaString& filename)
{
	con_assert(_atlas != nullptr, "zero pointer");

	if (_atlas == nullptr)
	{
		return nullptr;
	}

	iPixmapPtr pixmap = iTextureResourceFactory::getInstance().loadPixmap(filename);

	iMeshBuilder meshBuilder;
	meshBuilder.setJoinVertexes(false);

	const float32 gridSize = 16;
	const iaVector2f xdir(2 * gridSize,-1 * gridSize);
	const iaVector2f ydir(-2 * gridSize, -1 * gridSize);

	for (int y = 0; y < pixmap->getHeight(); ++y)
	{
		for (int x = 0; x < pixmap->getWidth(); ++x)
		{
			uint8 tileType = pixmap->getPixel(x, y, 0);
			if (tileType < _atlas->getFrameCount())
			{
				 iAtlas::Frame frame = _atlas->getFrame(tileType);
				 iaVector2f pos = (xdir * x) + (ydir * y);
				 addTile(meshBuilder, pos._x, pos._y, gridSize * 4.0, gridSize * 2.0, frame);
			}
		}
	}

	delete pixmap;

	return meshBuilder.createMesh();
}