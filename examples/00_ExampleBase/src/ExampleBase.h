//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2014-2020 by Martin Loga
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

#ifndef IGOR_EXAMPLEBASE_H
#define IGOR_EXAMPLEBASE_H

#include <igor/igor.h>
using namespace igor;

/*! Examples base class
*/
class ExampleBase : public iLayer
{

public:
    /*! init example
    */
    ExampleBase(iWindowPtr window, const iaString &name, bool createBaseSetup = true, bool createSkyBox = true, int32 zIndex = 0);

    /*! release resources
    */
    virtual ~ExampleBase();

    /*! \retruns the example's view
    */
    iView &getView();

    /*! \retruns the example's orthogonal view
    */
    iView &getViewOrtho();

    /*! \returns the examples scene
    */
    iScenePtr getScene();

    /*! \returns font
    */
    iTextureFontPtr getFont() const;

    /*! \returns outline font
    */
    iTextureFontPtr getOutlineFont() const;

    /*! \returns font material
    */
    iMaterialID getFontMaterial() const;

protected:
    /*! initialize example
    */
    virtual void onInit() override;

    /*! deinitialize example
    */
    virtual void onDeinit() override;

    /*! called every frame before draw

    override if you need to work with it
    */
    virtual void onUpdate() override;

    /*! called on any other event
    */
    virtual void onEvent(iEvent &event) override;

    /*! called by orthogonal view
    */
    virtual void onRenderOrtho();

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iEventKeyDown &event);

    /*! called when key was released

    \param event the event to handle
    */
    bool onKeyUp(iEventKeyUp &event);

    /*! \returns help screen string
    */
    virtual iaString getHelpString();

private:
    /*! the view we render 2D to
    */
    iView _viewOrtho;

    /*! the view we render 3D to
    */
    iView _view;

    /*! the scene holding our 3d nodes
    */
    iScenePtr _scene = nullptr;

    /*! igor logo
    */
    iTexturePtr _igorLogo;

    /*! outline font
    */
    iTextureFontPtr _outlineFont;

    /*! standard font
    */
    iTextureFontPtr _standardFont;    

    /*! if true we display a help screen
    */
    bool m_displayHelpScreen;

    /*! material definition for the sky box
    */
    iMaterialPtr _materialSkyBox;

    /*! draw igor logo
    */
    void drawLogo();

    /*! draw help screen
    */
    void drawHelpScreen();

    /*! handle window resize event

    \param event the window resize event
    \returns true if consumed
    */
    bool onWindowResize(iEventWindowResize &event);
};

#endif // IGOR_EXAMPLEBASE_H
