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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __iDIALOGFILESELECT__
#define __iDIALOGFILESELECT__

#include <igor/ui/dialogs/iDialog.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

	class iWidgetGrid;
	class iWidgetLabel;
	class iWidget;
	class iDialog;
	class iWidgetSpacer;
	class iWidgetTextEdit;
	class iWidgetScroll;

	/*! file dialog porpose definition
	*/
	enum class iFileDialogPurpose
	{
		Load,
		Save
	};

	/*! file select dialog

	\todo need some sort of UserDialogBaseClass with some convinient methods
	*/
	class Igor_API iDialogFileSelect : public iDialog
	{

	public:
		/*! does nothing
		*/
		iDialogFileSelect() = default;

		/*! deinitializes gui elements
		*/
		~iDialogFileSelect() = default;

		/*! \returns the widgets type
        */
		virtual iWidgetType getWidgetType() const override;

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
		iWidgetScroll *_scroll = nullptr;

		/*! file and folder grid
		*/
		iWidgetGrid *_fileGrid = nullptr;

		/*! path edit text field
		*/
		iWidgetTextEdit *_pathEdit = nullptr;

		/*! file name text field
		*/
		iWidgetTextEdit *_filenameEdit = nullptr;

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

		/*! clears fiel grid
		*/
		void clearFileGrid();

		/*! add entry to file grid

		\param col collumn in grid
		\param row row in grid
		\param path full path of entry
		\param displayName representive name of entry
		\param isFolder if true it is a folder; if fals it is a file
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

#endif