//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __SUPREMACY__
#define __SUPREMACY__

#include "Components.h"

/*! Supremacy
 */
class Supremacy : public iLayer
{

public:
    Supremacy(iWindow *window);

    ~Supremacy();

private:

    /*! the view we render 2D to
     */
    iView _viewOrtho;

    /*! material id of a non textured material
    */
    uint64 _materialWithTextureAndBlending = iMaterial::INVALID_MATERIAL_ID;

    /*! plain material for rendering primitives
    */
    uint64 _plainMaterial = iMaterial::INVALID_MATERIAL_ID;

    /*! entity scene
     */
    iEntityScene _entityScene;

    /*! handle to player entity
    */
    iEntity _player;

    /*! random numbers
     */
    iaRandomNumberGeneratoru _rand;

    /*! update timer
    */
    iTimerHandlePtr _updateTimerHandle;

    /*! async loading of textures
    */
    iTaskID _taskFlushTextures = iTask::INVALID_TASK_ID;    

    /*! quadtree
    */
    iQuadtree<float64, iEntityID> _quadtree;

    /*! floor shadow
     */
    iTexturePtr _shadow;

    /*! called when added to layer stack
     */
    void onInit() override;

    /*! called when removed from layer stack
     */
    void onDeinit() override;

    /*! called on application pre draw event
     */
    void onPreDraw() override;

    /*! called on any other event
     */
    void onEvent(iEvent &event) override;

    /*! called when render ortho viewport
     */
    void onRenderOrtho();

    /*! game logic intervall
    */
    void onUpdate();

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iEventKeyDown &event);

    /*! called when key was released

    \param event the event to handle
    */
    bool onKeyUp(iEventKeyUp &event);

    //bool onEntityCreated(iEventEntityCreated &event);
    //bool onEntityDestroyed(iEventEntityDestroyed &event);
};

#endif // __SUPREMACY__
