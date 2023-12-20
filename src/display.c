#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL; 

int window_width = 800;
int window_height = 600;

void destroy_window(void) {
  free(color_buffer);
  color_buffer = NULL;
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  window_width = display_mode.w;
  window_height = display_mode.h;

  // Create a SDL Window
  window = SDL_CreateWindow(
      NULL,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      window_width,
      window_height,
      SDL_WINDOW_BORDERLESS);

  if (!window) {
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  return true;
}

void render_color_buffer(void) {
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (window_width * sizeof(uint32_t)));
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
  // go through all rows and columns
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++) {
      color_buffer[(window_width * y) + x] = color;
    }
  }
}

void draw_grid() {
  for (int y = 0; y < window_height; y += 1) {
    for (int x = 0; x < window_width; x += 1) {
      if (x % 10 == 0 || y % 10 == 0) {
        color_buffer[(window_width * y) + x] = 0xFF333333;
      }
    }
  }
}

void draw_pixel(int x, int y, uint32_t color) {
  if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
    color_buffer[(window_width * y) + x] = color;
  }
}

void draw_rectangle(int mx, int my, int width, int height, uint32_t color) {
   // inefficient
  //  for (int y = 0; y < window_height; y += 1) {
  //   for (int x = 0; x < window_width; x += 1) {
  //     // find mx and my. keep drawing until width 
  //     // then next row, find mx again keep drawing until width
  //     // row will be limited by y 
  //     if (y >= my && x >= mx && x <= width && y <= height) {
  //       color_buffer[(window_width * y) + x] = color;
  //     }
  //   }
  // }
  // efficient
  for (int y = my; y < my + height; y++) {
    for (int x = mx; x < mx + width; x++) {
      draw_pixel(x, y, color);
    }
  }
}