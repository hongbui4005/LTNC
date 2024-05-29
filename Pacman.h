#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "string"
#include "Map.h"
#include "Enemy.h"
#include "Back.h"
#ifndef pac

#define pac


class Pacman{
public:

    static const int PAC_WIDTH = 32;
    static const int PAC_HEIGHT = 32;
    const int PAC_VEL = 4;

    Pacman(int _mPosX, int _mPosY);
    void reset(int _mPosX, int _mPosY);
    bool loadPac(std::string path, SDL_Surface* tmpSurface = NULL);
    bool loadPacDie(std::string path, SDL_Surface* tmpSurface = NULL);
    void handleEvent(SDL_Event&e);
    void Move(Enemy * ene, Back * ba);
    void Render();
    void RenderDeath();
    bool checkCollision();
    bool checkCollision2();
    bool checkCollisionToEnemy(Enemy * ene);
    void miniUpdate();
    int getX();
    int getY();
    void death(Enemy * ene);
    void eatFood(Enemy * ene);
    void updateFrame();

private:
    int mPosX, mPosY;
    int mVelX, mVelY;
    int preVelX, preVelY;
    int pacId, prePacId;
    bool isDead;
    SDL_Texture* mPac;
    SDL_Texture* mPacDie;
    SDL_Rect src;
};

int Pacman::getX(){
    return mPosX;
}

int Pacman::getY(){
    return mPosY;
}

bool Pacman::checkCollision(){
    int x1 = mPosX, x2 = (mPosX+PAC_WIDTH - ((mPosX + PAC_WIDTH) % PAC_WIDTH == 0))%640, y1 = mPosY, y2 = (mPosY + PAC_HEIGHT - ((mPosY + PAC_HEIGHT) % PAC_HEIGHT == 0))%480;

    x1 /= PAC_WIDTH;
    x2 /= PAC_WIDTH;
    y1 /= PAC_HEIGHT;
    y2 /= PAC_HEIGHT;

    if (Map[y1][x1] == 1 || Map[y2][x1] == 1 || Map[y1][x2] == 1 || Map[y2][x2] == 1) return false;

    return true;
}

bool Pacman::checkCollision2(){
    int x1 = mPosX, y1 = mPosY;
    int i = x1 / PAC_WIDTH;
    int j = y1 / PAC_HEIGHT;
    switch(pacId){
        case 6:{
            if (prePacId ==  4) return false;
            if (prePacId == 0 && Map[j+1][i+1] != 1 && (abs(x1-32*(i+1)) <= 20) ){
                mPosX = (i + 1)* 32;
                miniUpdate();
                return true;
            }

            if ((prePacId == 2 && Map[j+1][i]!=1 && (abs(x1 -32*i) <= 20)) || (prePacId == 0 && Map[j+1][i] !=1 && x1 % 32 == 0)){
                mPosX = i * 32;
                miniUpdate();
                return true;
            }
            break;
        }
        case 4:{
            if (prePacId ==  6) return false;
            if (prePacId == 0 && Map[j-1][i+1] != 1 && (abs(x1-32*(i+1)) <= 20) ){
                mPosX = (i + 1)* 32;
                miniUpdate();
                return true;
            }

            if ((prePacId == 2 && Map[j-1][i]!=1 && (abs(x1 -32*i) <= 20)) || (prePacId == 0 && Map[j-1][i]!=1 && x1 % 32 == 0)){
                mPosX = i * 32;
                miniUpdate();
                return true;
            }
            break;
        }

        case 0:{
            if (prePacId ==  2) return false;


            if (prePacId == 6 && Map[j+1][i+1] != 1 && (abs(y1-32*(j+1)) <= 20) ){
                mPosY = (j + 1)* 32;
                miniUpdate();
                return true;
            }

            if ((prePacId == 4 && Map[j][i+1]!=1 && (abs(y1 -32*j) <= 20)) || (prePacId == 6 && Map[j+1][i+1]!=1 && y1 % 32 == 0)){
                mPosY = j * 32;
                miniUpdate();
                return true;
            }
            break;
        }

        case 2:{
            if (prePacId ==  0) return false;

            if (prePacId == 6 && Map[j+1][i-1] !=1 && (abs(y1-32*(j+1)) <= 20) ){
                mPosY = (j + 1)* 32;
                miniUpdate();
                return true;
            }

            if ((prePacId == 4 && Map[j][i-1]!=1 && (abs(y1 -32*j) <= 20)) || (prePacId == 6 && Map[j+1][i-1]!=1 && y1 % 32 == 0)){
                mPosY = j * 32;
                miniUpdate();
                return true;
            }
            break;
        }
    }

    return false;
}

bool Pacman::loadPac(std::string path, SDL_Surface *tmpSurface){
    tmpSurface = IMG_Load(path.c_str());
    mPac = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);
    return mPac != NULL;
}

bool Pacman::loadPacDie(std::string path, SDL_Surface *tmpSurface){
    tmpSurface = IMG_Load(path.c_str());
    mPacDie = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);
    if (mPacDie == NULL) std::cout << "Yes";
    else std::cout << "No";


    return mPacDie != NULL;
}


