#ifndef INCLUDED_SEQUENCE_GAME_PAUSE_H
#define INCLUDED_SEQUENCE_GAME_PAUSE_H

class Image;

namespace Sequence
{
	namespace  Game
	{
		class Parent;

		class Pause
		{
		public:
			Pause();
			~Pause();
			void update(Parent*);
		private:
			Image* mImage;
			int mCursor;
		};
	}
}
#endif
