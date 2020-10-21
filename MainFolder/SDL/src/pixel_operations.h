#ifndef PIXEL_OPERATIONS_H_
#define PIXEL_OPERATIONS_H_

#include <stdlib.h>
#include <SDL.h>
#include <stdbool.h>

Uint32 get_pixel(SDL_Surface* surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface* surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);

size_t seuil(SDL_Surface *image_surface);
//int *get_histogram(SDL_Surface *image);

int* makeArray(SDL_Surface *img);

bool is_white_pixel(SDL_Surface *image, int h, int w);

#endif