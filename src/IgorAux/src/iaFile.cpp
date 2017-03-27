// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iaFile.h>

#ifdef __IGOR_WIN__
#include <iaConsole.h>
#include <iaDirectory.h>

namespace IgorAux
{

	iaFile::iaFile(const iaString & fileName)
	{
		_filename = iaDirectory::fixPath(fileName, true);
	}

	iaFile::~iaFile()
	{
		close();
	}

	bool iaFile::open(bool writeableMode)
	{
		if(_windowsFileHandle)
		{
			con_err("file is already open");
			return false;
		}

		_isWriteable = writeableMode;

		// open mode
		DWORD accessMode = (_isWriteable) ? (GENERIC_WRITE | GENERIC_READ) : (GENERIC_READ);
		DWORD openMode = OPEN_EXISTING;
		DWORD shareMode = 0; //! \todo: FILE_SHARE_READ;

		// if it does not exist, create new one
		if(!exist())
		{
            if (_isWriteable)
            {
                // file does not exist, so create new file
                con_warn("file does not exist: " << getFullFileName() << ". creating new one.");
                openMode = CREATE_NEW;
            }
            else
            {
                return false;
            }
		}

		// open the file
		_windowsFileHandle = CreateFile(getFullFileName().getData(), accessMode, shareMode, 0, openMode, FILE_ATTRIBUTE_NORMAL, 0);
		if(_windowsFileHandle == INVALID_HANDLE_VALUE)
		{
			con_err_win("cannot open/create file: " << getFullFileName());
			_windowsFileHandle = 0;
			return false;
		}

		// get the 64bit size
		DWORD sizeHigh = 0;
		const DWORD sizeLow = GetFileSize(_windowsFileHandle, &sizeHigh);
		if(sizeLow == INVALID_FILE_SIZE)
		{
			// must not be an error
			const DWORD error = GetLastError();
			if(error !=	NO_ERROR)
			{
				con_err_win("cannot get file size: " << getFullFileName());
				return false;
			}
		}
		else
		{
			_size = sizeLow + ((uint64)sizeHigh) * ((uint64)((uint32) - 1));
		}

		return true;
	}

	void iaFile::rename(const iaString & newFileName, bool replaceExisting)
	{
		const iaString newName = iaDirectory::fixPath(newFileName, true);

        if (!MoveFileEx(getFullFileName().getData(), newName.getData(), (replaceExisting) ? MOVEFILE_REPLACE_EXISTING : 0))
		{
			con_err_win("cant rename file: " << getFullFileName() << " to: " << newName);
		}
	}

    iaFile iaFile::copy(const iaString & newFileName) const
	{
		const iaString newName = iaDirectory::fixPath(newFileName, true);

		if(!CopyFile(getFullFileName().getData(), newName.getData(), false))
		{
			con_err_win("cant copy file: " << getFullFileName() << " to: " << newName);
		}

		return iaFile(newName);
	}

	bool iaFile::exist() const
	{
		return iaFile::exist(getFullFileName());
	}

	bool iaFile::exist(const iaString & filename)
	{
		WIN32_FIND_DATAW findData;
		HANDLE findHandle = FindFirstFile(filename.getData(), &findData);

		// file does not exist
		if(findHandle == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			return false;
		}

		FindClose(findHandle);
		return true;
	}

    iaString iaFile::getPath() const
    {
        uint64 pos = _filename.findLastOf(iaDirectory::getPathSeperator());

        if (pos != iaString::INVALID_POSITION)
        {
            return _filename.getSubString(0, pos+1);
        }
        else
        {
            return _filename;
        }
    }

	iaString iaFile::getFileName() const
	{
        uint64 pos = _filename.findLastOf(iaDirectory::getPathSeperator());

        if (pos != iaString::INVALID_POSITION)
        {
            return _filename.getSubString(pos + 1, iaString::INVALID_POSITION);
        }
        else
        {
            return "";
        }
	}

