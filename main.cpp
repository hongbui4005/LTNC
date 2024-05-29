#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "string"
#include "Pacman.h"

Pacman Pac(32,32);
Enemy Ene(32*10,32);
Back Ba;
void init();


int main(int argc, char ** argv){

    init();

    while (isRunning){
        int timeFrame =  SDL_GetTicks();
        // handle event
        SDL_Event e;
        SDL_PollEvent(&e);

        if (e.type == SDL_QUIT) isRunning = false;
        if(gameState == 0){
            //if (SDL_GetTicks() >= 2000) gameState = 0;
            Pac.reset(32,32);
            Ene.reset(32*10, 32);
            totalPoint = 0;
            resetMap();

            SDL_RenderClear(gRenderer);
            Ba.render();
            SDL_RenderPresent(gRenderer);
            Ba.handleKeyEvent(e);
        }
        else{
            Pac.handleEvent(e);

            // update
            Pac.Move(&Ene, &Ba);
            Ene.Move(Pac.getX(), Pac.getY());

            // render
            SDL_SetRenderDrawColor(gRenderer, 0, 0,0,0);
            SDL_RenderClear(gRenderer);
            drawMap();
            Pac.Render();
            Ene.Render();

            SDL_RenderPresent(gRenderer);
        }


        //set Fps
        timeFrame = SDL_GetTicks() - timeFrame;
        if (timeFrame < timePerFrame){
            SDL_Delay(timePerFrame - timeFrame);
        }
    }
    return 0;
}


void init(){
    SDL_Init(SDL_INIT_EVERYTHING);
    gWindow = SDL_CreateWindow("PAC MAN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface* tmpSurface = NULL;

    Pac.loadPac("image/pac2.png", tmpSurface);
    Pac.loadPacDie("image/pacDie.png", tmpSurface);
    Ene.loadEnemy("image/Enemy.png", tmpSurface);
    Ene.loadSageMode("image/sageMode.png", tmpSurface);

    tmpSurface = IMG_Load("image/wall.png");
    gWall = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);
    tmpSurface = IMG_Load("image/food.png");
    gFood = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);

    tmpSurface = NULL;
    tmpSurface = IMG_Load("image/bigFood.png");

    Ba.loadImg("image/tex1.png","image/win.png","image/lose.png", tmpSurface);

    bigFood = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);

    SDL_FreeSurface(tmpSurface);

    Hash();
    src1.x = 0;
    src1.y = 0;
    src1.w = src1.h = 12;
}
