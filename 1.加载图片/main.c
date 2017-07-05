#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char *argv[]) {
	SDL_Init( SDL_INIT_VIDEO );
	SDL_Window *window = SDL_CreateWindow( "My first window",30,30,
//											SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
											1421, 1080,
											SDL_WINDOW_SHOWN );
	
	SDL_Rect rect;
	rect.x = 100;
	rect.y = 100;										
	SDL_Surface *surface = SDL_GetWindowSurface( window );
//	SDL_Surface *image   = SDL_LoadBMP( "F:\hello.bmp" );	
	SDL_Surface *element = IMG_Load( "F:\dark night.png" );
									
	SDL_BlitSurface( element, NULL, surface, NULL );										
	SDL_UpdateWindowSurface( window );										
											
	SDL_Delay( 2000 );
	
	SDL_FreeSurface( element );
	SDL_FreeSurface( surface );
	SDL_DestroyWindow( window );
	SDL_Quit();
	
	return 0;
}
