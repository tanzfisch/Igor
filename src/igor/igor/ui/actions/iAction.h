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

#ifndef __IGOR_ACTION__
#define __IGOR_ACTION__

#include <igor/ui/actions/iActionContext.h>
#include <igor/resources/texture/iTexture.h>

namespace igor
{

    /*! action base class
	*/
    class IGOR_API iAction
    {

    public:
        /*! init members

		\param name the identifier of this action

        must be a unique name so it can be registered with the action manager
		*/
        iAction(const iaString &name);

        /*! does nothing
		*/
        virtual ~iAction() = default;

        /*! executed when action gets triggered

		\param context the context the action was called with
		*/
        virtual void execute(const iActionContext &context) = 0;

        /*! \returns true if this action will execute with given context

		\param context the context the action was called with
		*/
        virtual bool isCompatible(const iActionContext &context) = 0;

        /*! \returns the action identifier
		*/
        iaString getName() const;

        /*! sets text of action

		\param brief the brief description
        \param description the full description
		*/
        void setDescription(const iaString &brief, const iaString &description = "");

        /*! \returns the brief description
		*/
        const iaString &getBrief() const;

        /*! \returns the full description
		*/
        const iaString &getDescription() const;

        /*! sets icon of the action

		\param alias alias or id of icon
		*/
        void setIcon(const iaString &alias);

        /*! \returns the icon texture of this action
		*/
        const iaString &getIcon() const;

    private:
        /*! name of the action
		*/
        iaString _name;

        /*! brief description of the action
		*/
        iaString _brief;

        /*! description of the action
		*/
        iaString _description;

        /*! icon of action
		*/
        iaString _alias;
    };

    /*! action pointer definition
    */
    typedef iAction *iActionPtr;

} // namespace igor

#endif // __IGOR_ACTION__