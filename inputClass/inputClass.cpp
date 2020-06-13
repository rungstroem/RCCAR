#include <iostream>
#include <SDL2/SDL.h>


class inputClass{
	public:
	//Constructor
	inputClass();
	//Destructor
	~inputClass();
	
	//Input loop
	void loop(){
		while(!quit){
			SDL_SetRenderDrawColor(render, 0,0,0,0);
			SDL_RenderClear(render);
			
			//Handle input events
			while(SDL_PollEvent(&inputEvent) != 0){
				if(inputEvent.type == SDL_QUIT || inputEvent.key.keysym.sym == SDLK_ESCAPE){
					quit = true;
				} else if(inputEvent.type == SDL_KEYDOWN){
					switch (inputEvent.key.keysym.sym){
						case SDLK_UP:
							yVel = 1;
							break;
						case SDLK_DOWN:
							yVel = -1;
							break;
						case SDLK_LEFT:
							xVel = -1;
							break;
						case SDLK_RIGHT:
							xVel = 1;
							break;
					}
				} else if(inputEvent.type == SDL_KEYUP) {
					switch(inputEvent.key.keysym.sym){
						case SDLK_UP:
							yVel = 0;
							break;
						case SDLK_DOWN:
							yVel = 0;
							break;
						case SDLK_LEFT:
							xVel = 0;
							break;
						case SDLK_RIGHT:
							xVel = 0;
							break;
					}
				}
			}
			posX += xVel;
			posY += yVel;
			
			if(posX > screenWidth) posX = 0;
			if(posX < 0) posX = screenWidth;
			if(posY > screenHeight) posY = 0;
			if(posY < 0) posY = screenHeight;

			SDL_SetRenderDrawColor(render, 255,255,255,255);
			SDL_RenderDrawPoint(render, posX, posY);
		
			//End of loop - draws to screen
			SDL_RenderPresent(render);
		}
		SDL_DestroyRenderer(render);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	
	protected:
	const int screenHeight = 200;
	const int screenWidth = 200;
	bool quit = false;
	int posX = 100;
	int posY = 100;
	int go = 0; 
	int turn = 0;
	int xVel = 0; 
	int yVel = 0;

	SDL_Window *window = NULL;
	SDL_Renderer *render = NULL;
	SDL_Event inputEvent;

};


inputClass::inputClass() {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Visual dot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

inputClass::~inputClass(){
	//Nothing to do
};

