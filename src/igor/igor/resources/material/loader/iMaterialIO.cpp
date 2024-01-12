// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/loader/iMaterialIO.h>

#include <igor/resources/iResourceManager.h>
#include <igor/data/iXMLHelper.h>

#include <iaux/system/iaFile.h>

#include <tinyxml.h>

#include <fstream>
#include <iostream>

namespace igor
{

    bool iMaterialIO::readTextures(TiXmlElement *materialXML, const iMaterialPtr &material)
    {
        TiXmlElement *element = materialXML->FirstChildElement("Texture");
        if (element == nullptr)
        {
            return true;
        }

        do
        {
            const uint32 texUnit = iaString::toInt(iaString(element->Attribute("unit")));

            TiXmlNode *textNode = element->FirstChild();
            if (textNode == nullptr ||
                textNode->Type() != TiXmlNode::NodeType::TINYXML_TEXT)
            {
                continue;
            }

            iResourceID textureID(iaString(textNode->ValueStr().c_str()));

            iTexturePtr texture = iResourceManager::getInstance().requestResource<iTexture>(textureID);
            material->setTexture(texture, texUnit);

            element = element->NextSiblingElement("Texture");
        } while (element != nullptr);

        return true;
    }

    bool iMaterialIO::readMaterial(TiXmlElement *materialXML, const iMaterialPtr &material)
    {
        const iaColor3f diffuse = iXMLHelper::getValue<iaColor3f>(materialXML, "Diffuse", iaColor3f(0.5f, 0.5f, 0.5f));
        const iaColor3f ambient = iXMLHelper::getValue<iaColor3f>(materialXML, "Ambient", iaColor3f(0.4f, 0.4f, 0.4f));
        const iaColor3f specular = iXMLHelper::getValue<iaColor3f>(materialXML, "Specular", iaColor3f(0.6f, 0.6f, 0.6f));
        const iaColor3f emissive = iXMLHelper::getValue<iaColor3f>(materialXML, "Emissive", iaColor3f(0.0f, 0.0f, 0.0f));
        const float32 shininess = iXMLHelper::getValue<float32>(materialXML, "Shininess", 5.0f);
        const float32 alpha = iXMLHelper::getValue<float32>(materialXML, "Alpha", 5.0f);
        const iaVector2f tiling = iXMLHelper::getValue<iaVector2f>(materialXML, "Tiling", iaVector2f(1.0f, 1.0f));

        const iaString shaderMaterial = iXMLHelper::getValue<iaString>(materialXML, "ShaderMaterial", "");
        if (!shaderMaterial.isEmpty())
        {
            material->setShaderMaterial(iResourceManager::getInstance().loadResource<iShaderMaterial>(shaderMaterial));
        }

        // TODO shader material
        material->setDiffuse(diffuse);
        material->setAmbient(ambient);
        material->setSpecular(specular);
        material->setEmissive(emissive);
        material->setShininess(shininess);
        material->setAlpha(alpha);
        material->setTiling(tiling);

        return readTextures(materialXML, material);
    }

    bool iMaterialIO::read(const iaString &filename, const iMaterialPtr &material)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        TiXmlDocument document(temp);
        if (!document.LoadFile())
        {
            con_err("can't read \"" << filename << "\". TinyXML:" << document.ErrorDesc());
            return false;
        }

        TiXmlElement *root = document.FirstChildElement("Igor");
        if (!root)
        {
            con_err("not an igor xml file");
            return false;
        }

        TiXmlElement *materialXML = root->FirstChildElement("Material");
        if (materialXML == nullptr)
        {
            con_err("missing Material element");
            return false;
        }

        return readMaterial(materialXML, material);
    }

    bool iMaterialIO::write(const iaString &filename, const iMaterialPtr &material)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::wofstream file;
        file.open(temp);

        if (!file.is_open())
        {
            con_err("can't open to write \"" << temp << "\"");
            return false;
        }

        file << "<?xml version=\"1.0\"?>\n";
        file << "<Igor>\n";
        file << "\t<Material>\n";

        const auto diffuse = material->getDiffuse();
        const auto ambient = material->getAmbient();
        const auto specular = material->getSpecular();
        const auto emissive = material->getEmissive();
        const auto tiling = material->getTiling();

        if (material->getShaderMaterial())
        {
            file << "\t\t<ShaderMaterial>" << material->getShaderMaterial()->getID() << "</ShaderMaterial>\n";
        }
        file << "\t\t<Diffuse>" << diffuse._r << ", " << diffuse._g << ", " << diffuse._b << "</Diffuse>\n";
        file << "\t\t<Ambient>" << ambient._r << ", " << ambient._g << ", " << ambient._b << "</Ambient>\n";
        file << "\t\t<Specular>" << specular._r << ", " << specular._g << ", " << specular._b << "</Specular>\n";
        file << "\t\t<Emissive>" << emissive._r << ", " << emissive._g << ", " << emissive._b << "</Emissive>\n";
        file << "\t\t<Shininess>" << material->getShininess() << "</Shininess>\n";
        file << "\t\t<Alpha>" << material->getAlpha() << "</Alpha>\n";
        file << "\t\t<Tiling>" << tiling._x << ", " << tiling._y << "</Tiling>\n";

        for (const auto &tex : material->getTextures())
        {
            if (tex.second == nullptr)
            {
                continue;
            }

            file << "\t\t<Texture unit=\"" << tex.first << "\">" << tex.second->getID().toString() << "</Texture>\n";
        }

        file << "\t</Material>\n";
        file << "</Igor>\n";

        return true;
    }

} // namespace igor
