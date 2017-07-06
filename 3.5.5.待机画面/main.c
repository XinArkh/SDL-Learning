#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

bool moveright = true;
bool movedown = true;

void updatePosition(SDL_Rect* prect){		
		if(moveright == true){
			prect->x++;
			if(prect->x + prect->w >=WINDOW_WIDTH){
				moveright = false;
			}
		}
		else{
			prect->x--;
			if(prect->x <=0){
				moveright = true;
			}
		}
		
		if(movedown == true){
			prect->y++;
			if(prect->y + prect->h >=WINDOW_HEIGHT){
				movedown = false;
			}
		}
		else{
			prect->y--;
			if(prect->y <=0){
				movedown = true;
			}
		}		
		
}


int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("mousemotion-renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);	

	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(rend);
	SDL_Surface* bikesurface = SDL_LoadBMP("bike.bmp");
	SDL_Texture* bike = SDL_CreateTextureFromSurface(rend, bikesurface);
	
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = bikesurface->w;
	rect.h = bikesurface->h; 
	
	int flag = 0;
	SDL_Event event;
	bool quit = false;
	while(quit == false){
		while(SDL_PollEvent(&event)){
			if((flag==1)&&(event.type == SDL_QUIT || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYDOWN || event.type == SDL_MOUSEMOTION)){
				quit = true;
			}
			if(event.type == SDL_MOUSEMOTION){
				flag = 1; 
			}
		}
		updatePosition(&rect);
		SDL_Delay(5);
		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, bike, NULL, &rect);
		SDL_RenderPresent(rend); 
	}
	
	SDL_FreeSurface(bikesurface);
	SDL_DestroyWindow(window);
	SDL_Quit();	
	
	return 0;
}
