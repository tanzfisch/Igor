// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/config/iConfigReader.h>
#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <tinyxml.h>

namespace igor
{
    iConfigReader::iConfigReader()
    {
    }

    iConfigReader::~iConfigReader()
    {
    }

    void iConfigReader::readResourceManagerConfig(TiXmlElement *resourceManager)
    {
        TiXmlElement *paths = resourceManager->FirstChildElement("SearchPaths");
        if (paths != nullptr)
        {
            TiXmlElement *element = paths->FirstChildElement("SearchPath");
            if (element != nullptr)
            {
                do
                {
                    TiXmlNode *text = element->FirstChild();
                    if (text->Type() == TiXmlNode::NodeType::TINYXML_TEXT)
                    {
                        iResourceManager::getInstance().addSearchPath(text->ValueStr().data());
                    }

                    element = element->NextSiblingElement("SearchPath");
                } while (element != nullptr);
            }
        }
    }

    void iConfigReader::readLoggingConfig(TiXmlElement *logging)
    {
        TiXmlElement *logLevel = logging->FirstChildElement("LogLevel");
        if (logLevel != nullptr)
        {
            TiXmlNode *text = logLevel->FirstChild();
            if (text->Type() == TiXmlNode::NodeType::TINYXML_TEXT)
            {
                iaString level(text->ValueStr().data());

                if (level == "Assert")
                {
                    iaConsole::getInstance().setLogLevel(iaLogLevel::Fatal);
                }
                else if (level == "Error")
                {
                    iaConsole::getInstance().setLogLevel(iaLogLevel::Error);
                }
                else if (level == "Warning")
                {
                    iaConsole::getInstance().setLogLevel(iaLogLevel::Warning);
                }
                else if (level == "Info")
                {
                    iaConsole::getInstance().setLogLevel(iaLogLevel::Info);
                }
                else if (level == "User")
                {
                    iaConsole::getInstance().setLogLevel(iaLogLevel::User);
                }
                else if (level == "Debug")
                {
                    iaConsole::getInstance().setLogLevel(iaLogLevel::Debug);
                }
                else if (level == "Trace")
                {
                    iaConsole::getInstance().setLogLevel(iaLogLevel::Trace);
                }
            }
        }
    }

    void iConfigReader::readConfiguration(const iaString &filename)
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

            TiXmlElement *logging = root->FirstChildElement("Logging");
            if (logging)
            {
                readLoggingConfig(logging);
            }

            TiXmlElement *resourceManager = root->FirstChildElement("ResourceManager");
            if (resourceManager)
            {
                readResourceManagerConfig(resourceManager);
            }
        }
    }
} // namespace igor