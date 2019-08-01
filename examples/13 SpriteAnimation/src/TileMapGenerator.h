#ifndef __TILEMAPGENERATOR__
#define __TILEMAPGENERATOR__

#include <iMesh.h>
#include <iAtlas.h>
#include <iPixmap.h>
#include <iScene.h>
#include <iMaterial.h>
using namespace Igor;

#include <iaString.h>
using namespace IgorAux;

class TileMapGenerator
{

public:

	TileMapGenerator() = default;

	void setAtlas(iAtlasPtr atlas);
	iNodePtr generateFromBitmap(const iaString& filename);
	void setMaterial(uint64 material);

private:

	uint64 _material = iMaterial::INVALID_MATERIAL_ID;
	iAtlasPtr _atlas = nullptr;

	void addTile(iMeshBuilder& meshBuilder, float32 x, float32 y, float32 sizeX, float32 sizeY, const iAtlas::Frame& frame);
	iMeshPtr generateMesh(const iPixmapPtr pixmap, const iaVector2i& pos, const iaVector2i& size);

};

#endif