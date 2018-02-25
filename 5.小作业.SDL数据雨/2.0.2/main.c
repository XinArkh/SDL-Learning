//C����ģ��������-2.0.1
//2016.8.8

//SDL�汾�� SDL2-2.0.4	SDL2_image-2.0.2

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h> 

#define WINDOW_WIDTH 800		//���ڿ�� 
#define WINDOW_HEIGHT 600		//���ڸ߶� 
#define OPICITY 25				//��͸���� 
#define DATA_LINE 30			//�������� 
#define DATA_ROW 10				//�������� 
#define STEP_LENGTH 17			//���ݲ��� 
#define FLOW_SPEED 80			//�������� 

//����������Ԫ�ؽṹ�� 
typedef struct{
	int x;
	int y;
	int w;
	int h;
	int speed;
	int alpha;
}Flowelement;

//��ͼ���� 
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

//�ƶ����� 
void moveFlowelement( Flowelement* element )
{
	element->y += element->speed;
	if( element->y >= WINDOW_HEIGHT ){
		element->y = 0;
	}
}

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
	SDL_SetTextureBlendMode( data, SDL_BLENDMODE_BLEND );		//��������
	SDL_SetTextureBlendMode( black, SDL_BLENDMODE_BLEND );		//���ú�ɫ�ɰ� 
	SDL_SetTextureAlphaMod( black, OPICITY );					//���ú�ɫ�ɰ治͸���� 
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);		// ��ʼ��SDL_mixer 
	Mix_Music* backgroundmus = Mix_LoadMUS("Clubbed To Death.mp3");
	Mix_PlayMusic(backgroundmus, -1);		//�ڶ���������-1ʱ����һֱѭ�� 
	

	int i, j;		//ѭ����� 
	int imgWidth  = datasurface->w / DATA_ROW;		//�������ݿ�� 
	int imgHeight = datasurface->h / DATA_LINE;		//�������ݸ߶�
	int winWidth  = WINDOW_WIDTH / DATA_ROW;		//������� 
	
	//��ɫ�ɰ���� 
	SDL_Rect rectWindow;
	rectWindow.w = WINDOW_WIDTH;
	rectWindow.h = WINDOW_HEIGHT;
	rectWindow.x = 0;
	rectWindow.y = 0;
	
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
	
	//����������Ԫ�ؽṹ�� 
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
	
	bool quit = false;		//�˳���� 
	SDL_Event event;		//�����¼� 
	i = j = 0;
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
			drawFlowelement( rend, data, &clips[i][j], &element[i] );
			drawFlowelement( rend, data, &clips[i][j], &element[i] );
			drawFlowelement( rend, data, &clips[i][j], &element[i] );
			drawFlowelement( rend, data, &clips[i][j], &element[i] );
			moveFlowelement( &element[i] );
		}
		j = ( j + 1 ) % DATA_LINE;
		SDL_RenderPresent( rend );		//��ʾ���� 
		SDL_Delay( FLOW_SPEED );		//�������� 
	} 
	
//	Mix_FadeOutMusic(1000); 
//	SDL_Delay( 2000 );

	//�ͷ�ͼ�� 
	SDL_FreeSurface( datasurface );	
	datasurface = NULL; 
	SDL_FreeSurface( blacksurface );
	blacksurface = NULL; 
	SDL_DestroyWindow( window );		//���ٴ��� 
	SDL_Quit();							//�˳�ϵͳ 
	
	return 0;
}
