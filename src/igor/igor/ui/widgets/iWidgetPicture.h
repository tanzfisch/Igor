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

#ifndef __IGOR_WIDGETPICTURE_H__
#define __IGOR_WIDGETPICTURE_H__

#include <igor/ui/widgets/iWidget.h>
#include <igor/resources/texture/iTexture.h>

#include <iaux/data/iaString.h>
using namespace iaux;

#include <memory>
#include <limits>

namespace igor
{

	/*! picture widget
	*/
	class Igor_API iWidgetPicture : public iWidget
	{

	public:
		/*! ctor initializes member variables

		\param parent optional parnt
		*/
		iWidgetPicture(const iWidgetPtr parent = nullptr);

		/*! release texture
		*/
		virtual ~iWidgetPicture();

		/*! sets the texture to use for the picture

		\param texturePath path to texture
		*/
		void setTexture(const iaString &texturePath);

		/*! \returns texture path
		*/
		const iaString &getTexture() const;

		/*! \returns true if picture has a valid and loaded texture
        */
		bool hasTexture() const;

		/*! sets maximum display size of picture

		\param width max width
		\param height max height
		*/
		void setMaxSize(int32 width, int32 height);

		/*! \returns maximum width
		*/
		int32 getMaxWidth();

		/*! \returns maximum height
		*/
		int32 getMaxHeight();

		/*! sets if the aspect ratio of the picture must be kept

		\paran keep if true the aspect ratio must be kept
		*/
		void setKeepAspectRatio(bool keep = true);

		/*! \returns true if aspect ratio will be kept
		*/
		bool getKeepAspectRatio() const;

	private:
		/*! texture path
		*/
		iaString _texturePath;

		/*! flag if the aspect ratio must be kept when resized
		*/
		bool _keepAspectRatio = true;

		/*! shared pointer to texture
		*/
		iTexturePtr _texture;

		/*! maximum dispaly width
		*/
		int32 _maxWidth = std::numeric_limits<int32>::max();

		/*! maximum dispaly height
		*/
		int32 _maxHeight = std::numeric_limits<int32>::max();

		/*! updates size based on it's content
		*/
		void calcMinSize() override;

		/*! draws the widget
		*/
		void draw();
	};
} // namespace igor

#endif // __IGOR_WIDGETPICTURE_H__
