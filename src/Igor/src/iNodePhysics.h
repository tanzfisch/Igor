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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iNODEPHYSICS__
#define __iNODEPHYSICS__

#include <iNodeVolume.h>
#include <iaMatrix.h>
#include <iPhysicsBody.h>

#include <memory>
using namespace std;

namespace Igor
{
    
    class iNodePhysics : public iNodeVolume
	{
		
		friend class iNodeVisitorUpdateTransform;
        friend class iNodeFactory;
        friend class iPhysicsManager;
        friend class iPhysicsManagerTask;

    public:

        uint64 getBody() const;
        
	protected:

        /*! set world matrix

        \param matrix matrix to set
        */
        virtual void onUpdateTransform(iaMatrixf& matrix);

        /*! sets physic body id

        \param bodyID the body id
        */
        void setBody(uint64 bodyID);

        /*! does nothing
        */
        virtual void draw() = 0;

        /*! called to init physics
        */
        virtual void initPhysics() = 0;

        /*! \returns true if physics was already initialized
        */
        bool isInitialized();

        /*! initializes member variables
        */
        iNodePhysics();

        /*! copy ctor
        */
        iNodePhysics(iNodePhysics* node);

        /*! does nothing
        */
        virtual ~iNodePhysics();

    private:

        bool _physicsInitialized = false;

        /*! physics body
        */
        uint64 _bodyID = iPhysicsBody::INVALID_BODY_ID;

        /*! updates physics
        */
        void updatePhysics();

        /*! shutdown physics
        */
        void shutdownPhysics();
		
	};

};

#endif
