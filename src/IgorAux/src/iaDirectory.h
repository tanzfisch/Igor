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
// (c) Copyright 2014-2016 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __IGOR_AUX_DIRECTORY__
#define __IGOR_AUX_DIRECTORY__

#include <iaDefines.h>
#include <iaString.h>

#include <vector>
using namespace std;

namespace IgorAux
{

    /*! handles directories and containing files
    */
	class IgorAux_API iaDirectory
	{

		friend class iaFile;

	public:

        /*! copy operator

        \param directoryName path of directory
        */
		iaDirectory operator= (const iaString& directoryName);

		/*! \returns get the absolute path name
        */
		iaString getFullDirectoryName() const;

        /*! \returns the name of the directory
        */
        iaString getDirectoryName() const;

		/*! \returns the absolute path name of parent directory
        */
		iaString getFullParentDirectoryName() const;

        /*! \returns relative path from path to path

        \param from the path from where the relative path comes from (can also be a full file path)
        \param to the path the relative path will point to
        */
        static iaString getRelativePath(const iaString& from, const iaString& to);

		/*! \returns all subdirectorys

        \param searchPattern what to search for
        \param recursive true: search recursively; false: search only in current directory
        */
		vector<iaDirectory> getDirectorys(iaString searchPattern = L"*", bool recursive = false);

		/*! \returns all files

        \param searchPattern what to search for
        \param recursive true: search recursively; false: search only in current directory
        */
		vector<iaFile> getFiles(iaString searchPattern = L"*", bool recursive = false);

        /*! set's the current or working directory

        \param directoyName the directory path
        */
        static void setCurrentDirectory(const iaString& directoyName);

        /*! \returns the current or working directory
        */
		static iaString getCurrentDirectory();

		/*! \returns the directory where the program starts from
        */
		static iaString getApplicationDirectory();

		/*! \returns true: if a directory is absolute; false: if diretory is relative
        */
		static bool directoryIsAbsolute(const iaString &directoryname);

		/*! \returns true: if a directory is realy a directory; false: if directory is a file
		don't use this to check if a path is a file try iaFile::exist
        */
		static bool isDirectory(const iaString &directoryname);

        /*! \returns the path seperator (win: \, linux: /)
        */
        static const wchar_t getPathSeperator();
        
        /*! \returns true: if root folder aka c:, d: or /
        */
        bool isRoot();

        /*! fixes the path from something like "../bla\blubber/temp.txt" to an absolute c:\bla\blubber\temp.txt

        \param directoryName path to fix
        \param file true: it's a file; false: it's a directory
        \returns fixed path
        */
        static iaString fixPath(const iaString& directoryName, bool file);

        /*! creates an object for some directory

        \param directoryName path of directory
        */
        iaDirectory(const iaString& directoryName);

    private:

        /*! the path to the directory
        */
        iaString _directoryName;

    protected:

        /*! internal recursive implementation of getFiles

        \param directory current directory
        \param searchPattern what to search for
        \param recursive true: proceed recursively; false: proceed for current directory
        */
        vector<iaFile> getFilesRecursive(const iaString &directory, const iaString &searchPattern = L"", const bool recursive = false);

        /*! internal recursive implementation of getDirectorys

        \param directory current directory
        \param searchPattern what to search for
        \param recursive true: proceed recursively; false: proceed for current directory
        */
        vector<iaDirectory> getDirectorysRecursive(const iaString &directory, const iaString &searchPattern = L"", const bool recursive = false);

	};

}

#endif