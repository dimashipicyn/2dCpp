//
//  Audio.hpp
//  2dCpp
//
//  Created by Дима Щипицын on 15/11/2022.
//

#ifndef Audio_hpp
#define Audio_hpp

#include <unordered_map>
#include <string>

typedef struct _Mix_Music Mix_Music;
typedef struct Mix_Chunk Mix_Chunk;

class Audio
{
public:
	Audio();
	~Audio();

	void play_music(const std::string& name);
	void pause_music();
	void resume_music();
	void stop_music();

	void play_chunk(const std::string& name);

	bool load_chunk(const std::string& name, const std::string& filename);
	bool load_music(const std::string& name, const std::string& filename);

private:

	/*using Mix_Music_ptr = std::unique_ptr<Mix_Music, void(*)(Mix_Music*)>;
	using Mix_Chunk_ptr = std::unique_ptr<Mix_Chunk, void(*)(Mix_Chunk*)>;

	std::unordered_map<std::string, Mix_Music_ptr>	musics_;
	std::unordered_map<std::string, Mix_Chunk_ptr>	chunks_;*/

	bool isInitialize_ = false;
};

#endif /* Audio_hpp */
