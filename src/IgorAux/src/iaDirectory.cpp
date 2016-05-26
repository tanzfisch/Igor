// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iaDirectory.h>

#include <iaFile.h>
#include <iaConsole.h>

#include <sstream>
using namespace std;

namespace IgorAux
{

	iaDirectory::iaDirectory(const iaString & directoryName)
	{
        _directoryName = directoryName;
	}

	iaDirectory iaDirectory::operator= (const iaString& directoryName)
	{
		_directoryName = directoryName;
		return *this;
	}

	vector<iaDirectory> iaDirectory::getDirectorys(iaString searchPattern, bool recursive)
	{
        iaString fullPath = fixPath(_directoryName, false);
        return getDirectorysRecursive(fullPath, searchPattern, recursive);
	}

	vector<iaDirectory> iaDirectory::getDirectorysRecursive(const iaString &directory, const iaString &searchPattern, const bool recursive)
	{
		vector<iaDirectory> result;
		WIN32_FIND_DATA findData;
		HANDLE findHandle;

		iaString filesToFind = directory;
		filesToFind += getPathSeperator();
		filesToFind += searchPattern;

		// find the first dir
		findHandle = FindFirstFile(filesToFind.getData(), &findData);

		if(findHandle != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (!( (findData.cFileName[0]=='.') && ( (findData.cFileName[1]=='.' && findData.cFileName[2]==0) || findData.cFileName[1]==0 ) ))
				{
					if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						// save first file
						result.push_back(iaDirectory(directory + getPathSeperator() + findData.cFileName));
					}
				}
			}while(FindNextFile(findHandle, &findData));

