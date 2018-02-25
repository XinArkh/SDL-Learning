//C语言模拟数据雨
//2016.8.4

//SDL版本： SDL2-2.0.4	SDL2_image-2.0.1 

#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#define WINDOW_WIDTH 800		//窗口宽度 
#define WINDOW_HEIGHT 600		//窗口高度 
#define OPICITY 25				//不透明度 
#define DATA_LINE 30			//数据行数 
#define DATA_ROW 10				//数据列数 
#define STEP_LENGTH 17			//数据步长 
#define REFRESH 35				//刷新间隔 
#define FLOW_SPEED 80			//数据流速 

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
	SDL_SetTextureBlendMode( black, SDL_BLENDMODE_BLEND );		//设置黑色蒙版 
	SDL_SetTextureAlphaMod( black, OPICITY );					//设置黑色蒙版不透明度 

	int i, j;		//循环标记 
	int flag;		//刷新标记 
	int imgWidth  = datasurface->w / DATA_ROW;		//单个数据宽度 
	int imgHeight = datasurface->h / DATA_LINE;		//单个数据高度 
	int winWidth  = WINDOW_WIDTH / DATA_ROW;		//分屏宽度 
	
	//黑色蒙版矩形 
	SDL_Rect rectWindow;
	rectWindow.w = WINDOW_WIDTH;
	rectWindow.h = WINDOW_HEIGHT;
	rectWindow.x = 0;
	rectWindow.y = 0;
	
	//数据位置矩形 
	SDL_Rect position[DATA_ROW];
	for( i = 0; i < DATA_ROW; i++ ){
		position[i].w = imgWidth;
		position[i].h = imgHeight; 
		position[i].x = i * winWidth;
		position[i].y = 0;
	}
	
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
	
	bool quit = false;		//退出标记 
	SDL_Event event;		//创建事件 
	i = j = 0;
	flag = 0;
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
			SDL_RenderCopy( rend, data, &clips[i][j], &position[i] );
			position[i].y += STEP_LENGTH;
			j = ( j + 1 ) % DATA_LINE;
		}
		//刷新数据流 
		if( flag++ == REFRESH ){
			for( i = 0; i < DATA_ROW; i++ ){
				position[i].y = 0;
			}
			flag = 0;
		}
		SDL_RenderPresent( rend );		//显示画面 
		SDL_Delay(FLOW_SPEED);			//控制流速 
	}
	
	//释放图像 
	SDL_FreeSurface( datasurface );	
	datasurface = NULL; 
	SDL_FreeSurface( blacksurface );
	blacksurface = NULL; 
	SDL_DestroyWindow( window );		//销毁窗口 
	SDL_Quit();							//退出系统 
	
	return 0;
}
