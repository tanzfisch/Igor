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

#ifndef __iINSTANCER__
#define __iINSTANCER__

#include <igor/iDefines.h>

namespace igor
{

    /*! handles multiple render instances 

    source: http://sol.gfxile.net/instancing.html
    */
    class iInstancer
    {

    public:
        /*! initialize buffer

        \param instanceSize size of a single instance
        \param maxInstanceCount max instance count
        */
        iInstancer(uint32 instanceSize, uint32 maxInstanceCount);

        /*! free memory
        */
        ~iInstancer();

        /*! clears instances
        */
        void clearInstances();

        /*! copies instance buffer in to instances buffer

        no range check, no error handling, make sure you know what you are doing here

        \param buffer the instance to be added
        */
        void addInstance(void *buffer);

        /*! sets instance array object

        \param instanceArrayObject instance array object id
        */
        void setInstanceArrayObject(uint32 instanceArrayObject);

        /*! \returns per instance size
        */
        uint32 getInstanceSize() const;

        /*! \returns instances count
        */
        uint32 getInstanceCount() const;

        /*! \returns max instances count
        */
        uint32 getInstanceMaxCount() const;

        /*! \returns instances data buffer
        */
        const void *getInstanceDataBuffer() const;

        /*! \returns instances buffer size
        */
        uint32 getInstanceDataBufferSize() const;

        /*! \returns instance array object id
        */
        uint32 getInstanceArrayObject() const;

    private:
        /*! instance array object id
        */
        uint32 _instanceArrayObject = 0;

        /*! per instance size
        */
        uint32 _instanceSize = 0;

        /*! maximum instances count
        */
        uint32 _maxInstanceCount = 0;

        /*! instances count
        */
        uint32 _instanceCount = 0;

        /*! size of instances buffer
        */
        uint32 _bufferSize = 0;

        /*! instances data buffer
        */
        char *_instanceDataBuffer = nullptr;

        /*! current position in instances buffer
        */
        char *_currentBufferPosition = nullptr;
    };

#include <igor/graphics/iInstancer.inl>

} // namespace igor

#endif