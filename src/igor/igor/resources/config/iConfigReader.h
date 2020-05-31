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
// contact: martinloga@gmx.de

#ifndef __iCONFIGREADER__
#define __iCONFIGREADER__

#include <iaux/data/iaString.h>
using namespace IgorAux;

class TiXmlElement;

namespace Igor
{

	/*! reader for generall Igor configuration file.

	usually located at config/Igor.xml
	*/
	class iConfigReader
	{

	public:
		/*! does nothing
		*/
		iConfigReader();

		/*! does nothing
		*/
		virtual ~iConfigReader();

		/*! opens and reads configuration file from filesystem.

		using tinyXML API

		\param filename path to configuration file
		*/
		void readConfiguration(const iaString &filename);

	private:
		/*! analyses the xml elements

		\param resourceManager root element
		*/
		void readResourceManagerConfig(TiXmlElement *resourceManager);

		/*! read logging xml elements

		\param logging root element
		*/
		void readLoggingConfig(TiXmlElement *logging);
	};

} // namespace Igor

#endif