#ifndef Back_h

#define Back_h

#include<SDL.h>
#include<SDL_image.h>
#include "Map.h"
#include "iostream"
class Back{
    public:

    int state = 0;
    bool loadImg(std::string path1, std::string path2, std::string path3, SDL_Surface* tmpSurface){
        tmpSurface = IMG_Load(path1.c_str());
        tex1 = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);

        tmpSurface = IMG_Load(path2.c_str());
        win = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);

        tmpSurface = IMG_Load(path3.c_str());
        lose = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);

        return true;
    }


    void render(){
        if (state == 0){
            SDL_RenderCopy(gRenderer, tex1, NULL, NULL);
        }
        else if (state == 1){
            SDL_RenderCopy(gRenderer, win, NULL, NULL);
        }
        else if (state == 2){
            SDL_RenderCopy(gRenderer, lose, NULL, NULL);
        }
    }

    void handleKeyEvent(SDL_Event e){
        if (e.type == SDL_KEYDOWN){
            switch(e.key.keysym.sym){
                case SDLK_SPACE:
                    if (state == 0){
                        gameState = 1;
                    }
                    else{
                        state = 0;
                        gameState = 0;
                    }
                    break;
            }
        }
    }
    void setWin(int x){
        if (x == 0){
            state = 1;
        }
        else state = 2;
    }

    private:
    SDL_Texture* tex1;
    SDL_Texture* win;
    SDL_Texture* lose;

};


#endif // Back_h
