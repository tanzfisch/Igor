//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2026 by Martin A. Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: igorgameengine@protonmail.com

#ifndef IAUX_PATH_H
#define IAUX_PATH_H

#include <iaux/iaDefines.h>
#include <iaux/data/iaString.h>
#include <iaux/system/iaTime.h>

#include <vector>

namespace iaux
{

    class iaFile;

    /*! handles directories and containing files
     */
    class IAUX_API iaPath
    {

    public:
        /*! creates an object for some directory

        \param path path of directory
        */
        iaPath(const iaString &path);

        /*! empty directory ctor
         */
        iaPath();

        /*! copy operator

        \param path path of directory
        */
        iaPath operator=(const iaString &path);

        /*! equal operator

        \param other the other path to compare with
        */
        bool operator==(const iaString &other);

        /*! unequal operator

        \param other the other path to compare with
        */
        bool operator!=(const iaString &other);

        /*! \returns get the absolute path (if possible)

        To generate an absolute path it check if it exists relative to the current directory
        */
        const iaString getAbsolutePath() const;

        /*! \returns the last name in path

        examples:
        "c:\foo\bar" -> "bar"
        "/foo/bar.txt" -> "bar.txt"
        */
        const iaString getName() const;

        /*! \returns the last name from given path

        examples:
        "c:\foo\bar" -> "bar"
        "/foo/bar.txt" -> "bar.txt"

        \param path the given path
        */
        static const iaString getName(const iaString &path);

        /*! \returns the absolute path name of parent directory

        e.g. if the path is c:\foo\bar then this returns c:\foo
        */
        const iaString getParentPath() const;

        /*! \returns the absolute path name of parent directory from given path

        e.g. if the path is c:\foo\bar then this returns c:\foo

        \param path the given path
        */
        static const iaString getParentPath(const iaString &path);

        /*! \returns filename without extension

        "" if this is not a file
         */
        const iaString getStem() const;

        /*! \returns filename without extension from given path

        "" if this is not a file

        \param path the given path
         */
        static const iaString getStem(const iaString &path);

        /*! \returns the file extension
         */
        const iaString getExtension() const;

        /*! \returns the file extension from given path

        \param path the given path
         */
        static const iaString getExtension(const iaString &path);

        /*! \returns relative path from path to path

        \param from the path from where the relative path comes from (can also be a full file path)
        \param to the path the relative path will point to
        */
        static iaString getRelativePath(const iaString &from, const iaString &to);

        /*! \returns all sub directories

        \param recursive true: search recursively; false: search only in current directory
        \param orderAlphabetically true: returns directories in alphabetical order
        */
        std::vector<iaPath> getDirectories(bool recursive = false, bool orderAlphabetically = true) const;

        /*! \returns files of given directory

        assuming path is an existing directory

        \param searchPattern what to search for using regular expression
        \param recursive true: search recursively; false: search only in current directory
        \param orderAlphabetically true: returns files in alphabetical order
        */
        std::vector<iaFile> getFiles(const iaString &searchPattern = L"*", bool recursive = false, bool orderAlphabetically = true) const;

        /*! set's the current or working directory

        \param path the directory path
        */
        static void setCurrentDirectory(const iaString &path);

        /*! \returns the current or working directory
         */
        static iaString getCurrentDirectory();

        /*! \returns true: if a path is absolute; false: if path is relative

        \param path the given path
        */
        static bool directoryIsAbsolute(const iaString &path);

        /*! \returns true: if a path is directory; false: if path is not a directory

        \param path the given path
         */
        static bool isDirectory(const iaString &path);

        /*! \returns true: if a path is directory; false: if path is not a directory
         */
        bool isDirectory() const;

        /*! \returns true: if a path is a file or a symbolic link

        \param path the given path
         */
        static bool isFile(const iaString &path);

        /*! \returns true: if a path is a file or a symbolic link
         */
        bool isFile() const;

        /*! \returns true if given path is a symbolic link

        \param path the given path
        */
        static bool isSymlink(const iaString &path);

        /*! \returns true if given path is a symbolic link
         */
        bool isSymlink() const;

        /*! deletes/removes path from filesystem

        \param path the file or directory to remove
        */
        static void remove(const iaString &path);

        /*! copys the file to a new destination

        \param newFileName name of copy destination
        */
        static void copy(const iaString &src, const iaString &dst);

        /*! renames path to a new name

        \param src the source path
        \param dst the destination path
        \param replaceExisting true: destination will be replaced; false: no action will be taken if destination already exists
        */
        static void rename(const iaString &src, const iaString &dst, bool replaceExisting = false);

        /*! creates directory at given path

        \param path given path
        */
        static void makeDirectory(const iaString &path);

        /*! \returns true if given directory exists

        relative to current or absolute

        \param path the given path
        */
        static bool exists(const iaString &path);

        /*! \returns true if given directory exists
         */
        bool exists() const;

        /*! \returns true if given directory is empty

        \param path the given directory
        */
        static bool isEmpty(const iaString &path);

        /*! \returns true: if root folder

        eg c:, d: (windows)
        / (linux)
        */
        bool isRoot();

        /*! fixes the path from something like "../bla\blubber/temp.txt" to an absolute c:\bla\blubber\temp.txt

        \param path path to fix
        \returns fixed path
        */
        static iaString fixPath(const iaString &path);

        /*! \returns valid unique filename for given filename
        \param filename the given filename
        */
        static iaString generateUniqueFilename(const iaString &filename);       
        
        /*! \returns last modified time of path
         */
        iaTime getLastModifiedTime() const;

        /*! \returns last modified time of given path

        \param path the given path
        */
        static iaTime getLastModifiedTime(const iaString &path);        

    private:
        /*! the path to the directory
         */
        iaString _path;
    };

    /*! stream operator

    \param stream the destination
    \param dir the directory to stream
    \returns the resulting stream
    */
    IAUX_API std::wostream &operator<<(std::wostream &stream, const iaPath &dir);

} // namespace iaux

#endif // IAUX_DIRECTORY_H