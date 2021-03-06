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
// (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __MICA_H__
#define __MICA_H__

#include "Workspace.h"

class Mica
{

public:
	/*! init mica
	*/
	Mica();

	/*! deinit mica
	*/
	virtual ~Mica();

	/*! run mica

	\param filename name of file to start with
	*/
	void run(const iaString &filename);

private:
	/*! main window of mica
	*/
	iWindowPtr _window = nullptr;

	/*! the worspace we are working with
	*/
	WorkspacePtr _workspace;

	/*! id of textures flush task
	*/
	iTaskID _taskFlushTextures = iTask::INVALID_TASK_ID;

	/*! id of models flush task
	*/
	iTaskID _taskFlushModels = iTask::INVALID_TASK_ID;
};

#endif // __MICA_H__
