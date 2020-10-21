#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "image.h"

int main()
{

    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;
    size_t threshold;

    init_sdl();

    image_surface = load_image("f.jpg");
    screen_surface = display_image(image_surface);
    threshold = seuil(image_surface);


    wait_for_keypressed();
    size_t width = image_surface->w;
    size_t height = image_surface->h;
    //met l'image en gris
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            Uint32 pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            Uint32 average = 0.3 * r + 0.59 * g + 0.11 * b;
            r = g = b = average;
            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, x, y, pixel);
        }
    }
    update_surface(screen_surface, image_surface);

    wait_for_keypressed();

    //met l'image en noir et blanc
    for(size_t x=0; x < width; x++)
    {
        for(size_t y=0; y < height; y++)
        {
            Uint32 pixel;
            pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel,image_surface->format, &r, &g, &b);
            size_t graylevel = r;
            if (graylevel < threshold)
            { r = 0; g = 0; b = 0; }
            else
            { r = 255; g = 255; b=255; }
            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, x, y, pixel);
        }
    }
    update_surface(screen_surface, image_surface);

    wait_for_keypressed();
    
    makeArray(image_surface);


    wait_for_keypressed();


    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return 0;
}


//gcc src/main.c -o bin/test -I include -L lib -lmingw32 -l SDL2main -lSDL2