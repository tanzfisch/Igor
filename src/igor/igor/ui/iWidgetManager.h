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

#ifndef __IGOR_WIDGETMANAGER__
#define __IGOR_WIDGETMANAGER__

#include <igor/iDefines.h>
#include <igor/system/iKeyboard.h>
#include <igor/ui/widgets/iWidget.h>
#include <igor/events/iEventKeyboard.h>
#include <igor/events/iEventMouse.h>
#include <igor/events/iEventWindow.h>
#include <igor/resources/module/iModule.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/ui/iDrag.h>

#include <vector>
#include <unordered_map>
#include <deque>

namespace igor
{

    class iDialog;
    typedef iDialog *iDialogPtr;
    class iWidgetDockingLayout;
    typedef iWidgetDockingLayout *iWidgetDockingLayoutPtr;

    /*! manages the widgets
     */
    class IGOR_API iWidgetManager : public iModule<iWidgetManager>
    {

        friend class iModule<iWidgetManager>;
        friend class iWidget;
        friend class iDialog;
        friend class iDrag;

    public:
        /*! called on any other event
         */
        void onEvent(iEvent &event);

        /*! shows tooltip at given position

        \param pos the position to show the tooltip
        \param text the text of the tooltip
        */
        void showTooltip(const iaVector2f &pos, const iaString &text);

        /*! hides the tooltip
         */
        void hideTooltip();

        /*! \returns widget by id

        \param id id of widget
        */
        iWidgetPtr getWidget(iWidgetID id) const;

        /*! \returns dialog by id

        \param id id of dialog
        */
        iDialogPtr getDialog(iWidgetID id);

        /*! \returns the theme in use
         */
        iWidgetThemePtr getTheme() const;

        /*! sets the theme to use

        \param theme the theme to use
        */
        void setTheme(const iWidgetThemePtr &theme);

        /*! sets the desktop dimensions

        you should update this e.g. when the window size changes

        \param width width of desktop
        \param height height of desktop
        */
        void setDesktopDimensions(uint32 width, uint32 height);

        /*! \returns desktop width
         */
        uint32 getDesktopWidth() const;

        /*! \returns desktop height
         */
        uint32 getDesktopHeight() const;

        /*! draws the widgets if theme is defined

        needs to be called by application

        make sure you have set up an orthogonal coordinate system with the same dimensions as set via setDesktopDimensions
        */
        void draw();

        /*! set this widget exclusively modal
         */
        void setModal(iDialogPtr dialog);

        /*! \returns current modal widget
         */
        iDialogPtr getModal() const;

        /*! \returns true: if widget is modal

        \param dialog the dialog to check if it is modal
        */
        bool isModal(iDialogPtr dialog);

        /*! reset modal flag
         */
        void resetModal();

        /*! updates recursively all widgets before rendering
         */
        void onUpdate();

        /*! \returns true if in drag
        */
        bool inDrag() const;

        /*! \returns current drag if exists

        test with inDrag if it exists first
        */
        const iDrag& getDrag() const;        

    private:
        /*! modal marker
         */
        iDialogPtr _modal = nullptr;

        /*! pointer to current theme
         */
        iWidgetThemePtr _currentTheme;

        /*! list of all widgets
         */
        std::unordered_map<iWidgetID, iWidgetPtr> _widgets;

        /*! list of all dialogs
         */
        std::unordered_map<iWidgetID, iDialogPtr> _dialogs;

        /*! docker layouts
         */
        std::unordered_map<iWidgetID, iWidgetDockingLayoutPtr> _dockerLayouts;

        /*! current desktop width
         */
        uint32 _desktopWidth = 0;

        /*! current desktop height
         */
        uint32 _desktopHeight = 0;

        /*! tooltip position
         */
        iaVector2f _tooltipPos;

        /*! tooltip text
         */
        iaString _tooltipText;

        /*! list of dialogs to close
         */
        std::set<iWidgetID> _dialogsToClose;

        /*! current drag
        */
        std::unique_ptr<iDrag> _drag;

