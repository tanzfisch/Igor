// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
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

	void iAction::setDescription(const iaString &brief, const iaString &description)
	{
		_description = description;
		_brief = brief;
	}

	const iaString &iAction::getBrief() const
	{
		return _brief;
	}

	const iaString &iAction::getDescription() const
	{
		return _description;
	}

	void iAction::setIcon(const iaString &alias)
	{
		_alias = alias;
	}

	const iaString &iAction::getIcon() const
	{
		return _alias;
	}

} // namespace igor
