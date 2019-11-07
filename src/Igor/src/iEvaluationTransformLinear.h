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

#ifndef __iEVALUATION_TRANSFORM_LINEAR__
#define __iEVALUATION_TRANSFORM_LINEAR__

#include <iEvaluation.h>

#include <iaMatrix.h>
#include <iaTransform.h>
using namespace IgorAux;

#include <map>

namespace Igor
{

    /*! evaluation that menipulates transform nodes in a linear fashion
    */
	class Igor_API iEvaluationTransformLinear : public iEvaluation
	{

        friend class iEvaluationManager;

    public:

        /*! evaluates something

        \param time current application time in seconds
        */
        virtual void evaluate(float64 time) override;

        /*! set up the evaluation to reach matrix in given time

        the start time will be automatically set to current frame time

        \param matrix the target matrix to reach
        \param duration the time to reach the target in seconds from now
        */
        void setTarget(const iaMatrixd& matrix, float64 duration);

        /*! same as setTarget but including start time

        \param matrix the target matrix to reach
        \param startTime the time the evaluation begins
        \param duration the time to reach the target in seconds
        */
        void setTarget(const iaMatrixd& matrix, float64 startTime, float64 duration);

    protected:

        /*! init members

        \param nodeID id of node to take control of
        */
        iEvaluationTransformLinear(uint64 nodeID);

    private:

        iaTransformd _targetTransform;

	};

    typedef iEvaluationTransformLinear* iEvaluationTransformLinearPtr;

};

#endif
