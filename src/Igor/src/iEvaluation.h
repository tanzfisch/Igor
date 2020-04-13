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
#include <iaEasing.h>
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

        /*! sets stop based on current start plus duration

        \param duration the duration to be set
        */
        void setDuration(float64 duration);

        /*! \retuens duration
        */
        float64 getDuration() const;

        /*! sets the interpolation function

        only has an effect on deriving classes that respect the easing function like iEvaluationTransform

        \param interpolationFunction the interpolation function type to set
        */
        void setEasingFunction(Easing::EasingFunction interpolationFunction);

        /*! \retruns interpolation function type
        */
        Easing::EasingFunction getEasingFunction() const;

        /*! sets the amplitude of the easing function

        only has an effect with Elastic ease function

        \param amplitude the new amplitude
        */
        void setAmplitude(float64 amplitude);

        /*! \returns the amplitude of the easing function
        */
        float64 getAmplitude() const;

        /*! sets the period of the easing function

        only has an effect with Elastic ease function

        \param period the new period
        */
        void setPeriod(float64 period);

        /*! \returns the period of the easing function
        */
        float64 getPeriod() const;

        /*! sets the overshoot of the easing function

        only has an effect with Back ease function

        \param overshoot the new overshoot value
        */
        void setOvershoot(float64 overshoot);

        /*! \returns the overshoot of the easing function
        */
        float64 getOvershoot() const;

        /*! \returns id of the evaluation
        */
        uint64 getID() const;

        /*! sets if this evaluation runs in an endless loop
        */
        void setLooped(bool loop);

        /*! \returns true if evaluation is in loop mode
        */
        bool isLooped() const;

        /*! resets to original settings/data
        */
        virtual void reset();

    protected:

        /*! id of node to control
        */
        uint64 _nodeID = iNode::INVALID_NODE_ID;

        /*! interpolation function
        */
        Easing::EasingFunction _easingFunction;

        /*! start time of evaluation
        */
        float64 _start;

        /*! stop time of evaluation
        */
        float64 _stop;

        /*! amplitude of elastic easing function
        */
        float64 _amplitude;

        /*! period of elastic easing function
        */
        float64 _period;

        /*! overshoot of back easing function
        */
        float64 _overshoot;

        /*! looped flag
        */
        bool _looped;

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

        /*! id of this evaluation
        */
        uint64 _evaluatorID = iEvaluation::INVALID_EVALUATOR_ID;

        /*! evaluates something

        \param t the scale form 0 to 1 from start to stop
        */
        virtual void evaluate(float64 t) = 0;

    };

    /*! evaluation pointer definition
    */
    typedef iEvaluation* iEvaluationPtr;

}

#endif
