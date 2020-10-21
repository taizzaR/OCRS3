#ifndef IMAGE_H
#define IMAGE_H

void init_sdl();

SDL_Surface* load_image(char* path);

SDL_Surface* display_image(SDL_Surface* img);

void wait_for_keypressed();

#endif

