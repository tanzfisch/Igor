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

#ifndef __IGOR_EVENT__
#define __IGOR_EVENT__

#include <igor/iDefines.h>

#include <functional>
#include <memory>

namespace igor
{
    class iWindow;
    typedef iWindow* iWindowPtr;

    /*! event type definitions
    */
    enum class iEventType
    {
        iEventKeyDown,
        iEventKeyUp,
        iEventKeyASCII,

        iEventMouseKeyDown,
        iEventMouseKeyUp,
        iEventMouseKeyDoubleClick,
        iEventMouseMove,
        iEventMouseWheel,

        iEventWindowClose,
        iEventWindowOpen,
        iEventWindowResize,

        iEventNodeAddedToScene,
        iEventNodeRemovedFromScene,
        iEventSceneSelectionChanged,

        iEventEntityCreated,
        iEventEntityDestroyed,

        // has to be last
        iEventTypeCount
    };

    /*! event kind definition
    */
    enum class iEventKind
    {
        Input = 1,
        Keyboard = 2,
        Mouse = 4,
        Window = 8,
        Scene = 16,
    };

    /*! define event kind mask
    */
    typedef uint32 iEventKindMask;

    /*! event base class
    */
    class IGOR_API iEvent
    {
    public:
        /*! init members

        \param window the window that event came from (only in case it was an event created by a window)
        */
        iEvent(iWindowPtr window = nullptr);

        /*! does nothing
        */
        virtual ~iEvent() = default;

        /*! \returns true if event is of given kind

        \param kind the given kind
        */
        bool isOfKind(const iEventKind kind);

        /*! \returns event type
        */
        virtual iEventType getEventType() const = 0;

        /*! \returns event kind mask
        */
        virtual iEventKindMask getEventKindMask() const = 0;

        /*! \returns name of event
        */
        virtual const iaString getName() const = 0;

        /*! \returns information to event
        */
        virtual const iaString getInfo() const;

        /*! \returns the window
        */
        iWindowPtr getWindow() const;

        /*! \returns true if already consumed
        */
        bool isConsumed() const;

        /*! consumes this event
        */
        void consume();

        /*! dispatch event

        only executes if event is of type T

        \param func the given function to call with the event
        \returns true if event was consumed by the given function
        */
        template <typename T, typename F>
        bool dispatch(const F &func)
        {
            if (!isConsumed() &&
                getEventType() == T::getStaticType())
            {
                if (func(static_cast<T &>(*this)))
                {
                    consume();
                    return true;
                }
            }
            return false;
        }

    private:
        /*! if true event was already consumed
        */
        bool _consumed = false;

        /*! the window
        */
        iWindowPtr _window = nullptr;
    };

    /*! event pointer definition
    */
    typedef std::shared_ptr<iEvent> iEventPtr;

    /*! stream operator

	\param stream the destination
	\param event the event
	\returns the resulting stream
	*/
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iEvent &event);

    /*! helper macro to define event type
    */
#define IGOR_EVENT_CLASS_TYPE(type)                                              \
    static iEventType getStaticType() { return iEventType::type; }               \
    virtual iEventType getEventType() const override { return getStaticType(); } \
    virtual const iaString getName() const override { return #type; }

    /*! helper to generate event kind mask
    */
#define IGOR_EVENT_KIND_MASK(mask) \
    virtual iEventKindMask getEventKindMask() const override { return mask; }

    /*! helper macro for defining a member function
    */
#define IGOR_BIND_EVENT_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

}; // namespace igor

#endif // __IGOR_EVENT__