			FindClose(findHandle);
		}

		// if recursive then search for directories now
		// the search after files and directorys is separeted because of the search pattern
		if(recursive)
		{
			filesToFind = directory;
			filesToFind += getPathSeperator();
			filesToFind += L"*.*";
			iaString current;

			// find first directory
			findHandle = FindFirstFile(filesToFind.getData(), &findData);

			if(findHandle != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (!( (findData.cFileName[0]=='.') && ( (findData.cFileName[1]=='.' && findData.cFileName[2]==0) || findData.cFileName[1]==0 ) ))
					{
						if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							vector<iaDirectory> result_add = getDirectorysRecursive(directory + getPathSeperator() + findData.cFileName, searchPattern, true);

                            for (unsigned int i = 0; i < result_add.size(); ++i)
                            {
                                result.push_back(result_add[i]);
                            }
							result_add.clear();
						}
					}
				}while(FindNextFile(findHandle, &findData));

				FindClose(findHandle);
			}
		}

		return result;
	}

	bool iaDirectory::isDirectory(const iaString &directoryname)
	{
		WIN32_FIND_DATAW findData;
		bool result = false;
		HANDLE findHandle = FindFirstFile(fixPath(directoryname, true).getData(), &findData);

		if(findHandle != INVALID_HANDLE_VALUE)
		{
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                result = true;
            }

			FindClose(findHandle);
		}

		return result;
	}

	vector<iaFile> iaDirectory::getFiles(iaString searchPattern, bool recursive)
	{
        iaString fullPath = fixPath(_directoryName, false);
        return getFilesRecursive(fullPath, searchPattern, recursive);
	}

	vector<iaFile> iaDirectory::getFilesRecursive(const iaString &directory, const iaString &searchPattern, const bool recursive)
	{
		vector<iaFile> result;
		WIN32_FIND_DATA findData;
		HANDLE findHandle;

		iaString filesToFind = directory;
		filesToFind += getPathSeperator();
		filesToFind += searchPattern;

		// find the first file
		findHandle = FindFirstFile(filesToFind.getData(), &findData);

		if(findHandle != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (!( (findData.cFileName[0]=='.') && ( (findData.cFileName[1]=='.' && findData.cFileName[2]==0) || findData.cFileName[1]==0 ) ))
				{
					if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						// save first file
						iaFile file = iaFile(directory + getPathSeperator() + findData.cFileName);
						result.push_back(file);
					}
				}
			}while(FindNextFile(findHandle, &findData));

			FindClose(findHandle);
		}

		// if recursive then search for directories now
		// the search after files and directorys is separeted because of the search pattern
		if(recursive)
		{
			filesToFind = directory;
			filesToFind += getPathSeperator();
			filesToFind += L"*.*";
			iaString current;

			// find first directory
			findHandle = FindFirstFile(filesToFind.getData(), &findData);

			if(findHandle != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (!( (findData.cFileName[0]=='.') && ( (findData.cFileName[1]=='.' && findData.cFileName[2]==0) || findData.cFileName[1]==0 ) ))
					{
						if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							vector<iaFile> result_add = getFilesRecursive(directory + getPathSeperator() + findData.cFileName, searchPattern, true);

                            for (unsigned int i = 0; i < result_add.size(); ++i)
                            {
                                result.push_back(result_add[i]);
                            }
							result_add.clear();
						}
					}
				}while(FindNextFile(findHandle, &findData));

				FindClose(findHandle);
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
        return fullPath.getSubString(fullPath.findLastOf(getPathSeperator()) + 1);
    }

	iaString iaDirectory::getFullParentDirectoryName() const
	{
        iaString fullPath = fixPath(_directoryName, false);
        return fullPath.getSubString(0, fullPath.findLastOf(getPathSeperator()));
	}

    bool iaDirectory::isRoot()
    {
        if (_directoryName.isEmpty())
        {
            return false;
        }
        else if(_directoryName.getSize() > 1 && _directoryName.getSize() <= 3)
        {
            wchar_t first = _directoryName[0];
            if (iswalpha(first) && _directoryName[1] == ':')
            {
                return true;
            }
        }

        return false;
    }

	bool iaDirectory::directoryIsAbsolute(const iaString &directoryname)
	{
		if(directoryname.getSize())
		{
			if(directoryname[1] == ':')
			{
				return true;
			}
			else if(directoryname[0] == '\\' && directoryname[1] == '\\') 
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	iaString iaDirectory::fixPath(const iaString & directoryName, bool file)
	{
		iaString temp = directoryName;
		wchar_t dirSeperator = getPathSeperator();
		wchar_t notDirSeperator = (dirSeperator == '\\') ? '/' : '\\';

		if(iaFile::exist(temp) && !file)
		{
			temp = temp.getSubString(0,temp.findLastOf(dirSeperator));
		}

		// converts / to the other one not allowed in sourcecode (back-slash), vise versa at linux ;)
        for (size_t pos = temp.findFirstOf(notDirSeperator); pos != iaString::INVALID_POSITION; pos = temp.findFirstOf(notDirSeperator, pos))
        {
            temp[pos] = dirSeperator;
        }

		// does some relative to absolue path thinngys
		if(!directoryIsAbsolute(temp))
		{
            temp = iaDirectory::getCurrentDirectory() + dirSeperator + temp;
		}

		// crops the current path for every .. and double slash we find
		int last_pos = 0;
		vector<iaString> path;

        temp.split(iaDirectory::getPathSeperator(), path);
		temp.clear();

		int skipp_count = 0;

		do
		{
			iaString part = path.back();
			path.pop_back();

			if(part != L"")
			{
				if(part == L"..")
				{
					skipp_count++;
				}
				else
				{
					if(skipp_count > 0)
					{
						skipp_count--;
					}
					else
					{
						if(temp == L"")
						{
							temp =  part;
						}
						else
						{
							temp =  part + dirSeperator + temp;
						}
					}
				}
			}
		}
		while(path.size());

		return temp;
    }

    iaString iaDirectory::getRelativePath(const iaString& from, const iaString& to)
    {
        iaString result;

        iaString fromPath;

        if (iaFile::exist(from))
        {
            iaFile file(from);
            fromPath = file.getPath();
        }
        else
        {
            iaDirectory dirFrom(from);
            fromPath = dirFrom.getFullDirectoryName();
        }
        
        iaFile fileTo(to);
        vector<iaString> elementsFrom;
        fromPath.split(iaDirectory::getPathSeperator(), elementsFrom);

        iaString toPath = fileTo.getPath();
        vector<iaString> elementsTo;
        toPath.split(iaDirectory::getPathSeperator(), elementsTo);

        int index = 0;

        if (elementsFrom[index] == elementsTo[index])
        {
            index++;

            while (index < elementsFrom.size() &&
                index < elementsTo.size() &&
                elementsFrom[index] == elementsTo[index])
            {
                index++;
            }

            for (int i = index; i < elementsFrom.size(); ++i)
            {
                if (result.isEmpty())
                {
                    result = "..";
                }
                else
                {
                    result = result + iaDirectory::getPathSeperator() + "..";
                }
            }

            while (index < elementsTo.size())
            {
                result = result + iaDirectory::getPathSeperator() + elementsTo[index];
                index++;
            }
                
            if(result.isEmpty())
            {
                result = ".";
            }

            result = result + iaDirectory::getPathSeperator() + fileTo.getFileName();
        }
        else
        {
            con_warn("paths are not on the same drive");
        }

        return result;
    }

	const wchar_t iaDirectory::getPathSeperator()
	{
		return '\\';
	}

    iaString iaDirectory::getApplicationDirectory()
    {
		wchar_t result[MAX_PATH];
        GetModuleFileName(NULL, result, MAX_PATH);
        iaFile file(result);
        return file.getPath();
    }

	iaString iaDirectory::getCurrentDirectory()
	{
		static wchar_t directory[1024];
		int numChars = GetCurrentDirectory(1024, directory);

		// got an error
		if(numChars == 0)
		{
			con_err_win("cant get directory");
			return L"";
		}
		else if(numChars < 1024) // got current directory correctly
		{
			return directory;
		}

		// buffer was too small
		else
		{
			// reserve mem, get directory, free and return
			wchar_t* tempDirectory = new wchar_t[numChars];
			if(GetCurrentDirectory(numChars, tempDirectory) == 0)
			{
				con_err_win("cant get directory");
				return L"";
			}

			iaString temp = tempDirectory;
			delete [] tempDirectory;

			return temp;
		}
	}

	void iaDirectory::setCurrentDirectory(const iaString & directory)
	{
		iaString fixedDirectory = iaDirectory::fixPath(directory, false);

		if(!SetCurrentDirectory(fixedDirectory.getData()))
		{
			con_err_win("cant set directory to: " << fixedDirectory);
		}
	}

}
