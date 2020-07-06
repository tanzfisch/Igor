#include <igor/resources/sound/loader/iLoaderWAV.h>

#include <iaux/system/iaFile.h>

namespace igor
{

    bool loadWav(const iaString &filename, iWAVHeader &header, char **buffer, int32 &bufferSize)
    {
        iaFile file(filename);

        if (!file.open())
        {
            return false;
        }

        if (!file.read(sizeof(iWAVHeader), reinterpret_cast<char *>(&header)))
        {
            return false;
        }

        con_debug_endl("loading wav \"" << filename << "\"");
        con_debug_endl("iWAVHeader      : " << sizeof(iWAVHeader));
        con_debug_endl("chunk id        : " << header._chunkID[0] << header._chunkID[1] << header._chunkID[2] << header._chunkID[3]);
        con_debug_endl("chunk size      : " << header._chunkSize);
        con_debug_endl("format          : " << header._format[0] << header._format[1] << header._format[2] << header._format[3]);
        con_debug_endl("sub chunk 1 id  : " << header._subchunk1ID[0] << header._subchunk1ID[1] << header._subchunk1ID[2] << header._subchunk1ID[3]);
        con_debug_endl("sub chunk 1 size: " << header._subchunk1Size);
        con_debug_endl("audio format    : " << header._audioFormat);
        con_debug_endl("channels        : " << header._numChannels);
        con_debug_endl("sample rate     : " << header._sampleRate << "Hz");
        con_debug_endl("byte rate       : " << header._byteRate);
        con_debug_endl("block align     : " << header._blockAlign);
        con_debug_endl("bits per sample : " << header._bitsPerSample);
        con_debug_endl("extra param size: " << header._extraParamSize);
        con_debug_endl("sub chunk 2 id  : " << header._subchunk2ID[0] << header._subchunk2ID[1] << header._subchunk2ID[2] << header._subchunk2ID[3]);
        con_debug_endl("sub chunk 2 size: " << header._subchunk2Size);
        con_debug_endl("sampleCount     : " << header._chunkSize / (float64)header._blockAlign);
        con_debug_endl("media length    : " << (header._chunkSize / (float64)header._blockAlign) / (float64)header._sampleRate << "s");

        iaString riff(header._chunkID, 4);
        if (riff != "RIFF")
        {
            con_err("invalid chunk id " << riff);
            return false;
        }

        if (header._audioFormat != 1)
        {
            con_err("unknown audio format");
            return false;
        }

        bufferSize = header._chunkSize - (sizeof(iWAVHeader) - 8);
        *buffer = new char[bufferSize];

        if (!file.read(bufferSize, *buffer))
        {
            delete[] * buffer;
            return false;
        }

        return true;
    }

} // namespace igor