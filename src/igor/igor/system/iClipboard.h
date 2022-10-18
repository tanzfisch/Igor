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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_CLIPBOARD_H__
#define __IGOR_CLIPBOARD_H__

#include <igor/resources/module/iModule.h>

#include <any>

namespace igor
{

    /*! supported clipboard formats
    */
    enum class iClipboardFormat
    {
        IgorNodes,
        Empty
    };

    /*! clipboard

    \todo make the clipboard work outside of igor
    Are we even using this?
	*/
    class IGOR_API iClipboard : public iModule<iClipboard>
    {

        friend class iModule<iClipboard>;

    public:
        /*! set data on clipboard
        */
        void setData(iClipboardFormat format, const std::any &data);

        /*! \returns data on clipboard
        */
        const std::any &getData() const;

        /*! \returns true if there is data in the clipboard
        */
        bool hasData() const;

        /*! \returns the format of the data in the clipboard
        */
        iClipboardFormat getFormat() const;

        /*! clears the content of the clipboard
        */
        void clear();

    private:
        /*! format of data held in the clipboard
        */
        iClipboardFormat _format = iClipboardFormat::Empty;

        /*! the data on this clipboard
        */
        std::any _data;

        /*! does nothing
		*/
        iClipboard() = default;

        /*! does nothing
		*/
        ~iClipboard() = default;
    };

}; // namespace igor

#endif // __IGOR_CLIPBOARD_H__