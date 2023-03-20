// OMPF 3d model file format
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <ompf/chunks/ompfGroupChunk.h>

namespace OMPF
{

    ompfGroupChunk::ompfGroupChunk()
        : ompfBaseChunk(OMPFChunkType::Group)
    {
    }

    uint32 ompfGroupChunk::getSize(const ompfSettings &settings)
    {
        return ompfBaseChunk::getSize(settings);
    }

    bool ompfGroupChunk::write(std::ofstream &stream, const ompfSettings &settings)
    {
        return ompfBaseChunk::write(stream, settings);
    }

    bool ompfGroupChunk::read(std::ifstream &stream, ompfSettings &settings)
    {
        return ompfBaseChunk::read(stream, settings);
    }

} // namespace OMPF
