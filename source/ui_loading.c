/*
Copyright (C) 2003-2006 Andrey Nazarov

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "ui_local.h"

/*
=============================================================================

CONNECTION / LOADING SCREEN

=============================================================================
*/

/*
==============
UI_DrawLoading
==============
*/
void UI_DrawLoading( int realtime ) {
    clientStatus_t	loadingStatus;
	char buffer[MAX_STRING_CHARS];
	char *s;
	int x, y;

	client.GetClientStatus( &loadingStatus );

    if( uis.glconfig.renderer == GL_RENDERER_SOFTWARE ) {
        ref.SetClipRect( DRAW_CLIP_MASK, &uis.clipRect );
    } else {
    	ref.SetScale( &uis.scale );
    }

#if 0
	if( loadingStatus.mapname[0] ) {
		qhandle_t hPic;

		Com_sprintf( buffer, sizeof( buffer ), "/levelshots/%s.jpg", loadingStatus.mapname );
		if( ( hPic = ref.RegisterPic( buffer ) ) != 0 ) {
			ref.DrawStretchPic( 0, 0, uis.width, uis.height, hPic );
		} else {
			ref.DrawFill( 0, 0, uis.width, uis.height, 0x00 );
		}
	} else
#endif
		ref.DrawFill( 0, 0, uis.width, uis.height, 0x00 );

	x = uis.width / 2;
	y = 8;
    
    s = loadingStatus.demoplayback ? "Playing back " : "Connecting to ";
	Q_concat( buffer, sizeof( buffer ), s, loadingStatus.servername, NULL );
	UI_DrawString( x, y, NULL, UI_CENTER|UI_DROPSHADOW, buffer );
	y += 40;

	if( loadingStatus.fullname[0] ) {
		UI_DrawString( x, y, colorYellow, UI_CENTER|UI_DROPSHADOW, loadingStatus.fullname );
	}
	y += 60;

	switch( loadingStatus.connState ) {
	case ca_challenging:
		Com_sprintf( buffer, sizeof( buffer ), "Challenging... %i", loadingStatus.connectCount );
        s = buffer;
		break;
	case ca_connecting:
        if( loadingStatus.connectCount == -1 ) {
            s = "Loading anticheat...";
        } else {
    		Com_sprintf( buffer, sizeof( buffer ), "Connecting... %i", loadingStatus.connectCount );
            s = buffer;
        }
		break;
	case ca_connected:
		s = "Receiving server data...";
		break;
	case ca_loading:
		Com_sprintf( buffer, sizeof( buffer ), "Loading... %s", loadingStatus.loadingString );
        s = buffer;
		break;
    case ca_precached:
	    s = "Awaiting server frame...";
        break;
	default:
		Com_Error( ERR_DROP, "%s: bad cls.state %i", __func__, loadingStatus.connState );
		break;
	}
	UI_DrawString( x, y, NULL, UI_CENTER|UI_DROPSHADOW, s );
	y += 64;

	if( loadingStatus.connState > ca_connected ) {
		return;
	}
	
	// draw message string
	if( loadingStatus.loadingString[0] ) {
		UI_DrawString( x, y, colorRed, UI_CENTER|UI_DROPSHADOW|UI_MULTILINE, loadingStatus.loadingString );
	}

    if( uis.glconfig.renderer == GL_RENDERER_SOFTWARE ) {
        ref.SetClipRect( DRAW_CLIP_DISABLED, NULL );
    } else {
	    ref.SetScale( NULL );
    }
}

