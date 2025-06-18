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

#ifndef IGOR_EVENT_PROJECT_H
#define IGOR_EVENT_PROJECT_H

#include <igor/events/iEvent.h>

namespace igor
{

    /*! project was loaded event
     */
    class IGOR_API iEventProjectLoaded : public iEvent
    {
    public:
        /*! init members

        \param filename filename of loaded project
        */
        iEventProjectLoaded(const iaString &filename);

        /*! \returns information to event
         */
        virtual const iaString getInfo() const override;

        /*! \returns the filename
         */
        const iaString &getFilename() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Project)
        IGOR_EVENT_CLASS_TYPE(iEventProjectLoaded)

    private:
        /*! filename of the loaded project
         */
        iaString _filename;
    };

    /*! project was unloaded event
     */
    class IGOR_API iEventProjectUnloaded : public iEvent
    {
    public:
        /*! init members
         */
        iEventProjectUnloaded();

        /*! \returns information to event
         */
        virtual const iaString getInfo() const override;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Project)
        IGOR_EVENT_CLASS_TYPE(iEventProjectUnloaded)
    };

}; // namespace igor

#endif // IGOR_EVENT_PROJECT_H