// OMPF 3d model file format
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <ompf/chunks/ompfExternalReferenceChunk.h>

namespace OMPF
{

    ompfExternalReferenceChunk::ompfExternalReferenceChunk()
        : ompfBaseChunk(OMPFChunkType::External)
    {
    }

    void ompfExternalReferenceChunk::setReference(const iaString &reference)
    {
        _reference = reference;
    }

    const iaString& ompfExternalReferenceChunk::getReference() const
    {
        return _reference;
    }

    uint32 ompfExternalReferenceChunk::getSize(const ompfSettings &settings)
    {
        return static_cast<uint32>(_reference.getUTF8Size() + 2) + static_cast<uint32>(ompfBaseChunk::getSize(settings));
    }

    bool ompfExternalReferenceChunk::write(std::ofstream &stream, const ompfSettings &settings)
    {
        if (!ompfBaseChunk::write(stream, settings))
        {
            return false;
        }

        con_trace("---------------------------------------------------");
        con_trace("write ompfExternalReferenceChunk " << this->getName());

        if (!iaSerializable::writeUTF8(stream, _reference))
        {
            return false;
        }
        con_trace("reference " << _reference);

        return true;
    }

    bool ompfExternalReferenceChunk::read(std::ifstream &stream, ompfSettings &settings)
    {
        if (!ompfBaseChunk::read(stream, settings))
        {
            return false;
        }

        con_trace("---------------------------------------------------");
        con_trace("read ompfExternalReferenceChunk " << this->getName());

        if (!iaSerializable::readUTF8(stream, _reference))
        {
            return false;
        }
        con_trace("reference " << _reference);

        return true;
    }

} // namespace OMPF
