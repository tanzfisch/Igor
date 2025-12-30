// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/script/iScriptEngine.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

extern "C"
{
#include <luajit.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#include <codecvt>
#include <unordered_map>
#include <thread>

namespace igor
{
    static std::string wstringToUtf8(const std::wstring &wstr)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }

    static std::string toStdString(const iaString &text)
    {
        const wchar_t *wideData = text.getData();
        int64 charCount = text.getLength();

        // Direct conversion: wchar_t (usually 16-bit or 32-bit) → char (truncate to 8-bit)
        std::string result;
        result.reserve(charCount);

        for (int64 i = 0; i < charCount; ++i)
        {
            result += static_cast<char>(wideData[i]);
        }

        return result;
    }

    static int print(lua_State *lua)
    {
        const auto message = luaL_checkstring(lua, 1);
        con_endl(message);
        return 0;
    }

    static int printInfo(lua_State *lua)
    {
        const auto message = luaL_checkstring(lua, 1);
        con_info(message);
        return 0;
    }

    static int printWarning(lua_State *lua)
    {
        const auto message = luaL_checkstring(lua, 1);
        con_warn(message);
        return 0;
    }

    static int printError(lua_State *lua)
    {
        const auto message = luaL_checkstring(lua, 1);
        con_err(message);
        return 0;
    }

    class iScriptEngineImpl
    {
        friend class iScriptEngine;

    public:
        iScriptEngineImpl()
        {
        }

        bool execute(const char *script)
        {
            auto lua = _lua[std::this_thread::get_id()];
            if (luaL_dostring(lua, script))
            {
                const auto error = lua_tostring(lua, -1);
                con_endl(script);
                con_err("script: " << (error != nullptr ? error : "unknown error"));
                lua_pop(lua, 1);
                return false;
            }

            return true;
        }

        void debugStack(lua_State *lua)
        {
            iaConsole::getInstance() << LOCK;
            iaConsole::getInstance().printHeader(iaLogLevel::Debug);
            iaConsole::getInstance() << iaForegroundColor::Gray;

            int count = lua_gettop(lua);

            if (count > 0)
            {
                iaConsole::getInstance() << "lua stack:" << endl;

                int index = -1;
                for (int i = 0; i < count; ++i)
                {
                    int type = lua_type(lua, index);
                    iaConsole::getInstance() << __IGOR_LOGGING_TAB__ << "[" << index << "] " << lua_typename(lua, type);

                    switch (type)
                    {
                    case LUA_TSTRING:
                        iaConsole::getInstance() << " " << lua_tostring(lua, index);
                        break;
                    case LUA_TBOOLEAN:
                        iaConsole::getInstance() << " " << (lua_toboolean(lua, index) ? "true" : "false");
                        break;
                    case LUA_TNUMBER:
                        iaConsole::getInstance() << " " << lua_tonumber(lua, index);
                        break;
                    case LUA_TTABLE:
                        lua_getmetatable(lua, index);
                        if (lua_istable(lua, index))
                        {
                            iaConsole::getInstance() << " (has metatable)";
                        }
                        break;
                    }

                    index--;

                    iaConsole::getInstance() << endl;
                }
            }
            else
            {
                iaConsole::getInstance() << "lua stack: empty" << endl;
            }

            iaConsole::getInstance() << UNLOCK;
        }

        bool initEntityScript(iEntityPtr entity, const char *script, int &envRef, int &initRef, int &updateRef, int &finalRef, int &messageRef, int &eventRef)
        {
            auto lua = _lua[std::this_thread::get_id()];

            luabridge::LuaRef entityTable = luabridge::newTable(lua);

            // Critical: Inherit ALL globals via metatable
            entityTable.push(lua);                     // push our env table
            luabridge::getGlobal(lua, "_G").push(lua); // push global _G
            lua_setmetatable(lua, -2);                 // setmetatable(env, _G)

            exposeGlobals(lua, entityTable);

            entityTable["entity"] = entity;

            envRef = luaL_ref(lua, LUA_REGISTRYINDEX);
            lua_rawgeti(lua, LUA_REGISTRYINDEX, envRef);

            _entityTables[entity] = envRef;

            // 3. Wrap and load script
            std::string wrappedScript =
                "local env = ... \n"
                "return function() \n"
                "    setfenv(1, env) \n" +
                std::string(script) +
                "\nend";

            if (luaL_loadstring(lua, wrappedScript.c_str()) != 0)
            {
                con_err("failed to load: " << lua_tostring(lua, -1));
                lua_pop(lua, 2);
                return false;
            }

            // push env
            lua_pushvalue(lua, -2);
            // call wrapper function
            if (lua_pcall(lua, 1, 1, 0) != 0)
            {
                con_err("pcall wrapper failed: " << lua_tostring(lua, -1));
                lua_pop(lua, 2); // error + env
                return false;
            }

            // call wrapped function
            if (lua_pcall(lua, 0, 0, 0) != 0)
            {
                con_err("script execution failed: " << lua_tostring(lua, -1));
                lua_pop(lua, 2); // error + env
                return false;
            }

            auto getFunctionRef = [&](const char *name, int &ref)
            {
                lua_getfield(lua, -1, name); // push env[name]
                if (lua_isfunction(lua, -1))
                {
                    ref = luaL_ref(lua, LUA_REGISTRYINDEX); // pop and store
                }
                else
                {
                    lua_pop(lua, 1); // remove nil
                    ref = LUA_NOREF;
                    con_err("script does not contain " << name << " function");
                }
            };

            getFunctionRef("onInit", initRef);
            getFunctionRef("onUpdate", updateRef);
            getFunctionRef("onFinal", finalRef);
            getFunctionRef("onMessage", messageRef);
            getFunctionRef("onEvent", eventRef);

            lua_pop(lua, 1);

            return true;
        }

        void deinitEntityScript(int envIndex)
        {
            auto lua = _lua[std::this_thread::get_id()];

            // TODO
        }

        void executeInit(int initRef)
        {
            con_assert(initRef != LUA_NOREF, "invalid function reference");
            if (initRef == LUA_NOREF)
            {
                con_err("invalid function reference");
                return;
            }

            auto lua = _lua[std::this_thread::get_id()];

            lua_rawgeti(lua, LUA_REGISTRYINDEX, initRef);
            lua_pushnumber(lua, 9); // TODO push self aka entity
            if (lua_pcall(lua, 1, 0, 0) != 0)
            {
                con_err("function call error: " << lua_tostring(lua, -1));
                lua_pop(lua, 1);
            }
        }

        void executeUpdate(int updateRef, iEntityPtr entity, double dt = 0.0) // Add entity and optional params
        {
            if (updateRef == LUA_NOREF)
                return;

            auto lua = _lua[std::this_thread::get_id()];

            lua_rawgeti(lua, LUA_REGISTRYINDEX, updateRef);
            int envRef = _entityTables[entity];
            lua_rawgeti(lua, LUA_REGISTRYINDEX, envRef);
            lua_pushnumber(lua, dt);

            if (lua_pcall(lua, 2, 0, 0) != 0)
            {
                con_err("function call error: " << lua_tostring(lua, -1));
                lua_pop(lua, 1);
            }
        }

        /*void executeUpdate(int updateRef)
        {
            con_assert(updateRef != LUA_NOREF, "invalid function reference");
            if (updateRef == LUA_NOREF)
            {
                con_err("invalid function reference");
                return;
            }

            auto lua = _lua[std::this_thread::get_id()];

            lua_rawgeti(lua, LUA_REGISTRYINDEX, updateRef);
            lua_push(lua, 9);        // TODO push self aka entity
            lua_pushnumber(lua, 11); // TODO push time or a context
            if (lua_pcall(lua, 2, 0, 0) != 0)
            {
                con_err("function call error: " << lua_tostring(lua, -1));
                lua_pop(lua, 1);
            }
        }*/

        void registerThread()
        {
            lua_State *lua = luaL_newstate();
            con_assert_sticky(lua != nullptr, "failed to init Lua");

            _lua[std::this_thread::get_id()] = lua;

            luaL_openlibs(lua);

            createGlobals(lua);
        }

    private:
        void createGlobals(lua_State *lua)
        {
            lua_pushcfunction(lua, print);
            lua_setglobal(lua, "con_endl");

            lua_pushcfunction(lua, print);
            lua_setglobal(lua, "print");

            lua_pushcfunction(lua, printInfo);
            lua_setglobal(lua, "con_info");

            lua_pushcfunction(lua, printWarning);
            lua_setglobal(lua, "con_warn");

            lua_pushcfunction(lua, printError);
            lua_setglobal(lua, "con_err");
        }

        void exposeGlobals(lua_State *lua, luabridge::LuaRef entityTable)
        {
            auto copyGlobal = [&](const char *name)
            {
                entityTable[name] = luabridge::getGlobal(lua, name);
            };

            copyGlobal("type");
            copyGlobal("tostring");
            copyGlobal("pairs");
            copyGlobal("ipairs");
            copyGlobal("next");
            copyGlobal("select");
            copyGlobal("unpack"); // or table.unpack in 5.2+

            copyGlobal("pcall");
            copyGlobal("xpcall");
            copyGlobal("assert");
            copyGlobal("error");

            copyGlobal("math");
            copyGlobal("string");
            copyGlobal("table");

            auto pushCustom = [&](const char *name, lua_CFunction fn)
            {
                lua_pushcfunction(lua, fn);
                entityTable[name] = luabridge::LuaRef::fromStack(lua, -1);
                lua_pop(lua, 1);
            };

            pushCustom("print", print);
            pushCustom("con_endl", print);
            pushCustom("con_info", printInfo);
            pushCustom("con_warn", printWarning);
            pushCustom("con_err", printError);

            luabridge::getGlobalNamespace(lua)
                .beginClass<iEntity>("iEntity")
                .addFunction("getID", [](const iEntityPtr entity)
                             { return toStdString(entity->getID().toString()); })
                .addFunction("getName", [](const iEntityPtr entity)
                             { return toStdString(entity->getName()); })
                .endClass();
        }

        /*! lua instance
         */
        std::unordered_map<std::thread::id, lua_State *> _lua;

        /*! lua tables for entities
         */
        std::unordered_map<iEntityPtr, int> _entityTables;
    };

    iScriptEngine::iScriptEngine()
    {
        _impl = std::make_unique<iScriptEngineImpl>();
        _impl->registerThread();

        const char *script = R"(con_info("LuaJIT initialized!"))";
        _impl->execute(script);
    }

    iScriptEngine::~iScriptEngine()
    {
    }

    void iScriptEngine::registerThread()
    {
        _impl->registerThread();
    }

    void iScriptEngine::execute(const char *script)
    {
        _impl->execute(script);
    }

    bool iScriptEngine::initEntityScript(iEntityPtr entity, iScriptData &scriptData)
    {
        return _impl->initEntityScript(entity, scriptData._script->getScript(), scriptData._envRef, scriptData._initRef, scriptData._updateRef, scriptData._finalRef, scriptData._messageRef, scriptData._eventRef);
    }

    void iScriptEngine::deinitEntityScript(iEntityPtr entity, iScriptData &scriptData)
    {
        _impl->deinitEntityScript(scriptData._envRef);

        scriptData._envRef = LUA_NOREF;
        scriptData._initRef = LUA_NOREF;
        scriptData._updateRef = LUA_NOREF;
        scriptData._finalRef = LUA_NOREF;
        scriptData._messageRef = LUA_NOREF;
        scriptData._eventRef = LUA_NOREF;
    }

    void iScriptEngine::callEntityInit(iEntityPtr entity, iScriptData &scriptData)
    {
        _impl->executeInit(scriptData._initRef);
        scriptData._scriptState = iEntityScriptState::Update;
    }
    void iScriptEngine::callEntityUpdate(iEntityPtr entity, iScriptData &scriptData)
    {
        _impl->executeUpdate(scriptData._updateRef, entity);
    }
    void iScriptEngine::callEntityFinal(iEntityPtr entity, iScriptData &scriptData)
    {
    }
    void iScriptEngine::callEntityMessage(iEntityPtr entity, iScriptData &scriptData)
    {
    }
    void iScriptEngine::callEntityEvent(iEntityPtr entity, iScriptData &scriptData)
    {
    }
}
