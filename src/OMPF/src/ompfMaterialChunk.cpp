// OMPF 3d model file format
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <ompfMaterialChunk.h>

namespace OMPF
{

    ompfMaterialChunk::ompfMaterialChunk()
        : ompfBaseChunk(OMPFChunkTypes::Material)
    {
        _renderStates[static_cast<unsigned int>(OMPFRenderState::DepthTest)] = static_cast<uint8>(OMPFRenderStateValue::On);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::DepthMask)] = static_cast<uint8>(OMPFRenderStateValue::On);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Blend)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::CullFace)] = static_cast<uint8>(OMPFRenderStateValue::On);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Fog)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Lighting)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Light0)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Light1)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Light2)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Light3)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Light4)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Light5)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Light6)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Light7)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture2D0)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture2D1)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture2D2)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture2D3)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture2D4)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture2D5)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture2D6)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture2D7)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture1D0)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture1D1)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture1D2)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture1D3)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture1D4)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture1D5)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture1D6)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Texture1D7)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::ColorMaskRed)] = static_cast<uint8>(OMPFRenderStateValue::On);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::ColorMaskGreen)] = static_cast<uint8>(OMPFRenderStateValue::On);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::ColorMaskBlue)] = static_cast<uint8>(OMPFRenderStateValue::On);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::ColorMaskAlpha)] = static_cast<uint8>(OMPFRenderStateValue::On);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Wireframe)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::DepthFunc)] = static_cast<uint8>(OMPFRenderStateValue::Less);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::CullFaceFunc)] = static_cast<uint8>(OMPFRenderStateValue::Back);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::BlendFuncSource)] = static_cast<uint8>(OMPFRenderStateValue::SourceAlpha);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::BlendFuncDestination)] = static_cast<uint8>(OMPFRenderStateValue::OneMinusSourceAlpha);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Instanced)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::InstancedFunc)] = static_cast<uint8>(OMPFRenderStateValue::PositionOrientation);
    }

    void ompfMaterialChunk::setMaterialName(const iaString& material)
    {
        _materialName = material;
    }

    iaString ompfMaterialChunk::getMaterialName() const
    {
        return _materialName;
    }

    void ompfMaterialChunk::setOrder(int32 order)
    {
        _order = order;
    }

    int32 ompfMaterialChunk::getOrder() const
    {
        return _order;
    }

    void ompfMaterialChunk::addShader(const iaString& filename, OMPFShaderType type)
    {
        Shader shader;
        shader._filename = filename;
        shader._type = type;

        auto iter = _shaders.begin();
        while (iter != _shaders.end())
        {
            if ((*iter)._filename == filename)
            {
                con_warn("filename allready exists " << filename);
                return;
            }

            iter++;
        }

        _shaders.push_back(shader);
    }

    void ompfMaterialChunk::removeShader(const iaString& filename)
    {
        auto iter = _shaders.begin();
        while (iter != _shaders.end())
        {
            if ((*iter)._filename == filename)
            {
                _shaders.erase(iter);
                return;
            }
        }

        con_warn("filename not found " << filename);
    }

    void ompfMaterialChunk::setRenderStateValue(OMPFRenderState state, OMPFRenderStateValue value)
    {
        _renderStates[static_cast<int>(state)] = static_cast<uint8>(value);
    }

    OMPFRenderStateValue ompfMaterialChunk::getRenderStateValue(OMPFRenderState state) const
    {
        return static_cast<OMPFRenderStateValue>(_renderStates[static_cast<int>(state)]);
    }

    iaString ompfMaterialChunk::getShaderFilename(uint32 index) const
    {
        return _shaders[index]._filename;
    }

    OMPFShaderType ompfMaterialChunk::getShaderType(uint32 index) const
    {
        return _shaders[index]._type;
    }

    uint32 ompfMaterialChunk::getShaderObjectCount() const
    {
        return static_cast<uint32>(_shaders.size());
    }

    void ompfMaterialChunk::printInfo()
    {
        con_debug_endl("---------------------------------------------------");
        con_debug_endl("ompfMaterialChunk " << getName());
        con_debug_endl("materialName " << _materialName);
        con_debug_endl("shader count " << _shaders.size());

        auto iterShader = _shaders.begin();
        while (iterShader != _shaders.end())
        {
            con_debug_endl("shader " << ((*iterShader)._filename));
            iterShader++;
        }
    }

    uint32 ompfMaterialChunk::getSize(const ompfSettings& settings)
    {
        uint32 size = ompfBaseChunk::getSize(settings);
        size += _materialName.getUTF8Size() + 2;
        size += 1; // shader count
        
        for (int i = 0; i < static_cast<int>(_shaders.size()); ++i)
        {
            size += 1; // shader type size
            size += _shaders[i]._filename.getUTF8Size() + 2;
        }

        size += _renderStateSetCount; // render states count
        size += 4; // order
        return size;
    }

    bool ompfMaterialChunk::write(ofstream& file, const ompfSettings& settings)
    {
        if (!ompfBaseChunk::write(file, settings))
        {
            return false;
        }

        if (!iaSerializable::writeUTF8(file, _materialName))
        {
            return false;
        }

        if (!iaSerializable::writeInt32(file, _order))
        {
            return false;
        }

        uint8 shaderCount = static_cast<uint8>(_shaders.size());
        if (!iaSerializable::writeUInt8(file, shaderCount))
        {
            return false;
        }

        auto iterShader = _shaders.begin();
        while (iterShader != _shaders.end())
        {
            if (!iaSerializable::writeUInt8(file, static_cast<uint8>((*iterShader)._type)))
            {
                return false;
            }

            if (!iaSerializable::writeUTF8(file, (*iterShader)._filename))
            {
                return false;
            }
            iterShader++;
        }

        if (!iaSerializable::write(file, reinterpret_cast<char*>(_renderStates), 0x28))
        {
            return false;
        }

#ifdef __IGOR_DEBUG__
        printInfo();
#endif

        return true;
    }

    bool ompfMaterialChunk::read(ifstream& file, ompfSettings& settings)
    {
        if (!ompfBaseChunk::read(file, settings))
        {
            return false;
        }

        if (!iaSerializable::readUTF8(file, _materialName))
        {
            return false;
        }

        if (!iaSerializable::readInt32(file, _order))
        {
            return false;
        }

        uint8 shaderCount = 0;
        if (!iaSerializable::readUInt8(file, shaderCount))
        {
            return false;
        }

        for (int i = 0; i < shaderCount; ++i)
        {
            uint8 type = 0;
            if (!iaSerializable::readUInt8(file, type))
            {
                return false;
            }

            iaString filename;
            if (!iaSerializable::readUTF8(file, filename))
            {
                return false;
            }

            addShader(filename, static_cast<OMPFShaderType>(type));
        }

        if (!iaSerializable::read(file, reinterpret_cast<char*>(_renderStates), 0x28))
        {
            return false;
        }

#ifdef __IGOR_DEBUG__
        printInfo();
#endif

        return true;
    }


}
