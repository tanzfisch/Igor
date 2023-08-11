// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/model/loader/iModelDataIO.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    const iaString &iModelDataIO::getName()
    {
        return _name;
    }

    const iaString &iModelDataIO::getIdentifier()
    {
        return _identifier;
    }

    iNodePtr iModelDataIO::importData(const iaString &filename, iModelDataInputParameterPtr _parameter)
    {
        con_err("import functionality is not supported by \"" << _name << "\"");
        return nullptr;
    }

    void iModelDataIO::exportData(const iaString &filename, iNodePtr node, iSaveMode saveMode)
    {
        con_err("export functionality is not supported by \"" << _name << "\"");
    }

} // namespace igor
