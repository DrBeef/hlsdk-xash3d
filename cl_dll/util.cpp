/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// util.cpp
//
// implementation of class-less helper functions
//

#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "hud.h"
#include "cl_util.h"
#include <string.h>

#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

extern vec3_t vec3_origin;

#ifdef _MSC_VER
vec3_t vec3_origin;
#endif

double sqrt( double x );

float Length( const float *v )
{
	int	i;
	float	length;

	length = 0;
	for( i = 0; i < 3; i++ )
		length += v[i] * v[i];
	length = sqrt( length );		// FIXME

	return length;
}

void VectorAngles( const float *forward, float *angles )
{
	float tmp, yaw, pitch;

	if( forward[1] == 0 && forward[0] == 0 )
	{
		yaw = 0;
		if( forward[2] > 0 )
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		yaw = ( atan2( forward[1], forward[0]) * 180 / M_PI );
		if( yaw < 0 )
			yaw += 360;

		tmp = sqrt( forward[0] * forward[0] + forward[1] * forward[1] );
		pitch = ( atan2( forward[2], tmp ) * 180 / M_PI );
		if( pitch < 0 )
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

float VectorNormalize( float *v )
{
	float length, ilength;

	length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	length = sqrt( length );		// FIXME

	if( length )
	{
		ilength = 1 / length;
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}

	return length;
}

void VectorInverse( float *v )
{
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

void VectorScale( const float *in, float scale, float *out )
{
	out[0] = in[0] * scale;
	out[1] = in[1] * scale;
	out[2] = in[2] * scale;
}

void VectorMA( const float *veca, float scale, const float *vecb, float *vecc )
{
	vecc[0] = veca[0] + scale * vecb[0];
	vecc[1] = veca[1] + scale * vecb[1];
	vecc[2] = veca[2] + scale * vecb[2];
}

HSPRITE LoadSprite( const char *pszName )
{
	int i;
	char sz[256];

	if( ScreenWidth < 640 )
		i = 320;
	else
		i = 640;

	sprintf( sz, pszName, i );

	return SPR_Load( sz );
}

// Added NormalizeAngles and GetAnglesFromVectors - Max Vollmer, 2017-08-17
void NormalizeAngles(Vector &angles)
{
	while (angles.x >= 360) angles.x -= 360;
	while (angles.y >= 360) angles.y -= 360;
	while (angles.z >= 360) angles.z -= 360;
	while (angles.x < 0) angles.x += 360;
	while (angles.y < 0) angles.y += 360;
	while (angles.z < 0) angles.z += 360;
}

void GetAnglesFromVectors(const Vector &forward, const Vector &right, const Vector &up, Vector &angles)
{
	float sr, sp, sy, cr, cp, cy;

	sp = -forward[2];

	float cp_x_cy = forward[0];
	float cp_x_sy = forward[1];
	float cp_x_sr = -right[2];
	float cp_x_cr = up[2];

	float yaw = atan2(cp_x_sy, cp_x_cy);
	float roll = atan2(cp_x_sr, cp_x_cr);

	cy = cos(yaw);
	sy = sin(yaw);
	cr = cos(roll);
	sr = sin(roll);

	if (fabs(cy) > EPSILON)
	{
		cp = cp_x_cy / cy;
	}
	else if (fabs(sy) > EPSILON)
	{
		cp = cp_x_sy / sy;
	}
	else if (fabs(sr) > EPSILON)
	{
		cp = cp_x_sr / sr;
	}
	else if (fabs(cr) > EPSILON)
	{
		cp = cp_x_cr / cr;
	}
	else
	{
		cp = cos(asin(sp));
	}

	float pitch = atan2(sp, cp);

	angles[0] = pitch / (M_PI*2.f / 360.f);
	angles[1] = yaw / (M_PI*2.f / 360.f);
	angles[2] = roll / (M_PI*2.f / 360.f);

	NormalizeAngles(angles);
}
