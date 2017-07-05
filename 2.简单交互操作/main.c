#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>


int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("IO Experiment", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

	SDL_Surface* surface = SDL_GetWindowSurface(window);
	
	SDL_Surface* face1 = SDL_LoadBMP("pikaqiu.bmp");
	SDL_Surface* face2 = SDL_LoadBMP("duolaameng.bmp");
	SDL_Surface* currentface = face1;
	SDL_BlitSurface(currentface, NULL, surface, NULL);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	
	SDL_Event event;
	bool quit = false;
	while(quit == false){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT: quit = true; break;
				case SDL_MOUSEBUTTONDOWN: 
					if(event.button.button == SDL_BUTTON_LEFT){
						currentface = face1;
						SDL_BlitSurface(currentface, NULL, surface, &rect);
					}
					else if(event.button.button == SDL_BUTTON_RIGHT){
						currentface = face2;
						SDL_BlitSurface(currentface, NULL, surface, &rect);
					}
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_UP: rect.y-=10; break;
						case SDLK_DOWN: rect.y+=10; break;
						case SDLK_LEFT: rect.x-=10; break;
						case SDLK_RIGHT: rect.x+=10; break; 
					}
					SDL_FillRect(surface, NULL, 0);
					SDL_BlitSurface(currentface, NULL, surface, &rect);
					break;
			}
			
			SDL_UpdateWindowSurface(window);
		}
	}
	
	SDL_FreeSurface(face1);
	SDL_FreeSurface(face2);
	SDL_FreeSurface(currentface);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

