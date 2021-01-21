#ifndef INCLUDED_SEQUENCE_GAME_CLEAR_H
#define INCLUDED_SEQUENCE_GAME_CLEAR_H

class Image;

namespace Sequence
{
	namespace Game
	{
		class Parent;

		class Clear
		{
		public:
			Clear();
			~Clear();
			void update(Parent*);
		private:
			Image* mImage;
			int mCount;
		};

		
	}
}


#endif