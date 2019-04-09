// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iSoundResourceFactory.h>

#include <iResourceManager.h>
#include <iaConsole.h>
#include <iRenderer.h>
#include <iaMatrix.h>

#include <al.h>
#include <AL/alut.h>
//#include <vorbis/vorbisfile.h>

namespace Igor
{

	vector<iSound*> listener_linked_sources; // liste der sourcen die sich mit dem listener mitbewegen

#ifdef __IGOR_DEBUG__
#define AL_CHECK_ERROR()\
	do\
	{\
	ALenum error = alGetError();\
	if (error != AL_NO_ERROR)\
	{\
	switch(error)\
	{\
	case AL_INVALID_NAME:		con_err("AL_INVALID_NAME"); break;\
	case AL_INVALID_ENUM:		con_err("AL_INVALID_ENUM"); break;\
	case AL_INVALID_VALUE:		con_err("AL_INVALID_VALUE"); break;\
	case AL_INVALID_OPERATION:	con_err("AL_INVALID_OPERATION"); break;\
	case AL_OUT_OF_MEMORY:		con_err("AL_OUT_OF_MEMORY"); break;\
	default: con_err("UNKNOWN");\
	};\
	}\
		else break;\
	} while(0)
#else
#define AL_CHECK_ERROR() 1
#endif

	iSoundResourceFactory::iSoundResourceFactory()
	{
		alutInit(0,NULL);												AL_CHECK_ERROR();
	}

	iSoundResourceFactory::~iSoundResourceFactory()
	{
		if(sounds.size())
			con_warn(sounds.size() << " sounds(s) not released");
		sounds.clear();

		alutExit();														AL_CHECK_ERROR();

		listener_linked_sources.clear();
	}

	void iSoundResourceFactory::setListenerMatrix(iaMatrixf &matrix, bool calc_velocity)
	{
		if(calc_velocity)
		{
			setListenerVelocity(matrix.pos - listener_matrix.pos);
		}

		listener_matrix = matrix;

		alListenerfv(AL_POSITION, matrix.pos.getData());				AL_CHECK_ERROR();
		float32 ori[6] = {matrix.depth.x,matrix.depth.y,matrix.depth.z, matrix.top.x,matrix.top.y,matrix.top.z};
		alListenerfv(AL_ORIENTATION, ori);								AL_CHECK_ERROR();

		// update linked sources
		for(int i=0;i<listener_linked_sources.size();++i)
			listener_linked_sources[i]->setSourcePosition(listener_matrix.pos);
	}

	void iSoundResourceFactory::setListenerVelocity(iaVector3f &vel)
	{
		alListenerfv(AL_VELOCITY, vel.getData());						AL_CHECK_ERROR();
	}

	iSoundBuffer* iSoundResourceFactory::loadWAVBuffer(iaString filename)
	{
		ALenum     format;
		ALsizei    size;
		ALsizei    freq;
		ALboolean  loop;
		ALvoid*    data;

		iSoundBuffer *buffer = new iSoundBuffer(filename);

		alutLoadWAVFile((ALbyte*)filename.c_str(), &format, &data, &size, &freq, &loop);
		if (alGetError() != AL_NO_ERROR)
		{
            con_warn("can't load file \"" << filename << "\"");
			return buffer;
		}

		alBufferData(buffer->bufferid,format,data,size,freq);
		if (alGetError() != AL_NO_ERROR)
		{
			con_err("can't fill buffer with \"" << filename << "\"");
			return buffer;
		}

		alutUnloadWAV(format,data,size,freq);						AL_CHECK_ERROR();

		buffer->isdummy = false;

		return buffer;
	}

