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

#ifndef __iMODELDATAIO__
#define __iMODELDATAIO__

#include <iaString.h>
#include <iaMemBlock.h>
using namespace IgorAux;

#include <iNode.h>
#include <iModel.h>

namespace Igor
{

    /*! save mode
    */
    enum class iSaveMode
    {
        /*! keeps external references unchanged
        */
        KeepExternals,

        /*! converts external reference data but keeps it in seperate files
        \todo not implemented
        */
        // KeepAndConvertExternals,

        /*! converts external data and embeds it in to one file
        */
        EmbedExternals
    };

	/*! pure abstract interface for node tree loaders
	*/
	class Igor_API iModelDataIO
	{
		
	public:

        /*! \returns name of loader
        */
        const iaString& getName();

        /*! \returns identifier or file extension
        */
        const iaString& getIdentifier();

		/*! loades the data from filesystem and returns the result 
        
        (in some cases the data might not be loaded but generated)

		\param filename filename of file to load
		\return parameter optional import or generate parameters
		*/
        virtual iNode* importData(const iaString& filename, iModelDataInputParameter* parameter = nullptr);

		/*! saved node and underlying tree to filesystem

		\param filename destination filename
		\param node root node of tree to save
		*/
		virtual void exportData(const iaString& filename, iNode* node, iSaveMode saveMode = iSaveMode::KeepExternals);

		/*! does nothing
		*/
		iModelDataIO() = default;

		/*! does nothing
		*/
		virtual ~iModelDataIO() = default;

    protected:

        /*! name of loader
        */
        iaString _name;

        /*! data identifier or file extension
        */
        iaString _identifier;
	};

}

#endif
