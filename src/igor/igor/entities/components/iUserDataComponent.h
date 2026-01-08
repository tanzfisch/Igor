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

#ifndef IGOR_MIME_DATA_COMPONENT_H
#define IGOR_MIME_DATA_COMPONENT_H

#include <igor/entities/iEntityComponent.h>
#include <igor/data/iMimeData.h>

namespace igor
{
    /*! mime data component
     */
    class IGOR_API iUserDataComponent : public iEntityComponent
    {
    public:
        /*! ctor
         */
        iUserDataComponent();

        /*! creates instance of this component type
         */
        static iEntityComponentPtr createInstance();

        /*! \returns type name of component
         */
        static const iaString &getTypeName();

        /*! \returns a set of info strings
         */
        std::vector<iaString> getInfo() const override;

        /*! set value for given key

        key "name" is a reserved key. Every item has a name

        \param key to set
        \param value to set
        */
        template <typename T>
        void setValue(const iaString &key, const T &value);

        /*! \returns value for given key

        key "name" is a reserved key. Every item has a name

        \param key the given key
        */
        template <typename T>
        T getValue(const iaString &key) const;

        /*! \returns true if there is a value for given key
         */
        bool hasValue(const iaString &key) const;

        /*! \returns mime data
         */
        iMimeData &getData();

    private:
        /*! mime data
         */
        iMimeData _data;

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override;
    };

#include <igor/entities/components/iUserDataComponent.inl>

    /*! script component pointer definition
     */
    typedef iUserDataComponent *iUserDataComponentPtr;
}

#endif // IGOR_MIME_DATA_COMPONENT_H