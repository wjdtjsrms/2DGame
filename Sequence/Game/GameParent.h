#ifndef INCLUDED_SEQUENCE_GAME_H
#define INCLUDED_SEQUENCE_GAME_H

#include "Sequence/Child.h"
class State;

namespace Sequence
{
	class Parent;

	namespace Game 
	{
		class Child; // Game의 child 이다

		class Parent :public Sequence::Child // Sequence의 child를 상속 받는다.
		{
		public:
			typedef Sequence::Parent GrandParent; // 상위 계층 객체

			enum NextSequence {
				NEXT_ENDING, // GrandParent 호출
				NEXT_GAME_OVER, // GrandParent 호출
				NEXT_TITLE, // GrandParent 호출

				NEXT_NONE,
			};
			enum Mode {
				MODE_1P,
				MODE_2P,
				MODE_NONE,
			};
			enum PlayerID {
				PLAYER_1,
				PLAYER_2,
				PLAYER_NONE,
			};
		public:
			Parent(GrandParent::Mode);
			~Parent();
			Base* update(GrandParent*);
			void moveTo(NextSequence);

			PlayerID winner() const;
			void setWinner(PlayerID);
			State* state();
			void drawState() const;
			bool hasFinalStageCleared() const; // 최종 스테이지 확인
			int lifeNumber() const;
			Mode mode() const;
			void startLoading();
			void reduceLife();
			void goToNextStage();

		private:
			State * mState;
			int mStageID;
			int mLife;
			PlayerID mWinner;
			static const int FINAL_STAGE = 2;
			static const int INITIALI_LIFE_NUMBER = 2;

			NextSequence mNextSequence;
			Game::Child* mChild; // 네임 스페이스로 명시해주어야 제대로 호출한다.
		};

	} // namespace Game
} // namespace Sequence
#endif