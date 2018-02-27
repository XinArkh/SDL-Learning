#include <stdlib.h> 
#include <stdbool.h>
#include <SDL.h>



int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("name",
							SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);	//设置透明度开关（有了这个函数下一行函数的最后一个参数才可用） 
	SDL_SetRenderDrawColor(rend, 0, 128, 0, 255);	//设置颜色 
	SDL_RenderDrawPoint(rend, 100, 100);			//画点 
	
	SDL_Rect rect;
	rect.w = 100;
	rect.h = 100;
	rect.x = 0;
	rect.y = 0;
	
	SDL_RenderDrawRect(rend, &rect);	//用 SDL_RenderFillRect(rend, &rect); 填充矩形 
	
	int x1 = 0;
	int y1 = 0; 
	
	bool quit = false;
	bool state = false;
	SDL_Event event;
	while(quit == false){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				quit = true;
			}
			else if(event.type == SDL_MOUSEMOTION){
				if(state == true){
					int x = event.motion.x;
					int y = event.motion.y;
					SDL_RenderDrawLine(rend, x1, y1, x, y);
					x1 = x;
					y1 = y;
				}
			}
			if(event.type == SDL_MOUSEBUTTONDOWN){
				state = true;
				x1 = event.motion.x;
				y1 = event.motion.y;
			}
			if(event.type == SDL_MOUSEBUTTONUP){
				state = false;
			}
		}
		SDL_RenderPresent(rend);
		SDL_Delay(10);
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();	
	
	return 0;
}
