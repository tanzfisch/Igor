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

#ifndef __iEVALUATION_MANAGER__
#define __iEVALUATION_MANAGER__

#include <iEvaluation.h>

#include <iaSingleton.h>
using namespace IgorAux;

#include <unordered_map>

namespace Igor
{

    /*! evaluation manager
    */
	class Igor_API iEvaluationManager : public iaSingleton<iEvaluationManager>
	{

        friend class iaSingleton<iEvaluationManager>;
        friend class iApplication;
        friend class iEvaluation;

    public:

        /*! creates an evaluation

        evaluations are owned by the manager but will stick arround until destroyed by application

        \param nodeID id of node this evaluation controls
        \returns pointer to new node
        */
        template<class T>
        T* createEvaluation(uint64 nodeID);

        /*! destroyes evaluation by id

        \param id given id
        */
        void destroyEvaluation(uint64 id);

        /*! \returns true if evaluation ID exists

        \param id the evaluation ID
        */
        bool isEvaluation(uint64 id) const;

        /*! \returns evaluation for given evaluatin id

        returns nullptr for invalid id

        \param id the given evaluaiton id
        */
        iEvaluationPtr getEvaluation(uint64 id) const;

    private:

        /*! map of all evaluations
        */
        std::unordered_map<uint64, iEvaluationPtr> _evaluations;

        /*! list with active evaluations
        */
        std::vector<uint64> _activeEvaluations;

        /*! list with dirty evaluations
        */
        std::vector<uint64> _dirtyEvaluations;

        /*! does nothing
        */
        iEvaluationManager() = default;

        /*! clean up
        */
        ~iEvaluationManager();

        /*! called by application every frame. calls all the evaluator handles
        */
        void handle();

        /*! sets given evaluation dirty by id

        \param id of given evaluation to set dirty
        */
        void setEvaluationDirty(uint64 id);

	};

#include <iEvaluationManager.inl>

};

#endif
