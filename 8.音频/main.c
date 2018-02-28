#include <stdlib.h> 
#include <stdbool.h>
#include <SDL.h>
#include <SDL_mixer.h> 

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("name",
							SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	SDL_RenderClear(rend);
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);		// ≥ı ºªØSDL_mixer 
	Mix_Music* sound1 = Mix_LoadMUS("sword.wav");
	Mix_Music* sound2 = Mix_LoadMUS("injured.wav");
	
	bool quit = false;
	SDL_Event event;
	while(quit == false){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				quit = true;
			}
			else if(event.type == SDL_MOUSEBUTTONDOWN){
				if(event.button.button == SDL_BUTTON_LEFT){
					Mix_PlayMusic(sound1, 1);
				}
				else if(event.button.button == SDL_BUTTON_RIGHT){
					Mix_PlayMusic(sound2, 1);
				}
			}
		}
		SDL_RenderPresent(rend);
		SDL_Delay(10);
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();	
	
	return 0;
}
