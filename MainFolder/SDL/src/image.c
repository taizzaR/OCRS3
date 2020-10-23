#include <err.h>
#include <stdbool.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"


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

/*void array_swap(int array[], size_t i, size_t j)
{
  int element1 = array[i];
  int element2 = array[j];
  array[i] = element2;
  array[j] = element1;
}

void array_select_sort(int array[], size_t len)
{
  size_t i = 0;
  size_t j;
  int min_index;
  while(i<len)
  {
    
    j= i;
    min_index = j;
    while(j<len)
    {
        if(array[j]<array[min_index])
        {
            min_index = j;
        }
        j+=1;
    }
    array_swap(array,i,min_index);
    i++;
  }
}*/

int main()
{

    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;
    size_t threshold;
    

    init_sdl();

    image_surface = load_image("simple_text_noise.png");
    screen_surface = display_image(image_surface);
    threshold = seuil(image_surface);


    wait_for_keypressed();

    size_t width = image_surface->w;
    size_t height = image_surface->h;
    
    //met l'image en gris

    for (size_t x = 0; x < height; x++)
    {
        for (size_t y = 0; y < width; y++)
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
    for(size_t x=0; x < height; x++)
    {
        for(size_t y=0; y < width; y++)
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

    int width2 = image_surface->w;
    int height2 = image_surface->h;

    const int delta[8][2] =
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    
    for (int h = 0; h < height2; h++)
    {
        for (int w = 0; w < width2; w++)
        {
            int sum = 0;
            for (int dir = 0; dir < 8; dir++)
            {
                int nh = h + delta[dir][0];
                int nw = w + delta[dir][1];
                if (nh >= 0 && nh < image_surface->h && nw >= 0 && nw < image_surface->w)
                    sum += is_white_pixel(image_surface, nh, nw);
            }

            Uint32 white_pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
            Uint32 black_pixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
            if (sum > 4)
                put_pixel(image_surface, h, w, white_pixel);
            else
                put_pixel(image_surface, h, w, black_pixel);
        }
    }

  /*int w;
  int h;
  w = image_surface -> w;
  h = image_surface -> h;
  int pixelTable[5];

 for(int i = 0; i < w; i++)
 {
   for(int j = 0; j < h; j++)
   {
     for(int k = j; k <= j + 4; k++)
     {
       //Borders of picture
       if(i == 0)
       {
         if(k == 0)
         {
            pixelTable[0] = get_pixel(image_surface, i, k);
            pixelTable[1] = get_pixel(image_surface, i, k);
            pixelTable[2] = get_pixel(image_surface, i, k);
            pixelTable[3] = get_pixel(image_surface, i, k + 1);
            pixelTable[4] = get_pixel(image_surface, i + 1, k);
            break;
         }
         if(k == h)
         {
            pixelTable[0] = get_pixel(image_surface, i, k);
            pixelTable[1] = get_pixel(image_surface, i, k - 1);
            pixelTable[2] = get_pixel(image_surface, i, k);
            pixelTable[3] = get_pixel(image_surface, i, k);
            pixelTable[4] = get_pixel(image_surface, i + 1, k);
            break;
         }
         else
         {
          pixelTable[0] = get_pixel(image_surface, i, k);
          pixelTable[1] = get_pixel(image_surface, i, k - 1);
          pixelTable[2] = get_pixel(image_surface, i, k);
          pixelTable[3] = get_pixel(image_surface, i, k + 1);
          pixelTable[4] = get_pixel(image_surface, i + 1, k);
          break;
         }
       }
       if(i == w)
       {
          if(k == 0)
          {
            pixelTable[0] = get_pixel(image_surface, i, k);
            pixelTable[1] = get_pixel(image_surface, i, k);
            pixelTable[2] = get_pixel(image_surface, i - 1, k);
            pixelTable[3] = get_pixel(image_surface, i, k + 1);
            pixelTable[4] = get_pixel(image_surface, i, k);
            break;
          }
          if(k == h)
          {
            pixelTable[0] = get_pixel(image_surface, i, k);
            pixelTable[1] = get_pixel(image_surface, i, k - 1);
            pixelTable[2] = get_pixel(image_surface, i - 1, k);
            pixelTable[3] = get_pixel(image_surface, i, k);
            pixelTable[4] = get_pixel(image_surface, i, k);
            break;
          }
          else
          {
            pixelTable[0] = get_pixel(image_surface, i, k);
            pixelTable[1] = get_pixel(image_surface, i, k - 1);
            pixelTable[2] = get_pixel(image_surface, i - 1, k);
            pixelTable[3] = get_pixel(image_surface, i, k + 1);
            pixelTable[4] = get_pixel(image_surface, i, k);
            break;
          }
       }
       if(k == 0)
       {
          pixelTable[0] = get_pixel(image_surface, i, k);
          pixelTable[1] = get_pixel(image_surface, i, k);
          pixelTable[2] = get_pixel(image_surface, i - 1, k);
          pixelTable[3] = get_pixel(image_surface, i, k + 1);
          pixelTable[4] = get_pixel(image_surface, i + 1, k);
          break;
       }
       if(k == h)
       {
        pixelTable[0] = get_pixel(image_surface, i, k);
        pixelTable[1] = get_pixel(image_surface, i, k - 1);
        pixelTable[2] = get_pixel(image_surface, i - 1, k);
        pixelTable[3] = get_pixel(image_surface, i, k);
        pixelTable[4] = get_pixel(image_surface, i + 1, k);
        break;
       }
       else
       {
        pixelTable[0] = get_pixel(image_surface, i, k);
        pixelTable[1] = get_pixel(image_surface, i, k - 1);
        pixelTable[2] = get_pixel(image_surface, i - 1, k);
        pixelTable[3] = get_pixel(image_surface, i, k + 1);
        pixelTable[4] = get_pixel(image_surface, i + 1, k);
        break;
       }
     }
      array_select_sort(pixelTable, 5);
      int med = pixelTable[2];
      put_pixel(image_surface, i, j, med);
   }
 }*/
 

    update_surface(screen_surface, image_surface);

    //makeArray(image_surface);


    wait_for_keypressed();


    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return 0;
}


//gcc src/main.c -o bin/test -I include -L lib -lmingw32 -l SDL2main -lSDL2