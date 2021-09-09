#pragma once
#include <SDL.h>
extern SDL_Texture* ballTexture;
extern SDL_Rect* ballRect;
extern SDL_Surface* ballImage;
extern int* ballSpeedX;
extern int* ballSpeedY;
extern int ballSpeedChangeX;
extern int maxBallSpeed;
extern int actualBalls;
void centerBall(int ballIndex);
void closeTextureBall();
void initBall();
void changeBallMovementWithBrick(SDL_Rect brick, int ballIndex);
void incrementBallSpeedX(int ballIndex);
void incrementBallSpeedY(int ballIndex);
void decrementBallSpeedX(int ballIndex);
void decrementBallSpeedY(int ballIndex);
SDL_Point* getBallSpeedMultiplicators(SDL_Point* ballMultiplicator);
void setMultiplicatorBallSpeed(SDL_Point* ballMultiplicator);
void initBallSpeed(int ballIndex);
void showBalls(SDL_Renderer* renderer);
void adNewBall();
void restartBall();
SDL_Point* getActualBallPercentages(SDL_Point* percentages);
void reziseBalls();