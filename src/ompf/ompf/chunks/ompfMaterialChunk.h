//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #  3d model file format
//
// (c) Copyright 2012-2023 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: igorgameengine@protonmail.com

#ifndef __MATERIALCHUNK__
#define __MATERIALCHUNK__

#include <ompf/chunks/ompfBaseChunk.h>

#include <vector>

namespace OMPF
{

    /*! render states that are stored in OMPF
    */
    enum class OMPFRenderState
    {
        DepthTest = 0x00,
        DepthMask = 0x01,
        Blend = 0x02,
        CullFace = 0x03,
        Wireframe = 0x0c,
        DepthFunc = 0x0d,
        CullFaceFunc = 0x0e,
        BlendFuncSource = 0x0f,
        BlendFuncDestination = 0x10,
        Instanced = 0x11,
        InstancedFunc = 0x12
    };

    /*! possible values for render states
    */
    enum class OMPFRenderStateValue
    {
        Off = 0x00,
        On = 0x01,
        One = 0x02,
        Zero = 0x03,
        DestinationColor = 0x04,
        OneMinusDestinationColor = 0x05,
        SourceAlpha = 0x06,
        OneMinusSourceAlpha = 0x07,
        DestinationAlpha = 0x08,
        OneMinusDestinationAlpha = 0x09,
        SourceColor = 0x0a,
        OneMinusSourceColor = 0x0b,
        Never = 0x0c,
        Less = 0x0d,
        Equal = 0x0e,
        LessOrEqual = 0x0f,
        Greater = 0x10,
        NotEqual = 0x11,
        GreaterOrEqual = 0x12,
        Always = 0x13,
        Front = 0x14,
        Back = 0x015,
        Invalid = 0x16,
        PositionOrientation = 0x17,
        Position = 0x18
    };

    /*! shader types supported in ompf
    */
    enum class OMPFShaderType
    {
        Vertex = 0x00,
        Fragment = 0x01,
        Geometry = 0x02
    };

    /*! the material chunk contains all details to set up the renderer to render a certain object
    */
    class OMPF_API ompfMaterialChunk : public ompfBaseChunk
    {

        friend class OMPF;

        /*! internal helper struct to store shader information
        */
        struct Shader
        {
            iaString _filename;
            iaString _source;
            OMPFShaderType _type;
        };

    public:
        /*! initializes default values 
        */
        ompfMaterialChunk();

        /*! does nothing
        */
        virtual ~ompfMaterialChunk() = default;

        /*! sets name of material

        \param material name of the material
        */
        void setMaterialName(const iaString &material);

        /*! \retruns name of material
        */
        iaString getMaterialName() const;

        /*! sets render order index

        \param order render order index
        */
        void setOrder(int32 order);

        /*! \returns render order index
        */
        int32 getOrder() const;

        /*! \returns size of chunk
        */
        uint32 getSize(const ompfSettings &settings) override;

        /*! adds shader to export list

        \param filename name of the shader (optional)
        \param source the shader source
        \param type the type of the shader
        */
        void addShader(const iaString &filename, const iaString &source, OMPFShaderType type);

        /*! \returns shader name with specified index

        \param index the index of the shader name to be returned
        */
        iaString getShaderFilename(uint32 index) const;

        /*! \returns shader source for given shader index

        \param index the index of the shader
        */
        iaString getShaderSource(uint32 index) const;

        /*! \returns shader type with specified index

        \param index the index of the shader type to be returned
        */
        OMPFShaderType getShaderType(uint32 index) const;

        /*! \returns shader object count
        */
        uint32 getShaderObjectCount() const;

        /*! sets value of a render state

        \param state the state to change
        \param value to value to set for the state
        */
        void setRenderStateValue(OMPFRenderState state, OMPFRenderStateValue value);

        /*! \returns value of given render state

        \param state the state given
        */
        OMPFRenderStateValue getRenderState(OMPFRenderState state) const;

    private:
        /*! constant to define how many render state we have
        */
        static const uint32 _renderStateSetCount = 30; // 11 reserve

        /*! name of the material
        */
        iaString _materialName;

        /*! render order index
        */
        int32 _order = 100;

        /*! list of shaders stored
        */
        std::vector<Shader> _shaders;

        /*! render states
        */
        uint8 _renderStates[_renderStateSetCount];

        /*! debug output
        */
        void printInfo();

        /*! writes data to file stream

        \param file the file stream to write to
        \param settings ompf settings that apply for this export
        */
        virtual bool write(std::ofstream &stream, const ompfSettings &settings);

        /*! read data from file stream

        \param file the file stream to read from
        \param settings ompf settings that apply for this import
        */
        virtual bool read(std::ifstream &stream, ompfSettings &settings);
    };

} // namespace OMPF

#endif