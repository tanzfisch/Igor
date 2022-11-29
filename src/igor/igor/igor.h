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
// (c) Copyright 2012-2022 by Martin Loga
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

// https://wiki.lspace.org/Igor ;-)
#ifndef __IGOR_H__
#define __IGOR_H__

// igor includes
#include <igor/audio/iAudio.h>

#include <igor/data/iAABox.h>
#include <igor/data/iAACube.h>
#include <igor/data/iBone.h>
#include <igor/data/iFrustum.h>
#include <igor/data/iIntersection.h>
#include <igor/data/iJoint.h>
#include <igor/data/iPlane.h>
#include <igor/data/iRay.h>
#include <igor/data/iSkeleton.h>
#include <igor/data/iQuadtree.h>

#include <igor/evaluation/iEvaluationManager.h>
#include <igor/evaluation/iEvaluationScript.h>
#include <igor/evaluation/iEvaluationTransform.h>

#include <igor/generation/iContouringCubes.h>
#include <igor/generation/iLSystem.h>
#include <igor/generation/iPerlinNoise.h>

#include <igor/renderer/iRenderer2.h>
#include <igor/renderer/iView.h>

#include <igor/layers/iLayer.h>
#include <igor/layers/iLayerWidgets.h>
#include <igor/layers/iLayerProfiler.h>

#include <igor/physics/iPhysics.h>
#include <igor/physics/iPhysicsMaterialCombo.h>

#include <igor/resources/iResourceManager.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/material/iTargetMaterial.h>
#include <igor/resources/mesh/iMeshBuilder.h>
#include <igor/resources/mesh/iMeshBuilderUtils.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/resources/texture/iAtlas.h>
#include <igor/resources/texture/iPixmap.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/texture/iTextureResourceFactory.h>

#include <igor/scene/iScene.h>
#include <igor/scene/iSceneFactory.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeAudioListener.h>
#include <igor/scene/nodes/iNodeAudioSource.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeEmitter.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>
#include <igor/scene/nodes/iNodeLODTrigger.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeParticleSystem.h>
#include <igor/scene/nodes/iNodePhysics.h>
#include <igor/scene/nodes/iNodeSkyBox.h>
#include <igor/scene/nodes/iNodeSwitch.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeWater.h>
#include <igor/scene/traversal/iNodeVisitorBoundings.h>
#include <igor/scene/traversal/iNodeVisitorPrintTree.h>
#include <igor/scene/traversal/iNodeVisitorRenderColorID.h>

#include <igor/simulation/iParticleSystem2D.h>

#include <igor/system/iApplication.h>
#include <igor/system/iKeyboard.h>
#include <igor/system/iMouse.h>
#include <igor/system/iTimer.h>
#include <igor/system/iTimerHandle.h>
#include <igor/system/iWindow.h>

#include <igor/events/iEventECS.h>
#include <igor/events/iEventKeyboard.h>
#include <igor/events/iEventMouse.h>
#include <igor/events/iEventWindow.h>
#include <igor/events/iEventScene.h>

#include <igor/entities/iEntityManager.h>
#include <igor/entities/iEntity.h>

#include <igor/terrain/iVoxelTerrain.h>
#include <igor/terrain/iVoxelTerrainMeshGenerator.h>
#include <igor/terrain/data/iVoxelBlock.h>
#include <igor/terrain/data/iVoxelData.h>
#include <igor/terrain/tasks/iTaskGenerateVoxels.h>
#include <igor/terrain/tasks/iTaskPropsOnVoxels.h>

#include <igor/threading/iTaskManager.h>
#include <igor/threading/tasks/iTask.h>
#include <igor/threading/tasks/iTaskFlushModels.h>
#include <igor/threading/tasks/iTaskFlushTextures.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/actions/iAction.h>
#include <igor/ui/actions/iActionManager.h>
#include <igor/ui/dialogs/iDialogColorChooser.h>
#include <igor/ui/dialogs/iDialogColorGradient.h>
#include <igor/ui/dialogs/iDialogDecisionBox.h>
#include <igor/ui/dialogs/iDialogFileSelect.h>
#include <igor/ui/dialogs/iDialogGraph.h>
#include <igor/ui/dialogs/iDialogIndexMenu.h>
#include <igor/ui/dialogs/iDialogMessageBox.h>
#include <igor/ui/dialogs/iDialogMenu.h>
#include <igor/ui/theme/iWidgetDefaultTheme.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetCheckBox.h>
#include <igor/ui/widgets/iWidgetColor.h>
#include <igor/ui/widgets/iWidgetColorGradient.h>
#include <igor/ui/widgets/iWidgetGraph.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/widgets/iWidgetGroupBox.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetMenu.h>
#include <igor/ui/widgets/iWidgetMenuBar.h>
#include <igor/ui/widgets/iWidgetNumberChooser.h>
#include <igor/ui/widgets/iWidgetPicture.h>
#include <igor/ui/widgets/iWidgetScroll.h>
#include <igor/ui/widgets/iWidgetSelectBox.h>
#include <igor/ui/widgets/iWidgetSlider.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetTextEdit.h>
#include <igor/ui/user_controls/iUserControlAction.h>
#include <igor/ui/user_controls/iUserControlColorChooser.h>
#include <igor/ui/user_controls/iUserControlFileChooser.h>

// version
#include <igor/iVersion.h>

// iaux includes
#include <iaux/data/iaColor3.h>
#include <iaux/data/iaColor4.h>
#include <iaux/data/iaConvert.h>
#include <iaux/data/iaGradient.h>
#include <iaux/data/iaString.h>
#include <iaux/data/iaSphere.h>
#include <iaux/data/iaRectangle.h>

#include <iaux/math/iaBSpline.h>
#include <iaux/math/iaMatrix.h>
#include <iaux/math/iaRandomNumberGenerator.h>
#include <iaux/math/iaVector2.h>
#include <iaux/math/iaVector3.h>

#include <iaux/system/iaFile.h>
#include <iaux/system/iaConsole.h>

/*! Igor namespace that contains all high level functionality of Igor
*/
namespace igor
{
    /*! should be called before any other interface of Igor is used	

	\param argc command line parameter count
	\param argv command line parameters
	*/
    void IGOR_API startupArgs(int argc, wchar_t **argv);

    /*! alternative startup
	*/
    void IGOR_API startup();

    /*! should be called last in your application. do not use any Igor interface after that
	
	Shuts down all the modules.
	Not thread save. make shure only one thread is left when calling this.

    \todo it would be nice to have the order of shutting down singletons implicitly or configurable
	*/
    void IGOR_API shutdown();

} // namespace igor

#endif // __IGOR_H__
