// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/animation/iAnimation.h>

namespace igor
{

	iAnimation::iAnimation(const iParameters &parameters)
		: iResource(parameters)
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

	bool iAnimation::hasFrameIndexAnimation() const
	{
		return !_frameIndex.isEmpty();
	}

	const uint32 iAnimation::getFrameIndex(float64 t) const
	{
		return _frameIndex.getValue(t);
	}

	void iAnimation::setTranslateAnimation(const iaKeyFrameGraphVector3d &translate)
	{
		_translate = translate;
	}

	void iAnimation::setRotateAnimation(const iaKeyFrameGraphVector3d &rotate)
	{
		_rotate = rotate;
	}

	void iAnimation::setScaleAnimation(const iaKeyFrameGraphVector3d &scale)
	{
		_scale = scale;
	}

	void iAnimation::setFrameIndexAnimation(const iaKeyFrameGraphui &frameIndex)
	{
		_frameIndex = frameIndex;
	}

	const iaTime iAnimation::getStart() const
	{
		iaTime result = iaTime::fromSeconds(999999999);

		if (!_translate.isEmpty())
		{
			result = std::min(result, iaTime::fromSeconds(_translate.getValues().front().first));
		}

		if (!_rotate.isEmpty())
		{
			result = std::min(result, iaTime::fromSeconds(_rotate.getValues().front().first));
		}

		if (!_scale.isEmpty())
		{
			result = std::min(result, iaTime::fromSeconds(_scale.getValues().front().first));
		}

		if (!_frameIndex.isEmpty())
		{
			result = std::min(result, iaTime::fromSeconds(_frameIndex.getValues().front().first));
		}

		return result;
	}

	const iaTime iAnimation::getStop() const
	{
		iaTime result;

		if (!_translate.isEmpty())
		{
			result = std::max(result, iaTime::fromSeconds(_translate.getValues().back().first));
		}

		if (!_rotate.isEmpty())
		{
			result = std::max(result, iaTime::fromSeconds(_rotate.getValues().back().first));
		}

		if (!_scale.isEmpty())
		{
			result = std::max(result, iaTime::fromSeconds(_scale.getValues().back().first));
		}

		if (!_frameIndex.isEmpty())
		{
			result = std::max(result, iaTime::fromSeconds(_frameIndex.getValues().back().first));
		}

		return result;
	}

}; // namespace igor