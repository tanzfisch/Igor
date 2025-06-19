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
        // TODO
        return false;
    }

    bool iaPath::isDirectory(const iaString &path)
    {
        if (path.isEmpty())
        {
            return false;
        }

        std::filesystem::file_status entry = std::filesystem::status(path.getData());
        if (std::filesystem::is_directory(entry))
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
        if (std::filesystem::is_directory(fspath) && std::filesystem::exists(fspath))
        {
            return true;
        }

        const iaString fullDir = iaPath::getCurrentDirectory() + IGOR_PATHSEPARATOR + path;

        std::filesystem::path fspath2(fullDir.getData());
        return std::filesystem::is_directory(fspath2) && std::filesystem::exists(fspath2);
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
            con_warn("directory already exists " << path);
            return;
        }

        std::filesystem::path directory(path.getData());
        if (!std::filesystem::create_directories(directory))
        {
            con_err("can't create directory \"" << path << "\"");
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

    const iaString iaPath::getName() const
    {
        iaString cleanPath = iaString::trimRight(_path, IGOR_PATHSEPARATOR);
        return cleanPath.getSubString(cleanPath.findLastOf(IGOR_PATHSEPARATOR) + 1);
    }

    const iaString iaPath::getParentPath() const
    {
        iaString cleanPath = iaString::trimRight(_path, IGOR_PATHSEPARATOR);
        return cleanPath.getSubString(0, cleanPath.findLastOf(IGOR_PATHSEPARATOR));
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

        iaString temp = path;

        // converts to OS specific path seperator
        for (int i = 0; i < temp.getLength(); ++i)
        {
            if (temp[i] == IGOR_NOT_PATHSEPARATOR)
            {
                temp[i] = IGOR_PATHSEPARATOR;
            }
        }

#ifdef IGOR_LINUX
        // check if this is the user home folder
        if (temp[0] == '~')
        {
            passwd *pw = getpwuid(getuid());
            const iaString homeDirectory(pw->pw_dir);

            temp = homeDirectory + temp.getSubString(1, temp.getLength() - 1);
        }
#endif

        // does some relative to absolute path magic
        if (!directoryIsAbsolute(temp))
        {
            temp = iaPath::getCurrentDirectory() + IGOR_PATHSEPARATOR + temp;
        }

        std::filesystem::path fspath(temp.getData());
        return iaString(fspath.lexically_normal().c_str());
    }

    iaString iaPath::getRelativePath(const iaString &from, const iaString &to)
    {
        iaString tempFrom;

        if (iaFile::exists(from))
        {
            iaFile file(from);
            tempFrom = file.getPath();
        }
        else
        {
            iaPath dirFrom(from);
            if (iaPath::isDirectory(from))
            {
                tempFrom = dirFrom.getAbsolutePath();
            }
            else
            {
                // assuming the subfolder is actually a filename
                tempFrom = dirFrom.getParentPath();
            }
        }

        std::filesystem::path fromPath(tempFrom.getData());
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

    std::wostream &operator<<(std::wostream &stream, const iaPath &dir)
    {
        const auto path = dir.getAbsolutePath();
        if (!path.isEmpty())
        {
            stream << path.getData();
        }

        return stream;
    }

} // namespace iaux
