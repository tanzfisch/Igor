// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeAudioListener.h>

#include <igor/scene/iScene.h>
#include <igor/audio/iAudio.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	uint64 iNodeAudioListener::_activeListener = IGOR_INVALID_ID;

	iNodeAudioListener::iNodeAudioListener()
		: iNode()
	{
		setName(L"iNodeAudioListener");
		_nodeType = iNodeType::iNodeAudioListener;
		_nodeKind = iNodeKind::Audio;
	}

	iNodeAudioListener::iNodeAudioListener(iNodeAudioListener *node)
		: iNode()
	{
		con_assert(node != nullptr, "zero pointer");

		_nodeType = node->_nodeType;
		_nodeKind = node->_nodeKind;

		setName(node->getName());
	}

	iNodeAudioListener::~iNodeAudioListener()
	{
		setScene(nullptr);
	}

	void iNodeAudioListener::setActiveListener()
	{
		_activeListener = getID();
	}

	bool iNodeAudioListener::isActiveListener() const
	{
		return _activeListener == getID();
	}

	void iNodeAudioListener::getWorldMatrix(iaMatrixd &matrix)
	{
		matrix = _worldMatrix;
	}

	void iNodeAudioListener::onUpdateTransform(iaMatrixd &matrix)
	{
		iaVector3d velocity = _worldMatrix._pos - matrix._pos;

		// skip big jumps
		if (velocity.length2() > 100.0 * 100.0)
		{
			velocity.set(0,0,0);
		}

		_worldMatrix = matrix;

		if (isActiveListener())
		{
			iAudio::getInstance().updateListener(_worldMatrix, velocity);
		}
	}

}; // namespace igor
