// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterialIO.h>

#include <iaux/system/iaFile.h>

#include <tinyxml.h>

#include <fstream>
#include <iostream>

namespace igor
{
    static iRenderStateValue getRenderStateValue(TiXmlElement *states, const char *stateName, iRenderStateValue defaultValue)
    {
        TiXmlElement *state = states->FirstChildElement(stateName);
        if (state == nullptr)
        {
            return defaultValue;
        }

        TiXmlNode *textNode = state->FirstChild();
        if (textNode == nullptr ||
            textNode->Type() != TiXmlNode::NodeType::TINYXML_TEXT)
        {
            return defaultValue;
        }

        const iaString value(textNode->ValueStr().c_str());

        if (value == "Off")
            return iRenderStateValue::Off;
        if (value == "On")
            return iRenderStateValue::On;
        if (value == "One")
            return iRenderStateValue::One;
        if (value == "Zero")
            return iRenderStateValue::Zero;
        if (value == "DestinationColor")
            return iRenderStateValue::DestinationColor;
        if (value == "OneMinusDestinationColor")
            return iRenderStateValue::OneMinusDestinationColor;
        if (value == "SourceAlpha")
            return iRenderStateValue::SourceAlpha;
        if (value == "OneMinusSourceAlpha")
            return iRenderStateValue::OneMinusSourceAlpha;
        if (value == "DestinationAlpha")
            return iRenderStateValue::DestinationAlpha;
        if (value == "OneMinusDestinationAlpha")
            return iRenderStateValue::OneMinusDestinationAlpha;
        if (value == "SourceColor")
            return iRenderStateValue::SourceColor;
        if (value == "OneMinusSourceColor")
            return iRenderStateValue::OneMinusSourceColor;
        if (value == "Never")
            return iRenderStateValue::Never;
        if (value == "Less")
            return iRenderStateValue::Less;
        if (value == "Equal")
            return iRenderStateValue::Equal;
        if (value == "LessOrEqual")
            return iRenderStateValue::LessOrEqual;
        if (value == "Greater")
            return iRenderStateValue::Greater;
        if (value == "NotEqual")
            return iRenderStateValue::NotEqual;
        if (value == "GreaterOrEqual")
            return iRenderStateValue::GreaterOrEqual;
        if (value == "Always")
            return iRenderStateValue::Always;
        if (value == "Front")
            return iRenderStateValue::Front;
        if (value == "Back")
            return iRenderStateValue::Back;
        if (value == "Keep")
            return iRenderStateValue::Keep;
        if (value == "Replace")
            return iRenderStateValue::Replace;
        if (value == "Increment")
            return iRenderStateValue::Increment;
        if (value == "IncrementWrap")
            return iRenderStateValue::IncrementWrap;
        if (value == "Decrement")
            return iRenderStateValue::Decrement;
        if (value == "DecrementWrap")
            return iRenderStateValue::DecrementWrap;
        if (value == "Invert")
            return iRenderStateValue::Invert;
        if (value == "Invalid")
            return iRenderStateValue::Invalid;
        if (value == "PositionOrientation")
            return iRenderStateValue::PositionOrientation;
        if (value == "Position")
            return iRenderStateValue::Position;

        return defaultValue;
    }

    static void readStates(TiXmlElement *states, const iMaterialPtr &material)
    {
        material->setRenderState(iRenderState::DepthTest, getRenderStateValue(states, "DepthTest", iRenderStateValue::On));
        material->setRenderState(iRenderState::DepthMask, getRenderStateValue(states, "DepthMask", iRenderStateValue::On));
        material->setRenderState(iRenderState::Blend, getRenderStateValue(states, "Blend", iRenderStateValue::On));
        material->setRenderState(iRenderState::CullFace, getRenderStateValue(states, "CullFace", iRenderStateValue::On));
        material->setRenderState(iRenderState::Wireframe, getRenderStateValue(states, "Wireframe", iRenderStateValue::Off));
        material->setRenderState(iRenderState::DepthFunc, getRenderStateValue(states, "DepthFunc", iRenderStateValue::Less));
        material->setRenderState(iRenderState::CullFaceFunc, getRenderStateValue(states, "CullFaceFunc", iRenderStateValue::Back));
        material->setRenderState(iRenderState::Instanced, getRenderStateValue(states, "Instanced", iRenderStateValue::Off));
        material->setRenderState(iRenderState::InstancedFunc, getRenderStateValue(states, "InstancedFunc", iRenderStateValue::PositionOrientation));
        material->setRenderState(iRenderState::DepthTest, getRenderStateValue(states, "DepthTest", iRenderStateValue::On));
    }

    static void readShader(TiXmlElement *element, iShaderObjectType shaderObjectType, iShaderProgramPtr shaderProgram)
    {
        TiXmlAttribute *attrib = element->FirstAttribute();
        iaString filename;
        while (attrib)
        {
            if (attrib->NameTStr() == "filename")
            {
                filename = iaString(attrib->Value());
            }

            attrib = attrib->Next();
        }

        if (filename.isEmpty())
        {
            TiXmlNode *text = element->FirstChild();
            if (text != nullptr &&
                text->Type() == TiXmlNode::NodeType::TINYXML_TEXT)
            {
                shaderProgram->addSource(text->ValueStr().c_str(), shaderObjectType, "mat.Vertex");
            }
            else
            {
                con_err("invalid node");
            }
        }
        else
        {
            shaderProgram->addShader(filename, shaderObjectType);
        }
    }