void Pacman::reset(int _mPosX, int _mPosY){
    mPosX = _mPosX;
    mPosY = _mPosY;
    mVelX = 0;
    mVelY = 0;
    preVelX = 0;
    preVelY = 0;
    src.x = src.y = 0;
    src.w = src.h = 12;
    isDead = 0;
}

Pacman::Pacman(int _mPosX, int _mPosY){
    mPosX = _mPosX;
    mPosY = _mPosY;
    mVelX = 0;
    mVelY = 0;
    preVelX = 0;
    preVelY = 0;
    src.x = src.y = 0;
    src.w = src.h = 12;
    isDead = 0;
}


void Pacman::miniUpdate(){
    prePacId = pacId;
    preVelX = mVelX;
    preVelY = mVelY;
}

void Pacman::handleEvent(SDL_Event& e){

    if (e.type == SDL_KEYDOWN && e.key.repeat == 0){
        switch(e.key.keysym.sym){
            case SDLK_UP:
                mVelY = -PAC_VEL;
                mVelX = 0;
                pacId = 4;
                break;
            case SDLK_DOWN:
                mVelY = PAC_VEL;
                mVelX = 0;
                pacId = 6;
                break;
            case SDLK_RIGHT:
                mVelX = PAC_VEL;
                mVelY = 0;
                pacId = 0;
                break;
            case SDLK_LEFT:
                mVelX = -PAC_VEL;
                mVelY = 0;
                pacId = 2;
                break;
        }
    }
}

void Pacman::Move(Enemy * ene, Back * ba){

    if (checkCollisionToEnemy(ene)){
        //isRunning = false;
        death(ene);
        ba->state = 2;
        gameState = 0;
        return;
    }

    if (totalPoint == 135){
        gameState = 0;
        ba->state = 1;
        return;
    }
    bool check3 = false;

    check3 = checkCollision2();

    if (check3 == false){

        mPosX = (mPosX + mVelX + 640) % 640;
        mPosY = (mPosY + mVelY + 480) % 480;
            //std::cout << mPosX << ' '  << mPosY << ' ' << pacId << ' ' << mVelX << ' ' << mVelY << ' ' << check3 << '\n';

        if (!checkCollision()){
            //std::cout << mPosX << ' '  << mPosY << ' ' << pacId << ' ' << mVelX << ' ' << mVelY << ' ' << check3 << '\n';
            mPosX = (mPosX - mVelX) % 640;
            mPosY = (mPosY - mVelY) % 480;
            pacId = prePacId;
            mVelX = preVelX;
            mVelY = preVelY;

        }
        else miniUpdate();
    }

    eatFood(ene);
    updateFrame();
}


void Pacman::Render(){
    SDL_Rect clip = {mPosX, mPosY, PAC_WIDTH, PAC_HEIGHT};
    SDL_RenderCopy(gRenderer, mPac, &src, &clip);
}

void Pacman::eatFood(Enemy * ene){
    if (Map[mPosY/PAC_HEIGHT][mPosX/PAC_WIDTH] == 0) {
        Map[mPosY/32][mPosX/32] = 3;
        totalPoint++;
    }
    else if (Map[mPosY/PAC_HEIGHT][mPosX/PAC_WIDTH] == 2){
        Map[mPosY/PAC_HEIGHT][mPosX/PAC_WIDTH] = 3;
        ene->timeStone();
        totalPoint++;
    }
    std::cout << totalPoint << '\n';
}

void Pacman::updateFrame(){
    src.x = 12 * pacId + 12* (int)(((SDL_GetTicks()/100) % 2));
}

bool Pacman::checkCollisionToEnemy(Enemy * ene){
    bool killEnemy = ene->getRageMode();
    int x = ene->getX();
    int y = ene->getY();


    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = mPosX;
    rightA = mPosX + edge;
    topA = mPosY;
    bottomA= mPosY + edge;

    leftB = x;
    rightB = x + edge;
    topB = y;
    bottomB = y + edge;


    bool isCollision = 1;
    if( bottomA <= topB)
    {
        isCollision = 0;
    }

    if( topA >= bottomB )
    {
        isCollision = 0;
    }

    if( rightA <= leftB )
    {
        isCollision = 0;
    }

    if( leftA >= rightB )
    {
        isCollision = 0;
    }
    if (!isCollision) return false;

    if (killEnemy){
        ene->dead();
        return false;
    }

    return true;
}


void Pacman::death(Enemy * ene){
    ene->over();

    for (int i = 0; i < 9; i++){
        src.x = 12 * i;

        SDL_RenderClear(gRenderer);
        drawMap();
        ene->Render();
        RenderDeath();
        SDL_RenderPresent(gRenderer);
        SDL_Delay(100);
    }

    SDL_Delay(1000);
}

void Pacman::RenderDeath(){
    SDL_Rect clip = {mPosX, mPosY, PAC_WIDTH, PAC_HEIGHT};
    SDL_RenderCopy(gRenderer, mPacDie, &src, &clip);
}
#endif // pac
