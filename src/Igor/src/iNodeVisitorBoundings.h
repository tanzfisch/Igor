//
//  ###
//   #
//   #  #### #### ##
//   #  #  # #  # #
//  ### #### #### #
//         #
//       ###
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

#ifndef __iNODEVISITORBOUNDINGS__
#define __iNODEVISITORBOUNDINGS__

#include <iNodeVisitor.h>
#include <iDefines.h>
#include <iSphere.h>

namespace Igor
{

    /*! calculates the combined boundings of a subtree of nodes
    */
    class Igor_API iNodeVisitorBoundings : public iNodeVisitor
	{

	public:

        /*! \returns merged bounding sphere of all children while traversal
        */
        void getSphere(iSphered& sphere);

        /*! initializes node visitor
        */
        iNodeVisitorBoundings();

        /*! does nothing
        */
        virtual ~iNodeVisitorBoundings() = default;

	protected:

        /*! does nothing
        */
        virtual void preTraverse();

        /*! merges bounding spheres from all children while traversal

        \param node current node
        */
        virtual bool preOrderVisit(iNodePtr node);

        /*! does nothing

        \param node current node
        */
        virtual void postOrderVisit(iNodePtr node);

        /*! does nothing
        */
        virtual void postTraverse();
		
	private:

        /*! flag to mark the first bounding sphere found
        */
        bool _initialSphere = true;

        /*! temp and end result of bounding sphere
        */
        iSphered _sphere;

	};

};

#endif
