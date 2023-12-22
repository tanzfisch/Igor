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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_RESOURCE_DICTIONARY__
#define __IGOR_RESOURCE_DICTIONARY__

#include <igor/resources/iResource.h>

#include <unordered_map>

class TiXmlElement;

namespace igor
{

    /*! resource dictionary
     */
    class iResourceDictionary
    {
    public:
        /*! does nothing
         */
        iResourceDictionary() = default;

        /*! write the current resources in to a dictionary

        \param filename resource dictionary filename
        \returns true if successful
        */
        bool write(const iaString &filename);

        /*! imports resource dictionary

        \param filename filename of resource dictionary to import
        \param internal marks all these entries to not be exported during write
        \returns true if successful
        */
        bool read(const iaString &filename);

        /*! clears the resource dictionary
         */
        void clear();

        /*! \returns filename for given resource id

        \param id the given resource id
        */
        const iaString &getFilename(iResourceID id) const;

        /*! \returns alias for given resource id

        \param id the given resource id
        */
        const iaString &getAlias(iResourceID id) const;

        /*! set alias for given resource (only accept the alias if unique in current environment)

        \param id the given id
        \param alias the given alias
        */
        void setAlias(iResourceID id, const iaString &alias);

        /*! \returns resource id for given alias of filepath

        \param text the given alias or filepath
        */
        const iResourceID getResource(const iaString &text) const;

        /*! adds a resource to the dictionary

        \param filename the file name of the resource
        \param alias the alias to use for this resource
        \param internal if true it is marked for internal use in engine only
        */
        const iResourceID addResource(const iaString &filename, const iaString &alias = "", bool internal = false);

        /*! removes resource with given id

        \param resourceID the given resource id
        */
        void removeResource(iResourceID resourceID);

    private:
        /*! for faster access aliases and resource ids are all resource ids
         */
        std::unordered_map<iResourceID, iaString> _resourceDictionaryLookup;

        /*! alias hash to resource id
         */
        std::unordered_map<iResourceID, iResourceID> _aliasLookup;

        /*! redundant data kept for export

        resource id, filename, alias
        */
        std::vector<std::tuple<iResourceID, iaString, iaString>> _data;

        /*! reads resource dictionary element

        \param element the element to read
        \returns true if successful
        */
        bool readResourceDictionaryElement(TiXmlElement *element, bool internal);

        /*! internal add resource implementation

        \param uuid the uuid to add
        \param filename the filename to add
        \param alias the alias to add
        \param internal if true it will be added to the lookup but not to the export data
        */
        bool addResource(const iResourceID &uuid, const iaString &filename, const iaString &alias, bool internal);
    };

} // namespace igor

#endif // __IGOR_RESOURCE_DICTIONARY__