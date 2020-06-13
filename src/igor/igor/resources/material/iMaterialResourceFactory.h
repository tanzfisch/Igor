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
// contact: martinloga@gmx.de

#ifndef __iMATERIALRESOURCEFACTORY__
#define __iMATERIALRESOURCEFACTORY__

#include <igor/iDefines.h>
#include <igor/resources/material/iMaterial.h>

#include <iaux/data/iaString.h>
#include <iaux/system/iaSingleton.h>
#include <iaux/system/iaMutex.h>
#include <iaux/system/iaEvent.h>
using namespace iaux;

#include <list>
#include <map>
#include <vector>

namespace igor
{

    /*! event triggered by material created

    \param materialID id of material created
    */
    iaEVENT(iMaterialCreatedEvent, iMaterialCreatedDelegate, void, (uint64 materialID), (materialID));

    /*! event triggered by material destroyed

    \param materialID id of material destroyed
    */
    iaEVENT(iMaterialDestroyedEvent, iMaterialDestroyedDelegate, void, (uint64 materialID), (materialID));

    class iTargetMaterial;

    /*! material resource factory
    */
    class Igor_API iMaterialResourceFactory : public iaSingleton<iMaterialResourceFactory>
    {
        friend class iaSingleton<iMaterialResourceFactory>;

    public:
        /*! creates material

        \returns id of new material
        */
        uint64 createMaterial(iaString name = L"");

        /*! destoys material by id

        \param materialID the materials id
        */
        void destroyMaterial(uint64 materialID);

        /*! creates and returns a target material

        \returns pointer to new target material
        */
        iTargetMaterial *createTargetMaterial();

        /*! destroyes given target material

        \param targetMaterial target material to destroy
        */
        void destroyTargetMaterial(iTargetMaterial *targetMaterial);

        /*! \returns materials with given material id
        \param materialID the materials id
        */
        iMaterialPtr getMaterial(uint64 materialID);

        /*! \returns default material
        */
        iMaterialPtr getDefaultMaterial();

        /*! \returns default material ID
        */
        uint64 getDefaultMaterialID() const;

        /*! \returns color ID material ID
        */
        uint64 getColorIDMaterialID() const;

        /*! \returns materials with given material name
        \param materialName the materials name
        */
        iMaterialPtr getMaterial(iaString materialName);

        /*! \retruns material id with given material name
        \param materialName the materials name
        */
        uint64 getMaterialID(iaString materialName);

        /*! \returns the currently activated material
        */
        iMaterialPtr getCurrentMaterial();

        /*! \returns materials list sorted by render order
        */
        std::vector<iMaterialPtr> getSortedMaterials();

        /*! register delegate to material created event

        \param materialCreatedDelegate delegate to register
        */
        void registerMaterialCreatedDelegate(iMaterialCreatedDelegate materialCreatedDelegate);

        /*! unregister delegate from material created event

        \param materialCreatedDelegate delegate to unregister
        */
        void unregisterMaterialCreatedDelegate(iMaterialCreatedDelegate materialCreatedDelegate);

        /*! register delegate to material destroyed event

        \param materialDestroyedDelegate delegate to register
        */
        void registerMaterialDestroyedDelegate(iMaterialDestroyedDelegate materialDestroyedDelegate);

        /*! unregister delegate from material destroyed event

        \param materialDestroyedDelegate delegate to unregister
        */
        void unregisterMaterialDestroyedDelegate(iMaterialDestroyedDelegate materialDestroyedDelegate);

    private:
        /*! material created event
        */
        iMaterialCreatedEvent _materialCreatedEvent;

        /*! material destroyed event
        */
        iMaterialDestroyedEvent _materialDestroyedEvent;

        /*! dirty flag to control changes in render order
        */
        bool _dirtyMaterials = true;

        /*! materials sorted by render order
        */
        std::vector<iMaterialPtr> _sortedMaterials;

        /*! lookup table for materials
        */
        std::map<uint64, iMaterialPtr> _materials;

        /*! mutex to protect the target material list
        */
        iaMutex _targetMaterialMutex;

        /*! list of target materials
        */
        std::vector<iTargetMaterial *> _targetMaterials;

        /*! current material in use
        */
        iMaterialPtr _currentMaterial = nullptr;

        /*! mutex for material lists
        */
        iaMutex _mutexMaterial;

        /*! default material ID
        */
        uint64 _defaultMaterial = iMaterial::INVALID_MATERIAL_ID;

        /*! color id material id
        */
        uint64 _colorIDMaterial = iMaterial::INVALID_MATERIAL_ID;

        /*! set a material as the active material.

        changes render states as needed.

        ATTENTION can only be used if window is open and renderer initialized

        \param materialID the material's id to be activated
        */
        void setMaterial(uint64 materialID);

        /*! sorts the materials if needed
        */
        virtual void updateGroups();

        /*! initializes default materials
        */
        void initDefaultMaterials();

        /*! initialisation of members
        */
        iMaterialResourceFactory();

        /*! clean up and error handling
        */
        virtual ~iMaterialResourceFactory();
    };

}; // namespace igor

#endif
