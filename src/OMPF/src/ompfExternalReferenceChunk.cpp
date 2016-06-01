// OMPF 3d model file format
// (c) Copyright 2014-2016 by Martin Loga
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
        return _filename.getUTF8Size() + 2 + ompfBaseChunk::getSize(settings);
    }

    bool ompfExternalReferenceChunk::write(ofstream& file, const ompfSettings& settings)
    {
        if (!ompfBaseChunk::write(file, settings))
        {
            return false;
        }

        if (!iaSerializable::writeUTF8(file, _filename))
        {
            return false;
        }

        return true;
    }

    bool ompfExternalReferenceChunk::read(ifstream& file, ompfSettings& settings)
    {
		if (!ompfBaseChunk::read(file, settings))
		{
			return false;
		}
        
        if (!iaSerializable::readUTF8(file, _filename))
        {
            return false;
        }

		return true;
	}

}

