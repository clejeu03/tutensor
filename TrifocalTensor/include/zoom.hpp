#ifndef __ZOOM_HPP__
#define __ZOOM_HPP__

#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>

int myZoom(int active, SDL_Surface * image, double position_x, double position_y, SDL_Surface *screen, SDL_Rect *imageOffset);

#endif