#ifndef IGOR_TILE_MAP_GENERATOR_H
#define IGOR_TILE_MAP_GENERATOR_H

#include <igor/igor.h>
using namespace igor;

/*! The tile map generator it's generating a mesh based on tiles
*/
class TileMapGenerator
{

public:
	/*! nothing to do
	*/
	TileMapGenerator() = default;

	/*! sets the sprite to use for generation

	\param sprite the sprite to use
	*/
	void setSprite(iSpritePtr sprite);

	/*! generates map based on a texture

	!!! ugly hard coded color keys !!!

	\param filename filename of texture to use
	\returns a node containing a hierarchy of multiple patches of tiles
	*/
	iNodePtr generateFromTexture(const iaString &filename);

	/*! generates map based on ranged random values

	\param size size of map
	\param from the lower end of the range
	\param to the upper end of the range
	\returns a node containing a hierarchy of multiple patches of tiles
	*/
	iNodePtr generateFromRandom(const iaVector2i &size, uint32 from, uint32 to);

	/*! set's material to use for the mesh node

	\param material the material id
	*/
	void setMaterial(const iShaderMaterialPtr &material);

private:
	/*! the material to use
	*/
	iShaderMaterialPtr _material;

	/*! the sprite to use
	*/
	iSpritePtr _sprite = nullptr;

	/*! random number generator
	*/
	iaRandomNumberGenerator _random;

	/*! adds a tile to a mesh builder

	\param meshBuilder the mesh builder to add the tile to
	\param pos the positio of the tile in world space
	\param size the size of the tile in world space
	\param frame the frame to use within the sprite
	*/
	void addTile(iMeshBuilder &meshBuilder, const iaVector2i &pos, const iaVector2i &size, const iSprite::iFrame &frame);

	/*! generates the mesh of a single patch of tiles based on texture information

	\param pixmap pointer to the pixmap
	\param pos position in pixel within the pixmap
	\param size size in pixel within the pixmap
	\returns the mesh patch
	*/
	iMeshPtr generateMesh(const iPixmapPtr pixmap, const iaVector2i &pos, const iaVector2i &size);

	/*! generates the mesh of a single patch of tiles based on randomized tiles

	\param from the lower end of the range
	\param to the upper end of the range
	\param pos position in pixel within the pixmap
	\param size size in pixel within the pixmap
	\returns the mesh patch
	*/
	iMeshPtr generateMesh(uint32 from, uint32 to, const iaVector2i &pos, const iaVector2i &size);
};

#endif // IGOR_TILE_MAP_GENERATOR_H