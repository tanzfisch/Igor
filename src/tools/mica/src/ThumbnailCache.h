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

#ifndef __THUMBNAIL_CACHE__
#define __THUMBNAIL_CACHE__

#include "TaskGenerateThumbnails.h"

/*! the thumbnail cache
 */
class ThumbnailCache
{

    friend class TaskGenerateThumbnails;

public:

    /*! \returns singleton instance of thumbnail cache
    */
    static ThumbnailCache &getInstance();

    /*! \returns thumbnail for given filename

    \param filename full path of existing filename
    */
    iTexturePtr getThumbnail(const iaString &filename);

private:
    /*! path to thumbnail cache
     */
    iaString _thumbnailCachePath;

    /*! queue to process thumbnails
    */
    std::deque<std::pair<iaString, iaString>> _thumbnailProcessQueue;

    /*! mutex for processing queue
    */
    iaMutex _queueMutex;

    /*! generates thumbnails
    */
    void generateThumbnails();

    /*! init cache
     */
    ThumbnailCache();
};

#endif // __THUMBNAIL_CACHE__
