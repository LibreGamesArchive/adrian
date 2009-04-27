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
