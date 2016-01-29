// OMPF 3d model file format
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <ompfGroupChunk.h>

namespace OMPF
{

	ompfGroupChunk::ompfGroupChunk()
		: ompfBaseChunk(OMPFChunkTypes::Group)
	{
	}
	
	ompfGroupChunk::~ompfGroupChunk()
	{
	}

    uint32 ompfGroupChunk::getSize(const ompfSettings& settings)
    {
        return ompfBaseChunk::getSize(settings);
    }

    bool ompfGroupChunk::write(ofstream& file, const ompfSettings& settings)
    {
        return ompfBaseChunk::write(file, settings);
    }

    bool ompfGroupChunk::read(ifstream& file, ompfSettings& settings)
    {
        return ompfBaseChunk::read(file, settings);
    }

}

