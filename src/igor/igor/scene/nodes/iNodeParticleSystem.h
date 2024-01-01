//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
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

#ifndef __IGOR_NODEPARTICLESYSTEM__
#define __IGOR_NODEPARTICLESYSTEM__

#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/resources/texture/iTexture.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/simulation/iParticleSystem.h>

#include <iaux/system/iaEvent.h>
#include <iaux/data/iaKeyFrameGraph.h>
using namespace iaux;

#include <vector>

namespace igor
{

    /*! event triggered when particle system is finished
     */
    IGOR_EVENT_DEFINITION(iParticleSystemFinished, void, iNodeID);

    IGOR_DISABLE_WARNING(4275); // yes iNodeVolume is intentionally not a fully exported interface

    /*! node to control a 3d particle system
     */
    class IGOR_API iNodeParticleSystem : public iNodeVolume
    {

        friend class iNodeManager;

    public:
        /*! clears all particles
         */
        void reset();

        /*! starts or resumes particle system
         */
        void start();

        /*! stops or pauses the particle system
         */
        void stop();

        /*! \returns true if particle system is currently running
         */
        bool isRunning();

        /*! \returns true if particle system is finished
         */
        bool isFinished() const;

        /*! \returns current particle count in use

        there is no setter because the amount of particles depends on particle emission over time
        */
        uint32 getParticleCount();

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
        bool isLooped() const;

        /*! sets emitter node id

        \param emitterID the emitter node's id
        */
        void setEmitter(uint64 emitterID);

        /*! \returns the emitter node's id
         */
        uint64 getEmitter() const;

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

        /*! sets the tiling resolution of the first texture layer

        \param columns column count (<= 1)
        \param rows row count (<= 1)
        */
        void setTextureTiling(uint32 columns, uint32 rows);

        /*! \returns the horizontal tiling resolution of the first texture layer
         */
        uint32 getTextureColumns() const;

        /*! \returns the vertical tiling resolution of the first texture layer
         */
        uint32 getTextureRows() const;

        /*! sets rainbow gradient for particles color

        \param rainbow the rainbow gradient
        */
        void setColorGradient(const iaKeyFrameGraphColor4f &colorGradient);

        /*! returns the rainbow gradient

        \param[out] rainbow out value for the rainbow gradient
        */
        void getColorGradient(iaKeyFrameGraphColor4f &colorGradient) const;

        /*! sets emission gradient for particles per frame

        \param emissionGradient the emission gradient
        */
        void setEmissionGradient(const iaKeyFrameGraphf &emissionGradient);

        /*! returns the emission gradient

        \param[out] emissionGradient out value for the emission gradient
        */
        void getEmissionGradient(iaKeyFrameGraphf &emissionGradient) const;

        /*! sets visible gradient for particles per frame

        \param visibleGradient the visible gradient
        */
        void setStartAgeGradient(const iaKeyFrameGraphVector2f &visibleGradient);

        /*! returns the emission gradient

        \param[out] visibleGradient out value for the visible gradient
        */
        void getStartAgeGradient(iaKeyFrameGraphVector2f &visibleGradient) const;

        /*! sets size scale gradient for particles per frame

        \param sizeScaleGradient the size gradient
        */
        void setSizeScaleGradient(const iaKeyFrameGraphf &sizeScaleGradient);

        /*! returns the size scale gradient

        \param[out] sizeScaleGradient out value for the size gradient
        */
        void getSizeScaleGradient(iaKeyFrameGraphf &sizeScaleGradient) const;

        /*! sets start size gradient for particles at birth

        \param sizeGradient the start size gradient with min and max values
        */
        void setStartSizeGradient(const iaKeyFrameGraphVector2f &sizeGradient);

        /*! returns the start size gradient for particles at birth

        \param[out] sizeGradient out value for the start size gradient
        */
        void getStartSizeGradient(iaKeyFrameGraphVector2f &sizeGradient) const;

        /*! sets min max start velocity gradient for particles at birth

        \param velocityGradient the min max start velocity gradient
        */
        void setStartVelocityGradient(const iaKeyFrameGraphVector2f &velocityGradient);

