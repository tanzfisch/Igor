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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_MIME_DATA_H
#define IGOR_MIME_DATA_H

#include <igor/ui/widgets/iWidget.h>
#include <igor/resources/iResource.h>

#include <iaux/data/iaString.h>
using namespace iaux;

#include <unordered_map>
#include <cstring>

namespace igor
{

    /*! mime data container
    */
    class IGOR_API iMimeData
    {
    public:

        /*! default dtor
        */
        virtual ~iMimeData() = default;

        /*! sets mime data

        \param mimeType the type to use
        \param data the actual data (makes a copy and keeps ownership)
        \param dataSize size of data
        */
        void setData(const iaString &mimeType, const uint8 *data, uint32 dataSize);

        /*! returns the mime data

        \param mimeType the type to access
        \param[out] data pointer to data (iMimeData keeps ownership)
        \param[out] dataSize size of data
        */
        void getData(const iaString &mimeType, uint8 **data, uint32 &dataSize) const;

        /*! \returns true if mime data contains given mime type
        */
        bool hasType(const iaString &mimeType) const;

        /*! sets text for mime type "text/plain"

        \param text the text to set
        */
        void setText(const iaString &text);

        /*! \returns plain text if there is any
         */
        const iaString getText() const;

        /*! \returns true if mime type "text/plain" is present
         */
        bool hasText() const;

        /*! sets mime type "application/vnd.igor.widget-id"

        \param widgetID the widget id to set
        */
        void setWidgetID(iWidgetID widgetID);

        /*! \returns widget id
         */
        iWidgetID getWidgetID() const;

        /*! \returns true if mime type "application/vnd.igor.widget-id" is present
         */
        bool hasWidgetID() const;        

        /*! sets mime type "application/vnd.igor.resource-id"

        \param resourceID the resource id to set
        */
        void setResourceID(iResourceID resourceID);

        /*! \returns resource id
         */
        iResourceID getResourceID() const;        

        /*! \returns true if mime type "application/vnd.igor.resource-id" is present
         */
        bool hasResourceID() const;

        /*! clear all data
        */
        void clear();

        /*! compare two mime data sets

        \returns true if equal
        \param other the other item
        */
        bool operator==(const iMimeData &other) const;

        /*! compare two mime data sets for being not equal

        \returns true if not equal
        \param other the other item
        */
        bool operator!=(const iMimeData &other) const;

        /*! \returns true if it has any data
        */
        bool hasData() const;

    private:
        /*! data buffer
         */
        struct iMimeDataBuffer
        {
            uint8 *_data = nullptr;
            uint32 _dataSize = 0;

            iMimeDataBuffer(const uint8 *data, uint32 dataSize)
            {
                _data = new uint8[dataSize];
                _dataSize = dataSize;
                std::memcpy(_data, data, dataSize);
            }

            iMimeDataBuffer(const iMimeDataBuffer &other)
                : _data(new uint8[other._dataSize]), _dataSize(other._dataSize)
            {
                std::memcpy(_data, other._data, _dataSize);
            }

            iMimeDataBuffer &operator=(const iMimeDataBuffer &other)
            {
                if (this == &other)
                {
                    return *this;
                }

                delete[] _data;
                _dataSize = other._dataSize;
                _data = new uint8[_dataSize];
                std::memcpy(_data, other._data, _dataSize);
            }

            bool operator==(const iMimeDataBuffer &other) const
            {
                if(_dataSize != other._dataSize)
                {
                    return false;
                }

                return std::memcmp(_data, other._data, _dataSize) == 0;
            }

            bool operator!=(const iMimeDataBuffer &other) const
            {
                if(_dataSize != other._dataSize)
                {
                    return true;
                }

                return std::memcmp(_data, other._data, _dataSize) != 0;
            }            

            ~iMimeDataBuffer()
            {
                delete[] _data;
            }
        };

        /*! the data
         */
        std::unordered_map<iaString, iMimeDataBuffer> _data;
    };

}

#endif // IGOR_MIME_DATA_H