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

#ifndef __iEVALUATION__
#define __iEVALUATION__

#include <iNodeTransform.h>

#include <iaIDGenerator.h>
using namespace IgorAux;

#include <vector>

namespace Igor
{

    /*! evaluation base class
    */
    class Igor_API iEvaluation
    {

        friend class iEvaluationManager;

    public:

        /*! invalid node id definition
        */
        static const uint64 INVALID_EVALUATOR_ID = IGOR_INVALID_ID;

        /*! sets start time of evaluation

        \param start the start time for evaluation
        */
        void setStart(float64 start);

        /*! \returns start time of evaluation
        */
        float64 getStart() const;

        /*! sets stop time of evaluation

        \param stop the stop time for evaluation
        */
        void setStop(float64 stop);

        /*! \returns stop time of evaluation
        */
        float64 getStop() const;

        /*! \returns id of the evaluation
        */
        uint64 getID() const;

        /*! evaluates something

        \param time current application time in seconds
        */
        virtual void evaluate(float64 time) = 0;

        /*! sets if this evaluation runs in an endless loop
        */
        void setLooped(bool loop);

        /*! \returns true if evaluation is in loop mode
        */
        bool isLooped() const;

    protected:

        /*! id of node to control
        */
        uint64 _nodeID = iNode::INVALID_NODE_ID;

        /*! start time of evaluation
        */
        float64 _start = 0;

        /*! stop time of evaluation
        */
        float64 _stop = 0;

        /*! init members

        \param nodeID id of node to take control of
        */
        iEvaluation(uint64 nodeID);

        /*! does nothing
        */
        virtual ~iEvaluation() = default;

    private:

        /*! the next node id
        */
        static iaIDGenerator64 _idGenerator;

        /*! looped flag
        */
        bool _looped = false;

        /*! id of this evaluation
        */
        uint64 _evaluatorID = iEvaluation::INVALID_EVALUATOR_ID;

    };

    /*! evaluation pointer definition
    */
    typedef iEvaluation* iEvaluationPtr;

}

#endif
