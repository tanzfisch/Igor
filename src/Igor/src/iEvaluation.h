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

#include <iDefines.h>

#include <iaIDGenerator.h>
using namespace IgorAux;

#include <vector>

namespace Igor
{

    class iNode; typedef iNode* iNodePtr;

    /*! evaluation base class
    */
    class Igor_API iEvaluation
    {

    public:

        /*! invalid node id definition
        */
        static const uint64 INVALID_EVALUATOR_ID = IGOR_INVALID_ID;

        /*! init members
        */
        iEvaluation();

        /*! does nothing
        */
        virtual ~iEvaluation() = default;

        /*! \returns id of the evaluation
        */
        uint64 getID() const;

        /*! evaluates something
        */
        virtual void evaluate() = 0;

        /*! \returns true if evaluation is finished
        */
        //virtual bool finalized() const = 0;

        /*! adds a node to this evaluation

        \param node the node to add
        */
        virtual void addNode(const iNodePtr node);

        /*! removes node frmo evaluation

        \param node the node to remove
        */
        virtual void removeNode(const iNodePtr node);

        /*! sets if this evaluation runs in an endless loop
        */
        void setLooped(bool loop);

        /*! \returns true if evaluation is in loop mode
        */
        bool isLooped() const;

    protected:

        std::vector<uint64> _nodes;

    private:

        /*! the next node id
        */
        static iaIDGenerator64 _idGenerator;



        /*! id of this evaluation
        */
        uint64 _evaluatorID = iEvaluation::INVALID_EVALUATOR_ID;

        /*! looped flag
        */
        bool _looped = false;

    };

    /*! evaluation pointer definition
    */
    typedef iEvaluation* iEvaluationPtr;

}

#endif
