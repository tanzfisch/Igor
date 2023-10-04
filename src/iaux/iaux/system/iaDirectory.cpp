// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaDirectory.h>

#include <iaux/system/iaFile.h>
#include <iaux/system/iaConsole.h>

#include <sstream>
#include <filesystem>
#include <algorithm>

#ifdef __IGOR_LINUX__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

namespace iaux
{

    iaDirectory::iaDirectory(const iaString &directoryName)
    {
        _directoryName = directoryName;
    }

    iaDirectory iaDirectory::operator=(const iaString &directoryName)
    {
        _directoryName = directoryName;
        return *this;
    }

    std::vector<iaDirectory> iaDirectory::getDirectorys(bool recursive, bool orderAlphabetically)
    {
        iaString fullPath = fixPath(_directoryName, false);
        std::vector<iaDirectory> result;

        if(!isDirectory(fullPath))
        {
            return result;
        }

        if (recursive)
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(fullPath.getData()))
            {
                if (entry.is_directory())
                {
                    result.push_back(iaDirectory(entry.path().c_str()));
                }
            }
        }
        else
        {
            for (const auto &entry : std::filesystem::directory_iterator(fullPath.getData()))
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
        if(path.isEmpty())
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
        return iaDirectory::exists(getFullDirectoryName());
    }

    bool iaDirectory::exists(const iaString &path)
    {
        if(path.isEmpty())
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
        if(iaDirectory::exists(path))
        {
            con_warn("directory already exists " << path);
            return;
        }

        std::filesystem::path directory(path.getData());
        std::filesystem::create_directories(directory);
    }

    /*! \returns true if it's a file that matches given search pattern
    \param entry the file entry
    \param searchPattern the given pattern
    */
    static bool fileMatch(const std::filesystem::directory_entry &entry, iaux::iaString &searchPattern)
    {
        if (entry.is_regular_file())
        {
            iaString filename(entry.path().c_str());
            if (iaString::matchRegex(filename, searchPattern))
            {
                return true;
            }
        }

        return false;
    }

    std::vector<iaFile> iaDirectory::getFiles(iaString searchPattern, bool recursive, bool orderAlphabetically)
    {
        iaString fullPath = fixPath(_directoryName, false);
        std::vector<iaFile> result;

        if(!isDirectory(fullPath))
        {
            return result;
        }

        if (recursive)
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(fullPath.getData()))
            {
                if (fileMatch(entry, searchPattern))
                {
                    result.push_back(iaFile(entry.path().c_str()));
                }
            }
        }
        else
        {
            for (const auto &entry : std::filesystem::directory_iterator(fullPath.getData()))
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
        return fixPath(_directoryName, false);
    }

    iaString iaDirectory::getDirectoryName() const
    {
        iaString fullPath = fixPath(_directoryName, false);
        return fullPath.getSubString(fullPath.findLastOf(__IGOR_PATHSEPARATOR__) + 1);
    }

    iaString iaDirectory::getFullParentDirectoryName() const
    {
        iaString fullPath = fixPath(_directoryName, false);
        return fullPath.getSubString(0, fullPath.findLastOf(__IGOR_PATHSEPARATOR__));
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

#ifdef __IGOR_LINUX__
        if (_directoryName.getLength() == 1 &&
            _directoryName[1] == __IGOR_PATHSEPARATOR__)
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

#ifdef __IGOR_LINUX__
        if (directoryname[0] == __IGOR_PATHSEPARATOR__)
        {
            return true;
        }
#endif

        return false;
    }

    iaString iaDirectory::fixPath(const iaString &directoryName, bool file)
    {
        if (directoryName.isEmpty())
        {
            return iaString();
        }

        iaString temp = directoryName;
        const wchar_t pathSeperator = __IGOR_PATHSEPARATOR__;
        const wchar_t notPathSeperator = __IGOR_NOT_PATHSEPARATOR__;

        // converts to OS specific path seperator
        for (int i = 0; i < temp.getLength(); ++i)
        {
            if (temp[i] == notPathSeperator)
            {
                temp[i] = pathSeperator;
            }
        }

        // if this is a folder get rid of the last path seperator
        if (iaFile::exists(temp) && !file)
        {
            temp = temp.getSubString(0, temp.findLastOf(pathSeperator));
        }

#ifdef __IGOR_LINUX__
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
            temp = iaDirectory::getCurrentDirectory() + pathSeperator + temp;
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
        iaString fixedDirectory = iaDirectory::fixPath(directory, false);
        std::error_code error;
        std::filesystem::current_path(fixedDirectory.getData(), error);
        if (error)
        {
            con_err("can't set current directory to " << fixedDirectory);
        }
    }

} // namespace iaux
