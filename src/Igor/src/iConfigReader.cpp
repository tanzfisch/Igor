// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iConfigReader.h>
#include <iResourceManager.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <tinyxml.h>

namespace Igor
{
	iConfigReader::iConfigReader()
	{
	}

	iConfigReader::~iConfigReader()
	{
	}

	void iConfigReader::readResourceManagerConfig(TiXmlElement* resourceManager)
	{
        TiXmlElement* paths = resourceManager->FirstChildElement("SearchPaths");
        if (paths != nullptr)
        {
            TiXmlElement* element = paths->FirstChildElement("SearchPath");
            if (element != nullptr)
            {
                do
                {
                    TiXmlNode* text = element->FirstChild();
                    if (text->Type() == TiXmlNode::NodeType::TINYXML_TEXT)
                    {
                        iResourceManager::getInstance().addSearchPath(text->ValueStr().data());
                    }

                    element = element->NextSiblingElement("SearchPath");
                } while (element != nullptr);
            }
        }
    }

	void iConfigReader::readLoggingConfig(TiXmlElement* logging)
	{
		TiXmlElement* logLevel = logging->FirstChildElement("LogLevel");
		if (logLevel != nullptr)
		{
			TiXmlNode* text = logLevel->FirstChild();
			if (text->Type() == TiXmlNode::NodeType::TINYXML_TEXT)
			{
				iaString level(text->ValueStr().data());

				if (level == "Assert")
				{
					iaConsole::getInstance().setLogLevel(LogLevel::Assert);
				}
				else if (level == "Error")
				{
					iaConsole::getInstance().setLogLevel(LogLevel::Error);
				}
				else if (level == "Warning")
				{
					iaConsole::getInstance().setLogLevel(LogLevel::Warning);
				}
				else if (level == "Info")
				{
					iaConsole::getInstance().setLogLevel(LogLevel::Info);
				}
				else if (level == "DebugInfo")
				{
					iaConsole::getInstance().setLogLevel(LogLevel::DebugInfo);
				}
				else if (level == "Debug")
				{
					iaConsole::getInstance().setLogLevel(LogLevel::Debug);
				}
			}
		}
	}

	void iConfigReader::readConfiguration(const iaString& filename)
	{
		char temp[2048];
		filename.getData(temp, 2048);

		TiXmlDocument document(temp);
		document.LoadFile();

		TiXmlElement* root = document.FirstChildElement("Igor");
		if(root)
		{
			TiXmlElement* resourceManager = root->FirstChildElement("ResourceManager");
			if(resourceManager)
			{
				readResourceManagerConfig(resourceManager);
			}
			
			TiXmlElement* logging = root->FirstChildElement("Logging");
			if (logging)
			{
				readLoggingConfig(logging);
			}
		}
	}
}