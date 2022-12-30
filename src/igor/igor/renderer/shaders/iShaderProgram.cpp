// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/shaders/iShaderProgram.h>

#include <igor/renderer/utils/iRendererUtils.h>
#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaFile.h>

namespace igor
{

    iShaderProgramPtr iShaderProgram::create()
    {
        return std::make_shared<iShaderProgram>();
    }

    iShaderProgram::iShaderProgram()
    {
        _shaderProgram = glCreateProgram();
        GL_CHECK_ERROR();
        con_assert_sticky(_shaderProgram != 0, "can't create shader program");
    }

    iShaderProgram::~iShaderProgram()
    {
        glDeleteProgram(_shaderProgram);
        GL_CHECK_ERROR();
    }

    const std::vector<std::pair<iaString, iShaderObjectType>>& iShaderProgram::getShaderSources() const
    {
        return _sources;
    }

    void iShaderProgram::addShader(const iaString& filename, iShaderObjectType type)
    {
        iaFile file(iResourceManager::getInstance().getPath(filename));

        if (file.open())
        {
            auto fileSize = file.getSize();
            char *buffer = new char[fileSize + 1];
            buffer[fileSize] = 0;
            file.read(fileSize, buffer);
            file.close();

            if (addSource(buffer, type, file.getFullFileName()))
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

    bool iShaderProgram::addSource(const char *source, iShaderObjectType type, const iaString &sourceName)
    {
        int32 shaderObject = glCreateShader(iRendererUtils::convertType(type));
        GL_CHECK_ERROR();

        glShaderSource(shaderObject, 1, &source, nullptr);
        GL_CHECK_ERROR();
        glCompileShader(shaderObject);
        GL_CHECK_ERROR();

        GLint compiled;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &compiled);
        GL_CHECK_ERROR();

        if (compiled != 0)
        {
            GLsizei messageLength = 0;
            GLchar message[1024];
            glGetShaderInfoLog(shaderObject, 1024, &messageLength, message);
            GL_CHECK_ERROR();

            if (messageLength > 0)
            {
                con_err("shader compiler error\n"
                        << message);
                con_endl("source:\n"
                         << iaString(source));
            }

            glDeleteShader(shaderObject);
            GL_CHECK_ERROR();
            return false;
        }

        _shaderObjects.push_back(shaderObject);
        _sources.push_back({sourceName, type});

        return true;
    }

    void iShaderProgram::compile()
    {
        if(_shaderObjects.empty())
        {
            _isValid = false;
            return;
        }

        for (auto object : _shaderObjects)
        {
            glAttachShader(_shaderProgram, object);
            GL_CHECK_ERROR();
        }

        glLinkProgram(_shaderProgram);
        GL_CHECK_ERROR();

        _shaderObjects.clear();
        _isValid = true;
    }

    void iShaderProgram::bind()
    {
        glUseProgram(_shaderProgram);
        GL_CHECK_ERROR();
    }

    void iShaderProgram::unbind()
    {
        glUseProgram(0);
        GL_CHECK_ERROR();
    }

    bool iShaderProgram::isValid()
    {
        return _isValid;
    }

    bool iShaderProgram::hasUniformLocation(const iaString &uniform)
    {
        char temp[128];
        uniform.getData(temp, 128);
        return glGetUniformLocation(_shaderProgram, temp) != -1;
    }

    void iShaderProgram::setInt(const iaString &uniform, int value)
    {
        char temp[128];
        uniform.getData(temp, 128);
        GLint location = glGetUniformLocation(_shaderProgram, temp);
        glUniform1i(location, value);
        GL_CHECK_ERROR();
    }

    void iShaderProgram::setFloat(const iaString &uniform, float32 value)
    {
        char temp[128];
        uniform.getData(temp, 128);
        GLint location = glGetUniformLocation(_shaderProgram, temp);
        glUniform1f(location, value);
        GL_CHECK_ERROR();
    }

    void iShaderProgram::setFloat2(const iaString &uniform, const iaVector2f &value)
    {
        char temp[128];
        uniform.getData(temp, 128);
        GLint location = glGetUniformLocation(_shaderProgram, temp);
        glUniform2f(location, value._x, value._y);
        GL_CHECK_ERROR();
    }

    void iShaderProgram::setFloat3(const iaString &uniform, const iaVector3f &value)
    {
        char temp[128];
        uniform.getData(temp, 128);
        GLint location = glGetUniformLocation(_shaderProgram, temp);
        glUniform3f(location, value._x, value._y, value._z);
        GL_CHECK_ERROR();
    }

    void iShaderProgram::setFloat4(const iaString &uniform, const iaVector4f &value)
    {
        char temp[128];
        uniform.getData(temp, 128);
        GLint location = glGetUniformLocation(_shaderProgram, temp);
        glUniform4f(location, value._x, value._y, value._z, value._w);
        GL_CHECK_ERROR();
    }

    void iShaderProgram::setMatrix(const iaString &uniform, const iaMatrixf &value)
    {
        char temp[128];
        uniform.getData(temp, 128);
        GLint location = glGetUniformLocation(_shaderProgram, temp);
        glUniformMatrix4fv(location, 1, GL_FALSE, value.getData());
        GL_CHECK_ERROR();
    }

} // namespace igor
