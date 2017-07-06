#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("mousemotion-renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);	
//用renderer函数代替	SDL_Surface* surface = SDL_GetWindowSurface(window);
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(rend);
	
	SDL_Surface* bikesurface = SDL_LoadBMP("bike.bmp");
//不能简单地把surface格式的图片贴到屏幕上	SDL_BlitSurface(bike, NULL, surface, NULL);
//用texture函数把surface格式转成texture格式 
	SDL_Texture* bike = SDL_CreateTextureFromSurface(rend, bikesurface);
	
	SDL_Rect rect;
//	必须声明矩形长宽
	rect.w = bikesurface->w;
	rect.h = bikesurface->h; 
	
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
//用renderclear函数代替				SDL_FillRect(surface, NULL, 0);
//用rendercopy函数代替				SDL_BlitSurface(bike, NULL, surface, &rect);
				SDL_RenderClear(rend);
				SDL_RenderCopy(rend, bike, NULL, &rect); 
//用renderpresent函数代替				SDL_UpdateWindowSurface(window);	
			}
		}
//将delay和renderpresent函数放到第二个while循环内（包括if函数内）会使程序运行缓慢 
		SDL_Delay(5);  /*设置延时函数防止CPU负荷过大*/
		SDL_RenderPresent(rend); 
	}
	
	SDL_FreeSurface(bikesurface);
//	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();	
	
	return 0;
}
