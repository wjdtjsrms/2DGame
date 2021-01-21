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

		class Parent // state�� ������
		{
		public:
			typedef Sequence::Parent GrandParent; // ���� ���� ��ü
			enum SeqID
			{
				SEQ_STAGE_SELECT, // GrandParent ȣ��
				SEQ_TITLE, // GrandParent ȣ��
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