//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #
//
//  (c) Copyright by Martin Loga
//
// This library is free software; you can redistribute it and or modify it   
// under the terms of the GNU Lesser General Public License as published by  
// the Free Software Foundation; either version 2.1 of the License, or (at   
// your option) any later version.                                           
// 
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
// 
// You should have received a copy of the GNU Lesser General Public          
// License along with this library; if not, write to the Free Software       
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA 
// 
// contact: martinloga@gmx.de  

#ifndef __PARTICLESYSTEMCHUNK__
#define __PARTICLESYSTEMCHUNK__

#include <ompfBaseChunk.h>

namespace OMPF
{

    /*! ompf emitter chunk
    */
	class OMPF_API ompfParticleSystemChunk : public ompfBaseChunk
	{

	public:

        /*! init base class
        */
        ompfParticleSystemChunk();

        /*! does nothing
        */
		virtual ~ompfParticleSystemChunk() = default;

        /*! sets the maximum particle count

        set to zero if you don't want a limit of created particles

        \param max the maximum count of particles used
        */
        void setMaxParticleCount(uint16 max);

        /*! \returns the maximum particle count used
        */
        uint16 getMaxParticleCount() const;

        /*! sets if the particle system runs in a loop

        \param loop if true particle system runs in loop
        */
        void setLoop(bool loop = true);

        /*! \returns true if particle system runs in loop
        */
        bool getLoop() const;

        /*! sets rainbow gradient for particles color

        \param rainbow the rainbow gradient
        */
        void setColorGradient(const iaGradientColor4f& colorGradient);

        /*! returns the rainbow gradient

        \param[out] rainbow out value for the rainbow gradient
        */
        void getColorGradient(iaGradientColor4f& colorGradient) const;

        /*! sets emission gradient for particles per frame

        \param emissionGradient the emission gradient
        */
        void setEmissionGradient(const iaGradientf& emissionGradient);

        /*! returns the emission gradient

        \param[out] emissionGradient out value for the emission gradient
        */
        void getEmissionGradient(iaGradientf& emissionGradient) const;

        /*! sets the range of vortex torque

        \param min minimum vortex torque
        \param max maximum vortex torque
        */
        void setVortexTorque(float32 min, float32 max);

        /*! \returns minimum vortex torque
        */
        float32 getVortexTorqueMin();

        /*! \returns maximum vortex torque
        */
        float32 getVortexTorqueMax();

        /*! sets minimum and maximum range of vortexes

        \param min minimum range of vortex
        \param max maximum range of vortex
        */
        void setVortexRange(float32 min, float32 max);

        /*! \returns minimum range of vortexes
        */
        float32 getVortexRangeMin();

        /*! \returns maximum range of vortexes
        */
        float32 getVortexRangeMax();

        /*! sets vortex check range

        \param particles distance in indexes from vortex particle
        */
        void setVortexCheckRange(uint8 particles);

        /*! \return vortex check rangein indexes from vortex particle
        */
        uint8 getVortexCheckRange();

        /*! sets the vortex to particle ratio

        \param rate the rate of vortex particles to particles. default 0.01 = 1%
        */
        void setVortexToParticleRate(float32 rate);

        /*! \returns likeliness of vortex particle to appear
        */
        float32 getVortexToParticleRate() const;

        /*! sets vorticity confinement force

        \param vorticityConfinement the vorticity confinement force
        */
        void setVorticityConfinement(float32 vorticityConfinement);

        /*! \returns vorticity confinement force
        */
        float32 getVorticityConfinement();

        /*! \returns size of chunk in bytes

        \param settings the settings the size calculation is based on
        */
        virtual uint32 getSize(const ompfSettings& settings);

        /*! sets the tiling resolution of the first texture layer

        \param columns column count (<= 1)
        \param rows row count (<= 1)
        */
        void setFirstTextureTiling(uint8 columns, uint8 rows);

        /*! \returns the horizontal tiling resolution of the first texture layer
        */
        uint8 getFirstTextureColumns() const;

        /*! \returns the vertical tiling resolution of the first texture layer
        */
        uint8 getFirstTextureRows() const;

        /*! sets visible gradient for particles per frame

        \param visibleGradient the visible gradient
        */
        void setStartVisibleTimeGradient(const iaGradientVector2f& visibleGradient);

        /*! returns the emission gradient

        \param[out] visibleGradient out value for the visible gradient
        */
        void getStartVisibleTimeGradient(iaGradientVector2f& visibleGradient) const;

        /*! sets size scale gradient for particles per frame

        \param sizeScaleGradient the size gradient
        */
        void setSizeScaleGradient(const iaGradientf& sizeScaleGradient);

        /*! returns the size scale gradient

        \param[out] sizeScaleGradient out value for the size gradient
        */
        void getSizeScaleGradient(iaGradientf& sizeScaleGradient) const;

        /*! sets start size gradient for particles at birth

        \param sizeGradient the start size gradient with min and max values
        */
        void setStartSizeGradient(const iaGradientVector2f& sizeGradient);

        /*! returns the start size gradient for particles at birth

        \param[out] sizeGradient out value for the start size gradient
        */
        void getStartSizeGradient(iaGradientVector2f& sizeGradient) const;

        /*! sets min max start velocity gradient for particles at birth

        \param velocityGradient the min max start velocity gradient
        */
        void setStartVelocityGradient(const iaGradientVector2f& velocityGradient);

