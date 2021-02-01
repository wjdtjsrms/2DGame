#ifndef INCLUDED_SOUND_MANAGER_H
#define INCLUDED_SOUND_MANAGER_H

#include "GameLib\Sound\Wave.h"
#include "GameLib\Sound\Player.h"

class SoundManager
{
public:
	static SoundManager* Instance();
	static void create();
	static void destroy();
	bool isReady();

	enum Bgm
	{
		BGM_TITLE, // 타이틀 브금
		BGM_GAME, // 플레이 브금

		BGM_MAX, // 최대 재생 가능 개수 
	};

	enum Se {
		SE_EXPLOSION, // 폭발 효과음
		SE_DEATH, // 죽음 효과음
		SE_CURSOR_MOVE, // 커서 효과음
		SE_SELECTION, // 선택 효과음
		SE_SET_BOMB, // 폭탄 설치 효과음

		SE_MAX, // 최대 재생 가능 개수
	};

	void playBgm(Bgm);
	void stopBgm();
	void playSe(Se);
private:
	SoundManager();
	~SoundManager();

	GameLib::Sound::Wave mBgmWaves[BGM_MAX];
	GameLib::Sound::Wave mSeWaves[SE_MAX];

	GameLib::Sound::Player mBgmPlayer;
	static const int SE_PLAYER_MAX = 4;
	GameLib::Sound::Player mSePlayer[SE_PLAYER_MAX];
	int mSePlayerPos;

	static SoundManager* mInstance;
};
#endif
