// OMPF 3d model file format
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <ompfHeaderChunk.h>
#include <ompfVersion.h>
#include <ompfSettings.h>
#include <OMPF.h>

#include <iaConsole.h>

using namespace IgorAux;

namespace OMPF
{

	ompfHeaderChunk::ompfHeaderChunk()
		: ompfBaseChunk(OMPFChunkTypes::Header)
	{

	}
	
	ompfHeaderChunk::~ompfHeaderChunk()
	{
	}

    bool ompfHeaderChunk::write(ofstream& file, const ompfSettings& settings)
    {
        if (!iaSerializable::write(file, "OMPF", 4))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(file, settings.getMajorVersion()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(file, settings.getMinorVersion()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(file, settings.getPatchVersion()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(file, settings.getTypeIDSize()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(file, settings.getChunkIDSize()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(file, settings.getChunkSizeSize()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(file, settings.getCustomDataSizeSize()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(file, settings.getStringSizeSize()))
        {
            return false;
        }

        if (!iaSerializable::write(file, "I\3Igor", 6)) // I?IGOR
        {
            return false;
        }

        con_debug_endl("write OMPF with following configuration" << endl << "MajorVersion:" << settings.getMajorVersion() << endl \
            << "MinorVersion:" << settings.getMinorVersion() << endl << "PatchVersion:" << settings.getPatchVersion() << endl \
            << "TypeIDSize:" << settings.getTypeIDSize() << endl << "ChunkIDSize:" << settings.getChunkIDSize() << endl \
            << "ChunkSizeSize:" << settings.getChunkSizeSize() << endl << "CustomDataSizeSize:" << settings.getCustomDataSizeSize() << endl \
            << "StringSizeSize:" << settings.getStringSizeSize());

        return ompfBaseChunk::write(file, settings);
    }
    
    bool ompfHeaderChunk::read(ifstream& file, ompfSettings& settings)
    {
        char magicNumber[5];
#ifdef __IGOR_DEBUG__
        memset(magicNumber, '#', 5);
#endif
        iaSerializable::read(file, magicNumber, 4);
        magicNumber[4] = 0;

        if (string(magicNumber) != "OMPF")
        {
            con_err("invalid magic number");
            return false;
        }

        uint8 majorVersion = 0;
        uint8 minorVersion = 0;
        uint8 patchVersion = 0;

        iaSerializable::readUInt8(file, majorVersion);
        iaSerializable::readUInt8(file, minorVersion);
        iaSerializable::readUInt8(file, patchVersion);

        uint32 version = (static_cast<uint32>(majorVersion) << 16) | (static_cast<uint32>(minorVersion) << 8) | static_cast<uint32>(patchVersion);
        
		//! \todo check minor and patch too
        if (version > settings.getVersion())
        {
            con_err("Only versions up to " << OMPF_VERSION_MAJOR << "." << OMPF_VERSION_MINOR << "." << OMPF_VERSION_PATCH << " supported.");
			return false;
        }

        uint8 typeIDSize = 0;
        if (!iaSerializable::readUInt8(file, typeIDSize))
        {
            return false;
        }

        settings.setTypeIDSize(typeIDSize);

        uint8 chunkIDSize = 0;
        if (!iaSerializable::readUInt8(file, chunkIDSize))
        {
            return false;
        }

        settings.setChunkIDSize(chunkIDSize);

        uint8 chunkSizeSize = 0;
        if (!iaSerializable::readUInt8(file, chunkSizeSize))
        {
            return false;
        }
        
        settings.setChunkSizeSize(chunkSizeSize);

        uint8 customdataSizeSize = 0;
        if (!iaSerializable::readUInt8(file, customdataSizeSize))
        {
            return false;
        }

        settings.setCustomDataSizeSize(customdataSizeSize);

        uint8 stringSizeSize = 0;
        if (!iaSerializable::readUInt8(file, stringSizeSize))
        {
            return false;
        }

        settings.setStringSizeSize(stringSizeSize);

        con_debug_endl("read OMPF with following configuration" << endl << "MajorVersion:" << settings.getMajorVersion() << endl \
            << "MinorVersion:" << settings.getMinorVersion() << endl << "PatchVersion:" << settings.getPatchVersion() << endl \
            << "TypeIDSize:" << settings.getTypeIDSize() << endl << "ChunkIDSize:" << settings.getChunkIDSize() << endl \
            << "ChunkSizeSize:" << settings.getChunkSizeSize() << endl << "CustomDataSizeSize:" << settings.getCustomDataSizeSize() << endl \
            << "StringSizeSize:" << settings.getStringSizeSize());

        char buffer[6];
        iaSerializable::read(file, buffer, 6);

        return ompfBaseChunk::read(file, settings);
    }

    uint32 ompfHeaderChunk::getSize(const ompfSettings& settings)
    {
        return 16 + ompfBaseChunk::getSize(settings);
	}

}

