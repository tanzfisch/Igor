// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iAction.h>

namespace Igor
{
	void iAction::setText(const iaString& text)
	{
		_text = text;
	}

	const iaString& iAction::getText() const
	{
		return _text;
	}

	void iAction::setPicturePath(const iaString& filename)
	{
		_picture = filename;
	}

	const iaString& iAction::getPicturePath() const
	{
		return _picture;
	}

	void iAction::setIdentifier(const iaString& name)
	{
		_name = name;
	}

	const iaString& iAction::getIdentifier() const
	{
		return _name;
	}
}
