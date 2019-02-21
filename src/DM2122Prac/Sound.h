#pragma once
#include <memory>
#include <iostream>
#include <irrKlang.h>
#include <stdexcept>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

class Sound
{
public:
	Sound();
	~Sound();
	bool init();
	void playsound(const char * music, bool looping);
	void playsound(const char* music);
	void setsoundvol(const float volume);
private:
	static short count; // Count How many sounds is playing
	ISoundEngine* engine;
};

namespace music
{
	extern Sound player;
}