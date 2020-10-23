#ifndef SDL_H
#define SDL_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

SDL_Surface *image_load(const char *path);
SDL_Surface *image_new(const int height, const int width);

Uint32 image_get_pixel(SDL_Surface *image, int h, int w);
void image_set_pixel(SDL_Surface *image, int h, int w, Uint32 pixel);

#endif
