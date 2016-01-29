// OMPF 3d model file format
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <ompfTransformChunk.h>

#include <iaConsole.h>

namespace OMPF
{

	ompfTransformChunk::ompfTransformChunk()
		: ompfBaseChunk(OMPFChunkTypes::Transform)
	{
	}
	
    uint32 ompfTransformChunk::getSize(const ompfSettings& settings)
    {
        return (sizeof(float32) * 16) + ompfBaseChunk::getSize(settings);
    }

    void ompfTransformChunk::setMatrix(const iaMatrixf& matrix)
    {
        _matrix = matrix;
    }

    void ompfTransformChunk::getMatrix(iaMatrixf& matrix) const
    {
        matrix = _matrix;
    }

    bool ompfTransformChunk::write(ofstream& file, const ompfSettings& settings)
    {
        if (!ompfBaseChunk::write(file, settings))
        {
            return false;
        }

        if (!iaSerializable::write(file, reinterpret_cast<char*>(_matrix.getData()), sizeof(float32) * 16))
        {
            return false;
        }

        return true;
    }

    bool ompfTransformChunk::read(ifstream& file, ompfSettings& settings)
    {
        if (!ompfBaseChunk::read(file, settings))
        {
            return false;
        }

        if (!iaSerializable::read(file, reinterpret_cast<char*>(_matrix.getData()), sizeof(float32) * 16))
        {
            return false;
        }

        return false;
    }

}

