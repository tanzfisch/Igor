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

#ifndef IGOR_THUMBNAIL_CACHE_H
#define IGOR_THUMBNAIL_CACHE_H

#include <igor/threading/tasks/iTaskGenerateThumbnails.h>
#include <igor/resources/texture/iTexture.h>

#include <iaux/data/iaString.h>
using namespace iaux;

#include <deque>

namespace igor
{

    /*! the thumbnail cache (singleton)    
     */
    class IGOR_API iThumbnailCache
    {

        friend class iTaskGenerateThumbnails;

    public:
        /*! \returns singleton instance of thumbnail cache
         */
        static iThumbnailCache &getInstance();

        /*! \returns thumbnail for given filename

        \param filename full path of existing filename
        */
        iTexturePtr getThumbnail(const iaString &filename);

        /*! \returns thumbnail for given resource id

        \param resourceID the resource id
        */
        iTexturePtr getThumbnail(const iResourceID &resourceID);

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
        iThumbnailCache();
    };

}

#endif // IGOR_THUMBNAIL_CACHE_H
