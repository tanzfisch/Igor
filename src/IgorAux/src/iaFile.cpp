// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iaFile.h>

#include <iaConsole.h>
#include <iaDirectory.h>

namespace IgorAux
{

    iaFile::iaFile(const iaString &fileName)
    {
        _filename = iaDirectory::fixPath(fileName, true);
    }

    iaFile::~iaFile()
    {
        close();
    }

    bool iaFile::open(bool writeableMode)
    {
        if (_fileHandle != nullptr)
        {
            con_err("file is already open");
            return false;
        }

        _isWriteable = writeableMode;

#ifdef __IGOR_WINDOWS__
        // open mode
        DWORD accessMode = (_isWriteable) ? (GENERIC_WRITE | GENERIC_READ) : (GENERIC_READ);
        DWORD openMode = OPEN_EXISTING;
        DWORD shareMode = 0; //! \todo: FILE_SHARE_READ;

        // if it does not exist, create new one
        if (!exist())
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
        _fileHandle = CreateFile(getFullFileName().getData(), accessMode, shareMode, 0, openMode, FILE_ATTRIBUTE_NORMAL, 0);
        if (_fileHandle == INVALID_HANDLE_VALUE)
        {
            con_err("cannot open/create file: " << getFullFileName());
            _fileHandle = 0;
            return false;
        }

        // get the 64bit size
        DWORD sizeHigh = 0;
        const DWORD sizeLow = GetFileSize(_fileHandle, &sizeHigh);
        if (sizeLow == INVALID_FILE_SIZE)
        {
            // must not be an error
            const DWORD error = GetLastError();
            if (error != NO_ERROR)
            {
                con_err("cannot get file size: " << getFullFileName());
                return false;
            }
        }
        else
        {
            _size = sizeLow + ((uint64)sizeHigh) * ((uint64)((uint32)-1));
        }
#endif

#ifdef __IGOR_LINUX__
        // TODO
        return false;
#endif

        return true;
    }

    void iaFile::rename(const iaString &newFileName, bool replaceExisting)
    {
        const iaString newName = iaDirectory::fixPath(newFileName, true);

#ifdef __IGOR_WINDOWS__
        if (!MoveFileEx(getFullFileName().getData(), newName.getData(), (replaceExisting) ? MOVEFILE_REPLACE_EXISTING : 0))
        {
            con_err("cant rename file: " << getFullFileName() << " to: " << newName);
        }
#endif

#ifdef __IGOR_LINUX__
        // TODO
#endif
    }

    iaFile iaFile::copy(const iaString &newFileName) const
    {
        const iaString newName = iaDirectory::fixPath(newFileName, true);

#ifdef __IGOR_WINDOWS__
        if (!CopyFile(getFullFileName().getData(), newName.getData(), false))
        {
            con_err("cant copy file: " << getFullFileName() << " to: " << newName);
        }
#endif

#ifdef __IGOR_LINUX__
        // TODO
#endif

        return iaFile(newName);
    }

    bool iaFile::exist() const
    {
        return iaFile::exist(getFullFileName());
    }

    bool iaFile::exist(const iaString &filename)
    {
#ifdef __IGOR_WINDOWS__
        WIN32_FIND_DATAW findData;
        HANDLE findHandle = FindFirstFile(filename.getData(), &findData);

        // file does not exist
        if (findHandle == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            return false;
        }

        FindClose(findHandle);
        return true;
#endif

#ifdef __IGOR_LINUX__
        // TODO
        return false;
#endif
    }

    iaString iaFile::getPath() const
    {
        uint64 pos = _filename.findLastOf(iaDirectory::getPathSeperator());

        if (pos != iaString::INVALID_POSITION)
        {
            return _filename.getSubString(0, pos + 1);
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

    iaString iaFile::getStem() const
    {
        iaString stem = getFileName();

        uint64 pos = stem.findLastOf('.');

        if (pos != iaString::INVALID_POSITION &&
            pos > 0)
        {
            return stem.getSubString(0, pos);
        }

        return stem;
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
        uint64 pos = _filename.findLastOf('.');

        if (pos != iaString::INVALID_POSITION &&
            pos < _filename.getLength())
        {
            return _filename.getSubString(pos + 1, iaString::INVALID_POSITION);
        }

        return "";
    }

    void iaFile::close()
    {
        if (_fileHandle)
        {
#ifdef __IGOR_WINDOWS__
            if (!CloseHandle(_fileHandle))
            {
                con_err("cant close file: " << getFullFileName());
            }
#endif

#ifdef __IGOR_LINUX__
            // TODO
#endif
            _fileHandle = nullptr;
        }
    }

    bool iaFile::isOpen()
    {
        return (nullptr != _fileHandle) ? true : false;
    }

    bool iaFile::setFilePointer(uint64 position)
    {
#ifdef __IGOR_WINDOWS__
        _LARGE_INTEGER largeInt;
        largeInt.QuadPart = position;

        // set the file pointer
        if (SetFilePointerEx(_fileHandle, largeInt, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {
            // must not be an error
            const DWORD error = GetLastError();
            if (error != NO_ERROR)
            {
                con_err("setting file pointer: " << getFullFileName());
                return false;
            }
        }
#endif

#ifdef __IGOR_LINUX__
        // TODO
        return false;
#endif

        return true;
    }

    bool iaFile::readFromFile(uint64 offset, int32 size, char *destination)
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

        // read
#ifdef __IGOR_WINDOWS__
        DWORD bytesRead = 0;
        if (!ReadFile(_fileHandle, destination, (DWORD)size, &bytesRead, 0))
        {
            con_err("read from file: " << getFullFileName());
            return false;
        }
#endif

#ifdef __IGOR_LINUX__
        // TODO
        return false;
#endif

        return true;
    }

    bool iaFile::writeToFile(uint64 offset, int32 size, const char *source)
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

        if (!_isWriteable && _fileHandle)
        {
            con_err("file openend readonly!");
            return false;
        }

        // write
#ifdef __IGOR_WINDOWS__
        DWORD bytesWriten = 0;
        if (!WriteFile(_fileHandle, source, (DWORD)size, &bytesWriten, 0))
        {
            con_err("write to file: " << getFullFileName());
            return false;
        }
#endif

#ifdef __IGOR_LINUX__
        // TODO
        return false;
#endif

        return true;
    }

    int64 iaFile::getFileSize()
    {
        return _size;
    }

    bool iaFile::setFileSize(int64 newSize)
    {
        if (setFilePointer(newSize))
        {
#ifdef __IGOR_WINDOWS__
            if (!SetEndOfFile(_fileHandle))
            {
                con_err("cant change size of file: " << getFullFileName());
                return false;
            }
            else
            {
                return true;
            }
#endif

#ifdef __IGOR_LINUX__
            // TODO
            return false;
#endif
        }
        else
        {
            return false;
        }
    }
} // namespace IgorAux
