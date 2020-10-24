#ifndef PIXEL_OPERATIONS_H_
#define PIXEL_OPERATIONS_H_

#include <stdlib.h>
#include <SDL.h>
#include <stdbool.h>

Uint32 get_pixel(SDL_Surface* surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface* surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);
size_t seuil(SDL_Surface *image_surface);
void image_grayscale(SDL_Surface *image_surface);
void image_binarize(SDL_Surface *image_surface);
void array_swap(int array[], size_t i, size_t j);
void array_select_sort(int array[], size_t len);
void noiseReduction(SDL_Surface *image_surface);
void lineCut(SDL_Surface *image_surface);
//void charCut(SDL_Surface *image_surface);
void cutchar(SDL_Surface *image_surface);
int mediumPixelSpacingVertical(SDL_Surface *image_surface);
int mediumPixelSpacingHorizontal(SDL_Surface *image_surface);
void blockDetection_horizontal(SDL_Surface *image_surface);
void blockDetection_vertical(SDL_Surface *image_surface);
//int *get_histogram(SDL_Surface *image);
//int seuil2(SDL_Surface *image_surface);


int* makeArray(SDL_Surface *img);

bool is_white_pixel(SDL_Surface *image, int h, int w);

#endif