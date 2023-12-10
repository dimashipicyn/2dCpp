//
//  Audio.hpp
//  2dCpp
//
//  Created by Дима Щипицын on 15/11/2022.
//

#ifndef Audio_hpp
#define Audio_hpp

#include "Export.h"

#include <unordered_map>
#include <string>
#include <memory>

class TWODCPP_EXPORT Audio
{
public:
	Audio();
	~Audio();

	void play_music(const std::string& name);
	void pause_music();
	void resume_music();
	void stop_music();

private:
	struct Impl;
	std::unique_ptr<Impl> impl_;
};

#endif /* Audio_hpp */
