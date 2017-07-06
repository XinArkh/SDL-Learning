#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("mousemotion-renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);	
//��renderer��������	SDL_Surface* surface = SDL_GetWindowSurface(window);
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(rend);
	
	SDL_Surface* bikesurface = SDL_LoadBMP("bike.bmp");
//���ܼ򵥵ذ�surface��ʽ��ͼƬ������Ļ��	SDL_BlitSurface(bike, NULL, surface, NULL);
//��texture������surface��ʽת��texture��ʽ 
	SDL_Texture* bike = SDL_CreateTextureFromSurface(rend, bikesurface);
	
	SDL_Rect rect;
//	�����������γ���
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
//��renderclear��������				SDL_FillRect(surface, NULL, 0);
//��rendercopy��������				SDL_BlitSurface(bike, NULL, surface, &rect);
				SDL_RenderClear(rend);
				SDL_RenderCopy(rend, bike, NULL, &rect); 
//��renderpresent��������				SDL_UpdateWindowSurface(window);	
			}
		}
//��delay��renderpresent�����ŵ��ڶ���whileѭ���ڣ�����if�����ڣ���ʹ�������л��� 
		SDL_Delay(5);  /*������ʱ������ֹCPU���ɹ���*/
		SDL_RenderPresent(rend); 
	}
	
	SDL_FreeSurface(bikesurface);
//	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();	
	
	return 0;
}
