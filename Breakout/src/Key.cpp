#include <SDL.h>
#include "Paddle.h"
#include "Restart.h"
#include "GlobalVariables.h"
#include "Menu.h"
#include "Ball.h"
#include "Update.h"
#include "Command.h"
#include "Capsule.h"

bool gameOver = false;

//put the rect in the x and y percentage of the screen
SDL_Rect setWindowRectCoordinates(int percentageX, int percentageY, SDL_Rect rect) {
	rect.x = (percentageX * SCREEN_WIDTH) / 100 - rect.w / 2;
	rect.y = (percentageY * SCREEN_HEIGHT) / 100 - rect.h / 2;
	return rect;
}

//get the coordinates in percentage of the screen of the rect(center) in x,y
void getActualRectCoordinates(int* x, int* y, SDL_Rect rect) {
	int centerRectX = rect.x + rect.w / 2;
	int centerRectY = rect.y + rect.h / 2;
	*x = 100 * centerRectX / SCREEN_WIDTH;
	*y = 100 * centerRectY / SCREEN_HEIGHT;
}

//key of quit and resize window
void generalKey(SDL_Event event, SDL_Renderer* renderer) {
	if (event.type == SDL_QUIT) {
		gameOver = true;
	}
	else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
		//change the window size
		int paddleX, paddleMultiplicator;
		SDL_Point* ballPercentages = new SDL_Point[actualBalls];
		SDL_Point* ballMultiplicator = new SDL_Point[actualBalls];
		ballPercentages = getActualBallPercentages(ballPercentages);
		getActualPadddleCoordinates(&paddleX);
		ballMultiplicator = getBallSpeedMultiplicators(ballMultiplicator);
		paddleMultiplicator = getMultiplicatorPaddleSpeed();
		SDL_Point* capsulePercentages = new SDL_Point[actualCapsules];
		capsulePercentages = getActualCapsulePercentages(capsulePercentages);
		SDL_Point* bulletPercentages = new SDL_Point[actualBullets];
		bulletPercentages = getActualBulletPercentages(bulletPercentages);

		changeWindowGameSize(renderer);

		setWindowPaddleCoordinates(paddleX);
		for (int i = 0; i < actualBalls; i++) {
			ballRect[i] = setWindowRectCoordinates(ballPercentages[i].x, ballPercentages[i].y, ballRect[i]);

		}
		reziseBalls();
		setMultiplicatorBallSpeed(ballMultiplicator);
		setMultiplicatorPaddleSpeed(paddleMultiplicator);
		resizeCapsules(); 
		for (int i = 0; i < actualCapsules; i++) {
			capsules[i].rect = setWindowRectCoordinates(capsulePercentages[i].x, capsulePercentages[i].y, capsules[i].rect);
		}
		delete[] ballMultiplicator;
		delete[] capsulePercentages;
		delete[] ballPercentages;
		resizeBullets();
		for (int i = 0; i < actualBullets; i++) {
			bullets[i] = setWindowRectCoordinates(bulletPercentages[i].x, bulletPercentages[i].y, bullets[i]);
		}
		delete[] bulletPercentages;
	}
}

void detectGameKey(SDL_Renderer* renderer) {
	SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		generalKey(event, renderer);
		if (event.type == SDL_KEYDOWN) {
			if (commandLine) {
				if (keys[SDL_SCANCODE_H]) 
					allCommands = !allCommands;
				else
					addWordToCommand(renderer, event.key.keysym.scancode);
			}
			if (keys[SDL_SCANCODE_ESCAPE]) {
				menuIndex = 1;
			}
			else if (event.key.keysym.scancode == 47) {
				commandLine = true;
			}
			else if (keys[SDL_SCANCODE_RETURN]) {
				commandLine = false;
				checkCommandMatch(renderer);
				command = "";
				writeCommandLineText(renderer, command);
				allCommands = false;
			}
		}
	}
	if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) {
		movePaddleLeft();
	}
	if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) {
		movePaddleRight();
	}
	if (keys[SDL_SCANCODE_F]) {
		shoot();
	}
	if (keys[SDL_SCANCODE_R]) {
		restart(renderer);
	}
}

void detectPauseKey(SDL_Renderer* renderer) {
	SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		generalKey(event, renderer);
		if (event.type == SDL_KEYDOWN) {
			if (commandLine) {
				if (keys[SDL_SCANCODE_H])
					allCommands = !allCommands;
				else
					addWordToCommand(renderer, event.key.keysym.scancode);
			}
			if (keys[SDL_SCANCODE_ESCAPE]) {
				menuIndex = 0;
			}
			else if (event.key.keysym.scancode == 47) {
				commandLine = true;
			}
			else if (keys[SDL_SCANCODE_RETURN]) {
				commandLine = false;
				checkCommandMatch(renderer);
				command = "";
				writeCommandLineText(renderer, command);
				allCommands = false;
			}	
		}
		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
			int mouseX;
			int mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			SDL_Rect mouseRect;
			mouseRect.x = mouseX;
			mouseRect.y = mouseY;
			mouseRect.w = 1;
			mouseRect.h = 1;

			if (SDL_HasIntersection(&mouseRect, &pauseRects[5])) {
				//exit text
				gameOver = true;
			}

			for (int i = 0; i < 5; i++) {
				//volume rects
				if (SDL_HasIntersection(&mouseRect, &musicVolumeRect[i])) {
					changeMusicRects(renderer, i + 1);
					break;
				}
				if (SDL_HasIntersection(&mouseRect, &soundVolumeRect[i])) {
					changeSoundRects(renderer, i + 1);
					break;
				}
			}

			for (int i = 6; i < 9; i++) {
				if (SDL_HasIntersection(&mouseRect, &pauseRects[i])) {
					switch (i - 5) {
					case 1:
						musicOnOff(renderer);
						break;
					case 2:
						soundOnOff(renderer);
						break;
					case 3:
						int paddleX;
						SDL_Point* ballMultiplicator = new SDL_Point[actualBalls];
						getActualPadddleCoordinates(&paddleX);
						SDL_Point* ballPercentages = new SDL_Point[actualBalls];
						ballPercentages = getActualBallPercentages(ballPercentages);
						ballMultiplicator = getBallSpeedMultiplicators(ballMultiplicator);
						fullscreenOnOff(renderer);
						changeFullscreenGameSize(renderer);
						setWindowPaddleCoordinates(paddleX);
						for (int i = 0; i < actualBalls; i++) {
							ballRect[i] = setWindowRectCoordinates(ballPercentages[i].x, ballPercentages[i].y, ballRect[i]);
						}
						reziseBalls();
						setMultiplicatorBallSpeed(ballMultiplicator);
						delete[] ballMultiplicator;
						delete[] ballPercentages;
						break;
					}
					break;
				}
			}
		}
	}
}