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

	transform = engine->addSoundSourceFromFile("Sound/transform.mp3");
	drive = engine->addSoundSourceFromFile("Sound/Drive.wav");
	game = engine->addSoundSourceFromFile("Sound/Game.mp3");
	main = engine->addSoundSourceFromFile("Sound/Main.mp3");
	selections = engine->addSoundSourceFromFile("Sound/Picking.mp3");
	ableToPlace = engine->addSoundSourceFromFile("Sound/CorrectPlace.wav");
	unableToPlace = engine->addSoundSourceFromFile("Sound/Wrongplace.wav");
	music::player.setsoundvol(0.15);
	
}

void Sound::playsound(ISoundSource* music, bool looping)
{
	engine->play2D(music,looping);
}

void Sound::playsound(ISoundSource* music)
{
	engine->play2D(music);
}

void Sound::setsoundvol(const float volume)
{
	engine->setSoundVolume(volume);
}

ISoundSource* Sound::getTransformSound()
{
	return transform;
}

ISoundSource * Sound::getDriveSound()
{
	return drive;
}

ISoundSource * Sound::getGameSound()
{
	return game;
}

ISoundSource * Sound::getMainSound()
{
	return main;
}

ISoundSource * Sound::getSelectionSound()
{
	return selections;
}

ISoundSource * Sound::getCanPlaceSound()
{
	return ableToPlace;
}

ISoundSource * Sound::getCantPlaceSound()
{
	return unableToPlace;
}

ISoundEngine * Sound::getEngine()
{
	return engine;
}
