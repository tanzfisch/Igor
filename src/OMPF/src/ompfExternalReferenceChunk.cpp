// OMPF 3d model file format
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <ompfExternalReferenceChunk.h>

namespace OMPF
{

    ompfExternalReferenceChunk::ompfExternalReferenceChunk()
        : ompfBaseChunk(OMPFChunkType::External)
	{
	}

	void ompfExternalReferenceChunk::setFilename(const iaString& filename)
	{
		_filename = filename;
	}

	iaString ompfExternalReferenceChunk::getFilename() const
	{
		return _filename;
	}

    uint32 ompfExternalReferenceChunk::getSize(const ompfSettings& settings)
    {
        return static_cast<uint32>(_filename.getUTF8Size() + 2) + static_cast<uint32>(ompfBaseChunk::getSize(settings));
    }

    bool ompfExternalReferenceChunk::write(ofstream& stream, const ompfSettings& settings)
    {
        if (!ompfBaseChunk::write(stream, settings))
        {
            return false;
        }

        con_debug_endl("---------------------------------------------------");
        con_debug_endl("write ompfExternalReferenceChunk " << this->getName());

        if (!iaSerializable::writeUTF8(stream, _filename))
        {
            return false;
        }
        con_debug_endl("filename " << _filename);

        return true;
    }

    bool ompfExternalReferenceChunk::read(ifstream& stream, ompfSettings& settings)
    {
		if (!ompfBaseChunk::read(stream, settings))
		{
			return false;
		}

        con_debug_endl("---------------------------------------------------");
        con_debug_endl("read ompfExternalReferenceChunk " << this->getName());
        
        if (!iaSerializable::readUTF8(stream, _filename))
        {
            return false;
        }
        con_debug_endl("filename " << _filename);

		return true;
	}

}

