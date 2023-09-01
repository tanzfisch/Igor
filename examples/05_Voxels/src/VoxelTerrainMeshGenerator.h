//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __GENERATORTERRAIN__
#define __GENERATORTERRAIN__

#include <igor/igor.h>
using namespace igor;
using namespace iaux;

class VoxelTerrainMeshGenerator : public iModelDataIO
{

public:
    /*! initialize members
    */
    VoxelTerrainMeshGenerator();

    /*! does nothing
    */
    virtual ~VoxelTerrainMeshGenerator() = default;

    /*! generates terrain tiles 

    !!! ATTENTION consumes and deletes "parameter"

    \param sectionname name of tile section
    \param parameter tile parameters
    \returns root node of generated scene graph
    */
    iNodePtr importData(const iParameters &parameters) override;

    /*! creates an instance of this class

    \returns new instance
    */
    static iModelDataIO *createInstance();
};

#endif
