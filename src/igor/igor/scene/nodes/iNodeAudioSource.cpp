// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeAudioSource.h>

#include <igor/scene/iScene.h>
#include <igor/audio/iAudio.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	iNodeAudioSource::iNodeAudioSource()
		: iNode()
	{
		setName(L"iNodeAudioSource");
		_nodeType = iNodeType::iNodeAudioSource;
		_nodeKind = iNodeKind::Audio;

		iAudio::getInstance().createAudioSource(_source);
	}

	iNodeAudioSource::iNodeAudioSource(iNodeAudioSource *node)
		: iNode()
	{
		con_assert(node != nullptr, "zero pointer");

		_nodeType = node->_nodeType;
		_nodeKind = node->_nodeKind;

		setName(node->getName());

		iAudio::getInstance().createAudioSource(_source);
	}

	iNodeAudioSource::~iNodeAudioSource()
	{
		setScene(nullptr);
		iAudio::getInstance().destroyAudioSource(_source);
	}

	void iNodeAudioSource::setSound(iResourcePtr sound)
	{
		iAudio::getInstance().bindSource(_source, sound);
	}

	void iNodeAudioSource::play()
	{
		iAudio::getInstance().playSource(_source);
	}

	void iNodeAudioSource::stop()
	{
		iAudio::getInstance().stopSource(_source);
	}

	void iNodeAudioSource::onUpdateTransform(iaMatrixd &matrix)
	{
		_velocity = _position - matrix._pos;
		_position= matrix._pos;

		iAudio::getInstance().updateSource(_source, _position, _velocity);
	}

	void iNodeAudioSource::setPitch(float32 pitch)
	{
		_pitch = pitch;
		iAudio::getInstance().setAudioSourcePitch(_source, _pitch);
	}
	float32 iNodeAudioSource::getPitch() const
	{
		return _pitch;
	}
	void iNodeAudioSource::setGain(float32 gain)
	{
		_gain = gain;
		iAudio::getInstance().setAudioSourceGain(_source, _gain);
	}
	float32 iNodeAudioSource::getGain() const
	{
		return _gain;
	}
	void iNodeAudioSource::setLoop(bool loop)
	{
		_isLooped = loop;
		iAudio::getInstance().setAudioSourceLoop(_source, _isLooped);
	}
	bool iNodeAudioSource::isLooped() const
	{
		return _isLooped;
	}

}; // namespace igor