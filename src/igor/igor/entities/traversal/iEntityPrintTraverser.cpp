// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/traversal/iEntityPrintTraverser.h>

#include <igor/entities/components/iTransformComponent.h>

#include <iaux/system/iaConsole.h>

namespace igor
{
    static iaString getIndent(uint32 indentation)
    {
        iaString indent;
        for(int i=0;i<indentation;++i)
        {
            indent += "    ";
        }
        return indent;
    }

    iEntityPrintTraverser::iEntityPrintTraverser()
    {
        setIgnoreInactive(false);
    }

    void iEntityPrintTraverser::preTraverse()
    {
        _indentation = 0;
    }

    bool iEntityPrintTraverser::preOrderVisit(iEntityPtr entity)
    {        
        con_endl(getIndent(_indentation) << entity->getName() << " (" << entity->getID()<< ")");

        auto transform = entity->getComponent<iTransformComponent>();
        if(transform)
        {
            con_endl(getIndent(_indentation) << "Position: " << transform->getPosition()); 
        }

        _indentation++;
        return true;
    }

    void iEntityPrintTraverser::postOrderVisit(iEntityPtr entity)
    {
        _indentation --;
    }

    void iEntityPrintTraverser::postTraverse()
    {
        con_assert(_indentation == 0, "invalid indentation");
    }

} // namespace igor