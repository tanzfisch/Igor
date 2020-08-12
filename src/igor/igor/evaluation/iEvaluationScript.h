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
// (c) Copyright 2012-2020 by Martin Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __iEVALUATION_SCRIPT__
#define __iEVALUATION_SCRIPT__

#include <igor/evaluation/iEvaluation.h>
#include <igor/scene/nodes/iNodeTransform.h>

#include <iaux/math/iaMatrix.h>
#include <iaux/math/iaTransform.h>
#include <iaux/system/iaDelegate.h>
using namespace iaux;

#include <map>

namespace igor
{

    /*! evaluation script delegate
    */
    iaDELEGATE(iEvaluationScriptDelegate, void, (iNodeTransformPtr transformNode, float64 t), (transformNode, t));

    /*! evaluation that menipulates transform nodes via given callback function
    */
    class IGOR_API iEvaluationScript : public iEvaluation
    {

        friend class iEvaluationManager;

    public:
        /*! sets script to use for evaluation
        */
        void setScript(iEvaluationScriptDelegate evaluationScriptDelegate);

    private:
        /*! evaluation script delegate
        */
        iEvaluationScriptDelegate _evaluationScriptDelegate;

        /*! init members

        \param nodeID id of node to take control of
        */
        iEvaluationScript(uint64 nodeID);

        /*! evaluates something

        \param t the scale form 0 to 1 from start to stop
        */
        virtual void evaluate(float64 t) override;
    };

    /*! evaluation script pointer definition
    */
    typedef iEvaluationScript *iEvaluationScriptPtr;

}; // namespace igor

#endif
