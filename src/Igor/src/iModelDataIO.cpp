// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iModelDataIO.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    const iaString& iModelDataIO::getName()
    {
        return _name;
    }

    const iaString& iModelDataIO::getIdentifier()
    {
        return _identifier;
    }

    iNode* iModelDataIO::importData(const iaString& filename, iModelDataInputParameter* _parameter)
    {
        con_err("import functionality is not supported by \"" << _name << "\"");
        return nullptr;
    }

    void iModelDataIO::exportData(const iaString& filename, iNode* node, iSaveMode saveMode)
    {
        con_err("export functionality is not supported by \"" << _name << "\"");
    }

}