// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/script/iScriptEngine.h>

#include <iaux/system/iaConsole.h>

#include <chaiscript/chaiscript.hpp>

namespace igor
{

    void print(const std::string &t_name)
    {
        con_endl(t_name.c_str());
    }

    void printInfo(const std::string &t_name)
    {
        con_info(t_name.c_str());
    }

    class iScriptEngineImpl
    {
        friend class iScriptEngine;

    public:
        iScriptEngineImpl()
        {
            _chai.add(chaiscript::fun(&print), "print");
            _chai.add(chaiscript::fun(&printInfo), "printInfo");
        }

    private:
        /*! chaiscript instance
         */
        chaiscript::ChaiScript _chai;
    };

    iScriptEngine::iScriptEngine()
    {
        _impl = new iScriptEngineImpl();

        _impl->_chai.eval(R"(
            printInfo("ChaiScript initialized");
            )");
    }

    iScriptEngine::~iScriptEngine()
    {
        delete _impl;
    }

    template <typename T>
    void iScriptEngine::addVariable(T &var, const std::string &name)
    {
        try
        {
            _impl->_chai.add(chaiscript::var(&var), name);
        }
        catch (...)
        {
            con_err("could not add variable \"" << name << "\"");
        }
    }

    template <typename T>
    void iScriptEngine::addConst(const T &var, const std::string &name)
    {
        try
        {
            _impl->_chai.add(chaiscript::const_var(&var), name);
        }
        catch (...)
        {
            con_err("could not add constant \"" << name << "\"");
        }
    }

    template <typename F>
    void iScriptEngine::addFunction(F &&func, const std::string &name)
    {
        try
        {
            _impl->_chai.add(chaiscript::fun(std::forward<F>(func)), name);
        }
        catch (...)
        {
            con_err("could not add function \"" << name << "\"");
        }
    }

    template <typename T, typename... Args>
    void iScriptEngine::addMethod(T *obj, void (T::*method)(Args...), const std::string &name)
    {
        try
        {
            _impl->_chai.add(chaiscript::fun(method, obj), name);
        }
        catch (...)
        {
            con_err("could not add method \"" << name << "\"");
        }
    }

    void iScriptEngine::executeScript(const iaString &filename)
    {
        char temp[1024];
        filename.getData(temp, 1024);

        try
        {
            _impl->_chai.use(temp);
        }
        catch (const chaiscript::exception::eval_error &e)
        {
            con_err("load error '" << filename << "': " << e.pretty_print().c_str());
        }
    }

    void iScriptEngine::execute(const std::string &code)
    {
        try
        {
            _impl->_chai.eval(code);
        }
        catch (const chaiscript::exception::eval_error &e)
        {
            con_err("eval error: " << e.pretty_print().c_str());
        }
    }
}