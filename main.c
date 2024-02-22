#include <stdio.h>
#include <stdlib.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>


#define __UNUSED(x) ((void)x)

int debug(const char* format, ...) 
{
    int ret = 0;

	va_list args;
    va_start(args, format);
    ret = printf("INFO: ");
    ret = vprintf(format, args);
    va_end(args);

	return ret;
}

// window size
const int WINDOW_WIDTH 	= 640;
const int WINDOW_HEIGHT = 480;

int main(int argc, char *argv[])
{
    __UNUSED(argc);
    __UNUSED(argv);


	// init SDL
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == -1) {
		debug("SDL initialization failed, %s\n", SDL_GetError());
		return 1;
	}
	debug("SDL initialized.\n");

	// create a window
	SDL_Window *window = SDL_CreateWindow(
			"Games in Memories <3oR>",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN);
	if (window == NULL) {
		debug("Window creation failed, %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	debug("Window creation completed\n");

	// create a renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		debug("Renderer creation failed, %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	debug("Renderer creation completed\n");
	
	// 0:xxx; 1:xxx
	int shape = 0;

	SDL_Event event;
	int quit_flag = 0;
	while (quit_flag == 0) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) 
			{
				case SDL_QUIT:
					quit_flag = 1;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_SPACE:
							shape = (shape + 1) % 2;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}


		// clear renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderClear(renderer);

		// draw a red filled rect
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		const SDL_Rect rect = {.x = 0, .y = 0, .w = 300, .h = 100};
		SDL_RenderFillRect(renderer, &rect);


		SDL_RenderPresent(renderer);
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return 0;
}




