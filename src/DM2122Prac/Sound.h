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
	void playsound(ISoundSource * music, bool looping);
	void playsound(ISoundSource* music);
	void setsoundvol(const float volume);
	ISoundSource* getTransformSound();
	ISoundSource* getDriveSound();
	ISoundSource* getGameSound();
	ISoundSource* getMainSound();
	ISoundSource* getSelectionSound();
	ISoundSource* getCanPlaceSound();
	ISoundSource* getCantPlaceSound();
	ISoundEngine* getEngine();
private:
	static short count; // Count How many sounds is playing
	ISoundEngine* engine;
	ISoundSource* transform, *drive, *game, *main, *selections, *ableToPlace, *unableToPlace;
};

namespace music
{
	extern Sound player;
}