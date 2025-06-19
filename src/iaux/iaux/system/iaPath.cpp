// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaPath.h>

#include <iaux/system/iaFile.h>
#include <iaux/system/iaConsole.h>

#include <sstream>
#include <filesystem>
#include <algorithm>
#include <regex>

#ifdef IGOR_LINUX
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

namespace iaux
{
    std::wostream &operator<<(std::wostream &stream, const iaPath &dir)
    {
        const auto path = dir.getAbsolutePath();
        if (!path.isEmpty())
        {
            stream << path.getData();
        }

        return stream;
    }

    iaPath::iaPath()
    {
    }

    iaPath::iaPath(const iaString &path)
    {
        _path = path;
    }

    iaPath iaPath::operator=(const iaString &path)
    {
        _path = path;
        return *this;
    }

    bool iaPath::operator==(const iaString &other)
    {
        return _path == other;
    }

    bool iaPath::operator!=(const iaString &other)
    {
        return _path != other;
    }

    std::vector<iaPath> iaPath::getDirectories(bool recursive, bool orderAlphabetically) const
    {
        std::vector<iaPath> result;

        if (!isDirectory(_path))
        {
            return result;
        }

        if (recursive)
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(_path.getData()))
            {
                if (entry.is_directory())
                {
                    result.push_back(iaPath(entry.path().c_str()));
                }
            }
        }
        else
        {
            for (const auto &entry : std::filesystem::directory_iterator(_path.getData()))
            {
                if (entry.is_directory())
                {
                    result.push_back(iaPath(entry.path().c_str()));
                }
            }
        }

        if (orderAlphabetically)
        {
            std::sort(result.begin(), result.end(),
                      [](iaPath const a, iaPath const b)
                      {
                          const iaString sa = iaString::toLower(a.getAbsolutePath());
                          const iaString sb = iaString::toLower(b.getAbsolutePath());
                          return sa < sb;
                      });
        }

        return result;
    }

    bool iaPath::isFile(const iaString &path)
    {
        if (!exists(path))
        {
            return false;
        }

        std::filesystem::path fspath(path.getData());
        if (std::filesystem::is_regular_file(fspath) ||
            std::filesystem::is_symlink(fspath))
        {
            return true;
        }

        return false;
    }

    bool iaPath::isSymlink(const iaString &path)
    {
        if (!exists(path))
        {
            return false;
        }

        std::filesystem::path fspath(path.getData());
        if (std::filesystem::is_symlink(fspath))
        {
            return true;
        }

        return false;
    }

    bool iaPath::isDirectory(const iaString &path)
    {
        if (!exists(path))
        {
            return false;
        }

        std::filesystem::path fspath(path.getData());
        if (std::filesystem::is_directory(fspath))
        {
            return true;
        }

        return false;
    }

    bool iaPath::exists() const
    {
        return iaPath::exists(_path);
    }

    bool iaPath::exists(const iaString &path)
    {
        if (path.isEmpty())
        {
            return false;
        }

        std::filesystem::path fspath(path.getData());
        if (std::filesystem::exists(fspath))
        {
            return true;
        }

        const iaString fullDir = iaPath::getCurrentDirectory() + IGOR_PATHSEPARATOR + path;

        std::filesystem::path fspath2(fullDir.getData());
        return std::filesystem::exists(fspath2);
    }

    bool iaPath::isEmpty(const iaString &path)
    {
        con_assert(!path.isEmpty(), "invalid parameter");

        std::filesystem::path fspath(path.getData());
        return std::filesystem::is_directory(fspath) && std::filesystem::is_empty(fspath);
    }

    void iaPath::makeDirectory(const iaString &path)
    {
        if (iaPath::exists(path))
        {
            return;
        }

        std::filesystem::path fspath(path.getData());
        if (!std::filesystem::create_directories(fspath))
        {
            con_err("can't create directory \"" << path << "\"");
        }
        else
        {
            con_trace("created directory " << path);
        }
    }

    /*! \returns true if it's a file that matches given search pattern
    \param entry the file entry
    \param searchPattern the given pattern
    */
    static bool fileMatch(const std::filesystem::directory_entry &entry, const iaString &searchPattern)
    {
        if (entry.is_regular_file())
        {
            iaString filename(entry.path().filename().c_str());
            if (iaString::matchRegex(filename, iaString::wildcardToRegex(searchPattern)))
            {
                return true;
            }
        }

        return false;
    }

    std::vector<iaFile> iaPath::getFiles(const iaString &searchPattern, bool recursive, bool orderAlphabetically) const
    {
        std::vector<iaFile> result;

        if (!isDirectory(_path))
        {
            return result;
        }

        if (recursive)
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(_path.getData()))
            {
                if (fileMatch(entry, searchPattern))
                {
                    result.push_back(iaFile(entry.path().c_str()));
                }
            }
        }
        else
        {
            for (const auto &entry : std::filesystem::directory_iterator(_path.getData()))
            {
                if (fileMatch(entry, searchPattern))
                {
                    result.push_back(iaFile(entry.path().c_str()));
                }
            }
        }

        if (orderAlphabetically)
        {
            std::sort(result.begin(), result.end(),
                      [](iaFile const a, iaFile const b)
                      {
                          const iaString sa = iaString::toLower(a.getFullFileName());
                          const iaString sb = iaString::toLower(b.getFullFileName());
                          return sa < sb;
                      });
        }

        return result;
    }

    const iaString iaPath::getAbsolutePath() const
    {
        return fixPath(_path);
    }

    const iaString iaPath::getName(const iaString &path)
    {
        iaString cleanPath = iaString::trimRight(path, IGOR_PATHSEPARATOR);
        return cleanPath.getSubString(cleanPath.findLastOf(IGOR_PATHSEPARATOR) + 1);
    }

    const iaString iaPath::getName() const
    {
        return getName(_path);
    }

    const iaString iaPath::getParentPath(const iaString &path)
    {
        iaString cleanPath = iaString::trimRight(path, IGOR_PATHSEPARATOR);
        return cleanPath.getSubString(0, cleanPath.findLastOf(IGOR_PATHSEPARATOR));
    }

    const iaString iaPath::getParentPath() const
    {
        return getParentPath(_path);
    }

    bool iaPath::isRoot()
    {
        if (_path.isEmpty())
        {
            return false;
        }

#ifdef IGOR_WINDOWS
        if (_path.getLength() > 1 &&
            _path.getLength() <= 3)
        {
            if (iswalpha(_path[0]) &&
                _path[1] == ':')
            {
                return true;
            }
        }
#endif

#ifdef IGOR_LINUX
        if (_path.getLength() == 1 &&
            _path[1] == IGOR_PATHSEPARATOR)
        {
            return true;
        }
#endif

        return false;
    }

    bool iaPath::directoryIsAbsolute(const iaString &directoryname)
    {
        if (directoryname.isEmpty())
        {
            return false;
        }

#ifdef IGOR_WINDOWS
        if (iswalpha(directoryname[0]) &&
            directoryname[1] == ':')
        {
            return true;
        }
        else if (directoryname[0] == '\\' && directoryname[1] == '\\')
        {
            return true;
        }
#endif

#ifdef IGOR_LINUX
        if (directoryname[0] == IGOR_PATHSEPARATOR)
        {
            return true;
        }
#endif

        return false;
    }

    iaString iaPath::fixPath(const iaString &path)
    {
        if (path.isEmpty())
        {
            return iaString();
        }

        iaString result = path;

        // converts to OS specific path seperator
        for (int i = 0; i < result.getLength(); ++i)
        {
            if (result[i] == IGOR_NOT_PATHSEPARATOR)
            {
                result[i] = IGOR_PATHSEPARATOR;
            }
        }

#ifdef IGOR_LINUX
        // check if this is the user home folder
        if (result[0] == '~')
        {
            passwd *pw = getpwuid(getuid());
            const iaString homeDirectory(pw->pw_dir);

            result = homeDirectory + result.getSubString(1, result.getLength() - 1);
        }
#endif

        // does some relative to absolute path magic
        if (!directoryIsAbsolute(result))
        {
            result = iaPath::getCurrentDirectory() + IGOR_PATHSEPARATOR + result;
        }

        std::filesystem::path fspath(result.getData());
        result = fspath.lexically_normal().c_str();

        return result;
    }

    void iaPath::remove(const iaString &path)
    {
        if (!exists(path))
        {
            return;
        }

        std::filesystem::path fspath(path.getData());

        if (std::filesystem::is_regular_file(fspath) || std::filesystem::is_symlink(fspath))
        {
            std::filesystem::remove(fspath);
            con_trace("removed file " << path);
        }
        else if (std::filesystem::is_directory(fspath))
        {
            std::uintmax_t count = std::filesystem::remove_all(fspath); // removes directory recursively
            con_trace("directory removed recursively (" << count << " items): " << path);
        }
    }

    iaString iaPath::getRelativePath(const iaString &from, const iaString &to)
    {
        std::filesystem::path fromPath(from.getData());
        std::filesystem::path toPath(to.getData());
        return iaString(toPath.lexically_relative(fromPath).c_str());
    }

    iaString iaPath::getCurrentDirectory()
    {
        return iaString(std::filesystem::current_path().c_str());
    }

    void iaPath::setCurrentDirectory(const iaString &directory)
    {
        std::error_code error;
        std::filesystem::current_path(directory.getData(), error);
        if (error)
        {
            con_err("can't set current directory to " << directory);
        }
    }

    void iaPath::rename(const iaString &src, const iaString &dst, bool replaceExisting)
    {
        if (exists(dst))
        {
            if (!replaceExisting)
            {
                remove(dst);
            }
            else
            {
                con_err("can't rename " << src << " to " << dst << ". Destination already exists");
                return;
            }
        }

        const std::filesystem::path fssrc(src.getData());
        const std::filesystem::path fsdst(dst.getData());
        std::error_code error;

        std::filesystem::rename(fssrc, fsdst, error);

        if (error)
        {
            con_err("can't rename: " << src << " to: " << dst);
        }
        else
        {
            con_trace("renamed: " << src << " to: " << dst);
        }
    }

    void iaPath::copy(const iaString &src, const iaString &dst)
    {
        std::filesystem::path fssrc(src.getData());
        std::filesystem::path fsdst(dst.getData());

        std::error_code error;
        std::filesystem::copy(fssrc, fsdst, error);

        if (error)
        {
            con_err("cant copy file: " << src << " to: " << dst);
        }
        else
        {
            con_trace("copied file: " << src << " to: " << dst);
        }
    }

    bool iaPath::isFile() const
    {
        return isFile(_path);
    }

    bool iaPath::isDirectory() const
    {
        return isDirectory(_path);
    }

    bool iaPath::isSymlink() const
    {
        return isSymlink(_path);
    }

    const iaString iaPath::getStem(const iaString &path)
    {
        if (!isFile(path))
        {
            return "";
        }

        iaString stem = getName(path);

        int64 pos = stem.findLastOf('.');

        if (pos != iaString::INVALID_POSITION &&
            pos > 0)
        {
            return stem.getSubString(0, pos);
        }

        return "";
    }

    const iaString iaPath::getStem() const
    {
        return getStem(_path);
    }

    const iaString iaPath::getExtension() const
    {
        return getExtension(_path);
    }

    const iaString iaPath::getExtension(const iaString &path)
    {
        int64 pos = path.findLastOf('.');

        if (pos != iaString::INVALID_POSITION &&
            pos < path.getLength())
        {
            return path.getSubString(pos + 1, iaString::INVALID_POSITION);
        }

        return "";        
    }

    iaString iaPath::generateUniqueFilename(const iaString &filename)
    {
        const iaFile file(filename);
        const iaString extension = file.getExtension();
        const iaString stem = file.getStem();
        const iaString path = file.getParentPath();

        auto result = filename;

        int index = 1;
        while (iaPath::exists(result)) {
            result = path + stem + iaString::toString(index) + '.' + extension;
            ++index;
        }

        return result;
    }

    iaTime iaPath::getLastModifiedTime() const
    {
        return getLastModifiedTime(getAbsolutePath());
    }

    iaTime iaPath::getLastModifiedTime(const iaString &path)
    {
        const auto lastModifiedTime = std::filesystem::last_write_time(path.getData());
        const auto duration = lastModifiedTime.time_since_epoch();
        const int64 ms = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

        return iaTime::fromMicroseconds(ms);
    }    

} // namespace iaux
