// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaFile.h>

#include <iaux/system/iaConsole.h>
#include <iaux/system/iaDirectory.h>

#include <filesystem>

#ifdef __IGOR_WINDOWS__
#include <io.h>
#endif

#ifdef __IGOR_LINUX__
#include <unistd.h>
#endif

namespace iaux
{

    iaFile::iaFile(const iaString &filename)
    {
        _filename = iaDirectory::fixPath(filename, true);
    }

    iaFile::~iaFile()
    {
        close();
    }

    /*! \returns file size for given file handle
    \param fileHandle the given file handle
    */
    static int64 getFileSize(FILE *fileHandle)
    {
        int64 size = 0;
        fseek(fileHandle, 0, SEEK_END);
        fpos_t pos;
        fgetpos(fileHandle, &pos);
#ifdef __IGOR_WINDOWS__
        size = pos;
#endif
#ifdef __IGOR_LINUX__
        size = (int64)pos.__pos;
#endif
        return size;
    }

    bool iaFile::open(bool writeableMode)
    {
        if (_fileHandle != nullptr)
        {
            con_err("file is already open");
            return false;
        }

        _isWriteable = writeableMode;

        // if it does not exist and we don't want to write it abort
        if (!exist() && !_isWriteable)
        {
            return false;
        }

        char temp[1024];
        _filename.getData(temp, 1024);
        _fileHandle = fopen(temp, _isWriteable ? "rwb" : "rb");

        _size = getFileSize(_fileHandle);

        setFilePointer(0);

        return true;
    }

    void iaFile::rename(const iaString &newFileName, bool replaceExisting)
    {
        const iaString newName = iaDirectory::fixPath(newFileName, true);

        std::filesystem::path oldPath(_filename.getData());
        std::filesystem::path newPath(newName.getData());

        std::error_code error;
        std::filesystem::rename(oldPath, newPath, error);

        if (error)
        {
            con_err("cant rename file: " << getFullFileName() << " to: " << newName);
        }
    }

    iaFile iaFile::copy(const iaString &newFileName) const
    {
        const iaString newName = iaDirectory::fixPath(newFileName, true);

        std::filesystem::path fromPath(_filename.getData());
        std::filesystem::path toPath(newName.getData());

        std::error_code error;
        std::filesystem::copy(fromPath, toPath, error);

        if (error)
        {
            con_err("cant copy file: " << getFullFileName() << " to: " << newName);
        }

        return iaFile(newName);
    }

    bool iaFile::exist() const
    {
        return iaFile::exist(getFullFileName());
    }

    bool iaFile::exist(const iaString &filename)
    {
        std::filesystem::path path(filename.getData());
        return !std::filesystem::is_directory(path) && std::filesystem::exists(path);
    }

    iaString iaFile::getPath() const
    {
        int64 pos = _filename.findLastOf(__IGOR_PATHSEPARATOR__);

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
        int64 pos = _filename.findLastOf(__IGOR_PATHSEPARATOR__);

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

        int64 pos = stem.findLastOf('.');

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
        int64 pos = _filename.findLastOf('.');

        if (pos != iaString::INVALID_POSITION &&
            pos < _filename.getLength())
        {
            return _filename.getSubString(pos + 1, iaString::INVALID_POSITION);
        }

        return "";
    }

    void iaFile::close()
    {
        if (_fileHandle != nullptr)
        {
            if (fclose(_fileHandle) != 0)
            {
                con_err("cant close file: " << getFullFileName());
            }
            else
            {
                _fileHandle = nullptr;
            }
        }
    }

    bool iaFile::isOpen()
    {
        return (nullptr != _fileHandle) ? true : false;
    }

    bool iaFile::setFilePointer(int64 position)
    {
#ifdef __IGOR_WINDOWS__
        fpos_t pos = position;
#endif
#ifdef __IGOR_LINUX__
        fpos_t pos;
        pos.__pos = position;
#endif
        if (fsetpos(_fileHandle, &pos) != 0)
        {
            con_err("can't set file pointer: " << getFullFileName());
            return false;
        }

        return true;
    }

    bool iaFile::read(int32 size, char *destination, int64 offset)
    {
        if (offset != -1)
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
        }

        if (fread(destination, size, 1, _fileHandle) != 1)
        {
            con_err("Can't read from file: " << getFullFileName());
            return false;
        }

        return true;
    }

    bool iaFile::write(int32 size, const char *source, int64 offset)
    {
        if (offset != -1)
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
        }

        if (!_isWriteable && _fileHandle != nullptr)
        {
            con_err("file openend readonly!");
            return false;
        }

        size_t result = fwrite(source, size, 1, _fileHandle);
        if (result != size)
        {
            con_err("Can't write to file: " << getFullFileName());
            return false;
        }

        _size = getFileSize(_fileHandle);

        return true;
    }

    int64 iaFile::getSize()
    {
        return _size;
    }

    bool iaFile::setSize(int64 size)
    {
#ifdef __IGOR_WINDOWS__
        if (_chsize(fileno(_fileHandle), size) != 0)
        {
            con_err("cant change size of file: " << getFullFileName() << " to " << size);
            return false;
        }
#endif

#ifdef __IGOR_LINUX__
        if (ftruncate(fileno(_fileHandle), size) != 0)
        {
            con_err("cant change size of file: " << getFullFileName() << " to " << size);
            return false;
        }
#endif

        return true;
    }
} // namespace iaux
