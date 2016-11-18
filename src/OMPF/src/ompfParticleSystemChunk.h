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

        /*! \returns size of chunk in bytes

        \param settings the settings the size calculation is based on
        */
        virtual uint32 getSize(const ompfSettings& settings);

    private:

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