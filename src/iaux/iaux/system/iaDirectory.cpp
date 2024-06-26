// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaDirectory.h>

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
    iaDirectory::iaDirectory()
    {
    }

    iaDirectory::iaDirectory(const iaString &directoryName)
    {
        _directoryName = directoryName;
    }

    iaDirectory iaDirectory::operator=(const iaString &directoryName)
    {
        _directoryName = directoryName;
        return *this;
    }

    std::vector<iaDirectory> iaDirectory::getDirectories(bool recursive, bool orderAlphabetically) const
    {
        std::vector<iaDirectory> result;

        if (!isDirectory(_directoryName))
        {
            return result;
        }

        if (recursive)
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(_directoryName.getData()))
            {
                if (entry.is_directory())
                {
                    result.push_back(iaDirectory(entry.path().c_str()));
                }
            }
        }
        else
        {
            for (const auto &entry : std::filesystem::directory_iterator(_directoryName.getData()))
            {
                if (entry.is_directory())
                {
                    result.push_back(iaDirectory(entry.path().c_str()));
                }
            }
        }

        if (orderAlphabetically)
        {
            std::sort(result.begin(), result.end(), [](iaDirectory const a, iaDirectory const b)
                      { 
                        iaString sa = a.getFullDirectoryName();
                        sa.toLower();
                        iaString sb = b.getFullDirectoryName();
                        sb.toLower();
                        return sa < sb; });
        }

        return result;
    }

    bool iaDirectory::isDirectory(const iaString &path)
    {
        if (path.isEmpty())
        {
            return false;
        }

        std::filesystem::directory_entry entry(path.getData());
        if (entry.is_directory())
        {
            return true;
        }

        return false;
    }

    bool iaDirectory::exists() const
    {
        return iaDirectory::exists(_directoryName);
    }

    bool iaDirectory::exists(const iaString &path)
    {
        if (path.isEmpty())
        {
            return false;
        }

        std::filesystem::path fspath(path.getData());
        return std::filesystem::is_directory(fspath) && std::filesystem::exists(fspath);
    }

    bool iaDirectory::isEmpty(const iaString &path)
    {
        con_assert(!path.isEmpty(), "invalid parameter");

        std::filesystem::path fspath(path.getData());
        return std::filesystem::is_directory(fspath) && std::filesystem::is_empty(fspath);
    }

    void iaDirectory::makeDirectory(const iaString &path)
    {
        if (iaDirectory::exists(path))
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

    std::vector<iaFile> iaDirectory::getFiles(const iaString &searchPattern, bool recursive, bool orderAlphabetically) const
    {
        std::vector<iaFile> result;

        if (!isDirectory(_directoryName))
        {
            return result;
        }

        if (recursive)
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(_directoryName.getData()))
            {
                if (fileMatch(entry, searchPattern))
                {
                    result.push_back(iaFile(entry.path().c_str()));
                }
            }
        }
        else
        {
            for (const auto &entry : std::filesystem::directory_iterator(_directoryName.getData()))
            {
                if (fileMatch(entry, searchPattern))
                {
                    result.push_back(iaFile(entry.path().c_str()));
                }
            }
        }

        if (orderAlphabetically)
        {
            std::sort(result.begin(), result.end(), [](iaFile const a, iaFile const b)
                      { 
                        iaString sa = a.getFullFileName();
                        sa.toLower();
                        iaString sb = b.getFullFileName();
                        sb.toLower();
                        return sa < sb; });
        }

        return result;
    }

    iaString iaDirectory::getFullDirectoryName() const
    {
        return _directoryName;
    }

    iaString iaDirectory::getDirectoryName() const
    {
        return _directoryName.getSubString(_directoryName.findLastOf(IGOR_PATHSEPARATOR) + 1);
    }

    iaString iaDirectory::getFullParentDirectoryName() const
    {
        return _directoryName.getSubString(0, _directoryName.findLastOf(IGOR_PATHSEPARATOR));
    }

    bool iaDirectory::isRoot()
    {
        if (_directoryName.isEmpty())
        {
            return false;
        }

#ifdef IGOR_WINDOWS
        if (_directoryName.getLength() > 1 &&
            _directoryName.getLength() <= 3)
        {
            if (iswalpha(_directoryName[0]) &&
                _directoryName[1] == ':')
            {
                return true;
            }
        }
#endif

#ifdef IGOR_LINUX
        if (_directoryName.getLength() == 1 &&
            _directoryName[1] == IGOR_PATHSEPARATOR)
        {
            return true;
        }
#endif

        return false;
    }

    bool iaDirectory::directoryIsAbsolute(const iaString &directoryname)
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

    iaString iaDirectory::fixPath(const iaString &directoryName)
    {
        if (directoryName.isEmpty())
        {
            return iaString();
        }

        iaString temp = directoryName;

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
            temp = iaDirectory::getCurrentDirectory() + IGOR_PATHSEPARATOR + temp;
        }

        std::filesystem::path path(temp.getData());
        return iaString(path.lexically_normal().c_str());
    }

    iaString iaDirectory::getRelativePath(const iaString &from, const iaString &to)
    {
        iaString tempFrom;

        if (iaFile::exists(from))
        {
            iaFile file(from);
            tempFrom = file.getPath();
        }
        else
        {
            iaDirectory dirFrom(from);
            if (iaDirectory::isDirectory(from))
            {
                tempFrom = dirFrom.getFullDirectoryName();
            }
            else
            {
                // assuming the subfolder is actually a filename
                tempFrom = dirFrom.getFullParentDirectoryName();
            }
        }

        std::filesystem::path fromPath(tempFrom.getData());
        std::filesystem::path toPath(to.getData());
        return iaString(toPath.lexically_relative(fromPath).c_str());
    }

    iaString iaDirectory::getCurrentDirectory()
    {
        return iaString(std::filesystem::current_path().c_str());
    }

    void iaDirectory::setCurrentDirectory(const iaString &directory)
    {
        std::error_code error;
        std::filesystem::current_path(directory.getData(), error);
        if (error)
        {
            con_err("can't set current directory to " << directory);
        }
    }

} // namespace iaux
