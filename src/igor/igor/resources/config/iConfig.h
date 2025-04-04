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
// (c) Copyright 2012-2025 by Martin Loga
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

#ifndef IGOR_CONFIG_H
#define IGOR_CONFIG_H

#include <igor/resources/module/iModule.h>

#include <iaux/data/iaString.h>
using namespace iaux;

#include <unordered_map>

class TiXmlElement;

namespace igor
{

	/*! reader for general Igor configuration file.

	usually located at config/Igor.xml
	*/
	class iConfig : public iModule<iConfig>
	{

		friend class iModule<iConfig>;

	public:

		/*! opens and reads configuration file from filesystem.

		using tinyXML API

		\param filename path to configuration file
		*/
		void read(const iaString &filename);

		/*! writes current configuration to given file

		\param filename the given filename
		\returns true if successful
		*/
		bool write(const iaString &filename);

		/*! \returns value for given setting as string
		
		\param key the given setting
		*/
		const iaString getValue(const iaString &setting) const;

		/*! \returns value for given setting as int

		\param key the given setting
		*/
		int64 getValueAsInt(const iaString &setting) const;

		/*! \returns value for given setting as float

		\param key the given setting
		*/
		float64 getValueAsFloat(const iaString &setting) const;

		/*! \returns value for given setting array of string

		\param key the given setting
		*/
		const std::vector<iaString> getValueAsArray(const iaString &setting) const;

		/*! \returns true if given setting is set

		\param setting the given setting
		*/
		bool hasSetting(const iaString &setting) const;

	private:

		/*! the key value data
		*/
		std::unordered_map<iaString, std::vector<iaString>> _settings;

		/*! read configuration

		\param config config xml element
		*/
		void readConfigElement(TiXmlElement *config);

		/*! sets value on setting and overrides all values that been there before

		\param setting the setting to set
		\param value the value to set
		*/
		void set(const iaString &setting, const iaString &value);

		/*! sets multiple values on setting and overrides all values that been there before

		\param setting the setting to set
		\param values the values to set
		*/
		void set(const iaString &setting, const std::vector<iaString> &values);

		/*! removes given setting

		\param setting the setting to remove
		*/
		void reset(const iaString &setting);

		/*! does nothing
		 */
		iConfig();

		/*! does nothing
		 */
		virtual ~iConfig();		
	};

} // namespace igor

#endif // IGOR_CONFIG_H