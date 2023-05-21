// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/animation/iAnimationFactory.h>

#include <igor/resources/iResourceManager.h>

namespace igor
{

    const iaString &iAnimationFactory::getType() const
    {
        const static iaString typeName(L"animation");
        return typeName;
    }

    iResourcePtr iAnimationFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iAnimation(parameters));
    }

    bool iAnimationFactory::loadResource(iResourcePtr resource)
    {
        const auto &parameters = resource->getParameters();
        iAnimationPtr animation = std::dynamic_pointer_cast<iAnimation>(resource);

        // TODO load from file once there is a file format. and then override data from files with additional parameters

        const auto translateAnimation = parameters.getParameter<iaKeyFrameGraphVector3d>("translateAnimation");
        if (!translateAnimation.isEmpty())
        {
            animation->setTranslateAnimation(translateAnimation);
        }

        const auto rotateAnimation = parameters.getParameter<iaKeyFrameGraphVector3d>("rotateAnimation");
        if (!rotateAnimation.isEmpty())
        {
            animation->setRotateAnimation(rotateAnimation);
        }

        const auto scaleAnimation = parameters.getParameter<iaKeyFrameGraphVector3d>("scaleAnimation");
        if (!scaleAnimation.isEmpty())
        {
            animation->setScaleAnimation(scaleAnimation);
        }

        return true;
    }

    void iAnimationFactory::unloadResource(iResourcePtr resource)
    {
        con_info("released animation \"" << resource->getName() << "\"");
    }

    iaString iAnimationFactory::getHashData(const iParameters &parameters) const
    {
        return "";
    }

    bool iAnimationFactory::matchingType(const iParameters &parameters) const
    {
        if (parameters.getParameter<iaString>("type") == getType())
        {
            return true;
        }

        return false;
    }

}; // namespace igor