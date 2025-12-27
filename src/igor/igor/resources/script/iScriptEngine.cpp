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

        void printStack(lua_State *lua)
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

        bool initEntityScript(const char *script, int &envRef, int &initRef, int &updateRef, int &finalRef, int &messageRef, int &eventRef)
        {
            auto lua = _lua[std::this_thread::get_id()];

            lua_newtable(lua);
            envRef = luaL_ref(lua, LUA_REGISTRYINDEX);
            lua_rawgeti(lua, LUA_REGISTRYINDEX, envRef);

            exposeGlobals(lua); // assumes this sets fields on -1 (the env table)

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

        void executeUpdate(int updateRef)
        {
            con_assert(updateRef != LUA_NOREF, "invalid function reference");
            if (updateRef == LUA_NOREF)
            {
                con_err("invalid function reference");
                return;
            }

            auto lua = _lua[std::this_thread::get_id()];

            lua_rawgeti(lua, LUA_REGISTRYINDEX, updateRef);
            lua_pushnumber(lua, 9);  // TODO push self aka entity
            lua_pushnumber(lua, 11); // TODO push time or a context
            if (lua_pcall(lua, 2, 0, 0) != 0)
            {
                con_err("function call error: " << lua_tostring(lua, -1));
                lua_pop(lua, 1);
            }
        }

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

        void exposeGlobal(lua_State *lua, const char *symbol)
        {
            lua_getglobal(lua, symbol);
            if (lua_isnil(lua, -1))
            {
                con_warn("can't find symbol " << symbol);
                lua_pop(lua, 1);
            }
            else
            {
                lua_setfield(lua, -2, symbol);
            }
        }

        void exposeGlobals(lua_State *lua)
        {
            exposeGlobal(lua, "con_endl");
            exposeGlobal(lua, "print");
            exposeGlobal(lua, "con_info");
            exposeGlobal(lua, "con_warn");
            exposeGlobal(lua, "con_err");
            exposeGlobal(lua, "math");
            exposeGlobal(lua, "table");
            exposeGlobal(lua, "string");
        }

        /*! lua instance
         */
        std::unordered_map<std::thread::id, lua_State *> _lua;
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
        return _impl->initEntityScript(scriptData._script->getScript(), scriptData._envRef, scriptData._initRef, scriptData._updateRef, scriptData._finalRef, scriptData._messageRef, scriptData._eventRef);
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
        _impl->executeUpdate(scriptData._updateRef);
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
