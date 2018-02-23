#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("change moothly", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);	
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* girlsurface = SDL_LoadBMP("girl.bmp");
	SDL_Texture* girl = SDL_CreateTextureFromSurface(rend, girlsurface);
	
	SDL_SetTextureBlendMode(girl, SDL_BLENDMODE_BLEND);
	
	SDL_Rect rect;
	rect.w = girlsurface->w;
	rect.h = girlsurface->h;
	rect.x = 50;
	rect.y = 50;
	
	bool change = true;
	int alpha = 255;
	
	SDL_Event event;
	bool quit = false;
	while(quit == false){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				quit = true;
			}
		}
		SDL_RenderClear(rend);
		SDL_SetTextureAlphaMod(girl, alpha);
		SDL_RenderCopy(rend, girl, NULL, &rect);
		SDL_RenderPresent(rend);
		
		if(change == true){
			alpha--;
			if(alpha == 0){
				change = false;
			}
		}
		else{
			alpha++;
			if(alpha == 255){
				change = true;
			}
		}
		SDL_Delay(5);
	}
	
	SDL_FreeSurface(girlsurface);
	SDL_DestroyWindow(window);
	SDL_Quit();	
	
	return 0;
}
