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
#include "hud.h"
#include "cl_util.h"
#include "demo.h"
#include "demo_api.h"
#include <memory.h>
#include "Exports.h"

#include <map>

int g_demosniper = 0;
int g_demosniperdamage = 0;
float g_demosniperorg[3];
float g_demosniperangles[3];
float g_demozoom;

// FIXME:  There should be buffer helper functions to avoid all of the *(int *)& crap.

/*
=====================
Demo_WriteBuffer

Write some data to the demo stream
=====================
*/
void Demo_WriteBuffer( int type, int size, unsigned char *buffer )
{
	int pos = 0;
	unsigned char buf[ 32 * 1024 ];
	*( int * )&buf[pos] = type;
	pos+=sizeof( int );

	memcpy( &buf[pos], buffer, size );

	// Write full buffer out
	gEngfuncs.pDemoAPI->WriteBuffer( size + sizeof( int ), buf );
}

bool Is_Valid_Demo_Msg(int value)
{
	return (value == static_cast<int>(DEMO_MSG::SNIPERDOT)) && (value == static_cast<int>(DEMO_MSG::ZOOM));
}

/*
=====================
Demo_ReadBuffer

Engine wants us to parse some data from the demo stream
=====================
*/
void DLLEXPORT Demo_ReadBuffer( int size, unsigned char *buffer )
{
//	RecClReadDemoBuffer(size, buffer);

	int i = 0;

	const int type = *( int * )buffer;

	if (!Is_Valid_Demo_Msg(type))
	{
		gEngfuncs.Con_DPrintf( "Unknown demo buffer type, skipping.\n" );
		return;
	}

	i += sizeof( int );
	switch ( static_cast<DEMO_MSG>(type) )
	{
	case DEMO_MSG::SNIPERDOT:
		g_demosniper = *(int * )&buffer[ i ];
		i += sizeof( int );
		
		if ( g_demosniper )
		{
			g_demosniperdamage = *( int * )&buffer[ i ];
			i += sizeof( int );

			g_demosniperangles[ 0 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperangles[ 1 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperangles[ 2 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperorg[ 0 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperorg[ 1 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperorg[ 2 ] = *(float *)&buffer[i];
			i += sizeof( float );
		}
		break;
	case DEMO_MSG::ZOOM:
		g_demozoom = *(float * )&buffer[ i ];
		i += sizeof( float );
		break;
	default:
		break;
	}
}