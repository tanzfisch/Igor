// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/loader/iMaterialIO.h>

#include <igor/resources/iResourceManager.h>
#include <iaux/system/iaFile.h>

#include <tinyxml.h>

#include <fstream>
#include <iostream>

namespace igor
{
    
    bool iMaterialIO::readMaterial(TiXmlElement *materialXML, const iMaterialPtr &material)
    {
     
        return true;
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
// TODO         file << "\t<Material uuid=\"" << material->getID() << "\">\n";

        // TODO

        file << "\t</Material>\n";
        file << "</Igor>\n";

        return true;
    }

} // namespace igor