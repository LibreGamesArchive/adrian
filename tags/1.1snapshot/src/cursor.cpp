/*
 * ADRIAN - A 3D role playing game
 * Copyright (C) 2003  Bhanu Chetlapalli, Vamsi Krishna, Swamy Suman
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "globals.h"
#include <SDL/SDL.h>

/* Creates a new mouse cursor from an XPM */


SDL_Cursor *init_system_cursor(const char *image[])
{
  int i, row, col;
  Uint8 data[4*32];
	Uint8 mask[4*32];
	int hot_x, hot_y;
	i = -1;
	for ( row=0; row<32; ++row ) {
		for ( col=0; col<32; ++col ) {
			if ( col % 8 ) {
				data[i] <<= 1;
		        mask[i] <<= 1;
		    } else {
	    		++i;
			    data[i] = mask[i] = 0;
		    }
		    switch (image[4+row][col]) {
	        case '.':
         		 data[i] |= 0x01;
		         //k[i] |= 0x01;
				 break;
          	case '+':
		         mask[i] |= 0x01;
		         break;
//	        case 'X':
  //       		 data[i] |= 0x01;
	//	         //k[i] |= 0x01;
//				 break;
  //        	case '.':
	//	         mask[i] |= 0x01;
	//	         break;
			case ' ':
			     break;
		    }
    	}
	}
	sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
	return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}
