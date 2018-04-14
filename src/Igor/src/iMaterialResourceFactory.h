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
// (c) Copyright 2012-2017 by Martin Loga
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
#include <iaMutex.h>
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
        uint64 createMaterial(iaString name = L"");

        /*! destoys material by id

        \param materialID the materials id
        */
        void destroyMaterial(uint64 materialID);

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

        \param materialID the material's id to be activated
        */
        void setMaterial(uint64 materialID);

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
        vector<iMaterialPtr> getSortedMaterials();

    private:

        /*! dirty flag to control changes in render order
        */
        bool _dirtyMaterials = true;

        /*! materials sorted by render order
        */
        vector<iMaterialPtr> _sortedMaterials;

        /*! lookup table for materials
        */
        map<uint64, iMaterialPtr> _materials;

        /*! mutex to protect the target material list
        */
        iaMutex _targetMaterialMutex;

        /*! list of target materials
        */
        vector<iTargetMaterial*> _targetMaterials;

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

};

#endif

