#include "SoundManager.h"
#include "GameLib\GameLib.h"
#include <sstream>
using namespace GameLib;

SoundManager* SoundManager::mInstance = 0;

SoundManager* SoundManager::Instance()
{
	return mInstance;
}

void SoundManager::create()
{
	mInstance = new SoundManager();
}

void SoundManager::destroy()
{
	ASSERT(mInstance);
	SAFE_DELETE(mInstance);
}


SoundManager::SoundManager()
	:mSePlayerPos(0)
{
	const char* bgmFiles[] =
	{
		"charara.wav", //BGM_TITLE
		"charara.wav", //BGM_GAME
	};

	const char* seFiles[] =
	{
		"dokaan.wav", //SE_EXPLOSION
		"dokaan.wav", //SE_DEATH
		"dokaan.wav", //SE_CURSOR_MOVE
		"dokaan.wav", //SE_SELECTION
		"dokaan.wav", //SE_SET_BOMB
	};

	std::ostringstream oss;
	for (int i = 0; i < BGM_MAX; ++i)
	{
		oss.str("");
		oss << "data/sound/bgm/" << bgmFiles[i];
		mBgmWaves[i] = Sound::Wave::create(oss.str().c_str());
	}

	for (int i = 0; i < SE_MAX; ++i)
	{
		oss.str("");
		oss << "data/sound/se/" << seFiles[i];
		mSeWaves[i] = Sound::Wave::create(oss.str().c_str());
	}
}

SoundManager::~SoundManager()
{
}

bool SoundManager::isReady()
{
	bool ret = true;
	for (int i = 0; i < BGM_MAX; ++i)
	{
		ret = ret && mBgmWaves[i].isReady();
	}

	for (int i = 0; i < SE_MAX; ++i)
	{
		ret = ret && mSeWaves[i].isReady();
	}

	return ret;
}

void SoundManager::playBgm(Bgm bgm)
{
	mBgmPlayer = Sound::Player::create(mBgmWaves[bgm]);
	mBgmPlayer.play(true); // loop true
}

void SoundManager::stopBgm()
{
	if (mBgmPlayer)
	{
		mBgmPlayer.stop();
	}
}

void SoundManager::playSe(Se se)
{
	mSePlayer[mSePlayerPos] = Sound::Player::create(mSeWaves[se]);
	mSePlayer[mSePlayerPos].play(false);
	++mSePlayerPos;

	if (mSePlayerPos == SE_PLAYER_MAX)
	{
		mSePlayerPos = 0;
	}
}