// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iShader.h>

#include <iaConsole.h>
#include <iRenderer.h>
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
    }

    iShader::~iShader()
    {
        if (-1 != _shaderProgram)
        {
            if (iRenderer::getInstance().isReady())
            {
                iRenderer::getInstance().destroyShaderProgram(_shaderProgram);
            }
        }
    }

    bool iShader::isReady()
    {
        return _ready;
    }

    void iShader::compile()
    {
        if (iRenderer::getInstance().isReady())
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

    bool iShader::loadSource(const char* source, iShaderObjectType type)
    {
        if (iRenderer::getInstance().isReady())
        {
            int32 shaderObject = iRenderer::getInstance().createShaderObject(type);

            if (!iRenderer::getInstance().compileShaderObject(shaderObject, source))
            {
                iRenderer::getInstance().destroyShaderObject(shaderObject);
                return false;
            }

            _shaderObjects.push_back(shaderObject);
        }
        else
        {
            return false;
        }

        return true;
    }

	const vector<iShaderSource>& iShader::getShaderSources() const
	{
		return _shaderSources;
	}

    void iShader::loadFile(iaString filename, iShaderObjectType type)
    {
		auto iterSources = _shaderSources.begin();
		while(iterSources != _shaderSources.end())
		{
			if ((*iterSources)._filename == filename)
			{
				con_warn("shader " << filename << " already added");
				return;
			}

            iterSources++;
		}

		iShaderSource shaderSource;
		shaderSource._filename = filename;
		shaderSource._type = type;
		_shaderSources.push_back(shaderSource);

        iaFile file(iResourceManager::getInstance().getPath(filename));
        file.open(false);
        auto fileSize = file.getFileSize();
        char* buffer = new char[fileSize+1];
        buffer[fileSize] = 0;
        file.readFromFile(0, fileSize, buffer);
        file.close();

        if (loadSource(buffer, type))
        {
            con_info("loaded " << ((type == iShaderObjectType::Vertex) ? "vertex" : "fragment") << " shader", filename);
        }

        delete[] buffer;
    }

    void iShader::enable()
	{
        if (iRenderer::getInstance().isReady())
        {
            iRenderer::getInstance().useShaderProgram(_shaderProgram);
        }
    }

    void iShader::disable()
    {
        if (iRenderer::getInstance().isReady())
        {
            iRenderer::getInstance().useShaderProgram(0);
        }
    }

}