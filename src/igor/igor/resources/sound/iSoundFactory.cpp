// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/sound/iSoundFactory.h>

#include <igor/resources/sound/iSound.h>
#include <igor/resources/iResourceManager.h>
#include <igor/resources/sound/loader/iLoaderWAV.h>
#include <igor/audio/iAudio.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

namespace igor
{

    const iaString &iSoundFactory::getType() const
    {
        const static iaString typeName(L"sound");
        return typeName;
    }

    iResourcePtr iSoundFactory::createResource(const iResourceParameters &parameters) const
    {
        return iResourcePtr(new iSound(parameters));
    }

    bool iSoundFactory::loadResource(iResourcePtr resource) const
    {
        bool result = false;
        iaString filename = iResourceManager::getInstance().getPath(resource->getName());

        iWAVHeader header;
        char *buffer = nullptr;
        int32 bufferSize;
        if (loadWav(filename, header, &buffer, bufferSize))
        {
            auto sound = std::dynamic_pointer_cast<iSound>(resource);
            con_assert(sound != nullptr, "zero pointer");

            sound->_bitsPerSample = header._bitsPerSample;
            sound->_byteRate = header._byteRate;
            sound->_numChannels = header._numChannels;
            sound->_sampleRate = header._sampleRate;

            sound->_bytesPerSample = header._blockAlign;
            sound->_sampleCount = bufferSize / sound->_bytesPerSample;

            if (iAudio::getInstance().createBuffer(sound->_buffer, sound->_numChannels, sound->_bitsPerSample, sound->_sampleRate, buffer, bufferSize))
            {
                iaString channels;
                switch (header._numChannels)
                {
                case 1:
                    channels = L"mono";
                    break;
                case 2:
                    channels = L"stereo";
                    break;
                }

                con_info("loaded sound \"" << resource->getName() << "\" [" << sound->_bitsPerSample << "bit " << header._sampleRate << "Hz " << channels << "]");
                result = true;
            }

            delete[] buffer;
        }

        return result;
    }

    void iSoundFactory::unloadResource(iResourcePtr resource) const
    {
        auto sound = std::dynamic_pointer_cast<iSound>(resource);
        con_assert(sound != nullptr, "zero pointer");

        iAudio::getInstance().destroyBuffer(sound->_buffer);

        con_info("released sound \"" << resource->getName() << "\"");
    }

    iaString iSoundFactory::getHashData(const iResourceParameters &parameters) const
    {
        // there is no type specific data for sounds at this point
        return L"";
    }

    bool iSoundFactory::matchingType(const iResourceParameters &parameters) const
    {
        if (parameters._type == getType())
        {
            return true;
        }

        iaFile file(parameters._name);
        const iaString &extension = file.getExtension();
        if (extension == "wav")
        {
            return true;
        }

        return false;
    }

}; // namespace igor