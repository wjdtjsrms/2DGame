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
		BGM_TITLE, // Ÿ��Ʋ ���
		BGM_GAME, // �÷��� ���

		BGM_MAX, // �ִ� ��� ���� ���� 
	};

	enum Se {
		SE_EXPLOSION, // ���� ȿ����
		SE_DEATH, // ���� ȿ����
		SE_CURSOR_MOVE, // Ŀ�� ȿ����
		SE_SELECTION, // ���� ȿ����
		SE_SET_BOMB, // ��ź ��ġ ȿ����

		SE_MAX, // �ִ� ��� ���� ����
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
