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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IGOR_SCRIPT_COMPONENT_H
#define IGOR_SCRIPT_COMPONENT_H

#include <igor/entities/iEntityComponent.h>
#include <igor/resources/script/iScript.h>

#include <iaux/system/iaEvent.h>
using namespace iaux;

#define IGOR_LUA_NOREF -2

namespace igor
{
    class iEntity;
    typedef iEntity *iEntityPtr;

    /*! script delegate definition
     */
    IGOR_DELEGATE_DEFINITION(iScript, iEntityPtr);

    /*! basically means which state to trigger next beginning with Init

    Init -> Update -> Final -> Stop
             ^  |
             |__|

    */
    enum class iEntityScriptState
    {
        Init,   //! triggers onInit(self) in script
        Update, //! triggers onUpdate(self, dt) in script
        Final,  //! triggers onFinal(self) in script
        Stop    //! cleanup script env
    };

    /*! script data
     */
    struct iScriptData
    {
        /*! delegate to be executed with given entity and user data

        deprecated in future only use scripts
         */
        iScriptDelegate _delegate;

        /*! script script
         */
        iScriptPtr _script;

        /*! which state to trigger next inside the script
         */
        iEntityScriptState _scriptState = iEntityScriptState::Init;

        /*! script environment table reference
        */
        int _envRef = IGOR_LUA_NOREF;
        int _initRef = IGOR_LUA_NOREF;
        int _updateRef = IGOR_LUA_NOREF;
        int _finalRef = IGOR_LUA_NOREF;
        int _messageRef = IGOR_LUA_NOREF;
        int _eventRef = IGOR_LUA_NOREF;
    };

    /*! script component
     */
    class iScriptComponent : public iEntityComponent
    {
    public:
        /*! ctor
         */
        iScriptComponent();

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance();

        /*! \returns type name of component
         */
        static const iaString &getTypeName();

        /*! \returns a set of info strings
         */
        std::vector<iaString> getInfo() const override;

        /*! \returns scripts
         */
        const std::vector<iScriptData> &getScripts() const;

        /*! adds script from code

        \param script the script to add
        */
        void addScript(const iScriptDelegate &delegate);

        /*! adds script from script

        \param script the script script to add
        \param name the name of the script
        */
        void addScript(const iScriptPtr script);

        /*! removes script from entity

        \param script the script to be removed
        */
        void removeScript(const iScriptDelegate &delegate);

        /*! executes all behaviours
         */
        void execute();

    private:
        /*! scripts
         */
        std::vector<iScriptData> _scripts;

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override;
    };

    /*! script component pointer definition
     */
    typedef iScriptComponent *iScriptComponentPtr;
}

#endif // IGOR_SCRIPT_COMPONENT_H