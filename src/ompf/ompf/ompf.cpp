// OMPF 3d model file format
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <ompf/ompf.h>

#include <iaux/system/iaConsole.h>
#include <iaux/system/iaDirectory.h>
using namespace iaux;

#include <iostream>

#ifdef IGOR_WINDOWS
#include <windows.h>

//! gets called when the dll is loaded into memory
#pragma warning(disable : 4100)
bool WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH: //!< when loaded
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH: //!< when unloaded
        break;

    default:
        break;
    }

    return true;
}
#pragma warning(default : 4100)
#endif
namespace OMPF
{

    OMPF::OMPF()
    {
        init();
    }

    OMPF::~OMPF()
    {
        deInit();
    }

    void OMPF::init()
    {
        resetChunkID();

        _root = new ompfHeaderChunk();
        _root->setID(OMPFDefaultConfiguration::INVALID_CHUNK_ID);
        _chunks[_root->getID()] = _root;
    }

    void OMPF::deInit()
    {
        clearChunks();

        _root = nullptr; // was deleted within clearChunks
    }

    void OMPF::reset()
    {
        deInit();
        init();
    }

    void OMPF::resetChunkID()
    {
        _nextChunkID = OMPFDefaultConfiguration::INVALID_CHUNK_ID;
    }

    uint32 OMPF::getNextChunkID()
    {
        return ++_nextChunkID;
    }

    void OMPF::clearChunks()
    {
        for (const auto &pair : _chunks)
        {
            delete pair.second;
        }
        _chunks.clear();

        for (const auto material : _materialReferenceChunks)
        {
            delete material;
        }
        _materialReferenceChunks.clear();
    }

    ompfGroupChunk *OMPF::createGroupChunk()
    {
        ompfGroupChunk *result = new ompfGroupChunk();
        result->setID(getNextChunkID());
        _chunks[result->getID()] = result;
        return result;
    }

    ompfTransformChunk *OMPF::createTransformChunk()
    {
        ompfTransformChunk *result = new ompfTransformChunk();
        result->setID(getNextChunkID());
        _chunks[result->getID()] = result;
        return result;
    }

    ompfMaterialReferenceChunk *OMPF::createMaterialReferenceChunk()
    {
        ompfMaterialReferenceChunk *result = new ompfMaterialReferenceChunk();
        result->setID(getNextChunkID());
        _materialReferenceChunks.push_back(result);
        return result;
    }

    ompfMeshChunk *OMPF::createMeshChunk()
    {
        ompfMeshChunk *result = new ompfMeshChunk();
        result->setID(getNextChunkID());
        _chunks[result->getID()] = result;
        return result;
    }

    ompfEmitterChunk *OMPF::createEmitterChunk()
    {
        ompfEmitterChunk *result = new ompfEmitterChunk();
        result->setID(getNextChunkID());
        _chunks[result->getID()] = result;
        return result;
    }

    ompfParticleSystemChunk *OMPF::createParticleSystemChunk()
    {
        ompfParticleSystemChunk *result = new ompfParticleSystemChunk();
        result->setID(getNextChunkID());
        _chunks[result->getID()] = result;
        return result;
    }

    ompfExternalReferenceChunk *OMPF::createExternalReferenceChunk()
    {
        ompfExternalReferenceChunk *result = new ompfExternalReferenceChunk();
        result->setID(getNextChunkID());
        _chunks[result->getID()] = result;
        return result;
    }

    ompfHeaderChunk *OMPF::getRoot()
    {
        return _root;
    }

    ompfBaseChunk *OMPF::getChunk(uint32 chunkID)
    {
        return _chunks[chunkID];
    }

    bool OMPF::getNextChunk(std::ifstream &file, uint32 &typeID, uint32 &chunkSize)
    {
        std::streampos pos = file.tellg();

        uint64 value = 0;
        if (!iaSerializable::readUInt(file, value, _settings.getTypeIDSize()))
        {
            return false;
        }
        typeID = static_cast<uint32>(value);

        if (!iaSerializable::readUInt(file, value, _settings.getChunkSizeSize()))
        {
            return false;
        }
        chunkSize = static_cast<uint32>(value);

        file.seekg(pos);

        return true;
    }

