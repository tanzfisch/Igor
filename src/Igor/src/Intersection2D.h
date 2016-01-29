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

#ifndef __INTERACTION2D__
#define __INTERACTION2D__

#include <Mathematics.h>

namespace Intersection2D
{
	// POINT
	template <class T> class Point
	{
		public:

			iaVector2<T> pos;
	};

	typedef Point<float32> Pointf;
	typedef Point<double> Pointd;

	// SHAPE
	template <class T> class Shape
	{
		public:

			vector<iaVector2<T>> positions;
	};

	typedef Shape<float32> Shapef;
	typedef Shape<double> Shaped;

	// RECTANGLE
	template <class T> class Rectangle
	{
		public:

			T x;
			T y;
			T width;
			T height;

			Rectangle()
			{
				x = (T)0;
				y = (T)0;
				width = (T)0;
				height = (T)0;
			}
			
	};

	typedef Rectangle<float32> Rectanglef;
	typedef Rectangle<double> Rectangled;
	typedef Rectangle<int> Rectanglei;
	typedef Rectangle<long> Rectanglel;

	// INTERSECTIONS
	template<typename T>
	bool intersection(Point<T> point, Shape<T> shape)
    {
		int i, j;
		bool result = false;
		for (i = 0, j = shape.positions.size()-1; i < shape.positions.size(); j = i++)
		{
			if ((((shape.positions[i].y <= point.pos.y) && (point.pos.y < shape.positions[j].y)) ||
				((shape.positions[j].y <= point.pos.y) && (point.pos.y < shape.positions[i].y))) &&
				(point.pos.x < (shape.positions[j].x - shape.positions[i].x) * (point.pos.y - shape.positions[i].y) / (shape.positions[j].y - shape.positions[i].y) + shape.positions[i].x))
					result = !result;
		}
		return result;
    }

};

#endif
