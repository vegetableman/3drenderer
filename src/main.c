// This is from Pikuma's - learn computer grahics programming.

#include "display.h"
#include "vector.h"

// Declare an array of vectors/points

const int N_POINTS = 9 * 9 * 9;
vec3_t cube_points[N_POINTS]; //9x9x9 cube

int point_count = 0;

bool is_running = false;

void setup(void) {
  // malloc allocates memory based on the data type provided to hold the color buffer.
  color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
  // Texture is used to display color buffer.
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

  // Start loading my array of vectors
  // From -1 to 1 (in this 9x9x9 cube)
  for (float x = -1; x <= 1; x *= 0.25) {
    for (float y = -1;  y <= 1; y += 0.25) {
      for (float z = -1; z <= 1; z += 0.25) {
        vec3_t new_point = { .x = x, .y = y, .z = z };
        // array is not dynamic in c
        cube_points[point_count++] = new_point;
      }
    }
  }
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

void render(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  draw_grid();

  // draw_rectangle(100, 100, 500, 500, 0xFFFF0000);
  draw_pixel(20, 30, 0xFFFF0000);

  // render should come before clear
  render_color_buffer();
  clear_color_buffer(0xFF000000);

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