        /*! returns the min max start velocity gradient for particles at birth

        \param[out] velocityGradient out value for the start velocity gradient
        */
        void getStartVelocityGradient(iaGradientVector2f& velocityGradient) const;

        /*! sets min max start lift gradient for particles at birth

        \param liftGradient the min max start lift gradient
        */
        void setStartLiftGradient(const iaGradientVector2f& liftGradient);

        /*! returns the min max start lift gradient for particles at birth

        \param[out] liftGradient out value for the start lift gradient
        */
        void getStartLiftGradient(iaGradientVector2f& liftGradient) const;

        /*! sets min max start orientation gradient for particles at birth

        \param orientationGradient the min max start orientation gradient
        */
        void setStartOrientationGradient(const iaGradientVector2f& orientationGradient);

        /*! returns the min max start lift gradient for particles at birth

        \param[out] orientationGradient out value for the start orientation gradient
        */
        void getStartOrientationGradient(iaGradientVector2f& orientationGradient) const;

        /*! sets start min max orientation rate gradient for particles during visible time

        \param orientationRateGradient the min max orientation rate gradient
        */
        void setStartOrientationRateGradient(const iaGradientVector2f& orientationRateGradient);

        /*! returns the start min max orientation offset gradient for particles during visuble time

        \param[out] orientationRateGradient out value for the orientation rate gradient
        */
        void getStartOrientationRateGradient(iaGradientVector2f& orientationRateGradient) const;

        /*! sets air drag 
        0.0-1.0
        1.0 means basically no air drag
        0.0 is a full stop
        and about 0.99 is a usefull value for simulating air drag

        \param airDrag the air drag factor (0.0 - 1.0)
        */
        void setAirDrag(float32 airDrag);

        /*! \returns air drag factor
        */
        float32 getAirDrag() const;

        /*! sets period time of this particle system

        if loop is true. the particle system will be restarted after this time
        if loop is false. the particle system simulation will be stopped

        \param periodTime the period time
        */
        void setPeriodTime(float32 periodTime);

        /*! \returns period time of this particle system
        */
        float32 getPeriodTime() const;

        /*! sets the velocity oriented flag
        particles will be rendered oriented by thair velocity and screen

        default id off

        \param velocityOriented if true particles get rendered velocity oriented
        */
        void setVelocityOriented(bool velocityOriented = true);

        /*! \returns velocity oriented flag
        */
        bool getVelocityOriented() const;

        /*! sets first texture

        \param filename file name of texture
        */
        void setTextureA(const iaString& filename);

        /*! sets second texture

        \param filename file name of texture
        */
        void setTextureB(const iaString& filename);

        /*! sets third texture

        \param filename file name of texture
        */
        void setTextureC(const iaString& filename);

        /*! \returns file name of first texture
        */
        iaString getTextureA() const;

        /*! \returns file name of second texture
        */
        iaString getTextureB() const;

        /*! \returns file name of third texture
        */
        iaString getTextureC() const;

        /*! sets material chunk id
        \param id the material chunk id
        */
        void setMaterialChunkID(uint32 id);

        /*! \returns material chunk id
        */
        uint32 getMaterialChunkID() const;

    private:

        bool _velocityOriented = false;
        iaString _textureA;
        iaString _textureB;
        iaString _textureC;

        float32 _airDrag = 0;
        float32 _periodTime = 0;
        float32 _vortexToParticleRate = 0;
        float32 _vorticityConfinement = 0;

        /*! material chunk id
        */
        uint32 _materialChunkID = OMPFDefaultConfiguration::INVALID_CHUNK_ID;

        iaGradientVector2f _orientationRateGradient;
        iaGradientVector2f _orientationGradient;
        iaGradientVector2f _liftGradient;
        iaGradientVector2f _velocityGradient;

        /*! start size gradient
        */
        iaGradientVector2f _sizeGradient;

        /*! size scale gradient
        */
        iaGradientf _sizeScaleGradient;

        /*! start visibility time
        */
        iaGradientVector2f _startVisibleTimeGradient;

        /*! first texture tiling in collumns
        */
        uint8 _firstTectureTilingColumns;

        /*! first texture tiling in rows
        */
        uint8 _firstTectureTilingRows;

        /*! min vortex tourque
        */
        float32 _minVortexTorque = 0;

        /*! max vortex tourque
        */
        float32 _maxVortexTorque = 0;

        /*! min vortex range
        */
        float32 _minVortexRange = 0;

        /*! max vortex range
        */
        float32 _maxVortexRange = 0;
        
        /*! vortex check range
        */
        uint8 _vortexCheckRange = 0;

        /*! emission gradient
        */
        iaGradientf _emissionGradient;

        /*! color gradient for particles during their lifetime
        */
        iaGradientColor4f _colorGradient;

        /*! maximum particle count
        */
        uint16 _maxParticleCount = 500;

        /*! if true particle system is in endless loop
        */
        bool _loop = true;

        /*! writes chunk to stream

        \param stream destination stream
        \param settings the settings how to write the chunk
        */
        virtual bool write(ofstream& stream, const ompfSettings& settings);

        /*! reads chunk from stream

        \param stream source stream
        \param settings the settings how to read the chunk
        */
        virtual bool read(ifstream& stream, ompfSettings& settings);

	};

}

#endif 