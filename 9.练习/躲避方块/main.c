/*
C���Զ�ܷ���2.0.3
2016.8.18-
SDL�汾��SDL2-2.0.4 SDL2_image-2.0.1 SDL2_mixer-2.0.1 SDL2_ttf-2.0.14
w.x. 
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#define WINDOW_WIDTH 700		//���ڿ�� 
#define WINDOW_HEIGHT 720		//���ڸ߶� 
#define RECTNUM 4				//�������� 
#define SPEED 1					//��ʼ�ٶ� 
#define ACCLERATE 1				//�ٶ�����
#define UPDATESPEED 500			//���ټ�� 

//���巽���ƶ��ṹ��
typedef struct{
	SDL_Rect rect;
	int speed;
	int acclerate;
}Move;

//���·���λ�� 
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

//�ж��Ƿ�ʧ�ܣ�ʧ�ܷ���1��δʧ�ܷ���0 
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
	//��Ϸ��ѭ����� 
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
				
		//����ͼƬ���� 
		SDL_Rect bgrect;
		bgrect.w = backgroundsurface->w;
		bgrect.h = backgroundsurface->h;
		bgrect.x = 0;
		bgrect.y = 0;
	
		//���ͼƬ���� 
		SDL_Rect gamer;
		gamer.w = pikaqiusurface->w / 2;
		gamer.h = pikaqiusurface->h / 2;
		gamer.x = WINDOW_WIDTH / 2 - gamer.w / 2;
		gamer.y = WINDOW_HEIGHT / 2 - gamer.h / 2;
		
		//��ʼ�������� 
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
		
		//�������ƶ��ṹ��
		Move move[RECTNUM];
		for(i = 0; i < RECTNUM; i++){
			move[i].rect = rect[i];
			move[i].speed = SPEED;
			move[i].acclerate = ACCLERATE;
		}
	
		//��ʼ�������ƶ����� 
		int* moveright[RECTNUM];
		int* movedown[RECTNUM];
		for(i = 0; i < RECTNUM; i++){
			moveright[i] = (int*)malloc(sizeof(int));
			movedown[i]  = (int*)malloc(sizeof(int));
			*moveright[i] = rand() % 2;
			*movedown[i]  = rand() % 2;
		}
			
		bool quit = false;		//�˳�ѭ����� 
		bool begin = false;		//��ʼ��Ϸ��� 
		bool control = false; 	//�����Ʊ�� 
		Uint32 starttime = 0;	//��ʼ��Ϸʱ�� 
		int count = 0;			//���ٱ�� 
		SDL_Event event;		//�����¼�
		 
		while(quit == false){
			while(SDL_PollEvent(&event)){
				//�˳�����ѭ��
				if(event.type == SDL_QUIT){
					quit = true;
					bigboss = true;	
					Mix_FadeOutMusic(10);
				}
				//�������Ҿ��������ڰ���
				if(event.type == SDL_MOUSEBUTTONDOWN){
					if(event.motion.x >= gamer.x && event.motion.x <= gamer.x + gamer.w && 
						event.motion.y >= gamer.y && event.motion.y <= gamer.y + gamer.h){
							begin = true;		//��ʼ��Ϸ��� 
							control = true;		//���ƿ��ر�� 
							//��¼��ʼ��Ϸʱ�� 
							if(starttime == 0){
								starttime = SDL_GetTicks();
							}
					}
				}
				//�������Ҿ����������ɿ�ʱ�رտ��ƿ��� 
				else if(event.type == SDL_MOUSEBUTTONUP){
					control = false;
				}
				//���ƿ��ؿ�����������Ҿ���λ�� 
				if(control == true){
					gamer.x = event.motion.x - gamer.w / 2;
					gamer.y = event.motion.y - gamer.h / 2;	
				}
			}
			//��Ϸ��ʼ�󷽿�����ƶ� 
			if(begin == true){
				//���·���λ�� 
				for(i = 0; i < RECTNUM; i++){
					updatePosition(&move[i], moveright[i], movedown[i]);
					if(count == UPDATESPEED){
						move[i].speed += move[i].acclerate;
					}
				}
				count = count % UPDATESPEED + 1;
				
				SDL_RenderClear(rend);		//�����Ⱦ�� 
				SDL_RenderCopy(rend, background, NULL, &bgrect);	//���ɱ������� 
				SDL_RenderCopy(rend, pikaqiu, NULL, &gamer);		//�������ͼƬ���� 
							
				//���ɷ�������+�ж��Ƿ�ʧ�� 
				for(i = 0; i < RECTNUM; i++){
					SDL_RenderCopy(rend, bike, NULL, &move[i].rect);
					//ʧ��
					if(judgePosition(&move[i].rect, &gamer)){
						quit = true;		//ʧ�ܱ�� 
						Mix_FadeOutMusic(1000);
					}
				}
				if(quit == false){
					//��ʾ����ʱ��
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
			//��Ϸδ��ʼ������ͼ��ֹ 
			else{
				SDL_RenderCopy(rend, background, NULL, &bgrect);		//���ɱ������� 
				SDL_RenderCopy(rend, pikaqiu, NULL, &gamer);			//�������ͼƬ���� 
				//���ɷ�������
				for(i = 0; i < RECTNUM; i++){
					SDL_RenderCopy(rend, bike, NULL, &move[i].rect);
				}
			}
			
			SDL_RenderPresent(rend);		//��ʾ��Ⱦ������ 
			SDL_Delay(10);					//��ʱ 
		}
			
		//��ʾ�ܹ�����ʱ��+���¿�ʼ��ť
		char gameovertext_out[100];
		totaltime = (SDL_GetTicks() - starttime) / 1000.0;
		sprintf(gameovertext_out, "/****Game Over! You have survived for %.3lfs!****/", totaltime);
	
		TTF_Init();
	    SDL_Surface* timetextsurface_out = TTF_RenderText_Blended(font1, gameovertext_out, white);  
	    SDL_Surface* resumesurface = TTF_RenderText_Blended(font2, "resume", white);  
	    SDL_Texture* timetext = SDL_CreateTextureFromSurface(rend, timetextsurface_out);
	    SDL_Texture* resume   = SDL_CreateTextureFromSurface(rend, resumesurface);
		//ʧ�ܼ�ʱ����Ϣ���� 
		SDL_Rect textzone_out;
		textzone_out.w = timetextsurface_out->w;
		textzone_out.h = timetextsurface_out->h;
		textzone_out.x = 10;
		textzone_out.y = 40;
		//���¿�ʼ���� 
		SDL_Rect resumezone;
		resumezone.w = resumesurface->w;
		resumezone.h = resumesurface->h;
		resumezone.x = 150;
		resumezone.y = 73;
		
		SDL_RenderCopy(rend, timetext, NULL, &textzone_out);
		SDL_RenderCopy(rend, resume, NULL, &resumezone);
		SDL_RenderPresent(rend);
		
		//�˳������¿�ʼ��Ϸ 
		if(bigboss == false){
			quit = false;
			while(quit == false){
				while(SDL_PollEvent(&event)){
					//�˳�����ѭ�� 
					if(event.type == SDL_QUIT){
						quit = true;
						bigboss = true;
					}
					//���¿�ʼ��Ϸ 
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

		//�ͷ��ڴ�+���ٴ��� 
		for(i = 0; i < RECTNUM; i++){
			free(moveright[i]);
			free(movedown[i]);
		}
		SDL_FreeSurface(bikesurface);
		SDL_FreeSurface(pikaqiusurface);
		SDL_FreeSurface(backgroundsurface);
		SDL_DestroyWindow(window);
//		SDL_Quit();			//������һ����ѧ���⣬������һ��֮���һ�����¿�ʼ��Ϸû�����֣��ڶ��γ������ 
	}
}
