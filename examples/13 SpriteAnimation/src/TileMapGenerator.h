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

/*! The tile map generator it's generating a mesh based on tiles
*/
class TileMapGenerator
{

public:

	/*! nothing to do
	*/
	TileMapGenerator() = default;

	/*! sets the atlas to use for generation

	\param atlas the atlas to use
	*/
	void setAtlas(iAtlasPtr atlas);

	/*! generates map based on a texture

	!!! ugly hard coded color keys !!!

	\param filename filename of texture to use
	\returns a node containing a hierarchy of multiple patches of tiles
	*/ 
	iNodePtr generateFromTexture(const iaString& filename);

	/*! generates map based on ranged random values

	\param size size of map
	\param from the lower end of the range
	\param to the upper end of the range
	\returns a node containing a hierarchy of multiple patches of tiles
	*/
	iNodePtr generateFromRandom(const iaVector2i& size, uint32 from, uint32 to);

	/*! set's material to use for the mesh node

	\param material the material id
	*/
	void setMaterial(uint64 material);

private:

	/*! the material to use
	*/
	uint64 _material = iMaterial::INVALID_MATERIAL_ID;

	/*! the atlas to use
	*/
	iAtlasPtr _atlas = nullptr;

	/*! random number generator
	*/
	iaRandomNumberGeneratoru _random;

	/*! adds a tile to a mesh builder

	\param meshBuilder the mesh builder to add the tile to
	\param pos the positio of the tile in world space
	\param size the size of the tile in world space
	\param frame the frame to use within the atlas
	*/
	void addTile(iMeshBuilder& meshBuilder, const iaVector2i& pos, const iaVector2i& size, const iAtlas::Frame& frame);

	/*! generates the mesh of a single patch of tiles based on texture information

	\param pixmap pointer to the pixmap
	\param pos position in pixel within the pixmap
	\param size size in pixel within the pixmap
	\returns the mesh patch
	*/
	iMeshPtr generateMesh(const iPixmapPtr pixmap, const iaVector2i& pos, const iaVector2i& size);

	/*! generates the mesh of a single patch of tiles based on randomized tiles

	\param from the lower end of the range
	\param to the upper end of the range
	\param pos position in pixel within the pixmap
	\param size size in pixel within the pixmap
	\returns the mesh patch
	*/
	iMeshPtr generateMesh(uint32 from, uint32 to, const iaVector2i& pos, const iaVector2i& size);

};

#endif