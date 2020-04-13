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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __iEVALUATION_TRANSFORM__
#define __iEVALUATION_TRANSFORM__

#include <iEvaluation.h>

#include <iaMatrix.h>
#include <iaTransform.h>
using namespace IgorAux;

#include <map>

namespace Igor
{

    /*! evaluation that menipulates transform nodes in a linear fashion
    */
	class Igor_API iEvaluationTransform : public iEvaluation
	{

        friend class iEvaluationManager;

    public:

        /*! adds a matrix as keyframe

        If using Elastic or Back more then two key frames will 

        \param matrix the matrix to add
        */
        void addKeyframe(const iaMatrixd& matrix);

        /*! resets to original settings/data
        */
        virtual void reset() override;

    private:

        /*! the transform keyframes
        */
        std::vector<iaTransformd> _keyframes;

        /*! init members

        \param nodeID id of node to take control of
        */
        iEvaluationTransform(uint64 nodeID);

        /*! evaluates something

        \param t the scale form 0 to 1 from start to stop
        */
        virtual void evaluate(float64 t) override;

	};

    /*! evaluation transform pointer definition
    */
    typedef iEvaluationTransform* iEvaluationTransformPtr;

};

#endif
