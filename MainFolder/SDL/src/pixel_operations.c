#include <err.h>
#include "SDL/SDL.h"
#include <stdbool.h>
#include "pixel_operations.h"

static inline
Uint8* pixel_ref(SDL_Surface* surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface* surface, unsigned x, unsigned y)
{
    Uint8* p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
    case 1:
        return *p;

    case 2:
        return *(Uint16*)p;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32*)p;
    }

    return 0;
}

void put_pixel(SDL_Surface* surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8* p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16*)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32*)p = pixel;
        break;
    }
}

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}


/*static int *get_histogram(SDL_Surface *image)
{
    int *histogram = calloc(256, sizeof(int));
    for (int h = 0; h < image->h; h++)
    {
        for (int w = 0; w < image->w; w++)
        {
            Uint8 r, g, b;
            Uint32 pixel = image_get_pixel(image, h, w);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            // This is a grayscale image so we have r = g = b
            histogram[r]++;
        }
    }
    return histogram;
}*/

size_t seuil(SDL_Surface *image_surface) // on grayscale image_surface
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;
    size_t total = height*width;
    float histogram[256] = { 0.0F };
    for(size_t x=0; x < width; x++)
    {
        for(size_t y=0; y < height; y++)
        {
            Uint32 pixel;
            pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel,image_surface->format, &r, &g, &b);
            int grayLevel = r;
            histogram[grayLevel] += 1;
        }
    }

    for (int i = 0; i < 256; i++)
        histogram[i] /= total;

    float ut = 0;
    for (int i = 0; i < 256; i++)
        ut += i*histogram[i];

    int max_k = 0;
    float max_sigma_k = 0;
    for (int k = 0; k < 256; k++)
    {
        float wk = 0;
        for (int i = 0; i <= k; i++)
            wk += histogram[i];
        float uk = 0;
        for (int i = 0; i <= k; i++)
            uk += i*histogram[i];

        float sigma_k = 0;
        if (wk != 0 && wk != 1)
            sigma_k = ((ut*wk - uk)*(ut*wk - uk)) / (wk*(1 - wk));
        if (sigma_k > max_sigma_k)
        {
            max_k = k;
            max_sigma_k = sigma_k;
        }
    }
    return (size_t)max_k;
}

int* makeArray(SDL_Surface *img){
         int *array = NULL;
         array = malloc(sizeof(int) * ((img->h) * (img->w)));
         int *arrayX = array;
          for(int y = 0; y < img->h; ++y)
          {
                  for(int x = 0; x < img->w; ++x)
                  {
                        Uint32 p = get_pixel(img, x, y);
                        Uint8 r, g, b;
                        SDL_GetRGB(p, img->format, &r, &g, &b);
                        if(r >= 128)
                                 *arrayX = 0;
                            else
                                 *arrayX = 1;
                                ++arrayX;
                  }
            }

   return array;
    }

bool is_white_pixel(SDL_Surface *image, int h, int w)
{
    Uint8 r, g, b;
    Uint32 pixel = get_pixel(image, h, w);
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
    return r == 255 && g == 255 && b == 255;
}



