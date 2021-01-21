#ifndef INCLUDED_SEQUENCE_GAME_H
#define INCLUDED_SEQUENCE_GAME_H

class State;

namespace Sequence
{
	class Parent;

	namespace Game 
	{
		class Clear;
		class Ready; // loading 대체
		class Pause; // menu 대체
		class Play;
		class Failure;
		class Judge;


		class Parent // state를 가진다
		{
		public:
			typedef Sequence::Parent GrandParent; // 상위 계층 객체

			enum NextSequence {
				NEXT_CLEAR,
				NEXT_READY,
				NEXT_PAUSE,
				NEXT_PLAY,
				NEXT_FAILURE,
				NEXT_JUDGE,
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

		public:
			Parent();
			~Parent();
			void update(GrandParent*);
			void moveTo(NextSequence);

			State* state();
			bool hasFinalStageCleared() const; // 최종 스테이지 확인
			int lifeNumber() const;
			Mode mode() const;
			void startLoading();
		

		private:
			State * mState;
			int mStageID;
			int mLife;

			static const int FINAL_STAGE = 2;
			static const int INITIALI_LIFE_NUMBER = 2;

			NextSequence mNextSequence;

			Clear* mClear;
			Ready* mReady;
			Pause* mPause;
			Play* mPlay;
			Failure* mFailure;
			Judge* mJudge;


		};

	} // namespace Game
} // namespace Sequence
#endif