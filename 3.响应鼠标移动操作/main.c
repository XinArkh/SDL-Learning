#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("mousemotion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);	
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	SDL_Surface* bike = SDL_LoadBMP("bike.bmp");
	SDL_BlitSurface(bike, NULL, surface, NULL);
	
	SDL_Rect rect;
	SDL_Event event;
	bool quit = false;
	while(quit == false){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				quit = true;
			}
			else if(event.type == SDL_MOUSEMOTION){
				rect.x = event.motion.x - bike->w / 2;
				rect.y = event.motion.y - bike->h / 2;
				SDL_FillRect(surface, NULL, 0);
				SDL_BlitSurface(bike, NULL, surface, &rect);
			}
			SDL_UpdateWindowSurface(window);
		}
	}
	
	SDL_FreeSurface(bike);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();	
	
	return 0;
}
