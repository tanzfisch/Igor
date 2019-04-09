// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iShader.h>
#include <iRenderer.h>

#include <iaConsole.h>
#include <iResourceManager.h>
#include <iaFile.h>
using namespace IgorAux;

namespace Igor
{

    iShader::iShader()
    {
        if (iRenderer::getInstance().isReady())
        {
            _shaderProgram = iRenderer::getInstance().createShaderProgram();
        }
        else
        {
            con_err("renderer was not ready yet");
        }
    }

    iShader::~iShader()
    {
        if (iShader::INVALID_SHADER_ID != _shaderProgram)
        {
            if (iRenderer::getInstance().isReady())
            {
                iRenderer::getInstance().destroyShaderProgram(_shaderProgram);
            }
        }
    }

    uint32 iShader::getProgram() const
    {
        return _shaderProgram;
    }

    bool iShader::isReady()
    {
        return _ready;
    }

    void iShader::compile()
    {
        if (iRenderer::getInstance().isReady())
        {
            if (iShader::INVALID_SHADER_ID != _shaderProgram)
            {
                iRenderer::getInstance().linkShaderProgram(_shaderProgram, _shaderObjects);

                auto object = _shaderObjects.begin();
                while (_shaderObjects.end() != object)
                {
                    iRenderer::getInstance().destroyShaderObject((*object));
                    object++;
                }

                _shaderObjects.clear();

                _ready = true;
            }
        }
        else
        {
            con_err("renderer was not ready");
        }
    }

    bool iShader::loadSource(const char* source, iShaderObjectType type)
    {
        if (iRenderer::getInstance().isReady())
        {
            int32 shaderObject = iRenderer::getInstance().createShaderObject(type);
            if (shaderObject != iRenderer::INVALID_ID)
            {
                if (!iRenderer::getInstance().compileShaderObject(shaderObject, source))
                {
                    iRenderer::getInstance().destroyShaderObject(shaderObject);
                    return false;
                }

                _shaderObjects.push_back(shaderObject);
            }
            else
            {
                con_err("failed to create shader object");
            }
        }
        else
        {
            return false;
        }

        return true;
    }

    void iShader::loadFile(iaString filename, iShaderObjectType type)
    {
        iaFile file(iResourceManager::getInstance().getPath(filename));
        if (file.open(false))
        {
            auto fileSize = file.getFileSize();
            char* buffer = new char[fileSize + 1];
            buffer[fileSize] = 0;
            file.readFromFile(0, fileSize, buffer);
            file.close();

            if (loadSource(buffer, type))
            {
                con_info("loaded " << ((type == iShaderObjectType::Vertex) ? "vertex" : "fragment") << " shader", file.getFullFileName());
            }

            delete[] buffer;
        }
        else
        {
            con_err("can't open " << filename);
        }
    }

    void iShader::enable()
	{
        if (iRenderer::getInstance().isReady())
        {
            if (iShader::INVALID_SHADER_ID != _shaderProgram)
            {
                iRenderer::getInstance().useShaderProgram(_shaderProgram);
            }
        }
        else
        {
            con_err("renderer is not ready");
        }
    }

    void iShader::disable()
    {
        if (iRenderer::getInstance().isReady())
        {
            iRenderer::getInstance().useShaderProgram(0);
        }
        else
        {
            con_err("renderer is not ready");
        }
    }

}
