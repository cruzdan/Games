// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <SDL.h>
#include <SDL_mixer.h>

Mix_Music* music;
Mix_Chunk* sound;

void initSounds() {
	music = Mix_LoadMUS("sounds/kart-mario.mp3");
	if (!music) {
		SDL_Log("The music is not loaded");
	}
	sound = Mix_LoadWAV("sounds/hit.wav");
	if (!sound) {
		SDL_Log("The sound is not loaded");
	}
}

void updateMusic() {
	if (!Mix_PlayingMusic()) {
		Mix_PlayMusic(music, -1);
	}
	else if (Mix_PausedMusic()) {
		Mix_ResumeMusic();
	}
	else {
		Mix_PauseMusic();
	}
}

void closeMusic() {
	Mix_FreeChunk(sound);
	Mix_FreeMusic(music);
}