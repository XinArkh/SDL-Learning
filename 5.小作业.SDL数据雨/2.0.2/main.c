//C语言模拟数据雨-2.0.1
//2016.8.8

//SDL版本： SDL2-2.0.4	SDL2_image-2.0.2

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h> 

#define WINDOW_WIDTH 800		//窗口宽度 
#define WINDOW_HEIGHT 600		//窗口高度 
#define OPICITY 25				//不透明度 
#define DATA_LINE 30			//数据行数 
#define DATA_ROW 10				//数据列数 
#define STEP_LENGTH 17			//数据步长 
#define FLOW_SPEED 80			//数据流速 

//定义数据流元素结构体 
typedef struct{
	int x;
	int y;
	int w;
	int h;
	int speed;
	int alpha;
}Flowelement;

//画图函数 
void drawFlowelement( SDL_Renderer* rend, SDL_Texture* data, SDL_Rect* clip, Flowelement* element )
{
	SDL_Rect rect;
	rect.w = element->w;
	rect.h = element->h;
	rect.x = element->x;
	rect.y = element->y;
	SDL_SetTextureAlphaMod( data, element->alpha );
	SDL_RenderCopy( rend, data, clip, &rect );
}

//移动函数 
void moveFlowelement( Flowelement* element )
{
	element->y += element->speed;
	if( element->y >= WINDOW_HEIGHT ){
		element->y = 0;
	}
}

int main(int argc, char *argv[])
{
	SDL_Init( SDL_INIT_VIDEO );		//初始化 
	SDL_Window *window = SDL_CreateWindow( "Matrix", 
											SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
											WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );		//创建窗口 
	SDL_Renderer* rend = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );		//创建渲染器 
	SDL_Surface* datasurface  = IMG_Load( "pic\\data.png" );		//读取数据图片 
	SDL_Surface* blacksurface = SDL_LoadBMP( "pic\\black.bmp" );	//读取黑色蒙版图片 
	SDL_SetColorKey( datasurface, SDL_TRUE, SDL_MapRGB( datasurface->format, 255, 255, 255 ) );		//去除数据背景 
	SDL_Texture* data  = SDL_CreateTextureFromSurface( rend, datasurface );		//创建数据纹理 
	SDL_Texture* black = SDL_CreateTextureFromSurface( rend, blacksurface );	//创建黑色蒙版纹理 
	SDL_SetTextureBlendMode( data, SDL_BLENDMODE_BLEND );		//设置数据
	SDL_SetTextureBlendMode( black, SDL_BLENDMODE_BLEND );		//设置黑色蒙版 
	SDL_SetTextureAlphaMod( black, OPICITY );					//设置黑色蒙版不透明度 
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);		// 初始化SDL_mixer 
	Mix_Music* backgroundmus = Mix_LoadMUS("Clubbed To Death.mp3");
	Mix_PlayMusic(backgroundmus, -1);		//第二个参数是-1时音乐一直循环 
	

	int i, j;		//循环标记 
	int imgWidth  = datasurface->w / DATA_ROW;		//单个数据宽度 
	int imgHeight = datasurface->h / DATA_LINE;		//单个数据高度
	int winWidth  = WINDOW_WIDTH / DATA_ROW;		//分屏宽度 
	
	//黑色蒙版矩形 
	SDL_Rect rectWindow;
	rectWindow.w = WINDOW_WIDTH;
	rectWindow.h = WINDOW_HEIGHT;
	rectWindow.x = 0;
	rectWindow.y = 0;
	
	//数据分割矩形 
	SDL_Rect clips[DATA_ROW][DATA_LINE];
	for( i = 0; i < DATA_ROW; i++ ){
		for( j = 0; j < DATA_LINE; j++ ){
			clips[i][j].w = imgWidth;
			clips[i][j].h = imgHeight;
			clips[i][j].x = i * imgWidth;
			clips[i][j].y = j * imgHeight;
		}	
	}
	
	//创建数据流元素结构体 
	Flowelement element[DATA_ROW];
	srand(time(NULL)); 
	for( i = 0; i < DATA_ROW; i++ ){
		element[i].x = i * winWidth;
		element[i].y = rand() % WINDOW_HEIGHT;
		element[i].w = imgWidth;
		element[i].h = imgHeight;
		element[i].speed = STEP_LENGTH;
		element[i].alpha = rand() % 255 + 1;
	}
	
	bool quit = false;		//退出标记 
	SDL_Event event;		//创建事件 
	i = j = 0;
	//循环体 
	while( quit == false ){
		//事件反馈 
		while( SDL_PollEvent( &event ) ){
			//退出循环 
			if( event.type == SDL_QUIT ){
				quit = true;
			}
		}
		SDL_RenderCopy( rend, black, NULL, &rectWindow );		//覆盖黑色蒙版 
		//数据流 
		for( i = 0; i < DATA_ROW; i++ ){
			drawFlowelement( rend, data, &clips[i][j], &element[i] );
			drawFlowelement( rend, data, &clips[i][j], &element[i] );
			drawFlowelement( rend, data, &clips[i][j], &element[i] );
			drawFlowelement( rend, data, &clips[i][j], &element[i] );
			moveFlowelement( &element[i] );
		}
		j = ( j + 1 ) % DATA_LINE;
		SDL_RenderPresent( rend );		//显示画面 
		SDL_Delay( FLOW_SPEED );		//控制流速 
	} 
	
//	Mix_FadeOutMusic(1000); 
//	SDL_Delay( 2000 );

	//释放图像 
	SDL_FreeSurface( datasurface );	
	datasurface = NULL; 
	SDL_FreeSurface( blacksurface );
	blacksurface = NULL; 
	SDL_DestroyWindow( window );		//销毁窗口 
	SDL_Quit();							//退出系统 
	
	return 0;
}
