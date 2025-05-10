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

    static iaString cleanupTypeName(const iaString &text)
    {
        std::wstring mangled = text.getData();
        iaString result;

        // Skip leading and trailing character
        size_t pos = 1;
        size_t end = mangled.size() - 1;

        bool first = true;

        while (pos < end)
        {
            // Parse the number
            int len = 0;
            while (pos < end && iswdigit(mangled[pos]))
            {
                len = len * 10 + (mangled[pos] - L'0');
                ++pos;
            }

            // Get the name part
            if (pos + len > end)
            {
                break;
            }
            std::wstring part = mangled.substr(pos, len);
            pos += len;

            // Add to result
            if (!first)
            {
                result += L"::";
            }
            result += part.c_str();
            first = false;
        }

        return result;
    }

    bool iEntityPrintTraverser::preOrderVisit(iEntityPtr entity)
    {
        con_endl(getIndent(_indentation) << "_________________________________________");
        con_endl(getIndent(_indentation) << "|" << entity->getName() << " (" << entity->getID() << ") " << (entity->isActive() ? "Active" : "Inactive"));

        for (const auto &compType : entity->getComponentTypes())
        {
            auto component = entity->getComponent(compType);
            con_assert(component != nullptr, "zero pointer");

            std::wstringstream stream;
            stream << component->getState();
            con_endl(getIndent(_indentation) << "|  * " << cleanupTypeName(compType.name()) + " (" + component->getID().toString() + ") " + stream.str().c_str());

            if (_verbose)
            {
                for (const auto &info : component->getInfo())
                {
                    con_endl(getIndent(_indentation) << "|      - " << info);
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