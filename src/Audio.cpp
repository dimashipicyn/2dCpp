//
//  Audio.cpp
//  2dCpp
//
//  Created by Дима Щипицын on 15/11/2022.
//

#include "stdafx.h"

#include "Audio.hpp"

#include <SDL.h>
#include <cassert>

Audio::Audio() {
	if(SDL_Init(SDL_INIT_AUDIO) < 0) {
		return;
	}
	// if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
	// 	return;
	// }
	isInitialize_ = true;
}

Audio::~Audio() {

}

bool Audio::load_music(const std::string &name, const std::string &filename) {
	/*Mix_Music_ptr mus(Mix_LoadMUS(filename.c_str()), Mix_FreeMusic);
	if (!mus) {
		return false;
	}
	musics_.emplace(name, std::move(mus));*/
	return true;
}


bool Audio::load_chunk(const std::string &name, const std::string &filename) { 
	/*Mix_Chunk_ptr chunk(Mix_LoadWAV(filename.c_str()), Mix_FreeChunk);
	if (!chunk) {
		return false;
	}
	chunks_.emplace(name, std::move(chunk));*/
	return true;
}

void Audio::play_chunk(const std::string &name) {
	/*auto chunk = chunks_.find(name);
	assert(chunk != chunks_.end());
	Mix_PlayChannel(-1, chunk->second.get(), 0);*/
}

void Audio::play_music(const std::string &name) {
	/*auto music = musics_.find(name);
	assert(music != musics_.end());
	if (Mix_PlayingMusic()) {
		Mix_HaltMusic();
	}
	Mix_PlayMusic(music->second.get(), -1);*/
}

void Audio::stop_music() { 
	/*Mix_HaltMusic();*/
}

void Audio::resume_music() {
	/*if (!Mix_PlayingMusic()) {
		return;
	}
	if (Mix_PausedMusic()) {
		Mix_ResumeMusic();
	}*/
}

void Audio::pause_music() {
	/*if (!Mix_PlayingMusic()) {
		return;
	}
	if (!Mix_PausedMusic()) {
		Mix_PauseMusic();
	}*/
}



