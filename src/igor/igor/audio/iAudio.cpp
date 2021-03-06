#include <igor/audio/iAudio.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

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

            _device = alcOpenDevice(nullptr);
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

            con_info("OpenAL Version : " << alGetString(AL_VERSION) << endlTab
                                         << "OpenAL Vendor  : " << alGetString(AL_VENDOR) << endlTab
                                         << "OpenAL Renderer: " << alGetString(AL_RENDERER));
        }

        ~iAudioImpl()
        {
            con_info("shutdown audio");

            if (_context != nullptr &&
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

        bool createBuffer(iAudioBuffer &audioBuffer, int16 numChannels, int16 bitsPerSample, int32 sampleRate, const char *buffer, int32 bufferSize)
        {
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
                con_err("Can't fill audio buffer: " << numChannels << "ch " << bitsPerSample << "bit " << sampleRate << "Hz");
                return false;
            }

            return true;
        }

        void destroyBuffer(const iAudioBuffer &buffer)
        {
            alDeleteBuffers(1, (ALuint *)&buffer._id);
        }

        void updateListener(const iaMatrixd &matrix, const iaVector3d velocity)
        {
            const ALfloat listenerOri[] = {
                (ALfloat)matrix._depth._x, (ALfloat)matrix._depth._y, (ALfloat)matrix._depth._z,
                (ALfloat)matrix._top._x, (ALfloat)matrix._top._y, (ALfloat)matrix._top._z};

            alListener3f(AL_POSITION, matrix._pos._x, matrix._pos._y, matrix._pos._z);
            AL_CHECK_ERROR();
            alListener3f(AL_VELOCITY, velocity._x, velocity._y, velocity._z);
            AL_CHECK_ERROR();
            alListenerfv(AL_ORIENTATION, listenerOri);
            AL_CHECK_ERROR();
        }

        bool createSource(iAudioSource &audioSource)
        {
            alGenSources((ALuint)1, (ALuint *)&audioSource._id);
            if (!alIsSource(audioSource._id))
            {
                con_err("Can't create audio source");
                return false;
            }
            return true;
        }

        void destroySource(const iAudioSource &source)
        {
            alDeleteSources(1, (ALuint *)&source._id);
            AL_CHECK_ERROR();
        }

        void setSourcePitch(const iAudioSource &source, float32 pitch)
        {
            alSourcef(source._id, AL_PITCH, pitch);
            AL_CHECK_ERROR();
        }

        void setSourceDirectChannel(const iAudioSource &source, bool on)
        {
            alSourcei(source._id, AL_DIRECT_CHANNELS_SOFT, on ? AL_TRUE : AL_FALSE);
            AL_CHECK_ERROR();
        }

        void setSourceGain(const iAudioSource &source, float32 gain)
        {
            alSourcef(source._id, AL_GAIN, gain);
            AL_CHECK_ERROR();
        }

        void setSourceLoop(const iAudioSource &source, bool loop)
        {
            alSourcei(source._id, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
            AL_CHECK_ERROR();
        }

        void updateSource(const iAudioSource &source, const iaVector3d &position, const iaVector3d velocity)
        {
            alSource3f(source._id, AL_POSITION, position._x, position._y, position._z);
            AL_CHECK_ERROR();
            alSource3f(source._id, AL_VELOCITY, velocity._x, velocity._y, velocity._z);
            AL_CHECK_ERROR();
        }

        void playSource(const iAudioSource &source)
        {
            alSourcePlay(source._id);
            AL_CHECK_ERROR();
        }

        void stopSource(const iAudioSource &source)
        {
            alSourceStop(source._id);
            AL_CHECK_ERROR();
        }

        void bindSource(const iAudioSource &source, iAudioBuffer &buffer)
        {
            alSourcei(source._id, AL_BUFFER, buffer._id);
            AL_CHECK_ERROR();
        }

    private:
        /*! audio device
        */
        ALCdevice *_device = nullptr;

        /*! audio context
        */
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

    void iAudio::destroyBuffer(const iAudioBuffer &buffer)
    {
        _impl->destroyBuffer(buffer);
    }

    bool iAudio::createBuffer(iAudioBuffer &audioBuffer, int16 numChannels, int16 bitsPerSample, int32 sampleRate, const char *buffer, int32 bufferSize)
    {
        return _impl->createBuffer(audioBuffer, numChannels, bitsPerSample, sampleRate, buffer, bufferSize);
    }

    void iAudio::updateListener(const iaMatrixd &matrix, const iaVector3d velocity)
    {
        _impl->updateListener(matrix, velocity);
    }

    bool iAudio::createSource(iAudioSource &source)
    {
        return _impl->createSource(source);
    }

    void iAudio::destroySource(const iAudioSource &source)
    {
        _impl->destroySource(source);
    }

    void iAudio::setSourcePitch(const iAudioSource &source, float32 pitch)
    {
        _impl->setSourcePitch(source, pitch);
    }

    void iAudio::setSourceGain(const iAudioSource &source, float32 gain)
    {
        _impl->setSourceGain(source, gain);
    }

    void iAudio::setSourceLoop(const iAudioSource &source, bool loop)
    {
        _impl->setSourceLoop(source, loop);
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
            _impl->setSourceDirectChannel(source, sound->getNumberOfChannels() > 1);
            _impl->bindSource(source, sound->_buffer);
        }
    }

} // namespace igor