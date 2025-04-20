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

#ifndef IGOR_EVENT_FILESYSTEM_H
#define IGOR_EVENT_FILESYSTEM_H

#include <igor/events/iEvent.h>

#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNode.h>

namespace igor
{

    /*! file was crated event
     */
    class IGOR_API iEventFileCreated : public iEvent
    {
    public:
        /*! init members

        \param filename filename of the crated file
        */
        iEventFileCreated(const iaString &filename);

        /*! \returns information to event
         */
        virtual const iaString getInfo() const override;

        /*! \returns the filename
         */
        const iaString& getFilename() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Filesystem)
        IGOR_EVENT_CLASS_TYPE(iEventFileCreated)

    private:
        /*! filename of the created file
         */
        iaString _filename;

    };

    /*! file was deleted event
     */
    class IGOR_API iEventFileDeleted : public iEvent
    {
    public:
        /*! init members

        \param filename filename of the crated file
        */
        iEventFileDeleted(const iaString &filename);

        /*! \returns information to event
         */
        virtual const iaString getInfo() const override;

        /*! \returns the filename
         */
        const iaString& getFilename() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Filesystem)
        IGOR_EVENT_CLASS_TYPE(iEventFileDeleted)

    private:
        /*! filename of the created file
         */
        iaString _filename;

    };

    /*! file was moved from event
     */
    class IGOR_API iEventFileMovedFrom : public iEvent
    {
    public:
        /*! init members

        \param filename filename of the crated file
        */
       iEventFileMovedFrom(const iaString &filename);

        /*! \returns information to event
         */
        virtual const iaString getInfo() const override;

        /*! \returns the filename
         */
        const iaString& getFilename() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Filesystem)
        IGOR_EVENT_CLASS_TYPE(iEventFileMovedFrom)

    private:
        /*! filename of the created file
         */
        iaString _filename;

    };        

    /*! file was moved to event
     */
    class IGOR_API iEventFileMovedTo : public iEvent
    {
    public:
        /*! init members

        \param filename filename of the crated file
        */
       iEventFileMovedTo(const iaString &filename);

        /*! \returns information to event
         */
        virtual const iaString getInfo() const override;

        /*! \returns the filename
         */
        const iaString& getFilename() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Filesystem)
        IGOR_EVENT_CLASS_TYPE(iEventFileMovedTo)

    private:
        /*! filename of the created file
         */
        iaString _filename;

    };  

    /*! file was changed event
     */
    class IGOR_API iEventFileChanged : public iEvent
    {
    public:
        /*! init members

        \param filename filename of the crated file
        */
       iEventFileChanged(const iaString &filename);

        /*! \returns information to event
         */
        virtual const iaString getInfo() const override;

        /*! \returns the filename
         */
        const iaString& getFilename() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Filesystem)
        IGOR_EVENT_CLASS_TYPE(iEventFileChanged)

    private:
        /*! filename of the created file
         */
        iaString _filename;

    };         

}; // namespace igor

#endif // IGOR_EVENT_FILESYSTEM_H