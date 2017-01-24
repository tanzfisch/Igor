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
// (c) Copyright 2014-2016 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iPHYSICSMATERIALCOMBO__
#define __iPHYSICSMATERIALCOMBO__

#include <iDefines.h>

#include <iaEvent.h>
#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iPhysicsBody;
    class iPhysicsMaterial;

    /*! event triggered if there was a contact detected
    */
    iaEVENT(iContactEvent, iContactDelegate, void, (iPhysicsBody* body0, iPhysicsBody* body1), (body0, body1));

    /*! describes combination of two materials
    */
    class Igor_API iPhysicsMaterialCombo
    {

        friend class iPhysics;
        
    public:

        /*! sets name of material combo

        \param name the new name of this combo
        */
        void setName(const iaString& name);

        /*! \returns name of material combo
        */
        const iaString& getName() const;

        /*! register delegate to contact event

        \param contactDelegate contact delegate to register
        */
        void registerContactDelegate(iContactDelegate contactDelegate);

        /*! unregister delegate from contact event

        \param contactDelegate contact delegate to unregister
        */
        void unregisterContactDelegate(iContactDelegate contactDelegate);

        /*! sets sofftness of collition between the two materials

        \param value softness factor
        */
        void setSoftness(float32 value);

        /*! sets elasticity coeficient between the two materials

        \param elasticCoef elasticity coeficient
        */
        void setElasticity(float32 elasticCoef);

        /*! sets friction between the two materials

        \param staticFriction static friction value
        \param kineticFriction kinetic friction value
        */
        void setFriction(float32 staticFriction, float32 kineticFriction);

        /*! \returns id of first material
        */
        int64 getMaterial0() const;

        /*! \returns id of second material
        */
        int64 getMaterial1() const;

        /*! triggers contact event

        supposed to be only called by iPhysics but like this we can save one call to wrap it.
        and it does not hurt to call it any time

        \param body0 first body involved in contact
        \param body1 second body involved in contact
        */
        void contact(iPhysicsBody* body0, iPhysicsBody* body1);

        /*! dtor material combo

        \param material0 first material
        \param material1 second material
        */
        iPhysicsMaterialCombo(iPhysicsMaterial* material0, iPhysicsMaterial* material1);

        /*! does nothing
        */
        virtual ~iPhysicsMaterialCombo() = default;

    private:

        /*! first material
        */
        int64 _material0 = 0;

        /*! second material
        */
        int64 _material1 = 0;

        /*! contact event
        */
        iContactEvent _contactEvent;

        /*! material name
        */
        iaString _name;

    };
    
}

#endif
