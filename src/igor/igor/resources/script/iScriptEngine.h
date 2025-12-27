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

#ifndef IGOR_SCRIPT_ENGINE_H
#define IGOR_SCRIPT_ENGINE_H

#include <igor/resources/module/iModule.h>
#include <igor/entities/iEntity.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{
    class iScriptEngineImpl;

    class IGOR_API iScriptEngine : public iModule<iScriptEngine>
    {

        friend class iThread;
        friend class iModule<iScriptEngine>;

    public:
        /*! run given script from string

        \param script the script to run
        */
        void execute(const char *script);

        /*! initialize script for entity

        \param script the script to run
        \returns the script environment handle
        */
        bool initEntityScript(const char *script, int &envRef, int &initRef, int &updateRef, int &finalRef, int &messageRef, int &eventRef);

        /*! deinitialize script for entity

        \param envRef environment handle to clean up script
        */
        void deinitEntityScript(int envRef);

        /*! execute function in given script environment

        \param functionRef the function reference to call
        */
        void callEntityInit(iEntityPtr entity, int initRef);
        void callEntityUpdate(iEntityPtr entity, int updateRef);
        void callEntityFinal(iEntityPtr entity, int finalRef);
        void callEntityMessage(iEntityPtr entity, int messageRef);
        void callEntityEvent(iEntityPtr entity, int eventRef);

    private:
        /*! pimpl
         */
        std::unique_ptr<iScriptEngineImpl> _impl;

        /*! registers current thread to the script engine
         */
        void registerThread();

        /*! init members
         */
        iScriptEngine();

        /*! cleanup
         */
        virtual ~iScriptEngine();
    };
}

#endif // IGOR_SCRIPT_ENGINE_H