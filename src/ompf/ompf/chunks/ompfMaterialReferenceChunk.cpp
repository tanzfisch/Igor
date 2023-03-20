// OMPF 3d model file format
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <ompf/chunks/ompfMaterialReferenceChunk.h>

namespace OMPF
{

    ompfMaterialReferenceChunk::ompfMaterialReferenceChunk()
        : ompfBaseChunk(OMPFChunkType::MaterialReference)
    {
    }

    void ompfMaterialReferenceChunk::setFilename(const iaString &filename)
    {
        _filename = filename;
    }

    iaString ompfMaterialReferenceChunk::getFilename() const
    {
        return _filename;
    }

    uint32 ompfMaterialReferenceChunk::getSize(const ompfSettings &settings)
    {
        return static_cast<uint32>(_filename.getUTF8Size() + 2) + static_cast<uint32>(ompfBaseChunk::getSize(settings));
    }

    bool ompfMaterialReferenceChunk::write(std::ofstream &stream, const ompfSettings &settings)
    {
        if (!ompfBaseChunk::write(stream, settings))
        {
            return false;
        }

        con_trace("---------------------------------------------------");
        con_trace("write ompfMaterialReferenceChunk " << this->getName());

        if (!iaSerializable::writeUTF8(stream, _filename))
        {
            return false;
        }
        con_trace("filename " << _filename);

        return true;
    }

    bool ompfMaterialReferenceChunk::read(std::ifstream &stream, ompfSettings &settings)
    {
        if (!ompfBaseChunk::read(stream, settings))
        {
            return false;
        }

        con_trace("---------------------------------------------------");
        con_trace("read ompfMaterialReferenceChunk " << this->getName());

        if (!iaSerializable::readUTF8(stream, _filename))
        {
            return false;
        }
        con_trace("filename " << _filename);

        return true;
    }

} // namespace OMPF
