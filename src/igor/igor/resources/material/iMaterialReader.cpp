// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterialReader.h>

#include <tinyxml.h>

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
        material->setRenderState(iRenderState::Texture2D0, getRenderStateValue(states, "Texture2D0", iRenderStateValue::Off));
        material->setRenderState(iRenderState::Texture2D1, getRenderStateValue(states, "Texture2D1", iRenderStateValue::Off));
        material->setRenderState(iRenderState::Texture2D2, getRenderStateValue(states, "Texture2D2", iRenderStateValue::Off));
        material->setRenderState(iRenderState::Texture2D3, getRenderStateValue(states, "Texture2D3", iRenderStateValue::Off));
        material->setRenderState(iRenderState::Texture2D4, getRenderStateValue(states, "Texture2D4", iRenderStateValue::Off));
        material->setRenderState(iRenderState::Texture2D5, getRenderStateValue(states, "Texture2D5", iRenderStateValue::Off));
        material->setRenderState(iRenderState::Texture2D6, getRenderStateValue(states, "Texture2D6", iRenderStateValue::Off));
        material->setRenderState(iRenderState::Texture2D7, getRenderStateValue(states, "Texture2D7", iRenderStateValue::Off));
        material->setRenderState(iRenderState::Wireframe, getRenderStateValue(states, "Wireframe", iRenderStateValue::Off));
        material->setRenderState(iRenderState::DepthFunc, getRenderStateValue(states, "DepthFunc", iRenderStateValue::Less));
        material->setRenderState(iRenderState::CullFaceFunc, getRenderStateValue(states, "CullFaceFunc", iRenderStateValue::Back));
        material->setRenderState(iRenderState::InstancedFunc, getRenderStateValue(states, "InstancedFunc", iRenderStateValue::PositionOrientation));
        material->setRenderState(iRenderState::DepthTest, getRenderStateValue(states, "DepthTest", iRenderStateValue::On));
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
            TiXmlNode *text = vertex->FirstChild();
            if (text != nullptr &&
                text->Type() == TiXmlNode::NodeType::TINYXML_TEXT)
            {
                shaderProgram->addSource(text->ValueStr().c_str(), iShaderObjectType::Vertex);
            }
            else
            {
                con_err("invalid node");
            }
        }

        if (fragment)
        {
            TiXmlNode *text = fragment->FirstChild();
            if (text != nullptr &&
                text->Type() == TiXmlNode::NodeType::TINYXML_TEXT)
            {
                shaderProgram->addSource(text->ValueStr().c_str(), iShaderObjectType::Fragment);
            }
            else
            {
                con_err("invalid node");
            }
        }

        shaderProgram->compile();

        material->setShaderProgram(shaderProgram);
    }

    static void readMaterial(TiXmlElement *materialXML, const iMaterialPtr &material)
    {
        TiXmlAttribute *attrib = materialXML->FirstAttribute();
        while (attrib)
        {
            if(attrib->NameTStr() == "name")
            {
                material->setName(attrib->Value());
            }
            else if(attrib->NameTStr() == "uuid")
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

    void iMaterialReader::read(const iaString &filename, const iMaterialPtr &material)
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
} // namespace igor