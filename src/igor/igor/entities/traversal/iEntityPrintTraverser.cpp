// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/traversal/iEntityPrintTraverser.h>

#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iCameraComponent.h>

#include <iaux/system/iaConsole.h>

#include <regex>

namespace igor
{
    static iaString getIndent(uint32 indentation)
    {
        iaString indent;
        for (int i = 0; i < indentation; ++i)
        {
            indent += "    ";
        }
        return indent;
    }

    iEntityPrintTraverser::iEntityPrintTraverser(bool verbose)
        : _verbose(verbose)
    {
        setIgnoreInactive(false);
    }

    void iEntityPrintTraverser::preTraverse()
    {
        _indentation = 0;
    }

    static iaString cleanupTypeName(const iaString &typeName)
    {
        std::wregex pattern(L"^N\\d+(\\w+)\\d+(\\w+)E$");
        std::wsmatch match;
        std::wstring const text(typeName.getData());
        iaString result;

        if (!std::regex_match(text, match, pattern)) {
            con_err("no match");
            return result;
        }
        
        result += match[1].str().c_str();
        result += "::";
        result += match[2].str().c_str();

        return result;
    }

    bool iEntityPrintTraverser::preOrderVisit(iEntityPtr entity)
    {
        con_endl(getIndent(_indentation) << entity->getName() << " (" << entity->getID() << ") " << (entity->isActive() ? "active" : "inactive"));

        for (const auto &compType : entity->getComponentTypes())
        {
            con_endl(getIndent(_indentation) << "| " << cleanupTypeName(compType.name()));
            if (_verbose)
            {
                auto component = entity->getComponent(compType);
                con_assert(component != nullptr, "zero pointer");
                for(const auto &info : component->getInfo())
                {
                    con_endl(getIndent(_indentation) << "|   " << info);
                }
            }
        }

        _indentation++;
        return true;
    }

    void iEntityPrintTraverser::postOrderVisit(iEntityPtr entity)
    {
        _indentation--;
    }

    void iEntityPrintTraverser::postTraverse()
    {
        con_assert(_indentation == 0, "invalid indentation");
    }

} // namespace igor