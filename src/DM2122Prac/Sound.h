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
	void init();
	void initdrive();
	void playsound(ISoundSource * music, bool looping);
	void playsound(ISoundSource* music);
	void setsoundvol(const float volume);
	ISoundSource* getTransformSound();
	ISoundSource* getDriveSound();
	ISoundSource* getGameSound();
	ISoundSource* getitemSound();
	ISoundSource* getWinSound();
	ISoundSource* getMainSound();
	ISoundSource* getSelectionSound();
	ISoundSource* getCanPlaceSound();
	ISoundSource* getCantPlaceSound();
	ISoundEngine* getEngine();
	ISound * getSound();
	void setSound(ISoundSource* sound);
	bool isplaying(ISoundSource* sound);
private:
	static short count; // Count How many sounds is playing
	ISoundEngine* engine;
	ISoundSource* transform, *drive, *game, *main, *selections, *ableToPlace, *unableToPlace, *item ,* win;
	ISound * snd;
};

namespace music
{
	extern Sound player;
}