#include "Sound.h"
short Sound::count = 0;
Sound music::player;

Sound::Sound()
{
	// Throw exception if attempting to create more than one ShaderProg
	if (count == 1)
		throw std::exception("Sound-> Can only create 1 player");
	++count;
	init();
}


Sound::~Sound()
{
	--count;
	engine->drop(); // delete engine
}

bool Sound::init()
{
	// start the sound engine with default parameters
	engine = createIrrKlangDevice();

	if (!engine)
		return false; // error starting up the engine
}

void Sound::playsound(const char* music, bool looping)
{
	engine->play2D(music,looping);
}

void Sound::playsound(const char* music)
{
	engine->play2D(music);
}

void Sound::setsoundvol(const float volume)
{
	engine->setSoundVolume(volume);
}
