// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/actions/iAction.h>

namespace Igor
{
	void iAction::setText(const iaString &text)
	{
		_text = text;
	}

	const iaString &iAction::getText() const
	{
		return _text;
	}

	void iAction::setPicturePath(const iaString &filename)
	{
		_picture = filename;
	}

	const iaString &iAction::getPicturePath() const
	{
		return _picture;
	}

	int64 iAction::getID() const
	{
		return _name.getHashValue();
	}

	void iAction::setIDName(const iaString &name)
	{
		_name = name;
	}

	const iaString &iAction::getIDName() const
	{
		return _name;
	}
} // namespace Igor