        /*! closes the dialog and queues a close event in to be called after the update handle
         */
        void closeDialog(iDialogPtr dialog);

        /*! registers widget to WidgetManager so we can track if all widgets got destroyed at shutdown

        \param widget the widget to track
        */
        void registerWidget(iWidgetPtr widget);

        /*! unregister widget from WidgetManager so we don't track this one anymore

        \param widget the widget to not track anymore
        */
        void unregisterWidget(iWidgetPtr widget);

        /*! registers dialog to WidgetManager so we can track if all dialogs got destroyed at shutdown

        \param dialog the dialog to track
        */
        void registerDialog(iDialogPtr dialog);

        /*! unregister dialog from WidgetManager so we don't track this one anymore

        \param dialog the dialog to not track anymore
        */
        void unregisterDialog(iDialogPtr dialog);

        /*! registers docker layout

        \param dialog the docker to track
        */
        void registerDockerLayout(iWidgetDockingLayoutPtr dockerLayout);

        /*! unregister docker layout

        \param dialog the docker to not track anymore
        */
        void unregisterDockerLayout(iWidgetDockingLayoutPtr dockerLayout);

        /*! puts dialog in front by manipulating it's z index and the index of other dialogs

        \param dialog the dialog to put in front
        */
        void putDialogInFront(iDialogPtr dialog);

        /*! traverse widget tree and updates positions and sizes

        \param widget current widget to update
        */
        void traverseContentSize(iWidgetPtr widget);

        /*! traverse widget tree and updates alignment
         */
        void traverseAlignment(iWidgetPtr widget, int32 offsetX, int32 offsetY, int32 clientRectWidth, int32 clientRectHeight);

        /*! returns the active dialogs

        \param[out] dialogs resulting list of active dialogs
        \param sortedAscending if true the output list is sorted z index ascending if false the opposite
        */
        void getActiveDialogs(std::vector<iDialogPtr> &dialogs, bool sortedAscending = true);

        /*! actual implementation that handles a moved mouse

        \param to the mouse postion to use
        */
        bool onMouseMove(const iaux::iaVector2f &pos);

        /*! called when key was pressed

        \param event the event to handle
        */
        bool onKeyDown(iEventKeyDown &event);

        /*! called when key was released

        \param event the event to handle
        */
        bool onKeyUp(iEventKeyUp &event);

        /*! called on ascii input

        \param event the event to handle
        */
        bool onKeyASCIIInput(iEventKeyASCII &event);

        /*! handles mouse key down event

        \param event the mouse key down event
        \returns true if consumed
        */
        bool onMouseKeyDownEvent(iEventMouseKeyDown &event);

        /*! handles mouse key up event

        \param event the mouse key up event
        \returns true if consumed
        */
        bool onMouseKeyUpEvent(iEventMouseKeyUp &event);

        /*! handles mouse key double click event

        \param event the mouse key double click event
        \returns true if consumed
        */
        bool onMouseKeyDoubleClickEvent(iEventMouseKeyDoubleClick &event);

        /*! handles mouse move event

        \param event the mouse move event
        \returns true if consumed
        */
        bool onMouseMoveEvent(iEventMouseMove &event);

        /*! handles mouse wheel event

        \param event the mouse wheel event
        \returns true if consumed
        */
        bool onMouseWheelEvent(iEventMouseWheel &event);

        /*! handle window resize event

        \param event the window resize event
        \returns true if consumed
        */
        bool onWindowResize(iEventWindowResize &event);

        /*! begin drag

        makes copy and keeps ownership

        \param drag the drag to drag 
        */
        void beginDrag(const iDrag& drag);

        /*! resets the drag object
        */
        void endDrag();

        /*! init
         */
        iWidgetManager();

        /*! checks for mem leaks and releases all left over widgets
         */
        virtual ~iWidgetManager();
    };

#include <igor/ui/iWidgetManager.inl>

} // namespace igor

#endif // __IGOR_WIDGETMANAGER__