	iaString iaFile::getFullFileName() const
	{
		iaString name;

		name += getPath();
		name += getFileName();

		return name;
	}

	iaString iaFile::getExtension() const
	{
		return _filename.getSubString(_filename.findLastOf('.') + 1, iaString::INVALID_POSITION);
	}

	void iaFile::close()
	{
		if(_windowsFileHandle)
		{
            if (!CloseHandle(_windowsFileHandle))
            {
                con_err("cant close file: " << getFullFileName());
            }
			_windowsFileHandle = 0;
		}
	}

    bool iaFile::isOpen()
    {
        return (nullptr != _windowsFileHandle) ? true : false;
    }

    bool iaFile::setFilePointer(uint64 position)
	{
        _LARGE_INTEGER largeInt;
        largeInt.QuadPart = position;

		// set the file pointer
        if (SetFilePointerEx(_windowsFileHandle, largeInt, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			// must not be an error
			const DWORD error = GetLastError();
			if(error !=	NO_ERROR)
			{
				con_err("setting file pointer: " << getFullFileName());
				return false;
			}
		}

		return true;
	}

	bool iaFile::readFromFile(uint64 offset, int32 size, char * destination, bool asynchron)
	{
        if (!(offset >= 0 && size > 0))
        {
            return false;
        }

		// set position inside file
        if (!setFilePointer(offset))
        {
            return false;
        }

		// synchron / asynchron read
		DWORD bytesRead = 0;
		if(!asynchron)
		{
			if (!ReadFile(_windowsFileHandle, destination, (DWORD) size, & bytesRead, 0))
			{
				con_err("read from file: " << getFullFileName());
				return false;
			}
		}
		else
		{
			if (!ReadFileEx(_windowsFileHandle, destination, (DWORD) size, 0, (LPOVERLAPPED_COMPLETION_ROUTINE) readCompletionCallback))
			{
                con_err("read from file: " << getFullFileName());
				return false;
			}
		}

		return true;
	}

	bool iaFile::writeToFile(uint64 offset, int32 size, const char * source, bool asynchron)
	{
        if (!(offset >= 0 && size > 0))
        {
            return false;
        }

		// set position inside file
        if (!setFilePointer(offset))
        {
            return false;
        }

		if(!_isWriteable && _windowsFileHandle)
		{
			con_err("file openend readonly!");
			return false;
		}

		// synchron / asynchron write
		DWORD bytesWriten = 0;
		if(!asynchron)
		{
			if (!WriteFile(_windowsFileHandle, source, (DWORD) size, &bytesWriten, 0))
			{
				con_err("write to file: " << getFullFileName());
				return false;
			}
		}
		else
		{
			if (!WriteFileEx(_windowsFileHandle, source, (DWORD) size, 0, (LPOVERLAPPED_COMPLETION_ROUTINE) writeCompletionCallback))
			{
				con_err("write to file: " << getFullFileName());
				return false;
			}
		}

		return true;
	}

	int64 iaFile::getFileSize()
	{
		return _size;
	}

	bool iaFile::setFileSize(int64 newSize)
	{
		if(setFilePointer(newSize))
		{
			if(!SetEndOfFile(_windowsFileHandle))
			{
				con_err_win("cant change size of file: " << getFullFileName());
				return false;
			}
            else
            {
                return true;
            }
		}
        else
        {
            return false;
        }
	}

#pragma warning(disable:4100)
    void __stdcall iaFile::writeCompletionCallback(uint32 errorCode, uint32 size, void* overlapped)
	{
        con_err("not implemented");
		//! \todo: maybe pass some function pointer to execute after read/write
	}
#pragma warning(default:4100)

#pragma warning(disable:4100)
    void __stdcall iaFile::readCompletionCallback(uint32 errorCode, uint32 size, void* overlapped)
    {
        con_err("not implemented");
        //! \todo: maybe pass some function pointer to execute after read/write
    }
#pragma warning(default:4100)
}

#endif