        /*! returns the min max start velocity gradient for particles at birth

        \param[out] velocityGradient out value for the start velocity gradient
        */
        void getStartVelocityGradient(iaKeyFrameGraphVector2f &velocityGradient) const;

        /*! sets min max start lift gradient for particles at birth

        \param liftGradient the min max start lift gradient
        */
        void setStartLiftGradient(const iaKeyFrameGraphVector2f &liftGradient);

        /*! returns the min max start lift gradient for particles at birth

        \param[out] liftGradient out value for the start lift gradient
        */
        void getStartLiftGradient(iaKeyFrameGraphVector2f &liftGradient) const;

        /*! sets min max start orientation gradient for particles at birth

        \param orientationGradient the min max start orientation gradient
        */
        void setStartOrientationGradient(const iaKeyFrameGraphVector2f &orientationGradient);

        /*! returns the min max start lift gradient for particles at birth

        \param[out] orientationGradient out value for the start orientation gradient
        */
        void getStartOrientationGradient(iaKeyFrameGraphVector2f &orientationGradient) const;

        /*! sets start min max orientation rate gradient for particles during visible time

        \param orientationRateGradient the min max orientation rate gradient
        */
        void setStartOrientationRateGradient(const iaKeyFrameGraphVector2f &orientationRateGradient);

        /*! returns the start min max orientation offset gradient for particles during visuble time

        \param[out] orientationRateGradient out value for the orientation rate gradient
        */
        void getStartOrientationRateGradient(iaKeyFrameGraphVector2f &orientationRateGradient) const;

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

        /*! sets the air drag

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
        void setTextureA(const iaString &filename);

        /*! sets second texture

        \param filename file name of texture
        */
        void setTextureB(const iaString &filename);

        /*! sets third texture

        \param filename file name of texture
        */
        void setTextureC(const iaString &filename);

        /*! \returns file name of first texture
         */
        iaString getTextureA() const;

        /*! \returns file name of second texture
         */
        iaString getTextureB() const;

        /*! \returns file name of third texture
         */
        iaString getTextureC() const;

        /*! sets the increment the tile index is progressing per frame (default = 0.0)

        franctional values are allowed

        \param tileIncrement the tile increment to use
        */
        void setTileIncrement(float32 tileIncrement);

        /*! \returns tile increment
         */
        float32 getTileIncrement() const;

        /*! register delegate to particle system finished event

        \param delegate the delegate to register
        */
        void registerParticleSystemFinishedDelegate(iParticleSystemFinishedDelegate delegate);

        /*! unregister delegate from particle system finished event

        \param delegate the delegate to unregister
        */
        void unregisterParticleSystemFinishedDelegate(iParticleSystemFinishedDelegate delegate);

    private:
        /*! triggered when particle system is finished
         */
        iParticleSystemFinishedEvent _fishedEvent;

        /*! inverted world matrix
         */
        iaMatrixd _worldMatrixInv;

        /*! id of emitter node
         */
        uint64 _emitterID = iNode::INVALID_NODE_ID;

        /*! the actual particle system implementation
         */
        iParticleSystem _particleSystem;

        /*! target material
         */
        iMaterialPtr _material;

        /*! called after a node was copied

        \param nodeIDMap map with old node ids to new node ids
        */
        void onPostCopyLink(std::map<uint32, uint32> &nodeIDMap);

        /*! draw function
         */
        void draw();

        /*! called when there is a new transformation matrix for this node

        \param matrix the new world matrix
        */
        void onUpdateTransform(iaMatrixd &matrix) override;

        /*!
        \todo this is just a workaround untill we calc the particle system frames in a seperate thread
        */
        void handle();

        /*! init members
         */
        iNodeParticleSystem();

        /*! copy ctor

        \param node make copy from this node
        */
        iNodeParticleSystem(iNodeParticleSystem *node);

        /*! clean up
         */
        virtual ~iNodeParticleSystem();
    };

    IGOR_ENABLE_WARNING(4275);

}; // namespace igor

#endif // __IGOR_NODEPARTICLESYSTEM__
