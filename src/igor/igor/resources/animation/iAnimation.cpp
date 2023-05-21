// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/animation/iAnimation.h>

namespace igor
{

	iAnimation::iAnimation(const iParameters &parameters)
		: iResource("animation", parameters)
	{
	}

	bool iAnimation::hasTranslateAnimation() const
	{
		return !_translate.isEmpty();
	}

	const iaVector3d iAnimation::getTranslate(float64 t) const
	{
		return _translate.getValue(t);
	}

	bool iAnimation::hasRotateAnimation() const
	{
		return !_rotate.isEmpty();
	}

	const iaVector3d iAnimation::getRotate(float64 t) const
	{
		return _rotate.getValue(t);
	}

	bool iAnimation::hasScaleAnimation() const
	{
		return !_scale.isEmpty();
	}

	const iaVector3d iAnimation::getScale(float64 t) const
	{
		return _scale.getValue(t);
	}

	void iAnimation::setTranslateAnimation(iaKeyFrameGraphVector3d translate)
	{
		_translate = translate;
	}

	void iAnimation::setRotateAnimation(iaKeyFrameGraphVector3d rotate)
	{
		_rotate = rotate;
	}

	void iAnimation::setScaleAnimation(iaKeyFrameGraphVector3d scale)
	{
		_scale = scale;
	}

}; // namespace igor