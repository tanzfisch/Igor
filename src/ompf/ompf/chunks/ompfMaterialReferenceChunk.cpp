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

    void ompfMaterialReferenceChunk::setReference(const iaString &reference)
    {
        _reference = reference;
    }

    iaString ompfMaterialReferenceChunk::getReference() const
    {
        return _reference;
    }

    uint32 ompfMaterialReferenceChunk::getSize(const ompfSettings &settings)
    {
        return static_cast<uint32>(_reference.getUTF8Size() + 2) + static_cast<uint32>(ompfBaseChunk::getSize(settings));
    }

    bool ompfMaterialReferenceChunk::write(std::ofstream &stream, const ompfSettings &settings)
    {
        if (!ompfBaseChunk::write(stream, settings))
        {
            return false;
        }

        con_trace("---------------------------------------------------");
        con_trace("write ompfMaterialReferenceChunk " << this->getName());

        if (!iaSerializable::writeUTF8(stream, _reference))
        {
            return false;
        }
        con_trace("reference " << _reference);

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

        if (!iaSerializable::readUTF8(stream, _reference))
        {
            return false;
        }
        con_trace("reference " << _reference);

        return true;
    }

} // namespace OMPF
