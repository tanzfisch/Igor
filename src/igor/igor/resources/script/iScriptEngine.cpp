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

    class iScriptEngineImpl
    {
        friend class iScriptEngine;

    public:
        iScriptEngineImpl()
        {
            _lua = luaL_newstate();
            con_assert_sticky(_lua != nullptr, "failed to init Lua");

            luaL_openlibs(_lua);

            lua_pushcfunction(_lua, print);
            lua_setglobal(_lua, "con_endl");

            lua_pushcfunction(_lua, printInfo);
            lua_setglobal(_lua, "con_info");

            //_chai.add(chaiscript::fun(&print), "print");
            // _chai.add(chaiscript::fun(&printInfo), "printInfo");

            // _chai.add(chaiscript::var(&var), wstringToUtf8(name.getData()));
            // _chai.add(chaiscript::const_var(&var), wstringToUtf8(name.getData()));
            // _chai.add(chaiscript::fun(&printInfo), "printInfo");
            // _chai.add(chaiscript::fun(method, obj), wstringToUtf8(name.getData()));
        }

        bool execute(const char *script)
        {
            if (luaL_dostring(_lua, script))
            {
                con_err("script: " << lua_tostring(_lua, -1));
                lua_pop(_lua, 1);
                return false;
            }

            return true;
        }

    private:
        /*! lua instance
         */
        lua_State *_lua = nullptr;
    };

    iScriptEngine::iScriptEngine()
    {
        _impl = std::make_unique<iScriptEngineImpl>();

        const char *script = R"(con_info("LuaJIT initialized!"))";
        _impl->execute(script);
    }

    iScriptEngine::~iScriptEngine()
    {
    }

    void iScriptEngine::execute(const iaString &script)
    {
        _impl->execute(wstringToUtf8(script.getData()).c_str());
    }
}
