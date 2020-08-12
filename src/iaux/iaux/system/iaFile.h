//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IAUX_FILE__
#define __IAUX_FILE__

#include <iaux/iaDefines.h>
#include <iaux/data/iaString.h>

#include <vector>
#include <stdio.h>

namespace iaux
{

    /*! handles files

    \todo nice to have getCreationTime
    \todo nice to have getModifyTime
    \todo nice to have getAccessTime
    \todo nice to have getDriveLetter
    */
    class IAUX_API iaFile
    {

    public:
        /*! creates an object for a file

        \param fileName the path to the file associated with this object
        */
        iaFile(const iaString &fileName);

        /*! automatic closes the file if not closed yet
        */
        ~iaFile();

        /*! opens the file with the needed rights

        \param writeable true: open for writing; false: open read only
        */
        bool open(bool writeable = false);

        /*! closes the file
        */
        void close();

        /*! \returns true: if the file is open; false: if the file is closed
        */
        bool isOpen();

        /*! renames the file to a new name

        \param newFileName the new file name
        \param replaceExisting true: destination will be replaced; false: no action will be taken if destination already exists
        */
        void rename(const iaString &newFileName, bool replaceExisting = false);

        /*! copys the file to a new destination

        \param newFileName name of copy destination
        */
        iaFile copy(const iaString &newFileName) const;

        /*! checks if the file exist

        \returns true: if file exists; false: if not
        */
        bool exist() const;

        /*! checks if some file exist

        \param filename the file to be checked for
        \returns true: if file exists; false: if not
        */
        static bool exist(const iaString &filename);

        /*! \returns only the parent path
        */
        iaString getPath() const;

        /*! \returns only the filename
        */
        iaString getFileName() const;

        /*! \returns filename without extension
        */
        iaString getStem() const;

        /*! \returns the full path & filename
        */
        iaString getFullFileName() const;

        /*! \returns the file extension
        */
        iaString getExtension() const;

        /*! \returns the size of the file
        */
        int64 getSize();

        /*! resizes the file

        \param newSize the new size of the file
        \returns true: success; false: fail
        */
        bool setSize(int64 size);

        /*! reads from offset the number of size bytes to a (allocated) destination

        \param offset the offset to start reading from (-1 reads from current file pointer position)
        \param size size in bytes to read
        \param destination the destination to write to
        */
        bool read(int32 size, char *destination, int64 offset = -1);

        /*! writes from offset the number of size bytes from a destination

        \param offset the offset to start writing to (-1 writes at current file pointer position)
        \param size size in bytes to write
        \param source the source to read from
        */
        bool write(int32 size, const char *source, int64 offset = -1);

    protected:
        /*! sets the file pointer to a destination

        \param position new absolute position of filepointer
        */
        bool setFilePointer(int64 position);

    private:
        /*! the file name
        */
        iaString _filename = L"";

        /*! true: the file is opened writeable; false: the file is read only
        */
        bool _isWriteable = false;

        /*! size of file
        */
        int64 _size = 0;

        /*! the file handle
        */
        FILE *_fileHandle = nullptr;
    };

} // namespace iaux

#endif