// OMPF 3d model file format
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <ompf/chunks/ompfTransformChunk.h>

#include <iaux/system/iaConsole.h>

namespace OMPF
{

    ompfTransformChunk::ompfTransformChunk()
        : ompfBaseChunk(OMPFChunkType::Transform)
    {
    }

    uint32 ompfTransformChunk::getSize(const ompfSettings &settings)
    {
        return (sizeof(float32) * 16) + ompfBaseChunk::getSize(settings);
    }

    void ompfTransformChunk::setMatrix(const iaMatrixd &matrix)
    {
        _matrix = matrix;
    }

    void ompfTransformChunk::getMatrix(iaMatrixd &matrix) const
    {
        matrix = _matrix;
    }

    bool ompfTransformChunk::write(std::ofstream &file, const ompfSettings &settings)
    {
        if (!ompfBaseChunk::write(file, settings))
        {
            return false;
        }

        con_trace("---------------------------------------------------");
        con_trace("write ompfTransformChunk " << getName());
        con_trace("matrix " << _matrix);

        if (!iaSerializable::write(file, reinterpret_cast<char *>(_matrix.getData()), sizeof(float64) * 16))
        {
            return false;
        }

        return true;
    }

    bool ompfTransformChunk::read(std::ifstream &file, ompfSettings &settings)
    {
        if (!ompfBaseChunk::read(file, settings))
        {
            return false;
        }

        con_trace("---------------------------------------------------");
        con_trace("read ompfTransformChunk " << getName());
        con_trace("matrix " << _matrix);

        if (!iaSerializable::read(file, reinterpret_cast<char *>(_matrix.getData()), sizeof(float64) * 16))
        {
            return false;
        }

        return true;
    }

} // namespace OMPF
