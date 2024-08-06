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

#ifndef __IGOR_MODELDATAIO__
#define __IGOR_MODELDATAIO__

#include <igor/scene/nodes/iNode.h>
#include <igor/data/iParameters.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

    /*! pure abstract interface for node tree loaders
	*/
    class IGOR_API iModelDataIO
    {
    public:
        /*! \returns name of loader
        */
        const iaString &getName();

        /*! \returns identifier or file extension
        */
        const iaString &getIdentifier();

        /*! loads or generates data based on given parameters

        \param parameters key values pairs determine what to load or generate
        */
        virtual iNodePtr importData(const iParameters &parameters);

        /*! save node and underlying tree to filesystem

		\param parameters export parameters
		*/
        virtual void exportData(const iParameters &parameters);

        /*! specialized version of importData

        \param filename the filename to load
        */
        iNodePtr importData(const iaString &filename);

        /*! specialized version of exportData

        \param filename the output file name
        \param node the source node to export
        \param saveMode how to handle external references during export
        */
        void exportData(const iaString &filename, iNodePtr node, iSaveMode saveMode = iSaveMode::KeepExternals);

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

} // namespace igor

#endif
