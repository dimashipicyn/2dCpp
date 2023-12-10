//
//  Audio.cpp
//  2dCpp
//
//  Created by Дима Щипицын on 15/11/2022.
//

#include "Audio.hpp"
#include "log.h"
#include <memory>
#include <string>
#include <unordered_map>

#include "miniaudio.h"

#include <cassert>

struct Audio::Impl
{
    Impl()
    {
        ma_result result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS)
        {
			LOG_ERROR("Audio init failed!");
            return;
        }
    }
	~Impl()
	{
		for (auto& p : sounds)
		{
			ma_sound_uninit(p.second.get());
		}
		ma_engine_uninit(&engine);
	}

    void play(const std::string& s)
    {
		auto sound = sounds.find(s);
		if (sound == sounds.end())
		{
			sound = sounds.insert(std::make_pair(s, std::make_unique<ma_sound>())).first;
		}

        ma_result result = ma_sound_init_from_file(&engine, s.c_str(), 0, NULL, NULL, sound->second.get());
        if (result != MA_SUCCESS)
        {
            LOG_ERROR("Audio load failed!");
            return;
        }

        ma_sound_seek_to_pcm_frame(sound->second.get(), 0);
        ma_sound_start(sound->second.get());
    }

    ma_engine engine;
	std::unordered_map<std::string, std::unique_ptr<ma_sound>> sounds;
};

Audio::Audio() {
	impl_ = std::make_unique<Audio::Impl>();
}

Audio::~Audio() {

}

void Audio::play_music(const std::string &name) {
	//impl_->play(name);
}

void Audio::stop_music() { 

}

void Audio::resume_music() {

}

void Audio::pause_music() {

}



