#ifndef IMAGE_H
#define IMAGE_H

void init_sdl();

SDL_Surface* load_image(char* path);

SDL_Surface* display_image(SDL_Surface* img);

void wait_for_keypressed();

SDL_Surface *image_rotate(SDL_Surface *image, double angle);

SDL_Surface *image_deskew(SDL_Surface *image);

#endif

