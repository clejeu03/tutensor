#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#include <zoom.hpp>

#define ZOOMRATE 2.0 //This define doesn't work

/*Using the right-clic, this function allow the user to zoom in order to select more precisely points*/

int myZoom(int active, SDL_Surface * image, double position_x, double position_y, SDL_Surface *screen, SDL_Rect *imageOffset){

	/*We can zoom in a very simple way with these lines

	SDL_Surface *dst = zoomSurface(image, 2.0, 2.0, 1);
	SDL_BlitSurface(dst , NULL, screen, imageOffset); 
	SDL_FreeSurface(sdt); */

	/*If no image is currently zoomed*/
	if(!active){

		/*Creating resources*/
		SDL_Surface *src;
		SDL_Surface *transfert;
		SDL_Rect selection;
		int dstwidth, dstheight;

		src = image;//Safer for the image source
		transfert = image; //So got the rights values

		/*Here I've written myself my image width and height because
		//the application can't divide by 2 properly*/


		selection.w = (image->w)/2;
		selection.h = (image->h)/2;
		selection.x = position_x - (image->w)/4 ;
		selection.y = position_y - (image->h)/2 ;

		/*Pre-calculating the size of the SDL_Surface we'll need*/
		zoomSurfaceSize(selection.w, selection.h, 2, 2, &dstwidth, &dstheight);
		transfert->w = dstwidth;
		transfert->h = dstheight;

		/*Selecting and copying a part of src in a temporary surface*/
		SDL_BlitSurface(src, &selection, transfert, 0);
		

		SDL_Surface *dst = zoomSurface(transfert, 2.0, 2.0, 1);//Here is a seg fault, but the zoom is done after two right clics anyway
		SDL_BlitSurface(dst , NULL, screen, imageOffset); 
		SDL_UpdateRect(screen, 0, 0, dstwidth, dstheight );
	    SDL_FreeSurface(dst);
	    SDL_FreeSurface(src);

	    active = 1;
	}

	/*If an image is currently zoomed, undo-zoom*/
	if(active){

		active = 0;

	}

	return active;
}