#ifndef INCLUDED_SEQUENCE_GAME_CLEAR_H
#define INCLUDED_SEQUENCE_GAME_CLEAR_H

#include "Sequence\Game\GameChild.h"

class Image;

namespace Sequence
{
	namespace Game
	{
		class Parent;

		class Clear : public Child
		{
		public:
			Clear();
			~Clear();
			Base* update(Parent*);
		private:
			Image* mImage;
			int mCount;
		};

		
	}
}


#endif