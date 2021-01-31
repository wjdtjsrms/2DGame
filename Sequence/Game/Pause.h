#ifndef INCLUDED_SEQUENCE_GAME_PAUSE_H
#define INCLUDED_SEQUENCE_GAME_PAUSE_H

#include "Sequence\Game\GameChild.h"

class Image;

namespace Sequence
{
	namespace  Game
	{
		class Parent;

		class Pause : public Child
		{
		public:
			Pause();
			~Pause();
			Base* update(Parent*);
		private:
			Image* mImage;
			int mCursor;
		};
	}
}
#endif
