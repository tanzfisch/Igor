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

#ifndef IGOR_PARTICLESYSTEM2D_H
#define IGOR_PARTICLESYSTEM2D_H

#include <igor/simulation/iParticleSystem.h>
#include <igor/simulation/iParticleEmitter.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/shader_material/iShaderMaterial.h>

namespace igor
{

    /*! convenience wrapper for iParticleSystem to be used for orthogonal projections
     */
    class IGOR_API iParticleSystem2D
    {

    public:
        /*! init particle system
         */
        iParticleSystem2D();

        /*! does nothing
         */
        ~iParticleSystem2D() = default;

        /*! draw particle system
         */
        void draw(const iaMatrixd &matrix = iaMatrixd());

        /*! \returns particle system
         */
        iParticleSystem &getSystem();

        /*! \returns particle emitter
         */
        iParticleEmitter &getEmitter();

        /*! \returns target material
         */
        iMaterialPtr &getTargetMaterial();

    private:
        /*! particle system
         */
        iParticleSystem _particleSystem;

        /*! particle emitter
         */
        iParticleEmitter _particleEmitter;

        /*! particle target material
         */
        iMaterialPtr _material;

        /*! particle material
         */
        iShaderMaterialPtr _shaderMaterial;

        /*! initialize particle system
         */
        void init();

        /*! update particle system
         */
        void update();
    };

}; // namespace igor

#endif // IGOR_PARTICLESYSTEM2D_H