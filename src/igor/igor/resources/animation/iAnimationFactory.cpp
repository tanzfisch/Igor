// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/animation/iAnimationFactory.h>

#include <igor/resources/iResourceManager.h>

#include <tinyxml.h>

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
        const auto rotateAnimation = parameters.getParameter<iaKeyFrameGraphVector3d>("rotateAnimation");
        const auto scaleAnimation = parameters.getParameter<iaKeyFrameGraphVector3d>("scaleAnimation");
        const auto spriteAnimation = parameters.getParameter<iaKeyFrameGraphui>("spriteAnimation");

        bool directData = false;

        if (!translateAnimation.isEmpty())
        {
            animation->setTranslateAnimation(translateAnimation);
            directData = true;
        }
        
        if (!rotateAnimation.isEmpty())
        {
            animation->setRotateAnimation(rotateAnimation);
            directData = true;
        }

        if (!scaleAnimation.isEmpty())
        {
            animation->setScaleAnimation(scaleAnimation);
            directData = true;
        }
        
        if (!spriteAnimation.isEmpty())
        {
            animation->setFrameIndexAnimation(spriteAnimation);
            directData = true;
        }

        if(directData)
        {
            return true;
        }

        const iaString filename = iResourceManager::getInstance().getPath(resource->getName());
        return loadAnimation(filename, animation);
    }

    iaKeyFrameGraphui iAnimationFactory::readFrameIndexAnimation(TiXmlElement *animationElement, iAnimationPtr animation)
    {
        iaKeyFrameGraphui result;
        TiXmlElement *frame = animationElement->FirstChildElement("Frame");

        do
        {
            float64 time;
            frame->Attribute("time", &time);
            int value;
            frame->Attribute("value", &value);

            result.setValue(time, value);

        } while ((frame = frame->NextSiblingElement("Frame")) != nullptr);

        return result;
    }

    static iInterpolationMode getInterpolationMode(const iaString &mode)
    {
        if (mode == "None")
        {
            return iInterpolationMode::None;
        }

        // default
        return iInterpolationMode::Linear;
    }

    void iAnimationFactory::readAnimationElement(TiXmlElement *animationElement, iAnimationPtr animation)
    {
        const iaString keyFrameType(animationElement->Attribute("keyFrameType"));
        const iaString target(animationElement->Attribute("target"));
        iInterpolationMode interpolationMode = getInterpolationMode(animationElement->Attribute("interpolationMode"));
        float64 fps;
        animationElement->Attribute("fps", &fps); // TODO

        if (keyFrameType == "uint32" &&
            target == "FrameIndex")
        {
            iaKeyFrameGraphui result = readFrameIndexAnimation(animationElement, animation);
            result.setInterpolationMode(interpolationMode);
            animation->setFrameIndexAnimation(result);
        }

        animation->setFps(fps);
    }

    bool iAnimationFactory::loadAnimation(const iaString &filename, iAnimationPtr animation)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        TiXmlDocument document(temp);
        if (!document.LoadFile())
        {
            con_err("can't read \"" << filename << "\". " << document.ErrorDesc());
            return false;
        }

        TiXmlElement *root = document.FirstChildElement("Igor");
        if (root != nullptr)
        {
            TiXmlElement *animationElement = root->FirstChildElement("Animation");
            if (animationElement != nullptr)
            {
                readAnimationElement(animationElement, animation);
            }
        }

        con_info("loaded animation \"" << animation->getName());
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