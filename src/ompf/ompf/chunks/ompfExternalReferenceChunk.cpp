// OMPF 3d model file format
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <ompf/chunks/ompfExternalReferenceChunk.h>

namespace OMPF
{

    ompfExternalReferenceChunk::ompfExternalReferenceChunk()
        : ompfBaseChunk(OMPFChunkType::External)
    {
    }

    void ompfExternalReferenceChunk::setFilename(const iaString &filename)
    {
        _filename = filename;
    }

    iaString ompfExternalReferenceChunk::getFilename() const
    {
        return _filename;
    }

    uint32 ompfExternalReferenceChunk::getSize(const ompfSettings &settings)
    {
        return static_cast<uint32>(_filename.getUTF8Size() + 2) + static_cast<uint32>(ompfBaseChunk::getSize(settings));
    }

    bool ompfExternalReferenceChunk::write(std::ofstream &stream, const ompfSettings &settings)
    {
        if (!ompfBaseChunk::write(stream, settings))
        {
            return false;
        }

        con_debug("---------------------------------------------------");
        con_debug("write ompfExternalReferenceChunk " << this->getName());

        if (!iaSerializable::writeUTF8(stream, _filename))
        {
            return false;
        }
        con_debug("filename " << _filename);

        return true;
    }

    bool ompfExternalReferenceChunk::read(std::ifstream &stream, ompfSettings &settings)
    {
        if (!ompfBaseChunk::read(stream, settings))
        {
            return false;
        }

        con_debug("---------------------------------------------------");
        con_debug("read ompfExternalReferenceChunk " << this->getName());

        if (!iaSerializable::readUTF8(stream, _filename))
        {
            return false;
        }
        con_debug("filename " << _filename);

        return true;
    }

} // namespace OMPF
