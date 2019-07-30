#ifndef __TILEMAPGENERATOR__
#define __TILEMAPGENERATOR__

#include <iMesh.h>
#include <iAtlas.h>
using namespace Igor;

#include <iaString.h>
using namespace IgorAux;

class TileMapGenerator
{

public:

	TileMapGenerator() = default;

	void setAtlas(iAtlas* atlas);
	iMeshPtr generateFromBitmap(const iaString& filename);

private:

	iAtlas* _atlas = nullptr;

	void addTile(iMeshBuilder& meshBuilder, float32 x, float32 y, float32 sizeX, float32 sizeY, const iAtlas::Frame& frame);

};

#endif