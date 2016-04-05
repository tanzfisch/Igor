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
// (c) Copyright 2014-2016 by Martin Loga
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

#include <iDefines.h>
#include <iMaterial.h>

#include <iaString.h>
#include <iaSingleton.h>
using namespace IgorAux;

#include <list>
#include <map>
#include <vector>
using namespace std;

namespace Igor
{

    class iMaterialGroup;
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
        int32 createMaterial(iaString name = L"");

        /*! destoys material by id

        \param materialID the materials id
        */
        void destroyMaterial(int32 materialID);

        /*! creates and returns a target material

        \returns pointer to new target material
        */
        iTargetMaterial* createTargetMaterial();

        /*! destroyes given target material

        \param targetMaterial target material to destroy
        */
        void destroyTargetMaterial(iTargetMaterial* targetMaterial);

        /*! set a material as the active material.

        changes render states as needed.

        ATTENTION can only be used if window is open and renderer initialized

        \param materialID the materials id to be activated
        */
        void setMaterial(int32 materialID);

        /*! \returns materials with given material id
        \param materialID the materials id
        */
        iMaterial* getMaterial(int32 materialID);

        /*! \returns default material
        */
        iMaterial* getDefaultMaterial();

        /*! \returns default material ID
        */
        int32 getDefaultMaterialID() const;

        /*! \returns materials with given material name
        \param materialName the materials name
        */
        iMaterial* getMaterial(iaString materialName);

        /*! \retruns material id with given material name
        \param materialName the materials name
        */
        uint32 getMaterialID(iaString materialName);

        /*! \returns the currently activated material
        */
        iMaterial* getCurrentMaterial();

        /*! \returns material group with given material id

        \param materialID the materials id
        */
        iMaterialGroup* getMaterialGroup(int32 materialID);

        /*! \returns list of all material groups
        */
        list<iMaterialGroup*>* getMaterialGroups();

    private:

        /*! dirty flag for changes in materials list.

        if false the materials list has to be sorted
        */
        bool _dirtyMaterials = true;

        /*! list of materials
        */
        list<iMaterialGroup*> _materials;

        /*! lookup table for material groups.
        */
        map<int32, iMaterialGroup*> _materialMap;

        /*! mutex to protect the target material list
        */
        mutex _targetMaterialMutex;

        /*! list of target materials
        */
        vector<iTargetMaterial*> _targetMaterials;

        /*! current material in use
        */
        iMaterial* _currentMaterial = nullptr;

        /*! mutex for material lists
        */
        mutex _mutexMaterial;

        /*! default material ID
        */
        uint32 _defaultID = iMaterial::INVALID_MATERIAL_ID;

        /*! sorts the materials if needed
        */
        virtual void updateGroups();

        /*! initializes default materials
        */
        void initDefaultMaterial();

        /*! initialisation of members
        */
        iMaterialResourceFactory();

        /*! clean up and error handling
        */
        virtual ~iMaterialResourceFactory();

	};

};

#endif

