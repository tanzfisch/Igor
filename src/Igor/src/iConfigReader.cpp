// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iConfigReader.h>
#include <tinyxml.h>
#include <iResourceManager.h>

#include <IgorAux.h>
#include <iaConsole.h>
using namespace IgorAux;

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
		}
	}
}