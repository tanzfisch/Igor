#include <igor/audio/iAudio.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

#ifdef __IGOR_DEBUG__
#define AL_CHECK_ERROR()                              \
    do                                                \
    {                                                 \
        ALenum error = alGetError();                  \
        iaString errorString;                         \
        if (error != AL_NO_ERROR)                     \
        {                                             \
            switch (error)                            \
            {                                         \
            case AL_INVALID_NAME:                     \
                errorString = "AL_INVALID_NAME";      \
                break;                                \
            case AL_INVALID_ENUM:                     \
                errorString = "AL_INVALID_ENUM";      \
                break;                                \
            case AL_INVALID_VALUE:                    \
                errorString = "AL_INVALID_VALUE";     \
                break;                                \
            case AL_INVALID_OPERATION:                \
                errorString = "AL_INVALID_OPERATION"; \
                break;                                \
            case AL_OUT_OF_MEMORY:                    \
                errorString = "AL_OUT_OF_MEMORY";     \
                break;                                \
            default:                                  \
                errorString = "UNKNOWN";              \
            };                                        \
            con_err("OpenAL error: " << errorString); \
        }                                             \
        else                                          \
            break;                                    \
    } while (0)
#else
#define AL_CHECK_ERROR() 1
#endif

    class iAudioImpl
    {
    public:
        iAudioImpl()
        {
            con_info("initialize audio");

            std::vector<std::string> deviceNames;

            if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT"))
            {
                const char *devicesString = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);

                bool inString = false;
                int from = 0;
                int to = 0;

                while (true)
                {
                    if (inString)
                    {
                        if (devicesString[to] == 0)
                        {
                            deviceNames.push_back(std::string(devicesString + from));
                            inString = false;
                        }
                    }
                    else
                    {
                        if (devicesString[to] == 0)
                        {
                            break;
                        }
                        else
                        {
                            from = to;
                            inString = true;
                        }
                    }

                    to++;
                }
            }

            deviceNames.push_back("");

            for (const auto deviceName : deviceNames)
            {
                con_debug_endl("try to open: " << ((deviceName == "") ? "default" : deviceName.c_str()));

                _device = alcOpenDevice(deviceName.c_str());
                if (_device != nullptr)
                {
                    con_info("OpenAL Device  : " << ((deviceName == "") ? "default" : deviceName.c_str()));
                    break;
                }
            }

            if (_device == nullptr)
            {
                con_err("can't open sound device");
                return;
            }

            _context = alcCreateContext(_device, nullptr);
            if (_context == nullptr)
            {
                con_err("no sound context found");
                alcCloseDevice(_device);
                _device = nullptr;
                return;
            }

            if (!alcMakeContextCurrent(_context))
            {
                con_err("can't make context current");
                alcDestroyContext(_context);
                alcCloseDevice(_device);
                _context = nullptr;
                _device = nullptr;
                return;
            }

            con_info("OpenAL Version : " << alGetString(AL_VERSION));
            con_info("OpenAL Vendor  : " << alGetString(AL_VENDOR));
            con_info("OpenAL Renderer: " << alGetString(AL_RENDERER));

            _initialized = true;
        }

        ~iAudioImpl()
        {
            con_info("shutdown audio");

            if (_initialized &&
                _context != nullptr &&
                _device != nullptr)
            {
                alcMakeContextCurrent(nullptr);
                AL_CHECK_ERROR();
                alcDestroyContext(_context);
                AL_CHECK_ERROR();
                alcCloseDevice(_device);
                AL_CHECK_ERROR();
            }
        }

        bool createAudioBuffer(iAudioBuffer &audioBuffer, int16 numChannels, int16 bitsPerSample, int32 sampleRate, const char *buffer, int32 bufferSize)
        {
            if (!_initialized)
            {
                return false;
            }

            ALenum format = 0;

            if (bitsPerSample == 8)
            {
                if (numChannels == 1)
                {
                    format = AL_FORMAT_MONO8;
                }
                else if (numChannels == 2)
                {
                    format = AL_FORMAT_STEREO8;
                }
            }
            else if (bitsPerSample == 16)
            {
                if (numChannels == 1)
                {
                    format = AL_FORMAT_MONO16;
                }
                else if (numChannels == 2)
                {
                    format = AL_FORMAT_STEREO16;
                }
            }

            if (format == 0)
            {
                con_err("Unsupported format " << numChannels << "channels " << bitsPerSample << "bits");
                return false;
            }
            audioBuffer._id = 10;
            alGenBuffers(1, (ALuint *)&audioBuffer._id);
            if (!alIsBuffer(audioBuffer._id))
            {
                con_err("Can't create audio buffer " << audioBuffer._id);
                return false;
            }

            alBufferData((ALuint)audioBuffer._id, format, buffer, (ALsizei)(bufferSize), (ALsizei)(sampleRate));
            if (alGetError() != AL_NO_ERROR)
            {
                con_err("Can't fill audio buffer");
                return false;
            }

            return true;
        }

        void destroyAudioBuffer(const iAudioBuffer &buffer)
        {
            if (!_initialized)
            {
                return;
            }

            alDeleteBuffers(1, (ALuint *)&buffer._id);
        }

        void updateListener(const iaMatrixd &matrix, const iaVector3d velocity)
        {
            if (!_initialized)
            {
                return;
            }

            const ALfloat listenerOri[] = {
                matrix._right._x, matrix._right._y, matrix._right._z,
                matrix._top._x, matrix._top._y, matrix._top._z};

            alListener3f(AL_POSITION, matrix._pos._x, matrix._pos._y, matrix._pos._z);
            AL_CHECK_ERROR();
            alListener3f(AL_VELOCITY, velocity._x, velocity._y, velocity._z);
            AL_CHECK_ERROR();
            alListenerfv(AL_ORIENTATION, listenerOri);
            AL_CHECK_ERROR();
        }

        bool createAudioSource(iAudioSource &audioSource)
        {
            if (!_initialized)
            {
                return false;
            }

            alGenSources((ALuint)1, (ALuint *)&audioSource._id);
            if (!alIsSource(audioSource._id))
            {
                con_err("Can't create audio source");
                return false;
            }
            return true;
        }

        void destroyAudioSource(const iAudioSource &source)
        {
            if (!_initialized)
            {
                return;
            }

            alDeleteSources(1, (ALuint *)&source._id);
            AL_CHECK_ERROR();
        }

        void setAudioSourcePitch(const iAudioSource &source, float32 pitch)
        {
            if (!_initialized)
            {
                return;
            }

            alSourcef(source._id, AL_PITCH, pitch);
            AL_CHECK_ERROR();
        }

        void setAudioSourceGain(const iAudioSource &source, float32 gain)
        {
            if (!_initialized)
            {
                return;
            }

            alSourcef(source._id, AL_GAIN, gain);
            AL_CHECK_ERROR();
        }

        void setAudioSourceLoop(const iAudioSource &source, bool loop)
        {
            if (!_initialized)
            {
                return;
            }

            alSourcei(source._id, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
            AL_CHECK_ERROR();
        }

        void updateSource(const iAudioSource &source, const iaVector3d &position, const iaVector3d velocity)
        {
            if (!_initialized)
            {
                return;
            }

            alSource3f(source._id, AL_POSITION, position._x, position._y, position._z);
            AL_CHECK_ERROR();
            alSource3f(source._id, AL_VELOCITY, velocity._x, velocity._y, velocity._z);
            AL_CHECK_ERROR();
        }

        void playSource(const iAudioSource &source)
        {
            if (!_initialized)
            {
                return;
            }

            alSourcePlay(source._id);
            AL_CHECK_ERROR();
        }

        void stopSource(const iAudioSource &source)
        {
            if (!_initialized)
            {
                return;
            }

            alSourceStop(source._id);
            AL_CHECK_ERROR();
        }

        void bindSource(const iAudioSource &source, iAudioBuffer &buffer)
        {
            if (!_initialized)
            {
                return;
            }

            alSourcei(source._id, AL_BUFFER, buffer._id);
            AL_CHECK_ERROR();
        }

    private:
        bool _initialized = false;
        ALCdevice *_device = nullptr;
        ALCcontext *_context = nullptr;
    };

    iAudio::iAudio()
    {
        _impl = new iAudioImpl();
    }

    iAudio::~iAudio()
    {
        delete _impl;
    }

    void iAudio::destroyAudioBuffer(const iAudioBuffer &buffer)
    {
        _impl->destroyAudioBuffer(buffer);
    }

    bool iAudio::createAudioBuffer(iAudioBuffer &audioBuffer, int16 numChannels, int16 bitsPerSample, int32 sampleRate, const char *buffer, int32 bufferSize)
    {
        return _impl->createAudioBuffer(audioBuffer, numChannels, bitsPerSample, sampleRate, buffer, bufferSize);
    }

    void iAudio::updateListener(const iaMatrixd &matrix, const iaVector3d velocity)
    {
        _impl->updateListener(matrix, velocity);
    }

    bool iAudio::createAudioSource(iAudioSource &source)
    {
        return _impl->createAudioSource(source);
    }

    void iAudio::destroyAudioSource(const iAudioSource &source)
    {
        _impl->destroyAudioSource(source);
    }

    void iAudio::setAudioSourcePitch(const iAudioSource &source, float32 pitch)
    {
        _impl->setAudioSourcePitch(source, pitch);
    }

    void iAudio::setAudioSourceGain(const iAudioSource &source, float32 gain)
    {
        _impl->setAudioSourceGain(source, gain);
    }

    void iAudio::setAudioSourceLoop(const iAudioSource &source, bool loop)
    {
        _impl->setAudioSourceLoop(source, loop);
    }

    void iAudio::updateSource(const iAudioSource &source, const iaVector3d &position, const iaVector3d velocity)
    {
        _impl->updateSource(source, position, velocity);
    }

    void iAudio::playSource(const iAudioSource &source)
    {
        _impl->playSource(source);
    }

    void iAudio::stopSource(const iAudioSource &source)
    {
        _impl->stopSource(source);
    }

    void iAudio::bindSource(const iAudioSource &source, iResourcePtr resource)
    {
        auto sound = std::dynamic_pointer_cast<iSound>(resource);
        if (sound != nullptr)
        {
            _impl->bindSource(source, sound->_buffer);
        }
    }

} // namespace igor