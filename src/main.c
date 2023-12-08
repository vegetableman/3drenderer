// This is from Pikuma's - learn computer grahics programming.

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool is_running = false;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Used to display color buffer
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
  return true;
}

void setup(void) {
  // malloc allocates memory based on the data type provided to hold the color buffer.
  color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
  // Texture is used to display color buffer.
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
}

void process_input(void)
{
  SDL_Event event;
  // poll for input
  SDL_PollEvent(&event);
  switch (event.type) {
    case SDL_QUIT:
      is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        is_running = false;
      break;
    default:
      break;
  }
}

void update(void) {
  // TODO
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

void render(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // on every render, clear the color buffer.
  clear_color_buffer(0xFFFFFF00);
  render_color_buffer();

  SDL_RenderPresent(renderer);
}

int main(void) {
  is_running = initialize_window();

  setup();

  // event loop
  while(is_running) {
    process_input();
    update();
    render();
  }

  return 0;
}