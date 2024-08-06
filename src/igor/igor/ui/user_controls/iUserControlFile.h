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

#ifndef IGOR_USERCONTROL_FILE_H
#define IGOR_USERCONTROL_FILE_H

#include <igor/ui/user_controls/iUserControl.h>
#include <igor/ui/dialogs/iDialogFileSelect.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

    class iWidgetLineTextEdit;

    /*! User control file chooser
     */
    class IGOR_API iUserControlFile : public iUserControl
    {

    public:
        /*! ctor initializes member variables

        \param parent the optional parent
        */
        iUserControlFile(const iWidgetPtr parent = nullptr);

        /*! clean up
         */
        ~iUserControlFile();

        /*! sets the filename

        \param filename the filename
        */
        void setFileName(const iaString &filename);

        /*! \returns the filename
         */
        const iaString &getFileName() const;

        /*! sets path where the file dialog should start from

        \param path the path to start the file dialog from
        */
        void setPreselectedPath(const iaString &path);

        /*! \returns the path where the file dialog starts from
         */
        const iaString &getPreselectedPath() const;

        /*! register on filename change event

        \param changeDelegate the delegate to register
        */
        void registerOnChangedDelegate(iChangeDelegate changeDelegate);

        /*! unregister from filename change event

        \param changeDelegate the delegate to unregister
        */
        void unregisterOnChangedDelegate(iChangeDelegate changeDelegate);

        /*! sets where the path coming from the file dialog will be optimized or not

        optimized means in this case to make it a relative path to one of the search paths defined in iResourceManager

        \param optimizePath if true path will be optimized
        */
        void setOptimizePath(bool optimizePath = true);

        /*! \returns true if the path optimization is on
         */
        bool getOptimizePath() const;

    private:
        /*! the preselected path where the file dialog starts from
         */
        iaString _preselectedPath;

        /*! flag if selected path should be optimized
         */
        bool _optimizePath = true;

        /*! filename changed event
         */
        iChangeEvent _fileNameChanged;

        /*! text edit field for filename
         */
        iWidgetLineTextEdit *_fileNameTextEdit = nullptr;

        /*! file dialog used to select files
         */
        iDialogFileSelect *_fileDialog = nullptr;

        /*! called after file dialog is closed

        \param dialog pointer of closed dialog
        */
        void onFileLoadDialogClosed(iDialogPtr dialog);

        /*! called when text box's content changed

        \param source the source of the event
        */
        void onTextChanged(const iWidgetPtr source);

        /*! called when button was pressed

        \param source the source of the event
        */
        void onFileSelectButtonPressed(const iWidgetPtr source);

        /*! initialize gui elements
         */
        void initGUI();

        /*! release resources
         */
        void deinitGUI();
    };

    /*! user control file pointer definition
     */
    typedef iUserControlFile *iUserControlFilePtr;

} // namespace igor

#endif // IGOR_USERCONTROL_FILE_H