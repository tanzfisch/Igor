// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/script/iScriptEngine.h>

#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iVelocityComponent.h>

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

        void getFunctionRef(lua_State *lua, const char *name, int &ref)
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
        }

        bool initEntityScript(iEntityPtr entity, iScriptPtr script)
        {
            auto lua = _lua[std::this_thread::get_id()];

            luabridge::LuaRef entityTable = luabridge::newTable(lua);

            // Critical: Inherit ALL globals via metatable
            entityTable.push(lua);                     // push our env table
            luabridge::getGlobal(lua, "_G").push(lua); // push global _G
            lua_setmetatable(lua, -2);                 // setmetatable(env, _G)

            exposeGlobals(lua, entityTable);

            entityTable["entity"] = entity;

            iEntityData entityData;
            entityData._envRef = luaL_ref(lua, LUA_REGISTRYINDEX);
            lua_rawgeti(lua, LUA_REGISTRYINDEX, entityData._envRef);

            // 3. Wrap and load script
            std::string wrappedScript =
                "local env = ... \n"
                "return function() \n"
                "    setfenv(1, env) \n" +
                std::string(script->getScript()) +
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

            getFunctionRef(lua, "onInit", entityData._initRef);
            getFunctionRef(lua, "onUpdate", entityData._updateRef);
            getFunctionRef(lua, "onFinal", entityData._finalRef);
            getFunctionRef(lua, "onMessage", entityData._messageRef);
            getFunctionRef(lua, "onEvent", entityData._eventRef);
            _entityTables[entity] = entityData;

            lua_pop(lua, 1);

            return true;
        }

        bool deinitEntityScript(iEntityPtr entity)
        {
            auto lua = _lua[std::this_thread::get_id()];

            // TODO

            return true;
        }

        bool executeInit(iEntityPtr entity)
        {
            auto lua = _lua[std::this_thread::get_id()];
            const auto &entityData = _entityTables[entity];

            lua_rawgeti(lua, LUA_REGISTRYINDEX, entityData._initRef);
            lua_rawgeti(lua, LUA_REGISTRYINDEX, entityData._envRef);
            if (lua_pcall(lua, 1, 0, 0) != 0)
            {
                con_err("function call error: " << lua_tostring(lua, -1));
                lua_pop(lua, 1);
                return false;
            }

            return true;
        }

        bool executeUpdate(iEntityPtr entity, double dt = 0.0) // Add entity and optional params
        {
            auto lua = _lua[std::this_thread::get_id()];
            const auto &entityData = _entityTables[entity];

            lua_rawgeti(lua, LUA_REGISTRYINDEX, entityData._updateRef);
            lua_rawgeti(lua, LUA_REGISTRYINDEX, entityData._envRef);
            lua_pushnumber(lua, dt);

            if (lua_pcall(lua, 2, 0, 0) != 0)
            {
                con_err("function call error: " << lua_tostring(lua, -1));
                lua_pop(lua, 1);
                return false;
            }

            return true;
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
                .beginClass<iaVector3d>("Vector3")
                .addFunction("__add", [](const iaVector3d &self, const iaVector3d &other) -> iaVector3d
                             { return self + other; })
                .addProperty("x", [](iaVector3d &self) -> float64
                             { return self._x; }, [](iaVector3d &self, float64 value)
                             { self._x = value; })
                .addProperty("y", [](iaVector3d &self) -> float64
                             { return self._y; }, [](iaVector3d &self, float64 value)
                             { self._y = value; })
                .addProperty("z", [](iaVector3d &self) -> float64
                             { return self._z; }, [](iaVector3d &self, float64 value)
                             { self._z = value; })
                .addFunction("length", &iaVector3d::length)
                .addFunction("length2", &iaVector3d::length2)
                .addFunction("dot", &iaVector3d::dot)
                .addFunction("distance", &iaVector3d::distance)
                .addFunction("normalize", &iaVector3d::normalize)
                .addFunction("set", &iaVector3d::set)
                .addFunction("__tostring", [](const iaVector3d &v) -> std::string
                             {
            char buf[128];
            snprintf(buf, sizeof(buf), "Vector3(%.3f, %.3f, %.3f)", v._x, v._y, v._z);
            return std::string(buf); })
                .endClass()
                .addFunction("Vector3", [](float64 x, float64 y, float64 z) -> iaVector3d
                             { return iaVector3d(x, y, z); })
                .addFunction("Vector3", []() -> iaVector3d
                             { return iaVector3d(0, 0, 0); })

                .beginClass<iEntity>("iEntity")
                .addFunction("getID", [](const iEntityPtr entity)
                             { return toStdString(entity->getID().toString()); })
                .addFunction("getName", [](const iEntityPtr entity)
                             { return toStdString(entity->getName()); })
                .addFunction("getTransformComponent", [](iEntityPtr entity) -> iTransformComponentPtr
                             { return dynamic_cast<iTransformComponentPtr>(entity->getComponent(typeid(iTransformComponent))); })
                .addFunction("getVelocityComponent", [](iEntityPtr entity) -> iVelocityComponentPtr
                             { return dynamic_cast<iVelocityComponentPtr>(entity->getComponent(typeid(iVelocityComponent))); })
                .endClass()
                .beginClass<iTransformComponent>("iTransformComponent")
                .addFunction("getPosition", [](const iTransformComponentPtr transformComp) -> iaVector3d
                             { return transformComp->getPosition(); })
                .addFunction("setPosition", [](iTransformComponentPtr transformComp, const iaVector3d &pos)
                             { transformComp->setPosition(pos); })
                .endClass()
                .beginClass<iVelocityComponent>("iVelocityComponent")
                .addFunction("getVelocity", [](const iVelocityComponentPtr velocityComp) -> iaVector3d
                             { return velocityComp->getVelocity(); })
                .addFunction("setVelocity", [](iVelocityComponentPtr velocityComp, const iaVector3d &velocity)
                             { velocityComp->setVelocity(velocity); })
                .addFunction("getAngularVelocity", [](const iVelocityComponentPtr velocityComp) -> iaVector3d
                             { return velocityComp->getAngularVelocity(); })
                .addFunction("setAngularVelocity", [](iVelocityComponentPtr velocityComp, const iaVector3d &velocity)
                             { velocityComp->setAngularVelocity(velocity); })
                .endClass();
        }

        /*! lua instance per thread
         */
        std::unordered_map<std::thread::id, lua_State *> _lua;

        /*! entity data
         */
        // TODO currently only one thread is effectively used. Make sure entity data is also threadsafe
        struct iEntityData
        {
            int _envRef = IGOR_LUA_NOREF;     //! script environment table reference
            int _initRef = IGOR_LUA_NOREF;    //! script init function reference
            int _updateRef = IGOR_LUA_NOREF;  //! script update function reference
            int _finalRef = IGOR_LUA_NOREF;   //! script final function reference
            int _messageRef = IGOR_LUA_NOREF; //! script message function reference
            int _eventRef = IGOR_LUA_NOREF;   //! script event function reference
        };

        /*! lua tables for entities
         */
        std::unordered_map<iEntityPtr, iEntityData> _entityTables;
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

    bool iScriptEngine::initEntityScript(iEntityPtr entity, iScriptPtr script)
    {
        return _impl->initEntityScript(entity, script);
    }

    bool iScriptEngine::deinitEntityScript(iEntityPtr entity)
    {
        return _impl->deinitEntityScript(entity);
    }

    bool iScriptEngine::callEntityInit(iEntityPtr entity)
    {
        return _impl->executeInit(entity);
    }

    bool iScriptEngine::callEntityUpdate(iEntityPtr entity)
    {
        return _impl->executeUpdate(entity);
    }

    bool iScriptEngine::callEntityFinal(iEntityPtr entity)
    {
        return _impl->executeUpdate(entity);
    }

    bool iScriptEngine::callEntityMessage(iEntityPtr entity)
    {
        return _impl->executeUpdate(entity);
    }

    bool iScriptEngine::callEntityEvent(iEntityPtr entity)
    {
        return _impl->executeUpdate(entity);
    }
}
