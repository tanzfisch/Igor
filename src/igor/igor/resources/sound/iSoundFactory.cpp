// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/sound/iSoundFactory.h>

#include <igor/resources/iResourceManager.h>
#include <igor/resources/sound/loader/iLoaderWAV.h>
#include <igor/audio/iAudio.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

namespace igor
{

    iSoundFactory::iSoundFactory()
        : iFactory(IGOR_RESOURCE_SOUND)
    {
    }

    static bool isSound(const iaString &filename)
    {
        iaFile file(filename);
        const iaString &fileExtension = file.getExtension();

        for (const auto &extension : IGOR_SUPPORTED_MATERIAL_EXTENSIONS)
        {
            if (fileExtension == extension)
            {
                return true;
            }
        }

        return false;
    }

    iResourcePtr iSoundFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iSound(parameters));
    }

    bool iSoundFactory::loadResource(iResourcePtr resource)
    {
        const iaString filepath = iResourceManager::getInstance().getFilePath(resource->getID());
        const iaString filename = iResourceManager::getInstance().resolvePath(filepath);
        iSoundPtr sound = std::dynamic_pointer_cast<iSound>(resource);
        return loadSound(filename, sound);
    }

    bool iSoundFactory::loadSound(const iaString &filename, iSoundPtr sound)
    {
        iWAVHeader header;
        char *buffer = nullptr;
        int32 bufferSize;
        if (!loadWav(filename, header, &buffer, bufferSize))
        {
            return false;
        }

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

            con_debug("loaded sound \"" << sound->getInfo() << "\" [" << sound->_bitsPerSample << "bit " << header._sampleRate << "Hz " << channels << "]");
        }

        delete[] buffer;
        return true;
    }

    void iSoundFactory::unloadResource(iResourcePtr resource)
    {
        auto sound = std::dynamic_pointer_cast<iSound>(resource);
        con_assert(sound != nullptr, "zero pointer");

        iAudio::getInstance().destroyBuffer(sound->_buffer);
    }

    iaString iSoundFactory::getHashData(const iParameters &parameters) const
    {
        // there is no type specific data for sounds at this point
        return L"";
    }

    bool iSoundFactory::matchingType(const iParameters &parameters) const
    {
        if (parameters.getParameter<iaString>("type") == getType())
        {
            return true;
        }

        if (isSound(parameters.getParameter<iaString>("filename")) ||
            isSound(parameters.getParameter<iaString>("alias")))
        {
            return true;
        }

        return false;
    }

}; // namespace igor