/*
C语言躲避方块2.0.3
2016.8.18-
SDL版本：SDL2-2.0.4 SDL2_image-2.0.1 SDL2_mixer-2.0.1 SDL2_ttf-2.0.14
w.x. 
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#define WINDOW_WIDTH 700		//窗口宽度 
#define WINDOW_HEIGHT 720		//窗口高度 
#define RECTNUM 4				//方块数量 
#define SPEED 1					//初始速度 
#define ACCLERATE 1				//速度增量
#define UPDATESPEED 500			//加速间隔 

//定义方块移动结构体
typedef struct{
	SDL_Rect rect;
	int speed;
	int acclerate;
}Move;

//更新方块位置 
void updatePosition(Move* pmove,  int* moveright, int* movedown){		
		if(*moveright == 1){
			pmove->rect.x += pmove->speed;
			if(pmove->rect.x + pmove->rect.w >= WINDOW_WIDTH){
				*moveright = 0;
			}
		}
		else{
			pmove->rect.x -= pmove->speed;
			if(pmove->rect.x <= 0){
				*moveright = 1;
			}
		}
		
		if(*movedown == 1){
			pmove->rect.y += pmove->speed;
			if(pmove->rect.y + pmove->rect.h >= WINDOW_HEIGHT){
				*movedown = 0;
			}
		}
		else{
			pmove->rect.y -= pmove->speed;
			if(pmove->rect.y <= 0){
				*movedown = 1;
			}
		}
}

//判断是否失败，失败返回1，未失败返回0 
int judgePosition(SDL_Rect* prect, SDL_Rect* pgamer)
{
	int flag = 0;
	if(pgamer->x < 0 || pgamer->x + pgamer->w > WINDOW_WIDTH || pgamer->y < 0 || pgamer->y + pgamer->h > WINDOW_HEIGHT){
		flag = 1;
	}
	if(pgamer->x + pgamer->w > prect->x && pgamer->x < prect->x + prect->w){
		if(pgamer->y + pgamer->h > prect->y && pgamer->y < prect->y + prect->h){
			flag = 1;
		}
	}
	return flag;
}

int main(int argc, char** argv)
{
	//游戏大循环标记 
	bool bigboss = false;
	while(bigboss == false){
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_Window* window = SDL_CreateWindow("Escape rectangle",
								SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		
	
		
		SDL_Surface* bikesurface    = SDL_LoadBMP("bike.bmp");
		SDL_Surface* pikaqiusurface = SDL_LoadBMP("pikaqiu.bmp");
		SDL_Surface* backgroundsurface = IMG_Load("background.jpeg");
		SDL_Texture* bike = SDL_CreateTextureFromSurface(rend, bikesurface);
		SDL_Texture* pikaqiu = SDL_CreateTextureFromSurface(rend, pikaqiusurface);
		SDL_Texture* background = SDL_CreateTextureFromSurface(rend, backgroundsurface);
		
		SDL_RenderClear(rend);
		
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		Mix_Music* bgmus = Mix_LoadMUS("BGM.mp3");
		Mix_PlayMusic(bgmus, -1);
		
		double totaltime;
		TTF_Init();
		TTF_Font* font1 = TTF_OpenFont("C:\\Windows\\Fonts\\simhei.ttf", 16);
		TTF_Font* font2 = TTF_OpenFont("C:\\Windows\\Fonts\\simhei.ttf", 25);
	    SDL_Color white = {255, 255, 255};
				
		//背景图片矩形 
		SDL_Rect bgrect;
		bgrect.w = backgroundsurface->w;
		bgrect.h = backgroundsurface->h;
		bgrect.x = 0;
		bgrect.y = 0;
	
		//玩家图片矩形 
		SDL_Rect gamer;
		gamer.w = pikaqiusurface->w / 2;
		gamer.h = pikaqiusurface->h / 2;
		gamer.x = WINDOW_WIDTH / 2 - gamer.w / 2;
		gamer.y = WINDOW_HEIGHT / 2 - gamer.h / 2;
		
		//初始化方块组 
		SDL_Rect rect[RECTNUM];
		int i;
		srand(time(NULL)); 
		for(i = 0; i < RECTNUM; i++){
			rect[i].w = bikesurface->w / (rand() % 5 + 1);
			rect[i].h = bikesurface->h / (rand() % 5 + 1);
			rect[i].x = rand() % (WINDOW_WIDTH  - rect[i].w);
			rect[i].y = rand() % (WINDOW_HEIGHT - rect[i].h);
			while(judgePosition(&rect[i], &gamer)){
				rect[i].x = rand() % (WINDOW_WIDTH  - rect[i].w);
				rect[i].y = rand() % (WINDOW_HEIGHT - rect[i].h);
			}
		}
		
		//方块组移动结构体
		Move move[RECTNUM];
		for(i = 0; i < RECTNUM; i++){
			move[i].rect = rect[i];
			move[i].speed = SPEED;
			move[i].acclerate = ACCLERATE;
		}
	
		//初始化方块移动方向 
		int* moveright[RECTNUM];
		int* movedown[RECTNUM];
		for(i = 0; i < RECTNUM; i++){
			moveright[i] = (int*)malloc(sizeof(int));
			movedown[i]  = (int*)malloc(sizeof(int));
			*moveright[i] = rand() % 2;
			*movedown[i]  = rand() % 2;
		}
			
		bool quit = false;		//退出循环标记 
		bool begin = false;		//开始游戏标记 
		bool control = false; 	//鼠标控制标记 
		Uint32 starttime = 0;	//开始游戏时间 
		int count = 0;			//加速标记 
		SDL_Event event;		//创建事件
		 
		while(quit == false){
			while(SDL_PollEvent(&event)){
				//退出所有循环
				if(event.type == SDL_QUIT){
					quit = true;
					bigboss = true;	
					Mix_FadeOutMusic(10);
				}
				//鼠标在玩家矩形区域内按下
				if(event.type == SDL_MOUSEBUTTONDOWN){
					if(event.motion.x >= gamer.x && event.motion.x <= gamer.x + gamer.w && 
						event.motion.y >= gamer.y && event.motion.y <= gamer.y + gamer.h){
							begin = true;		//开始游戏标记 
							control = true;		//控制开关标记 
							//记录开始游戏时间 
							if(starttime == 0){
								starttime = SDL_GetTicks();
							}
					}
				}
				//鼠标在玩家矩形区域内松开时关闭控制开关 
				else if(event.type == SDL_MOUSEBUTTONUP){
					control = false;
				}
				//控制开关开启，更新玩家矩形位置 
				if(control == true){
					gamer.x = event.motion.x - gamer.w / 2;
					gamer.y = event.motion.y - gamer.h / 2;	
				}
			}
			//游戏开始后方块持续移动 
			if(begin == true){
				//更新方块位置 
				for(i = 0; i < RECTNUM; i++){
					updatePosition(&move[i], moveright[i], movedown[i]);
					if(count == UPDATESPEED){
						move[i].speed += move[i].acclerate;
					}
				}
				count = count % UPDATESPEED + 1;
				
				SDL_RenderClear(rend);		//清空渲染器 
				SDL_RenderCopy(rend, background, NULL, &bgrect);	//生成背景纹理 
				SDL_RenderCopy(rend, pikaqiu, NULL, &gamer);		//生成玩家图片纹理 
							
				//生成方块纹理+判断是否失败 
				for(i = 0; i < RECTNUM; i++){
					SDL_RenderCopy(rend, bike, NULL, &move[i].rect);
					//失败
					if(judgePosition(&move[i].rect, &gamer)){
						quit = true;		//失败标记 
						Mix_FadeOutMusic(1000);
					}
				}
				if(quit == false){
					//显示持续时间
					char gameovertext_in[100];
					totaltime = (SDL_GetTicks() - starttime) / 1000.0;
					sprintf(gameovertext_in, "/****You have survived for %.3lfs!****/", totaltime);
					SDL_Surface* timetextsurface_in = TTF_RenderText_Blended(font1, gameovertext_in, white);  
					SDL_Texture* timetext_in = SDL_CreateTextureFromSurface(rend, timetextsurface_in);
					SDL_Rect textzone_in;
					textzone_in.w = timetextsurface_in->w;
					textzone_in.h = timetextsurface_in->h;
					textzone_in.x = 10;
					textzone_in.y = 40;
					SDL_RenderCopy(rend, timetext_in, NULL, &textzone_in);
				}			
			}
			//游戏未开始，所有图像静止 
			else{
				SDL_RenderCopy(rend, background, NULL, &bgrect);		//生成背景纹理 
				SDL_RenderCopy(rend, pikaqiu, NULL, &gamer);			//生成玩家图片纹理 
				//生成方块纹理
				for(i = 0; i < RECTNUM; i++){
					SDL_RenderCopy(rend, bike, NULL, &move[i].rect);
				}
			}
			
			SDL_RenderPresent(rend);		//显示渲染器纹理 
			SDL_Delay(10);					//延时 
		}
			
		//显示总共持续时间+重新开始按钮
		char gameovertext_out[100];
		totaltime = (SDL_GetTicks() - starttime) / 1000.0;
		sprintf(gameovertext_out, "/****Game Over! You have survived for %.3lfs!****/", totaltime);
	
		TTF_Init();
	    SDL_Surface* timetextsurface_out = TTF_RenderText_Blended(font1, gameovertext_out, white);  
	    SDL_Surface* resumesurface = TTF_RenderText_Blended(font2, "resume", white);  
	    SDL_Texture* timetext = SDL_CreateTextureFromSurface(rend, timetextsurface_out);
	    SDL_Texture* resume   = SDL_CreateTextureFromSurface(rend, resumesurface);
		//失败及时间信息矩形 
		SDL_Rect textzone_out;
		textzone_out.w = timetextsurface_out->w;
		textzone_out.h = timetextsurface_out->h;
		textzone_out.x = 10;
		textzone_out.y = 40;
		//重新开始矩形 
		SDL_Rect resumezone;
		resumezone.w = resumesurface->w;
		resumezone.h = resumesurface->h;
		resumezone.x = 150;
		resumezone.y = 73;
		
		SDL_RenderCopy(rend, timetext, NULL, &textzone_out);
		SDL_RenderCopy(rend, resume, NULL, &resumezone);
		SDL_RenderPresent(rend);
		
		//退出及重新开始游戏 
		if(bigboss == false){
			quit = false;
			while(quit == false){
				while(SDL_PollEvent(&event)){
					//退出所有循环 
					if(event.type == SDL_QUIT){
						quit = true;
						bigboss = true;
					}
					//重新开始游戏 
					else if(event.type == SDL_MOUSEBUTTONDOWN){
						if(event.motion.x >= resumezone.x && event.motion.x <= resumezone.x + resumezone.w && 
								event.motion.y >= resumezone.y && event.motion.y <= resumezone.y + resumezone.h){
								quit = true;
								bigboss = false;
								}
					}
				}
				SDL_Delay(10);
			}
		}

		//释放内存+销毁窗口 
		for(i = 0; i < RECTNUM; i++){
			free(moveright[i]);
			free(movedown[i]);
		}
		SDL_FreeSurface(bikesurface);
		SDL_FreeSurface(pikaqiusurface);
		SDL_FreeSurface(backgroundsurface);
		SDL_DestroyWindow(window);
//		SDL_Quit();			//这里有一个玄学问题，加上这一句之后第一次重新开始游戏没有音乐，第二次程序崩溃 
	}
}
