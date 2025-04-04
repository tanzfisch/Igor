// OMPF 3d model file format
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <ompf/ompfSettings.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace OMPF
{

	uint8 ompfSettings::getCustomDataSizeSize() const
	{
		return _customDataSizeSize;
	}

	uint8 ompfSettings::getStringSizeSize() const
	{
		return _stringSizeSize;
	}

	bool ompfSettings::setCustomDataSizeSize(uint8 size)
	{
		if (size == 2)
		{
			_customDataSizeSize = size;
			return true;
		}
		else
		{
			con_err("unsupported custom data size size");
			return false;
		}
	}

	bool ompfSettings::setStringSizeSize(uint8 size)
	{
		if (size == 2)
		{
			_stringSizeSize = size;
			return true;
		}
		else
		{
			con_err("unsupported string size size");
			return false;
		}
	}

	uint8 ompfSettings::getMajorVersion() const
	{
		return _majorVersion;
	}

	uint8 ompfSettings::getMinorVersion() const
	{
		return _minorVersion;
	}

	uint8 ompfSettings::getPatchVersion() const
	{
		return _patchVersion;
	}

	uint32 ompfSettings::getVersion() const
	{
		return (static_cast<uint32>(_majorVersion) << 16) | (static_cast<uint32>(_minorVersion) << 8) | static_cast<uint32>(_patchVersion);
	}

	uint8 ompfSettings::getTypeIDSize() const
	{
		return _typeIDSize;
	}

	uint8 ompfSettings::getChunkIDSize() const
	{
		return _chunkIDSize;
	}

	uint8 ompfSettings::getChunkSizeSize() const
	{
		return _chunkSizeSize;
	}

	bool ompfSettings::setTypeIDSize(uint8 typeIDSize)
	{
		if (typeIDSize == 1)
		{
			_typeIDSize = typeIDSize;
			return true;
		}
		else
		{
			con_err("unsupported type id size");
			return false;
		}
	}

	bool ompfSettings::setChunkIDSize(uint8 chunkIDSize)
	{
		if (chunkIDSize == 4)
		{
			_chunkIDSize = chunkIDSize;
			return true;
		}
		else
		{
			con_err("unsupported chunk id size");
			return false;
		}
	}

	bool ompfSettings::setChunkSizeSize(uint8 chunkSizeSize)
	{
		if (_chunkSizeSize == 4)
		{
			_chunkSizeSize = chunkSizeSize;
			return true;
		}
		else
		{
			con_err("unsupported chunk id size");
			return false;
		}
	}
} // namespace OMPF