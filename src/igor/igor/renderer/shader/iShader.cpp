// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/shader/iShader.h>

#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

namespace igor
{

    iShader::iShader()
    {
        _shaderProgram = glCreateProgram();
    }

    iShader::~iShader()
    {
        glDeleteProgram(_shaderProgram);
    }

    void iShader::addShader(iaString filename, iShaderObjectType_New type)
    {
        iaFile file(iResourceManager::getInstance().getPath(filename));
        if (file.open(false))
        {
            auto fileSize = file.getSize();
            char *buffer = new char[fileSize + 1];
            buffer[fileSize] = 0;
            file.read(fileSize, buffer);
            file.close();

            if (addShader(buffer, type))
            {
                con_info("loaded " << type << " shader \"" << file.getFullFileName() << "\"");
            }
            else
            {
                con_err("can't load shader source from " << filename);
            }

            delete[] buffer;
        }
        else
        {
            con_err("can't open " << filename);
        }
    }

    bool iShader::addShader(const char *source, iShaderObjectType_New type)
    {
        int32 shaderObject = glCreateShader(iRendererUtils::getOGLShaderType(type));

        glShaderSource(shaderObject, 1, &source, nullptr);
        glCompileShader(shaderObject);

        GLint compiled;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &compiled);

        if (compiled != GL_TRUE)
        {
            GLsizei messageLength = 0;
            GLchar message[1024];
            glGetShaderInfoLog(shaderObject, 1024, &messageLength, message);

            if (messageLength > 0)
            {
                con_err("shader compiler error\n"
                        << message);
                con_endl("source:\n"
                         << iaString(source));
            }

            glDeleteShader(shaderObject);
            return false;
        }

        _shaderObjects.push_back(shaderObject);

        return true;
    }

    void iShader::compile()
    {
        for(auto object : _shaderObjects)
        {
            glAttachShader(_shaderProgram, object);
        }

        glLinkProgram(_shaderProgram);

        _shaderObjects.clear();

        _ready = true;
    }

    void iShader::bind()
    {
        glUseProgram(_shaderProgram);
    }

    void iShader::unbind()
    {
        glUseProgram(0);
    }

    bool iShader::isReady()
    {
        return _ready;
    }

} // namespace igor
