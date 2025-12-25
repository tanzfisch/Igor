// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/script/iScriptEngine.h>

#include <iaux/system/iaConsole.h>

#include <chaiscript/chaiscript.hpp>

#include <codecvt>

namespace igor
{
    static std::string wstringToUtf8(const std::wstring &wstr)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }

    static void print(const std::string &message)
    {
        con_endl(message.c_str());
    }

    static void printInfo(const std::string &message)
    {
        con_info(message.c_str());
    }

    class iScriptEngineImpl
    {
        friend class iScriptEngine;

    public:
        iScriptEngineImpl()
        {
            _chai.add(chaiscript::fun(&print), "print");
            _chai.add(chaiscript::fun(&printInfo), "printInfo");

            // _chai.add(chaiscript::var(&var), wstringToUtf8(name.getData()));
            // _chai.add(chaiscript::const_var(&var), wstringToUtf8(name.getData()));
            // _chai.add(chaiscript::fun(&printInfo), "printInfo");
            // _chai.add(chaiscript::fun(method, obj), wstringToUtf8(name.getData()));
        }

    private:
        /*! chaiscript instance
         */
        chaiscript::ChaiScript _chai;
    };

    iScriptEngine::iScriptEngine()
    {
        _impl = std::make_unique<iScriptEngineImpl>();

        _impl->_chai.eval(R"(
            printInfo("ChaiScript initialized");
            )");
    }

    iScriptEngine::~iScriptEngine()
    {
    }

    void iScriptEngine::executeScript(const iaString &filename)
    {
        try
        {
            _impl->_chai.use(wstringToUtf8(filename.getData()));
        }
        catch (const chaiscript::exception::eval_error &e)
        {
            con_err("load error '" << filename << "': " << e.pretty_print().c_str());
        }
    }

    void iScriptEngine::execute(const iaString &code)
    {
        try
        {
            _impl->_chai.eval(wstringToUtf8(code.getData()));
        }
        catch (const chaiscript::exception::eval_error &e)
        {
            con_err("eval error: " << e.pretty_print().c_str());
        }
    }
}
