#include <stdlib.h> 
#include <stdbool.h>
#include <SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define DOT_AMOUNT 300

typedef struct{
	int x;
	int y;
	int d;
	int speed;
	int alpha;
}Spot;

void drawSpot(Spot* spot, SDL_Renderer* rend, SDL_Texture* dot)
{
	SDL_Rect rect;
	rect.w = spot->d;
	rect.h = spot->d;
	rect.x = spot->x;
	rect.y = spot->y;
	SDL_SetTextureAlphaMod(dot, spot->alpha);
	SDL_RenderCopy(rend, dot, NULL, &rect);
}

void moveSpot(Spot* spot)
{
	spot->y += spot->speed;
	if(spot->y >= WINDOW_HEIGHT){
		spot->y = 0;
	}
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("name",
							SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* blacksurface = SDL_LoadBMP("black.bmp");
	SDL_Surface* dotsurface   = SDL_LoadBMP("dot.bmp");
	SDL_SetColorKey(dotsurface, SDL_TRUE, SDL_MapRGB(dotsurface->format, 195, 195, 195));
	
	SDL_Texture* black = SDL_CreateTextureFromSurface(rend, blacksurface);
	SDL_Texture* dot   = SDL_CreateTextureFromSurface(rend, dotsurface);
	SDL_SetTextureBlendMode(black, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(dot, SDL_BLENDMODE_BLEND);
	
	SDL_SetTextureAlphaMod(black, 50);
	SDL_SetTextureColorMod(dot, 0, 128, 0);
	
	SDL_Rect rectWindow;
	rectWindow.w = WINDOW_WIDTH;
	rectWindow.h = WINDOW_HEIGHT;
	rectWindow.x = 0;
	rectWindow.y = 0;
	
	Spot spots[DOT_AMOUNT];
	int i;
	for(i = 0; i < DOT_AMOUNT; i++){
		spots[i].x = rand() % WINDOW_WIDTH;
		spots[i].y = rand() % WINDOW_HEIGHT;
		spots[i].d = rand() % 4 + 1;
		spots[i].speed = rand() % 2 + 1;
		spots[i].alpha = rand() % 255 + 1;
	}
	
	bool quit = false;
	SDL_Event event;
	while(quit == false){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				quit = true;
			}
		}
		SDL_RenderCopy(rend, black, NULL, &rectWindow);
		for(i = 0; i < DOT_AMOUNT; i++){
			drawSpot(&spots[i], rend, dot);
			moveSpot(&spots[i]);
		}
		SDL_RenderPresent(rend); 
		SDL_Delay(10);
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();	
	
	return 0;
}