	// ogg wird bis aus weiteres nicht mehr integriert
	/*
	uint32 iSoundResourceFactory::loadOGGBuffer(iaString filename)
	{
	for(uint32 i=0;i<buffers.size();i++)
	{
	if(buffers[i]->filename == filename)
	{
	buffers[i]->refcount++;
	return buffers[i];
	}
	}

	iSoundBuffer *buffer = new iSoundBuffer();
	buffer->filename = filename;
	buffer->isdummy = true;

	vector<char> char_buffer;
	int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
	int bitStream;
	long bytes;
	char array[1024 * 64];
	FILE *file;
	ALenum format;
	ALsizei freq;
	vorbis_info *pInfo;
	OggVorbis_File oggFile;


	if(!(file = fopen(filename.c_str(), "rb")))
	{
	con_err("iSoundResourceFactory::loadOGGBuffer - can't open file \"" << filename << "\"" << endl;
	return buffer;
	}

	if(ov_open(file, &oggFile, NULL, 0) < 0)
	{
	fclose(file);

	con_err("iSoundResourceFactory::loadOGGBuffer - can't read file \"" << filename << "\"" << endl;
	return buffer;
	}

	// Get some information about the OGG file
	pInfo = ov_info(&oggFile, -1);

	// Check the number of channels... always use 16-bit samples
	if (pInfo->channels == 1)
	format = AL_FORMAT_MONO16;
	else
	format = AL_FORMAT_STEREO16;
	// end if

	// The frequency of the sampling rate
	freq = pInfo->rate;

	do
	{
	// Read up to a buffer's worth of decoded sound data
	bytes = ov_read(&oggFile, array, 1024 * 64, endian, 2, 1, &bitStream);
	// Append to end of buffer
	char_buffer.insert(char_buffer.end(), array, array + bytes);
	} while (bytes > 0);

	ov_clear(&oggFile);
	//! \todo ??? fclose(file);

	alBufferData(buffer->id, format, &char_buffer[0], (ALsizei)char_buffer.size(), freq);
	buffer->isdummy = false;

	return buffer;
	}*/

	void iSoundResourceFactory::unloadFile(iSound *sound)
	{
		if(!sound) return;

		vector<iSound*>::iterator it;

		for(it=sounds.begin();it<sounds.end();it++)
		{
			if((*it) == sound)
			{
				if((*it)->buffer->refcount > 0)
				{
					(*it)->buffer->refcount--;
					if((*it)->buffer->refcount == 0)
					{
						vector<iSoundBuffer*>::iterator it_buffer;
						for(it_buffer=buffers.begin();it_buffer!=buffers.end();++it_buffer)
						{
							if((*it_buffer) == (*it)->buffer)
							{
								delete (*it_buffer);
								buffers.erase(it_buffer);
								break;
							}
						}
					}
				}
				else
				{
					con_err("refcount underrun");
				}

				delete (*it);
				sounds.erase(it);
				return;
			}
		}
	}

	void iSoundResourceFactory::flush()
	{
		/*	vector<iSound*>::iterator it;pre

		it = sounds.begin();
		while(it != sounds.end())
		{
		if((*it)->buffer->refcount == 0)
		{
		con << "unloading sound \"" << (*it)->buffer->filename << "\"" << endl;
		delete (*it);
		it = sounds.erase(it);
		}
		else
		{
		if((*it)->buffer->isdummy)
		{
		(*it)->buffer->bufferid = loadBuffer((*it)->buffer->filename);
		if(alIsBuffer((*it)->buffer->bufferid))
		(*it)->buffer->isdummy = false;
		}
		it++;
		}
		}*/
	}

	iSoundBuffer* iSoundResourceFactory::loadBuffer(iaString filename)
	{
        filename = iResourceManager::getInstance().getSoundFolder(filename);

		iSoundBuffer *buffer = 0;

		iaString fileex = filename.substr(filename.find_last_of('.')+1);
		if(fileex == "wav")
		{
			buffer = loadWAVBuffer(filename);
		}
		/*	else if(fileex == "ogg")
		{
		return loadOGGBuffer(filename);
		}*/
		else
		{
			con_warn("can't identify format of \"" << filename << "\". Fall back to WAV");
			buffer = loadWAVBuffer(filename);
		}

		buffers.push_back(buffer);

		return buffer;
	}

	iSound* iSoundResourceFactory::loadFile(iaString filename)
	{
		for(uint32 i=0;i<sounds.size();i++)
		{
			if(sounds[i]->buffer->filename == filename)
			{
				sounds[i]->buffer->refcount++;
				return sounds[i];
			}
		}

		iSound *temp = new iSound(loadBuffer(filename));

		if(!temp->buffer->isdummy)
			alSourcei(temp->sourceid, AL_BUFFER, temp->buffer->bufferid);			AL_CHECK_ERROR();

		if(temp) sounds.push_back(temp);
		temp->buffer->refcount ++;
		return temp;
	}

	////////////// iSoundBuffer

