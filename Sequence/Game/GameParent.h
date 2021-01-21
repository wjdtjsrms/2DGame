#ifndef INCLUDED_SEQUENCE_GAME_H
#define INCLUDED_SEQUENCE_GAME_H

class State;

namespace Sequence
{
	class Parent;

	namespace Game 
	{
		class Clear;
		class Loading;
		class Menu;
		class Play;

		class Parent // state를 가진다
		{
		public:
			typedef Sequence::Parent GrandParent; // 상위 계층 객체
			enum SeqID
			{
				SEQ_STAGE_SELECT, // GrandParent 호출
				SEQ_TITLE, // GrandParent 호출
				SEQ_CLEAR,
				SEQ_MENU,
				SEQ_PLAY,

				SEQ_NONE
			};
		public:
			void moveTo(SeqID);
			Parent(int stageID);
			~Parent();
			void update(GrandParent*);
			void startLoading();
			State* state();

		private:
			State * mState;
			int mStageID;
			SeqID mNext;

			Clear* mClear;
			Loading* mLoading;
			Menu* mMenu;
			Play* mPlay;

		};

	} // namespace Game
} // namespace Sequence
#endif