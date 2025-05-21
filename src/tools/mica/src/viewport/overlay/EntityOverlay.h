//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef MICA_ENTITY_OVERLAY_H
#define MICA_ENTITY_OVERLAY_H

#include <igor/igor.h>
using namespace igor;

#include <memory>

/*! overlay modes
 */
enum class OverlayMode
{
    None,
    Translate,
    Scale,
    Rotate
};

/*! 3d overlay ui element for scene nodes

    to modify position, orientation and scale of objects in the scene
    to display additional information when nodes are selected
*/
class EntityOverlay
{

public:
    /*! initialize node overlay

    \param view the view to use
    */
    EntityOverlay(iViewPtr view);

    /*! cleanup
     */
    ~EntityOverlay() = default;

    /*! set entity used in this overlay

    \param entitySceneID the entity scene id
    \param entityID the entity id
     */
    virtual void setEntity(const iEntitySceneID &entitySceneID, const iEntityID &entityID);

    /*! \returns entity id
     */
    const iEntityID &getEntityID() const;

    /*! \returns entity scene id
     */
    const iEntityID &getSceneID() const;

    /*! sets node overlay active

    \param active true to set node overlay active
    */
    virtual void setActive(bool active);

    /*! \returns true id overlay is active
     */
    bool isActive() const;

    /*! sets overlay mode

    \param mode the new overlay mode
    */
    virtual void setOverlayMode(OverlayMode mode);

    /*! \returns true if mode in combination with a given entity can be handled by this node overlay

    \param mode the overlay mod
    \param entity the given entity
    */
    virtual bool accepts(OverlayMode mode, iEntityPtr entity) = 0;

    /*! handles mouse key down event

    \param event the mouse key down event
    \returns true if consumed
    */
    virtual bool onMouseKeyDownEvent(iEventMouseKeyDown &event);

    /*! handles mouse key up event

    \param event the mouse key up event
    \returns true if consumed
    */
    virtual bool onMouseKeyUpEvent(iEventMouseKeyUp &event);

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    virtual bool onMouseMoveEvent(iEventMouseMove &event);

    /*! called when key was pressed

    \param event the event to handle
    */
    virtual bool onKeyDown(iEventKeyDown &event);

    /*! \returns current overlay mode
     */
    OverlayMode getOverlayMode() const;

    /*! \returns the view
     */
    iViewPtr getView() const;

private:
    /*! entity scene this overlay if for
     */
    iEntitySceneID _entitySceneID = iEntitySceneID::getInvalid();

    /*! the entity this overlay is used on
     */
    iEntityID _entityID = iEntityID::getInvalid();

    /*! the overlay mode
     */
    OverlayMode _overlayMode = OverlayMode::None;

    /*! the view
     */
    iViewPtr _view = nullptr;

    /*! if overlay is active or not
     */
    bool _active = false;
};

/*! node overlay pointer definition
 */
typedef std::shared_ptr<EntityOverlay> EntityOverlayPtr;

#endif // MICA_ENTITY_OVERLAY_H
