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
// (c) Copyright 2012-2017 by Martin Loga
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

#ifndef __INTERACTION3D__
#define __INTERACTION3D__

#include <Mathematics.h>

class Igor_API Plane
{
	public:

		iaVector3f o;	// Ortsvektor
		iaVector3f n;	// Normale
};

class Igor_API AABox
{
	public:

		iaVector3f v1;
		iaVector3f v2;
};

class Igor_API Ray
{
	public:

		iaVector3f origin;	// Ortsvektor
		iaVector3f dir;		// Richtung

		bool intersect(Plane &plane, iaVector3f &resulting_position);
		bool intersect(AABox &aabox, iaVector3f &resulting_position);
};

class Igor_API Frustum
{
	public:

		Plane left_plane;
		Plane right_plane;
		Plane top_plane;
		Plane bottom_plane;
		Plane near_plane;
		Plane far_plane;

		void set(iaMatrixf &view_matrix,float32 width,float32 height,float32 near,float32 far);
};

class Igor_API Sphere
{
	public:

		iaVector3f o;		// Ortsvektor
		float32 radius;	// Radius

		bool inFrontOf(Plane &plane);
		bool intersect(Frustum &frustum); // not tested yet
};

#endif
