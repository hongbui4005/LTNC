#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "string"
#include "Map.h"
#ifndef enemy
#define enemy

class Enemy{
public:
    int evel = 2;

    Enemy(int _mposx, int _mposy);
    void reset(int _mposx, int _mposy);
    void Move(int hx, int hy);
    void Render();
    bool loadEnemy(std::string path, SDL_Surface* tmpSurface = NULL);
    bool loadSageMode(std::string path, SDL_Surface* tmpSurface = NULL);
    void updateVelocity();
    int getDirect(int dir, int start);
    void timeStone(){
        initTime = SDL_GetTicks();
    }
    void dead();
    bool getIsDead(){
        return isDead;
    }
    bool getRageMode(){
        return rageMode;
    }
    void over();
    int getX();
    int getY();


private:

    int mvelx, mvely;
    int mposx, mposy;
    int eId;

    bool rageMode = false, isDead = false;
    double initTime = -5000.0;
    SDL_Rect src;
    SDL_Texture *menemy;
    SDL_Texture *sageMode;
};

Enemy::Enemy(int _mposx, int _mposy){
    mposx = _mposx;
    mposy = _mposy;
    eId = 0;
    mvelx = mvely = 0;
    src.x = src.y = 0;
    src.h = src.w = 12;
}

void Enemy::reset(int _mposx, int _mposy){
    mposx = _mposx;
    mposy = _mposy;
    eId = 0;
    mvelx = mvely = 0;
    src.x = src.y = 0;
    src.h = src.w = 12;
}
void Enemy::updateVelocity(){
    if (eId == 0){
        mvelx = evel;
        mvely = 0;
    }
    else if (eId == 2){
        mvelx = -evel;
        mvely = 0;
    }
    else if (eId == 4){
        mvelx = 0;
        mvely = -evel;
    }
    else {
        mvelx = 0;
        mvely= evel;
    }
}

int Enemy::getDirect(int dir, int start){
    if (dir == start + 1){
        return  0;
    }
    else if (dir == start - 1){
        return  2;
    }
    else if (dir < start){
        return  4;
    }

    return  6;
}
void Enemy::Move(int hx, int hy){
    if (isDead) return;

    if (SDL_GetTicks() - initTime <= 5000) rageMode = 1;
    else rageMode = 0;

    int start = get(mposx, mposy);
    int dest = get(hx, hy);
    int dir = nxt[start-1][dest-1];


    if (rageMode == 0){
        if (mposx %32 == 0 && mposy %32 == 0)   eId = getDirect(dir, start);
    }
    else{
        if (mposx % 32 == 0 && mposy % 32 == 0){
            dir = getDirect(dir, start);
            for (int i = 0; i <= 6; i+=2){
                if (i == dir) continue;

                int newPosX = mposx + dx[i];
                int newPosY = mposy + dy[i];

                if(Map[newPosY/32][newPosX/32] != 1) {
                        eId = i;
                        break;
                }
            }
        }
    }

    updateVelocity();
    mposx += mvelx;
    mposy += mvely;

    src.x = 12 * eId + 12* (int)(((SDL_GetTicks()/100) % 2));
}

void Enemy::Render(){
    if (isDead) return;
    SDL_Rect clip = {mposx, mposy, edge, edge};

    if (rageMode){
        SDL_RenderCopy(gRenderer, sageMode, &src, &clip);
    }
    else{
        SDL_RenderCopy(gRenderer, menemy, &src, &clip);
    }
}

bool Enemy::loadEnemy(std::string path, SDL_Surface* tmpSurface){
    tmpSurface = IMG_Load(path.c_str());
    menemy = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);
    return menemy != NULL;
}

bool Enemy::loadSageMode(std::string path, SDL_Surface* tmpSurface){
    tmpSurface = IMG_Load(path.c_str());
    sageMode = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);
    return sageMode != NULL;
}

int Enemy::getX(){
    return mposx;
}

int Enemy::getY(){
    return mposy;
}

void Enemy::over(){
    mvelx = 0;
    mvely = 0;
}
void Enemy::dead(){
    isDead = true;
}
#endif // enemy
