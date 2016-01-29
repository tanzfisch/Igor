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
//    (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __OPENGL2DEXAMPLE__
#define __OPENGL2DEXAMPLE__

#include <iWindow.h>
#include <iView.h>
#include <iParticleSystem2D.h>
#include <iKeyboard.h>
#include <iRainbow.h>
#include <iPerlinNoise.h>
using namespace Igor;

#include <iaMatrix.h>
#include <iaVector2.h>
#include <iaBSpline.h>
using namespace IgorAux;

#include <memory>
using namespace std;

namespace Igor
{
    class iSprite;
    class iTexture;
    class iTextureFont;
    class iRenderStatistics;
}

class OpenGL2DExample
{
private:

    iPerlinNoise perlinNoise;

    iaVector2f _lastMousePos;
    iaVector2f _logoPosition;

	iaMatrixf _modelMatrix;
	iaMatrixf _particleMatrix;
	iWindow _window;
	iView _view;
    iRainbow _rainbow;
	
	iSprite* _sprite = nullptr;
	iTextureFont* _font = nullptr;

	shared_ptr<iTexture> _particleTexture;
	shared_ptr<iTexture> _dummyTexture;

	int32 _materialWithTextureAndBlending = 0;
	int32 _materialWithoutDepthTest = 0;

	iParticleSystem2D _particleSystem;

    iaBSpline _spline;
    iRenderStatistics* _renderStatistics = nullptr;

	float _logoRotationAngle = 0.0f;
	float _animationvalue = 0.0f;
	float _emitangle = 0.1f;
	bool _dir = true;

    void onMouseMove(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);
	void onWindowClosed();
    void onWindowResize(int32 clientWidth, int32 clientHeight);

	void keyPressed(iKeyCode key);
	void handle();
	void particlesHandle();
	void render();
	void init();
	void deinit();

public:

	OpenGL2DExample();
	virtual ~OpenGL2DExample();
	
	void run();

};

#endif

