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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iFILEDIALOG__
#define __iFILEDIALOG__

#include <iWidgetUserDialog.h>

#include <iaEvent.h>
#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iWidgetButton;
    class iWidgetGrid;
    class iWidgetLabel;
    class iWidget;
    class iWidgetDialog;
    class iWidgetSpacer;
    class iWidgetTextEdit;
    class iWidgetScroll;

    /*! file dialog return value
    */
    enum class iFileDialogReturnValue
    {
        Ok,
        Cancel,
        Error
    };

    /*! event triggered after file dialog was closed
    */
    iaEVENT(FileDialogCloseEvent, FileDialogCloseDelegate, void, (iFileDialogReturnValue value), (value));

    /*!

    \todo need User Dialog Base class with some convinient methods
    */
    class Igor_API iFileDialog : public iWidgetUserDialog
    {

    public:

        /*! initializes gui elements
        */
        iFileDialog();

        /*! deinitializes gui elements
        */
        ~iFileDialog();

        /*! opens dialog in load mode

        \param closeDelegate delegate called after dialog was closed
        \param path the path to start with
        */
        void load(FileDialogCloseDelegate closeDelegate, iaString path = "");

        /*! opens dialog in save mode

        \param closeDelegate delegate called after dialog was closed
        \param filepath the file path to start with
        */
        void save(FileDialogCloseDelegate closeDelegate, iaString filepath = "");

        /*! \returns full path currently selected by dialog
        */
        iaString getFullPath();

        /*! \returns directory currently selected by dialog
        */
        const iaString& getDirectory() const;
        
        /*! \returns filename currently selected by dialog
        */
        const iaString& getFilename() const;

        /*! \returns return state of dialog after it was closed
        */
        iFileDialogReturnValue getReturnState();

    private:

        /*! current directory in dialog
        */
        iaString _directory;

        /*! current filename in dialog
        */
        iaString _filename;

        /*! if true dialog is in load mode; if false it is in save mode
        */
        bool _load = false;

        /*! path last time selected when dialog was opened in load mode
        */
        static iaString _lastLoadPath;

        /*! path last time selected when dialog was opened in save mode
        */
        static iaString _lastSavePath;

        /*! file dialog close event
        */
        FileDialogCloseEvent _fileDialogCloseEvent;

        /*! file dialog return value
        */
        iFileDialogReturnValue _fileDialogReturnValue = iFileDialogReturnValue::Error;

        /*! ok button widget
        */
        iWidgetButton* _okButton = nullptr;

        /*! cancel button widget
        */
        iWidgetButton* _cancelButton = nullptr;

        /*! scroll widget for file and folder grid
        */
        iWidgetScroll* _scroll = nullptr;

        /*! over all grid
        */
        iWidgetGrid* _grid = nullptr;

        /*! file and folder grid
        */
        iWidgetGrid* _fileGrid = nullptr;

        /*! header label
        */
        iWidgetLabel* _headerLabel = nullptr;

        /*! path edit text field
        */
        iWidgetTextEdit* _pathEdit = nullptr;

        /*! buttion grid
        */
        iWidgetGrid* _buttonGrid = nullptr;

        /*! list of widgets in file grid
        */
        vector<iWidget*> _fileGridWidgets;

        /*! handles ok button click

        \param source source widget of event
        */
        void onOK(iWidget* source);

        /*! handles cancel button click

        \param source source widget of event
        */
        void onCancel(iWidget* source);

        /*! handles double click in file grid

        \param source source widget of event
        */
        void onDoubleClick(iWidget* source);

        /*! handles path edit change

        \param source source widget of event
        */
        void onPathEditChange(iWidget* source);

        /*! initializes directory and file name
        */
        void initFileDir();

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
        void addToFileGrid(int32 col, int32 row, iaString path, iaString displayName, bool isFolder);

        /*! closes the dialog
        */
        void close();

        /*! initialized widgets
        */
        void initGUI();

        /*! deinitializes widgets
        */
        void deinitGUI();

        /*! updates file grid entries
        */
        void updateFileGrid();

    };

}

#endif