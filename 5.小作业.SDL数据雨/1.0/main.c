//C����ģ��������
//2016.8.4

//SDL�汾�� SDL2-2.0.4	SDL2_image-2.0.1 

#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#define WINDOW_WIDTH 800		//���ڿ�� 
#define WINDOW_HEIGHT 600		//���ڸ߶� 
#define OPICITY 25				//��͸���� 
#define DATA_LINE 30			//�������� 
#define DATA_ROW 10				//�������� 
#define STEP_LENGTH 17			//���ݲ��� 
#define REFRESH 35				//ˢ�¼�� 
#define FLOW_SPEED 80			//�������� 

int main(int argc, char *argv[])
{
	SDL_Init( SDL_INIT_VIDEO );		//��ʼ�� 
	SDL_Window *window = SDL_CreateWindow( "Matrix", 
											SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
											WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );		//�������� 
	SDL_Renderer* rend = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );		//������Ⱦ�� 
	SDL_Surface* datasurface  = IMG_Load( "pic\\data.png" );		//��ȡ����ͼƬ 
	SDL_Surface* blacksurface = SDL_LoadBMP( "pic\\black.bmp" );	//��ȡ��ɫ�ɰ�ͼƬ 
	SDL_SetColorKey( datasurface, SDL_TRUE, SDL_MapRGB( datasurface->format, 255, 255, 255 ) );		//ȥ�����ݱ��� 
	SDL_Texture* data  = SDL_CreateTextureFromSurface( rend, datasurface );		//������������ 
	SDL_Texture* black = SDL_CreateTextureFromSurface( rend, blacksurface );	//������ɫ�ɰ����� 
	SDL_SetTextureBlendMode( black, SDL_BLENDMODE_BLEND );		//���ú�ɫ�ɰ� 
	SDL_SetTextureAlphaMod( black, OPICITY );					//���ú�ɫ�ɰ治͸���� 

	int i, j;		//ѭ����� 
	int flag;		//ˢ�±�� 
	int imgWidth  = datasurface->w / DATA_ROW;		//�������ݿ�� 
	int imgHeight = datasurface->h / DATA_LINE;		//�������ݸ߶� 
	int winWidth  = WINDOW_WIDTH / DATA_ROW;		//������� 
	
	//��ɫ�ɰ���� 
	SDL_Rect rectWindow;
	rectWindow.w = WINDOW_WIDTH;
	rectWindow.h = WINDOW_HEIGHT;
	rectWindow.x = 0;
	rectWindow.y = 0;
	
	//����λ�þ��� 
	SDL_Rect position[DATA_ROW];
	for( i = 0; i < DATA_ROW; i++ ){
		position[i].w = imgWidth;
		position[i].h = imgHeight; 
		position[i].x = i * winWidth;
		position[i].y = 0;
	}
	
	//���ݷָ���� 
	SDL_Rect clips[DATA_ROW][DATA_LINE];
	for( i = 0; i < DATA_ROW; i++ ){
		for( j = 0; j < DATA_LINE; j++ ){
			clips[i][j].w = imgWidth;
			clips[i][j].h = imgHeight;
			clips[i][j].x = i * imgWidth;
			clips[i][j].y = j * imgHeight;
		}	
	}
	
	bool quit = false;		//�˳���� 
	SDL_Event event;		//�����¼� 
	i = j = 0;
	flag = 0;
	//ѭ���� 
	while( quit == false ){
		//�¼����� 
		while( SDL_PollEvent( &event ) ){
			//�˳�ѭ�� 
			if( event.type == SDL_QUIT ){
				quit = true;
			}
		}
		SDL_RenderCopy( rend, black, NULL, &rectWindow );		//���Ǻ�ɫ�ɰ� 
		//������ 
		for( i = 0; i < DATA_ROW; i++ ){
			SDL_RenderCopy( rend, data, &clips[i][j], &position[i] );
			position[i].y += STEP_LENGTH;
			j = ( j + 1 ) % DATA_LINE;
		}
		//ˢ�������� 
		if( flag++ == REFRESH ){
			for( i = 0; i < DATA_ROW; i++ ){
				position[i].y = 0;
			}
			flag = 0;
		}
		SDL_RenderPresent( rend );		//��ʾ���� 
		SDL_Delay(FLOW_SPEED);			//�������� 
	}
	
	//�ͷ�ͼ�� 
	SDL_FreeSurface( datasurface );	
	datasurface = NULL; 
	SDL_FreeSurface( blacksurface );
	blacksurface = NULL; 
	SDL_DestroyWindow( window );		//���ٴ��� 
	SDL_Quit();							//�˳�ϵͳ 
	
	return 0;
}
