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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef MICA_USERCONTROL_TEXTURE_H
#define MICA_USERCONTROL_TEXTURE_H

#include "UserControlResource.h"

class UserControlTexture : public UserControlResource
{
public:
    /*! init user control

    \param resourceID the resource id to use
    \param parent the optional parent widget
    */
	UserControlTexture(iResourceID resourceID, const iWidgetPtr parent = nullptr);

	/*! cleanup
	*/
	~UserControlTexture();

    /*! init ui
     */
    virtual void init();

    /*! update ui with node data
     */
    virtual void update();

    /*! update node with ui data
     */
    virtual void updateResource();

private:

	/*! display the texture
	*/
	iWidgetPicturePtr _picture;

	/*! label with texture information
	*/
	iWidgetLabelPtr _infoLabel;
	
};

#endif // MICA_USERCONTROL_TEXTURE_H