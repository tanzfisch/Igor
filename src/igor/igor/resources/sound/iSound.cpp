// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/sound/iSound.h>

namespace igor
{

	iSound::iSound(const iParameters &parameters)
		: iResource(parameters)
	{
	}

	int16 iSound::getNumberOfChannels() const
	{
		return _numChannels;
	}

	int32 iSound::getSampleRate() const
	{
		return _sampleRate;
	}

	int32 iSound::getByteRate() const
	{
		return _byteRate;
	}

	int16 iSound::getBitsPerSample() const
	{
		return _bitsPerSample;
	}

}; // namespace igor