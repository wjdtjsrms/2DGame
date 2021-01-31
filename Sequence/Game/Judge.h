#ifndef INCLUDED_SEQUENCE_GAME_JUDGE_H
#define INCLUDED_SEQUENCE_GAME_JUDGE_H

#include "Sequence\Game\GameChild.h"

class Image;

namespace Sequence
{
	namespace Game
	{
		class Parent;

		class Judge : public Child
		{
		public:
			Judge();
			~Judge();
			Base* update(Parent*);
		private:
			Image* mImage;
			int mCursor;

		};
	}
}
#endif 
