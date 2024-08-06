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

#ifndef IGOR_DIALOG_FILE_SELECT_H
#define IGOR_DIALOG_FILE_SELECT_H

#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/widgets/iWidgetLineTextEdit.h>
#include <igor/ui/widgets/iWidgetScroll.h>
#include <igor/ui/layouts/iWidgetGridLayout.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{
    /*! file dialog purpose definition
	*/
    enum class iFileDialogPurpose
    {
        Load,
        Save,
        SelectFolder
    };

    /*! file select dialog

	\todo need some sort of UserDialogBaseClass with some convenient methods
	*/
    class IGOR_API iDialogFileSelect : public iDialog
    {

    public:
        /*! init members

		\param parent the parent of this widget
		*/
        iDialogFileSelect(const iWidgetPtr parent = nullptr);

        /*! de-initializes gui elements
		*/
        ~iDialogFileSelect() = default;

        /*! opens dialog in load mode

		\param closeDelegate delegate called after dialog was closed
		\param path the path to start with
		*/
        void open(iDialogCloseDelegate dialogCloseDelegate, iFileDialogPurpose purpose = iFileDialogPurpose::Load, const iaString &path = "");

        /*! \returns full path currently selected by dialog
		*/
        iaString getFullPath();

        /*! \returns directory currently selected by dialog
		*/
        const iaString &getDirectory() const;

        /*! \returns filename currently selected by dialog
		*/
        const iaString &getFilename() const;

        /*! \returns the purpose with which this dialog was opened
        */
        iFileDialogPurpose getPurpose() const;

    private:
        /*! current directory in dialog
		*/
        iaString _directory;

        /*! current filename in dialog
		*/
        iaString _filename;

        /*! the purpose of this file dialog
		*/
        iFileDialogPurpose _purpose = iFileDialogPurpose::Load;

        /*! scroll widget for file and folder grid
		*/
        iWidgetScrollPtr _scroll = nullptr;

        /*! file and folder grid
		*/
        iWidgetGridLayoutPtr _fileGrid = nullptr;

        /*! path edit text field
		*/
        iWidgetLineTextEditPtr _pathEdit = nullptr;

        /*! file name text field
		*/
        iWidgetLineTextEditPtr _filenameEdit = nullptr;

        /*! handles ok button click

		\param source source widget of event
		*/
        void onOK(const iWidgetPtr source);

        /*! handles cancel button click

		\param source source widget of event
		*/
        void onCancel(const iWidgetPtr source);

        /*! handles double click in file grid

		\param source source widget of event
		*/
        void onDoubleClick(const iWidgetPtr source);

        /*! handles path edit change

		\param source source widget of event
		*/
        void onPathEditChange(const iWidgetPtr source);

        /*! handles filename edit change

		\param source source widget of event
		*/
        void onFilenameEditChange(const iWidgetPtr source);

        /*! initializes directory and file name
		*/
        void updateFileDir();

        /*! clears file grid
		*/
        void clearFileGrid();

        /*! add entry to file grid

		\param col column in grid
		\param row row in grid
		\param path full path of entry
		\param displayName display name of entry
		\param isFolder if true it is a folder; if false it is a file
		*/
        void addToFileGrid(int32 col, int32 row, const iaString &path, iaString displayName, bool isFolder);

        /*! closes the dialog
		*/
        void close();

        /*! initialized widgets
		*/
        void initGUI();

        /*! updates file grid entries
		*/
        void updateFileGrid();
    };

    /*! dialog file select pointer definition
    */
    typedef iDialogFileSelect *iDialogFileSelectPtr;

} // namespace igor

#endif // IGOR_DIALOG_FILE_SELECT_H