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

#ifndef __iMODELNODE__
#define __iMODELNODE__

#include <iNodeModelQueue.h>
#include <iNode.h>

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

	class iModelData;
    class iModel;
    class iModelDataInputParameter;

    /*! represents a model within the scene
    */
	class Igor_API iNodeModel : public iNode
	{

        friend class iNodeModelQueue;
		friend class iNodeFactory;
        friend class iNodeVisitorUpdateTransform;

    public:

        /*! sets model by filename and or generator parameters

        \todo more docu here!

        \param modelName can be a filename or any unique identity name
        \param parameters optional loading parameters

        \todo instancing is for now limited to 20k objects. we need this variable

        */
        void setModel(const iaString& modelName, iModelDataInputParameter* parameters = nullptr);

        /*! \returns filename of model
        */
        iaString getModelName() const;

        /*! handles model nodes that are not loaded yet

        \todo this does not fit in here. the model queue should be handeled elsewhere
        */
        static void processModelNodes();

	private:

        /*! model node queue

        holds models that are meant to be loaded asynchronously
        */
        static iNodeModelQueue modelNodeQueue;

        /*! filename of model
        */
		iaString _filename;

        /*! loading parameters

        can't delete them they belong (usually) to application
        */
        iModelDataInputParameter* _parameters = nullptr;

        /*! shared poitner to requested model
        */
		shared_ptr<iModel> _model = nullptr;

        /*! true if requested model was actually loaded and subtree created
        */
		bool _initialized = false;

        /*! checks if model data was loaded and creates corresponding subtree

        \returns true if model data was present
        */
		bool updateModelData();

        /*! called by update transform run

        ignores the matrix

        \param matrix world matrix for the camera
        */
        void onUpdateTransform(iaMatrixf& matrix);

        /*! initializes memeber variables
        */
		iNodeModel();

        /*! copy ctor
        */
        iNodeModel(iNodeModel* node);

        /*! release model data
        */
		virtual ~iNodeModel();
        
	};

}

#endif
