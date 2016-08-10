// OMPF 3d model file format
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <ompfBaseChunk.h>
#include <ompfVersion.h>
#include <iaConsole.h>
#include <OMPF.h>
#include <ompfSettings.h>

namespace OMPF
{
	ompfBaseChunk::ompfBaseChunk(OMPFChunkType typeID)
	{
        con_assert(OMPFChunkType::Invalid != typeID, "can't use invalid type id");

		_type = typeID;
	}

	void ompfBaseChunk::insertChunk(ompfBaseChunk* chunk)
	{
        con_assert(chunk->getType() != OMPFChunkType::Material, "can't add material chunk as child");
        con_assert(chunk != nullptr, "zero pointer");

		if(chunk != nullptr)
		{
            for (uint32 i = 0; i < _children.size(); ++i)
            {
                if (_children[i] == chunk)
                {
                    con_err("chunk already added");
                    return;
                }
            }

			_children.push_back(chunk);
			chunk->_parentID = getID();
		}
        else
        {
            con_err("zero pointer");
        }
	}

    OMPFChunkType ompfBaseChunk::getType() const
    {
        return _type;
    }

    void ompfBaseChunk::setParentID(const uint32 parentID)
    {
        _parentID = parentID;
    }

    void ompfBaseChunk::setID(const uint32 chunkID)
    {
        _ID = chunkID;
    }

	uint32 ompfBaseChunk::getID() const
	{
		return _ID;
	}
	
	void ompfBaseChunk::removeChunk(ompfBaseChunk* chunk)
	{
        con_assert(chunk != nullptr, "zero pointer");

        if (chunk != nullptr)
		{
            vector<ompfBaseChunk*>::iterator it = _children.begin();

			while(it != _children.end())
			{
				if((*it) == chunk)
				{
					_children.erase(it);
					(*it)->_parentID = OMPFDefaultConfiguration::InvalidChunkID;
					return;
				}

                it++;
			}
		}
        else
        {
            con_err("zero pointer");
        }
	}

	bool ompfBaseChunk::hasChildren() const
	{
		return _children.size() ? true : false;
	}

	const vector<ompfBaseChunk*>& ompfBaseChunk::getChildren() const
	{
		return _children;
	}

    bool ompfBaseChunk::hasParent() const
    {
        return (_parentID != OMPFDefaultConfiguration::InvalidChunkID) ? true : false;
    }

    uint32 ompfBaseChunk::getParentID() const
    {
        return _parentID;
    }

	uint32 ompfBaseChunk::getSize(const ompfSettings& settings)
	{
		return static_cast<uint32>(settings.getTypeIDSize()) + static_cast<uint32>(settings.getChunkIDSize() * 2) + static_cast<uint32>(settings.getChunkSizeSize()) + static_cast<uint32>(_name.getUTF8Size() + 2);
	}

    void ompfBaseChunk::setName(const iaString& name)
    {
        _name = name;
    }

    iaString ompfBaseChunk::getName() const
    {
        return _name;
    }

    bool ompfBaseChunk::write(ofstream& file, const ompfSettings& settings)
    {
        _chunkSize = getSize(settings);

        uint64 value = static_cast<uint64>(_type);
        if (!iaSerializable::writeUInt(file, value, settings.getTypeIDSize()))
        {
            return false;
        }

        value = static_cast<uint64>(_chunkSize);
        if (!iaSerializable::writeUInt(file, value, settings.getChunkSizeSize()))
        {
            return false;
        }

        value = static_cast<uint64>(_ID);
        if (!iaSerializable::writeUInt(file, value, settings.getChunkIDSize()))
        {
            return false;
        }

        value = static_cast<uint64>(_parentID);
        if (!iaSerializable::writeUInt(file, value, settings.getChunkIDSize()))
        {
            return false;
        }
        
        if (!iaSerializable::writeUTF8(file, _name))
        {
            return false;
        }

        con_debug_endl("writing chunk type:0x" << hex << static_cast<uint64>(_type) << dec << " size:" << _chunkSize << " ID:" << _ID << " parent:" << _parentID);

        return true;
    }

    bool ompfBaseChunk::read(ifstream& file, ompfSettings& settings)
    {
        uint64 value = 0;
        if (!iaSerializable::readUInt(file, value, settings.getTypeIDSize()))
        {
            return false;
        }
        _type = static_cast<OMPFChunkType>(value);

#ifdef __IGOR_DEBUG__
        value = 0;
#endif
        if (!iaSerializable::readUInt(file, value, settings.getChunkSizeSize()))
        {
            return false;
        }
        _chunkSize = static_cast<uint32>(value);

#ifdef __IGOR_DEBUG__
        value = 0;
#endif
        if (!iaSerializable::readUInt(file, value, settings.getChunkIDSize()))
        {
            return false;
        }
        _ID = static_cast<uint32>(value);

#ifdef __IGOR_DEBUG__
        value = 0;
#endif
        if (!iaSerializable::readUInt(file, value, settings.getChunkIDSize()))
        {
            return false;
        }
        _parentID = static_cast<uint32>(value);

        if (!iaSerializable::readUTF8(file, _name))
        {
            return false;
        }
        
        con_debug_endl("reading chunk type: 0x" << hex << static_cast<uint32>(_type) << dec << " size:" << _chunkSize << " ID:" << _ID << " parent: " << _parentID << " name:" << _name);
        
        return true;
    }

    wostream& operator<<(wostream& stream, const OMPFChunkType chunkType)
    {
        static iaString text[] = {
            "Invalid",
            "Header",
            "Group",
            "Transform",
            "External",
            "ResourceSearchPath",
            "Mesh",
            "Material",
            "Emitter",
            "ParticleSystem"
        };

        con_assert(static_cast<int>(chunkType) >= 0 && static_cast<int>(chunkType) < 10, "out of range");

        stream << text[static_cast<int>(chunkType)].getData();
        return stream;
    }

    wostream& operator<<(wostream& stream, const OMPFPathType pathType)
    {
        static iaString text[] = {
            "RelativeToModel",
            "RelativeToApplication",
            "RelativeToCurrentDirectory",
            "Absolute",
            "Undefined"
        };

        con_assert(static_cast<int>(pathType) >= 0 && static_cast<int>(pathType) < 5, "out of range");

        stream << text[static_cast<int>(pathType)].getData();
        return stream;
    }

}