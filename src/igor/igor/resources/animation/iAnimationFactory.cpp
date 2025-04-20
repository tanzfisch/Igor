// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/animation/iAnimationFactory.h>

#include <igor/resources/iResourceManager.h>
#include <igor/utils/iJson.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

namespace igor
{

    iAnimationFactory::iAnimationFactory()
        : iFactory(IGOR_RESOURCE_ANIMATION, IGOR_SUPPORTED_ANIMATION_EXTENSIONS)
    {
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

        if (directData)
        {
            return true;
        }

        iaString filepath = iResourceManager::getInstance().getFilename(resource->getID());
        if (filepath.isEmpty())
        {
            filepath = resource->getSource();
        }
        const iaString fullFilepath = iResourceManager::getInstance().resolvePath(filepath);
        return load(fullFilepath, animation);
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

    bool iAnimationFactory::load(const iaString &filename, iAnimationPtr animation)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ifstream file(temp);
        json data = json::parse(file);

        if (!data.is_array())
        {
            con_err("incompatible data");
            return false;
        }

        // TODO refactor
        for (auto element : data)
        {
            if(!element.contains("animation"))
            {
                continue;
            }

            json animationJson = element["animation"];

            if (!animationJson.contains("frames"))
            {
                con_err("incompatible data");
                return false;
            }
            json framesJson = animationJson["frames"];

            const iaString keyFrameType = iJson::getValue<iaString>(animationJson, "keyFrameType", "int");
            const iaString target = iJson::getValue<iaString>(animationJson, "target", "FrameIndex");

            iInterpolationMode interpolationMode = iInterpolationMode::None;
            if (animationJson.contains("interpolationMode"))
            {
                interpolationMode = getInterpolationMode(animationJson["interpolationMode"].get<iaString>());
            }

            if (keyFrameType == "int" &&
                target == "FrameIndex")
            {
                iaKeyFrameGraphui result;

                for (auto element : framesJson)
                {
                    json frame = element["frame"];

                    if (!frame.contains("time") ||
                        !frame.contains("value"))
                    {
                        continue;
                    }

                    float64 time = frame["time"].get<float64>();
                    uint32 value = frame["value"].get<uint32>();

                    result.setValue(time, value);
                }

                result.setInterpolationMode(interpolationMode);
                animation->setFrameIndexAnimation(result);
                return true;
            }

            if (keyFrameType == "Vector3")
            {
                iaKeyFrameGraphVector3d result;

                for (auto element : framesJson)
                {
                    json frame = element["frame"];

                    if (!frame.contains("time") ||
                        !frame.contains("value"))
                    {
                        continue;
                    }

                    float64 time = frame["time"].get<float64>();
                    iaVector3d value = frame["value"].get<iaVector3d>();

                    result.setValue(time, value);
                }

                result.setInterpolationMode(interpolationMode);

                if (target == "Translate")
                {
                    animation->setTranslateAnimation(result);
                }

                if (target == "Rotate")
                {
                    animation->setRotateAnimation(result);
                }

                if (target == "Scale")
                {
                    animation->setScaleAnimation(result);
                }
            }
        }

        return true;
    }

    void iAnimationFactory::unloadResource(iResourcePtr resource)
    {
        // nothing to do here
    }

    iaString iAnimationFactory::getHashData(const iParameters &parameters) const
    {
        return "";
    }

}; // namespace igor