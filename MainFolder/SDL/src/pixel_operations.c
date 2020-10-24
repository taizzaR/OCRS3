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

void image_grayscale(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;
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
}

void image_binarize(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;
    size_t threshold;
    threshold = seuil(image_surface);
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

}


void array_swap(int array[], size_t i, size_t j)
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
}

void noiseReduction(SDL_Surface *image_surface)
{
    int w = image_surface -> w;
 int h = image_surface -> h;
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
 }
}

void lineCut(SDL_Surface *image_surface)
{
    Uint32 pixel;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int isBlank = 1;
    int firstCut = 1;
    int w = image_surface -> w;
    int h = image_surface -> h;
    //SDL_Surface *img_copy = copy_image(img);
    for(int i = 0; i < h ; i++)
    {
      isBlank = 1 ;
      for(int j = 0 ; j < w; j++)
      {
        pixel = get_pixel(image_surface, j, i);
        SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
        //Check if there is a black character in this line
        if(!r && !g && !b)
        {
          isBlank = 0;
          break;
        }
      }
      //For the first cut we cut the pixel line
      //before the line with a black character
      if(!isBlank && firstCut)
      {
          for(int k = 0; k < w; k++)
          {
            pixel = SDL_MapRGB(image_surface -> format, 255, 0, 0);
            put_pixel(image_surface, k, i - 1, pixel);
          }
          firstCut = 0;
      }
      //For the second cut we cut the first white line
      if(isBlank && !firstCut)
      {
        for(int k = 0; k < w; k++)
        {
          pixel = SDL_MapRGB(image_surface -> format, 255, 0, 0);
          put_pixel(image_surface, k, i, pixel);
        }
        firstCut = 1;
      }
    }
}

/*void charCut(SDL_Surface *image_surface)
{
    // Variables
  Uint32 pixel;
  Uint8 r ;
  Uint8 g;
  Uint8 b;
  int thereIsChar = 0;
  int lineWasWhite = 1;
  int canCut = 0;
  int w = image_surface -> w;
  int h = image_surface -> h;

  for(int i = 0; i < w; i++)
  {
    lineWasWhite = 1;
    for(int j = 0; j < h; j++)
    {
      pixel = get_pixel(image_surface, i, j);
      SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
      if(r == 0 && g == 0 && b == 0)
      {
          thereIsChar = 1;
          lineWasWhite = 0;
          break;
      }
    }
    if(lineWasWhite && !canCut)
    {
      continue;
    }
    if(thereIsChar && !canCut)
    {
      for(int k = 0; k < h; k++)
      {
        pixel = SDL_MapRGB(image_surface -> format, 255, 0, 0);
        put_pixel(image_surface, i - 1, k, pixel);
      }
      canCut = 1;
    }
    if(lineWasWhite && canCut)
    {
      for(int k = 0; k < h; k++)
      {
        pixel = SDL_MapRGB(image_surface -> format, 255, 0, 0);
        put_pixel(image_surface, i, k, pixel);
      }
      canCut = 0;
    }

  }
}*/

void cutchar(SDL_Surface *image_surface)
//cut the characters of the line with green columns
{
    Uint32 pixel;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int isBlack = 0;
    int firstcut = 1;
    int width = image_surface->w;
    int height = image_surface->h;
    for(int i = 0; i < width ; i++)
    {
        isBlack = 0;
        for(int j = 0;j < height; j++)
        {
            pixel = get_pixel(image_surface,i,j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if(r == 0 && g == 0 && b == 0)
            {
                isBlack = 1;
                break;
            }
        }
        //if there is a black character and no first cut before
        if(isBlack == 1  && firstcut == 1)
        {
            for(int k = 0; k < height; k++)
            {
                pixel = SDL_MapRGB(image_surface->format, 0, 255, 0);
                put_pixel(image_surface, i-1, k, pixel);
            }
            firstcut = 0;
        }
        //if there is a white column and a first cut before
        else if(isBlack == 0 && firstcut == 0)
        {
            for(int k = 0; k < height; k++)
            {
                pixel = SDL_MapRGB(image_surface->format, 0, 255, 0);
                put_pixel(image_surface, i, k, pixel);
            }
            firstcut = 1;
        }
    }
}

int mediumPixelSpacingVertical(SDL_Surface *image_surface)
{
  //Variables
  Uint32 pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  int whitePixel = 0;
  int blackPixel = 0;
  int res = 0;
  int w = image_surface -> w;
  int h = image_surface -> h;

  for(int i = 0; i < w; i++)
  {
    for(int j = 0; j < h; j++)
    {
      pixel = get_pixel(image_surface, i, j);
      SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
      if(r == 255 && g == 255 && b == 255)
        whitePixel++;
      else
        blackPixel++;
    }
  }
  res = whitePixel / (blackPixel / 2);
  return(res);
}

/**/
int mediumPixelSpacingHorizontal(SDL_Surface *image_surface)
{
  Uint32 pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  int whitePixel = 0;
  int blackPixel = 0;
  int res = 0;
  int w = image_surface -> w;
  int h = image_surface -> h;

  for(int i = 0; i < h; i++)
  {
    for(int j = 0; j < w; j++)
    {
      pixel = get_pixel(image_surface, j, i);
      SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
      if(r == 255 && g == 255 && b == 255)
        whitePixel++;
      else
        blackPixel++;
    }

  }
  res = whitePixel / (blackPixel / 2);
  return(res);
}

/* detect horizontal text block*/
void blockDetection_horizontal(SDL_Surface *image_surface)
{
  Uint32 pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  int C = mediumPixelSpacingHorizontal(image_surface)*4;
  int countWhite;
  int w = image_surface -> w;
  int h = image_surface -> h;

  for(int i = 0; i < h; i++)
  {
    countWhite = 0;
    for(int j = 0; j < w; j++)
    {
      pixel = get_pixel(image_surface, j, i);
      SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
      if(r == 255 && g == 255 && b == 255)
      {
        countWhite++;
      }
      if(r == 0 && g == 0 && b == 0)
      {
        if(countWhite <= C)
        {
            int k = j - 1;
            while(countWhite > 0)
            {
              pixel = SDL_MapRGB(image_surface -> format, 0, 0, 0);
              put_pixel(image_surface, k, i, pixel);
              countWhite--;
              k--;
            }
        }
        else
        {
          countWhite = 0;
        }
      }
    }
  }
}

/*detect vertical text block*/
void blockDetection_vertical(SDL_Surface *image_surface)
{
  Uint32 pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  int C = mediumPixelSpacingVertical(image_surface) * 4;
  int countBlack;
  int w = image_surface -> w;
  int h = image_surface -> h;

  for(int i = 0; i < w; i++)
  {
    countBlack = 0;
    for(int j = 0; j < h; j++)
    {
      pixel = get_pixel(image_surface, i, j);
      SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
      if(r == 255 && g == 255 && b == 255)
      {
        countBlack++;
      }
      else
      {
        if(countBlack <= C)
        {
            int k = j - 1;
            while(countBlack > 0)
            {
              pixel = SDL_MapRGB(image_surface -> format, 0, 0, 0);
              put_pixel(image_surface, i, k, pixel);
              countBlack--;
              k--;
            }
        }
        else
        {
          countBlack = 0;
        }
      }
    }
  }
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