    static void readProgram(TiXmlElement *program, const iMaterialPtr &material)
    {
        TiXmlElement *vertex = program->FirstChildElement("Vertex");
        TiXmlElement *fragment = program->FirstChildElement("Fragment");

        if (vertex == nullptr && fragment == nullptr)
        {
            con_err("no shaders specified");
            return;
        }

        iShaderProgramPtr shaderProgram = iShaderProgram::create();

        if (vertex)
        {
            readShader(vertex, iShaderObjectType::Vertex, shaderProgram);
        }

        if (fragment)
        {
            readShader(fragment, iShaderObjectType::Fragment, shaderProgram);
        }

        TiXmlElement *geometry = program->FirstChildElement("Geometry");
        if (geometry)
        {
            readShader(geometry, iShaderObjectType::Geometry, shaderProgram);
        }

        shaderProgram->compile();

        material->setShaderProgram(shaderProgram);
    }

    static void readMaterial(TiXmlElement *materialXML, const iMaterialPtr &material)
    {
        TiXmlAttribute *attrib = materialXML->FirstAttribute();
        while (attrib)
        {
            if (attrib->NameTStr() == "name")
            {
                material->setName(attrib->Value());
            }
            else if (attrib->NameTStr() == "uuid")
            {
                material->setID(iMaterialID(attrib->Value()));
            }
            else if (attrib->NameTStr() == "zIndex" ||
                     attrib->NameTStr() == "order")
            {
                if (attrib->ValueStr() == "RENDER_ORDER_DEFAULT" ||
                    attrib->ValueStr() == "DEFAULT")
                {
                    material->setOrder(iMaterial::RENDER_ORDER_DEFAULT);
                }
                else if (attrib->ValueStr() == "RENDER_ORDER_MIN" ||
                         attrib->ValueStr() == "MIN")
                {
                    material->setOrder(iMaterial::RENDER_ORDER_MIN);
                }
                else if (attrib->ValueStr() == "RENDER_ORDER_MAX" ||
                         attrib->ValueStr() == "MAX")
                {
                    material->setOrder(iMaterial::RENDER_ORDER_MAX);
                }
                else
                {
                    material->setOrder(iaString::toInt(attrib->Value()));
                }
            }

            attrib = attrib->Next();
        }

        TiXmlElement *states = materialXML->FirstChildElement("States");
        if (states)
        {
            readStates(states, material);
        }

        TiXmlElement *program = materialXML->FirstChildElement("Program");
        if (program)
        {
            readProgram(program, material);
        }
    }

    void iMaterialIO::read(const iaString &filename, const iMaterialPtr &material)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        TiXmlDocument document(temp);
        if (document.LoadFile())
        {
            TiXmlElement *root = document.FirstChildElement("Igor");
            if (!root)
            {
                con_err("not an igor xml file");
                return;
            }

            TiXmlElement *materialXML = root->FirstChildElement("Material");
            if (materialXML)
            {
                readMaterial(materialXML, material);
            }
        }
        else
        {
            con_err("can't read file \"" << filename << "\". " << document.ErrorDesc() << " In line " << document.ErrorRow());
        }
    }

    void iMaterialIO::write(const iaString &filename, const iMaterialPtr &material)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::wofstream file;
        file.open(temp);

        file << "<?xml version=\"1.0\"?>\n";
        file << "<Igor>\n";
        file << "\t<Material name=\"" << material->getName() << "\" uuid=\"" << material->getID() << "\" order=\"" << material->getOrder() << "\">\n";
        file << "\t\t<States>\n";
        file << "\t\t\t<DepthTest>" << material->getRenderState(iRenderState::DepthTest) << "</DepthTest>\n";
        file << "\t\t\t<DepthFunc>" << material->getRenderState(iRenderState::DepthFunc) << "</DepthFunc>\n";
        file << "\t\t\t<DepthMask>" << material->getRenderState(iRenderState::DepthMask) << "</DepthMask>\n";
        file << "\t\t\t<Blend>" << material->getRenderState(iRenderState::Blend) << "</Blend>\n";
        file << "\t\t\t<CullFace>" << material->getRenderState(iRenderState::CullFace) << "</CullFace>\n";
        file << "\t\t\t<CullFaceFunc>" << material->getRenderState(iRenderState::CullFaceFunc) << "</CullFaceFunc>\n";
        file << "\t\t\t<Wireframe>" << material->getRenderState(iRenderState::Wireframe) << "</Wireframe>\n";
        file << "\t\t\t<Instanced>" << material->getRenderState(iRenderState::Instanced) << "</Instanced>\n";
        file << "\t\t\t<InstancedFunc>" << material->getRenderState(iRenderState::InstancedFunc) << "</InstancedFunc>\n";
        file << "\t\t</States>\n";

        const auto &shaderSources = material->getShaderProgram()->getShaderSources();
        if (!shaderSources.empty())
        {
            file << "\t\t<Program>\n";

            for (const auto &source : shaderSources)
            {
                file << "\t\t\t<" << source._type;

                if (iaFile::exist(source._filename))
                {
                    file << " name=\"" << source._filename << "\" />\n";
                }
                else
                {
                    file << ">\n";
                    file << "<![CDATA[" << source._source << "]]>\n";
                    file << "\t\t\t</" << source._type << ">\n";
                }
            }

            file << "\t\t</Program>\n";
        }

        file << "\t</Material>\n";
        file << "</Igor>\n";
    }

} // namespace igor