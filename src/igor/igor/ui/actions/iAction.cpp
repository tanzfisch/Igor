// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/actions/iAction.h>

namespace igor
{

	iAction::iAction(const iaString &name)
		: _name(name)
	{
	}

	iaString iAction::getName() const
	{
		return _name;
	}

	void iAction::setDescription(const iaString &description)
	{
		_description = description;
	}

	const iaString &iAction::getDescription() const
	{
		return _description;
	}

	void iAction::setPicturePath(const iaString &filename)
	{
		_picture = filename;
	}

	const iaString &iAction::getPicturePath() const
	{
		return _picture;
	}

} // namespace igor
