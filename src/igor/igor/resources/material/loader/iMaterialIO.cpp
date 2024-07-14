// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/loader/iMaterialIO.h>

#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaFile.h>
#include <igor/utils/iJson.h>

#include <fstream>
#include <iostream>

namespace igor
{
    bool iMaterialIO::read(const iaString &filename, const iMaterialPtr &material)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ifstream file(temp);
        json data = json::parse(file);

        if (!data.contains("material"))
        {
            return false;
        }

        json materialJson = data["material"];
        if (!materialJson.contains("shader_material"))
        {
            return false;
        }
        const iaString shaderMaterial = materialJson["shader_material"].get<iaString>();
        if (!shaderMaterial.isEmpty())
        {
            material->setShader(iResourceManager::getInstance().loadResource<iShader>(shaderMaterial));
        }

        if (materialJson.contains("diffuse"))
        {
            material->setDiffuse(materialJson["diffuse"].get<iaColor3f>());
        }

        if (materialJson.contains("ambient"))
        {
            material->setAmbient(materialJson["ambient"].get<iaColor3f>());
        }

        if (materialJson.contains("specular"))
        {
            material->setSpecular(materialJson["specular"].get<iaColor3f>());
        }

        if (materialJson.contains("emissive"))
        {
            material->setEmissive(materialJson["emissive"].get<iaColor3f>());
        }

        if (materialJson.contains("shininess"))
        {
            material->setShininess(materialJson["shininess"].get<float32>());
        }

        if (materialJson.contains("alpha"))
        {
            material->setAlpha(materialJson["alpha"].get<float32>());
        }

        if (materialJson.contains("tiling"))
        {
            material->setTiling(materialJson["tiling"].get<iaVector2f>());
        }

        if (materialJson.contains("textures"))
        {
            auto textures = materialJson["textures"];

            for (auto textureJson : textures)
            {
                if (!textureJson.contains("unit") ||
                    !textureJson.contains("uuid"))
                {
                    continue;
                }

                uint32 texUnit = textureJson["unit"].get<uint32>();
                iResourceID textureID(textureJson["uuid"].get<iaString>());
                material->setTexture(iResourceManager::getInstance().requestResource<iTexture>(textureID), texUnit);
            }
        }

        return true;
    }

    bool iMaterialIO::write(const iaString &filename, const iMaterialPtr &material)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ofstream stream;
        stream.open(temp);

        if (!stream.is_open())
        {
            con_err("can't open to write \"" << temp << "\"");
            return false;
        }

        json materialJson = {
            {"diffuse", material->getDiffuse()},
            {"ambient", material->getAmbient()},
            {"specular", material->getSpecular()},
            {"emissive", material->getEmissive()},
            {"shininess", material->getShininess()},
            {"alpha", material->getAlpha()},
            {"tiling", material->getTiling()}};        

        if (material->getShader())
        {
            materialJson["shader_material"] = material->getShader()->getID().toString();
        }

        json textures = json::array();
        for (const auto &tex : material->getTextures())
        {
            if (tex.second == nullptr)
            {
                continue;
            }

            json texture = {
                {"unit", tex.first},
                {"uuid", tex.second->getID()}};

            textures.push_back(texture);
        }

        if (!textures.empty())
        {
            materialJson["textures"] = textures;
        }

        json dataJson;
        dataJson["material"] = materialJson;
        stream << dataJson.dump(4);

        return true;
    }

} // namespace igor
