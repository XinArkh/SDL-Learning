#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("illusion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);	
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* batsurface = SDL_LoadBMP("batman.bmp");
	SDL_Surface* blacksurface = SDL_LoadBMP("black.bmp");
	SDL_Texture* batman = SDL_CreateTextureFromSurface(rend, batsurface);
	SDL_Texture* black = SDL_CreateTextureFromSurface(rend, blacksurface);
	
	SDL_SetTextureBlendMode(black, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(black, 20);
	
	SDL_Rect rect;
	rect.w = batsurface->w;
	rect.h = batsurface->h;
	
	SDL_Rect rectWindow;
	rectWindow.w = 800;
	rectWindow.h = 600;
	rectWindow.x = 0;
	rectWindow.y = 0;
	
	SDL_Event event;
	bool quit = false;
	while(quit == false){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				quit = true;
			}
			else if(event.type == SDL_MOUSEMOTION){
				rect.x = event.motion.x - rect.w / 2;
				rect.y = event.motion.y - rect.h / 2;
			}
		}
		SDL_RenderCopy(rend, black, NULL, &rectWindow);
		SDL_RenderCopy(rend, batman, NULL, &rect);
		SDL_RenderPresent(rend);
		
		SDL_Delay(5);
	}
	
	SDL_FreeSurface(batsurface);
	SDL_FreeSurface(blacksurface);
	SDL_DestroyWindow(window);
	SDL_Quit();	
	
	return 0;
}
