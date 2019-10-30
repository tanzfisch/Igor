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

#ifndef __iEVALUATOR_TRANSFORM_LINEAR__
#define __iEVALUATOR_TRANSFORM_LINEAR__

#include <iEvaluatorNode.h>

#include <iaMatrix.h>
using namespace IgorAux;

#include <map>

namespace Igor
{

    /*! evaluation that menipulates transform nodes in a linear fashion
    */
	class Igor_API iEvaluatorTransformLinear : public iEvaluatorNode
	{

    public:

        /*! evaluates something
        */
        virtual void evaluate() override;

        /*! adds a node to this evaluator

        \param node the node to add
        */
        virtual void addNode(const iNodePtr node) override;

        /*! removes node frmo evaluator

        \param node the node to remove
        */
        virtual void removeNode(const iNodePtr node) override;

        /*!
        */
        void setTransform(const iNodePtr node, float64 startTime, float64 stopTime, const iaMatrixd& matrix);

    private:

        struct Transform
        {
            /*! destination transform matrix
            */
            iaMatrixd _matrix;

            /*! start time in seconds
            */
            float64 _startTime;

            /*! stop time in seconds
            */
            float64 _stopTime;
        };

        /*! registered node IDs
        */
        std::map<uint64, Transform> _transforms;

	};

};

#endif
