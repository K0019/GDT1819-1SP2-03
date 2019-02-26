#include "Sound.h"
short Sound::count = 0;
Sound music::player;

Sound::Sound()
{
	// Throw exception if attempting to create more than one ShaderProg
	if (count == 1)
		throw std::exception("Sound-> Can only create 1 player");
	++count;
	// start the sound engine with default parameters
	
}


Sound::~Sound()
{
	--count;
	snd->drop();
	engine->removeAllSoundSources();
	engine->drop(); // delete engine
}

void Sound::init()
{
	

	engine = createIrrKlangDevice();
	transform = engine->addSoundSourceFromFile("Sound/transform.mp3");
	item = engine->addSoundSourceFromFile("Sound/item.mp3");
	drive = engine->addSoundSourceFromFile("Sound/Drive.mp3");
	game = engine->addSoundSourceFromFile("Sound/Game.mp3");
	win = engine->addSoundSourceFromFile("Sound/win.mp3");
	main = engine->addSoundSourceFromFile("Sound/Main.mp3");
	selections = engine->addSoundSourceFromFile("Sound/Picking.mp3");
	ableToPlace = engine->addSoundSourceFromFile("Sound/CorrectPlace.mp3");
	unableToPlace = engine->addSoundSourceFromFile("Sound/Wrongplace.mp3");
	music::player.setsoundvol(0.5);

}

void Sound::initdrive()
{
	drive = engine->addSoundSourceFromFile("Sound/Drive.mp3");
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

ISoundSource * Sound::getitemSound()
{
	return item;
}

ISoundSource * Sound::getWinSound()
{
	return win;
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

ISound * Sound::getSound()
{
	return snd;
}

void Sound::setSound(ISoundSource * sound)
{
	if (snd == nullptr) {
		snd = engine->play2D(sound);
	}
	else
	{
		snd->drop();
		snd = engine->play2D(sound);

	}
}

bool Sound::isplaying(ISoundSource* sound)
{
	return engine->isCurrentlyPlaying(sound);
}
