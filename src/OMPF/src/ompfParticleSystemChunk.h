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

        /*! \returns size of chunk in bytes

        \param settings the settings the size calculation is based on
        */
        virtual uint32 getSize(const ompfSettings& settings);

    private:

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