#include <err.h>
#include <stdbool.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
//#include "segmentation.h"


void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1, "Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char* path)
{
    SDL_Surface* img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface* display_image(SDL_Surface* img)
{
    SDL_Surface* screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE | SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
            img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if (SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYUP);
}

int main()
{

    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;
    
    

    init_sdl();

    image_surface = load_image("image_test/noise.bmp");
    screen_surface = display_image(image_surface);
    


    wait_for_keypressed();

    //size_t width = image_surface->w;
    //size_t height = image_surface->h;
    
    //met l'image en gris

    image_grayscale(image_surface);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    //met l'image en noir et blanc
    image_binarize(image_surface);

    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    //noise reduction
    noiseReduction(image_surface);

    /*update_surface(screen_surface, image_surface);
    //SDL_SaveBMP(image_surface, "image_test/image_segmentation.bmp");
    wait_for_keypressed();

    lineCut(image_surface);

    update_surface(screen_surface, image_surface);
    wait_for_keypressed();
    
    cutchar(image_surface);*/


    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    /*blockDetection_horizontal(image_surface);

    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    blockDetection_vertical(image_surface);

    update_surface(screen_surface, image_surface);
    wait_for_keypressed();*/


    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return 0;
}


//gcc src/main.c -o bin/test -I include -L lib -lmingw32 -l SDL2main -lSDL2