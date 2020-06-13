#include <igor/resources/sound/loader/iLoaderWAV.h>

#include <iaux/system/iaFile.h>

namespace igor
{

    bool loadWav(const iaString& filename, iWAVHeader& header, char** buffer, int32 &bufferSize)
    {
        iaFile file(filename);

        if (!file.open())
        {
            return false;
        }

        if (!file.read(sizeof(iWAVHeader), reinterpret_cast<char*>(&header)))
        {
            return false;
        }

        // TODO maybe add more checks here to validate the header

        bufferSize = header._chunkSize - sizeof(iWAVHeader) + 8;
        *buffer = new char[bufferSize];

        if (!file.read(bufferSize, *buffer))
        {
            delete[] *buffer;
            return false;
        }

        return true;
    }

} // namespace igor