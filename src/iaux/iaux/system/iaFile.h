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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IAUX_FILE_H
#define IAUX_FILE_H

#include <iaux/iaDefines.h>
#include <iaux/data/iaString.h>
#include <iaux/system/iaTime.h>

#include <vector>
#include <stdio.h>

namespace iaux
{

    /*! file open modes
     */
    enum class iaFileOpenMode
    {
        ReadBinary,
        WriteBinary,
        ReadWriteBinary
    };

    /*! prints the file open mode in the console

    \param stream the stream to log to
    \param mode the file open mode
    \returns the stream
    */
    IAUX_API std::wostream &operator<<(std::wostream &stream, const iaFileOpenMode &mode);

    /*! \brief File handle
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

        \param mode file open mode
        */
        bool open(const iaFileOpenMode &mode = iaFileOpenMode::ReadBinary);

        /*! closes the file
         */
        void close();

        /*! \returns true: if the file is open; false: if the file is closed
         */
        bool isOpen() const;

        /*! \returns the file open mode if open
         */
        const iaFileOpenMode &getFileOpenMode() const;

        /*! \returns only the parent path
         */
        const iaString getParentPath() const;

        /*! \returns the full path & filename
         */
        const iaString getFullFileName() const;

        /*! \returns only the filename
         */
        const iaString getName() const;

        /*! \returns filename without extension
         */
        const iaString getStem() const;

        /*! \returns the file extension
         */
        const iaString getExtension() const;

        /*! \returns the size of the file
         */
        int64 getSize() const;

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

        /*! \returns last modified time of file
         */
        iaTime getLastModifiedTime() const;

    protected:
        /*! sets the file pointer to a destination

        \param position new absolute position of filepointer
        */
        bool setFilePointer(int64 position);

    private:
        /*! the file name
         */
        iaString _filename = L"";

        /*! file open mode
         */
        iaFileOpenMode _mode;

        /*! size of file
         */
        int64 _size = 0;

        /*! the file handle
         */
        FILE *_fileHandle = nullptr;
    };

} // namespace iaux

#endif // IAUX_FILE_H