// OMPF 3d model file format
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <ompf/chunks/ompfMaterialChunk.h>

namespace OMPF
{

    const uint32 ompfMaterialChunk::_renderStateSetCount;

    ompfMaterialChunk::ompfMaterialChunk()
        : ompfBaseChunk(OMPFChunkType::Material)
    {
        for (int i = 0; i < _renderStateSetCount; ++i)
        {
            _renderStates[i] = 0;
        }

        _renderStates[static_cast<unsigned int>(OMPFRenderState::DepthTest)] = static_cast<uint8>(OMPFRenderStateValue::On);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::DepthMask)] = static_cast<uint8>(OMPFRenderStateValue::On);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Blend)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::CullFace)] = static_cast<uint8>(OMPFRenderStateValue::On);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Wireframe)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::DepthFunc)] = static_cast<uint8>(OMPFRenderStateValue::Less);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::CullFaceFunc)] = static_cast<uint8>(OMPFRenderStateValue::Back);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::Instanced)] = static_cast<uint8>(OMPFRenderStateValue::Off);
        _renderStates[static_cast<unsigned int>(OMPFRenderState::InstancedFunc)] = static_cast<uint8>(OMPFRenderStateValue::PositionOrientation);
    }

    void ompfMaterialChunk::setMaterialName(const iaString &material)
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

    void ompfMaterialChunk::addShader(const iaString &filename, const iaString &source, OMPFShaderType type)
    {
        _shaders.push_back({filename, source, type});
    }

    void ompfMaterialChunk::removeShader(const iaString &filename)
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

    OMPFRenderStateValue ompfMaterialChunk::getRenderState(OMPFRenderState state) const
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
        con_debug("---------------------------------------------------" << endlTab
                                                                        << "ompfMaterialChunk: " << getName() << endlTab
                                                                        << "materialName: " << _materialName << endlTab
                                                                        << "shader count: " << _shaders.size());

        for (auto shader : _shaders)
        {
            con_debug("shader: " << shader._filename);
        }
    }

    uint32 ompfMaterialChunk::getSize(const ompfSettings &settings)
    {
        uint32 size = ompfBaseChunk::getSize(settings);
        size += static_cast<uint32>(_materialName.getUTF8Size()) + 2;
        size += 1; // shader count

        for (int i = 0; i < static_cast<int>(_shaders.size()); ++i)
        {
            size += 1; // shader type size
            size += static_cast<uint32>(_shaders[i]._filename.getUTF8Size()) + 2;
        }

        size += _renderStateSetCount; // render states count
        size += 4;                    // order
        return size;
    }

    bool ompfMaterialChunk::write(std::ofstream &stream, const ompfSettings &settings)
    {
        if (!ompfBaseChunk::write(stream, settings))
        {
            return false;
        }

        if (!iaSerializable::writeUTF8(stream, _materialName))
        {
            return false;
        }

        if (!iaSerializable::writeInt32(stream, _order))
        {
            return false;
        }

        uint8 shaderCount = static_cast<uint8>(_shaders.size());
        if (!iaSerializable::writeUInt8(stream, shaderCount))
        {
            return false;
        }

        auto iterShader = _shaders.begin();
        while (iterShader != _shaders.end())
        {
            if (!iaSerializable::writeUInt8(stream, static_cast<uint8>((*iterShader)._type)))
            {
                return false;
            }

            if (!iaSerializable::writeUTF8(stream, (*iterShader)._filename))
            {
                return false;
            }
            iterShader++;
        }

        if (!iaSerializable::write(stream, reinterpret_cast<char *>(_renderStates), _renderStateSetCount))
        {
            return false;
        }

#ifdef __IGOR_DEBUG__
        printInfo();
#endif

        return true;
    }

    bool ompfMaterialChunk::read(std::ifstream &stream, ompfSettings &settings)
    {
        if (!ompfBaseChunk::read(stream, settings))
        {
            return false;
        }

        if (!iaSerializable::readUTF8(stream, _materialName))
        {
            return false;
        }

        if (!iaSerializable::readInt32(stream, _order))
        {
            return false;
        }

        uint8 shaderCount = 0;
        if (!iaSerializable::readUInt8(stream, shaderCount))
        {
            return false;
        }

        for (int i = 0; i < shaderCount; ++i)
        {
            uint8 type = 0;
            if (!iaSerializable::readUInt8(stream, type))
            {
                return false;
            }

            iaString filename;
            if (!iaSerializable::readUTF8(stream, filename))
            {
                return false;
            }

            addShader(filename, "", static_cast<OMPFShaderType>(type));
        }

        if (!iaSerializable::read(stream, reinterpret_cast<char *>(_renderStates), _renderStateSetCount))
        {
            return false;
        }

#ifdef __IGOR_DEBUG__
        printInfo();
#endif

        return true;
    }

} // namespace OMPF
