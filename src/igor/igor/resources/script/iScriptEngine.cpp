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

        void prettyPrintError(lua_State *lua, iScriptPtr script)
        {
            constexpr int WRAPPER_LINE_OFFSET = 3;

            std::string err = lua_tostring(lua, -1);

            size_t pos = 0;
            while ((pos = err.find(":", pos)) != std::string::npos)
            {
                size_t lineStart = err.find_first_of("0123456789", pos + 1);
                if (lineStart != std::string::npos)
                {
                    size_t lineEnd = err.find(":", lineStart);
                    if (lineEnd != std::string::npos)
                    {
                        std::string lineNumStr = err.substr(lineStart, lineEnd - lineStart);
                        int lineNum = std::stoi(lineNumStr);
                        int adjusted = lineNum - WRAPPER_LINE_OFFSET;
                        if (adjusted > 0)
                        {
                            err.replace(lineStart, lineEnd - lineStart, std::to_string(adjusted));
                        }
                    }
                }
                pos += 1;
            }

            con_err("Script error: " << err.c_str() << " in " << script->getInfo());
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
            entityData._script = script;
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
                // con_err("failed to load: " << lua_tostring(lua, -1));
                prettyPrintError(lua, script);
                lua_pop(lua, 2);
                return false;
            }

            // push env
            lua_pushvalue(lua, -2);
            // call wrapper function
            if (lua_pcall(lua, 1, 1, 0) != 0)
            {
                // con_err("pcall wrapper failed: " << lua_tostring(lua, -1));
                prettyPrintError(lua, script);
                lua_pop(lua, 2); // error + env
                return false;
            }

            // call wrapped function
            if (lua_pcall(lua, 0, 0, 0) != 0)
            {
                // con_err("script execution failed: " << lua_tostring(lua, -1));
                prettyPrintError(lua, script);
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
                // con_err("function call error: " << lua_tostring(lua, -1));
                prettyPrintError(lua, entityData._script);
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
                // con_err("function call error: " << lua_tostring(lua, -1));
                prettyPrintError(lua, entityData._script);
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
        }

    private:
        void exposeEntity(lua_State *lua)
        {
            // can't use getGlobalNamespace because we need the wrapped table and not the global one
            luabridge::getNamespaceFromStack(lua).beginNamespace("igor").beginClass<iEntity>("iEntity").addFunction("getID", [](const iEntityPtr entity)
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
                .endClass()
                .endNamespace();
        }

        void exposeVector3(lua_State *lua)
        {
            // can't use getGlobalNamespace because we need the wrapped table and not the global one
            luabridge::getNamespaceFromStack(lua)
                .beginNamespace("igor")
                .beginClass<iaVector3d>("Vector3")
                .addConstructor<void(), void(float64, float64, float64)>()
                .addProperty("x", [](iaVector3d &self) -> float64
                             { return self._x; }, [](iaVector3d &self, float64 value)
                             { self._x = value; })
                .addProperty("y", [](iaVector3d &self) -> float64
                             { return self._y; }, [](iaVector3d &self, float64 value)
                             { self._y = value; })
                .addProperty("z", [](iaVector3d &self) -> float64
                             { return self._z; }, [](iaVector3d &self, float64 value)
                             { self._z = value; })
                .addFunction("length", [](const iaVector3d &self) -> float64
                             { return self.length(); })
                .addFunction("length2", [](const iaVector3d &self) -> float64
                             { return self.length2(); })
                .addFunction("distance", [](const iaVector3d &self, const iaVector3d &other) -> float64
                             { return self.distance(other); })
                .addFunction("distance2", [](const iaVector3d &self, const iaVector3d &other) -> float64
                             { return self.distance2(other); })
                .addFunction("angle", [](const iaVector3d &self, const iaVector3d &other) -> float64
                             { return self.angle(other); })
                .addFunction("normalize", [](iaVector3d &self)
                             { return self.normalize(); })
                .addFunction("dot", [](const iaVector3d &self, const iaVector3d &other) -> float64
                             { return self.dot(other); })
                .addFunction("normalize", &iaVector3d::normalize)
                .addFunction("__add", [](const iaVector3d &self, const iaVector3d &other) -> iaVector3d
                             { return self + other; })
                .addFunction("__sub", [](const iaVector3d &self, const iaVector3d &other) -> iaVector3d
                             { return self - other; })
                .addFunction("__mul", [](const iaVector3d &vec, float64 s) -> iaVector3d
                             { return vec * s; })
                .addFunction("__div", [](const iaVector3d &vec, float64 s) -> iaVector3d
                             { return vec / s; })
                .addFunction("__eq", &iaVector3d::operator==)
                .addFunction("__tostring", [](const iaVector3d &v) -> std::string
                             {
                                char buf[128];
                                snprintf(buf, sizeof(buf), "Vector3(%.3f, %.3f, %.3f)", v._x, v._y, v._z);
                                return std::string(buf); })
                .endClass()
                .endNamespace();
        }

        void exposeGlobalFunctions(lua_State *lua)
        {
            // can't use getGlobalNamespace because we need the wrapped table and not the global one
            luabridge::getNamespaceFromStack(lua)
                .addFunction("meaning", []() -> double
                             { return 42.0; })
                .addFunction("print", [](const std::string &message)
                             { con_endl(message.c_str()); })
                .addFunction("con_endl", [](const std::string &message)
                             { con_endl(message.c_str()); })
                .addFunction("con_info", [](const std::string &message)
                             { con_info(message.c_str()); })
                .addFunction("con_warn", [](const std::string &message)
                             { con_warn(message.c_str()); })
                .addFunction("con_err", [](const std::string &message)
                             { con_err(message.c_str()); })
                .addFunction("error", [](const std::string &message)
                             { con_err(message.c_str()); });
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
            copyGlobal("unpack");
            copyGlobal("pcall");
            copyGlobal("xpcall");
            copyGlobal("assert");
            copyGlobal("math");
            copyGlobal("string");

            exposeVector3(lua);
            exposeEntity(lua);
            exposeGlobalFunctions(lua);
        }

        /*! lua instance per thread
         */
        std::unordered_map<std::thread::id, lua_State *> _lua;

        /*! entity data
         */
        // TODO currently only one thread is effectively used. Make sure entity data is also threadsafe
        struct iEntityData
        {
            iScriptPtr _script;
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
