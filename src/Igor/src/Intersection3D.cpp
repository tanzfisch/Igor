// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <Intersection3D.h>

bool Ray::intersect(AABox &aabox, iaVector3f &resulting_position)
{
	bool inside = true;
	iaVector3f maxt(-1.0f,-1.0f,-1.0f);

	for(int i=0;i<3;++i)
	{
		if(origin[i] < aabox.v1[i])
		{
			resulting_position[i]= aabox.v1[i];
			inside = false;

			// Calculate T distances to candidate planes
			if((uint32)(dir[i]))	maxt[i] = (aabox.v1[i] - origin[i]) / dir[i];
		}
		else if(origin[i] > aabox.v2[i])
		{
			resulting_position[i] = aabox.v2[i];
			inside = false;

			// Calculate T distances to candidate planes
			if((uint32)(dir[i]))	maxt[i] = (aabox.v2[i] - origin[i]) / dir[i];
		}		
	}

	// Ray origin inside bounding box
	if(inside)
	{
		resulting_position = origin;
		return true;
	}

	// Get largest of the maxT's for final choice of intersection
	int which_plane = 0;
	if(maxt[1] > maxt[which_plane])	which_plane = 1;
	if(maxt[2] > maxt[which_plane])	which_plane = 2;

	// Check final candidate actually inside box
	//if(maxt[which_plane] > 9999999999) return false;

	for(int i=0;i<3;i++)
	{
		if(i!=which_plane)
		{
			resulting_position[i] = origin[i] + maxt[which_plane] * dir[i];
			if(resulting_position[i] < aabox.v1[i] || resulting_position[i] > aabox.v2[i])	return false;
		}
	}
	return true;	// ray hits box
}

bool Ray::intersect(Plane &plane, iaVector3f &resulting_position)
{
	float32 check = (plane.n * dir);
	if(check == 0.0f) return false;

	float32 t = (plane.n * (plane.o - origin)) / check;
	resulting_position = origin + dir * t;

	return true;
}

bool Sphere::inFrontOf(Plane &plane)
{
	if( ((o - plane.o) * plane.n) < radius) return false;
	else return true;
}

bool Sphere::intersect(Frustum &frustum)
{
	if(!inFrontOf(frustum.near_plane)) return false;
	if(!inFrontOf(frustum.far_plane)) return false;
	if(!inFrontOf(frustum.left_plane)) return false;
	if(!inFrontOf(frustum.right_plane)) return false;
	if(!inFrontOf(frustum.top_plane)) return false;
	if(!inFrontOf(frustum.bottom_plane)) return false;
	return true;
}

void Frustum::set(iaMatrixf &view_matrix,float32 width,float32 height,float32 nearPlane,float32 farPlane)
{
	near_plane.o = view_matrix.pos + view_matrix.depth * nearPlane;
	near_plane.n = view_matrix.depth;
	near_plane.n.conjugate();
	near_plane.n.normalize();

	far_plane.o = view_matrix.pos + view_matrix.depth * farPlane;
	far_plane.n = view_matrix.depth;
	far_plane.n.normalize();

	left_plane.o = view_matrix.pos;
	left_plane.n = (near_plane.o - view_matrix.right * width * 0.5) % view_matrix.top;

	right_plane.o = view_matrix.pos;
	right_plane.n = view_matrix.top % (near_plane.o + view_matrix.right * width * 0.5);

	top_plane.o = view_matrix.pos;
	top_plane.n = (near_plane.o + view_matrix.top * height * 0.5) % view_matrix.right;

	bottom_plane.o = view_matrix.pos;
	bottom_plane.n = view_matrix.right % (near_plane.o - view_matrix.top * height * 0.5);
}