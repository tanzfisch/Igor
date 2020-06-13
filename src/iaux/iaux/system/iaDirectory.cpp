// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaDirectory.h>

#include <iaux/system/iaFile.h>
#include <iaux/system/iaConsole.h>

#include <sstream>
#include <filesystem>

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

    std::vector<iaDirectory> iaDirectory::getDirectorys(bool recursive)
    {
        iaString fullPath = fixPath(_directoryName, false);
        std::vector<iaDirectory> result;

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

        return result;
    }

    bool iaDirectory::isDirectory(const iaString &directoryname)
    {
        std::filesystem::directory_entry entry(directoryname.getData());
        if (entry.is_directory())
        {
            return true;
        }

        return false;
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
            if (iaString::match(filename, searchPattern))
            {
                return true;
            }
        }

        return false;
    }

    std::vector<iaFile> iaDirectory::getFiles(iaString searchPattern, bool recursive)
    {
        iaString fullPath = fixPath(_directoryName, false);
        std::vector<iaFile> result;

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

#ifdef __IGOR_WINDOWS__
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

#ifdef __IGOR_WINDOWS__
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
        const wchar_t dirSeperator = __IGOR_PATHSEPARATOR__;
        const wchar_t notDirSeperator = (dirSeperator == '\\') ? '/' : '\\';

        if (iaFile::exist(temp) && !file)
        {
            temp = temp.getSubString(0, temp.findLastOf(dirSeperator));
        }

        // converts / to the other one not allowed in sourcecode (back-slash), vise versa at linux ;)
        for (uint64 pos = temp.findFirstOf(notDirSeperator); pos != iaString::INVALID_POSITION; pos = temp.findFirstOf(notDirSeperator, pos))
        {
            temp[pos] = dirSeperator;
        }

        // does some relative to absolue path thinngys
        if (!directoryIsAbsolute(temp))
        {
            temp = iaDirectory::getCurrentDirectory() + dirSeperator + temp;
        }

        std::filesystem::path path(temp.getData());
        return iaString(path.lexically_normal().c_str());
    }

    iaString iaDirectory::getRelativePath(const iaString &from, const iaString &to)
    {
        iaString result;

        iaString tempFrom;

        if (iaFile::exist(from))
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
