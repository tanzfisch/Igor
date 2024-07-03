// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
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

    iNodePtr iModelDataIO::importData(const iaString &filename)
    {
        iParameters parameters({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MODEL},
                                {IGOR_RESOURCE_PARAM_SOURCE, filename}});
        return importData(parameters);
    }

    iNodePtr iModelDataIO::importData(const iParameters &parameters)
    {
        con_err("import functionality is not supported by \"" << _name << "\" [" << _identifier << "]");
        return nullptr;
    }

    void iModelDataIO::exportData(const iaString &filename, iNodePtr node, iSaveMode saveMode)
    {
        iParameters parameters({{IGOR_RESOURCE_PARAM_SOURCE, filename},
                                {IGOR_RESOURCE_PARAM_NODE, node},
                                {IGOR_RESOURCE_PARAM_EXPORT_MODE, saveMode}});
        exportData(parameters);
    }

    void iModelDataIO::exportData(const iParameters &parameters)
    {
        con_err("export functionality is not supported by \"" << _name << "\" [" << _identifier << "]");
    }

} // namespace igor