	iSoundBuffer::iSoundBuffer(iaString filename)
	{
		this->filename = filename;
		isdummy = true;
		refcount = 0;

		alGenBuffers(1, (ALuint*)&bufferid);							AL_CHECK_ERROR();
		if(!alIsBuffer(bufferid))
			con_err("invalid buffer number");
	}

	iSoundBuffer::~iSoundBuffer()
	{
		alDeleteBuffers(1, (ALuint*)&bufferid);							AL_CHECK_ERROR(); 
	}

	////////////// iSound Methoden ////////////////

	iSound::iSound(iSoundBuffer *buffer)
	{
		this->buffer = buffer;
		loop = false;

		alGenSources(1, (ALuint*)&sourceid);							AL_CHECK_ERROR();
		if(!alIsSource(sourceid))
			con_err("invalid source number");
	}

	iSound::~iSound()
	{
		alDeleteSources(1, (ALuint*)&sourceid);							AL_CHECK_ERROR();
	}

	void iSound::setSourcePosition(iaVector3f &vec)
	{
		alSourcefv(sourceid, AL_POSITION, vec.getData());				AL_CHECK_ERROR();
	}

	void iSound::linkToListener()
	{
		listener_linked_sources.push_back(this);
	}

	void iSound::unlinkFromListener()
	{
		vector<iSound*>::iterator it;
		for(it=listener_linked_sources.begin();it!=listener_linked_sources.end();it++)
		{
			if((*it) == this)
			{
				listener_linked_sources.erase(it);
				break;
			}
		}
	}

	void iSound::setSourceVelocity(iaVector3f &vel)
	{
		alSourcefv(sourceid, AL_VELOCITY, vel.getData());				AL_CHECK_ERROR();
	}

	void iSound::play(bool loop)
	{
		alSourcei(sourceid, AL_LOOPING, loop);							AL_CHECK_ERROR();
		alSourcePlay(sourceid);											AL_CHECK_ERROR();
	}

	void iSound::pause()
	{
		alSourcePause(sourceid);										AL_CHECK_ERROR();
	}

	void iSound::stop()
	{
		alSourceStop(sourceid);											AL_CHECK_ERROR();
	}

	void iSound::rewind()
	{
		alSourceRewind(sourceid);										AL_CHECK_ERROR();
	}

	/////////////// iSoundNode

/*	iSoundNode::iSoundNode()
	{
		firsttime = true;
	}

	iSoundNode::~iSoundNode()
	{
		for(int i=0;i<sounds.size();++i)
		{
			iSoundResourceFactory::getInstance().unloadFile(sounds[i]);
		}

		sounds.clear();
		activesounds.clear();
	}

	void iSoundNode::addSoundFile(iaString filename)
	{
		sounds.push_back(iSoundResourceFactory::getInstance().loadFile(filename));
	}

	void iSoundNode::preRenderCustom()
	{
		if(firsttime)
		{
			sound_position = iRenderer::getInstance().getModelViewMatrix().pos;
			firsttime = false;
		}

		for(int i=0;i<activesounds.size();++i)
		{
			activesounds[i]->setSourcePosition(iRenderer::getInstance().getModelViewMatrix().pos);
			activesounds[i]->setSourceVelocity(iRenderer::getInstance().getModelViewMatrix().pos - sound_position);
		}

		sound_position = iRenderer::getInstance().getModelViewMatrix().pos;
	}

	void iSoundNode::play(uint32 soundid, bool loop)
	{
		if(soundid >= sounds.size()) return;
		activesounds.push_back(sounds[soundid]);
		sounds[soundid]->play(loop);
	}

	void iSoundNode::stop(uint32 soundid)
	{
		if(soundid >= sounds.size()) return;

		vector<iSound*>::iterator it;
		for(it=activesounds.begin();it!=activesounds.end();++it)
		{
			if((*it) == sounds[soundid])
			{
				activesounds.erase(it);
				break;
			}
		}

		sounds[soundid]->stop();
	}

	void iSoundNode::pause(uint32 soundid)
	{
		if(soundid >= sounds.size()) return;

		vector<iSound*>::iterator it;
		for(it=activesounds.begin();it!=activesounds.end();++it)
		{
			if((*it) == sounds[soundid])
			{
				activesounds.erase(it);
				break;
			}
		}

		sounds[soundid]->pause();
	}

	void iSoundNode::rewind(uint32 soundid)
	{
		if(soundid >= sounds.size()) return;
		sounds[soundid]->rewind();
	}
	*/
};

