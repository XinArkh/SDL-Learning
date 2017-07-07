#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("cartoon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* image = SDL_LoadBMP("walk.bmp");
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 237, 28, 36));
	SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, image);
	
	int i;
	int imgWidth  = image->w / 5;
	int imgHeight = image->h;
	
	SDL_Rect position;
	position.x = 0;
	position.y = 0;
	position.w = imgWidth;
	position.h = imgHeight; 
	
	SDL_Rect clips[5];
	for(i = 0; i < 5; i++){
		clips[i].x = i * imgWidth;
		clips[i].y = 0;
		clips[i].w = imgWidth;
		clips[i].h = imgHeight;
	}
	
	bool quit = false;
	SDL_Event event;
	i = 0;
	while(quit == false){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				quit = true;
			}
		}
		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, texture, &clips[i], &position);
		SDL_RenderPresent(rend);
		i = (i + 1) % 5;
		position.x += 5;
		position.y += 3;
		SDL_Delay(150); 
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
