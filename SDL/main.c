#include <SDL/SDL.h>


int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("SDL Test", "SDL Test");

	SDL_Surface *screen = SDL_SetVideoMode(640,480, 0,0);
	//SDL_Surface *temp = SDL_LoadBMP("sdl_logo.bmp");
	//SDL_Surface *bg = SDL_DisplayFormat(temp);
	//SDL_FreeSurface(temp);

	SDL_Event event;
	int gameover = 0;
	while(!gameover) {
		if(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					gameover = 1;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
						case SDLK_q:
							gameover = 1;
							break;
						default:
							break;
					}
					break;
			}
		}
		//SDL_BlitSurface(bg, NULL, screen, NULL);
		SDL_UpdateRect(screen, 0,0,0,0);
	}
	//SDL_FreeSurface(bg);
	SDL_Quit();
	return 0;
}
