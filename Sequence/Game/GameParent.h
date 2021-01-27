#ifndef INCLUDED_SEQUENCE_GAME_H
#define INCLUDED_SEQUENCE_GAME_H

class State;

namespace Sequence
{
	class Parent;

	namespace Game 
	{
		class Clear;
		class Ready; // loading ��ü
		class Pause; // menu ��ü
		class Play;
		class Failure;
		class Judge;


		class Parent // state�� ������
		{
		public:
			typedef Sequence::Parent GrandParent; // ���� ���� ��ü

			enum NextSequence {
				NEXT_CLEAR,
				NEXT_READY,
				NEXT_PAUSE,
				NEXT_PLAY,
				NEXT_FAILURE,
				NEXT_JUDGE,
				NEXT_ENDING, // GrandParent ȣ��
				NEXT_GAME_OVER, // GrandParent ȣ��
				NEXT_TITLE, // GrandParent ȣ��

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
			void update(GrandParent*);
			void moveTo(NextSequence);

			PlayerID winner() const;
			void setWinner(PlayerID);
			State* state();
			void drawState() const;
			bool hasFinalStageCleared() const; // ���� �������� Ȯ��
			int lifeNumber() const;
			Mode mode() const;
			void startLoading();
		

		private:
			State * mState;
			int mStageID;
			int mLife;
			PlayerID mWinner;
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