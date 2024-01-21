// OMPF 3d model file format
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <ompf/chunks/ompfHeaderChunk.h>

#include <ompf/ompfVersion.h>
#include <ompf/ompfSettings.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <cstring>

namespace OMPF
{

    ompfHeaderChunk::ompfHeaderChunk()
        : ompfBaseChunk(OMPFChunkType::Header)
    {
    }

    bool ompfHeaderChunk::write(std::ofstream &stream, const ompfSettings &settings)
    {
        if (!iaSerializable::write(stream, "OMPF", 4))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(stream, settings.getMajorVersion()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(stream, settings.getMinorVersion()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(stream, settings.getPatchVersion()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(stream, settings.getTypeIDSize()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(stream, settings.getChunkIDSize()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(stream, settings.getChunkSizeSize()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(stream, settings.getCustomDataSizeSize()))
        {
            return false;
        }

        if (!iaSerializable::writeUInt8(stream, settings.getStringSizeSize()))
        {
            return false;
        }

        if (!iaSerializable::write(stream, "I\3Igor", 6)) // I?IGOR
        {
            return false;
        }

        con_trace("write OMPF with following configuration" << endl
                                                            << "MajorVersion:" << settings.getMajorVersion() << endl
                                                            << "MinorVersion:" << settings.getMinorVersion() << endl
                                                            << "PatchVersion:" << settings.getPatchVersion() << endl
                                                            << "TypeIDSize:" << settings.getTypeIDSize() << endl
                                                            << "ChunkIDSize:" << settings.getChunkIDSize() << endl
                                                            << "ChunkSizeSize:" << settings.getChunkSizeSize() << endl
                                                            << "CustomDataSizeSize:" << settings.getCustomDataSizeSize() << endl
                                                            << "StringSizeSize:" << settings.getStringSizeSize());

        return ompfBaseChunk::write(stream, settings);
    }

    bool ompfHeaderChunk::read(std::ifstream &stream, ompfSettings &settings)
    {
        char magicNumber[5];
#ifdef IGOR_DEBUG
        memset(magicNumber, '#', 5);
#endif
        iaSerializable::read(stream, magicNumber, 4);
        magicNumber[4] = 0;

        if (std::string(magicNumber) != "OMPF")
        {
            con_err("invalid magic number");
            return false;
        }

        uint8 majorVersion = 0;
        uint8 minorVersion = 0;
        uint8 patchVersion = 0;

        iaSerializable::readUInt8(stream, majorVersion);
        iaSerializable::readUInt8(stream, minorVersion);
        iaSerializable::readUInt8(stream, patchVersion);

        uint32 version = (static_cast<uint32>(majorVersion) << 16) | (static_cast<uint32>(minorVersion) << 8) | static_cast<uint32>(patchVersion);

        //! \todo check minor and patch too
        if (version > settings.getVersion())
        {
            con_err("Only versions up to " << OMPF_VERSION_MAJOR << "." << OMPF_VERSION_MINOR << "." << OMPF_VERSION_PATCH << " supported.");
            return false;
        }

        uint8 typeIDSize = 0;
        if (!iaSerializable::readUInt8(stream, typeIDSize))
        {
            return false;
        }

        settings.setTypeIDSize(typeIDSize);

        uint8 chunkIDSize = 0;
        if (!iaSerializable::readUInt8(stream, chunkIDSize))
        {
            return false;
        }

        settings.setChunkIDSize(chunkIDSize);

        uint8 chunkSizeSize = 0;
        if (!iaSerializable::readUInt8(stream, chunkSizeSize))
        {
            return false;
        }

        settings.setChunkSizeSize(chunkSizeSize);

        uint8 customdataSizeSize = 0;
        if (!iaSerializable::readUInt8(stream, customdataSizeSize))
        {
            return false;
        }

        settings.setCustomDataSizeSize(customdataSizeSize);

        uint8 stringSizeSize = 0;
        if (!iaSerializable::readUInt8(stream, stringSizeSize))
        {
            return false;
        }

        settings.setStringSizeSize(stringSizeSize);

        con_trace("read OMPF with following configuration");
        con_trace("MajorVersion: " << settings.getMajorVersion());
        con_trace("MinorVersion: " << settings.getMinorVersion());
        con_trace("PatchVersion: " << settings.getPatchVersion());
        con_trace("TypeIDSize: " << settings.getTypeIDSize());
        con_trace("ChunkIDSize: " << settings.getChunkIDSize());
        con_trace("ChunkSizeSize: " << settings.getChunkSizeSize());
        con_trace("CustomDataSizeSize: " << settings.getCustomDataSizeSize());
        con_trace("StringSizeSize: " << settings.getStringSizeSize());

        char buffer[6];
        iaSerializable::read(stream, buffer, 6);

        return ompfBaseChunk::read(stream, settings);
    }

    uint32 ompfHeaderChunk::getSize(const ompfSettings &settings)
    {
        return 16 + ompfBaseChunk::getSize(settings);
    }

} // namespace OMPF
