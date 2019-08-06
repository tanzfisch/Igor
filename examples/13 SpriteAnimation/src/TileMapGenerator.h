#ifndef __TILEMAPGENERATOR__
#define __TILEMAPGENERATOR__

#include <iMesh.h>
#include <iAtlas.h>
#include <iPixmap.h>
#include <iScene.h>
#include <iMaterial.h>
using namespace Igor;

#include <iaString.h>
#include <iaRandomNumberGenerator.h>
using namespace IgorAux;

class TileMapGenerator
{

public:

	TileMapGenerator() = default;

	void setAtlas(iAtlasPtr atlas);
	iNodePtr generateFromTexture(const iaString& filename);
	iNodePtr generateFromRandom(const iaVector2i& size, uint32 from, uint32 to);
	void setMaterial(uint64 material);

private:

	uint64 _material = iMaterial::INVALID_MATERIAL_ID;
	iAtlasPtr _atlas = nullptr;
	iaRandomNumberGeneratoru _random;

	void addTile(iMeshBuilder& meshBuilder, const iaVector2i& pos, const iaVector2i& size, const iAtlas::Frame& frame);
	iMeshPtr generateMesh(const iPixmapPtr pixmap, const iaVector2i& pos, const iaVector2i& size);
	iMeshPtr generateMesh(uint32 from, uint32 to, const iaVector2i& pos, const iaVector2i& size);

};

#endif