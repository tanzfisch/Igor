// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaFile.h>

#include <iaux/system/iaConsole.h>
#include <iaux/system/iaPath.h>

#include <filesystem>

#ifdef IGOR_WINDOWS
#include <io.h>
#endif

#ifdef IGOR_LINUX
#include <unistd.h>
#endif

namespace iaux
{

    iaFile::iaFile(const iaString &filename)
    {
        con_assert(!filename.isEmpty(), "can't use empty filename");
        _filename = filename;
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
#ifdef IGOR_WINDOWS
        size = pos;
#endif
#ifdef IGOR_LINUX
        size = (int64)pos.__pos;
#endif
        return size;
    }

    static const char *getOpenFlags(const iaFileOpenMode &mode)
    {
        const static std::string text[] = {
            "rb",
            "wb",
            "rwb"};

        return text[static_cast<int>(mode)].c_str();
    }

    static bool isWriteable(const iaFileOpenMode &mode)
    {
        return mode == iaFileOpenMode::WriteBinary || mode == iaFileOpenMode::ReadWriteBinary;
    }

    bool iaFile::open(const iaFileOpenMode &mode)
    {
        if (_fileHandle != nullptr)
        {
            con_err("file is already open");
            return false;
        }

        _mode = mode;

        // if it does not exist and we don't want to write it abort
        if (!iaPath::exists(_filename) && !isWriteable(_mode))
        {
            return false;
        }

        char temp[1024];
        _filename.getData(temp, 1024);
        _fileHandle = fopen(temp, getOpenFlags(_mode));
        _size = getFileSize(_fileHandle);

        setFilePointer(0);

        return true;
    }

    iaTime iaFile::getLastModifiedTime() const
    {
        return iaPath::getLastModifiedTime(_filename);
    }

    const iaString iaFile::getFullFileName() const
    {
        return _filename;
    }

    const iaString iaFile::getParentPath() const
    {
        return iaPath::getParentPath(_filename);
    }

    const iaString iaFile::getName() const
    {
        return iaPath::getName(_filename);
    }

    const iaString iaFile::getStem() const
    {
        return iaPath::getStem(_filename);
    }

    const iaString iaFile::getExtension() const
    {
        return iaPath::getExtension(_filename);
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

    bool iaFile::isOpen() const
    {
        return (nullptr != _fileHandle) ? true : false;
    }

    const iaFileOpenMode &iaFile::getFileOpenMode() const
    {
        con_assert_sticky(isOpen(), "file is not open");

        return _mode;
    }

    bool iaFile::setFilePointer(int64 position)
    {
#ifdef IGOR_WINDOWS
        fpos_t pos = position;
#endif
#ifdef IGOR_LINUX
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

        if (!isWriteable(_mode) && _fileHandle != nullptr)
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

    int64 iaFile::getSize() const
    {
        return _size;
    }

    bool iaFile::setSize(int64 size)
    {
#ifdef IGOR_WINDOWS
        if (_chsize(fileno(_fileHandle), size) != 0)
        {
            con_err("cant change size of file: " << getFullFileName() << " to " << size);
            return false;
        }
#endif

#ifdef IGOR_LINUX
        if (ftruncate(fileno(_fileHandle), size) != 0)
        {
            con_err("cant change size of file: " << getFullFileName() << " to " << size);
            return false;
        }
#endif

        return true;
    }

    iaString toString(const iaFileOpenMode &value)
    {
        const static iaString text[] = {
            L"read",
            L"write",
            L"read&write"};

        return text[static_cast<int>(value)];
    }

    std::wostream &operator<<(std::wostream &stream, const iaFileOpenMode &mode)
    {
        stream << toString(mode);
        return stream;
    }

} // namespace iaux
