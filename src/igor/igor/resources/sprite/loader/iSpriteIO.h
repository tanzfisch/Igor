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
// (c) Copyright 2012-2026 by Martin A. Loga
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

#ifndef IGOR_SPRITE_IO_H
#define IGOR_SPRITE_IO_H

#include <igor/resources/sprite/iSprite.h>

namespace igor
{
    /*! import/export for Igor sprite files
     */
    class IGOR_API iSpriteIO
    {
    public:
        /*! reads sprite file

        \param filename path to sprite file
        \param sprite the destination sprite 
        */
        static bool read(const iaString &filename, iSpritePtr sprite);

        /*! writes sprite to file

        \param filename path to new prefab file
        \param sprite the sprite to write
        */
        static bool write(const iaString &filename, iSpritePtr sprite);    
    };

} // namespace igor

#endif // IGOR_SPRITE_IO_H