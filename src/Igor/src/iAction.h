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

#ifndef __iACTION__
#define __iACTION__

#include <iDefines.h>

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

	/*! action base class
	*/
	class Igor_API iAction
	{

		friend class iActionManager;

	public:

		/*! does nothing
		*/
		iAction() = default;

		/*! does nothing
		*/
		virtual ~iAction() = default;

		/*! executed when action gets triggered
		*/
		virtual void execute() = 0;

		/*! \returns the action identifier
		*/
		const iaString& getIdentifier() const;

		/*! sets text of action

		\param text the new text
		*/
		void setText(const iaString& text);

		/*! \returns the action text
		*/
		const iaString& getText() const;

		/*! sets path to a picture for the action

		\param filename the new text
		*/
		void setPicturePath(const iaString& filename);

		/*! \returns the action picture file path
		*/
		const iaString& getPicturePath() const;

	private:

		/*! text of the action
		*/
		iaString _text;

		/*! path to picture of action
		*/
		iaString _picture;

		/*! unique name of the action
		*/
		iaString _name;

		/*! sets action name (aka identifier)

		the action's name must be gloably unique

		\param name the name of the action
		*/
		void setIdentifier(const iaString& name);

	};

}

#endif