
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

#ifndef IGOR_CLIPBOARD_H
#define IGOR_CLIPBOARD_H

#include <igor/resources/module/iModule.h>

#include <igor/entities/iEntityComponent.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

    enum class iClipboardDataFormat
    {
        Text,
        EntityID
    };

    class IGOR_API iClipboard : public iModule<iClipboard>
    {
        friend class iModule<iClipboard>;

    public:
        /*! copy text to clipboard

        \param text the text to copy
        */
        void copyText(const iaString &text);

        /*! \returns pasted text from clipboard
         */
        const iaString pasteText() const;

        /*! copy entity ids to clipboard

        \param entityIDs the entity IDs to copy
        */
       void copyEntityIDs(const std::vector<iEntityID> &entityIDs);

       /*! \returns pasted entity ids from clipboard
        */
       const std::vector<iEntityID> pasteEntityIDs() const;        

        /*! \returns true if clipboard contains data of given format

        \param format the given data format
        */
        bool has(iClipboardDataFormat format) const;

    private:
        /*! init formats
         */
        iClipboard();

        /*! does nothing
         */
        virtual ~iClipboard() = default;
    };

} // namespace igor

#endif // IGOR_CLIPBOARD_H