    bool OMPF::analyze(std::ifstream &file)
    {
        if (!_root->read(file, _settings))
        {
            return false;
        }

        OMPFChunkType typeID;
        uint32 chunkSize;
        uint32 tempType;
        ompfBaseChunk *chunk = nullptr;

        while (!file.eof())
        {
            chunk = nullptr;

            if (!getNextChunk(file, tempType, chunkSize))
            {
                return true;
            }

            typeID = static_cast<OMPFChunkType>(tempType);
            bool result = true;

            switch (typeID)
            {
            case OMPFChunkType::Header:
                con_err("can't have second header chunk");
                return false;

            case OMPFChunkType::Transform:
                chunk = new ompfTransformChunk();
                result = chunk->read(file, _settings);
                break;

            case OMPFChunkType::External:
                chunk = new ompfExternalReferenceChunk();
                result = chunk->read(file, _settings);
                break;

            case OMPFChunkType::Group:
                chunk = new ompfGroupChunk();
                result = chunk->read(file, _settings);
                break;

            case OMPFChunkType::Mesh:
                chunk = new ompfMeshChunk();
                result = chunk->read(file, _settings);
                break;

            case OMPFChunkType::Emitter:
                chunk = new ompfEmitterChunk();
                result = chunk->read(file, _settings);
                break;

            case OMPFChunkType::ParticleSystem:
                chunk = new ompfParticleSystemChunk();
                result = chunk->read(file, _settings);
                break;

            case OMPFChunkType::MaterialReference:
                chunk = new ompfMaterialReferenceChunk();
                result = chunk->read(file, _settings);
                _materialReferenceChunks.push_back(static_cast<ompfMaterialReferenceChunk *>(chunk));

                chunk = nullptr;
                break;

            default:
                file.seekg(chunkSize, file.cur);
                con_warn("skipped unknown chunk with TypeID 0x" << std::hex << static_cast<int>(typeID) << " and size " << std::dec << chunkSize << " byte");
                break;
            }

            if (result)
            {
                if (chunk != nullptr)
                {
                    _chunks[static_cast<uint32>(chunk->getID())] = chunk;
                    if (static_cast<uint32>(chunk->getID()) > _nextChunkID)
                    {
                        _nextChunkID = static_cast<uint32>(chunk->getID());
                    }
                }
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    void OMPF::loadFile(iaString filename)
    {
        iaDirectory dir(filename);
        _filepath = dir.getFullDirectoryName();

        reset();

        std::ifstream file;
        char cfilename[1024];
        filename.getData(cfilename, 1024);

        file.open(cfilename, std::ios_base::in | std::ios_base::binary);

        con_trace("reading OMPF file: " << filename);

        if (!file.fail())
        {
            if (!analyze(file))
            {
                con_err("file " << filename << " seems currupted");
            }

            file.close();

            auto iter = _chunks.begin();
            while (iter != _chunks.end())
            {
                // skip header chunk. has no ID
                if ((*iter).second->getID() != OMPFDefaultConfiguration::INVALID_CHUNK_ID)
                {
                    if (_chunks[(*iter).second->getParentID()] != nullptr)
                    {
                        _chunks[(*iter).second->getParentID()]->insertChunk((*iter).second);
                    }
                    else
                    {
                        con_warn("inconsistant data");
                    }
                }

                iter++;
            }

            if (_root->getChildren().size() == 0)
            {
                con_err("no valid root of tree detected");
            }
        }
        else
        {
            con_err("can't open file to read " << filename);
        }
    }

    const std::vector<ompfMaterialReferenceChunk *> &OMPF::getMaterialReferenceChunks() const
    {
        return _materialReferenceChunks;
    }

    void OMPF::saveFile(iaString filename)
    {
        iaDirectory dir(filename);
        _filepath = dir.getFullParentDirectoryName();

        con_assert(_root != nullptr, "can never be zero");

        std::ofstream outfile;
        char cfilename[1024];
        filename.getData(cfilename, 1024);

        outfile.open(cfilename, std::ios_base::out | std::ios_base::binary);
        if (!outfile.fail())
        {
            write(outfile, _root, nullptr);
            writeMaterials(outfile);
            outfile.close();
        }
        else
        {
            con_err("can't open file to write " << filename);
        }
    }

    const iaString &OMPF::getFileDirectory() const
    {
        return _filepath;
    }

    void OMPF::writeMaterials(std::ofstream &outfile)
    {
        for (const auto material : _materialReferenceChunks)
        {
            material->write(outfile, _settings);
        }
    }

    void OMPF::write(std::ofstream &outfile, ompfBaseChunk *currentChunk, ompfBaseChunk *parentChunk)
    {
        if (parentChunk != nullptr)
        {
            currentChunk->setParentID(parentChunk->getID());
        }
        currentChunk->write(outfile, _settings);

        const std::vector<ompfBaseChunk *> &children = currentChunk->getChildren();
        std::vector<ompfBaseChunk *>::const_iterator iter = children.begin();

        while (iter != children.end())
        {
            write(outfile, (*iter), currentChunk);
            iter++;
        }
    }

} // namespace OMPF
