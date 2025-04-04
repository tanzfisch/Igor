// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/shader/loader/iShaderIO.h>

#include <igor/resources/iResourceManager.h>
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
        if (value == "PositionOrientationInstancing")
            return iRenderStateValue::PositionOrientationInstancing;

        return defaultValue;
    }

    static void readStates(TiXmlElement *states, const iShaderPtr &shader)
    {
        shader->setRenderState(iRenderState::DepthTest, getRenderStateValue(states, "DepthTest", iRenderStateValue::On));
        shader->setRenderState(iRenderState::DepthMask, getRenderStateValue(states, "DepthMask", iRenderStateValue::On));
        shader->setRenderState(iRenderState::DepthFunc, getRenderStateValue(states, "DepthFunc", iRenderStateValue::Less));
        shader->setRenderState(iRenderState::Blend, getRenderStateValue(states, "Blend", iRenderStateValue::On));
        shader->setRenderState(iRenderState::CullFace, getRenderStateValue(states, "CullFace", iRenderStateValue::On));
        shader->setRenderState(iRenderState::CullFaceFunc, getRenderStateValue(states, "CullFaceFunc", iRenderStateValue::Back));
        shader->setRenderState(iRenderState::Wireframe, getRenderStateValue(states, "Wireframe", iRenderStateValue::Off));
        shader->setRenderState(iRenderState::Instanced, getRenderStateValue(states, "Instanced", iRenderStateValue::Off));
        shader->setRenderState(iRenderState::InstancedFunc, getRenderStateValue(states, "InstancedFunc", iRenderStateValue::PositionOrientationInstancing));
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

    static void readProgram(TiXmlElement *program, const iShaderPtr &shader)
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

        shader->setShaderProgram(shaderProgram);
    }

    bool iShaderIO::readMaterial(TiXmlElement *materialXML, const iShaderPtr &shader)
    {
        TiXmlAttribute *attrib = materialXML->FirstAttribute();
        while (attrib)
        {
            if (attrib->NameTStr() == "zIndex" ||
                attrib->NameTStr() == "order")
            {
                if (attrib->ValueStr() == "RENDER_ORDER_DEFAULT" ||
                    attrib->ValueStr() == "DEFAULT")
                {
                    shader->setOrder(iShader::RENDER_ORDER_DEFAULT);
                }
                else if (attrib->ValueStr() == "RENDER_ORDER_MIN" ||
                         attrib->ValueStr() == "MIN")
                {
                    shader->setOrder(iShader::RENDER_ORDER_MIN);
                }
                else if (attrib->ValueStr() == "RENDER_ORDER_MAX" ||
                         attrib->ValueStr() == "MAX")
                {
                    shader->setOrder(iShader::RENDER_ORDER_MAX);
                }
                else
                {
                    shader->setOrder(iaString::toInt(attrib->Value()));
                }
            }
            else if (attrib->NameTStr() == "visibility")
            {
                if (attrib->ValueStr() == "Public")
                {
                    shader->setVisibility(iMaterialVisibility::Public);
                }
                else
                {
                    shader->setVisibility(iMaterialVisibility::Private);
                }
            }

            attrib = attrib->Next();
        }

        TiXmlElement *states = materialXML->FirstChildElement("States");
        if (states)
        {
            readStates(states, shader);
        }

        TiXmlElement *program = materialXML->FirstChildElement("Program");
        if (program)
        {
            readProgram(program, shader);
        }

        return true;
    }

    bool iShaderIO::read(const iaString &filename, const iShaderPtr &shader)
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

        TiXmlElement *materialXML = root->FirstChildElement("ShaderMaterial");
        if (materialXML == nullptr)
        {
            con_err("missing ShaderMaterial element");
            return false;
        }

        return readMaterial(materialXML, shader);
    }

    bool iShaderIO::write(const iaString &filename, const iShaderPtr &shader)
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
        file << "\t<ShaderMaterial uuid=\"" << shader->getID() << "\" order=\"" << shader->getOrder() << "\" visibility=\"" << shader->getVisibility() << "\">\n";
        file << "\t\t<States>\n";
        file << "\t\t\t<DepthTest>" << shader->getRenderState(iRenderState::DepthTest) << "</DepthTest>\n";
        file << "\t\t\t<DepthFunc>" << shader->getRenderState(iRenderState::DepthFunc) << "</DepthFunc>\n";
        file << "\t\t\t<DepthMask>" << shader->getRenderState(iRenderState::DepthMask) << "</DepthMask>\n";
        file << "\t\t\t<Blend>" << shader->getRenderState(iRenderState::Blend) << "</Blend>\n";
        file << "\t\t\t<CullFace>" << shader->getRenderState(iRenderState::CullFace) << "</CullFace>\n";
        file << "\t\t\t<CullFaceFunc>" << shader->getRenderState(iRenderState::CullFaceFunc) << "</CullFaceFunc>\n";
        file << "\t\t\t<Wireframe>" << shader->getRenderState(iRenderState::Wireframe) << "</Wireframe>\n";
        file << "\t\t\t<Instanced>" << shader->getRenderState(iRenderState::Instanced) << "</Instanced>\n";
        file << "\t\t\t<InstancedFunc>" << shader->getRenderState(iRenderState::InstancedFunc) << "</InstancedFunc>\n";
        file << "\t\t</States>\n";

        const auto &shaderSources = shader->getShaderProgram()->getShaderSources();
        if (!shaderSources.empty())
        {
            file << "\t\t<Program>\n";

            for (const auto &source : shaderSources)
            {
                file << "\t\t\t<" << source._type;

                if (iaFile::exists(source._filename))
                {
                    file << " filename=\"" << iResourceManager::getInstance().getRelativePath(source._filename) << "\" />\n";
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

        file << "\t</ShaderMaterial>\n";
        file << "</Igor>\n";

        return true;
    }

} // namespace igor